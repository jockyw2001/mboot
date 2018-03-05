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

#ifdef CONFIG_ENABLE_SBOOT_BOOT_LOGO
#include "sbootBootLogo.h"
#endif

extern MS_U8 _ld_APP_load_start[];

typedef void (*PF_AP_Entry)(void);

//#define PUTC(c) do { *(volatile unsigned short *)0xBF201300 = c; } while(0)
void PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

#if (CONFIG_Second_UART_Mode_1 || CONFIG_Second_UART_Mode_2  || CONFIG_Second_UART_Mode_3 ||\
    CONFIG_Third_UART_Mode_1 || CONFIG_Third_UART_Mode_2  || CONFIG_Third_UART_Mode_3 ||\
    CONFIG_Fourth_UART_Mode_1 || CONFIG_Fourth_UART_Mode_2  || CONFIG_Fourth_UART_Mode_3)
#define VERIFICATION_ENABLE 0
#else
#define VERIFICATION_ENABLE 1
#endif

#define PADCFG_END        0xFFFF
#define PADCFG_MEM_BASE   0x0000
#define REG_ADDR_BASE     0xBF000000

#define SHA_HASH_DIGI_LEN           8                                   // 32-bit count, 5 for SHA-1, 8 for SHA-256,384,512
void SHA256_Init(MS_U32 *hash);
void SHA256_Hash(const MS_U32 *data, MS_U32 *hash);
void SHA256_Hash_Message(MS_U8 *pu8Message, MS_U32 u32Length);
void flush_L2(void);


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

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#define WAKEUP_ADDR_MASK          0x0000FFF0
#define WAKEUP_FLAG_MASK          0x0000000F
#define WAKEUP_ADDR_SHIFT         16
#define WAKEUP_FLAG_INVALID       0
#define WAKEUP_FLAG_SLEPT         1
#define WAKEUP_FLAG_WKUP          2
#define MSTAR_SLEEP_MAGIC         0x4D535452
#define PMU_WAKEUP_ADDR_LMASK    0x000000FF
#define PMU_WAKEUP_ADDR_HMASK    0x0000FF00


#define MIU0_VIRTBASE1   0x80000000
#define MIU1_BASE_ADDR1  0x60000000
#define MIU1_BASE_ADDR2  0x70000000
#define MIU1_VIRTBASE1   0x00000000
#define MIU1_VIRTBASE2   0x20000000
#define _PHY_TO_VIRT(x) ((x<MIU1_BASE_ADDR1)?(x+MIU0_VIRTBASE1):    \
                          ((x<MIU1_BASE_ADDR2)?(x-MIU1_BASE_ADDR1+MIU1_VIRTBASE1):(x-MIU1_BASE_ADDR2+MIU1_VIRTBASE2)) )

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
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
    if((pmu_str_reg & WAKEUP_FLAG_MASK) == WAKEUP_FLAG_SLEPT && pwkupsave
       && pwkupsave[0]==MSTAR_SLEEP_MAGIC && pwkupsave[1])
    {
       return 1;
    }
    return 0;
}

int sboot_do_check_str_resume(void)
{

    unsigned long pmu_str_reg=0,*pwkupsave=0;

    pmu_str_reg=sboot_read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<16);

    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);

    if(_is_str_resume_sboot())
    {
    PUTC('\r'); PUTC('\n');
    PUTC('S'); PUTC('B'); PUTC('O'); PUTC('O');PUTC('T');PUTC('_');
    PUTC('R'); PUTC('E'); PUTC('S'); PUTC('U'); PUTC('M'); PUTC('E');
    PUTC('\r'); PUTC('\n');

        sboot_write_wkup_pmu((pmu_str_reg&~WAKEUP_FLAG_MASK)|WAKEUP_FLAG_WKUP);

        {
              void (*resume)(void);
              resume = (void *)PHY_TO_VIRT(pwkupsave[1]);
              if(resume)
               {
                  resume();
               }
        }
    }
    else
    {
        sboot_write_wkup_pmu(0);
    }
    return 0;
}
#endif //CONFIG_MSTAR_STR_ENABLE

void enable_cache(void)
{
    //patch for CA chip that it disable l1 cache in otp boot code. The valid data will be cached in l2.
    //When enable l1, the valid data in l2 may be fetched in l1, which is already an old data that does not sync with latest data in dram
    //So before enable,it should ensure the l2 data is invalid
    flush_L2();

    void enable_cache_asm(void);
    asm volatile(
            "jal enable_cache_asm\n"
            "nop\n" : : :
            "$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}

void flush_cache(void)
{
    void flush_cache_asm(void);
    asm volatile(
            "jal flush_cache_asm\n"
            "nop\n" : : :
            "$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}

void _CPU_Sync(void)
{
    asm volatile (
        "sync;"
        );
}

void flush_L2(void)
{
    unsigned int    dwReadData = 0;
    static unsigned int u8_4Lines[64];
    volatile unsigned int *pu8 = NULL;
    volatile unsigned int tmp  = 0;
    //Flush Prefetch Buffer and MIU Pipe
    if(((*(volatile unsigned int *)(REG_PREFTECH_L2_CONFIG)) & PREFTECH_L2_CONFIG_DISABLE) != 0)
    {
        _CPU_Sync();
    }
    else
    {
        if ((*(volatile U16 *)(REG_PREFTECH_L2_CONTROL) & PREFTECH_L2_CONTROL_MODE0DONE))
        {
            dwReadData = *(volatile U16 *)(R_DEBUG);    //Dummy Read
            *(volatile U16 *)(REG_PREFTECH_L2_CONTROL) = 0x04;          //4: Flush all and invalid cache
            dwReadData = *(volatile U16 *)(R_DEBUG);    //Dummy Read
            *(volatile U16 *)(REG_PREFTECH_L2_CONTROL) = 0x14;
            do
            {
                dwReadData = *(volatile U16 *)(R_DEBUG);   //Dummy Read
                dwReadData = *(volatile U16 *)(REG_PREFTECH_L2_CONTROL);
                dwReadData &= PREFTECH_L2_CONTROL_MODE34DONE;

            } while(dwReadData == 0);

            *(volatile U16 *)(REG_PREFTECH_L2_CONTROL) = 0x00;
        }
        // MIU0
        // Transfer the memory to noncache memory
        pu8 = ((volatile unsigned int *)(((unsigned int)u8_4Lines) | 0xa0000000));

        // Flush the data from pipe and buffer in MIU
        pu8[0] = pu8[16] = pu8[32] = pu8[48] = 1;
        // Flush the data in the EC bridge buffer
        _CPU_Sync();
        // final read back
        tmp = pu8[48] ;

        // final read back
        tmp = pu8[0] ;
        tmp = pu8[16] ;
        tmp = pu8[32] ;
        tmp = pu8[48] ;

    }

}

void _UART_Reset(void)
{
    //UART Init
    *(volatile volatile unsigned int*)(0xBF203D4c) = 0x0C01;
    *(volatile volatile unsigned int*)(0xBF001C24) = 0x0800;

    *(volatile volatile unsigned int*)(0xbf203d4c) = 0x0004;
    *(volatile volatile unsigned int*)(0xbf201318) = 0x0080;
    *(volatile volatile unsigned int*)(0xbf201300) = 0x0042;
    *(volatile volatile unsigned int*)(0xbf201308) = 0x0000;
    *(volatile volatile unsigned int*)(0xbf201318) = 0x0000;

    *(volatile volatile unsigned int*)(0xbf201310) = 0x0000;
    *(volatile volatile unsigned int*)(0xbf201310) = 0x0007;

    *(volatile volatile unsigned int*)(0xbf201318) = 0x0000;
    *(volatile volatile unsigned int*)(0xbf201318) = 0x0003;
    *(volatile volatile unsigned int*)(0xbf201320) = 0x0000;
    *(volatile volatile unsigned int*)(0xbf201308) = 0x0000;

#if (CONFIG_Second_UART_Mode_1)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0xff0f;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x0040;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xfff0;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0001;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Second_UART_Mode_2)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0xff0f;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x0040;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xfff0;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0002;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Second_UART_Mode_3)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0xff0f;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x0040;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xfff0;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0003;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Third_UART_Mode_1)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0xf0ff;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x0400;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xff0f;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0010;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Third_UART_Mode_2)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0xf0ff;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x0400;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xff0f;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0020;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Third_UART_Mode_3)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0xf0ff;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x0400;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xff0f;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0030;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Fourth_UART_Mode_1)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0x0fff;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x4000;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xf0ff;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0100;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Fourth_UART_Mode_2)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0x0fff;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x4000;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xf0ff;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0200;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

#if (CONFIG_Fourth_UART_Mode_3)
    *(volatile volatile unsigned int*)(0xbf203d4c) &= 0x0fff;
    *(volatile volatile unsigned int*)(0xbf203d4c) |= 0x4000;
    *(volatile volatile unsigned int*)(0xbf203c0c) &= 0xf0ff;
    *(volatile volatile unsigned int*)(0xbf203c0c) |= 0x0300;
    *(volatile volatile unsigned int*)(0xbf203d40) = 0x0000;
#endif

}
#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)

#define READ_OTP(x) (*(volatile MS_U32*)(0xBF320000 + (x)))
#define READ_OTP_SHDW(x) (*(volatile MS_U16*)(0xBF000000 + (0x171600<<1) + (x<<2) ))


MS_U8 MHal_GMAC_ReadReg8( MS_U32 bank, MS_U32 reg )
{
    MS_U8 val;
    MS_U32 address = 0xBF000000 + bank*0x100*2;
    address = address + (reg << 1) - (reg & 1);

    val = *( ( volatile MS_U8* ) address );
    return val;
}

void MHal_GMAC_WritReg8( MS_U32 bank, MS_U32 reg, MS_U8 val )
{
    MS_U32 address = 0xBF000000 + bank*0x100*2;
    address = address + (reg << 1) - (reg & 1);

    *( ( volatile MS_U8* ) address ) = val;
}

void LDR_IP_Trim(void)
{

    #if defined(CONFIG_MSTAR_KAYLA)
        #define LDR_Write2ByteMask( u32Reg, u16Val, u16Msk )                         \
        do{                                                                         \
        U32 u32Reg8 = 0xbf000000 + ( (u32Reg) <<1);                                      \
        WREG16( u32Reg8 ,(RREG16(u32Reg8) & ~(u16Msk)) | ((u16Val) & (u16Msk)) );     \
        }while(0)

        #define BIT(_bit_)                  (1 << (_bit_))

        MS_U32 u32OTPAddr = 0;
        MS_U16 U16OTPData = 0;
        MS_U8 U8OTPFlag = 0;

        //SMC IO PAD (padd_pdbs08p2dg_smc)
        U16OTPData = READ_OTP_SHDW(0x29);

        //LDR_PUTW(U16OTPData);
        LDR_Write2ByteMask( 0x101e00 +(0x2e<<1), BIT(15) ,BIT(15));
        if ((U16OTPData & (1<<2))==(1<<2))
        {
            //LDR_PUTS("SMC IO PAD\n");
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 1 ,BIT(5));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 2 ,BIT(6));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 3 ,BIT(7));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 4 ,BIT(8));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 5 ,BIT(9));

            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 7 ,BIT(10));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 8 ,BIT(11));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 9 ,BIT(12));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 10 ,BIT(13));
            LDR_Write2ByteMask( 0x101e00 +(0x29<<1), U16OTPData << 11 ,BIT(14));
        }

        //PM_SAR_ATOP
        LDR_Write2ByteMask( 0xe00 +(0x19<<1), RREG16(0xbf000000 + ((0xe00 +(0x79<<1))<<1)) ,BIT(8)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13));
        U16OTPData = READ_OTP_SHDW(0x32);
        LDR_Write2ByteMask( 0xe00 +(0x75<<1), U16OTPData ,BIT(8)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13)|BIT(14)|BIT(15));
    #elif defined(CONFIG_MSTAR_K5TN)
        //K5TN IP Trim Code
	#else
    MS_U32 otp = 0;
    MS_U16 data = 0;
    // SAR

    otp = (READ_OTP(0x3C8C) & 0x0000FC00 ) >> 10;                     // TR[5:0]
    *(volatile MS_U16*)(0xBF000000 + (0x0E32 << 1)) |= (otp << 8);

    // SMC
    if(READ_OTP(0x3C8C) & 0x00010000) // EN_TRIM_PU_11K
    {
        otp = (READ_OTP(0x3C8C) & 0x00040000 ) >> 18;                     // PU_11K_HIGH
        *(volatile MS_U16*)(0xBF000000 + (0x101E52 << 1)) |= ((otp << 8) | (otp << 6) | (otp << 5));
        otp = (READ_OTP(0x3C8C) & 0x00020000 ) >> 17;                     // PU_11K_LOW
        *(volatile MS_U16*)(0xBF000000 + (0x101E52 << 1)) |= ((otp << 13) | (otp << 11) | (otp << 10));
    }

    // GMAC
    //efuse read
    MS_U8 uRegVal;
    MS_U8 uEfuVal0;
    MS_U8 uEfuVal1;
    MS_U8 uEfuVal2;

    uEfuVal0 = *( ( volatile MS_U8* )0xBF323C88 );
    uEfuVal1 = *( ( volatile MS_U8* )0xBF323C89 );
    uEfuVal2 = *( ( volatile MS_U8* )0xBF323C8a );
    uEfuVal2 = uEfuVal2 & 0x03;

    //Set key to enable trim function
    uRegVal = MHal_GMAC_ReadReg8(0x0033, 0x60);
    uRegVal |= 0x04;
    MHal_GMAC_WritReg8(0x0033, 0x60,  uRegVal);

    uRegVal = MHal_GMAC_ReadReg8(0x0033, 0x69);
    uRegVal |= 0x80;
    MHal_GMAC_WritReg8(0x0033, 0x69,  uRegVal);

    //Set efuse value
    MHal_GMAC_WritReg8(0x0033, 0x68,  uEfuVal0);

    uRegVal = MHal_GMAC_ReadReg8(0x0033, 0x69);
    uRegVal &= 0xc0;
    uRegVal |= (uEfuVal1 & 0x3f);
    MHal_GMAC_WritReg8(0x0033, 0x69,  uRegVal);

    uRegVal = MHal_GMAC_ReadReg8(0x0033, 0x61);
    uRegVal &= 0xf0;
    uRegVal |= (uEfuVal2 << 2);
    uRegVal |= (uEfuVal1 & 0xc0) >> 6 ;
    MHal_GMAC_WritReg8(0x0033, 0x61,  uRegVal);
    #endif

    #ifdef CONFIG_MSTAR_VID_ENABLE    //VID Initial Voltage Identification
        #if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN)
        #define VID_ROSC_CRITERIA 0x4E00 // OSC fast than 9.75Mhz
        void BOOTROM_C_InitVID(MS_U32 u32Args)
        {
            MS_U32 u32otp = *(volatile MS_U32*)0xBF3238E0;
            MS_U32 u32ROSC = ( u32otp >> 16 ) & 0xFFFF;

            if(u32ROSC < VID_ROSC_CRITERIA)
            {
                *(volatile volatile unsigned int*)(0xbf206720)=0x5566;
                // set high voltage
                CpuCorePWRHigh();
            }
            else
            {
                *(volatile volatile unsigned int*)(0xbf206720)=0x5577;
                // set low voltage
                CpuCorePWRLow();
            }
        }

        #endif

        #if defined(CONFIG_MSTAR_KRIS)
        #define VID_ROSC_CRITERIA 0x5cd0

        MS_U32 u32otp = *(volatile MS_U32*)0xBF3238E0;
        MS_U32 u32ROSC = ( u32otp >> 16 ) & 0xFFFF;

        if(u32ROSC < VID_ROSC_CRITERIA)
        {
            // set high voltage
            CpuCorePWRHigh();
        }
        else
        {
            // set low voltage
            CpuCorePWRLow();
        }
        #endif
    #endif  /*CONFIG_MSTAR_VID_ENABLE*/
}
#endif
void LDR_PadConfig(void)
{
    U8  u8Length;
    U8* pu8PadConfig = (U8*)(padInitTbl+10);
    U16 u16Index; // register index
    U32 u32MapBase = REG_ADDR_BASE;
    volatile U8* u8Addr;

    WREG_DBG(DBG_F_LDR_PAD, 1);

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
                    u8Addr = (U8*)(u32MapBase + (((U32)(u16Index))<<1) - (u16Index & 1));

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
int DoUnCompression(unsigned int addr)
{
    //ISzAlloc g_Alloc = { SzAlloc, SzFree };
    //SRes res;
    int useFilter;
    //SizeT inSizePure;
    //ELzmaStatus status;
    unsigned int u32ROM_START = addr + 0x100;                  // This address should be 4' alignment
    unsigned int u32ROM_END = addr + *((unsigned int *)(addr + 0x8));   // This address should be 4' alignment
    unsigned int u32RAM_START, u32RAM_END,i =0;
    unsigned int CompressedFileLength = 0;
    unsigned int OriginalFileLength = 0;
    //unsigned char header[LZMA_PROPS_SIZE + 8];
    /* Original Param */
    //Byte *dest = NULL;
    unsigned int destLen = 0;
    //const Byte *src = NULL;
    unsigned int srcLen = 0;
    unsigned int dst = *(unsigned int*)addr;

    //info_mem =(unsigned int *)(0xA0880030);

    u32RAM_START = *((unsigned int *)(addr)) + 0x100;
    u32RAM_END = u32RAM_START+ *((unsigned int *)(addr + 0xC));

    CompressedFileLength = (u32ROM_END - u32ROM_START);
    OriginalFileLength = (u32RAM_END - u32RAM_START);

    //Copy compressed UBoot from SPI flash to RAM
    for(i = 0; i < ((CompressedFileLength + 7 + 0x100) & ~7UL); i += 4)
    {
        *((unsigned int *)(dst + i)) = *((unsigned int *)((u32ROM_START - 0x100) + i));
        // Copy from Addr (Storage to DRAM)
    }

    if(!LDR_APP_Auth((unsigned char *)dst, *(unsigned int *)(addr + 0x8)))
    {
        PUTC('!');
        return -1;
    }

    return 0;
}

void LDR_Entry_SPI(void)
{
    MS_U32              *src, *dst, *end;
    PF_AP_Entry         pfEntry;

    enable_cache();

    //Note: src, dst, end should be 16-byte aligned.
    src = (MS_U32*)_ld_APP_load_start;
    dst = (MS_U32*)(*(src));
    end = (MS_U32*)((MS_U32)src + *(src+3));
    pfEntry = (PF_AP_Entry)((MS_U32)dst + *(src+1));

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    LDR_PadConfig();

#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA)|| defined(CONFIG_MSTAR_K5TN)
    LDR_IP_Trim();
#endif

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
    sboot_do_check_str_resume();
#endif
#endif // !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)

#if defined(CONFIG_MSTAR_KIRIN)
    if((RREG16(REG_VERSION_ID) >> 8) >= 1) // U02~
    {
        WREG16(REG_LED_MODE, (RREG16(REG_LED_MODE) | (0x3 << 4))); // reg_led_mode = 3 after kirin U02
    }
#endif

#ifdef CONFIG_ENABLE_SBOOT_BOOT_LOGO
    do_bootlogo();
#endif

    if (1)
    {
#if defined(CONFIG_LDR_LZMA_COMPRESS)

        PUTC('d');PUTC('e');PUTC('c');PUTC('o');PUTC('m');PUTC('p');PUTC('\r');PUTC('\n');
        extern int DoLZMADecompression(U32 addr);
        if (DoLZMADecompression((U32)(_ld_APP_load_start)) < 0)
        {
            // coverity[no_escape]
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


        flush_cache();
        flush_L2();

        pfEntry();

    }

}
// SHA-256 Hash Computation

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
    MS_U32              msg_addr;
    MS_U32              tmp;

    SHA256_Init(sha_H);

    for(i = 0; i < sizeof(sha_M) / sizeof(sha_M[0]); i++)
        sha_M[i] = 0;

    msg_addr = (MS_U32)(pu8Message);
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



#define mips_cp0_count_get() ({MS_U32 count; asm volatile ("mfc0 %0, $9" : "=r" (count) :); count;})

void LDR_Delay(MS_U32 msec)
{
    MS_U32 tmo = (MS_U32)msec * (CPU_CLK_FREQ / 2 / 1000);

    asm volatile ("mtc0 $0, $11"); // clear cp0 compare
    asm volatile ("mtc0 $0, $9");  // clear cp0 compare
    while (mips_cp0_count_get() < tmo)
        ;
}

#if 1
#define LDR_PUTC(val) PUTC(val)

void LDR_PUTX(char val)
{
    if (val > 0x9)
        LDR_PUTC(0x41+(val-10));
    else
        LDR_PUTC(0x30+val);
}
void LDR_PUTW(MS_U16 val)
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

void LDR_PUTDW(MS_U32 val)
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

void LDR_PUTS(const char *s)
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

#if (defined(CONFIG_LD_BOOT_FROM_OTP) || defined(CONFIG_LD_BOOT_OTP_SIMULATE_ON_SPINOR)) && \
   (((defined(CONFIG_LD_BFN_VERSION) && (CONFIG_LD_BFN_VERSION == 4)) && (defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA))) || \
   ((defined(CONFIG_LD_BFN_VERSION) && (CONFIG_LD_BFN_VERSION == 5)) && (defined(CONFIG_MSTAR_K5TN))))

#include "drvNAND.h"

#define BINIDTAG    (0x42494E30)      // 'BIN0'
NAND_DRIVER NandDrv;

static MS_U8 pu8_RamBuf[1024*8]   __attribute__((aligned(16)));
static MS_U8 pu8_SpareBuf[2048]   __attribute__((aligned(16)));


PAIRED_PAGE_MAP_t ga_tPairedPageMap[512];


void * ldr_memset(void * s,char c,int count)
{
    char *xs = (char *) s;

    while (count--)
        *xs++ = c;

    return s;
}

int memcmp(const void * cs,const void * ct,size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}
static MS_U32 _NAND_ReadPage(MS_U8 *u8_buf, MS_U8 *u8_SpareBuf, MS_U32 pageNum)
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
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "NC_ReadSectors(0x%X)=0x%X\n", (unsigned int)u32_PageIdx, (unsigned int)u32_Err);

	    if (pu8_SpareBuf[0] != 0xFF)
		    return 0; // bad blk
	}

	return 1; // good blk


}

MS_U32 BOOT_NAND_1st(void)
{
    MS_U32 cnt = 0, i, max_redundancy = 256;
    MS_U32 ret = 0, u32_PageIdx=0;
    MS_U16 u16_NandMode = 0;
    //NAND_FLASH_INFO_t *pNandInfo;

    WREG_DBG(DBG_F_LDR_NAND_1ST, 1);

    ldr_memset((void *) &NandDrv, 0, sizeof(NandDrv));

    /* Check NAND Mode for PE setting */
    REG_READ_UINT16(reg_nf_en, u16_NandMode);

    u16_NandMode &= REG_NAND_MODE_MASK;

    nand_pads_init(&NandDrv, u16_NandMode);

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
        //LDR_PUTDW(cnt);LDR_Delay(10);
        //LDR_PUTC(':');LDR_Delay(10);
        //LDR_PUTDW(*(MS_U32*)pu8_RamBuf);LDR_Delay(10);LDR_PUTS("\n");

        if(ret != 0)
		{
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
            //LDR_PUTW(pNandInfo->u16_PageByteCnt);LDR_PUTC(' ');
            //LDR_PUTW(pNandInfo->u16_SpareByteCnt);LDR_PUTC(' ');
            //LDR_PUTW(pNandInfo->u16_BlkPageCnt);LDR_PUTC(' ');
            //LDR_PUTW(pNandInfo->u16_ECCType);LDR_PUTC(' ');

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
		break;
    }

    if(cnt == max_redundancy)
    {
        LDR_PUTS("Hit border 0\n");
        WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
        while(1){}
    }

    return 0;
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

#ifdef CONFIG_ENABLE_SBOOT_BOOT_LOGO
MS_BOOL NAND_Read_LogoData(MS_U8 *u8_buf, MS_U32 u32NandOffset)
{
    MS_U16 blkNum = 0;
    MS_U16 u16_BlkPageCnt=0, u16_BlkSectorCnt=0;
    MS_U32 u32_SectorCnt =0, u32_SectorCntRead=0, ret = 0;

	u16_BlkPageCnt = (NandDrv.u8_CellType)?NandDrv.u16_BlkPageCnt>>1:NandDrv.u16_BlkPageCnt;
	u16_BlkSectorCnt = u16_BlkPageCnt << NandDrv.u8_PageSectorCntBits;

    MS_U32 u32BlkSize = (MS_U32)((MS_U32)(NandDrv.u16_BlkPageCnt << NandDrv.u8_PageSectorCntBits) << NandDrv.u8_SectorByteCntBits);
    blkNum = (MS_U16)(u32NandOffset / u32BlkSize);

    //LDR_PUTS("\n blkNum = ");
    //LDR_PUTDW(blkNum);
    //LDR_PUTS("\n u32BlkSize = ");
    //LDR_PUTDW(u32BlkSize);
    //LDR_PUTS("\n");

    if(_NAND_IsGoodBlk(u8_buf,blkNum)==0)
    {
        LDR_PUTS("\n Error, Bad Block\n");
        return FALSE;
    }
    else
    {
        u32_SectorCntRead = u16_BlkSectorCnt;
        ret =_NAND_ReadPage(u8_buf, pu8_SpareBuf, blkNum*NandDrv.u16_BlkPageCnt);
        if(ret != UNFD_ST_SUCCESS)
        {
            LDR_PUTS("\n Read Logo Fail\n");
            return FALSE;
        }
        else
        {
            MS_U32 u32LogoSize = *((MS_U32*)u8_buf);

            if(u32LogoSize >= CONFIG_LOGO_SIZE)
            {
                LDR_PUTS("\n u32LogoSize = ");
                LDR_PUTDW(u32LogoSize);
                LDR_PUTS(" is too large, return false\n");
                return FALSE;
            }

            u32_SectorCnt = (u32LogoSize + CONFIG_LOGO_HEADER_SIZE + NandDrv.u16_SectorByteCnt-1) >> NandDrv.u8_SectorByteCntBits;

            while(u32_SectorCnt)
            {
                //error bonding check
                if(blkNum == 256)
                {
                    LDR_PUTS("Hit border 2\n");
                    //WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
                    while(1){}
                }

                if(_NAND_IsGoodBlk(u8_buf,blkNum)==0)
                    continue;

                if(u32_SectorCnt >= u16_BlkSectorCnt)
                    u32_SectorCntRead = u16_BlkSectorCnt;
                else
                    u32_SectorCntRead = u32_SectorCnt;

                ret = _NAND_Read_One_Blk((MS_U8 *)u8_buf, blkNum, u32_SectorCntRead);

                if(ret != UNFD_ST_SUCCESS)
                {
                    blkNum++;
                    LDR_PUTS("_NAND_Read_One_Blk failed \n");
                    return FALSE;
                }

                u8_buf += u32_SectorCntRead<<NandDrv.u8_SectorByteCntBits;
                u32_SectorCnt -= u32_SectorCntRead;
                blkNum ++;
            }
        }
    }
    return TRUE;
}
#endif

PF_AP_Entry BOOT_NAND_2nd(MS_U32* U32ld_APP_load_start)
{
    MS_U32 BINSize = 0, ret = 0;
    MS_U32 MIUPos = 0, BIN_Entry = 0;
    MS_U16 blkNum = 2; //blk#0#1 should be cis or loader
    MS_U16 read_blk = 0;
    MS_U32 END_TAG_ADDR = 0;
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
            //WREG_DBG(DBG_F_LDR_HITAPP, 1);
            MIUPos = *((MS_U32*)(pu8_RamBuf+4));
            *U32ld_APP_load_start = MIUPos;
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

        read_blk = 0;
        u32_SectorCnt = (BINSize + NandDrv.u16_SectorByteCnt-1) >> NandDrv.u8_SectorByteCntBits;

		//load binary
        while(u32_SectorCnt)
        {
            //LDR_PUTS("\n blkNum: ");LDR_PUTW(blkNum);
            //LDR_PUTS("\n MIUPos ");LDR_PUTDW(MIUPos);
            //LDR_PUTS("\n BINSize: ");LDR_PUTDW(BINSize);

            //error bonding check
            if(blkNum == 256){
                LDR_PUTS("Hit border 2\n");
                //WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
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
                LDR_PUTS("_NAND_Read_One_Blk failed \n");
                continue;
            }

#if defined(CONFIG_LD_BFN_VERSION) && (CONFIG_LD_BFN_VERSION < 5)
            if (*(MS_U32*)(pu8_SpareBuf+4) != (0x42010000|read_blk))
            {
                blkNum++;
                LDR_PUTS("wrong spare area ID tag,find backup\n");
                continue;
            }

            read_blk++;
#endif

            MIUPos += u32_SectorCntRead<<NandDrv.u8_SectorByteCntBits;
            u32_SectorCnt -= u32_SectorCntRead;
            blkNum ++;
        }

#if defined(CONFIG_LD_BFN_VERSION) && (CONFIG_LD_BFN_VERSION > 4)
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
            //WREG_DBG(DBG_E_LDR_LOAD_APP_FAIL, 2);
        }
#else
        return (PF_AP_Entry)BIN_Entry;
#endif
    }

    //error bonding
    LDR_PUTS("Hit border 3\n");
    //WREG_DBG(DBG_E_LDR_HIT_BORDER, 2);
    while(1){}

    return 0;

}

void LDR_Entry_NAND(void)
{
    PF_AP_Entry pfEntry;
    MS_U32 U32ld_APP_load_start = 0;

    WREG_DBG(DBG_F_LDR_ENTRY, 1);

    enable_cache(); //enable cache if cache is disable

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    LDR_PadConfig(); // switch PADs

#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
    LDR_IP_Trim();
#endif
#endif // !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)

#if (CONFIG_MSTAR_STR_ENABLE & CONFIG_MSTAR_STR_SBOOT_RESUME)
    sboot_do_check_str_resume();
#endif

    BOOT_NAND_1st(); //read CIS info for nand driver

#ifdef CONFIG_ENABLE_SBOOT_BOOT_LOGO
    do_bootlogo();
#endif

    pfEntry = (PF_AP_Entry)BOOT_NAND_2nd(&U32ld_APP_load_start); //load APP bin to DRAM

#if defined(CONFIG_MSTAR_KIRIN)
    if((RREG16(REG_VERSION_ID) >> 8) >= 1) // U02~
    {
        WREG16(REG_LED_MODE, (RREG16(REG_LED_MODE) | (0x3 << 4))); // reg_led_mode = 3 after kirin U02
    }
#endif
    //extern void DoMSDecompression(U32 addr);
    //DoMSDecompression(decompress_pos); // todo: to handle compress format

#if defined(CONFIG_LDR_LZMA_COMPRESS)
    PUTC('d');PUTC('e');PUTC('c');PUTC('o');PUTC('m');PUTC('p');PUTC('\r');PUTC('\n');
    extern int DoLZMADecompression(U32 addr);
    if (DoLZMADecompression((U32)(U32ld_APP_load_start)) < 0)
    {
        while(1);
    }
    PUTC('d');PUTC('o');PUTC('n');PUTC('e');PUTC('\r');PUTC('\n');
#endif


    flush_cache();

    PUTC('J');PUTC('2');PUTC('A');PUTC('P');PUTC('P');PUTC('\r');
    PUTC('\n');
    WREG_DBG(DBG_F_LDR_J2APP, 1);

    pfEntry();
}
#elif  (CONFIG_LD_BFN_VERSION == 2) && ((!defined(CONFIG_MSTAR_KELTIC)) && (!defined(CONFIG_MSTAR_KAPPA)) && (!defined(CONFIG_MSTAR_KRITI)))
#include "bootrom_nand.h"
static NAND_NECESS_INFO _nf_info   _SECTION_LDR_STACK_;

void LDR_NC_ConfigContext(void)
{
    if (_nf_info.u16ECCType < ECC_TYPE_24BIT1KB)
        _nf_info.u16SectorByteCnt = 0x200;
    else
        _nf_info.u16SectorByteCnt = 0x400;

    _nf_info.u16PageSectorCnt = _nf_info.u16PageByteCnt/_nf_info.u16SectorByteCnt;
    _nf_info.u16SectorSpareByteCnt = _nf_info.u16SpareByteCnt/_nf_info.u16PageSectorCnt;
    _nf_info.u16SpareByteCnt = _nf_info.u16SectorSpareByteCnt*_nf_info.u16PageSectorCnt;
}

void LDR_NC_ReInit()
{
    _nf_info.u16Reg48_Spare = _nf_info.u16SectorSpareByteCnt;
    _nf_info.u16Reg49_SpareSize = _nf_info.u16SpareByteCnt;
    _nf_info.u16Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;

    switch(_nf_info.u16PageByteCnt)
    {
        case 0x0200:
            _nf_info.u16Reg50_EccCtrl &= ~BIT_NC_PAGE_SIZE_512Bn;
            _nf_info.u16Reg48_Spare |= BIT_NC_ONE_COL_ADDR;
            break;
        case 0x0800:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;
            break;
        case 0x1000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;
            break;
        case 0x2000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_8KB;
            break;
        case 0x4000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_16KB;
            break;
        case 0x8000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_32KB;
            break;
    }

    switch (_nf_info.u16ECCType)
    {
        case ECC_TYPE_RS:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_RS;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_RS;
            _nf_info.u16ECCCorretableBit = 4;
            break;
        case ECC_TYPE_4BIT:
            _nf_info.u16Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_4BIT;
            _nf_info.u16ECCCorretableBit = 4;
            break;
        case ECC_TYPE_8BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT;
            _nf_info.u16ECCCorretableBit = 8;
            break;
        case ECC_TYPE_12BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_12BIT;
            _nf_info.u16ECCCorretableBit = 12;
            break;
        case ECC_TYPE_16BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_16b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_16BIT;
            _nf_info.u16ECCCorretableBit = 16;
            break;
        case ECC_TYPE_20BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_20b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_20BIT;
            _nf_info.u16ECCCorretableBit = 20;
            break;
        case ECC_TYPE_24BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT;
            _nf_info.u16ECCCorretableBit = 24;
            break;
        case ECC_TYPE_24BIT1KB:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT1KB;
            _nf_info.u16ECCCorretableBit = 24;
            break;
        default:
            break;
    }
    _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
}

void LDR_NC_Config()
{
    /*sector spare size*/
    HAL_WRITE_UINT16(NC_SPARE, _nf_info.u16Reg48_Spare);
    /* page spare size*/
    HAL_WRITE_UINT16(NC_SPARE_SIZE, _nf_info.u16Reg49_SpareSize);
    /* page size and ECC type*/
    HAL_WRITE_UINT16(NC_ECC_CTRL, _nf_info.u16Reg50_EccCtrl);
}

void LDR_NAND_SoftReset(void)
{
    MS_U16 u16Tmp;

    HAL_WRITE_UINT16(NC_CFG0,0x00);
    HAL_WRITE_UINT16(NC_CFG1,0x00);
    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,0);
    HAL_WRITE_UINT16(NC_CTRL,0);
    HAL_WRITE_UINT16(NC_MIE_EVENT,R_NC_JOB_END+R_NC_MMA_DATA_END);

    HAL_READ_UINT16(NC_TEST_MODE, u16Tmp);
    REG_CLR_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);

    LDR_Delay(1);

    REG_SET_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);

    LDR_Delay(2);

    HAL_WRITE_UINT16(NC_CFG0, _nf_info.u16NC_CFG0);

    HAL_WRITE_UINT16(NC_CFG1,0x00);
    HAL_WRITE_UINT16(NC_CTRL,0);
    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,R_NC_EN);

    LDR_NC_ConfigContext();
    LDR_NC_ReInit();
    LDR_NC_Config();

    HAL_WRITE_UINT16(NC_SDIO_CTL, 0x8210);

#if (defined CONFIG_MSTAR_KRONUS) || defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI)
    HAL_READ_UINT16(CIFIFO_CMD_ADDR, u16Tmp);
#endif

}

MS_U32 LDR_NAND_wNCJOBEND(MS_U8* u8Buf)
{
    MS_U16 u16Tmp,u16Tmp2,u16Cnt,u16_i,u16_j,u16_k,u16_ByteLoc;
    MS_U32 u32Cnt=0;

    do
    {
        HAL_READ_UINT16(NC_MIE_EVENT, u16Tmp);
        if (++u32Cnt == 0x3FFFF)
        {
            LDR_NAND_SoftReset();
            return 0;
        }
    }while ((!(u16Tmp & BIT_NC_R2N_ECC) )&&((u16Tmp & (R_NC_JOB_END|BIT_MMA_DATA_END)) != (R_NC_JOB_END|BIT_MMA_DATA_END)));

    HAL_READ_UINT16(NC_ECC_STAT2, u16Tmp2);
    if((u16Tmp2&BIT_NC_ECC_FLGA_MASK) == BIT_NC_ECC_FLGA_FAIL)
    {
        //LDR_NAND_SoftReset();
        return 0;
    }

    do
    {
        HAL_READ_UINT16(NC_MIE_EVENT, u16Tmp);
        if (++u32Cnt == 0x3FFFF)
        {
            LDR_NAND_SoftReset();
            return 0;
        }
    }while ((u16Tmp & (R_NC_JOB_END|BIT_MMA_DATA_END)) != (R_NC_JOB_END|BIT_MMA_DATA_END));

    if (u16Tmp & BIT_NC_R2N_ECC)
    {
        //LDR_PUTC('E');
        REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC);

        //correct ECC
        u16Cnt = ((u16Tmp2&BIT_NC_ECC_CNT_MASK)>>BIT_NC_ECC_CNT_SHIFT);
        //LDR_PUTDW(u16Cnt);
        for (u16_i=0;u16_i<u16Cnt;u16_i++)
        {
            REG_CLR_BITS_UINT16(NC_ECC_STAT2, BIT_NC_ECC_SEL_LOC_MASK);
            REG_SET_BITS_UINT16(NC_ECC_STAT2, u16_i<<BIT_NC_ECC_SEL_LOC_SHIFT);
LABEL_ECC_LOC_READ:
            HAL_READ_UINT16(NC_ECC_LOC, u16_j);
            HAL_READ_UINT16(NC_ECC_LOC, u16_k);
            if(u16_j != u16_k)
                goto LABEL_ECC_LOC_READ;
            u16_ByteLoc = u16_j>>3;
            if (u16_ByteLoc < _nf_info.u16SectorByteCnt)
                u8Buf[u16_ByteLoc] ^= 1<<(u16_j%8);
        }
    }

    return 1;
}

MS_U32 LDR_NAND_ReadSector(MS_U32 Row, MS_U32 SecInPage, MS_U16* u16Buf)
{
    MS_S32 i, j;
    MS_U16 u16Tmp;
    MS_U32 retry;

    // RIU and NAND transmission
    HAL_WRITE_UINT16(RIU2NAND_CTL, R_RIU2NAND_EN);
    //HAL_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC|BIT_NC_R2N_RDY|BIT_NC_JOB_END|BIT_MMA_DATA_END);

    //if (_nf_info.u16SectorByteCnt == 0x400)
    {
        HAL_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN);
        REG_CLR_BITS_UINT16(NC_PART_MODE, BIT_START_SECTOR_IDX_MASK);
        REG_SET_BITS_UINT16(NC_PART_MODE, SecInPage<<BIT_START_SECTOR_IDX_SHIFT);
    }

    // depends on user set ---> because SRAM might not enough for bigger page
    HAL_WRITE_UINT16(NC_JOB_BL_CNT, 1);

    HAL_WRITE_UINT16(NC_MMA_PRI_REG,0);

    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,R_MMA_ENABLE+R_NC_EN);

    // set row address
    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_ADRSET);
    HAL_WRITE_UINT16(NC_AUXR_DAT, (SecInPage*(_nf_info.u16SectorByteCnt)));
    HAL_WRITE_UINT16(NC_AUXR_DAT, Row);
    HAL_WRITE_UINT16(NC_AUXR_DAT, (Row>>16));

    // insert to queue
    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_INSTQUE);

    // if small block set one column mode first
    // write command 0x00
    HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)OP_ADR_C5TFS0<<8) + OP_CMD00_READ); // 5
    //HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)OP_ADR_C4TFS0<<8) + OP_CMD00_READ); // 4
    //HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)OP_ADR_C3TFS0<<8) + OP_CMD00_READ); // 3
    // write command 0x30
    HAL_WRITE_UINT16(NC_AUXR_DAT, ((MS_U16)OP_A_WAIT_READY_BUSY<<8) + OP_CMD30_READ);

    // write address
    HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)(OP_A_BREAK+0)<<8)+OP_A_SERIAL_DATA_IN); // repeat from instruct queue 0

    //HAL_WRITE_UINT16(NC_AUXR_DAT, ((MS_U16)OP_A_SERIAL_DATA_IN<<8) + OP_A_WAIT_READY_BUSY);
    //HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)(OP_A_BREAK)<<8)+OP_A_WAIT_READY_BUSY); // repeat from instruct queue 0

    // set CS to low
    //_NAND_Set_CS(0);

    // start to read data
    HAL_WRITE_UINT16(NC_CTRL, R_NC_CIFC_ACCESS+R_NC_JOB_START);

    for (i=0; i<(_nf_info.u16SectorByteCnt/512); i++)
    {
        // enable RIU to NAND
        HAL_WRITE_UINT16(RIU2NAND_CTL, R_RIU2NAND_EN);
        retry = 0;
        do
        {
            retry++;
            if(retry > 0xFFFFF)
            {
                //LDR_PUTC('F');
                goto lb_fail;
            }
            HAL_READ_UINT16(RIU2NAND_STAT, u16Tmp);
        } while ( !(u16Tmp & BIT_RIU_RDY_MMA) );

        if (i==0)
            HAL_WRITE_UINT16(RIU2NAND_CTL, R_RIU2NAND_EN | R_RIU2NAND_R_STR);
        //_PUTC('H');

        //_PUTDW(_nf_info.u16SectorByteCnt>>9);
        for (j=0; j<(256); j++)
        {
            // dangerous! no break mechanism? but if it fails, seems it is not the major problem
            do
            {
                HAL_READ_UINT16(NC_MIE_EVENT, u16Tmp);
            } while (!(u16Tmp & BIT_NC_R2N_RDY) );

            HAL_READ_UINT16(NC_R2N_DATA_RD, u16Buf[i*(256) +j]);

            //HAL_WRITE_UINT16(NC_MIE_EVENT, R_NC_RIU2NAND_END);

            HAL_WRITE_UINT16(NC_R2N_CTRL_SET, R_RIU2NAND_EN | R_RIU2NAND_R_EN);
        }

    }
lb_fail:
    HAL_WRITE_UINT16(RIU2NAND_CTL, BIT_R2N_MODE_EN|BIT_R2N_DI_END);
    retry = LDR_NAND_wNCJOBEND((MS_U8*)u16Buf);

    //_NAND_Set_CS(1);

    HAL_WRITE_UINT16(NC_PART_MODE, 0);

    HAL_WRITE_UINT16(RIU2NAND_CTL, 0);

    return retry;
}

MS_U32 LDR_NAND_ReadPage(MS_U32 Row, MS_U16* u16Buf, MS_U32 unit) // unit is the number of sector in the page
{
    MS_U16 i;
    MS_U32 retry;

    for (i=0;i<unit;i++)
    {
        //_PUTC('A');
        retry = LDR_NAND_ReadSector(Row,i,u16Buf+i*(_nf_info.u16SectorByteCnt>>1));
        if (!retry)
            return 0;
        //_PUTDW(*((MS_U32*)(u16Buf+i*(_nf_info.u16SectorByteCnt>>1))));
    }
    return 1;
}

void LDR_Entry_NAND(void)
{
    enable_cache();
    PF_AP_Entry         pfEntry;
    MS_U32 row_pos =0;
    MS_U32 blk_size = 0;
    MS_U32 decompress_pos = 0;

    HAL_READ_UINT16(NC_CFG0,_nf_info.u16NC_CFG0);
    _nf_info.u16PageByteCnt = 0x800;
    _nf_info.u16SpareByteCnt = 0x100;
    _nf_info.u16ECCType = NANDINFO_ECC_TYPE;
    LDR_NAND_SoftReset();

    // read the page size and page number from MIU block area
    REG_SET_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);
    while(1)
    {
        //LDR_PUTDW(row_pos);
        LDR_NAND_ReadPage(row_pos,_nf_info.pBuf,1);
        //LDR_PUTDW(*((MS_U32*)(_nf_info.pBuf)));
        if (*((MS_U32*)(_nf_info.pBuf)) == CISIDTAG)
        {
            _nf_info.u16PageByteCnt = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_PageByteCnt;
            _nf_info.u16BlockPageCnt = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_BlkPageCnt;
            _nf_info.u16SpareByteCnt_BK = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_SpareByteCnt;
            _nf_info.u16ECCType = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_ECCType;

            break;
        }

        row_pos += 32;
    }
    REG_CLR_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);

    // soft reset
    _nf_info.u16SpareByteCnt = _nf_info.u16SpareByteCnt_BK;
    LDR_NAND_SoftReset();

    MS_U16* MIUPos;
    MS_U32 BINSize;
    MS_U32 Entry_Offset = 0;
    MS_U32 i=0;
    row_pos = 10*_nf_info.u16BlockPageCnt;
    while(1)
    {
        LDR_PUTDW(row_pos);
        LDR_NAND_ReadPage(row_pos,_nf_info.pBuf,1);

        if (*((MS_U32*)(_nf_info.pBuf)) == BINIDTAG)
        {
            MIUPos = (MS_U16*)(*((MS_U32*)(_nf_info.pBuf+0x2)));//MIU pos is the loading position and the jump offset
            BINSize = *((MS_U32*)(_nf_info.pBuf+0xa));
            Entry_Offset = *((MS_U32*)(_nf_info.pBuf+0x12));

            break;
        }
        row_pos += _nf_info.u16BlockPageCnt;
    }

    pfEntry = (PF_AP_Entry)(MIUPos)+ Entry_Offset;

    i=0;
    while(BINSize)
    {
        LDR_NAND_ReadPage(row_pos+i, MIUPos, _nf_info.u16PageByteCnt>>9);
        //LDR_PUTDW(*((MS_U32*)(_nf_info.pBuf)));
        if (i==0)
        {
            if (*((MS_U32*)(MIUPos)) != BINIDTAG)
            {
                row_pos += _nf_info.u16BlockPageCnt;
            }
            else
            {
                i++;
            }

            continue;
        }

        i++;
        MIUPos += (_nf_info.u16PageByteCnt>>1);
        BINSize -= ((BINSize < _nf_info.u16PageByteCnt)?BINSize:_nf_info.u16PageByteCnt);
        if (i==_nf_info.u16BlockPageCnt)
        {
            row_pos += _nf_info.u16BlockPageCnt;
            i=0;
        }
    }

    LDR_PadConfig();

    if (1)
    {
        extern void DoMSDecompression(U32 addr);

        //DoMSDecompression(decompress_pos); // todo: to handle compress format

        flush_cache();

        pfEntry();
    }

}
#else
void LDR_Entry_NAND(void)
{
}
#endif // bfn

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
    // if we dont plan to use OTP BFN, no necessary for checking SPI
    // Switch SPI to XIU mode
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);//_HAL_ISP_Disable();

    //_HAL_SPI_Rest(); // to prevent BDMA fail
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);
    return 1;
#endif
}

void remap_dspram(void)
{
    void remap_dspram_asm(void);
    asm volatile(
            "jal remap_dspram_asm\n"
            "nop\n" : : :
            "$0","$1", "$2", "$3", "$4", "$5", "$6", "$7",
            "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
            "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
            "$24", "$25", "$26", "$27", "$29", "$31",
            "memory");
}

void *memcpy(void *dst, const void *src, size_t n)
{
    char *pdst = (char *)dst;
    const char *psrc = (const char *)src;

    for(; n > 0; ++pdst, ++psrc, --n)
    {
        *pdst = *psrc;
    }

    return (dst);
}

extern void LDR_Entry(void)                                             _SECTION_LDR_ENTRY_;
void LDR_Entry(void)
{
#ifdef CONFIG_NO_2ED_LOADER
    PUTC('\r'); PUTC('\n');
    PUTC('T'); PUTC('E'); PUTC('S'); PUTC('T'); PUTC('_');
    PUTC('L'); PUTC('D'); PUTC('R'); PUTC('_'); PUTC('O'); PUTC('K');
    PUTC('\r'); PUTC('\n');

    while(1) {
        // do nothing
    }

#else

    #if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA)
    //patch for ca otp boot chip, in otp the spi clk is low
    WREG16(REG_ADDR_BASE + (0x000e40 << 1),0x5098);
    #endif

    //note if l2 is not enable in this time
    //below function should not access l2 functions
    remap_dspram();

#if (CONFIG_Second_UART_Mode_1 || CONFIG_Second_UART_Mode_2  || CONFIG_Second_UART_Mode_3 ||\
    CONFIG_Third_UART_Mode_1 || CONFIG_Third_UART_Mode_2  || CONFIG_Third_UART_Mode_3 ||\
    CONFIG_Fourth_UART_Mode_1 || CONFIG_Fourth_UART_Mode_2  || CONFIG_Fourth_UART_Mode_3)
    _UART_Reset();
#endif
#if  ((!defined(CONFIG_MSTAR_KELTIC)) && (!defined(CONFIG_MSTAR_KAPPA)) && (!defined(CONFIG_MSTAR_KRITI)))

    if (LDR_SERFLASH_ReadID())
    {
        LDR_Entry_SPI();
    }
    else
    {
        LDR_Entry_NAND();
    }
#else
    LDR_Entry_SPI();
#endif
#endif
}
