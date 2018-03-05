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
/// file    bootrom_c.c
/// @brief  C boot rom code function
/// @author MStar Semiconductor Inc.
/// @note   Rom code functions is running on read-only memory (ROM), all function should follow the rules. \n
///         (1) source code should be standalone
///         (2) all functions are running in one thread
///         (3) all variables should be read only except local varaibles in the stack or register.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boot.h"
#include "chip.h"
#include "Board.h"
//#include "bootrom_nand.h"
#include "bootrom_spi.h"

MS_U64 _var_LDR_run_base __attribute__((section(".bss"))) __attribute__ ((aligned (0x10)));
MS_U32 _var_LDR_size __attribute__((section(".bss")));
MS_U64 _var_LDR_entry_point __attribute__((section(".bss")));
MS_U64 _var_LDR_stack_end __attribute__((section(".bss")));

MS_U32 _boot_flow_mode = BOOT_FLOW_IS_NONE ;

void DelayMS(MS_U32 msec)
{
    MS_U64 u64val1;
    MS_U64 u64val2;
    MS_U64 u64Interval;

    u64Interval = 12000*msec;
    asm volatile("mrs %0, cntpct_el0" : "=r" (u64val1));

    while(1) {
        asm volatile("mrs %0, cntpct_el0" : "=r" (u64val2));
        if ((u64val2-u64val1) >= u64Interval)
            break;
    }
}

void _PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

void _PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            _PUTC('\r');
        _PUTC(*s++);
    }
}

void _PUTX(char val)
{
    if (val > 0x9)
        _PUTC('A' - 10 + val);
    else
        _PUTC('0' + val);
}

void _PUTW(MS_U16 val)
{
    _PUTX((val >> 12) & 0xF);
    _PUTX((val >> 8) & 0xF);
    _PUTX((val >> 4) & 0xF);
    _PUTX(val & 0xF);
}

void _PUTDW(MS_U32 val)
{
    _PUTW((val >> 16) & 0xFFFF);
    _PUTW(val & 0xFFFF);
}

void BOOTROM_C_GetHashOneInfo(const MS_U32 u32HASH1[])
{
    _var_LDR_run_base = (MS_U64)u32HASH1[HASH1_PARAM_LEN];
    _var_LDR_size = u32HASH1[HASH1_PARAM_LEN+1];
}

void BOOTROM_C_InitDramMOBF(void)
{
    //Do Nothing
}

void BOOTROM_C_InitMIU_SPI(void)
{
    extern MS_U8 _ld_HASH1_run_start[];

    BOOTROM_C_GetHashOneInfo((MS_U32 *)_ld_HASH1_run_start);
    BOOTROM_C_InitParams(_ld_HASH1_run_start);

    BOOTROM_C_InitDramMOBF();
}

void BOOTROM_C_GetHashTwoInfo(void)
{
    MS_U64 header = _var_LDR_run_base;
    MS_U32 entry_offset = *(MS_U32*)(header + 0x4);
    _var_LDR_entry_point = (header + entry_offset); // entry_address
    _var_LDR_stack_end   = *(MS_U32*)(_var_LDR_run_base + 0x8);
}

static MS_U32 _HAL_SERFLASH_WaitReadDataRdy(void)
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

static MS_U32 _HAL_SERFLASH_WaitWriteDone(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*5);
    do
    {
        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if (_HAL_SERFLASH_WaitReadDataRdy() == 0 )
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

MS_U32 HAL_SERFLASH_ReadID()
{
#ifdef CONFIG_LD_BOOT_FROM_OTP
    MS_U32 bRet = 0;
    MS_U32 u32I;
    MS_U32 u32Size;
    MS_U8 u8ptr ;

    ISP_WRITE(REG_ISP_PASSWORD, 0xAAAA);//_HAL_ISP_Enable();

    if (!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_ReadID_return;
    }
    bRet = 1;

HAL_SERFLASH_ReadID_return:
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);//_HAL_ISP_Disable();

    //_HAL_SPI_Rest(); // to prevent BDMA fail
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);
    return bRet;
#else
    // if we dont plan to use OTP, no necessary for SPI boot
    return 1;
#endif

}
void BOOTROM_C_InitMIU_ENTRY(void)
{
    if (HAL_SERFLASH_ReadID())
    {
        _PUTS("SPI\n");
        _boot_flow_mode = BOOT_FLOW_IS_SPI;
        BOOTROM_C_InitMIU_SPI();
    }
}

