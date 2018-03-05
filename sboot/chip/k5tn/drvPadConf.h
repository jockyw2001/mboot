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

#include <stdio.h>

#include "datatype.h"
#include "drvRIU.h"
#include "Board.h"
#include "hwreg_K5TN.h"
#include "chip/bond.h"

#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high


#define _MEMMAP_ADDR(address)  _RVM1(0x0000, (MS_U8)address, 0xFF)
#define _MEMMAP_nonPM_  _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_PM_     _RVM1(0x0000, 0x00, 0xFF)

#define _END_OF_TBL2_       0xFF, 0xFF

const U8 padInitTbl[] =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
    1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
    0x00,                       // nor
    0x00,                       // nand
    0x00,                       // reserved
    0x00,                       // reserved
    0x00,                       // reserved
//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------
    _MEMMAP_nonPM_,

#if !(defined(ENABLE_MBOOT_FPGA) && (ENABLE_MBOOT_FPGA == 1))
 
 // Script Generated GPIO
    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a2, PAD_S_GPIO0_OUT, BIT4),
    _RVM1(0x25a2, PAD_S_GPIO0_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),        //Reg[101ea1]#7=0
    //reg_spdif_out_mode=0
    _RVM1(0x1e0a, 0, BIT5|BIT4),   //Reg[101e0a]#5~#4=0
    #endif
    
#if (defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266E_D01A))
    #if(PAD_S_GPIO0_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO0_QFP_OEN (PAD_S_GPIO0_QFP_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO0_QFP_OUT (PAD_S_GPIO0_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x262e, PAD_S_GPIO0_QFP_OUT, BIT4),
    _RVM1(0x262e, PAD_S_GPIO0_QFP_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_i2s_out_mode=0
    _RVM1(0x1e0b, 0, BIT4|BIT3),   //Reg[101e0b]#4~#3=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif
#endif

    #if(PAD_S_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO1_OEN (PAD_S_GPIO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO1_OUT (PAD_S_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2630, PAD_S_GPIO1_OUT, BIT4),
    _RVM1(0x2630, PAD_S_GPIO1_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_i2s_in_mode=0
    _RVM1(0x1e0b, 0, BIT0),   //Reg[101e0b]#0=0
    //reg_i2s_trx_mode=0
    _RVM1(0x1e0b, 0, BIT5),   //Reg[101e0b]#5=0
    //reg_i2s_out_mode=0
    _RVM1(0x1e0b, 0, BIT4|BIT3),   //Reg[101e0b]#4~#3=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_EJ_TCK_IS_GPIO != GPIO_NONE)
    #define PAD_EJ_TCK_OEN (PAD_EJ_TCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EJ_TCK_OUT (PAD_EJ_TCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2632, PAD_EJ_TCK_OUT, BIT4),
    _RVM1(0x2632, PAD_EJ_TCK_OEN, BIT5),
    //reg_gpio2_pe=1
    _RVM1(0x1e4e, BIT4, BIT4),   //Reg[101e4e]#4=1
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_i2s_in_mode=0
    _RVM1(0x1e0b, 0, BIT0),   //Reg[101e0b]#0=0
    //reg_i2s_trx_mode=0
    _RVM1(0x1e0b, 0, BIT5),   //Reg[101e0b]#5=0
    //reg_i2s_out_mode=0
    _RVM1(0x1e0b, 0, BIT4|BIT3),   //Reg[101e0b]#4~#3=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_EJ_TMS_IS_GPIO != GPIO_NONE)
    #define PAD_EJ_TMS_OEN (PAD_EJ_TMS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EJ_TMS_OUT (PAD_EJ_TMS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2634, PAD_EJ_TMS_OUT, BIT4),
    _RVM1(0x2634, PAD_EJ_TMS_OEN, BIT5),
    //reg_gpio4_pe=1
    _RVM1(0x1e4f, BIT4, BIT4),   //Reg[101e4f]#4=1
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_spdif_in_mode=0
    _RVM1(0x1e60, 0, BIT1|BIT0),   //Reg[101e60]#1~#0=0
    //reg_i2s_trx_mode=0
    _RVM1(0x1e0b, 0, BIT5),   //Reg[101e0b]#5=0
    //reg_i2s_out_mode=0
    _RVM1(0x1e0b, 0, BIT4|BIT3),   //Reg[101e0b]#4~#3=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266E_D01A))
    #if(PAD_S_GPIO4_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO4_QFP_OEN (PAD_S_GPIO4_QFP_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO4_QFP_OUT (PAD_S_GPIO4_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2636, PAD_S_GPIO4_QFP_OUT, BIT4),
    _RVM1(0x2636, PAD_S_GPIO4_QFP_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_i2s_out_mode=0
    _RVM1(0x1e0b, 0, BIT4|BIT3),   //Reg[101e0b]#4~#3=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    #endif
#endif

    #if(PAD_EJ_TDO_IS_GPIO != GPIO_NONE)
    #define PAD_EJ_TDO_OEN (PAD_EJ_TDO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EJ_TDO_OUT (PAD_EJ_TDO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b0, PAD_EJ_TDO_OUT, BIT4),
    _RVM1(0x25b0, PAD_EJ_TDO_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_bist_combo_gcr_bist_en=0
    _RVM1(0x1e89, 0, BIT2),   //Reg[101e89]#2=0
    #endif

    #if(PAD_EJ_TDI_IS_GPIO != GPIO_NONE)
    #define PAD_EJ_TDI_OEN (PAD_EJ_TDI_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EJ_TDI_OUT (PAD_EJ_TDI_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b2, PAD_EJ_TDI_OUT, BIT4),
    _RVM1(0x25b2, PAD_EJ_TDI_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_i2s_in_mode=0
    _RVM1(0x1e0b, 0, BIT0),   //Reg[101e0b]#0=0
    //reg_i2s_trx_mode=0
    _RVM1(0x1e0b, 0, BIT5),   //Reg[101e0b]#5=0
    //reg_i2s_out_mode=0
    _RVM1(0x1e0b, 0, BIT4|BIT3),   //Reg[101e0b]#4~#3=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_bist_combo_gcr_bist_en=0
    _RVM1(0x1e89, 0, BIT2),   //Reg[101e89]#2=0
    #endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_SM0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CLK_OEN (PAD_SM0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CLK_OUT (PAD_SM0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2502, PAD_SM0_CLK_OUT, BIT4),
    _RVM1(0x2502, PAD_SM0_CLK_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_sm0_open=0
    _RVM1(0x1e18, 0, BIT0),   //Reg[101e18]#0=0
    //reg_sm0_en=0
    _RVM1(0x1e19, 0, BIT0),   //Reg[101e19]#0=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif

    #if(PAD_SM0_RST_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_RST_OEN (PAD_SM0_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_RST_OUT (PAD_SM0_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2504, PAD_SM0_RST_OUT, BIT4),
    _RVM1(0x2504, PAD_SM0_RST_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_sm0_open=0
    _RVM1(0x1e18, 0, BIT0),   //Reg[101e18]#0=0
    //reg_sm0_en=0
    _RVM1(0x1e19, 0, BIT0),   //Reg[101e19]#0=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif
#endif

    #if(PAD_SM0_C4_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_C4_OEN (PAD_SM0_C4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_C4_OUT (PAD_SM0_C4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250a, PAD_SM0_C4_OUT, BIT4),
    _RVM1(0x250a, PAD_SM0_C4_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ocp0_en=0
    _RVM1(0x1e1c, 0, BIT0),   //Reg[101e1c]#0=0
    //reg_sm0_c48=0
    _RVM1(0x1e18, 0, BIT5|BIT4),   //Reg[101e18]#5~#4=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif

    #if(PAD_SM0_IO_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_IO_OEN (PAD_SM0_IO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_IO_OUT (PAD_SM0_IO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2500, PAD_SM0_IO_OUT, BIT4),
    _RVM1(0x2500, PAD_SM0_IO_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_sm0_io=0
    _RVM1(0x1e19, 0, BIT4),   //Reg[101e19]#4=0
    //reg_sm0_en=0
    _RVM1(0x1e19, 0, BIT0),   //Reg[101e19]#0=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif

    #if(PAD_SM0_C8_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_C8_OEN (PAD_SM0_C8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_C8_OUT (PAD_SM0_C8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250c, PAD_SM0_C8_OUT, BIT4),
    _RVM1(0x250c, PAD_SM0_C8_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_sm0_open=0
    _RVM1(0x1e18, 0, BIT0),   //Reg[101e18]#0=0
    //reg_ocp0_en=0
    _RVM1(0x1e1c, 0, BIT0),   //Reg[101e1c]#0=0
    //reg_sm0_c48=0
    _RVM1(0x1e18, 0, BIT5|BIT4),   //Reg[101e18]#5~#4=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif

    #if(PAD_SM0_CD_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CD_OEN (PAD_SM0_CD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CD_OUT (PAD_SM0_CD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2506, PAD_SM0_CD_OUT, BIT4),
    _RVM1(0x2506, PAD_SM0_CD_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_sm0_en=0
    _RVM1(0x1e19, 0, BIT0),   //Reg[101e19]#0=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif

    #if(PAD_SM0_GPIO_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_GPIO_OEN (PAD_SM0_GPIO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_GPIO_OUT (PAD_SM0_GPIO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250d, PAD_SM0_GPIO_OUT, BIT4),
    _RVM1(0x250d, PAD_SM0_GPIO_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_spdif_in_mode=0
    _RVM1(0x1e0a, 0, BIT1|BIT0),   //Reg[101e0a]#1~#0=0
    //reg_sm0_open=0
    _RVM1(0x1e18, 0, BIT0),   //Reg[101e18]#0=0
    //reg_sm0_en=0
    _RVM1(0x1e19, 0, BIT0),   //Reg[101e19]#0=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    #endif

    #if(PAD_SM0_PCT_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_PCT_OEN (PAD_SM0_PCT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_PCT_OUT (PAD_SM0_PCT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2508, PAD_SM0_PCT_OUT, BIT4),
    _RVM1(0x2508, PAD_SM0_PCT_OEN, BIT5),
    //reg_sm0_gpio_en=1
    _RVM1(0x1e51, BIT0, BIT0),   //Reg[101e51]#0=1
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ocp0_en=0
    _RVM1(0x1e1c, 0, BIT0),   //Reg[101e1c]#0=0
    //reg_sm0_en=0
    _RVM1(0x1e19, 0, BIT0),   //Reg[101e19]#0=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    _RVM1(0x294e, BIT5|BIT4, BIT5|BIT4),   //Reg[10294e]#5~#4=1
    _RVM1(0x294e, 0, BIT3|BIT2),   //Reg[10294e]#3~#2=0
    #endif

    //QFP_T2 BGA
#if !(defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266E_D01A))
    #if(PAD_T_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_T_GPIO3_OEN (PAD_T_GPIO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_T_GPIO3_OUT (PAD_T_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e6, PAD_T_GPIO3_OUT, BIT4),
    _RVM1(0x25e6, PAD_T_GPIO3_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_diseqc_mode=0
    _RVM1(0x1e05, 0, BIT5|BIT4),   //Reg[101e05]#5~#4=0
    #endif

    #if(PAD_T_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_T_GPIO4_OEN (PAD_T_GPIO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_T_GPIO4_OUT (PAD_T_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e8, PAD_T_GPIO4_OUT, BIT4),
    _RVM1(0x25e8, PAD_T_GPIO4_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_diseqc_mode=0
    _RVM1(0x1e05, 0, BIT5|BIT4),   //Reg[101e05]#5~#4=0
    #endif
#endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_FUART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_RX_OEN (PAD_FUART_RX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_RX_OUT (PAD_FUART_RX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a8, PAD_FUART_RX_OUT, BIT4),
    _RVM1(0x25a8, PAD_FUART_RX_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_FUART_TX_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_TX_OEN (PAD_FUART_TX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_TX_OUT (PAD_FUART_TX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25aa, PAD_FUART_TX_OUT, BIT4),
    _RVM1(0x25aa, PAD_FUART_TX_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_FUART_CTS_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_CTS_OEN (PAD_FUART_CTS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_CTS_OUT (PAD_FUART_CTS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ac, PAD_FUART_CTS_OUT, BIT4),
    _RVM1(0x25ac, PAD_FUART_CTS_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_FUART_RTS_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_RTS_OEN (PAD_FUART_RTS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_RTS_OUT (PAD_FUART_RTS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ae, PAD_FUART_RTS_OUT, BIT4),
    _RVM1(0x25ae, PAD_FUART_RTS_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_mspi_mode = 0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b4, PAD_GPIO2_OUT, BIT4),
    _RVM1(0x25b4, PAD_GPIO2_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b6, PAD_GPIO3_OUT, BIT4),
    _RVM1(0x25b6, PAD_GPIO3_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b8, PAD_GPIO4_OUT, BIT4),
    _RVM1(0x25b8, PAD_GPIO4_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_sd_rst_wp_en=0
    _RVM1(0x39c8, 0, BIT4),   //Reg[1239c8]#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_rstn_en=0
    _RVM1(0x1e6c, 0, BIT5),   //Reg[101e6c]#5=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ba, PAD_GPIO5_OUT, BIT4),
    _RVM1(0x25ba, PAD_GPIO5_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO6_OEN (PAD_GPIO6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO6_OUT (PAD_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25bc, PAD_GPIO6_OUT, BIT4),
    _RVM1(0x25bc, PAD_GPIO6_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_GPIO9_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO9_OEN (PAD_GPIO9_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO9_OUT (PAD_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c2, PAD_GPIO9_OUT, BIT4),
    _RVM1(0x25c2, PAD_GPIO9_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    #endif

    #if(PAD_GPIO10_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO10_OEN (PAD_GPIO10_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO10_OUT (PAD_GPIO10_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c4, PAD_GPIO10_OUT, BIT4),
    _RVM1(0x25c4, PAD_GPIO10_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    #endif

    #if(PAD_GPIO11_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO11_OEN (PAD_GPIO11_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO11_OUT (PAD_GPIO11_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c6, PAD_GPIO11_OUT, BIT4),
    _RVM1(0x25c6, PAD_GPIO11_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    #endif

    #if(PAD_GPIO12_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO12_OEN (PAD_GPIO12_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO12_OUT (PAD_GPIO12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c8, PAD_GPIO12_OUT, BIT4),
    _RVM1(0x25c8, PAD_GPIO12_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    #endif

    #if(PAD_GPIO13_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO13_OEN (PAD_GPIO13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO13_OUT (PAD_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ca, PAD_GPIO13_OUT, BIT4),
    _RVM1(0x25ca, PAD_GPIO13_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_GPIO14_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO14_OEN (PAD_GPIO14_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO14_OUT (PAD_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25cc, PAD_GPIO14_OUT, BIT4),
    _RVM1(0x25cc, PAD_GPIO14_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_GPIO15_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO15_OEN (PAD_GPIO15_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO15_OUT (PAD_GPIO15_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ce, PAD_GPIO15_OUT, BIT4),
    _RVM1(0x25ce, PAD_GPIO15_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_GPIO16_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO16_OEN (PAD_GPIO16_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO16_OUT (PAD_GPIO16_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25d0, PAD_GPIO16_OUT, BIT4),
    _RVM1(0x25d0, PAD_GPIO16_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    #endif

    #if(PAD_CI_A6_IS_GPIO != GPIO_NONE)   //PAD_CTRL
    #define PAD_CI_A6_OEN (PAD_CI_A6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A6_OUT (PAD_CI_A6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x256a, PAD_CI_A6_OUT, BIT4),
    _RVM1(0x256a, PAD_CI_A6_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    #endif

    #if(PAD_CI_A0_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A0_OEN (PAD_CI_A0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A0_OUT (PAD_CI_A0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2576, PAD_CI_A0_OUT, BIT4),
    _RVM1(0x2576, PAD_CI_A0_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_sm1_en=0
    _RVM1(0x1e31, 0, BIT0),   //Reg[101e31]#0=0
    #endif

    #if(PAD_CI_A2_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A2_OEN (PAD_CI_A2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A2_OUT (PAD_CI_A2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2572, PAD_CI_A2_OUT, BIT4),
    _RVM1(0x2572, PAD_CI_A2_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode2=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_A7_IS_GPIO != GPIO_NONE)   //PAD_SDLDO_VOUT_FB
    #define PAD_CI_A7_OEN (PAD_CI_A7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A7_OUT (PAD_CI_A7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2568, PAD_CI_A7_OUT, BIT4),
    _RVM1(0x2568, PAD_CI_A7_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_pd_smc_pad=0
    _RVM1(0x2958, 0, BIT6),   //Reg[102958]#6=0
    #endif

    #if(PAD_CI_A8_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A8_OEN (PAD_CI_A8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A8_OUT (PAD_CI_A8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2566, PAD_CI_A8_OUT, BIT4),
    _RVM1(0x2566, PAD_CI_A8_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO7_OEN (PAD_GPIO7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO7_OUT (PAD_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25be, PAD_GPIO7_OUT, BIT4),
    _RVM1(0x25be, PAD_GPIO7_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_hsync_en=0
    _RVM1(0x1e7e, 0, BIT0),   //Reg[101e7e]#0=0
    //reg_vsync_en=0
    _RVM1(0x1e7e, 0, BIT1),   //Reg[101e7e]#1=0
    #endif

    #if(PAD_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO8_OEN (PAD_GPIO8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO8_OUT (PAD_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c0, PAD_GPIO8_OUT, BIT4),
    _RVM1(0x25c0, PAD_GPIO8_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_hsync_en=0
    _RVM1(0x1e7e, 0, BIT0),   //Reg[101e7e]#0=0
    //reg_vsync_en=0
    _RVM1(0x1e7e, 0, BIT1),   //Reg[101e7e]#1=0
    #endif

    #if(PAD_CI_A12_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A12_OEN (PAD_CI_A12_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A12_OUT (PAD_CI_A12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255e, PAD_CI_A12_OUT, BIT4),
    _RVM1(0x255e, PAD_CI_A12_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_smx_io=0
    _RVM1(0x1e1b, 0, BIT4),   //Reg[101e1b]#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_A14_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A14_OEN (PAD_CI_A14_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A14_OUT (PAD_CI_A14_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255a, PAD_CI_A14_OUT, BIT4),
    _RVM1(0x255a, PAD_CI_A14_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_smx_open=0
    _RVM1(0x1e1a, 0, BIT0),   //Reg[101e1a]#0=0
    //reg_smx_en=0
    _RVM1(0x1e1b, 0, BIT0),   //Reg[101e1b]#0=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_CDZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_CDZ_OEN (PAD_CI_CDZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_CDZ_OUT (PAD_CI_CDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259a, PAD_CI_CDZ_OUT, BIT4),
    _RVM1(0x259a, PAD_CI_CDZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_smx_en=0
    _RVM1(0x1e1b, 0, BIT0),   //Reg[101e1b]#0=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_D3_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D3_OEN (PAD_CI_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D3_OUT (PAD_CI_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2580, PAD_CI_D3_OUT, BIT4),
    _RVM1(0x2580, PAD_CI_D3_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_D4_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D4_OEN (PAD_CI_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D4_OUT (PAD_CI_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257e, PAD_CI_D4_OUT, BIT4),
    _RVM1(0x257e, PAD_CI_D4_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ocpx_en=0
    _RVM1(0x1e1c, 0, BIT4),   //Reg[101e1c]#4=0
    //reg_smx_c48=0
    _RVM1(0x1e1a, 0, BIT5|BIT4),   //Reg[101e1a]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_D5_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D5_OEN (PAD_CI_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D5_OUT (PAD_CI_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257c, PAD_CI_D5_OUT, BIT4),
    _RVM1(0x257c, PAD_CI_D5_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ocpx_en=0
    _RVM1(0x1e1c, 0, BIT4),   //Reg[101e1c]#4=0
    //reg_smx_c48=0
    _RVM1(0x1e1a, 0, BIT5|BIT4),   //Reg[101e1a]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_D6_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D6_OEN (PAD_CI_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D6_OUT (PAD_CI_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257a, PAD_CI_D6_OUT, BIT4),
    _RVM1(0x257a, PAD_CI_D6_OEN, BIT5),
    //reg_all_pad_in1=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_sd_mode=0
    _RVM1(0x1e11, 0, BIT1|BIT0),   //Reg[101e11]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_D7_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D7_OEN (PAD_CI_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D7_OUT (PAD_CI_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2578, PAD_CI_D7_OUT, BIT4),
    _RVM1(0x2578, PAD_CI_D7_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_CEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_CEZ_OEN (PAD_CI_CEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_CEZ_OUT (PAD_CI_CEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2592, PAD_CI_CEZ_OUT, BIT4),
    _RVM1(0x2592, PAD_CI_CEZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_nand_mode=0
    _RVM1(0x1e10, 0, BIT6|BIT5|BIT4),   //Reg[101e10]#6~#4=0
    //reg_emmc_mode=0
    _RVM1(0x1e10, 0, BIT1|BIT0),   //Reg[101e10]#1~#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_ch34_dbg_mode=0
    _RVM1(0x1e0e, 0, BIT2|BIT1),   //Reg[101e0e]#2~#1=0
    #endif

    #if(PAD_CI_OEZ_IS_GPIO != GPIO_NONE)  //PAD_SM0_SMC_1P8V_SW
    #define PAD_CI_OEZ_OEN PAD_CI_OEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_OEZ_OUT PAD_CI_OEZ)_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258c, PAD_CI_OEZ_OUT, BIT4),
    _RVM1(0x258c, PAD_CI_OEZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_IORDZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_IORDZ_OEN (PAD_CI_IORDZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_IORDZ_OUT (PAD_CI_IORDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2588, PAD_CI_IORDZ_OUT, BIT4),
    _RVM1(0x2588, PAD_CI_IORDZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_IOWRZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_IOWRZ_OEN (PAD_CI_IOWRZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_IOWRZ_OUT (PAD_CI_IOWRZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258a, PAD_CI_IOWRZ_OUT, BIT4),
    _RVM1(0x258a, PAD_CI_IOWRZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_WEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_WEZ_OEN (PAD_CI_WEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_WEZ_OUT (PAD_CI_WEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258e, PAD_CI_WEZ_OUT, BIT4),
    _RVM1(0x258e, PAD_CI_WEZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sm1_open=0
    _RVM1(0x1e30, 0, BIT0),   //Reg[101e30]#0=0
    //reg_sm1_en=0
    _RVM1(0x1e31, 0, BIT0),   //Reg[101e31]#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_IRQAZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_IRQAZ_OEN (PAD_CI_IRQAZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_IRQAZ_OUT (PAD_CI_IRQAZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2594, PAD_CI_IRQAZ_OUT, BIT4),
    _RVM1(0x2594, PAD_CI_IRQAZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sm1_open=0
    _RVM1(0x1e30, 0, BIT0),   //Reg[101e30]#0=0
    //reg_sm1_en=0
    _RVM1(0x1e31, 0, BIT0),   //Reg[101e31]#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_RST_IS_GPIO != GPIO_NONE)   //PADA_TSIO_OUTN_CH[1]
    #define PAD_CI_RST_OEN (PAD_CI_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_RST_OUT (PAD_CI_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2598, PAD_CI_RST_OUT, BIT4),
    _RVM1(0x2598, PAD_CI_RST_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_gcr_en_reg=1
    _RVM1(0x1f00, BIT5, BIT5),   //Reg[171f00]#5=1
    //reg_pd_ib_tsio=1
    _RVM1(0x1f00, BIT12, BIT12),   //Reg[171f00]#12=1
    #endif

    #if(PAD_CI_WAITZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_WAITZ_OEN (PAD_CI_WAITZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_WAITZ_OUT (PAD_CI_WAITZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2596, PAD_CI_WAITZ_OUT, BIT4),
    _RVM1(0x2596, PAD_CI_WAITZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_sm1_en=0
    _RVM1(0x1e31, 0, BIT0),   //Reg[101e31]#0=0
    //reg_sm1_io=0
    _RVM1(0x1e31, 0, BIT4),   //Reg[101e31]#4=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_REGZ_IS_GPIO != GPIO_NONE)   //PADA_TSIO_OUTP_CH[0]
    #define PAD_CI_REGZ_OEN (PAD_CI_REGZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_REGZ_OUT (PAD_CI_REGZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2590, PAD_CI_REGZ_OUT, BIT4),
    _RVM1(0x2590, PAD_CI_REGZ_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    //reg_gcr_en_reg=1
    _RVM1(0x1f00, BIT5, BIT5),   //Reg[171f00]#5=1
    //reg_pd_ib_tsio=1
    _RVM1(0x1f00, BIT12, BIT12),   //Reg[171f00]#12=1
    #endif

    #if(PAD_CI_D0_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D0_OEN (PAD_CI_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D0_OUT (PAD_CI_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2586, PAD_CI_D0_OUT, BIT4),
    _RVM1(0x2586, PAD_CI_D0_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_D1_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D1_OEN (PAD_CI_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D1_OUT (PAD_CI_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2584, PAD_CI_D1_OUT, BIT4),
    _RVM1(0x2584, PAD_CI_D1_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif

    #if(PAD_CI_D2_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D2_OEN (PAD_CI_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D2_OUT (PAD_CI_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2582, PAD_CI_D2_OUT, BIT4),
    _RVM1(0x2582, PAD_CI_D2_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_ocp1_en=0
    _RVM1(0x1e32, 0, BIT4),   //Reg[101e32]#4=0
    //reg_sm1_en=0
    _RVM1(0x1e31, 0, BIT0),   //Reg[101e31]#0=0
    //reg_ci_mode=0
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //Reg[101e0e]#1~#0=0
    #endif
#endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2528, PAD_TS0_SYNC_OUT, BIT4),
    _RVM1(0x2528, PAD_TS0_SYNC_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2526, PAD_TS0_VLD_OUT, BIT4),
    _RVM1(0x2526, PAD_TS0_VLD_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2538, PAD_TS0_D0_OUT, BIT4),
    _RVM1(0x2538, PAD_TS0_D0_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2536, PAD_TS0_D1_OUT, BIT4),
    _RVM1(0x2536, PAD_TS0_D1_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2534, PAD_TS0_D2_OUT, BIT4),
    _RVM1(0x2534, PAD_TS0_D2_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2532, PAD_TS0_D3_OUT, BIT4),
    _RVM1(0x2532, PAD_TS0_D3_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2530, PAD_TS0_D4_OUT, BIT4),
    _RVM1(0x2530, PAD_TS0_D4_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252e, PAD_TS0_D5_OUT, BIT4),
    _RVM1(0x252e, PAD_TS0_D5_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252c, PAD_TS0_D6_OUT, BIT4),
    _RVM1(0x252c, PAD_TS0_D6_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252a, PAD_TS0_D7_OUT, BIT4),
    _RVM1(0x252a, PAD_TS0_D7_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2524, PAD_TS0_CLK_OUT, BIT4),
    _RVM1(0x2524, PAD_TS0_CLK_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts0_mode=0
    _RVM1(0x1e04, 0, BIT3|BIT2|BIT1|BIT0),   //Reg[101e04]#3~#0=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif
    
#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_TS1_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_SYNC_OEN (PAD_TS1_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_SYNC_OUT (PAD_TS1_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253c, PAD_TS1_SYNC_OUT, BIT4),
    _RVM1(0x253c, PAD_TS1_SYNC_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS1_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_VLD_OEN (PAD_TS1_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_VLD_OUT (PAD_TS1_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253e, PAD_TS1_VLD_OUT, BIT4),
    _RVM1(0x253e, PAD_TS1_VLD_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS1_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D0_OEN (PAD_TS1_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D0_OUT (PAD_TS1_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254e, PAD_TS1_D0_OUT, BIT4),
    _RVM1(0x254e, PAD_TS1_D0_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS1_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D1_OEN (PAD_TS1_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D1_OUT (PAD_TS1_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254c, PAD_TS1_D1_OUT, BIT4),
    _RVM1(0x254c, PAD_TS1_D1_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    #endif

    #if(PAD_TS1_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D2_OEN (PAD_TS1_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D2_OUT (PAD_TS1_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254a, PAD_TS1_D2_OUT, BIT4),
    _RVM1(0x254a, PAD_TS1_D2_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    #endif

    #if(PAD_TS1_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D3_OEN (PAD_TS1_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D3_OUT (PAD_TS1_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2548, PAD_TS1_D3_OUT, BIT4),
    _RVM1(0x2548, PAD_TS1_D3_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    #endif

    #if(PAD_TS1_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D4_OEN (PAD_TS1_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D4_OUT (PAD_TS1_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2546, PAD_TS1_D4_OUT, BIT4),
    _RVM1(0x2546, PAD_TS1_D4_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    #endif

    #if(PAD_TS1_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D5_OEN (PAD_TS1_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D5_OUT (PAD_TS1_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2544, PAD_TS1_D5_OUT, BIT4),
    _RVM1(0x2544, PAD_TS1_D5_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    #endif

    #if(PAD_TS1_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D6_OEN (PAD_TS1_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D6_OUT (PAD_TS1_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2542, PAD_TS1_D6_OUT, BIT4),
    _RVM1(0x2542, PAD_TS1_D6_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    #endif

    #if(PAD_TS1_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D7_OEN (PAD_TS1_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D7_OUT (PAD_TS1_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2540, PAD_TS1_D7_OUT, BIT4),
    _RVM1(0x2540, PAD_TS1_D7_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif

    #if(PAD_TS1_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_CLK_OEN (PAD_TS1_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_CLK_OUT (PAD_TS1_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253a, PAD_TS1_CLK_OUT, BIT4),
    _RVM1(0x253a, PAD_TS1_CLK_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_ts1_mode=0
    _RVM1(0x1e04, 0, BIT6|BIT5|BIT4),   //Reg[101e04]#6~#4=0
    //reg_mspi_mode=0
    _RVM1(0x1e12, 0, BIT7),   //Reg[101e12]#7=0
    _RVM1(0x1e13, 0, BIT0),   //Reg[101e13]#0=0
    _RVM1(0x1e86, 0, BIT0),   //Reg[101e86]#0=0
    #endif
#endif

    #if(PAD_I2CM0_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SCL_OEN (PAD_I2CM0_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SCL_OUT (PAD_I2CM0_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251e, PAD_I2CM0_SCL_OUT, BIT4),
    _RVM1(0x251e, PAD_I2CM0_SCL_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_dmd_agc_mode=0
    _RVM1(0x1e05, 0, BIT3),   //Reg[101e05]#3=0
    #endif

    #if(PAD_I2CM0_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SDA_OEN (PAD_I2CM0_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SDA_OUT (PAD_I2CM0_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251c, PAD_I2CM0_SDA_OUT, BIT4),
    _RVM1(0x251c, PAD_I2CM0_SDA_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_dmd_agc_mode=0
    _RVM1(0x1e05, 0, BIT3),   //Reg[101e05]#3=0
    #endif

    #if(PAD_I2CM1_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SCL_OEN (PAD_I2CM1_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SCL_OUT (PAD_I2CM1_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2522, PAD_I2CM1_SCL_OUT, BIT4),
    _RVM1(0x2522, PAD_I2CM1_SCL_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    #endif

    #if(PAD_I2CM1_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SDA_OEN (PAD_I2CM1_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SDA_OUT (PAD_I2CM1_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2520, PAD_I2CM1_SDA_OUT, BIT4),
    _RVM1(0x2520, PAD_I2CM1_SDA_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_test_in_mode=0
    _RVM1(0x1e24, 0, BIT1|BIT0),   //Reg[101e24]#1~#0=0
    //reg_test_out_mode=0
    _RVM1(0x1e24, 0, BIT5|BIT4),   //Reg[101e24]#5~#4=0
    //reg_i2cm0_mode=0
    _RVM1(0x1e12, 0, BIT2|BIT1|BIT0),   //Reg[101e12]#2~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    #endif
    
    _MEMMAP_PM_,

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266DD01A_S))
    #if(PAD_PM_IRIN_IS_GPIO != GPIO_NONE)
    #define PAD_PM_IRIN_OEN (PAD_PM_IRIN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_IRIN_OUT (PAD_PM_IRIN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f28, PAD_PM_IRIN_OUT, BIT1),
    _RVM1(0x0f28, PAD_PM_IRIN_OEN, BIT0),
    //reg_ir_is_gpio[0]=1
    _RVM1(0x0e38, BIT4, BIT4),   //Reg[000e38]#4=1
    #endif

    #if(PAD_PM_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ_OEN (PAD_PM_SPI_CZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CZ_OUT (PAD_PM_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OUT, BIT1),
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OEN, BIT0),
    //reg_spi_is_gpio[2]=1
    _RVM1(0x0e6a, BIT2, BIT2),   //Reg[000e6a]#2=1
    #endif

    #if(PAD_PM_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CK_OEN (PAD_PM_SPI_CK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CK_OUT (PAD_PM_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f32, PAD_PM_SPI_CK_OUT, BIT1),
    _RVM1(0x0f32, PAD_PM_SPI_CK_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DI_OEN (PAD_PM_SPI_DI_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DI_OUT (PAD_PM_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f34, PAD_PM_SPI_DI_OUT, BIT1),
    _RVM1(0x0f34, PAD_PM_SPI_DI_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DO_OEN (PAD_PM_SPI_DO_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DO_OUT (PAD_PM_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f36, PAD_PM_SPI_DO_OUT, BIT1),
    _RVM1(0x0f36, PAD_PM_SPI_DO_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_WPZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_WPZ_OEN (PAD_PM_SPI_WPZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_WPZ_OUT (PAD_PM_SPI_WPZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f88, PAD_PM_SPI_WPZ_OUT, BIT1),
    _RVM1(0x0f88, PAD_PM_SPI_WPZ_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_HOLDZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_HOLDZ_OEN (PAD_PM_SPI_HOLDZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_HOLDZ_OUT (PAD_PM_SPI_HOLDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f8a, PAD_PM_SPI_HOLDZ_OUT, BIT1),
    _RVM1(0x0f8a, PAD_PM_SPI_HOLDZ_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_RSTZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_RSTZ_OEN (PAD_PM_SPI_RSTZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_RSTZ_OUT (PAD_PM_SPI_RSTZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f8c, PAD_PM_SPI_RSTZ_OUT, BIT1),
    _RVM1(0x0f8c, PAD_PM_SPI_RSTZ_OEN, BIT0),
    #endif
#endif

    #if(PAD_PM_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO0_OEN (PAD_PM_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO0_OUT (PAD_PM_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f00, PAD_PM_GPIO0_OUT, BIT1),
    _RVM1(0x0f00, PAD_PM_GPIO0_OEN, BIT0),
    //reg_pwm0_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT1|BIT0),   //Reg[000e50]#1~#0=0
    //reg_sec_uart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT1|BIT0),   //Reg[000e51]#1~#0=0
    #endif

    #if(PAD_PM_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO1_OEN (PAD_PM_GPIO1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO1_OUT (PAD_PM_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f02, PAD_PM_GPIO1_OUT, BIT1),
    _RVM1(0x0f02, PAD_PM_GPIO1_OEN, BIT0),
    //reg_pwm1_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT3|BIT2),   //Reg[000e50]#3~#2=0
    //reg_sec_uart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT1|BIT0),   //Reg[000e51]#1~#0=0
    //reg_uart_is_gpio[1]=0
    _RVM1(0x0e6b, 0, BIT1),   //Reg[000e6b]#1=0
    #endif

    #if(PAD_PM_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO2_OEN (PAD_PM_GPIO2_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO2_OUT (PAD_PM_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f04, PAD_PM_GPIO2_OUT, BIT1),
    _RVM1(0x0f04, PAD_PM_GPIO2_OEN, BIT0),
    //reg_led_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT5|BIT4),   //Reg[000e50]#5~#4=0
    //reg_fuart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=0
    //reg_vid_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=0
    #endif

    #if(PAD_PM_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO3_OEN (PAD_PM_GPIO3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO3_OUT (PAD_PM_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f06, PAD_PM_GPIO3_OUT, BIT1),
    _RVM1(0x0f06, PAD_PM_GPIO3_OEN, BIT0),
    //reg_spi_is_gpio[3]=1
    _RVM1(0x0e6a, BIT3, BIT3),   //Reg[000e6a]#3=1
    //reg_led_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT5|BIT4),   //Reg[000e50]#5~#4=0
    //reg_fuart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=0
    //reg_vid_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=0
    //reg_irtx_mode=0
    _RVM1(0x0ef6, 0, BIT1),   //Reg[000ef6]#1=0
    #endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_PM_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO4_OEN (PAD_PM_GPIO4_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO4_OUT (PAD_PM_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f08, PAD_PM_GPIO4_OUT, BIT1),
    _RVM1(0x0f08, PAD_PM_GPIO4_OEN, BIT0),
    //reg_gpio_pm_lock[15:8]='hba
    _RVM1(0x0e25, 0xba, BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0),   //Reg[000e25]#7~#0='hba
    //reg_gpio_pm_lock[7:0]='hbe
    _RVM1(0x0e24, 0xbe, BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0),   //Reg[000e24]#7~#0='hbe
    #endif

    #if(PAD_PM_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO5_OEN (PAD_PM_GPIO5_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO5_OUT (PAD_PM_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0a, PAD_PM_GPIO5_OUT, BIT1),
    _RVM1(0x0f0a, PAD_PM_GPIO5_OEN, BIT0),
    //reg_gpio_rst_en=0
    _RVM1(0x0e39, 0, BIT4),   //Reg[000e39]#4=0
    #endif

    #if(PAD_PM_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO6_OEN (PAD_PM_GPIO6_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO6_OUT (PAD_PM_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0c, PAD_PM_GPIO6_OUT, BIT1),
    _RVM1(0x0f0c, PAD_PM_GPIO6_OEN, BIT0),
    //reg_mspi_mode=0
    _RVM1(0x0ef6, 0, BIT2),   //Reg[000ef6]#2=0
    #endif
#endif

    //QFP_S2 BGA
#if !(defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266E_D01A))
    #if(PAD_PM_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO7_OEN (PAD_PM_GPIO7_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO7_OUT (PAD_PM_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0e, PAD_PM_GPIO7_OUT, BIT1),
    _RVM1(0x0f0e, PAD_PM_GPIO7_OEN, BIT0),
    //reg_mspi_mode=0
    _RVM1(0x0ef6, 0, BIT2),   //Reg[000ef6]#2=0
    //reg_diseqc_mode=0
    _RVM1(0x1e05, 0, BIT5|BIT4),   //Reg[101e05]#5~#4=0
    #endif

    #if(PAD_PM_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO8_OEN (PAD_PM_GPIO8_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO8_OUT (PAD_PM_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f10, PAD_PM_GPIO8_OUT, BIT1),
    _RVM1(0x0f10, PAD_PM_GPIO8_OEN, BIT0),
    //reg_vid_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=0
    //reg_uart_is_gpio[0]=0
    _RVM1(0x0e6b, 0, BIT0),   //Reg[000e6b]#0=0
    //reg_mspi_mode=0
    _RVM1(0x0ef6, 0, BIT2),   //Reg[000ef6]#2=0
    //reg_diseqc_mode=0
    _RVM1(0x1e05, 0, BIT5|BIT4),   //Reg[101e05]#5~#4=0
    #endif
#endif
    //QFP_S2 BGA QFP_CA
#if !(defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A_S))
    #if(PAD_PM_GPIO9_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO9_OEN (PAD_PM_GPIO9_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO9_OUT (PAD_PM_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f12, PAD_PM_GPIO9_OUT, BIT1),
    _RVM1(0x0f12, PAD_PM_GPIO9_OEN, BIT0),
    //reg_pwm0_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT1|BIT0),   //Reg[000e50]#1~#0=0
    //reg_fuart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=0
    //reg_vid_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=0
    #endif

    #if(PAD_PM_GPIO10_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO10_OEN (PAD_PM_GPIO10_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO10_OUT (PAD_PM_GPIO10_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f14, PAD_PM_GPIO10_OUT, BIT1),
    _RVM1(0x0f14, PAD_PM_GPIO10_OEN, BIT0),
    //reg_pwm1_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT3|BIT2),   //Reg[000e50]#3~#2=0
    //reg_fuart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=0
    #endif
#endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_PM_GPIO11_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO11_OEN (PAD_PM_GPIO11_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO11_OUT (PAD_PM_GPIO11_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f16, PAD_PM_GPIO11_OUT, BIT1),
    _RVM1(0x0f16, PAD_PM_GPIO11_OEN, BIT0),
    //reg_mspi_mode=0
    _RVM1(0x0ef6, 0, BIT2),   //Reg[000ef6]#2=0
    #endif
#endif

    #if(PAD_PM_GPIO12_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO12_OEN (PAD_PM_GPIO12_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO12_OUT (PAD_PM_GPIO12_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f18, PAD_PM_GPIO12_OUT, BIT1),
    _RVM1(0x0f18, PAD_PM_GPIO12_OEN, BIT0),
    //reg_pwm1_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT3|BIT2),   //Reg[000e50]#3~#2=0
    //reg_sec_uart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT1|BIT0),   //Reg[000e51]#1~#0=0
    #endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_PM_GPIO13_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO13_OEN (PAD_PM_GPIO13_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO13_OUT (PAD_PM_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1a, PAD_PM_GPIO13_OUT, BIT1),
    _RVM1(0x0f1a, PAD_PM_GPIO13_OEN, BIT0),
    //reg_sec_uart_mode[1:0]=0
    _RVM1(0x0e51, 0, BIT1|BIT0),   //Reg[000e51]#1~#0=0
    #endif

    #if(PAD_PM_GPIO14_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO14_OEN (PAD_PM_GPIO14_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO14_OUT (PAD_PM_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1c, PAD_PM_GPIO14_OUT, BIT1),
    _RVM1(0x0f1c, PAD_PM_GPIO14_OEN, BIT0),
    //reg_spi_is_gpio[3]=1
    _RVM1(0x0e6a, BIT3, BIT3),   //Reg[000e6a]#3=1
    //reg_ir2_mode=0
    _RVM1(0x0ef6, 0, BIT0),   //Reg[000ef6]#0=0
    #endif
#endif

    #if(PAD_PHY_LED_LINK_IS_GPIO != GPIO_NONE)
    #define PAD_PHY_LED_LINK_OEN (PAD_PHY_LED_LINK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PHY_LED_LINK_OUT (PAD_PHY_LED_LINK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1e, PAD_PHY_LED_LINK_OUT, BIT1),
    _RVM1(0x0f1e, PAD_PHY_LED_LINK_OEN, BIT0),
    //reg_led_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT5|BIT4),   //Reg[000e50]#5~#4=0
    #endif

    #if(PAD_PHY_LED_DATA_IS_GPIO != GPIO_NONE)
    #define PAD_PHY_LED_DATA_OEN (PAD_PHY_LED_DATA_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PHY_LED_DATA_OUT (PAD_PHY_LED_DATA_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f20, PAD_PHY_LED_DATA_OUT, BIT1),
    _RVM1(0x0f20, PAD_PHY_LED_DATA_OEN, BIT0),
    //reg_led_mode[1:0]=0
    _RVM1(0x0e50, 0, BIT5|BIT4),   //Reg[000e50]#5~#4=0
    #endif

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #if(PAD_PM_HDMI_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_PM_HDMI_CEC_OEN (PAD_PM_HDMI_CEC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_HDMI_CEC_OUT (PAD_PM_HDMI_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2c, PAD_PM_HDMI_CEC_OUT, BIT1),
    _RVM1(0x0f2c, PAD_PM_HDMI_CEC_OEN, BIT0),
    //reg_cec_is_gpio=1
    _RVM1(0x0e38, BIT6, BIT6),   //Reg[000e38]#6=1
    #endif

    #if(PAD_PM_HDMI_HPD_IS_GPIO != GPIO_NONE)
    #define PAD_PM_HDMI_HPD_OEN (PAD_PM_HDMI_HPD_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_PM_HDMI_HPD_OUT (PAD_PM_HDMI_HPD_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1166, PAD_PM_HDMI_HPD_OUT, BIT2),
    _RVM1(0x1166, PAD_PM_HDMI_HPD_OEN, BIT3),
    #endif
#endif

    #if(PAD_SAR_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO0_OEN (PAD_SAR_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR_GPIO0_OUT (PAD_SAR_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR_GPIO0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR_GPIO0_OEN, BIT0),
    //reg_sar_aisel[0]=0
    _RVM1(0x1422, 0, BIT0),   //Reg[001422]#0=0
    #endif

    #if(PAD_SAR_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO1_OEN (PAD_SAR_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR_GPIO1_OUT (PAD_SAR_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR_GPIO1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR_GPIO1_OEN, BIT1),
    //reg_sar_aisel[1]=0
    _RVM1(0x1422, 0, BIT1),   //Reg[001422]#1=0
    #endif

    #if(PAD_SAR_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO2_OEN (PAD_SAR_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR_GPIO2_OUT (PAD_SAR_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR_GPIO2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR_GPIO2_OEN, BIT2),
    //reg_sar_aisel[2]=0
    _RVM1(0x1422, 0, BIT2),   //Reg[001422]#2=0
    #endif

    #if(PAD_VPLUG_DET_IS_GPIO != GPIO_NONE)
    #define PAD_VPLUG_DET_OEN (PAD_VPLUG_DET_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_VPLUG_DET_OUT (PAD_VPLUG_DET_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_VPLUG_DET_OUT, BIT5),
    _RVM1(0x1423, PAD_VPLUG_DET_OEN, BIT5),
    //reg_sar_aisel[5]=0
    _RVM1(0x1422, 0, BIT5),   //Reg[001422]#5=0
    #endif

    _MEMMAP_nonPM_,

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266B_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266C_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266E_D01A))
    #if(PAD_S_GPIO5_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO5_QFP_OEN (PAD_S_GPIO5_QFP_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO5_QFP_OUT (PAD_S_GPIO5_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2638, PAD_S_GPIO5_QFP_OUT, BIT4),
    _RVM1(0x2638, PAD_S_GPIO5_QFP_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_i2s_out_mute_mode=0
    _RVM1(0x1e0b, 0, BIT1),   //Reg[101e0b]#1=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_SecondUARTMode=0
    _RVM1(0x1e06, 0, BIT2|BIT1|BIT0),   //Reg[101e06]#2~#0=0
    //reg_ThirdUARTMode=0
    _RVM1(0x1e06, 0, BIT6|BIT5|BIT4),   //Reg[101e06]#6~#4=0
    //reg_ForthUARTMode=0
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0),   //Reg[101e07]#2~#0=0
    //reg_fuart_mode=0
    _RVM1(0x1e13, 0, BIT4|BIT3|BIT2),   //Reg[101e13]#4~#2=0
    #endif

    #if(PAD_HSYNC_OUT_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_HSYNC_OUT_QFP_OEN (PAD_HSYNC_OUT_QFP_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HSYNC_OUT_QFP_OUT (PAD_HSYNC_OUT_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e0, PAD_HSYNC_OUT_QFP_OUT, BIT4),
    _RVM1(0x25e0, PAD_HSYNC_OUT_QFP_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_spdif_in_mode=0
    _RVM1(0x1e0a, 0, BIT1|BIT0),   //Reg[101e0a]#1~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e12, 0, BIT6|BIT5|BIT4),   //Reg[101e12]#6~#4=0
    //reg_hsync_en=0
    _RVM1(0x1e7e, 0, BIT0),   //Reg[101e7e]#0=0
    #endif

    #if(PAD_VSYNC_OUT_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_VSYNC_OUT_QFP_OEN (PAD_VSYNC_OUT_QFP_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_VSYNC_OUT_QFP_OUT (PAD_VSYNC_OUT_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e2, PAD_VSYNC_OUT_QFP_OUT, BIT4),
    _RVM1(0x25e2, PAD_VSYNC_OUT_QFP_OEN, BIT5),
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    //reg_spdif_out_mode=0
    _RVM1(0x1e0a, 0, BIT1|BIT0),   //Reg[101e0a]#1~#0=0
    //reg_i2cm1_mode=0
    _RVM1(0x1e0a, 0, BIT5|BIT4),   //Reg[101e0a]#5~#4=0
    //reg_vsync_en=0
    _RVM1(0x1e7e, 0, BIT1),   //Reg[101e7e]#1=0
    #endif

    _MEMMAP_PM_,

    #if(PAD_IRIN_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_IRIN_QFP_OEN (PAD_IRIN_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_IRIN_QFP_OUT (PAD_IRIN_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f28, PAD_IRIN_QFP_OUT, BIT1),
    _RVM1(0x0f28, PAD_IRIN_QFP_OEN, BIT0),
    //reg_ir_is_gpio[0]=1
    _RVM1(0x0e38, BIT4, BIT4),   //Reg[000e38]#4=1
    #endif

    #if(PAD_SPI_CZ_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_CZ_QFP_OEN (PAD_SPI_CZ_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_CZ_QFP_OUT (PAD_SPI_CZ_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f30, PAD_SPI_CZ_QFP_OUT, BIT1),
    _RVM1(0x0f30, PAD_SPI_CZ_QFP_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_SPI_CK_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_CK_QFP_OEN (PAD_SPI_CK_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_CK_QFP_OUT (PAD_SPI_CK_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f32, PAD_SPI_CK_QFP_OUT, BIT1),
    _RVM1(0x0f32, PAD_SPI_CK_QFP_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_SPI_DI_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_DI_QFP_OEN (PAD_SPI_DI_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_DI_QFP_OUT (PAD_SPI_DI_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f34, PAD_SPI_DI_QFP_OUT, BIT1),
    _RVM1(0x0f34, PAD_SPI_DI_QFP_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_SPI_DO_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_DO_QFP_OEN (PAD_SPI_DO_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_DO_QFP_OUT (PAD_SPI_DO_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f36, PAD_SPI_DO_QFP_OUT, BIT1),
    _RVM1(0x0f36, PAD_SPI_DO_QFP_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_SPI_WPN_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_WPN_QFP_OEN (PAD_SPI_WPN_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_WPN_QFP_OUT (PAD_SPI_WPN_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f88, PAD_SPI_WPN_QFP_OUT, BIT1),
    _RVM1(0x0f88, PAD_SPI_WPN_QFP_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_SPI_HOLDN_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_HOLDN_QFP_OEN (PAD_SPI_HOLDN_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPI_HOLDN_QFP_OUT (PAD_SPI_HOLDN_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f8a, PAD_SPI_HOLDN_QFP_OUT, BIT1),
    _RVM1(0x0f8a, PAD_SPI_HOLDN_QFP_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1
    #endif

    #if(PAD_HDMI_CEC_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_HDMI_CEC_QFP_OEN (PAD_HDMI_CEC_QFP_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_HDMI_CEC_QFP_OUT (PAD_HDMI_CEC_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2c, PAD_HDMI_CEC_QFP_OUT, BIT1),
    _RVM1(0x0f2c, PAD_HDMI_CEC_QFP_OEN, BIT0),    
    _MEMMAP_nonPM_,
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    _MEMMAP_PM_,
    //reg_cec_is_gpio=1
    _RVM1(0x0e38, BIT6, BIT6),   //Reg[000e38]#6=1
    #endif

    #if(PAD_HDMI_HPD_QFP_IS_GPIO != GPIO_NONE)
    #define PAD_HDMI_HPD_QFP_OEN (PAD_HDMI_HPD_QFP_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HDMI_HPD_QFP_OUT (PAD_HDMI_HPD_QFP_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1166, PAD_HDMI_HPD_QFP_OUT, BIT2),
    _RVM1(0x1166, PAD_HDMI_HPD_QFP_OEN, BIT3),
    _MEMMAP_nonPM_,
    //reg_all_pad_in=0
    _RVM1(0x1ea1, 0, BIT7),   //Reg[101ea1]#7=0
    #endif
#endif    
    
//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------    
    _MEMMAP_nonPM_,

    #if (DRVPAD_SM0_OPEN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E18, DRVPAD_SM0_OPEN, BIT0), 
    #endif

    #if (DRVPAD_SM0_C48!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E18, DRVPAD_SM0_C48 << 4, BITMASK(5:4)),
    #endif

    #if (DRVPAD_SM0_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E19, DRVPAD_SM0_EN, BIT0),
    #endif

    #if (DRVPAD_SM0_IO!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E19, DRVPAD_SM0_IO << 4, BIT4),
    #endif

    #if (DRVPAD_I2CM0_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E12, DRVPAD_I2CM0_MODE, BITMASK(2:0)),
    #endif
    
    #if (DRVPAD_I2CM1_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E12, DRVPAD_I2CM1_MODE << 4, BITMASK(6:4)),
    #endif  

    #if (DRVPAD_TS0_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E04, DRVPAD_TS0_MODE, BITMASK(3:0)),
    #endif

    #if (DRVPAD_TS1_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E04, DRVPAD_TS1_MODE << 4, BITMASK(6:4)),
    #endif

    #if (DRVPAD_NAND_MODE!=PADMUX_MDOE_DEFAULT)
    // use the NF_REZ as the main different pin comparison 
    _RVM1(0x1E10, DRVPAD_NAND_MODE << 4, BITMASK(6:4)),
    #endif

    #if (DRVPAD_CI_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0E, DRVPAD_CI_MODE, BIT1|BIT0),
    #endif

    #if (DRVPAD_SPDIF_OUT_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0A, DRVPAD_SPDIF_OUT_MODE << 4, BITMASK(5:4)),
    #endif

    #if (DRVPAD_SPDIF_IN_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0A, DRVPAD_SPDIF_IN_MODE, BITMASK(1:0)),
    #endif

    #if (DRVPAD_I2S_IN_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0B, DRVPAD_I2S_IN_MODE, BIT0),
    #endif

    #if (DRVPAD_SECOND_UART_MODE!=PADMUX_MDOE_DEFAULT)
    // UART MUX 
    _RVM1(0x1E06, DRVPAD_SECOND_UART_MODE, BITMASK(2:0)),
    #endif

    #if (DRVPAD_THIRD_UART_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E06, DRVPAD_THIRD_UART_MODE << 4, BITMASK(6:4)), 
    #endif

    #if (DRVPAD_FORTH_UART_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E07, DRVPAD_FORTH_UART_MODE, BITMASK(2:0)),
    #endif

    #if (DRVPAD_DISEQC_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM2(0x1E04, DRVPAD_DISEQC_MODE << 12, BITMASK(13:12)),
    #endif

//---------------------------------------------------------------------
// END Pad Configuartion
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// Kronus U02 to fix smart card problem in 216 pin package
//---------------------------------------------------------------------

    _RVM1(0x1E45, 0, BIT2), // reg_156_mode set to 0 if the package is not 156 pin for SMC

//---------------------------------------------------------------------
    // UART_SRC_SEL
    #define UART_INV    ((UART0_INV ? BIT0 : 0) | \
                         (UART1_INV ? BIT1 : 0) | \
                         (UART2_INV ? BIT2 : 0) | \
                         (UART3_INV ? BIT3 : 0) | \
                         (UART4_INV ? BIT4 : 0))

    _RVM1(0x1EAB, UART_INV, BITMASK(4:0)),
    _RVM1(0x1EA6, (UART1_SRC_SEL << 4) | (UART0_SRC_SEL << 0), BITMASK(7:0)),
    _RVM1(0x1EA7, (UART3_SRC_SEL << 4) | (UART2_SRC_SEL << 0), BITMASK(7:0)),
    _RVM1(0x1EA8, (UART4_SRC_SEL << 0), BITMASK(3:0)),

    _MEMMAP_PM_,

    _RVM1(0x0E13, BIT3, BIT3),   // RX0_ENABLE
    _RVM1(0x0E13, 0 , BIT4),   // UART0 -> reg_uart_sel0

//---------------------------------------------------------------------
// REG PATCHS
//---------------------------------------------------------------------

    _MEMMAP_PM_, 
    _RVM1(0x0E50, REG_LED_MODE << 4, (BIT4|BIT5)),//LED HW MODE

    ///////////////////////////////////////////////////////////////////////////
    //PAD PE PS setting
    ///////////////////////////////////////////////////////////////////////////

    _MEMMAP_nonPM_, 
    //PAD_SPDIF_OUT, PE=1, PS=1
    _RVM1(0x1e69,0xff,0x0c),

    //PAD_S_GPIO1, PE=1, PS=1
    _RVM1(0x1e7e,0xff,0xc0),

    //PAD_T_GPIO3, PE=1, PS=1
    //PAD_T_GPIO4, PE=1, PS=1
    _RVM1(0x1e7f,0xff,0xcc),

    //PAD_FUART_RTS, PE=1, PS=1
    _RVM1(0x1e62,0xff,0xc0),

    //PAD_GPIO8, PE=1, PS=1
    _RVM1(0x1e4d,0xff,0x0c),

    //PAD_CI_A11, PE=1, PS=1
    //PAD_CI_A12, PE=1, PS=1
    //PAD_CI_A13, PE=1, PS=1
    //PAD_CI_A14, PE=1, PS=1
    _RVM1(0x1e49,0xff,0x78),
    _RVM1(0x1e55,0xff,0x78),

    //PAD_SM0_SMC_1P8V_SW(PAD_CI_OEZ), PE=1, PS=1
    _RVM1(0x1e4a,0xff,0x04),
    _RVM1(0x1e56,0xff,0x04),

    //PAD_HSYNC_OUT_QFP, PE=1, PS=1
    //PAD_VSYNC_OUT_QFP, PE=1, PS=1
    _RVM1(0x1e7d,0xff,0xcc),

    _MEMMAP_PM_, 
    //swch 3
    //PAD_PM_GPIO0, PE=1, PS=1
    _RVM1(0x0f01,0xff,0xc2),

    //PAD_PM_GPIO1, PE=1, PS=1
    _RVM1(0x0f03,0xff,0xc0),

    //PAD_PM_GPIO9, PE=1, PS=1
    _RVM1(0x0f13,0xff,0xc2),

    //PAD_PM_GPIO10, PE=1, PS=1
    _RVM1(0x0f15,0xff,0xc0),

    //PAD_PM_GPIO12, PE=1, PS=1
    _RVM1(0x0f19,0xff,0xc2),

    //---------------------------------------------------------------------
    // If SMC PAD mux is setup, apply below setting to make all SMC PAD are at voltage '0'
    // 1. Set SMC PAD power down
    // 2. Disable GCR 3.3V GPIO
    //
    // QFP : GPIO mode, reg_gcr_smc_ldo_3p3_gpio_en=1 (default setting)
    // BGA : Int. 8024 mode, reg_gcr_smc_ldo_3p3_gpio_en=0
    //---------------------------------------------------------------------
    _MEMMAP_nonPM_,

#if (defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A)|| defined(CONFIG_MSTAR_K5TN_BD_MST266A_D01A_S) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A) || defined(CONFIG_MSTAR_K5TN_BD_MST266D_D01A_S))
    #define SM0_PAD_PD  ( (DRVPAD_SM0_EN!=PADMUX_MDOE_DEFAULT) ? (BIT6) : 0)

    _RVM1(0x2958, SM0_PAD_PD, BIT6),

    #define SM0_GCR_SMC_3P3_GPIO  ((DRVPAD_SM0_EN!=PADMUX_MDOE_DEFAULT) ? (0) : BIT4)
   
    _RVM1(0x294E, SM0_GCR_SMC_3P3_GPIO, BIT4),
#endif
//---------------------------------------------------------------------
// END REG PATCHS
//---------------------------------------------------------------------
#endif
    // Clear all pad in
    _RVM1(0x1EA1, 0, BIT7),
    _END_OF_TBL2_,

//---------------------------------------------------------------------
// ISP_TOOL Write Protect (Need to check ????)
//---------------------------------------------------------------------
    _END_OF_TBL2_,
};

//-----------------------------------------------------------------
// Sanity check for PAD_TOP setting (TODO)
//-----------------------------------------------------------------

#if (UART0_SRC_SEL == UART1_SRC_SEL) || \
    (UART0_SRC_SEL == UART2_SRC_SEL) || \
    (UART0_SRC_SEL == UART3_SRC_SEL) || \
    (UART0_SRC_SEL == UART4_SRC_SEL) || \
    (UART1_SRC_SEL == UART2_SRC_SEL) || \
    (UART1_SRC_SEL == UART3_SRC_SEL) || \
    (UART1_SRC_SEL == UART4_SRC_SEL) || \
    (UART2_SRC_SEL == UART3_SRC_SEL) || \
    (UART2_SRC_SEL == UART4_SRC_SEL) || \
    (UART3_SRC_SEL == UART4_SRC_SEL)
    #error "UART src selection conflict"
#endif



