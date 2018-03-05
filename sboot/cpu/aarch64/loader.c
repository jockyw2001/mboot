//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    loader.c
/// @brief  Loader code running on RAM
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boot.h"
#include "chip.h"
#include "drvPadConf.h"
#include "bootrom_spi.h"
#if defined (CONFIG_MSTAR_BUILD_TEE_LOADER_DAT) && defined(CONFIG_MSTAR_NS_UBOOT)
#include "ns_uboot_loader.h"
#endif


#ifdef CONFIG_TEE_FASTCALL
#include "fastcall.h"
#elif (defined(CONFIG_2ND_LOADER_ENCRYPTION) || defined(CONFIG_2ND_LOADER_VERIFICATION))
#include "ns_uboot_verify.h"
#endif

extern MS_U8 _ld_APP_load_start[];
extern MS_U8 _boot_flow_mode;

typedef void (*PF_AP_Entry)(void);

#define VERIFICATION_ENABLE 1

#define PADCFG_END        0xFFFF
#define PADCFG_MEM_BASE   0x0000
#define REG_ADDR_BASE     0x1F000000

#define SHA_HASH_DIGI_LEN           8                                   // 32-bit count, 5 for SHA-1, 8 for SHA-256,384,512

#define MHal_RIU_WritReg16(u32reg, u16val ) *((volatile MS_U16*)(MS_U64)(REG_ADDR_BASE+ ((u32reg) << 1))) = (u16val)
#define MHal_RIU_ReadReg16(u32reg) *((volatile MS_U16*)(MS_U64)(REG_ADDR_BASE+ ((u32reg) << 1)))
#define MHal_RIU_WritReg8(u32reg, u16val ) *((volatile MS_U8*)(MS_U64)(REG_ADDR_BASE+ ((u32reg) << 1))) = (u16val)

#define READ_OTP_8(x) (*(volatile MS_U8*)(0x1F320000 + (x)))
#define READ_OTP_16(x) (*(volatile MS_U16*)(0x1F320000 + (x)))

#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
#define __teeloader__ __attribute__((section(".ldr.teeloader")))
#else
#define __teeloader__
#endif

void SHA256_Init(MS_U32 *hash);
void SHA256_Hash(const MS_U32 *data, MS_U32 *hash);
void SHA256_Hash_Message(MS_U8 *pu8Message, MS_U32 u32Length);
void MDrv_SHA256_Calculate(U32 u32SHAInBuf, U32 u32Size, U32 u32SHAOutBuf);

void icache_enable(void);
void icache_disable(void);
void dcache_enable(void);
void dcache_disable(void);

#ifdef CONFIG_MSTAR_VID_ENABLE
void BOOTROM_C_InitVID(void);
#endif

#ifdef CONFIG_MSTAR_DVFS_ENABLE
void BOOTROM_C_InitDVFS(void);
#endif

// SHA256 result
MS_U32   sha_H[SHA_HASH_DIGI_LEN] = {
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
};

// Golden APP SHA256 digest
// Here is the digest of the test.bin for testing
// Will be replaced when append the new APP by 'app_append.py'
MS_U32 AppDigest[SHA_HASH_DIGI_LEN] = {
    0x73F911A1, 0x8C9E84EC, 0xE56D24D1, 0xF01547A4, 0x7B3A0D67, 0x283679F0, 0x9FF9AE14, 0xFFF3D95C
};

// Secure Boot
// EmbeddedMBootRsaPub for Authenticating UBoot and Keybank
// Later the content of EmbeddedMBootRsaPub and EmbeddedMBootAes will be replaced by 'app_append_64bit.py'
MS_U8 EmbeddedMBootRsaPub[260] __attribute__((aligned(16)));
MS_U8 EmbeddedMBootAes[16] __attribute__((aligned(16)));

MS_U64 UBOOT_ENTRY;

//#define PUTC(c) do { *(volatile unsigned short *)0x1F201300 = c; } while(0)
void __teeloader__ PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

void __teeloader__ *memcpy(void *dst, const void *src, size_t n)
{
    char *pdst = (char *)dst;
    const char *psrc = (const char *)src;

    for(; n > 0; ++pdst, ++psrc, --n)
    {
        *pdst = *psrc;
    }

    return (dst);
}

void __teeloader__ *memset (void *s, int c, unsigned int n)
{
    unsigned char *t = s;

    while(n-- > 0)
        *t++ = c;

    return s;
}

int __teeloader__ memcmp(const void * cs,const void * ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}

#define ENABLE_PRNT_IN_LDR 1
#if ENABLE_PRNT_IN_LDR
#define LDR_PUTC(val) PUTC(val)

void __teeloader__ LDR_PUTX(char val)
{
    if (val > 0x9)
        LDR_PUTC(0x41+(val-10));
    else
        LDR_PUTC(0x30+val);
}

void LDR_PUTXX(char val)
{
    LDR_PUTX(val >> 4);
    LDR_PUTX(val & 0x0F);
}

void LDR_PUTARR(MS_U8 *pArr, MS_U32 len)
{
    MS_U32 i;
    for (i=0; i<len; i++)
    {
        if ((i % 16) == 0)
        {
            LDR_PUTS("\n");
            LDR_PUTDW(pArr + i);
            LDR_PUTS(": ");
        }
        LDR_PUTXX(pArr[i]);
        LDR_PUTS(" ");
    }
    LDR_PUTS("\n");
}

void __teeloader__ LDR_PUTW(MS_U16 val)
{
    MS_U8 value;

    value = (val>>12)&0xF;
    LDR_PUTX(value);
    value = (val>>8)&0xF;
    LDR_PUTX(value);
    value = (val>>4)&0xF;
    LDR_PUTX(value);
    value = (val)&0xF;
    LDR_PUTX(value);
}

void __teeloader__ LDR_PUTDW(MS_U32 val)
{
    MS_U8 value;

    value = (val>>28)&0xF;
    LDR_PUTX(value);
    value = (val>>24)&0xF;
    LDR_PUTX(value);
    value = (val>>20)&0xF;
    LDR_PUTX(value);
    value = (val>>16)&0xF;
    LDR_PUTX(value);
    value = (val>>12)&0xF;
    LDR_PUTX(value);
    value = (val>>8)&0xF;
    LDR_PUTX(value);
    value = (val>>4)&0xF;
    LDR_PUTX(value);
    value = (val)&0xF;
    LDR_PUTX(value);
}

void __teeloader__ LDR_PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            LDR_PUTC('\r');
        LDR_PUTC(*s++);
    }
}
#else
#define LDR_PUTC(val)
#define LDR_PUTX(val)
#define LDR_PUTW(val)
#define LDR_PUTDW(val)
#define LDR_PUTS(val)
#endif

unsigned int __teeloader__ current_el(void)
{
    unsigned int el;
    __asm__ __volatile__(
        "mrs %0, CurrentEL" : "=r" (el) : : "cc"
    );
    return el >> 2;
}

void __teeloader__ flush_cache(void)
{
    #if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)
    unsigned int el;
    el = current_el();
    if(el == 2)
    {
        invalidate_tlb_all_el2();
    }
    else if(el == 3)
    {
        invalidate_tlb_all();
    }
    flush_dcache_all();
    #else
    #error "flush_cache not defined"
    #endif
}

void __teeloader__ flush_L2(void)
{
    #if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)
    //does not enable l2 in loader, no need to implement
    #else
    #error "flush_L2 not defined"
    #endif
}

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
#define PMU_WAKEUP_ADDR_REGL     0x1F001D48
#define PMU_WAKEUP_ADDR_REGH     0x1F001D48
#define WAKEUP_ADDR_MASK          0x0000FFF0
#define WAKEUP_FLAG_MASK          0x0000000F
#define WAKEUP_ADDR_SHIFT         16
#define WAKEUP_FLAG_INVALID       0
#define WAKEUP_FLAG_SLEPT         1
#define WAKEUP_FLAG_WKUP          2
#define MSTAR_SLEEP_MAGIC         0x4D535452
#define PMU_WAKEUP_ADDR_LMASK    0x000000FF
#define PMU_WAKEUP_ADDR_HMASK    0x0000FF00

#define M512M (512*1024*1024)
#define _PHY_TO_VIRT(x) ( (((x)>=(CONFIG_MIU0_BUSADDR)&&(x)<(CONFIG_MIU0_BUSADDR+M512M)) \
                            ||((x)>=(CONFIG_MIU1_BUSADDR)&&(x)<(CONFIG_MIU1_BUSADDR+M512M)))? \
                            (x):(0) )

#define PHY_TO_VIRT(x) _PHY_TO_VIRT((unsigned long)(x))

unsigned long sboot_read_wkup_pmu(void)
{
    volatile unsigned long *preg=0;
    unsigned long val=0;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    val = ((*preg)&PMU_WAKEUP_ADDR_LMASK);
    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    val |= ((*preg)&PMU_WAKEUP_ADDR_HMASK);

    return val;
}

void sboot_write_wkup_pmu(unsigned long val)
{
    volatile unsigned long *preg=0;
    unsigned long oldval;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    oldval = ((*preg) & ~PMU_WAKEUP_ADDR_LMASK);
    oldval |= (val&PMU_WAKEUP_ADDR_LMASK);
    (*preg) = oldval;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    oldval = ((*preg) & ~PMU_WAKEUP_ADDR_HMASK);
    oldval |= (val&PMU_WAKEUP_ADDR_HMASK);
    (*preg) = oldval;

}

int _is_str_resume_sboot(void)
{
    unsigned long pmu_str_reg=0,*pwkupsave=0;
    pmu_str_reg=sboot_read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<WAKEUP_ADDR_SHIFT);

    if((pmu_str_reg & WAKEUP_FLAG_MASK) == WAKEUP_FLAG_SLEPT)
    {
       return 1;
    }
    return 0;
}

int sboot_do_check_str_resume(void)
{
    unsigned long pmu_str_reg=0,*pwkupsave=0;
    MS_VIRT _RIU   = 0x1f000000;

    pmu_str_reg=sboot_read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<16);

    if(_is_str_resume_sboot())
    {

        PUTC('\r'); PUTC('\n');
        PUTC('S'); PUTC('B'); PUTC('O'); PUTC('O');PUTC('T');PUTC('_');
        PUTC('R'); PUTC('E'); PUTC('S'); PUTC('U'); PUTC('M'); PUTC('E');
        PUTC('\r'); PUTC('\n');

#ifdef CONFIG_ARMv8_32BIT_KERNEL

        (*(volatile MS_U16*)(_RIU + (0x1012f0<<1))) = 0x0440;
        (*(volatile MS_U16*)(_RIU + (0x1012f2<<1))) = 0x0000;
        (*(volatile MS_U16*)(_RIU + (0x1012f4<<1))) = 0x0040;
        (*(volatile MS_U16*)(_RIU + (0x1012f6<<1))) = 0x0000;
        (*(volatile MS_U16*)(_RIU + (0x1012f8<<1))) = 0x3EFF;

        (*(volatile MS_U16*)(_RIU + (0x1006f0<<1))) = 0x73fe;
        (*(volatile MS_U16*)(_RIU + (0x1006f2<<1))) = 0x667f;
        (*(volatile MS_U16*)(_RIU + (0x1006f4<<1))) = 0x2a9f;
        (*(volatile MS_U16*)(_RIU + (0x1006f6<<1))) = 0x00ac;
        (*(volatile MS_U16*)(_RIU + (0x1006f8<<1))) = 0x4100;
        sboot_write_wkup_pmu((pmu_str_reg&~WAKEUP_FLAG_MASK)|WAKEUP_FLAG_WKUP);

        (*(volatile MS_U16*)(_RIU + (0x11131C<<1))) = 0x0003;
        sboot_str_jump_kernel(pwkupsave);
#else //ARMv8_64bit_Kernel

        //Set MIU0 group priority
        (*(volatile MS_U16*)(_RIU + (0x1012fE<<1))) = 0x80E1;
        (*(volatile MS_U16*)(_RIU + (0x1012fE<<1))) = 0x81E1;
        (*(volatile MS_U16*)(_RIU + (0x1012fE<<1))) = 0x80E1;

        //Set MIU1 group priority
        (*(volatile MS_U16*)(_RIU + (0x1006fE<<1))) = 0x20E1;
        (*(volatile MS_U16*)(_RIU + (0x1006fE<<1))) = 0x21E1;
        (*(volatile MS_U16*)(_RIU + (0x1006fE<<1))) = 0x20E1;

        //Set select MIU0
        (*(volatile MS_U16*)(_RIU + (0x1012f0<<1))) = 0x0440;
        (*(volatile MS_U16*)(_RIU + (0x1012f2<<1))) = 0x0000;
        (*(volatile MS_U16*)(_RIU + (0x1012f4<<1))) = 0x0040;
        (*(volatile MS_U16*)(_RIU + (0x1012f6<<1))) = 0x0000;
        (*(volatile MS_U16*)(_RIU + (0x1012f8<<1))) = 0x3E4C;
        (*(volatile MS_U16*)(_RIU + (0x1012fA<<1))) = 0x000F;

        //Set select MIU1
        (*(volatile MS_U16*)(_RIU + (0x1006f0<<1))) = 0x73FE;
        (*(volatile MS_U16*)(_RIU + (0x1006f2<<1))) = 0x667F;
        (*(volatile MS_U16*)(_RIU + (0x1006f4<<1))) = 0x3ADF;
        (*(volatile MS_U16*)(_RIU + (0x1006f6<<1))) = 0x00AC;
        (*(volatile MS_U16*)(_RIU + (0x1006f8<<1))) = 0x41B2;
        (*(volatile MS_U16*)(_RIU + (0x1006fA<<1))) = 0xFFF0;

        sboot_write_wkup_pmu((pmu_str_reg&~WAKEUP_FLAG_MASK)|WAKEUP_FLAG_WKUP);

        (*(volatile MS_U16*)(_RIU + (0x11131C<<1))) = 0x0003;

        //StrSetGICInterface
        asm volatile (
                "ldr     x3, =0x16001080\n\t"
                "ldr     w2, [x3]\n\t"
                "ldr     w4, =(0x3 << 30)\n\t"
                "orr     w2, w2, w4\n\t"
                "str     w2, [x3]\n\t"

                "ldr     x3, =0x16001000\n\t"
                "mov     w2, #3\n\t"
                "str     w2, [x3]\n\t"

                "ldr     x3, =0x16002000\n\t"
                "mov     w2, #3\n\t"
                "str     w2, [x3]\n\t"

                "ldr     x3, =0x16002004\n\t"
                "mov     w2, #0xf8\n\t"
                "str     w2, [x3]\n\t"

                "mov     x0, #0x73\n\t"
                "msr     ACTLR_EL3, x0\n\t"
                );

        icache_disable();
        dcache_disable();

        /* flush all data from d-cache into dram */
        flush_cache();

        sboot_str_jump_64bit_kernel();
#endif

    }
    else
    {
        sboot_write_wkup_pmu(0);
    }
    return 0;
}
#endif //CONFIG_MSTAR_STR_ENABLE

#ifdef CONFIG_TEE_LOADER
static void __teeloader__ LDR_DelayMS(MS_U32 msec)
#else
void  LDR_DelayMS(MS_U32 msec)
#endif
{
    MS_U64 u64val1;
    MS_U64 u64val2;
    MS_U64 u64Interval;

    u64Interval = 12000*msec;

    asm volatile("mrs %0, cntpct_el0" : "=r" (u64val1));

    while(1)
    {
        asm volatile("mrs %0, cntpct_el0" : "=r" (u64val2));
        if ((u64val2-u64val1) >= u64Interval)
            break;
    }
}

void LDR_PadConfig(void)
{
    U8  u8Length;
    U8* pu8PadConfig = (U8*)(padInitTbl+10);
    U16 u16Index; // register index
    U32 u32MapBase = REG_ADDR_BASE;
    volatile U8* u8Addr;

    while (1)
    {
        u16Index = (U16)(((U16)(*pu8PadConfig)<<8) + *(pu8PadConfig+1));
        switch (u16Index)
        {
            case PADCFG_END: //Check end of table
                return;

            case PADCFG_MEM_BASE: //Switch base address
                u32MapBase = REG_ADDR_BASE + ((U32)(*(pu8PadConfig+2))<<17);
                pu8PadConfig += 4;
                break;

            default: // Write reg
                u8Length = u16Index>>14;
                u16Index &= 0x3FFF;
                pu8PadConfig += 2;
                while (1)
                {
                    U8 u8Mask  = *(pu8PadConfig+1);
                    U8 u8Value = *pu8PadConfig;
                    u8Addr = (U8*)(unsigned long)(u32MapBase + (((U32)(u16Index))<<1) - (u16Index & 1));

                    *u8Addr = (*u8Addr & ~u8Mask) | (u8Value & u8Mask);

                    pu8PadConfig += 2;
                    if (u8Length == 0)
                    {
                        break;
                    }
                    else // Write more than 1 byte
                    {
                        u8Length--;
                        u16Index++;
                    }
                }
                break;
        }
    }
}
//#ifndef bfn

void LDR_IP_Trim(void)
{
    MS_U32 otp = 0;
    MS_U16 data = 0;
    MS_U16 data1 = 0;
#if defined(CONFIG_MSTAR_K6LITE) // k6lite
        // HDMI2TX_ATOP
        otp = (READ_OTP_8(0x3CED) & 0x4) >> 2;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF2) & 0x0F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x173000 << 1) + (0x38 << 2));
            data &= (~0xf0);
            data |= (otp << 4);
            *(volatile MS_U16*)(0x1F000000 + (0x173000 << 1) + (0x38 << 2)) = data;
        }

        // ETH_ATOP
        otp = (READ_OTP_8(0x3CEC) & 0x04) >> 2;
        if (otp)
        {
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2)) |= (0x1 << 2);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) |= (0x1 << 15);

            // ETH_cur_100t
            otp = (READ_OTP_8(0x3CF6) & 0x3e) >> 1;
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

            // ETH_cur_10t
            otp = (READ_OTP_8(0x3CF5) & 0xf0);
            otp |= ((READ_OTP_8(0x3CF6) & 0x01) << 8);
            otp = otp >> 4;
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
            data &= (~0x3e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

            // ETH_imp
            otp = (READ_OTP_8(0x3CF5) & 0x0f);
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
            data &= (~0x3c00);
            data |= (otp << 10);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

            // ETH_lpf
            otp = (READ_OTP_8(0x3CF4) & 0xf0) >> 4;
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2));
            data &= (~0x780);
            data |= (otp << 7);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2)) = data;
        }

        // SMC_ATOP
        otp = (READ_OTP_8(0x3DCC) & 0x4) >> 2;
        if (otp)
        {
            // SMC_ATOP
            otp = (READ_OTP_8(0x3DCC) & 0x03);
            data = *(volatile MS_U16*)(0x1F000000 + (0x102900 << 1) + (0x32 << 2));
            data &= (~0x03);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x102900 << 1) + (0x32 << 2)) = data;

            // sm0_PU_11K_HIGH
            otp = (READ_OTP_8(0x3CFB) & 0x08) >> 3;
            otp = (otp) | (otp << 1) | (otp << 2) | (otp << 3) | (otp << 4);
            data = *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2)) = data;

            // sm0_PU_11K_LOW
            otp = (READ_OTP_8(0x3CFB) & 0x10) >> 4;
            otp = (otp << 5) | (otp << 6) | (otp << 7) | (otp << 8) | (otp << 9);
            data = *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2));
            data &= (~0x3e0);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2)) = data;
        }

        // USB_XCVR_h (P0+P1+SS0)
        otp = (READ_OTP_8(0x3CED) & 0x01);
        if (otp)
        {
            otp = READ_OTP_8(0x3CF3) & 0x0F;

            // port 0
            data = *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x94 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x94 << 2)) = data;

            // port 1
            data = *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x14 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x14 << 2)) = data;;
        }

        // USB_XCVR (P2)
        otp = (READ_OTP_8(0x3CED) & 0x02) >> 1;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF3) & 0xF0) >> 4;

            // ss1
            data = *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x94 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x94 << 2)) = data;
        }

        // SATA_ATOP
        otp = (READ_OTP_8(0x3DCD) & 0x20) >> 5;
        if (otp)
        {
            // SATA_TX_Rterm
            otp = (READ_OTP_8(0x3DCC) & 0xF8) >> 3;
            data = *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2)) = data;

            // SATA_RX_Rterm
            otp = (READ_OTP_8(0x3DCD) & 0x1F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2));
            data &= (~0x1f00);
            data |= (otp << 8);
            *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2)) = data;
        }

        // TSIO_ATOP
        otp = (READ_OTP_8(0x3CF0) & 0x20) >> 5;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF0) & 0x1F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x171f00 << 1) + (0x0f << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x171f00 << 1) + (0x0f << 2)) = data;
        }

#elif defined(CONFIG_MSTAR_K6) // k6

        // HDMI2TX_ATOP
        otp = (READ_OTP_8(0x3CED) & 0x4) >> 2;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF2) & 0x0F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x173000 << 1) + (0x38 << 2));
            data &= (~0xf0);
            data |= (otp << 4);
            *(volatile MS_U16*)(0x1F000000 + (0x173000 << 1) + (0x38 << 2)) = data;
        }

        // ETH_ATOP
        otp = (READ_OTP_8(0x3CEC) & 0x04) >> 2;
        if (otp)
        {
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2)) |= (0x1 << 2);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) |= (0x1 << 15);

            // ETH_cur_100t
            otp = (READ_OTP_8(0x3CF6) & 0x3e) >> 1;
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

            // ETH_cur_10t
            otp = (READ_OTP_8(0x3CF5) & 0xf0);
            otp |= ((READ_OTP_8(0x3CF6) & 0x01) << 8);
            otp = otp >> 4;
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
            data &= (~0x3e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

            // ETH_imp
            otp = (READ_OTP_8(0x3CF5) & 0x0f);
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
            data &= (~0x3c00);
            data |= (otp << 10);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

            // ETH_lpf
            otp = (READ_OTP_8(0x3CF4) & 0xf0) >> 4;
            data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2));
            data &= (~0x780);
            data |= (otp << 7);
            *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2)) = data;
        }

        // USB3_XCVR
        otp = (READ_OTP_8(0x3CF8) & 0x20) >> 5;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF7) & 0xF8) >> 3;
            data = *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x04 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x04 << 2)) = data;

            otp = (READ_OTP_8(0x3CF8) & 0x1F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x08 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x08 << 2)) = data;
        }

        // U3PCIE_ATOP
        otp = (READ_OTP_8(0x3CFA) & 0x1);
        if (otp)
        {
            otp = (READ_OTP_16(0x3CF8) & 0x07C0) >> 6;
            data = *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x04 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x04 << 2)) = data;

            otp = (READ_OTP_8(0x3CF9) & 0xF8) >> 3;
            data = *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x08 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x08 << 2)) = data;
        }

        // SMC_ATOP
        otp = (READ_OTP_8(0x3DCC) & 0x4) >> 2;
        if (otp)
        {
            // SMC_ATOP
            otp = (READ_OTP_8(0x3DCC) & 0x03);
            data = *(volatile MS_U16*)(0x1F000000 + (0x102900 << 1) + (0x32 << 2));
            data &= (~0x03);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x102900 << 1) + (0x32 << 2)) = data;

            // sm0_PU_11K_HIGH
            otp = (READ_OTP_8(0x3CFB) & 0x08) >> 3;
            otp = (otp) | (otp << 1) | (otp << 2) | (otp << 3) | (otp << 4);
            data = *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2)) = data;

            // sm0_PU_11K_LOW
            otp = (READ_OTP_8(0x3CFB) & 0x10) >> 4;
            otp = (otp << 5) | (otp << 6) | (otp << 7) | (otp << 8) | (otp << 9);
            data = *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2));
            data &= (~0x3e0);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x1b << 2)) = data;
        }

        // USB_XCVR_h (P0+P1+SS0)
        otp = (READ_OTP_8(0x3CED) & 0x01);
        if (otp)
        {
            otp = READ_OTP_8(0x3CF3) & 0x0F;

            // port 0
            data = *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x54 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x54 << 2)) = data;

            // port 1
            data = *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x14 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x14 << 2)) = data;;

            // ss0
            data = *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x14 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x14 << 2)) = data;
        }

        // USB_XCVR (SS1)
        otp = (READ_OTP_8(0x3CED) & 0x02) >> 1;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF3) & 0xF0) >> 4;

            // ss1
            data = *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x54 << 2));
            data &= (~0x1e0);
            data |= (otp << 5);
            *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x54 << 2)) = data;
        }

        // SATA_ATOP
        otp = (READ_OTP_8(0x3DCD) & 0x20) >> 5;
        if (otp)
        {
            // SATA_TX_Rterm
            otp = (READ_OTP_8(0x3DCC) & 0xF8) >> 3;
            data = *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2)) = data;

            // SATA_RX_Rterm
            otp = (READ_OTP_8(0x3DCD) & 0x1F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2));
            data &= (~0x1f00);
            data |= (otp << 8);
            *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2)) = data;
        }

        // MHL_COMBO_ATOP
        otp = (READ_OTP_8(0x3CED) & 0x08) >> 3;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF2) & 0xF0) >> 4;
            *(volatile MS_U16*)(0x1F000000 + (0x172800 << 1) + (0x59 << 2)) |= 0x100;
            data = *(volatile MS_U16*)(0x1F000000 + (0x172800 << 1) + (0x6b << 2));
            data &= (~0xf000);
            data |= (otp << 12);
            *(volatile MS_U16*)(0x1F000000 + (0x172800 << 1) + (0x6b << 2)) = data;
        }

        // paddsv_pdus04dgz_arc_rx_sw
        otp = (READ_OTP_8(0x3DCD) & 0x40) >> 6;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF4) & 0x0F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x6e << 2));
            data &= (~0x0f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x101e00 << 1) + (0x6e << 2)) = data;
        }

        // TSIO_ATOP
        otp = (READ_OTP_8(0x3CF0) & 0x20) >> 5;
        if (otp)
        {
            otp = (READ_OTP_8(0x3CF0) & 0x1F);
            data = *(volatile MS_U16*)(0x1F000000 + (0x171f00 << 1) + (0x0f << 2));
            data &= (~0x1f);
            data |= otp;
            *(volatile MS_U16*)(0x1F000000 + (0x171f00 << 1) + (0x0f << 2)) = data;
        }
#else // kano and curry
    #if defined(CONFIG_MSTAR_KANO)
    // MHL_COMBO_ATOP
    otp = (READ_OTP_8(0x3CED) & 0x8) >> 3;
    if (otp)
    {
        otp = (READ_OTP_8(0x3CF2) & 0xF0) >> 4;
        *(volatile MS_U16*)(0x1F000000 + (0x172800 << 1) + (0x59 << 2)) |= 0x100;
        data = *(volatile MS_U16*)(0x1F000000 + (0x172800 << 1) + (0x6b << 2));
        data &= (~0xf000);
        data |= (otp << 12);
        *(volatile MS_U16*)(0x1F000000 + (0x172800 << 1) + (0x6b << 2)) = data;
    }
    #endif

    // HDMI2TX_ATOP
    otp = (READ_OTP_8(0x3CED) & 0x4) >> 2;
    if (otp)
    {
        otp = (READ_OTP_8(0x3CF2) & 0x0F);
        data = *(volatile MS_U16*)(0x1F000000 + (0x173000 << 1) + (0x38 << 2));
        data &= (~0xf0);
        data |= (otp << 4);
        *(volatile MS_U16*)(0x1F000000 + (0x173000 << 1) + (0x38 << 2)) = data;
    }

    #if defined(CONFIG_MSTAR_KANO)
    // USB3_XCVR
    otp = (READ_OTP_8(0x3CFA) & 0x1);
    if (otp)
    {
        otp = (READ_OTP_8(0x3CF7) & 0xF8) >> 3;
        data = *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x04 << 2));
        data &= (~0x1f);
        data |= otp;
        *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x04 << 2)) = data;

        otp = (READ_OTP_8(0x3CF8) & 0x1F);
        data = *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x08 << 2));
        data &= (~0x1f);
        data |= otp;
        *(volatile MS_U16*)(0x1F000000 + (0x102100 << 1) + (0x08 << 2)) = data;
    }

    // U3PCIE_ATOP
    otp = (READ_OTP_8(0x3CF8) & 0x20) >> 5;
    if (otp)
    {
        otp = (READ_OTP_16(0x3CF8) & 0x07C0) >> 6;
        data = *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x04 << 2));
        data &= (~0x1f);
        data |= otp;
        *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x04 << 2)) = data;

        otp = (READ_OTP_8(0x3CF9) & 0xF8) >> 3;
        data = *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x08 << 2));
        data &= (~0x1f);
        data |= otp;
        *(volatile MS_U16*)(0x1F000000 + (0x102300 << 1) + (0x08 << 2)) = data;
    }
    #endif

    // USB_XCVR
    otp = (READ_OTP_8(0x3CED) & 0x02) >> 1;
    if (otp)
    {
        otp = READ_OTP_8(0x3CF3) & 0x0f;

        // port 0
        data = *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x54 << 2));
        data &= (~0x1e0);
        data |= (otp << 5);
        *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x54 << 2)) = data;

        // port 1
        data = *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x14 << 2));
        data &= (~0x1e0);
        data |= (otp << 5);
        *(volatile MS_U16*)(0x1F000000 + (0x103a00 << 1) + (0x14 << 2)) = data;;

        #if defined(CONFIG_MSTAR_KANO)
        // port ssc0
        data = *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x14 << 2));
        data &= (~0x1e0);
        data |= (otp << 5);
        *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x14 << 2)) = data;
        #endif
    }

    // USB_XCVR (_h)
    otp = (READ_OTP_8(0x3CED) & 0x01);
    if (otp)
    {
        otp = (READ_OTP_8(0x3CF3) & 0xf0) >> 4;
        // port 2
        data = *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2));
        data &= (~0x1e0);
        data |= (otp << 5);
        *(volatile MS_U16*)(0x1F000000 + (0x103900 << 1) + (0x14 << 2)) = data;

        #if defined(CONFIG_MSTAR_KANO)
        // port ssc1
        data = *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x54 << 2));
        data &= (~0x1e0);
        data |= (otp << 5);
        *(volatile MS_U16*)(0x1F000000 + (0x103800 << 1) + (0x54 << 2)) = data;
        #endif
    }

    // ETH_ATOP
    otp = (READ_OTP_8(0x3CEC) & 0x04) >> 2;
    if (otp)
    {
        *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2)) |= (0x1 << 2);
        *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) |= (0x1 << 15);

        // ETH_cur_100t
        otp = (READ_OTP_8(0x3CF6) & 0x3e) >> 1;
        data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
        data &= (~0x1f);
        data |= otp;
        *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

        // ETH_cur_10t
        otp = (READ_OTP_8(0x3CF5) & 0xf0);
        otp |= ((READ_OTP_8(0x3CF6) & 0x01) << 8);
        otp = otp >> 4;
        data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
        data &= (~0x3e0);
        data |= (otp << 5);
        *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

        // ETH_imp
        otp = (READ_OTP_8(0x3CF5) & 0x0f);
        data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2));
        data &= (~0x3c00);
        data |= (otp << 10);
        *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x34 << 2)) = data;

        // ETH_lpf
        otp = (READ_OTP_8(0x3CF4) & 0xf0) >> 4;
        data = *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2));
        data &= (~0x780);
        data |= (otp << 7);
        *(volatile MS_U16*)(0x1F000000 + (0x003300 << 1) + (0x30 << 2)) = data;
    }
#endif
}

MS_U32 LDR_APP_Auth(MS_U8 *u8From, MS_U32 u32Size)
{
    PUTC('_');
#if defined(VERIFICATION_ENABLE) && VERIFICATION_ENABLE
    int i;
    SHA256_Hash_Message(u8From, u32Size);
    for(i = 0; i < SHA_HASH_DIGI_LEN; i++)
    {
        if(sha_H[i] != AppDigest[i])
            return 0;
    }

#endif

    return 1;
}


extern void core1_loop(void);

inline MS_U32 rotrFixed(MS_U32 x, unsigned int y)
{
    return((x>>y) | (x<<(sizeof(MS_U32)*8-y)));
}

static MS_U32 SHA256_K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define blk0(i)                     (W[i] = data[i])
#define blk2(i)                     (W[i&15] += s1(W[(i-2)&15])+W[(i-7)&15]+s0(W[(i-15)&15]))

#define Ch(x,y,z)                   (z^(x&(y^z)))
#define Maj(x,y,z)                  ((x&y)|(z&(x|y)))

#define a(i)                        T[(0-i)&7]
#define b(i)                        T[(1-i)&7]
#define c(i)                        T[(2-i)&7]
#define d(i)                        T[(3-i)&7]
#define e(i)                        T[(4-i)&7]
#define f(i)                        T[(5-i)&7]
#define g(i)                        T[(6-i)&7]
#define h(i)                        T[(7-i)&7]

// for SHA256
#define S0(x)                       (rotrFixed(x,2)^rotrFixed(x,13)^rotrFixed(x,22))
#define S1(x)                       (rotrFixed(x,6)^rotrFixed(x,11)^rotrFixed(x,25))
#define s0(x)                       (rotrFixed(x,7)^rotrFixed(x,18)^(x>>3))
#define s1(x)                       (rotrFixed(x,17)^rotrFixed(x,19)^(x>>10))

#define R(i)                        h(i) += S1(e(i))+Ch(e(i),f(i),g(i))+SHA256_K[i+j]+(j?blk2(i):blk0(i));  \
                                    d(i) += h(i);                                                           \
                                    h(i) += S0(a(i)) + Maj(a(i),b(i),c(i))

void SHA256_Init(MS_U32 *hash)
{
    hash[0] = 0x6a09e667;
    hash[1] = 0xbb67ae85;
    hash[2] = 0x3c6ef372;
    hash[3] = 0xa54ff53a;
    hash[4] = 0x510e527f;
    hash[5] = 0x9b05688c;
    hash[6] = 0x1f83d9ab;
    hash[7] = 0x5be0cd19;
}


void SHA256_Hash(const MS_U32 *data, MS_U32 *hash)
{
    MS_U32                 W[16];
    MS_U32                 T[8];
    int                 j;

    // Initialize state and local variable
    for (j = 0; j < 16; j++)
    {
        W[j] = 0;
    }
    for (j = 0; j < 8; j++)
    {
        T[j] = hash[j];
    }

    // Interative Hash Computation
    for (j = 0; j < 64; j += 16)
    {
        /* R( 0)
        h(0) += S1(e(0))+Ch(e(0),f(0),g(0))+SHA256_K[0+j]+(j?blk2(0):blk0(0));
        d(0) += h(0);
        h(0) += S0(a(0)) + Maj(a(0),b(0),c(0));
        */
        R( 0); R( 1); R( 2); R( 3);
        R( 4); R( 5); R( 6); R( 7);
        R( 8); R( 9); R(10); R(11);
        R(12); R(13); R(14); R(15);
    }

    // Update new hash value
    hash[0] += a(0);
    hash[1] += b(0);
    hash[2] += c(0);
    hash[3] += d(0);
    hash[4] += e(0);
    hash[5] += f(0);
    hash[6] += g(0);
    hash[7] += h(0);
}

void SHA256_Hash_Message(MS_U8 *pu8Message, MS_U32 u32Length)
{
    MS_U32              sha_M[SHA_HASH_DIGI_LEN * 2]; // temp buffer for message block

    int                 len, shift, i;
    unsigned char       msg[4];
    MS_U64              msg_addr;
    MS_U32              tmp;

    SHA256_Init(sha_H);

    for(i = 0; i < sizeof(sha_M) / sizeof(sha_M[0]); i++)
        sha_M[i] = 0;

    msg_addr = (MS_U64)(pu8Message);
    len = (int)(u32Length);

    while (len >= 0)
    {
        if (len < 64) // last 1 data block
        {
            // move n*32 bits data
            for (i = 0; i < (len&~3);)
            {
                *(MS_U32*)msg = *(MS_U32*)(msg_addr);
                msg_addr += 4;

                sha_M[i/4] = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
                i += 4;
            }

            // move non-32 bits data
            *(MS_U32*)msg = *(MS_U32*)(msg_addr);
            msg_addr += 4;

            // append and stuffing
            sha_M[i/4] = 0;
            while (i < len)
            {
                sha_M[i/4] |= msg[i%4] << (24-((i%4)*8));
                i++;
            }
            // 1 append
            sha_M[i/4] |= 0x80 << (24-((i%4)*8));
            i = (i + 4) & ~3;


            // If (len * 8 + 1(1 append) + 64(64-bit msg length)) > 512,
            // it's not enough for padding 8-byte(64-bit) length;
            // namely, if len=56~63.
            if (len >= 56)
            {
                // No room for message length

                // 0 append
                for (; i < 64;)
                {
                    sha_M[i/4] = 0;
                    i += 4;
                }
                SHA256_Hash(sha_M, sha_H);
                // reset to next block
                i = 0;
            }
            // 0 append
            for (; i < 56;)
            {
                sha_M[i/4] = 0;
                i += 4;
            }

            sha_M[14] = 0;
            //sha_M[15] = (*(U32*)_Rom_Public.p_message_length) * 8; // bit count
            sha_M[15] = u32Length << 3; // bit count
        }
        else
        {
            // move n*32 bits data
            for (i = 0; i < 64;)
            {
                *(MS_U32*)msg = *(MS_U32*)(msg_addr);
                msg_addr += 4;

                sha_M[i/4] = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
                i += 4;
            }
        }
        SHA256_Hash(sha_M, sha_H);
        len -= 64;
    }
    // Hash revert to digit
    for (i = 0; i < SHA_HASH_DIGI_LEN/2; i++)
    {
        tmp = sha_H[SHA_HASH_DIGI_LEN-1-i];
        sha_H[SHA_HASH_DIGI_LEN-1-i] = sha_H[i];
        sha_H[i] = tmp;
    }
}

#include "bootrom_spi.h"
static MS_U32 _LDR_SERFLASH_WaitReadDataRdy(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR);

    do
    {
        if ((ISP_READ(REG_ISP_SPI_RD_DATARDY) & ISP_SPI_RD_DATARDY_MASK) == ISP_SPI_RD_DATARDY )
        {
            bRet = 1;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    return bRet;
}

static MS_U32 _LDR_SERFLASH_WaitWriteDone(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*5);

    do
    {
        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if (_LDR_SERFLASH_WaitReadDataRdy() == 0 )
        {
            break;
        }

        if ((ISP_READ(REG_ISP_SPI_RDATA) & SF_SR_WIP_MASK) == 0 ) // WIP = 0 write done
        {
            bRet = 1;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    return bRet;
}

MS_U32 LDR_SERFLASH_ReadID(void)
{

#ifdef CONFIG_LD_BOOT_FROM_OTP
    //_PUTS("ID=0\n");
    //return 0;

    // HW doesn't support ReadID on MX/ST flash; use trigger mode instead.
    MS_U32 bRet = 0;
    MS_U32 u32I;
    MS_U32 u32Size;
    MS_U8 u8ptr ;

    ISP_WRITE(REG_ISP_PASSWORD, 0xAAAA);//_HAL_ISP_Enable();

    if (!_LDR_SERFLASH_WaitWriteDone())
    {
        goto LDR_SERFLASH_ReadID_return;
    }
    bRet = 1;

LDR_SERFLASH_ReadID_return:
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);//_HAL_ISP_Disable();

    //_HAL_SPI_Rest(); // to prevent BDMA fail
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);

    return bRet;

#elif CONFIG_LD_BOOT_OTP_SIMULATE_ON_SPINOR
    return 0;
#else
    return 1;
#endif
}

int DoUnCompression(unsigned int addr)
{
    //ISzAlloc g_Alloc = { SzAlloc, SzFree };
    //SRes res;
    int useFilter;
    //SizeT inSizePure;
    //ELzmaStatus status;
    unsigned int u32ROM_START = addr + 0x100;                  // This address should be 4' alignment
    unsigned int u32ROM_END = addr + *((unsigned int *)(unsigned long)(addr + 0x8));   // This address should be 4' alignment
    unsigned int u32RAM_START, u32RAM_END,i =0;
    unsigned int CompressedFileLength = 0;
    unsigned int OriginalFileLength = 0;
    //unsigned char header[LZMA_PROPS_SIZE + 8];
    /* Original Param */
    //Byte *dest = NULL;
    unsigned int destLen = 0;
    //const Byte *src = NULL;
    unsigned int srcLen = 0;
    unsigned int dst = *(unsigned int*)(unsigned long)addr;

    //info_mem =(unsigned int *)(0xA0880030);

    u32RAM_START = *((unsigned int *)((unsigned long)addr)) + 0x100;
    u32RAM_END = u32RAM_START+ *((unsigned int *)(unsigned long)(addr + 0xC));

    CompressedFileLength = (u32ROM_END - u32ROM_START);
    OriginalFileLength = (u32RAM_END - u32RAM_START);

    //Copy compressed UBoot from SPI flash to RAM
    for(i = 0; i < ((CompressedFileLength + 7 + 0x100) & ~7UL); i += 4)
    {
        *((unsigned int *)(unsigned long)(dst + i)) = *((unsigned int *)(unsigned long)((u32ROM_START - 0x100) + i));
        // Copy from Addr (Storage to DRAM)
    }

    if(!LDR_APP_Auth((unsigned char *)(unsigned long)dst, *(unsigned int *)(unsigned long)(addr + 0x8)))
    {
        PUTC('!');
        return -1;
    }

#ifndef CONFIG_MSTAR_NS_UBOOT_BRINGUP
#ifdef CONFIG_TEE_FASTCALL
    // 2nd loader verification & decryption
    {
        MS_U32 len = CompressedFileLength - sizeof(_SUB_SECURE_INFO);
        MS_U32 addr = dst + 0x100;

#ifdef CONFIG_2ND_LOADER_VERIFICATION_BY_FASTCALL
        // verify
        if (fastcall_verify_using_REEkebank(addr, len, ((_SUB_SECURE_INFO*)(addr + len))->u8Signature) == FALSE)
        {
            LDR_PUTS("Fastcall verify 2nd loader FAIL!");
            return -1;
        }
#endif

#ifdef CONFIG_2ND_LOADER_DECRYPTION_BY_FASTCALL
#if defined (CONFIG_CHIP_GENERIC_DRM)
        LDR_PUTS("GENERIC_DRM Delay IN\n");
        LDR_DelayMS(12*1000);
        LDR_PUTS("GENERIC_DRM Delay OUT\n");
#endif
        fastcall_decrypt_using_REEkeybank(addr, len);
#endif
    }
#endif
#endif

    return 0;
}

void LDR_Entry_SPI(void)
{
    MS_U32              *src, *dst, *end;
    PF_AP_Entry         pfEntry;

    //Note: src, dst, end should be 16-byte aligned.
    src = (MS_U32*)_ld_APP_load_start;
    dst = (MS_U32*)(MS_U64)(*(src));
    end = (MS_U32*)((MS_U64)src + *(src+3));
    pfEntry = (PF_AP_Entry)((MS_U64)dst + *(src+1));

    UBOOT_ENTRY = (MS_U64)pfEntry;

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    LDR_PadConfig();
#endif

#ifdef CONFIG_MSTAR_VID_ENABLE
    BOOTROM_C_InitVID();
#endif

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    BOOTROM_C_InitDVFS();
#endif

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)
    LDR_IP_Trim();
#endif
#endif

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
    sboot_do_check_str_resume();
#endif

    icache_enable();
    dcache_enable();
    if (1)
    {

#if defined(CONFIG_LDR_LZMA_COMPRESS)

        PUTC('d');PUTC('e');PUTC('c');PUTC('o');PUTC('m');PUTC('p');PUTC('\r');PUTC('\n');
        extern int DoLZMADecompression(U32 addr);
        MS_U64 u64ptr = (MS_U64)_ld_APP_load_start;
        if (DoLZMADecompression((MS_U32)(u64ptr)) < 0)
        {
            while(1);
        }

        PUTC('d');PUTC('o');PUTC('n');PUTC('e');PUTC('\r');PUTC('\n');
#elif defined(CONFIG_LDR_NOCOMPRESS)
        PUTC('a');PUTC('u');PUTC('t');PUTC('h');PUTC('\r');PUTC('\n');
        if (DoUnCompression((U32)(_ld_APP_load_start)) < 0)
        {
            while(1);
        }
        PUTC('o');PUTC('k');
#else
#error "Error"
#endif

    dcache_disable();
    icache_disable();

    JUMP_UBOOT();

    }
}

#if defined(CONFIG_MSTAR_BFN_EN)||defined(CONFIG_MSTAR_BFE_EN)|| (defined(CONFIG_MSTAR_NS_UBOOT) && defined(CONFIG_NAND_FLASH))
#define BINIDTAG    (0x42494E30)      // 'BIN0'
static MS_U8 pu8_RamBuf[1024*8]   __attribute__((aligned(16)));
#endif

#if defined(CONFIG_MSTAR_BFN_EN)|| (defined(CONFIG_MSTAR_NS_UBOOT) && defined(CONFIG_NAND_FLASH) && ( !defined(CONFIG_TEE_LOADER) || ( defined(CONFIG_TEE_LOADER) && defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT))))
#include "drvNAND.h"


NAND_DRIVER NandDrv;


static MS_U8 pu8_SpareBuf[2048]   __attribute__((aligned(16)));

PAIRED_PAGE_MAP_t ga_tPairedPageMap[512];

void * __teeloader__ ldr_memset(void * s,char c,int count)
{
    char *xs = (char *) s;

    while (count--)
        *xs++ = c;

    return s;
}




static MS_U32 __teeloader__ _NAND_ReadPage(MS_U8 *u8_buf, MS_U8 *u8_SpareBuf, MS_U32 pageNum)
{
    MS_U32 ret = 0;


    ret = NC_ReadPages(&NandDrv, pageNum, u8_buf, u8_SpareBuf, 1);

	if(ret != 0)
	{
		WREG_DBG(DBG_E_LDR_NAND_READ_FAIL, 1);
		LDR_PUTS("Nand read fail\n");
		//LDR_PUTDW(pageNum);LDR_PUTC(' ');
		//LDR_PUTDW(ret);LDR_PUTC(' ');
	}

    return ret;
}

static MS_U32 _NAND_IsGoodBlk(MS_U8 *buf, MS_U16 u16_PBA)
{
	MS_U32 u32_Err;
	MS_U32 i;
	MS_U32 u32_PageIdx;


	// old way checking
	for(i=0; i<4; i++)
	{
		u32_PageIdx = u16_PBA << NandDrv.u8_BlkPageCntBits;

	    switch(i)
	    {
			case 0:
			    break;
			case 1: u32_PageIdx++;
			    break;
			case 2: u32_PageIdx += NandDrv.u16_BlkPageCnt-1;
			    break;
			case 3: u32_PageIdx += NandDrv.u16_BlkPageCnt-2;
			    break;
	    }
	    u32_Err = _NAND_ReadPage(buf, pu8_SpareBuf, u32_PageIdx);

	    if (u32_Err != UNFD_ST_SUCCESS)
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "_NAND_ReadPage(0x%X)=0x%X\n", (unsigned int)u32_PageIdx, (unsigned int)u32_Err);

	    if (pu8_SpareBuf[0] != 0xFF)
		    return 0; // bad blk
	}

	return 1; // good blk
}

static MS_U32 _NAND_Read_One_Blk(MS_U8 *u8_buf, MS_U16 u16_PBA, MS_U32 u32_SectorCnt)
{
    MS_U32 u32_Err = UNFD_ST_SUCCESS,i;
	MS_U32 u32_Row =0;
	MS_U16 u16_PageCnt =0;
	MS_U16 u16_page_idx=0;
	MS_U16 u16_i;
	MS_U8  *u8_s = (MS_U8*)pu8_SpareBuf;


    if(u32_SectorCnt >= NandDrv.u16_PageSectorCnt)
    {
        u16_PageCnt = u32_SectorCnt >> NandDrv.u8_PageSectorCntBits;
        for(u16_i=0; u16_i< u16_PageCnt;u16_i++)
        {
		    u32_Row = u16_PBA << NandDrv.u8_BlkPageCntBits;
		    u32_Row += ga_tPairedPageMap[u16_page_idx].u16_LSB;

		    u32_Err = _NAND_ReadPage(u8_buf, u8_s, u32_Row);

		    if(u32_Err)
			    return u32_Err;

		    u8_buf += NandDrv.u16_PageByteCnt;
		    u16_page_idx++;
        }
		u32_SectorCnt -= (u16_PageCnt << NandDrv.u8_PageSectorCntBits);
	}

    if(u32_SectorCnt)
    {
	    u32_Row = u16_PBA << NandDrv.u8_BlkPageCntBits;
		u32_Row += ga_tPairedPageMap[u16_page_idx].u16_LSB;


		for(i=0;i< u32_SectorCnt;i++)
        {
		    u32_Err = NC_ReadSector(&NandDrv, u32_Row, i,
				    u8_buf, u8_s, 0);

            u8_buf += NandDrv.u16_SectorByteCnt;
            u8_s += NandDrv.u16_SectorSpareByteCnt;
 		    if(u32_Err)
			    return u32_Err;
        }
    }

    return u32_Err;

}


MS_U32 __teeloader__ BOOT_NAND_1st(void)
{
    MS_U32 cnt =0, i, max_redundancy = 256;
    MS_U32 ret = 0, u32_PageIdx=0;
    //NAND_FLASH_INFO_t *pNandInfo;

    //dcache_disable();
    //icache_disable();

    WREG_DBG(DBG_F_LDR_NAND_1ST, 1);

    ldr_memset((void *) &NandDrv, 0, sizeof(NandDrv));

    #if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)
    nand_pads_init(&NandDrv, NAND_MODE1);
    #else
    //nand_pads_init(&NandDrv, NAND_MODE3);
    #error "nand_pads_init not called"
    #endif

    ret = nand_unfd_init(&NandDrv);
    if(ret != 0)
    {
        LDR_PUTS("Nand init failed\n");
        WREG_DBG(DBG_E_LDR_INIT_FAIL, 2);
    }

    for(cnt = 0; cnt < max_redundancy; cnt++)
    {
        if(NandDrv.u8_AddrCycleIdx == 2)
        {
            NandDrv.u8_OpCode_RW_AdrCycle = ADR_C4TFS0;
            NandDrv.u8_OpCode_Erase_AdrCycle = ADR_C2TRS0;
        }

        LABEL_TRY_005A30:
        //read page 0
        u32_PageIdx = cnt*NandDrv.u16_BlkPageCnt;
 		ret = NC_ReadSector(&NandDrv,
                u32_PageIdx,
                0,
                pu8_RamBuf,
                pu8_SpareBuf,
                1);
        LDR_PUTS(".");
        //LDR_PUTX(cnt);
        //LDR_PUTC(':');
        //LDR_PUTX(pu8_RamBuf[0]);LDR_PUTC('\n');LDR_PUTC('\r');

        if(ret != 0){
            if(2==NandDrv.u8_AddrCycleIdx && ADR_C5TFS0!=NandDrv.u8_OpCode_RW_AdrCycle)
            {
                //nand_debug(0,1,"switch to 005A30, 0 \n");
                NandDrv.u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
                NandDrv.u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
                goto LABEL_TRY_005A30;
            }
            LDR_PUTS("NAND read fail,continue\n");
            continue;
        }

        if(memcmp((const void *)(pu8_RamBuf), "MSTARSEMIUNFDCIS", 16))
        {
            if(2==NandDrv.u8_AddrCycleIdx && ADR_C5TFS0!=NandDrv.u8_OpCode_RW_AdrCycle)
            {
                //nand_debug(0,1,"switch to 005A30, 0 \n");
                NandDrv.u8_OpCode_RW_AdrCycle = ADR_C5TFS0;
                NandDrv.u8_OpCode_Erase_AdrCycle = ADR_C3TRS0;
                goto LABEL_TRY_005A30;
            }
            continue;
        }
		LDR_PUTS("Hit CIS\n");

		ret = nand_set_param (&NandDrv, pu8_RamBuf);
		if(ret)
		{
		    LDR_PUTS("NAND set param fail\n");
            return ret;
		}


        if(NandDrv.u8_CellType == 0)
        {
	        for(i=0;i< 512;i++)
	        {
		        ga_tPairedPageMap[i].u16_LSB = i;
	        }
        }
        else
        {
	        ret = _NAND_ReadPage(pu8_RamBuf, pu8_SpareBuf, u32_PageIdx+NandDrv.u8_PairPageMapLoc);
		    if(ret != 0)
		    {
			    LDR_PUTS("Read Pair page table fail\n");
                while(1);
		    }

	        for(i=0;i< 512;i++)
	        {
		        ga_tPairedPageMap[i].u16_LSB = (pu8_RamBuf[i*4+1]<<8)+pu8_RamBuf[i*4];
	        }
        }
    #ifdef CONFIG_MSTAR_NS_UBOOT
        // read partition in CIS
        ret = _NAND_ReadPage(pu8_RamBuf, pu8_SpareBuf, ga_tPairedPageMap[u32_PageIdx+1].u16_LSB);
		if(ret != 0)
		{
			LDR_PUTS("Read Partition page table fail\n");
			while(1);
		}
        //set part info
        nand_set_partinfo (&NandDrv, pu8_RamBuf);
        #endif
		break;
    }

    if(cnt == max_redundancy)
    {
        LDR_PUTS("Hit border 0\n");
        WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
        while(1){}
    }
}

PF_AP_Entry BOOT_NAND_2nd(MS_U64* U64ld_APP_load_start)
{
    MS_U32 BINSize = 0, ret = 0;
    MS_U64 MIUPos = 0, BIN_Entry = 0;
    MS_U32 blkNum = 2; //blk#0#1 should be cis or loader
    MS_U64 END_TAG_ADDR = 0;
	MS_U16 u16_BlkPageCnt=0, u16_BlkSectorCnt=0;
	MS_U32 u32_SectorCnt =0, u32_SectorCntRead=0;

	u16_BlkPageCnt = (NandDrv.u8_CellType)?NandDrv.u16_BlkPageCnt>>1:NandDrv.u16_BlkPageCnt;
	u16_BlkSectorCnt = u16_BlkPageCnt << NandDrv.u8_PageSectorCntBits;

    for(; blkNum < 256; blkNum++ )
    {
        if(_NAND_IsGoodBlk(pu8_RamBuf,blkNum)==0)
		    continue;
        //read page 0 for each block to find image tag
        ret =_NAND_ReadPage(pu8_RamBuf, pu8_SpareBuf, blkNum*NandDrv.u16_BlkPageCnt);

        //LDR_PUTW(blkNum);
        //LDR_PUTC(':');
        //LDR_PUTX(pu8_RamBuf[0]); LDR_PUTX(pu8_RamBuf[1]); LDR_PUTX(pu8_RamBuf[2]); LDR_PUTX(pu8_RamBuf[3]);LDR_PUTC('\n');LDR_PUTC('\r');

        if (ret != UNFD_ST_SUCCESS)
           break;

        //find APP tag
        if(memcmp((const void *)(pu8_RamBuf), "BIN0", 4) == 0)
        {
            LDR_PUTS("Hit BIN\n");
            WREG_DBG(DBG_F_LDR_HITAPP, 1);
            MIUPos = *((MS_U32*)(pu8_RamBuf+4));
            *U64ld_APP_load_start = MIUPos;
            BINSize = *((MS_U32*)(pu8_RamBuf+12));
            BIN_Entry = *((MS_U32*)(pu8_RamBuf+8));
            END_TAG_ADDR = MIUPos + BINSize - 4;

            //LDR_PUTDW(MIUPos);LDR_PUTC(' ');
            //LDR_PUTDW(BINSize);LDR_PUTC(' ');
            //LDR_PUTDW(BIN_Entry);LDR_PUTC(' ');
            //LDR_PUTDW(END_TAG_ADDR);LDR_PUTC(' ');

            BIN_Entry = MIUPos + BIN_Entry;
        }
		else
            continue;
        u32_SectorCnt = (BINSize + NandDrv.u16_SectorByteCnt-1) >> NandDrv.u8_SectorByteCntBits;
        //load binary
        while (u32_SectorCnt)
        {
            //LDR_PUTS("\n blkNum: ");LDR_PUTW(blkNum);
            //LDR_PUTS("\n MIUPos ");LDR_PUTDW(MIUPos);
            //LDR_PUTS("\n BINSize: ");LDR_PUTDW(BINSize);

            //error bonding check
            if(blkNum == 256){
                LDR_PUTS("Hit border 2\n");
                WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
                while(1){}
            }
			if(_NAND_IsGoodBlk(pu8_RamBuf,blkNum)==0)
				continue;

			if(u32_SectorCnt >= u16_BlkSectorCnt)
				u32_SectorCntRead = u16_BlkSectorCnt;
			else
				u32_SectorCntRead = u32_SectorCnt;

            ret = _NAND_Read_One_Blk((MS_U8 *)MIUPos, blkNum, u32_SectorCntRead);
            //LDR_PUTS("\n *MIUPos ");LDR_PUTDW(*(MS_U32*)MIUPos);LDR_PUTS("\n");
            if(ret != UNFD_ST_SUCCESS)
            {
                blkNum++;
                LDR_PUTS("\n _NAND_Read_One_Blk failed \n");;
                continue;
            }

            MIUPos += u32_SectorCntRead<<NandDrv.u8_SectorByteCntBits;
            u32_SectorCnt -= u32_SectorCntRead;
            blkNum ++;
        }

        if (memcmp((const void *)(END_TAG_ADDR), "BINE", 4)==0)
        {
            return (PF_AP_Entry)BIN_Entry;
            //LDR_PUTS("check BINE ok\n ");
            //LDR_PUTW(blkNum);
        }
        else
        {
             //LDR_PUTDW(END_TAG_ADDR);
            LDR_PUTS("\ncheck BINE failed\n ");
            //LDR_PUTDW(*(MS_U64*)END_TAG_ADDR);
            WREG_DBG(DBG_E_LDR_LOAD_APP_FAIL, 2);
        }
    }

    //error bonding
    LDR_PUTS("Hit border 3\n");
    WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
    while(1){}

    return 0;

}


void LDR_Entry_NAND(void)
{
    MS_U32              *src, *dst, *end;
    PF_AP_Entry         pfEntry;
    MS_U64 MIUPos = 0, BIN_Entry = 0;
    MS_U64 U64ld_APP_load_start = 0;

    PUTC('N'); PUTC('A'); PUTC('N'); PUTC('D'); PUTC('\n'); PUTC('\r');
    icache_enable();
    dcache_enable();

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
    sboot_do_check_str_resume();
#endif

    BOOT_NAND_1st(); //read CIS info for nand driver

    pfEntry = BOOT_NAND_2nd(&U64ld_APP_load_start); //load APP bin to DRAM

    UBOOT_ENTRY = (MS_U64)pfEntry;

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    LDR_PadConfig();
#endif

#ifdef CONFIG_MSTAR_VID_ENABLE
    BOOTROM_C_InitVID();
#endif

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    BOOTROM_C_InitDVFS();
#endif

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)
    LDR_IP_Trim();
#endif
#endif

    if (1)
    {

#if defined(CONFIG_LDR_LZMA_COMPRESS)
    PUTC('d');PUTC('e');PUTC('c');PUTC('o');PUTC('m');PUTC('p');PUTC('\r');PUTC('\n');
    //extern int DoLZMADecompression(U32 addr);
    if (DoLZMADecompression((U32)(U64ld_APP_load_start)) < 0)
    {
        while(1);
    }
    PUTC('d');PUTC('o');PUTC('n');PUTC('e');PUTC('\r');PUTC('\n');
#endif

    dcache_disable();
    icache_disable();

    JUMP_UBOOT();

    }
}
#endif

#if defined(CONFIG_MSTAR_BFE_EN)
#define BFE_SEARCH_BLK_START (0x1000) //uboot mmc driver reserved(2MB)
#include "eMMC_err_codes.h"
#include "eMMC.h"

void LDR_Entry_EMMC(void)
{
    MS_U64 U64ld_APP_load_start = 0;
    MS_U64 U64MIUPos            = 0;
    MS_U64 BIN_Entry            = 0;
    MS_U32 U32Bin_size = 0;
    MS_U32 U32Blk_idx           = BFE_SEARCH_BLK_START;
    PF_AP_Entry         pfEntry;

    LDR_PUTS("EMMC\n");

    icache_enable();
    dcache_enable();

    //emmc init
    if(eMMC_Init()!=eMMC_ST_SUCCESS)
    {
        LDR_PUTS("emmc init failed\n");
    }

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
    sboot_do_check_str_resume();
#endif

    //find APP BIN page

    if(eMMC_CMD17(U32Blk_idx,(MS_U8*)pu8_RamBuf)!=eMMC_ST_SUCCESS)
    {
        LDR_PUTS("eMMC_CMD17 failed\n");
    }

    //LDR_PUTDW(*(MS_U32*)pu8_RamBuf);LDR_PUTC('\n');LDR_PUTC('\r');
    //LDR_PUTDW(*(MS_U32*)(pu8_RamBuf+4));LDR_PUTC('\n');LDR_PUTC('\r');
    //LDR_PUTDW(*(MS_U32*)(pu8_RamBuf+8));LDR_PUTC('\n');LDR_PUTC('\r');
    //LDR_PUTDW(*(MS_U32*)(pu8_RamBuf+12));LDR_PUTC('\n');LDR_PUTC('\r');

    U64MIUPos            = *(MS_U32*)pu8_RamBuf;
    U64ld_APP_load_start = U64MIUPos;
    U32Bin_size          = *(MS_U32*)(pu8_RamBuf+8);
    UBOOT_ENTRY          = U64MIUPos + *(MS_U32*)(pu8_RamBuf+4) ;

    if(eMMC_CMD18(U32Blk_idx,(MS_U8*)U64MIUPos,(U32Bin_size+511)/512)!=eMMC_ST_SUCCESS)
    {
        LDR_PUTS("eMMC_CMD18 failed\n");
    }

    #if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    LDR_PadConfig();
    #endif

#ifdef CONFIG_MSTAR_VID_ENABLE
    BOOTROM_C_InitVID();
#endif

#ifdef CONFIG_MSTAR_DVFS_ENABLE
    BOOTROM_C_InitDVFS();
#endif

    #if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    #if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)
    LDR_IP_Trim();
    #endif
    #endif

if (1)
    {
#if defined(CONFIG_UBOOT_AS_2ED_LOADER)
    // 2nd loader verification & decryption
    {
#if (defined(CONFIG_2ND_LOADER_ENCRYPTION) || defined(CONFIG_2ND_LOADER_VERIFICATION))
        MS_U32 len = *(unsigned int*)(UBOOT_ENTRY - 0x100 + 0x8) - *(unsigned int*)(UBOOT_ENTRY - 0x100 + 0x4) - sizeof(_SUB_SECURE_INFO);
        MS_U32 addr = UBOOT_ENTRY;
#endif
        // verify
#ifdef CONFIG_2ND_LOADER_VERIFICATION
    #ifdef CONFIG_2ND_LOADER_VERIFICATION_BY_FASTCALL
        if (fastcall_verify_using_REEkebank(addr, len, ((_SUB_SECURE_INFO*)(addr + len))->u8Signature) == FALSE)
        {
            LDR_PUTS("Fastcall verify 2nd loader FAIL!");
            return -1;
        }
    #else
        // Old version of security boot
        if (uboot_Verify(addr, len) == FALSE)
        {
            LDR_PUTS("uboot verify failed!");
            while(1);
        }
        LDR_PUTS("auth ok\n");
    #endif
#endif


#ifdef CONFIG_2ND_LOADER_ENCRYPTION

    #ifdef CONFIG_2ND_LOADER_DECRYPTION_BY_FASTCALL
    #if defined (CONFIG_CHIP_GENERIC_DRM)
        LDR_PUTS("GENERIC_DRM Delay IN\n");
        LDR_DelayMS(12*1000);
        LDR_PUTS("GENERIC_DRM Delay OUT\n");
    #endif
        fastcall_decrypt_using_REEkeybank(addr, len);
    #else
        // Old version of security boot
        if (uboot_Decrypt(BA2PA(addr), len, EmbeddedMBootAes) == FALSE)
        {
            LDR_PUTS("decrypt failed !\n");
            while(1);
        }
        LDR_PUTS("decrypt ok\n");
    #endif
#endif

    }
#endif

#if defined(CONFIG_LDR_LZMA_COMPRESS)
    PUTC('d');PUTC('e');PUTC('c');PUTC('o');PUTC('m');PUTC('p');PUTC('\r');PUTC('\n');
    //extern int DoLZMADecompression(U32 addr);
    if (DoLZMADecompression((U32)(U64ld_APP_load_start)) < 0)
    {
        while(1);
    }
    PUTC('d');PUTC('o');PUTC('n');PUTC('e');PUTC('\r');PUTC('\n');
#endif

    dcache_disable();
    icache_disable();

    JUMP_UBOOT();

    }

}
#endif


/*
MS_U32 LDR_check_emmc_exist(MS_U32 addr, MS_U32 size)
{
    MS_U32 ret =0;
    void LDR_check_emmc_exist_asm(void);
    asm volatile(
        "push  {r8,r9,r10,r11,r12}\n\t"
        "mov   r9, lr\n\t"
        "mov   r8, %[adr]\n\t"
        "mov   r10, %[sz]\n\t"
        "bl    LDR_check_emmc_exist_asm\n\t"
        "nop\n\t"
        "mov   %[rt], r8\n\t"
        "mov   lr, r9\n\t"
        "pop  {r8,r9,r10,r11,r12}\n\t"
        :: [adr] "r" (addr), [sz] "r" (size), [rt] "r" (ret) :
        "memory");
    return ret;
}
*/
/*====================================================================
   LDR Start Entry
======================================================================*/
#ifndef CONFIG_TEE_LOADER
extern void LDR_Entry(void)  _SECTION_LDR_ENTRY_;
#endif
extern MS_U32 Bootram_SecondaryCore[];

#if defined(CONFIG_MSTAR_KANO)
void __teeloader__ InitUTMIPLL (void)
{
    MS_U8 u8TempData = 0;

    //=======================================================================================================
    // Enable UTMI PLL clock
    //=======================================================================================================
    MHal_RIU_WritReg16(0x100800, 0x00c0); //enable UPLL0 clock
    MHal_RIU_WritReg8(0x10080E, 0x01); //enable 384MHz clk from UPLL0
    MHal_RIU_WritReg16(0x100804, 0x0110); //UPLL0
    //================================================================================================

    //initial UPLL1 & UTMISS0 & UTMISS1
    MHal_RIU_WritReg16(0x100820, 0x00c0); //enable UPLL1 clock
    MHal_RIU_WritReg16(0x100824, 0x0110); //UPLL1

    MHal_RIU_WritReg16(0x103A88, 0x040F); // UTMI0
    MHal_RIU_WritReg16(0x103A80, 0x7F05);
    MHal_RIU_WritReg16(0x103A08, 0x040F); // UTMI1
    MHal_RIU_WritReg16(0x103A00, 0x7F05);
    MHal_RIU_WritReg16(0x103908, 0x040F); // UTMI2
    MHal_RIU_WritReg16(0x103900, 0x7F05);
    MHal_RIU_WritReg16(0x103808, 0x040F); // UTMI-SS0
    MHal_RIU_WritReg16(0x103800, 0x7F05);
    MHal_RIU_WritReg16(0x103888, 0x040F); // UTMI-SS1
    MHal_RIU_WritReg16(0x103880, 0x7F05);

    // initial USBPLL ===================================================================================
    //Module: usb0 (0x100700)
    MHal_RIU_WritReg8(0x100700, 0x0a); //Disable MAC initial suspend, Reset UHC
    MHal_RIU_WritReg8(0x100700, 0x28); //Release UHC reset, enable UHC and OTG XIU function

    //Module: utmi0 (0x103A80)
    MHal_RIU_WritReg16(0x103AA2, 0x2088); //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    MHal_RIU_WritReg16(0x103AA0, 0x8051); //PLL_TEST[15]: Bypass 480MHz clock divider
    MHal_RIU_WritReg16(0x103A82, 0x2084); //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MHal_RIU_WritReg16(0x103A88, 0x0426); //bit<7>: Power down UTMI port-0 bandgap current

    MHal_RIU_WritReg16(0x103A80, 0x6bc3); //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103A80, 0x69c3); //Turn on UPLL, reg_pdn: bit<9>
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103A80, 0x0001); //Turn all (including hs_current) use override mode

    //================================================================================================

    //Module: usb1 (0x100780)
    MHal_RIU_WritReg8(0x100780, 0x0a); //Disable MAC initial suspend, Reset UHC
    MHal_RIU_WritReg8(0x100780, 0x28); //Release UHC reset, enable UHC and OTG XIU function

    //Module: utmi1 (0x103A00)
    MHal_RIU_WritReg16(0x103A22, 0x2088); //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    MHal_RIU_WritReg16(0x103A20, 0x8051); //PLL_TEST[15]: Bypass 480MHz clock divider
    MHal_RIU_WritReg16(0x103A02, 0x2084); //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MHal_RIU_WritReg16(0x103A08, 0x0426); //bit<7>: Power down UTMI port-0 bandgap current

    MHal_RIU_WritReg16(0x103A00, 0x6bc3); //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103A00, 0x69c3); //Turn on UPLL, reg_pdn: bit<9>
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103A00, 0x0001); //Turn all (including hs_current) use override mode

    //================================================================================================

    //Module: usb2 (0x113800)
    MHal_RIU_WritReg8(0x110200, 0x0a); //Disable MAC initial suspend, Reset UHC
    MHal_RIU_WritReg8(0x110200, 0x28); //Release UHC reset, enable UHC and OTG XIU function

    //Module: utmi2 (0x103900)
    MHal_RIU_WritReg16(0x103922, 0x2088); //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    MHal_RIU_WritReg16(0x103920, 0x8051); //PLL_TEST[15]: Bypass 480MHz clock divider
    MHal_RIU_WritReg16(0x103902, 0x2084); //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MHal_RIU_WritReg16(0x103908, 0x0426); //bit<7>: Power down UTMI port-0 bandgap current

    MHal_RIU_WritReg16(0x103900, 0x6bc3); //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103900, 0x69c3); //Turn on UPLL, reg_pdn: bit<9>
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103900, 0x0001); //Turn all (including hs_current) use override mode

    //================================================================================================

    //Module: usb3-ss0 (0x153200)
    MHal_RIU_WritReg8(0x140600, 0x0a); //Disable MAC initial suspend, Reset UHC
    MHal_RIU_WritReg8(0x140600, 0x28); //Release UHC reset, enable UHC and OTG XIU function

    //Module: utmi-ss0 (0x122300)
    MHal_RIU_WritReg16(0x103822, 0x2088); //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    MHal_RIU_WritReg16(0x103820, 0x8051); //PLL_TEST[15]: Bypass 480MHz clock divider
    MHal_RIU_WritReg16(0x103802, 0x2084); //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MHal_RIU_WritReg16(0x103808, 0x0426); //bit<7>: Power down UTMI port-0 bandgap current

    MHal_RIU_WritReg16(0x103800, 0x6bc3); //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103800, 0x69c3); //Turn on UPLL, reg_pdn: bit<9>
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103800, 0x0001); //Turn all (including hs_current) use override mode

    //================================================================================================

    //Module: usb3-ss1 (0x153280)
    MHal_RIU_WritReg8(0x140680, 0x0a); //Disable MAC initial suspend, Reset UHC
    MHal_RIU_WritReg8(0x140680, 0x28); //Release UHC reset, enable UHC and OTG XIU function

    //Module: utmi-ss1 (0x122380)
    MHal_RIU_WritReg16(0x1038A2, 0x2088); //PLL_TEST[30:28]: 3'b101 for IBIAS current select
    MHal_RIU_WritReg16(0x1038A0, 0x8051); //PLL_TEST[15]: Bypass 480MHz clock divider
    MHal_RIU_WritReg16(0x103882, 0x2084); //Enable CLK12_SEL bit <2> for select low voltage crystal clock
    MHal_RIU_WritReg16(0x103888, 0x0426); //bit<7>: Power down UTMI port-0 bandgap current

    MHal_RIU_WritReg16(0x103880, 0x6bc3); //reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103880, 0x69c3); //Turn on UPLL, reg_pdn: bit<9>
    LDR_DelayMS(1);

    MHal_RIU_WritReg16(0x103880, 0x0001); //Turn all (including hs_current) use override mode

    // exclusive for KANO USB3/PCIE combo
    // USB3 Phy SS port 0 init
    MHal_RIU_WritReg16(0x102106, 0x0104);
    MHal_RIU_WritReg16(0x102100, 0x0000);
    MHal_RIU_WritReg16(0x102102, 0x0000);
    MHal_RIU_WritReg16(0x10213A, 0x0000);

    MHal_RIU_WritReg16(0x102018, 0x0160);
    MHal_RIU_WritReg16(0x102020, 0x0000);
    MHal_RIU_WritReg16(0x102022, 0x0000);

    MHal_RIU_WritReg16(0x10213A, 0x0308);

    u8TempData = *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x210e<<1)+1));
    u8TempData &= (~0x44);
    *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x210e<<1)+1)) = u8TempData;

    MHal_RIU_WritReg16(0x1020C0, 0x9374);
    MHal_RIU_WritReg16(0x1020C2, 0x0018);
    MHal_RIU_WritReg16(0x1020C4, 0x7002);
    MHal_RIU_WritReg16(0x1020C6, 0x04D8);

    MHal_RIU_WritReg16(0x102160, 0x3932);
    MHal_RIU_WritReg16(0x102162, 0x3939);
    MHal_RIU_WritReg16(0x102164, 0x3932);
    MHal_RIU_WritReg16(0x102166, 0x3939);
    MHal_RIU_WritReg16(0x102112, 0x0400);

    *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x1021A0 << 1) + 1)) = 0x0;
    MHal_RIU_WritReg8(0x102012, 0xF4);

    u8TempData = *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x210e<<1)+1));
    u8TempData &= (~0x4);
    *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x210e<<1)+1)) = u8TempData;

    // USB3 Phy SS port 1 init
    MHal_RIU_WritReg16(0x102306, 0x0104);
    MHal_RIU_WritReg16(0x102300, 0x0000);
    MHal_RIU_WritReg16(0x102302, 0x0000);
    MHal_RIU_WritReg16(0x10233A, 0x0000);

    MHal_RIU_WritReg16(0x102218, 0x0160);
    MHal_RIU_WritReg16(0x102220, 0x0000);
    MHal_RIU_WritReg16(0x102222, 0x0000);

    MHal_RIU_WritReg16(0x10233A, 0x0308);

    u8TempData = *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x2302<<1)+1));
    u8TempData &= (~0x44);
    *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x2302<<1)+1)) = u8TempData;

    MHal_RIU_WritReg16(0x1022C0, 0x9374);
    MHal_RIU_WritReg16(0x1022C2, 0x0018);
    MHal_RIU_WritReg16(0x1022C4, 0x7002);
    MHal_RIU_WritReg16(0x1022C6, 0x04D8);

    MHal_RIU_WritReg16(0x102360, 0x3932);
    MHal_RIU_WritReg16(0x102362, 0x3939);
    MHal_RIU_WritReg16(0x102364, 0x3932);
    MHal_RIU_WritReg16(0x102366, 0x3939);
    MHal_RIU_WritReg16(0x102312, 0x0400);

    *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x1023A0 << 1) + 1)) = 0x0;
    MHal_RIU_WritReg8(0x102212, 0xF4);

    u8TempData = *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x230e<<1)+1));
    u8TempData &= (~0x4);
    *(volatile MS_U8 *)(MS_U64)(0x1f200000 + ((0x230e<<1)+1)) = u8TempData;

}
#endif

#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6)
void InitUSB30GPIO(void)
{
    MS_U16 u16Temp;

    *(volatile MS_U16*)(0x1F000000 + (REG_USB30_GPIO_CONTROL << 1)) = REG_USB30_GPIO_BANK;

    u16Temp = *(volatile MS_U16*)(0x1F000000 + ((REG_USB30_GPIO_CONTROL+2) << 1));
    u16Temp |= (REG_USB30_GPIO_BIT << 8);
    u16Temp |= REG_USB30_GPIO_OFFSET;
    u16Temp |= (REG_USB30_GPIO_LOW_ACTIVE << 11);
    *(volatile MS_U16*)(0x1F000000 + ((REG_USB30_GPIO_CONTROL+2) << 1)) = u16Temp;

#if defined(ENABLE_USB30_GPIO_PORT2)
    *(volatile MS_U16*)(0x1F000000 + (REG_USB30_PORT2_GPIO_CONTROL << 1)) = REG_USB30_GPIO_BANK;

    u16Temp = *(volatile MS_U16*)(0x1F000000 + ((REG_USB30_PORT2_GPIO_CONTROL+2) << 1));
    u16Temp |= (REG_USB30_PORT2_GPIO_BIT << 8);
    u16Temp |= REG_USB30_PORT2_GPIO_OFFSET;
    u16Temp |= (REG_USB30_PORT2_GPIO_LOW_ACTIVE << 11);
    *(volatile MS_U16*)(0x1F000000 + ((REG_USB30_PORT2_GPIO_CONTROL+2) << 1)) = u16Temp;
#endif
}
#endif

#if defined(CONFIG_MSTAR_K6) // different project might has different fcie/sdio bank address

#define    SD_SIGNATURE                0x3800

#define    FCIE_OPT                    ( (FCIE_CD_OPT<<8) | (FCIE_PC_OPT<<10) | (FCIE_WP_OPT<<12) )
#define SDIO_OPT                    ( (SDIO_CD_OPT<<8) | (SDIO_PC_OPT<<10) | (SDIO_WP_OPT<<12) )

#if (defined ENABLE_FCIE_GPIO_CONTROL) && ENABLE_FCIE_GPIO_CONTROL

void InitFCIEGPIO(void)
{
    MS_U16 u16RegValue;

    // signature
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x14<<2) ) = SD_SIGNATURE;

    // CD, PC, WP mode
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x15<<2) ) = FCIE_OPT;

    // enable access for below register
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x0C<<2) ) = 0x0080;

    // GPIO card detection
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x23<<2) ) = FCIE_CD_BANK;
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x24<<2) ) = (FCIE_CD_OFFSET<<8)|(FCIE_CD_BIT&0xFF);

    // GPIO power control
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x25<<2) ) = FCIE_PC_BANK;
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x26<<2) ) = (FCIE_PC_OFFSET<<8)|(FCIE_PC_BIT&0xFF);

    // GPIO write protection
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x27<<2) ) = FCIE_WP_BANK;
    u16RegValue = *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x28<<2) );
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x28<<2) ) = (u16RegValue&0xFF00)|(FCIE_WP_BIT&0xFF);
    u16RegValue = *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x0E<<2) );
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x0E<<2) ) = (u16RegValue&0x00FF)|(FCIE_WP_OFFSET<<8);

    // disable access for upper register
    *(volatile MS_U16*)( 0x1F000000 + (0x111300<<1) + (0x0C<<2) ) = 0x0000;

}

#endif

#if (defined ENABLE_SDIO_GPIO_CONTROL) && ENABLE_SDIO_GPIO_CONTROL

void InitSDIOGPIO(void)
{
    MS_U16 u16RegValue;

    // signature
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x14<<2) ) = SD_SIGNATURE;

    // CD, PC, WP mode
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x15<<2) ) = SDIO_OPT;

    // enable access for below register
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x0C<<2) ) = 0x0080;

    // GPIO card detection
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x23<<2) ) = SDIO_CD_BANK;
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x24<<2) ) = (SDIO_CD_OFFSET<<8)|(SDIO_CD_BIT&0xFF);

    // GPIO power control
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x25<<2) ) = SDIO_PC_BANK;
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x26<<2) ) = (SDIO_PC_OFFSET<<8)|(SDIO_PC_BIT&0xFF);

    // GPIO write protection
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x27<<2) ) = SDIO_WP_BANK;
    u16RegValue = *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x28<<2) );
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x28<<2) ) = (u16RegValue&0xFF00)|(SDIO_WP_BIT&0xFF);
    u16RegValue = *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x0E<<2) );
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x0E<<2) ) = (u16RegValue&0x00FF)|(SDIO_WP_OFFSET<<8);

    // disable access for upper register
    *(volatile MS_U16*)( 0x1F000000 + (0x111F00<<1) + (0x0C<<2) ) = 0x0000;
}

#endif

#endif


#ifdef CONFIG_MSTAR_VID_ENABLE    //Initial Voltage Identification
#if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)

#if defined(CONFIG_MSTAR_KANO)
    #define VID_SIDD_CRITERIA 0x64 // 50mA * 2
    #define VID_OSC_CRITERIA 0x117 // 8.74 MHz * 32
    #define CPU_CLOCK 1200
#elif defined(CONFIG_MSTAR_CURRY)
    #define CPU_CLOCK 800
#elif defined(CONFIG_MSTAR_K6)
    #define VID_SIDD_CRITERIA 0x48 // 36.273mA * 2
    #define VID_OSC_CRITERIA 0x133 // 9.606 MHz * 32
    #define CPU_CLOCK 1300
#endif

void BOOTROM_C_InitVID(void)
{
#if defined(CONFIG_MSTAR_KANO)
    MS_U32 u32otp = *(volatile MS_U32*)0x1f323cec;
    MS_U32 u32SIDD = ( u32otp >> 12 ) & 0x3FF;
    u32otp = *(volatile MS_U32*)0x1f323cf8;
    MS_U32 u32OSC = ( u32otp >> 17 ) & 0x1FF;
#elif defined(CONFIG_MSTAR_CURRY)
    //CPU
    MS_U32 u32otp = *(volatile MS_U32*)0x1f323cec;
    MS_U32 u32SIDD_CPU = ( u32otp >> 12 ) & 0x3FF;
    u32otp = *(volatile MS_U32*)0x1f323cf8;
    MS_U32 u32OSC_CPU = ( u32otp >> 17 ) & 0x3FF;

    //CORE
    u32otp = *(volatile MS_U32*)0x1f323cec;
    MS_U32 u32SIDD_CORE = ( u32otp >> 22 ) & 0x3FF;
    u32otp = *(volatile MS_U32*)0x1f323cf0;
    MS_U32 OSC_CORE_temp1 = ( u32otp >> 0 ) & 0x3F;
    u32otp = *(volatile MS_U32*)0x1f323cf8;
    MS_U32 OSC_CORE_temp2 = ( u32otp >> 27 ) & 0x1F;

    MS_U32 u32OSC_CORE = ((OSC_CORE_temp1) | (OSC_CORE_temp2 << 6));
#elif defined(CONFIG_MSTAR_K6)
    MS_U32 u32otp = *(volatile MS_U32*)0x1f323cec;
    MS_U32 u32SIDD = ( u32otp >> 12 ) & 0x3FF;
    u32otp = *(volatile MS_U32*)0x1f323cf8;
    MS_U32 u32OSC = ( u32otp >> 17 ) & 0x3FF;
#endif

    MS_U16 u16temp = 0;

    // Set Specific Label of VID
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x00 << 1)) = 0x2300;

    // Set Initial Value of CPU Clock Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x02 << 1)) = CPU_CLOCK;

    // Set Initial Value of CPU Temperature Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x04 << 1)) = 0x00FF;

    //Set Initial Value of Power Control Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = 0x0801;

    //Set Initial Value of Temperature Control Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x08 << 1)) = 0xFF00;

    //Set Initial Value of Temperature Control Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0A << 1)) = 0xFFFF;

    //Set Initial Value of Reserved Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0C << 1)) = 0x0000;

    //Set Initial Value of Special Clock Register
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0E << 1)) = 0x2454;

#if defined(CONFIG_MSTAR_KANO)
    if(u32SIDD <= VID_SIDD_CRITERIA || u32OSC <= VID_OSC_CRITERIA)
    {
        // Set CPU Power Type for SS Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 4);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set Core Power Type for SS Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 6);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set VID GPIO to SS core power
        CpuCorePWRHigh();
    }
    else
    {
        // Set CPU Power Type for TT/FF Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 4);
        u16temp |= (0x2 << 4);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set Core Power Type for TT/FF Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 6);
        u16temp |= (0x2 << 6);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set VID GPIO to TT/FF core power
        CpuCorePWRLow();
    }
#elif defined(CONFIG_MSTAR_CURRY)
    if(u32SIDD_CPU <= VID_CPU_SIDD_CRITERIA || u32OSC_CPU <= VID_CPU_OSC_CRITERIA)
    {
        // Set CPU Power Type for SS Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 4);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;
    }
    else
    {
        // Set CPU Power Type for TT/FF Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 4);
        u16temp |= (0x2 << 4);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;
    }

    if(u32SIDD_CORE <= VID_CORE_SIDD_CRITERIA || u32OSC_CORE <= VID_CORE_OSC_CRITERIA)
    {
        // Set Core Power Type for SS Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 6);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;
    }
    else
    {
        // Set Core Power Type for TT/FF Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 6);
        u16temp |= (0x2 << 6);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;
    }
#elif defined(CONFIG_MSTAR_K6)
    if(u32SIDD <= VID_SIDD_CRITERIA || u32OSC <= VID_OSC_CRITERIA)
    {
        // Set CPU Power Type for SS Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 4);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set Core Power Type for SS Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 6);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set VID GPIO to SS core power
        CpuCorePWRHigh();
    }
    else
    {
        // Set CPU Power Type for TT/FF Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 4);
        u16temp |= (0x2 << 4);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set Core Power Type for TT/FF Corner Chip
        u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
        u16temp &= ~(0x3 << 6);
        u16temp |= (0x2 << 6);
        *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

        // Set VID GPIO to TT/FF core power
        CpuCorePWRLow();
    }
#endif


    //Set VID Control Type for CPU Power : Use SWI2C
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
    u16temp &= ~(0x3 << 8);
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

    //Set VID Control Type for Core Power : Use SWI2C
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
    u16temp &= ~(0x3 << 10);
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

}
#endif
#endif  /*CONFIG_MSTAR_VID_ENABLE*/

#ifdef CONFIG_MSTAR_DVFS_ENABLE
#if defined(CONFIG_MSTAR_KANO)||defined(CONFIG_MSTAR_CURRY)||defined(CONFIG_MSTAR_K6)

#if defined(CONFIG_MSTAR_KANO)
#define CONFIG_MSTAR_DVFS_TEMPERATURE_LOWER_BOUND 80
#define CONFIG_MSTAR_DVFS_TEMPERATURE_UPPER_BOUND 100
#define CONFIG_MSTAR_DVFS_TEMPERATURE_THRESHOLD 120
#elif defined(CONFIG_MSTAR_K6)
#define CONFIG_MSTAR_DVFS_TEMPERATURE_LOWER_BOUND 80
#define CONFIG_MSTAR_DVFS_TEMPERATURE_UPPER_BOUND 100
#define CONFIG_MSTAR_DVFS_TEMPERATURE_THRESHOLD 120
#endif

void BOOTROM_C_InitDVFS(void)
{
    MS_U16 u16temp = 0;
    MS_U32 u32temp = 0;

    // Set Specific Label of DVFS
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x00 << 1)) = 0x3697;

    // Enable DVFS
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
    u16temp |= (0x1 << 1);
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;

#if defined(CONFIG_MSTAR_CURRY)
    u16temp = (*(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1))) & ~0xc;
    u16temp |= (CONFIG_CHIP_PACKAGE << 2);
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;
#else
    // Set Chip Package : BGA
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1));
    u16temp &= ~0xc;
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x06 << 1)) = u16temp;
#endif

    // Enable Lower/Uppper/Maximum Temperature Control
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x08 << 1));
    u16temp &= ~0x3;
    u16temp |= 0x3;
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x08 << 1)) = u16temp;

    // Set Lower Level Threshold of Temperature
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x08 << 1));
    u16temp &= ~(0xff << 8);
    u16temp |= CONFIG_MSTAR_DVFS_TEMPERATURE_LOWER_BOUND << 8;
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x08 << 1)) = u16temp;

    // Set Upper Level Threshold of Temperature
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0a << 1));
    u16temp &= ~(0xff);
    u16temp |= CONFIG_MSTAR_DVFS_TEMPERATURE_UPPER_BOUND;
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0a << 1)) = u16temp;

    // Set Maximum Level Threshold of Temperature
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0a << 1));
    u16temp &= ~(0xff << 8);
    u16temp |= CONFIG_MSTAR_DVFS_TEMPERATURE_THRESHOLD << 8;
    *(volatile MS_U16*)(0x1f000000 + (0x100500 << 1) + (0x0a << 1)) = u16temp;

    // Select external t-sensor
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x000e00 << 1) + (0x32 << 1));
    u16temp |= 0x8000;
    *(volatile MS_U16*)(0x1f000000 + (0x000e00 << 1) + (0x32 << 1)) = u16temp;

    // Enable t-sensor
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x000e00 << 1) + (0x34 << 1));
    u16temp |= 0x100;
    *(volatile MS_U16*)(0x1f000000 + (0x000e00 << 1) + (0x34 << 1)) = u16temp;

    // Set init clock to lpf
    u32temp = (0xD8000000UL / CPU_CLOCK);
    *(volatile MS_U16*)(0x1f000000 + (0x110a00 << 1) + (0xa0 << 1)) = (u32temp & 0xffff);
    *(volatile MS_U16*)(0x1f000000 + (0x110a00 << 1) + (0xa2 << 1)) = ((u32temp >> 16) & 0xffff);

#if defined(CONFIG_MSTAR_CURRY)
    // Disable CPU voltage high/low boundary protection
    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x000f00 << 1) + (0xfc << 1));
    u16temp |= 0x0003;
    *(volatile MS_U16*)(0x1f000000 + (0x000f00 << 1) + (0xfc << 1)) = u16temp;

    u16temp = *(volatile MS_U16*)(0x1f000000 + (0x000e00 << 1) + (0xc4 << 1));
    u16temp |= 0x0003;
    *(volatile MS_U16*)(0x1f000000 + (0x000e00 << 1) + (0xc4 << 1)) = u16temp;
#endif

}

#endif
#endif

#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
#define USB_LOADER_MEM_ADDR 0x20080000
#define USB_UBOOT_MEM_ADDR (USB_LOADER_MEM_ADDR + 0x20000) //need to alignment 128 kbytes
#define USB_UBOOT_MEM_SIZE 0x300000

void LDR_Entry_USB(void)
{
    LDR_PUTS("\nUSB\n");

    UBOOT_ENTRY = CONFIG_UBOOT_LOADADDR;
    memcpy( (void *)UBOOT_ENTRY, (const void *)USB_UBOOT_MEM_ADDR, USB_UBOOT_MEM_SIZE);

    LDR_PUTS("\nUBOOT_ENTRY = 0x");
    LDR_PUTDW(UBOOT_ENTRY);
    LDR_PUTS("\nmemcpy OK\n");
    JUMP_UBOOT();
}
#endif

extern MS_U32 mstar_secondary_reset[];

/*The main goal of this function is to set cpu wakeup address register(bank:0x101E register:0x6c,0x6d).
   When powering on cpuX, the cpuX will excute mstar_secondary_reset() first.
*/
void __teeloader__ LDR_CPU_Reset_Entry(void)
{

    unsigned short val;
    MS_U64 u64SecondaryCore_Entry = (MS_U64)mstar_secondary_reset;
    u64SecondaryCore_Entry = u64SecondaryCore_Entry >> 10;

    *(volatile MS_U16*)(0x1f000000 + (0x101E00 << 1) + (0x6c << 2)) = u64SecondaryCore_Entry &0xffff;

     val =  *(volatile MS_U16*)(0x1f000000 + (0x101E00 << 1) + (0x6d << 2));
     val = val & 0xffc0;
     val |= (u64SecondaryCore_Entry>>16)  & 0xff;
     val |= (1 << 15);
     *(volatile MS_U16*)(0x1f000000 + (0x101E00 << 1) + (0x6d << 2)) = val;

}
#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6)
#define POWER_FLAG_AC 0x12
#define DC_FLAG_REGISTER (0x1F001CDC + 1)
#define AC_FLAG_REGISTER (0x1F000C1C + 1)
void LDR_Adjust_CPU_Frequency(void)
{
    MS_U32 dwRegisterValue = CPU_FREQ_REGISTER_VALUE;
    *(volatile MS_U16 *)(0x1f000000 + (0x101df8 << 1)) = 0x0001;
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa4 << 1)) = (dwRegisterValue & 0xFFFF);
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa6 << 1)) = ((dwRegisterValue >> 16) & 0xFFFF);
    *(volatile MS_U16 *)(0x1f000000 + (0x110ab0 << 1)) = 0x01;   //switch to LPF control
    *(volatile MS_U16 *)(0x1f000000 + (0x110aaa << 1)) = 0x06;   //mu[2:0]
    *(volatile MS_U16 *)(0x1f000000 + (0x110aae << 1)) = 0x08;   //lpf_update_cnt[7:0]
    *(volatile MS_U16 *)(0x1f000000 + (0x110ab2 << 1)) |= (0x01 << 12);
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa8 << 1)) = 0x00;
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa8 << 1)) = 0x01;
    while(!(*(volatile MS_U16 *)(0x1f000000 + (0x110aba << 1))));
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa8 << 1)) = 0x00;
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa0 << 1)) = (dwRegisterValue & 0xFFFF);
    *(volatile MS_U16 *)(0x1f000000 + (0x110aa2 << 1)) = ((dwRegisterValue >> 16) & 0xFFFF);
}
#endif
#if defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
extern void LDR_Entry(void)  _SECTION_REE_ENTRY_;
#endif
void LDR_Entry(void)
{
#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6)
    // Check Boot status to do CPU frequency adjust
    MS_U8 u8DC_Flag = *(volatile MS_U8*)(DC_FLAG_REGISTER);
    MS_U8 u8AC_Flag = *(volatile MS_U8*)(AC_FLAG_REGISTER);

    // If boot with normal DC ON, adjust CPU to high speed
    if (u8DC_Flag == 0x01 || u8DC_Flag == 0x02 || u8DC_Flag == 0xF1)
    {
        LDR_Adjust_CPU_Frequency();
    }
    else
    {
        // If boot with AC on , adjust CPU to high speed
        if(u8AC_Flag != POWER_FLAG_AC)
        {
            LDR_Adjust_CPU_Frequency();
            *(volatile MS_U8*)(AC_FLAG_REGISTER) = POWER_FLAG_AC;
        }
        // If it's not DC on and AC on, it's a abnormal booting, so doesn't adjust CPU frequency
    }
#endif
#ifndef CONFIG_TEE_LOADER
    // For OTP code usage, setup jump address and magic number for Core1,2,3
    MS_U64 u64SecondaryCore_Entry = (MS_U64)Bootram_SecondaryCore;
    *(volatile MS_U16*)(0x1F2067AC) = (u64SecondaryCore_Entry >> 16) & 0xffff;
    *(volatile MS_U16*)(0x1F2067A8) = u64SecondaryCore_Entry & 0xffff;
    *(volatile MS_U16*)(0x1F2067B0) = 0xCAFE;
#endif

#ifndef CONFIG_TEE_LOADER
#if defined(CONFIG_MSTAR_KANO)
    InitUTMIPLL();
#endif
#endif
#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_K6)
    InitUSB30GPIO();
#endif

#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_CURRY)
    // Setup MIU ARB from BW table first
    // It avoid display abnormal before BW table setup with PQ driver
    // Above problem only happen 4K60p display
    MHal_RIU_WritReg16(0x1615e2, 0x001e);
    MHal_RIU_WritReg16(0x1615e8, 0x0018);
    MHal_RIU_WritReg16(0x1615ea, 0x0480);
    MHal_RIU_WritReg16(0x1615ec, 0x0202);
#endif

#ifndef CONFIG_TEE_LOADER
#if defined(CONFIG_MSTAR_CURRY)
LDR_CPU_Reset_Entry();
#endif
#endif

#if defined(CONFIG_MSTAR_K6)
    #if (defined ENABLE_FCIE_GPIO_CONTROL) && ENABLE_FCIE_GPIO_CONTROL
        InitFCIEGPIO();
    #endif
    #if (defined ENABLE_SDIO_GPIO_CONTROL) && ENABLE_SDIO_GPIO_CONTROL
        InitSDIOGPIO();
    #endif
#endif

#if defined(CONFIG_MSTAR_NS_UBOOT)
#ifndef CONFIG_TEE_LOADER
#ifdef CONFIG_NAND_FLASH
    BOOT_NAND_1st(); //read CIS info for nand driver
#endif
    if (!ns_uboot_init())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }

#if defined(CONFIG_MSTAR_K6)
#ifdef CONFIG_CHIP_TYPE_NAGRA
    extern MS_U32 mstar_secondary_reset[];
    MS_U64 u64SecondaryCore_Entry1 = (MS_U64)mstar_secondary_reset >> 10;
    *(volatile MS_U16*)(0x1F203DB0) = (u64SecondaryCore_Entry1 & 0xffff);
    *(volatile MS_U16*)(0x1F203DB4) = ((u64SecondaryCore_Entry1 >> 16) & 0xffff) | 0x8000;
#else
    *(volatile MS_U16*)(0x1F203DB0) = 0x0000;
    *(volatile MS_U16*)(0x1F203DB4) = 0x0000 | 0x8000;
#endif
    // protected until "ARM warm boot base address" of core1/2/3 has been set
    // After that, this register bank could be released to REE
    *(volatile MS_U16*)(0x1F247248) |= (1<<7);
    *(volatile MS_U16*)(0x1F247264) |= (1<<9);
#ifdef CONFIG_CHIP_TYPE_NAGRA
    *(volatile MS_U16*)(0x1F203BE4) = 0x000F;
#endif
#endif

#if defined (CONFIG_CHIP_GENERIC_DRM)
    LDR_PUTS("GENERIC_DRM Delay IN\n");
    LDR_DelayMS(12*1000);
    LDR_PUTS("GENERIC_DRM Delay OUT\n");
#endif
    if(!ns_uboot_read_keybank())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }

#if defined (CONFIG_CHIP_GENERIC_DRM)
    LDR_PUTS("GENERIC_DRM Delay IN\n");
    LDR_DelayMS(12*1000);
    LDR_PUTS("GENERIC_DRM Delay OUT\n");
#endif
    if (!ns_uboot_load_armfw())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }

    if (!ns_uboot_load_optee())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }

    if (!ns_uboot_anti_rollback())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }

    if (!ns_uboot_cleanup())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }

    if (!ns_uboot_set_volt_freq_limite())
    {
        ns_uboot_nonsecure_handler();
        while(1);
    }
#endif
#endif

#ifdef CONFIG_NO_2ED_LOADER
    PUTC('\r'); PUTC('\n');
    PUTC('T'); PUTC('E'); PUTC('S'); PUTC('T'); PUTC('_');
    PUTC('L'); PUTC('D'); PUTC('R'); PUTC('_'); PUTC('O'); PUTC('K');
    PUTC('\r'); PUTC('\n');


    while(1) {
        // do nothing
    }
#else

    #ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
        LDR_Entry_USB();
    #endif

    //remap_dspram();
    #if defined(CONFIG_BOOT_SEQUENCE_FORCE_SPINOR)
        LDR_Entry_SPI();
    #elif defined(CONFIG_BOOT_SEQUENCE_FORCE_BFN) && defined(CONFIG_MSTAR_BFN_EN)
        LDR_Entry_NAND();
    #elif defined(CONFIG_BOOT_SEQUENCE_FORCE_BFE) && defined(CONFIG_MSTAR_BFE_EN)
        LDR_Entry_EMMC();
    #elif defined(CONFIG_BOOT_SEQUENCE_AUTO)
        #error "CONFIG_BOOT_SEQUENCE_AUTO not implement!"
    #endif
#endif
}
