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

#include <ShareType.h>
#include <MsUtility.h>
#include <MsApiMiu.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
#include <MsSystem.h>
#include <drvPM.h>
#include <common.h>
#include <CusPM.h>
#include <MsApiPM.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <drvBDMA.h>
#include <miu/MsDrvMiu.h>
#include <str/MsDrvStr.h>
#include <MsStr.h>
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
#include <asm/armv7.h>
#endif

#if CONFIG_MSTAR_STR_ENABLE
#define BIT0 0x01
#define BIT3 0x08
#define BDMA_BANK 0x1f201200
#define MEM_REGION1_ST 0x00200000
#define MEM_REGION1_SZ 0x9F00000
#define MEM_REGION2_ST 0x10E00000
#define MEM_REGION2_SZ 0xC200000

#define MB_BANK 0x1F206600
#define PM_SLEEP 0x1F001C00

#define Check_STR_CRC 0x1F002090 // 0x1048

extern BOOLEAN get_poweroff_flag(void);

void HAL_MBOOT_Write2Byte( unsigned long u32Reg, unsigned short u16Val )
{
    *((volatile unsigned long *)u32Reg)=u16Val;
}
unsigned short HAL_MBOOT_Read2Byte( unsigned long u32Reg )
{
    return (unsigned short)(*(volatile unsigned long *)u32Reg);
}
unsigned long BDMACRCCalc(int miu, unsigned long u32Addr, unsigned long u32Len)
{
    UBOOT_TRACE("IN\n");
    unsigned long u32CRC = 0;
    unsigned long u32Timeout = 0;

    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x0c * 4, 0);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x0d * 4, 0);

    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x02 * 4, 0x340);

	if (miu == 0)
	{
		HAL_MBOOT_Write2Byte(BDMA_BANK + 0x03 * 4, 0x4000);    // miu 0
	}
	else if (miu == 1)
	{
		HAL_MBOOT_Write2Byte(BDMA_BANK + 0x03 * 4, 0x5000);    // miu 1
	}
	else if (miu == 2)
	{
		HAL_MBOOT_Write2Byte(BDMA_BANK + 0x03 * 4, 0x6000);
	}
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x04 * 4, u32Addr & 0x0FFFF);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x05 * 4, (u32Addr >> 16) & 0x0FFFF);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x06 * 4, u32Addr & 0x0FFFF);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x07 * 4, (u32Addr >> 16) & 0x0FFFF);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x08 * 4, u32Len & 0x0FFFF);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x09 * 4, (u32Len >> 16) & 0x0FFFF);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x0a * 4, 0x1DB7);
    HAL_MBOOT_Write2Byte(BDMA_BANK + 0x0b * 4, 0x04c1);
    HAL_MBOOT_Write2Byte(BDMA_BANK, HAL_MBOOT_Read2Byte(BDMA_BANK) | BIT0);
    while (1)
    {
        if ((HAL_MBOOT_Read2Byte(BDMA_BANK + 0x01 * 4) & BIT3) == 0x08)
            break;

        if (u32Timeout > 0xA00000)
        {
            printf("TIME OUT BDAM 0x%x \n",HAL_MBOOT_Read2Byte(BDMA_BANK + (0x01*4)));
            printf("----CRC TIME OUT ----\n");

            break;
        }
        udelay(10000);
        u32Timeout++;
    }
    u32CRC = HAL_MBOOT_Read2Byte(BDMA_BANK+0x0d*4);
    u32CRC = ((u32CRC << 16) | HAL_MBOOT_Read2Byte(BDMA_BANK + 0x0c * 4));
    UBOOT_TRACE("OK\n");
    return u32CRC;
}

void CRCCheck(void)
{
    char *pEnv = NULL;
    unsigned char crclevel = 0;
    char *startp = NULL, *endp = NULL;
    U16 STR_CRC_num = 0;
    int len = 0;
    unsigned long lx_mem_addr = 0, lx_mem_size = 0 , lx_mem2_addr = 0, lx_mem2_size = 0 , lx_mem3_addr = 0, lx_mem3_size = 0;
    unsigned long crc_result_1 = 0, crc_result_2 = 0, crc_result_3 = 0, pm_crc_1 = 0, pm_crc_2 = 0, pm_crc_3 = 0;
    int i;

    UBOOT_TRACE("IN\n");
    pEnv = getenv("str_crc");
    crclevel=simple_strtol(pEnv, NULL, 16);
    if (pEnv == NULL || crclevel == 3)
    {
	lx_mem_addr        = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc0 * 2) * 0x2000;
	lx_mem2_addr       = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc4 * 2) * 0x2000;
	lx_mem3_addr       = HAL_MBOOT_Read2Byte(0x1F200c00 + 0xc0 * 2) * 0x2000;
	lx_mem_size        = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc2 * 2) * 0x2000 - lx_mem_addr;
	lx_mem2_size       = HAL_MBOOT_Read2Byte(0x1F202400 + 0xc6 * 2) * 0x2000 - lx_mem2_addr;
	lx_mem3_size       = HAL_MBOOT_Read2Byte(0x1F200c00 + 0xc2 * 2) * 0x2000 - lx_mem3_addr;

	HAL_MBOOT_Write2Byte(0x1F200C00 + 0xF0*2, HAL_MBOOT_Read2Byte(0x1F200C00 + 0xF0*2) | 0x4000);
	crc_result_1 = BDMACRCCalc(0,lx_mem_addr, lx_mem_size);
	crc_result_2 = BDMACRCCalc(0,lx_mem2_addr, lx_mem2_size);
	crc_result_3 = BDMACRCCalc(1,lx_mem3_addr, lx_mem3_size);

	printf("\033[31m########################## \n");
	printf("MemRegion[%08lx,%08lx] CRC is %08lx \n", lx_mem_addr, lx_mem_size, crc_result_1);
	printf("MemRegion[%08lx,%08lx] CRC is %08lx \n", lx_mem2_addr, lx_mem2_size, crc_result_2);
	printf("MemRegion[%08lx,%08lx] CRC is %08lx \n", lx_mem3_addr, lx_mem3_size, crc_result_3);
	printf("########################## \033[0m\n");
    }
    UBOOT_TRACE("OK\n");
}

#define MSTAR_SLEEP_MAGIC         0x4D535452

#if (CONFIG_MIPS32 == 1)
#define PMU_WAKEUP_ADDR_REGL     0xBF001CC8
#define PMU_WAKEUP_ADDR_REGH     0xBF001CC8
#else
#define PMU_WAKEUP_ADDR_REGL     0x1F001D48
#define PMU_WAKEUP_ADDR_REGH     0x1F001D48
#endif
#define PMU_WAKEUP_ADDR_LMASK    0x000000FF
#define PMU_WAKEUP_ADDR_HMASK    0x0000FF00


#define WAKEUP_ADDR_MASK          0x0000FFF0
#define WAKEUP_FLAG_MASK          0x0000000F
#define WAKEUP_ADDR_SHIFT         16
#define WAKEUP_FLAG_INVALID       0
#define WAKEUP_FLAG_SLEPT         1
#define WAKEUP_FLAG_WKUP          2

#if CONFIG_MCU_MIPS32
#define MIU0_VIRTBASE1   0x80000000
#define MIU1_BASE_ADDR1  0x60000000
#define MIU1_BASE_ADDR2  0x70000000
#define MIU1_VIRTBASE1   0x00000000
#define MIU1_VIRTBASE2   0x20000000
#define _PHY_TO_VIRT(x) ((x<MIU1_BASE_ADDR1)?(x+MIU0_VIRTBASE1):    \
                          ((x<MIU1_BASE_ADDR2)?(x-MIU1_BASE_ADDR1+MIU1_VIRTBASE1):(x-MIU1_BASE_ADDR2+MIU1_VIRTBASE2)) )
#elif CONFIG_MCU_ARM
#define M512M (512*1024*1024)
#define _PHY_TO_VIRT(x) ( (((x)>=(CONFIG_MIU0_BUSADDR)&&(x)<(CONFIG_MIU0_BUSADDR+M512M)) \
                            ||((x)>=(CONFIG_MIU1_BUSADDR)&&(x)<(CONFIG_MIU1_BUSADDR+M512M)))? \
                            (x):(0) )
#else
#define _PHY_TO_VIRT(x) (x)
#endif
#define PHY_TO_VIRT(x) _PHY_TO_VIRT((unsigned long)(x))

unsigned long read_wkup_pmu(void)
{
    volatile unsigned long *preg=0;
    unsigned long val=0;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    val = ((*preg)&PMU_WAKEUP_ADDR_LMASK);
    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    val |= ((*preg)&PMU_WAKEUP_ADDR_HMASK);

    return val;
}
void write_wkup_pmu(unsigned long val)
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

int _is_str_resume(void)
{
    unsigned long pmu_str_reg=0,*pwkupsave=0;
    pmu_str_reg=read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<WAKEUP_ADDR_SHIFT);
#if CONFIG_MCU_MIPS32
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
#endif
    if((pmu_str_reg & WAKEUP_FLAG_MASK) == WAKEUP_FLAG_SLEPT && pwkupsave
    	&& pwkupsave[0]==MSTAR_SLEEP_MAGIC && pwkupsave[1])
    {
		return 1;
    }
    return 0;
}

int check_str_resume(void)
{
    if(!(MDrv_PM_PowerOnMode() == E_PM_POWERON_ACON) &&
        (_is_str_resume()==1))
    {
        CRCCheck();
        return 1;
    }

    return 0;
}

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
static void do_nonsec_virt_switch(void)
{
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
	if (armv7_switch_nonsec() == 0)
#ifdef CONFIG_ARMV7_VIRT
	if (armv7_switch_hyp() == 0)
		debug("entered HYP mode\n");
#else
	debug("entered non-secure state\n");
#endif
#endif
}
#endif

int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    unsigned long pmu_str_reg=0,*pwkupsave=0;
    char* s = NULL;


    pmu_str_reg=read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<16);

#if CONFIG_MCU_MIPS32
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
#endif
    if(_is_str_resume())
    {
        s = getenv("KERNEL_PROTECT");
        if (s != NULL)
        {
            MsApi_kernelProtect();
        }
		check_str_resume();

        write_wkup_pmu((pmu_str_reg&~WAKEUP_FLAG_MASK)|WAKEUP_FLAG_WKUP);
        printf("[AT][MB][jump to kernel][%lu]\n", MsSystemGetBootTime());
        MsDrv_StrSetGICInterface();
#if CONFIG_MCU_MIPS32
        {
            void (*resume)(void);
            resume = (void *)PHY_TO_VIRT(pwkupsave[1]);
            if(resume)
            resume();
        }
#elif CONFIG_MCU_ARM
    cleanup_before_linux();
#if CONFIG_ARMv8_64BIT_KERNEL

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	printf("\033[0;32;31m [Ian] %s %d ARMv8 Trustzone STR flow\033[m\n",__func__,__LINE__);

	__asm__ __volatile__(
	"ldr  r0, =0x8400000F\n\t"
	"ldr  r1, =0x0\n\t"
	"ldr  r2, =0x0\n\t"
	"ldr  r3, =0x0\n\t"
	"ldr  r4, =0x0\n\t"
	"smc #0         \n\t"
	::
	: "r0","r3","r4","r2"
	);
#else
	__asm__ __volatile__(
		"ldr r0, [%0]\n"
		"smc #0\n"
		::"r"(&pwkupsave[1]):"r0");
#endif //CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE

#else

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)

		do_nonsec_virt_switch();
#endif
        __asm__ volatile (
            "ldr r1, %0\n"
            "ldr r1, [r1, #4]\n"
            "cpsid if, #0x13\n"
            //; Disable MMU
            "MRC   p15, 0, r0, c1, c0, 0\n"
            "BIC   r0, r0, #0x1\n"
            "MCR   p15, 0, r0, c1, c0, 0\n"
            //; jump to wakeup address
            "mov pc, r1\n"
            ::"m"(pwkupsave):"r0","r1","memory","cc");
#endif
#endif
    }
    else
    {
        write_wkup_pmu(0);
    }
    return 0;
}



#endif
