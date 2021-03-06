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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "datatype.h"
#include "drvRIU.h"
#include "Board.h"
#include "c_riubase.h"
#include "hwreg_K6.h"
#include "chip/bond.h"

#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high
#define MUX_MODE_1              1
#define MUX_MODE_2              2
#define MUX_MODE_3              3
#define MUX_MODE_4              4


#define _MEMMAP_nonPM_  _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_PM_     _RVM1(0x0000, 0x00, 0xFF)

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

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    _MEMMAP_PM_,

    #if(PAD_PM_IRIN_IS_GPIO != GPIO_NONE)
    #define PAD_PM_IRIN_OEN (PAD_PM_IRIN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_IRIN_OUT (PAD_PM_IRIN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f28, PAD_PM_IRIN_OUT, BIT1),
    _RVM1(0x0f28, PAD_PM_IRIN_OEN, BIT0),
    //reg_ir_is_gpio[0]
    _RVM1(0x0e38, BIT4, BIT4),   //Reg[000e38]#4=1b
    #endif

    #if(PAD_PM_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ_OEN (PAD_PM_SPI_CZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CZ_OUT (PAD_PM_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OUT, BIT1),
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OEN, BIT0),
    //reg_spi_is_gpio[1]
    _RVM1(0x0e6a, BIT1, BIT1),   //Reg[000e6a]#1=1b
    #endif

    #if(PAD_PM_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CK_OEN (PAD_PM_SPI_CK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CK_OUT (PAD_PM_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f32, PAD_PM_SPI_CK_OUT, BIT1),
    _RVM1(0x0f32, PAD_PM_SPI_CK_OEN, BIT0),
    //reg_spi_is_gpio[0]
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1b
    #endif

    #if(PAD_PM_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DI_OEN (PAD_PM_SPI_DI_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DI_OUT (PAD_PM_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f34, PAD_PM_SPI_DI_OUT, BIT1),
    _RVM1(0x0f34, PAD_PM_SPI_DI_OEN, BIT0),
    //reg_spi_is_gpio[0]
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1b
    #endif

    #if(PAD_PM_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DO_OEN (PAD_PM_SPI_DO_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DO_OUT (PAD_PM_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f36, PAD_PM_SPI_DO_OUT, BIT1),
    _RVM1(0x0f36, PAD_PM_SPI_DO_OEN, BIT0),
    //reg_spi_is_gpio[0]
    _RVM1(0x0e6a, BIT0, BIT0),   //Reg[000e6a]#0=1b
    #endif

    #if(PAD_PM_SPI_WPZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_WPZ_OEN (PAD_PM_SPI_WPZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_WPZ_OUT (PAD_PM_SPI_WPZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f88, PAD_PM_SPI_WPZ_OUT, BIT1),
    _RVM1(0x0f88, PAD_PM_SPI_WPZ_OEN, BIT0),
    //reg_spi_is_gpio[4]
    _RVM1(0x0e6a, BIT4, BIT4),   //Reg[000e6a]#4=1b
    #endif

    #if(PAD_PM_SPI_HOLDZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_HOLDZ_OEN (PAD_PM_SPI_HOLDZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_HOLDZ_OUT (PAD_PM_SPI_HOLDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f8a, PAD_PM_SPI_HOLDZ_OUT, BIT1),
    _RVM1(0x0f8a, PAD_PM_SPI_HOLDZ_OEN, BIT0),
    //reg_spi_is_gpio[7:6]
    _RVM1(0x0e6a, 0, BIT7|BIT6),   //Reg[000e6a]#7~#6=00b
    #endif

    #if(PAD_PM_SPI_RSTZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_RSTZ_OEN (PAD_PM_SPI_RSTZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_RSTZ_OUT (PAD_PM_SPI_RSTZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f8c, PAD_PM_SPI_RSTZ_OUT, BIT1),
    _RVM1(0x0f8c, PAD_PM_SPI_RSTZ_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO0_OEN (PAD_PM_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO0_OUT (PAD_PM_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f00, PAD_PM_GPIO0_OUT, BIT1),
    _RVM1(0x0f00, PAD_PM_GPIO0_OEN, BIT0),
    //reg_spi_is_gpio[7:6]
    _RVM1(0x0e6a, 0, BIT7|BIT6),   //Reg[000e6a]#7~#6=00b
    //reg_pwm0_mode[1:0]
    _RVM1(0x0e50, 0, BIT1|BIT0),   //Reg[000e50]#1~#0=00b
    //reg_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=00b
    //reg_gpu_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=00b
    //reg_miic_mode[1:0]
    _RVM1(0x0e50, 0, BIT7|BIT6),   //Reg[000e50]#7~#6=00b
    #endif

    #if(PAD_PM_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO1_OEN (PAD_PM_GPIO1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO1_OUT (PAD_PM_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f02, PAD_PM_GPIO1_OUT, BIT1),
    _RVM1(0x0f02, PAD_PM_GPIO1_OEN, BIT0),
    //reg_spi_is_gpio[7:6]
    _RVM1(0x0e6a, 0, BIT7|BIT6),   //Reg[000e6a]#7~#6=00b
    //reg_pwm1_mode[1:0]
    _RVM1(0x0e50, 0, BIT3|BIT2),   //Reg[000e50]#3~#2=00b
    //reg_uart_is_gpio[2:1]
    _RVM1(0x0e6b, 0, BIT2|BIT1),   //Reg[000e6b]#2~#1=00b
    #endif

    #if(PAD_PM_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO2_OEN (PAD_PM_GPIO2_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO2_OUT (PAD_PM_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f04, PAD_PM_GPIO2_OUT, BIT1),
    _RVM1(0x0f04, PAD_PM_GPIO2_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO3_OEN (PAD_PM_GPIO3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO3_OUT (PAD_PM_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f06, PAD_PM_GPIO3_OUT, BIT1),
    _RVM1(0x0f06, PAD_PM_GPIO3_OEN, BIT0),
    //reg_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=00b
    //reg_gpu_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=00b
    //reg_miic_mode[1:0]
    _RVM1(0x0e50, 0, BIT7|BIT6),   //Reg[000e50]#7~#6=00b
    #endif

    #if(PAD_PM_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO4_OEN (PAD_PM_GPIO4_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO4_OUT (PAD_PM_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f08, PAD_PM_GPIO4_OUT, BIT1),
    _RVM1(0x0f08, PAD_PM_GPIO4_OEN, BIT0),
    //reg_gpio_pm_lock[15:8]
    _RVM1(0x0e25, BIT7|BIT5|BIT4|BIT3|BIT1, BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0),   //Reg[000e25]#7~#0=10111010b
    //reg_gpio_pm_lock[7:0]
    _RVM1(0x0e24, BIT7|BIT5|BIT4|BIT3|BIT2|BIT1, BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0),   //Reg[000e24]#7~#0=10111110b
    //reg_link_wkint2gpio4
    _RVM1(0x0e38, 0, BIT3),   //Reg[000e38]#3=0b
    #endif

    #if(PAD_PM_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO5_OEN (PAD_PM_GPIO5_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO5_OUT (PAD_PM_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0a, PAD_PM_GPIO5_OUT, BIT1),
    _RVM1(0x0f0a, PAD_PM_GPIO5_OEN, BIT0),
    //reg_pwm1_mode[1:0]
    _RVM1(0x0e50, 0, BIT3|BIT2),   //Reg[000e50]#3~#2=00b
    #endif

    #if(PAD_PM_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO6_OEN (PAD_PM_GPIO6_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO6_OUT (PAD_PM_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0c, PAD_PM_GPIO6_OUT, BIT1),
    _RVM1(0x0f0c, PAD_PM_GPIO6_OEN, BIT0),
    //reg_pwm0_mode[1:0]
    _RVM1(0x0e50, 0, BIT1|BIT0),   //Reg[000e50]#1~#0=00b
    #endif

    #if(PAD_PM_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO7_OEN (PAD_PM_GPIO7_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO7_OUT (PAD_PM_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0e, PAD_PM_GPIO7_OUT, BIT1),
    _RVM1(0x0f0e, PAD_PM_GPIO7_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO8_OEN (PAD_PM_GPIO8_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO8_OUT (PAD_PM_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f10, PAD_PM_GPIO8_OUT, BIT1),
    _RVM1(0x0f10, PAD_PM_GPIO8_OEN, BIT0),
    //reg_spi_is_gpio[3]
    _RVM1(0x0e6a, BIT3, BIT3),   //Reg[000e6a]#3=1b
    //reg_uart_is_gpio[0]
    _RVM1(0x0e6b, 0, BIT0),   //Reg[000e6b]#0=0b
    #endif

    #if(PAD_PM_GPIO9_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO9_OEN (PAD_PM_GPIO9_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO9_OUT (PAD_PM_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f12, PAD_PM_GPIO9_OUT, BIT1),
    _RVM1(0x0f12, PAD_PM_GPIO9_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO10_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO10_OEN (PAD_PM_GPIO10_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO10_OUT (PAD_PM_GPIO10_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f14, PAD_PM_GPIO10_OUT, BIT1),
    _RVM1(0x0f14, PAD_PM_GPIO10_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO11_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO11_OEN (PAD_PM_GPIO11_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO11_OUT (PAD_PM_GPIO11_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f16, PAD_PM_GPIO11_OUT, BIT1),
    _RVM1(0x0f16, PAD_PM_GPIO11_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO12_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO12_OEN (PAD_PM_GPIO12_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO12_OUT (PAD_PM_GPIO12_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f18, PAD_PM_GPIO12_OUT, BIT1),
    _RVM1(0x0f18, PAD_PM_GPIO12_OEN, BIT0),
    //reg_mspi_mode
    _RVM1(0x0e51, 0, BIT7),   //Reg[000e51]#7=0b
    #endif

    #if(PAD_PM_GPIO13_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO13_OEN (PAD_PM_GPIO13_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO13_OUT (PAD_PM_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1a, PAD_PM_GPIO13_OUT, BIT1),
    _RVM1(0x0f1a, PAD_PM_GPIO13_OEN, BIT0),
    //reg_mspi_mode
    _RVM1(0x0e51, 0, BIT7),   //Reg[000e51]#7=0b
    #endif

    #if(PAD_PM_GPIO14_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO14_OEN (PAD_PM_GPIO14_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO14_OUT (PAD_PM_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1c, PAD_PM_GPIO14_OUT, BIT1),
    _RVM1(0x0f1c, PAD_PM_GPIO14_OEN, BIT0),
    //reg_mspi_mode
    _RVM1(0x0e51, 0, BIT7),   //Reg[000e51]#7=0b
    #endif

    #if(PAD_PM_GPIO15_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO15_OEN (PAD_PM_GPIO15_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO15_OUT (PAD_PM_GPIO15_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1e, PAD_PM_GPIO15_OUT, BIT1),
    _RVM1(0x0f1e, PAD_PM_GPIO15_OEN, BIT0),
    //reg_mspi_mode
    _RVM1(0x0e51, 0, BIT7),   //Reg[000e51]#7=0b
    #endif

    #if(PAD_PM_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_PM_CEC_OEN (PAD_PM_CEC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_CEC_OUT (PAD_PM_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2c, PAD_PM_CEC_OUT, BIT1),
    _RVM1(0x0f2c, PAD_PM_CEC_OEN, BIT0),
    //reg_cec_is_gpio
    _RVM1(0x0e38, BIT6, BIT6),   //Reg[000e38]#6=1b
    #endif

    #if(PAD_HDMITX_HPD_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_HPD_OEN (PAD_HDMITX_HPD_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HDMITX_HPD_OUT (PAD_HDMITX_HPD_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1166, PAD_HDMITX_HPD_OUT, BIT2),
    _RVM1(0x1166, PAD_HDMITX_HPD_OEN, BIT3),
    //reg_hdmi_hpd_bypass
    _RVM1(0x0e4f, 0, BIT7),   //Reg[000e4f]#7=0b
    #endif

    #if(PAD_HDMIRX_HPD_IS_GPIO != GPIO_NONE)
    #define PAD_HDMIRX_HPD_OEN (PAD_HDMIRX_HPD_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_HDMIRX_HPD_OUT (PAD_HDMIRX_HPD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0e4e, PAD_HDMIRX_HPD_OUT, BIT4),
    _RVM1(0x0e4e, PAD_HDMIRX_HPD_OEN, BIT0),
    //reg_hdmi_hpd_bypass
    _RVM1(0x0e4f, 0, BIT7),   //Reg[000e4f]#7=0b
    #endif

    #if(PAD_RX_DDCDA_CK_IS_GPIO != GPIO_NONE)
    #define PAD_RX_DDCDA_CK_OEN (PAD_RX_DDCDA_CK_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_RX_DDCDA_CK_OUT (PAD_RX_DDCDA_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0496, PAD_RX_DDCDA_CK_OUT, BIT2),
    _RVM1(0x0496, PAD_RX_DDCDA_CK_OEN, BIT1),
    //reg_gpio2d0_en
    _RVM1(0x0496, BIT7, BIT7),   //Reg[000496]#7=1b
    #endif

    #if(PAD_RX_DDCDA_DA_IS_GPIO != GPIO_NONE)
    #define PAD_RX_DDCDA_DA_OEN (PAD_RX_DDCDA_DA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_RX_DDCDA_DA_OUT (PAD_RX_DDCDA_DA_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0496, PAD_RX_DDCDA_DA_OUT, BIT6),
    _RVM1(0x0496, PAD_RX_DDCDA_DA_OEN, BIT5),
    //reg_gpio2d0_en
    _RVM1(0x0496, BIT7, BIT7),   //Reg[000496]#7=1b
    #endif

    #if(PAD_PM_SD_CDZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SD_CDZ_OEN (PAD_PM_SD_CDZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SD_CDZ_OUT (PAD_PM_SD_CDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f8e, PAD_PM_SD_CDZ_OUT, BIT1),
    _RVM1(0x0f8e, PAD_PM_SD_CDZ_OEN, BIT0),
    //reg_sd_cdz_mode
    _RVM1(0x0e51, 0, BIT6),   //Reg[000e51]#6=0b
    #endif

    #if(PAD_HOTPLUGA_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGA_HDMI20_5V_OEN (PAD_HOTPLUGA_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HOTPLUGA_HDMI20_5V_OUT (PAD_HOTPLUGA_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0718, PAD_HOTPLUGA_HDMI20_5V_OUT, BIT4),
    _RVM1(0x0718, PAD_HOTPLUGA_HDMI20_5V_OEN, BIT5),
    #endif

    #if(PAD_VID0_IS_GPIO != GPIO_NONE)
    #define PAD_VID0_OEN (PAD_VID0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID0_OUT (PAD_VID0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f90, PAD_VID0_OUT, BIT1),
    _RVM1(0x0f90, PAD_VID0_OEN, BIT0),
    //reg_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=00b
    //reg_gpu_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=00b
    //reg_miic_mode[1:0]
    _RVM1(0x0e50, 0, BIT7|BIT6),   //Reg[000e50]#7~#6=00b
    #endif

    #if(PAD_VID1_IS_GPIO != GPIO_NONE)
    #define PAD_VID1_OEN (PAD_VID1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID1_OUT (PAD_VID1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f92, PAD_VID1_OUT, BIT1),
    _RVM1(0x0f92, PAD_VID1_OEN, BIT0),
    //reg_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT5|BIT4),   //Reg[000e51]#5~#4=00b
    //reg_gpu_vid_mode[1:0]
    _RVM1(0x0e51, 0, BIT3|BIT2),   //Reg[000e51]#3~#2=00b
    //reg_miic_mode[1:0]
    _RVM1(0x0e50, 0, BIT7|BIT6),   //Reg[000e50]#7~#6=00b
    #endif

    #if(PAD_PM_LED0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_LED0_OEN (PAD_PM_LED0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_LED0_OUT (PAD_PM_LED0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f94, PAD_PM_LED0_OUT, BIT1),
    _RVM1(0x0f94, PAD_PM_LED0_OEN, BIT0),
    //reg_led_mode[1:0]
    _RVM1(0x0e50, 0, BIT5|BIT4),   //Reg[000e50]#5~#4=00b
    #endif

    #if(PAD_PM_LED1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_LED1_OEN (PAD_PM_LED1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_LED1_OUT (PAD_PM_LED1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f96, PAD_PM_LED1_OUT, BIT1),
    _RVM1(0x0f96, PAD_PM_LED1_OEN, BIT0),
    //reg_led_mode[1:0]
    _RVM1(0x0e50, 0, BIT5|BIT4),   //Reg[000e50]#5~#4=00b
    #endif

    #if(PAD_SAR_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO0_OEN (PAD_SAR_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR_GPIO0_OUT (PAD_SAR_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR_GPIO0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR_GPIO0_OEN, BIT0),
    //reg_sar_aisel[0]
    _RVM1(0x1422, 0, BIT0),   //Reg[001422]#0=0b
    #endif

    #if(PAD_SAR_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO1_OEN (PAD_SAR_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR_GPIO1_OUT (PAD_SAR_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR_GPIO1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR_GPIO1_OEN, BIT1),
    //reg_sar_aisel[1]
    _RVM1(0x1422, 0, BIT1),   //Reg[001422]#1=0b
    #endif

    #if(PAD_SAR_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO2_OEN (PAD_SAR_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR_GPIO2_OUT (PAD_SAR_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR_GPIO2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR_GPIO2_OEN, BIT2),
    //reg_sar_aisel[2]
    _RVM1(0x1422, 0, BIT2),   //Reg[001422]#2=0b
    #endif

    #if(PAD_SAR_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO3_OEN (PAD_SAR_GPIO3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_SAR_GPIO3_OUT (PAD_SAR_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_SAR_GPIO3_OUT, BIT3),
    _RVM1(0x1423, PAD_SAR_GPIO3_OEN, BIT3),
    //reg_sar_aisel[3]
    _RVM1(0x1422, 0, BIT3),   //Reg[001422]#3=0b
    #endif

    #if(PAD_SAR_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO4_OEN (PAD_SAR_GPIO4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_SAR_GPIO4_OUT (PAD_SAR_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1424, PAD_SAR_GPIO4_OUT, BIT4),
    _RVM1(0x1423, PAD_SAR_GPIO4_OEN, BIT4),
    //reg_sar_aisel[4]
    _RVM1(0x1422, 0, BIT4),   //Reg[001422]#4=0b
    #endif

    _MEMMAP_nonPM_,

    #if(PAD_HSYNC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_HSYNC_OUT_OEN (PAD_HSYNC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HSYNC_OUT_OUT (PAD_HSYNC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ec, PAD_HSYNC_OUT_OUT, BIT4),
    _RVM1(0x25ec, PAD_HSYNC_OUT_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0),   //reg[101e12]#1 ~ #0 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT1|BIT0),   //reg[101e06]#1 ~ #0 = 00b
    #endif

    #if(PAD_VSYNC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_VSYNC_OUT_OEN (PAD_VSYNC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_VSYNC_OUT_OUT (PAD_VSYNC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ee, PAD_VSYNC_OUT_OUT, BIT4),
    _RVM1(0x25ee, PAD_VSYNC_OUT_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0),   //reg[101e12]#1 ~ #0 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT1|BIT0),   //reg[101e06]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2S_OUT_BCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_BCK_OEN (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_BCK_OUT (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2556, PAD_I2S_OUT_BCK_OUT, BIT4),
    _RVM1(0x2556, PAD_I2S_OUT_BCK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_in_mode
    _RVM1(0x1e0b, 0, BIT0),   //reg[101e0b]#0 = 0b
    //reg_i2s_trx_mode
    _RVM1(0x1e0b, 0, BIT2),   //reg[101e0b]#2 = 0b
    //reg_i2s_out_mode
    _RVM1(0x1e0b, 0, BIT4),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_MCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MCK_OEN (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MCK_OUT (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2558, PAD_I2S_OUT_MCK_OUT, BIT4),
    _RVM1(0x2558, PAD_I2S_OUT_MCK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mode
    _RVM1(0x1e0b, 0, BIT4),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_WS_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_WS_OEN (PAD_I2S_OUT_WS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_WS_OUT (PAD_I2S_OUT_WS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255a, PAD_I2S_OUT_WS_OUT, BIT4),
    _RVM1(0x255a, PAD_I2S_OUT_WS_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_in_mode
    _RVM1(0x1e0b, 0, BIT0),   //reg[101e0b]#0 = 0b
    //reg_i2s_out_mode
    _RVM1(0x1e0b, 0, BIT4),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_SD0_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD0_OEN (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD0_OUT (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255c, PAD_I2S_OUT_SD0_OUT, BIT4),
    _RVM1(0x255c, PAD_I2S_OUT_SD0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_spdif_in_mode
    _RVM1(0x1e0a, 0, BIT1|BIT0),   //reg[101e0a]#1 ~ #0 = 00b
    //reg_i2s_in_mode
    _RVM1(0x1e0b, 0, BIT0),   //reg[101e0b]#0 = 0b
    //reg_i2s_trx_mode
    _RVM1(0x1e0b, 0, BIT2),   //reg[101e0b]#2 = 0b
    //reg_i2s_out_mode
    _RVM1(0x1e0b, 0, BIT4),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_SD1_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD1_OEN (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD1_OUT (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255e, PAD_I2S_OUT_SD1_OUT, BIT4),
    _RVM1(0x255e, PAD_I2S_OUT_SD1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_trx_mode
    _RVM1(0x1e0b, 0, BIT2),   //reg[101e0b]#2 = 0b
    //reg_i2s_out_mode2
    _RVM1(0x1e0b, 0, BIT6|BIT5),   //reg[101e0b]#6 ~ #5 = 00b
    #endif

    #if(PAD_I2S_OUT_SD2_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD2_OEN (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD2_OUT (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2560, PAD_I2S_OUT_SD2_OUT, BIT4),
    _RVM1(0x2560, PAD_I2S_OUT_SD2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mode2
    _RVM1(0x1e0b, 0, BIT6|BIT5),   //reg[101e0b]#6 ~ #5 = 00b
    #endif

    #if(PAD_I2S_OUT_SD3_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD3_OEN (PAD_I2S_OUT_SD3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD3_OUT (PAD_I2S_OUT_SD3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2562, PAD_I2S_OUT_SD3_OUT, BIT4),
    _RVM1(0x2562, PAD_I2S_OUT_SD3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mode2
    _RVM1(0x1e0b, 0, BIT6|BIT5),   //reg[101e0b]#6 ~ #5 = 00b
    #endif

    #if(PAD_I2S_OUT_MUTE_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MUTE_OEN (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MUTE_OUT (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2564, PAD_I2S_OUT_MUTE_OUT, BIT4),
    _RVM1(0x2564, PAD_I2S_OUT_MUTE_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mute_mode
    _RVM1(0x1e0b, 0, BIT7),   //reg[101e0b]#7 = 0b
    #endif

    #if(PAD_SD_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_SD_CLK_OEN (PAD_SD_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_CLK_OUT (PAD_SD_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a4, PAD_SD_CLK_OUT, BIT4),
    _RVM1(0x25a4, PAD_SD_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0),   //reg[101e12]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_CMD_IS_GPIO != GPIO_NONE)
    #define PAD_SD_CMD_OEN (PAD_SD_CMD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_CMD_OUT (PAD_SD_CMD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a6, PAD_SD_CMD_OUT, BIT4),
    _RVM1(0x25a6, PAD_SD_CMD_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0),   //reg[101e12]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D0_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D0_OEN (PAD_SD_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D0_OUT (PAD_SD_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a8, PAD_SD_D0_OUT, BIT4),
    _RVM1(0x25a8, PAD_SD_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D1_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D1_OEN (PAD_SD_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D1_OUT (PAD_SD_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25aa, PAD_SD_D1_OUT, BIT4),
    _RVM1(0x25aa, PAD_SD_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D2_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D2_OEN (PAD_SD_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D2_OUT (PAD_SD_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ac, PAD_SD_D2_OUT, BIT4),
    _RVM1(0x25ac, PAD_SD_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D3_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D3_OEN (PAD_SD_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D3_OUT (PAD_SD_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ae, PAD_SD_D3_OUT, BIT4),
    _RVM1(0x25ae, PAD_SD_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO0_OEN (PAD_GPIO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO0_OUT (PAD_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b0, PAD_GPIO0_OUT, BIT4),
    _RVM1(0x25b0, PAD_GPIO0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b 
    //reg_i2cm3_mode
    _RVM1(0x1e12, 0, BIT7|BIT6),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    _RVM1(0x1e07, 0, BIT1|BIT0),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm0_mode
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //reg[101e0e]#1 ~ #0 = 00b
    //reg_usb30vctl_mode
    _RVM1(0x1e26, 0, BIT1|BIT0),   //reg[101e26]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b2, PAD_GPIO1_OUT, BIT4),
    _RVM1(0x25b2, PAD_GPIO1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm3_mode
    _RVM1(0x1e12, 0, BIT7|BIT6),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    _RVM1(0x1e07, 0, BIT1|BIT0),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm0_mode
    _RVM1(0x1e0e, 0, BIT1|BIT0),   //reg[101e0e]#1 ~ #0 = 00b
    //reg_usb30vctl_mode
    _RVM1(0x1e26, 0, BIT1|BIT0),   //reg[101e26]#1 ~ #0 = 00b
    //reg_usb30vctl_mode1
    _RVM1(0x1e27, 0, BIT1|BIT0),   //reg[101e27]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b4, PAD_GPIO2_OUT, BIT4),
    _RVM1(0x25b4, PAD_GPIO2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm3_mode
    _RVM1(0x1e12, 0, BIT7|BIT6),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    _RVM1(0x1e07, 0, BIT1|BIT0),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm1_mode
    _RVM1(0x1e0e, 0, BIT5|BIT4),   //reg[101e0e]#5 ~ #4 = 00b
    //reg_usb30vctl_mode
    _RVM1(0x1e26, 0, BIT1|BIT0),   //reg[101e26]#1 ~ #0 = 00b
    //reg_usb30vctl_mode1
    _RVM1(0x1e27, 0, BIT1|BIT0),   //reg[101e27]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b6, PAD_GPIO3_OUT, BIT4),
    _RVM1(0x25b6, PAD_GPIO3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm3_mode
    _RVM1(0x1e12, 0, BIT7|BIT6),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    _RVM1(0x1e07, 0, BIT1|BIT0),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    _RVM1(0x1e05, 0, BIT3|BIT2),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm1_mode
    _RVM1(0x1e0e, 0, BIT5|BIT4),   //reg[101e0e]#5 ~ #4 = 00b
    //reg_usb30vctl_mode1
    _RVM1(0x1e27, 0, BIT1|BIT0),   //reg[101e27]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b8, PAD_GPIO4_OUT, BIT4),
    _RVM1(0x25b8, PAD_GPIO4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm4_mode
    _RVM1(0x1e13, 0, BIT5|BIT4),   //reg[101e13]#5 ~ #4 = 00b
    //reg_fuart_mode
    _RVM1(0x1e13, 0, BIT3|BIT2),   //reg[101e13]#3 ~ #2 = 00b
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ba, PAD_GPIO5_OUT, BIT4),
    _RVM1(0x25ba, PAD_GPIO5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm4_mode
    _RVM1(0x1e13, 0, BIT5|BIT4),   //reg[101e13]#5 ~ #4 = 00b
    //reg_fuart_mode
    _RVM1(0x1e13, 0, BIT3|BIT2),   //reg[101e13]#3 ~ #2 = 00b
    #endif

    #if(PAD_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO6_OEN (PAD_GPIO6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO6_OUT (PAD_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25bc, PAD_GPIO6_OUT, BIT4),
    _RVM1(0x25bc, PAD_GPIO6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    #endif

    #if(PAD_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO7_OEN (PAD_GPIO7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO7_OUT (PAD_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25be, PAD_GPIO7_OUT, BIT4),
    _RVM1(0x25be, PAD_GPIO7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    #endif

    #if(PAD_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO8_OEN (PAD_GPIO8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO8_OUT (PAD_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c0, PAD_GPIO8_OUT, BIT4),
    _RVM1(0x25c0, PAD_GPIO8_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_spdif_out_mode
    _RVM1(0x1e0a, 0, BIT5|BIT4),   //reg[101e0a]#5 ~ #4 = 00b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    #endif

    #if(PAD_GPIO9_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO9_OEN (PAD_GPIO9_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO9_OUT (PAD_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25c2, PAD_GPIO9_OUT, BIT4),
    _RVM1(0x25c2, PAD_GPIO9_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_spdif_in_mode
    _RVM1(0x1e0a, 0, BIT1|BIT0),   //reg[101e0a]#1 ~ #0 = 00b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    #endif

    #if(PAD_SM0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CLK_OEN (PAD_SM0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CLK_OUT (PAD_SM0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2500, PAD_SM0_CLK_OUT, BIT4),
    _RVM1(0x2500, PAD_SM0_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_sm0_open
    _RVM1(0x1e18, 0, BIT0),   //reg[101e18]#0 = 0b
    //reg_sm0_en
    _RVM1(0x1e19, 0, BIT0),   //reg[101e19]#0 = 0b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    //reg_sm0_gpio_en
    _RVM1(0x1e37, BIT2, BIT2),   //reg[101e37]#2 = 1b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_RST_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_RST_OEN (PAD_SM0_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_RST_OUT (PAD_SM0_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2502, PAD_SM0_RST_OUT, BIT4),
    _RVM1(0x2502, PAD_SM0_RST_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_sm0_open
    _RVM1(0x1e18, 0, BIT0),   //reg[101e18]#0 = 0b
    //reg_sm0_en
    _RVM1(0x1e19, 0, BIT0),   //reg[101e19]#0 = 0b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    //reg_sm0_gpio_en
    _RVM1(0x1e37, BIT3, BIT3),   //reg[101e37]#3 = 1b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_VCC_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_VCC_OEN (PAD_SM0_VCC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_VCC_OUT (PAD_SM0_VCC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2504, PAD_SM0_VCC_OUT, BIT4),
    _RVM1(0x2504, PAD_SM0_VCC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ocp0_en
    _RVM1(0x1e1c, 0, BIT0),   //reg[101e1c]#0 = 0b
    //reg_sm0_en
    _RVM1(0x1e19, 0, BIT0),   //reg[101e19]#0 = 0b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_CD_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CD_OEN (PAD_SM0_CD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CD_OUT (PAD_SM0_CD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2506, PAD_SM0_CD_OUT, BIT4),
    _RVM1(0x2506, PAD_SM0_CD_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_sm0_en
    _RVM1(0x1e19, 0, BIT0),   //reg[101e19]#0 = 0b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    _RVM1(0x1e05, 0, BIT7|BIT6),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_IO_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_IO_OEN (PAD_SM0_IO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_IO_OUT (PAD_SM0_IO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2508, PAD_SM0_IO_OUT, BIT4),
    _RVM1(0x2508, PAD_SM0_IO_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_sm0_io
    _RVM1(0x1e19, 0, BIT4),   //reg[101e19]#4 = 0b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    //reg_sm0_gpio_en
    _RVM1(0x1e37, BIT4, BIT4),   //reg[101e37]#4 = 1b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_VSEL_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_VSEL_OEN (PAD_SM0_VSEL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_VSEL_OUT (PAD_SM0_VSEL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250a, PAD_SM0_VSEL_OUT, BIT4),
    _RVM1(0x250a, PAD_SM0_VSEL_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ocp0_en
    _RVM1(0x1e1c, 0, BIT0),   //reg[101e1c]#0 = 0b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    //reg_sd_config
    _RVM1(0x1e11, 0, BIT1|BIT0),   //reg[101e11]#1 ~ #0 = 00b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_C4_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_C4_OEN (PAD_SM0_C4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_C4_OUT (PAD_SM0_C4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250c, PAD_SM0_C4_OUT, BIT4),
    _RVM1(0x250c, PAD_SM0_C4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ocp0_en
    _RVM1(0x1e1c, 0, BIT0),   //reg[101e1c]#0 = 0b
    //reg_sm0_c48
    _RVM1(0x1e18, 0, BIT5|BIT4),   //reg[101e18]#5 ~ #4 = 00b
    //reg_i2cm2_mode
    _RVM1(0x1e12, 0, BIT5|BIT4),   //reg[101e12]#5 ~ #4 = 00b
    //reg_sm0_gpio_en
    _RVM1(0x1e37, BIT5, BIT5),   //reg[101e37]#5 = 1b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_SM0_C8_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_C8_OEN (PAD_SM0_C8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_C8_OUT (PAD_SM0_C8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250d, PAD_SM0_C8_OUT, BIT4),
    _RVM1(0x250d, PAD_SM0_C8_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_sm0_c48
    _RVM1(0x1e18, 0, BIT5|BIT4),   //reg[101e18]#5 ~ #4 = 00b
    //reg_i2cm2_mode
    _RVM1(0x1e12, 0, BIT5|BIT4),   //reg[101e12]#5 ~ #4 = 00b
    //reg_sm0_gpio_en
    _RVM1(0x1e37, BIT6, BIT6),   //reg[101e37]#6 = 1b
    _RVM1(0x2958, 0, BIT6),   //reg[102958]#6 = 0b
    #endif

    #if(PAD_CI_RST_IS_GPIO != GPIO_NONE)
    #define PAD_CI_RST_OEN (PAD_CI_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_RST_OUT (PAD_CI_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x269a, PAD_CI_RST_OUT, BIT4),
    _RVM1(0x269a, PAD_CI_RST_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_WAITZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_WAITZ_OEN (PAD_CI_WAITZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_WAITZ_OUT (PAD_CI_WAITZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x269c, PAD_CI_WAITZ_OUT, BIT4),
    _RVM1(0x269c, PAD_CI_WAITZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_IRQAZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_IRQAZ_OEN (PAD_CI_IRQAZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_IRQAZ_OUT (PAD_CI_IRQAZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x269e, PAD_CI_IRQAZ_OUT, BIT4),
    _RVM1(0x269e, PAD_CI_IRQAZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_IORDZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_IORDZ_OEN (PAD_CI_IORDZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_IORDZ_OUT (PAD_CI_IORDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26a0, PAD_CI_IORDZ_OUT, BIT4),
    _RVM1(0x26a0, PAD_CI_IORDZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_REGZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_REGZ_OEN (PAD_CI_REGZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_REGZ_OUT (PAD_CI_REGZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26a2, PAD_CI_REGZ_OUT, BIT4),
    _RVM1(0x26a2, PAD_CI_REGZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_WEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_WEZ_OEN (PAD_CI_WEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_WEZ_OUT (PAD_CI_WEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26a4, PAD_CI_WEZ_OUT, BIT4),
    _RVM1(0x26a4, PAD_CI_WEZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_IOWRZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_IOWRZ_OEN (PAD_CI_IOWRZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_IOWRZ_OUT (PAD_CI_IOWRZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26a6, PAD_CI_IOWRZ_OUT, BIT4),
    _RVM1(0x26a6, PAD_CI_IOWRZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_CEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_CEZ_OEN (PAD_CI_CEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_CEZ_OUT (PAD_CI_CEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26a8, PAD_CI_CEZ_OUT, BIT4),
    _RVM1(0x26a8, PAD_CI_CEZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_OEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_OEZ_OEN (PAD_CI_OEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_OEZ_OUT (PAD_CI_OEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26aa, PAD_CI_OEZ_OUT, BIT4),
    _RVM1(0x26aa, PAD_CI_OEZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_CDZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_CDZ_OEN (PAD_CI_CDZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_CDZ_OUT (PAD_CI_CDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26ac, PAD_CI_CDZ_OUT, BIT4),
    _RVM1(0x26ac, PAD_CI_CDZ_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sec_mspi2_mode
    _RVM1(0x3908, 0, BIT0),   //reg[123908]#0 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A0_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A0_OEN (PAD_CI_A0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A0_OUT (PAD_CI_A0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26ae, PAD_CI_A0_OUT, BIT4),
    _RVM1(0x26ae, PAD_CI_A0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A1_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A1_OEN (PAD_CI_A1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A1_OUT (PAD_CI_A1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26b0, PAD_CI_A1_OUT, BIT4),
    _RVM1(0x26b0, PAD_CI_A1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sec_mspi2_mode
    _RVM1(0x3908, 0, BIT0),   //reg[123908]#0 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A2_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A2_OEN (PAD_CI_A2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A2_OUT (PAD_CI_A2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26b2, PAD_CI_A2_OUT, BIT4),
    _RVM1(0x26b2, PAD_CI_A2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A3_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A3_OEN (PAD_CI_A3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A3_OUT (PAD_CI_A3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26b4, PAD_CI_A3_OUT, BIT4),
    _RVM1(0x26b4, PAD_CI_A3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A4_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A4_OEN (PAD_CI_A4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A4_OUT (PAD_CI_A4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26b6, PAD_CI_A4_OUT, BIT4),
    _RVM1(0x26b6, PAD_CI_A4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A5_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A5_OEN (PAD_CI_A5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A5_OUT (PAD_CI_A5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26b8, PAD_CI_A5_OUT, BIT4),
    _RVM1(0x26b8, PAD_CI_A5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A6_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A6_OEN (PAD_CI_A6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A6_OUT (PAD_CI_A6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26ba, PAD_CI_A6_OUT, BIT4),
    _RVM1(0x26ba, PAD_CI_A6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A7_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A7_OEN (PAD_CI_A7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A7_OUT (PAD_CI_A7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26bc, PAD_CI_A7_OUT, BIT4),
    _RVM1(0x26bc, PAD_CI_A7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A8_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A8_OEN (PAD_CI_A8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A8_OUT (PAD_CI_A8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26be, PAD_CI_A8_OUT, BIT4),
    _RVM1(0x26be, PAD_CI_A8_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A9_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A9_OEN (PAD_CI_A9_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A9_OUT (PAD_CI_A9_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26c0, PAD_CI_A9_OUT, BIT4),
    _RVM1(0x26c0, PAD_CI_A9_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A10_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A10_OEN (PAD_CI_A10_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A10_OUT (PAD_CI_A10_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26c2, PAD_CI_A10_OUT, BIT4),
    _RVM1(0x26c2, PAD_CI_A10_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A11_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A11_OEN (PAD_CI_A11_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A11_OUT (PAD_CI_A11_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26c4, PAD_CI_A11_OUT, BIT4),
    _RVM1(0x26c4, PAD_CI_A11_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A12_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A12_OEN (PAD_CI_A12_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A12_OUT (PAD_CI_A12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26c6, PAD_CI_A12_OUT, BIT4),
    _RVM1(0x26c6, PAD_CI_A12_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A13_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A13_OEN (PAD_CI_A13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A13_OUT (PAD_CI_A13_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26c8, PAD_CI_A13_OUT, BIT4),
    _RVM1(0x26c8, PAD_CI_A13_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_A14_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A14_OEN (PAD_CI_A14_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A14_OUT (PAD_CI_A14_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26ca, PAD_CI_A14_OUT, BIT4),
    _RVM1(0x26ca, PAD_CI_A14_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D0_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D0_OEN (PAD_CI_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D0_OUT (PAD_CI_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26cc, PAD_CI_D0_OUT, BIT4),
    _RVM1(0x26cc, PAD_CI_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sec_mspi2_mode
    _RVM1(0x3908, 0, BIT0),   //reg[123908]#0 = 0b
    //reg_sm1_open
    _RVM1(0x1e18, 0, BIT1),   //reg[101e18]#1 = 0b
    //reg_sm1_en
    _RVM1(0x1e19, 0, BIT1),   //reg[101e19]#1 = 0b
    //reg_i2cm4_mode
    _RVM1(0x1e13, 0, BIT5|BIT4),   //reg[101e13]#5 ~ #4 = 00b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D1_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D1_OEN (PAD_CI_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D1_OUT (PAD_CI_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26ce, PAD_CI_D1_OUT, BIT4),
    _RVM1(0x26ce, PAD_CI_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sm1_open
    _RVM1(0x1e18, 0, BIT1),   //reg[101e18]#1 = 0b
    //reg_sm1_en
    _RVM1(0x1e19, 0, BIT1),   //reg[101e19]#1 = 0b
    //reg_i2cm4_mode = 0b
    _RVM1(0x1e13, 0, BIT5|BIT4),   //reg[101e13]#5 ~ #4 = 00b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D2_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D2_OEN (PAD_CI_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D2_OUT (PAD_CI_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26d0, PAD_CI_D2_OUT, BIT4),
    _RVM1(0x26d0, PAD_CI_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sec_mspi2_mode
    _RVM1(0x3908, 0, BIT0),   //reg[123908]#0 = 0b
    //reg_ocp1_en
    _RVM1(0x1e1c, 0, BIT1),   //reg[101e1c]#1 = 0b
    //reg_sm1_en
    _RVM1(0x1e19, 0, BIT1),   //reg[101e19]#1 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D3_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D3_OEN (PAD_CI_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D3_OUT (PAD_CI_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26d2, PAD_CI_D3_OUT, BIT4),
    _RVM1(0x26d2, PAD_CI_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sm1_en
    _RVM1(0x1e19, 0, BIT1),   //reg[101e19]#1 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D4_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D4_OEN (PAD_CI_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D4_OUT (PAD_CI_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26d4, PAD_CI_D4_OUT, BIT4),
    _RVM1(0x26d4, PAD_CI_D4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sec_mspi2_mode
    _RVM1(0x3908, 0, BIT0),   //reg[123908]#0 = 0b
    //reg_sm1_io
    _RVM1(0x1e19, 0, BIT5),   //reg[101e19]#5 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D5_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D5_OEN (PAD_CI_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D5_OUT (PAD_CI_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26d6, PAD_CI_D5_OUT, BIT4),
    _RVM1(0x26d6, PAD_CI_D5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ocp1_en
    _RVM1(0x1e1c, 0, BIT1),   //reg[101e1c]#1 = 0b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D6_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D6_OEN (PAD_CI_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D6_OUT (PAD_CI_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26d8, PAD_CI_D6_OUT, BIT4),
    _RVM1(0x26d8, PAD_CI_D6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sec_mspi2_mode
    _RVM1(0x3908, 0, BIT0),   //reg[123908]#0 = 0b
    //reg_ocp1_en
    _RVM1(0x1e1c, 0, BIT1),   //reg[101e1c]#1 = 0b
    //reg_sm1_c48
    _RVM1(0x1e18, 0, BIT7|BIT6),   //reg[101e18]#7 ~ #6 = 00b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_CI_D7_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D7_OEN (PAD_CI_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D7_OUT (PAD_CI_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x26da, PAD_CI_D7_OUT, BIT4),
    _RVM1(0x26da, PAD_CI_D7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sm1_c48
    _RVM1(0x1e18, 0, BIT7|BIT6),   //reg[101e18]#7 ~ #6 = 00b
    //reg_ci_mode
    _RVM1(0x1e0e, 0, BIT6),   //reg[101e0e]#6 = 0b
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2524, PAD_TS0_CLK_OUT, BIT4),
    _RVM1(0x2524, PAD_TS0_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2526, PAD_TS0_SYNC_OUT, BIT4),
    _RVM1(0x2526, PAD_TS0_SYNC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    //reg_fuart_mode
    _RVM1(0x1e13, 0, BIT3|BIT2),   //reg[101e13]#3 ~ #2 = 00b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2528, PAD_TS0_VLD_OUT, BIT4),
    _RVM1(0x2528, PAD_TS0_VLD_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    //reg_fuart_mode
    _RVM1(0x1e13, 0, BIT3|BIT2),   //reg[101e13]#3 ~ #2 = 00b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252a, PAD_TS0_D0_OUT, BIT4),
    _RVM1(0x252a, PAD_TS0_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252c, PAD_TS0_D1_OUT, BIT4),
    _RVM1(0x252c, PAD_TS0_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252e, PAD_TS0_D2_OUT, BIT4),
    _RVM1(0x252e, PAD_TS0_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2530, PAD_TS0_D3_OUT, BIT4),
    _RVM1(0x2530, PAD_TS0_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2532, PAD_TS0_D4_OUT, BIT4),
    _RVM1(0x2532, PAD_TS0_D4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT1|BIT0),   //reg[101e06]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2534, PAD_TS0_D5_OUT, BIT4),
    _RVM1(0x2534, PAD_TS0_D5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT1|BIT0),   //reg[101e06]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2536, PAD_TS0_D6_OUT, BIT4),
    _RVM1(0x2536, PAD_TS0_D6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2538, PAD_TS0_D7_OUT, BIT4),
    _RVM1(0x2538, PAD_TS0_D7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    _RVM1(0x1e04, 0, BIT0),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS1_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_CLK_OEN (PAD_TS1_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_CLK_OUT (PAD_TS1_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263a, PAD_TS1_CLK_OUT, BIT4),
    _RVM1(0x263a, PAD_TS1_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_SYNC_OEN (PAD_TS1_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_SYNC_OUT (PAD_TS1_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263c, PAD_TS1_SYNC_OUT, BIT4),
    _RVM1(0x263c, PAD_TS1_SYNC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_VLD_OEN (PAD_TS1_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_VLD_OUT (PAD_TS1_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263e, PAD_TS1_VLD_OUT, BIT4),
    _RVM1(0x263e, PAD_TS1_VLD_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D0_OEN (PAD_TS1_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D0_OUT (PAD_TS1_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2640, PAD_TS1_D0_OUT, BIT4),
    _RVM1(0x2640, PAD_TS1_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D1_OEN (PAD_TS1_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D1_OUT (PAD_TS1_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2642, PAD_TS1_D1_OUT, BIT4),
    _RVM1(0x2642, PAD_TS1_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D2_OEN (PAD_TS1_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D2_OUT (PAD_TS1_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2644, PAD_TS1_D2_OUT, BIT4),
    _RVM1(0x2644, PAD_TS1_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D3_OEN (PAD_TS1_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D3_OUT (PAD_TS1_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2646, PAD_TS1_D3_OUT, BIT4),
    _RVM1(0x2646, PAD_TS1_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D4_OEN (PAD_TS1_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D4_OUT (PAD_TS1_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2648, PAD_TS1_D4_OUT, BIT4),
    _RVM1(0x2648, PAD_TS1_D4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D5_OEN (PAD_TS1_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D5_OUT (PAD_TS1_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x264a, PAD_TS1_D5_OUT, BIT4),
    _RVM1(0x264a, PAD_TS1_D5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D6_OEN (PAD_TS1_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D6_OUT (PAD_TS1_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x264c, PAD_TS1_D6_OUT, BIT4),
    _RVM1(0x264c, PAD_TS1_D6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS1_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D7_OEN (PAD_TS1_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D7_OUT (PAD_TS1_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x264e, PAD_TS1_D7_OUT, BIT4),
    _RVM1(0x264e, PAD_TS1_D7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts1_mode
    _RVM1(0x1e04, 0, BIT2|BIT1),   //reg[101e04]#2 ~ #1 = 00b
    //reg_ts_out_mode
    _RVM1(0x1e0f, 0, BIT7|BIT6),   //reg[101e0f]#7 ~ #6 = 00b
    #endif

    #if(PAD_TS2_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_CLK_OEN (PAD_TS2_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_CLK_OUT (PAD_TS2_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2650, PAD_TS2_CLK_OUT, BIT4),
    _RVM1(0x2650, PAD_TS2_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_ts2_mode
    _RVM1(0x1e04, 0, BIT4|BIT3),   //reg[101e04]#4 ~ #3 = 00b
    #endif

    #if(PAD_TS2_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_SYNC_OEN (PAD_TS2_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_SYNC_OUT (PAD_TS2_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2652, PAD_TS2_SYNC_OUT, BIT4),
    _RVM1(0x2652, PAD_TS2_SYNC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_VLD_OEN (PAD_TS2_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_VLD_OUT (PAD_TS2_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2654, PAD_TS2_VLD_OUT, BIT4),
    _RVM1(0x2654, PAD_TS2_VLD_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D0_OEN (PAD_TS2_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D0_OUT (PAD_TS2_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2656, PAD_TS2_D0_OUT, BIT4),
    _RVM1(0x2656, PAD_TS2_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D1_OEN (PAD_TS2_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D1_OUT (PAD_TS2_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2658, PAD_TS2_D1_OUT, BIT4),
    _RVM1(0x2658, PAD_TS2_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D2_OEN (PAD_TS2_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D2_OUT (PAD_TS2_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x265a, PAD_TS2_D2_OUT, BIT4),
    _RVM1(0x265a, PAD_TS2_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D3_OEN (PAD_TS2_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D3_OUT (PAD_TS2_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x265c, PAD_TS2_D3_OUT, BIT4),
    _RVM1(0x265c, PAD_TS2_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D4_OEN (PAD_TS2_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D4_OUT (PAD_TS2_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x265e, PAD_TS2_D4_OUT, BIT4),
    _RVM1(0x265e, PAD_TS2_D4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D5_OEN (PAD_TS2_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D5_OUT (PAD_TS2_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2660, PAD_TS2_D5_OUT, BIT4),
    _RVM1(0x2660, PAD_TS2_D5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D6_OEN (PAD_TS2_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D6_OUT (PAD_TS2_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2662, PAD_TS2_D6_OUT, BIT4),
    _RVM1(0x2662, PAD_TS2_D6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_TS2_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D7_OEN (PAD_TS2_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_D7_OUT (PAD_TS2_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2664, PAD_TS2_D7_OUT, BIT4),
    _RVM1(0x2664, PAD_TS2_D7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    #endif

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a0, PAD_SPDIF_OUT_OUT, BIT4),
    _RVM1(0x25a0, PAD_SPDIF_OUT_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_spdif_out_mode
    _RVM1(0x1e0a, 0, BIT5|BIT4),   //reg[101e0a]#5 ~ #4 = 00b
    //reg_ts2_mode
    _RVM1(0x1e04, 0, BIT4|BIT3),   //reg[101e04]#4 ~ #3 = 00b
    #endif

    #if(PAD_I2CM0_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SCL_OEN (PAD_I2CM0_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SCL_OUT (PAD_I2CM0_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251c, PAD_I2CM0_SCL_OUT, BIT4),
    _RVM1(0x251c, PAD_I2CM0_SCL_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0),   //reg[101e12]#1 ~ #0 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT1|BIT0),   //reg[101e06]#1 ~ #0 = 00b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2CM0_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SDA_OEN (PAD_I2CM0_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SDA_OUT (PAD_I2CM0_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251e, PAD_I2CM0_SDA_OUT, BIT4),
    _RVM1(0x251e, PAD_I2CM0_SDA_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    _RVM1(0x1e24, 0, BIT1|BIT0),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    _RVM1(0x1e12, 0, BIT1|BIT0),   //reg[101e12]#1 ~ #0 = 00b
    //reg_SecondUARTMode
    _RVM1(0x1e06, 0, BIT1|BIT0),   //reg[101e06]#1 ~ #0 = 00b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2CM1_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SCL_OEN (PAD_I2CM1_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SCL_OUT (PAD_I2CM1_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2520, PAD_I2CM1_SCL_OUT, BIT4),
    _RVM1(0x2520, PAD_I2CM1_SCL_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    //reg_ThirdUARTMode
    _RVM1(0x1e06, 0, BIT5|BIT4),   //reg[101e06]#5 ~ #4 = 00b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2CM1_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SDA_OEN (PAD_I2CM1_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SDA_OUT (PAD_I2CM1_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2522, PAD_I2CM1_SDA_OUT, BIT4),
    _RVM1(0x2522, PAD_I2CM1_SDA_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_i2cm1_mode
    _RVM1(0x1e12, 0, BIT3|BIT2),   //reg[101e12]#3 ~ #2 = 00b
    //reg_ThirdUARTMode
    _RVM1(0x1e06, 0, BIT5|BIT4),   //reg[101e06]#5 ~ #4 = 00b
    //reg_mspi1_mode1
    _RVM1(0x1e05, 0, BIT1|BIT0),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_HDMITX_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_SCL_OEN (PAD_HDMITX_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HDMITX_SCL_OUT (PAD_HDMITX_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25da, PAD_HDMITX_SCL_OUT, BIT4),
    _RVM1(0x25da, PAD_HDMITX_SCL_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_i2cm2_mode
    _RVM1(0x1e12, 0, BIT5|BIT4),   //reg[101e12]#5 ~ #4 = 00b
    //reg_ThirdUARTMode
    _RVM1(0x1e06, 0, BIT5|BIT4),   //reg[101e06]#5 ~ #4 = 00b
    //reg_hdmitx_ddc_mode
    _RVM1(0x1e17, 0, BIT4),   //reg[101e17]#4 = 0b
    #endif

    #if(PAD_HDMITX_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_SDA_OEN (PAD_HDMITX_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HDMITX_SDA_OUT (PAD_HDMITX_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25dc, PAD_HDMITX_SDA_OUT, BIT4),
    _RVM1(0x25dc, PAD_HDMITX_SDA_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_i2cm2_mode
    _RVM1(0x1e12, 0, BIT5|BIT4),   //reg[101e12]#5 ~ #4 = 00b
    //reg_ThirdUARTMode
    _RVM1(0x1e06, 0, BIT5|BIT4),   //reg[101e06]#5 ~ #4 = 00b
    //reg_hdmitx_ddc_mode
    _RVM1(0x1e17, 0, BIT4),   //reg[101e17]#4 = 0b
    #endif

    #if(PAD_HDMITX_ARC_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_ARC_OEN (PAD_HDMITX_ARC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HDMITX_ARC_OUT (PAD_HDMITX_ARC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25de, PAD_HDMITX_ARC_OUT, BIT4),
    _RVM1(0x25de, PAD_HDMITX_ARC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_hdmitx_arc_mode
    _RVM1(0x1e17, 0, BIT0),   //reg[101e17]#0 = 0b
    //reg_hdmitx_arc_gpio_en
    _RVM1(0x1e50, BIT3, BIT3),   //reg[101e50]#3 = 1b
    #endif

    #if(PAD_HDMIRX_ARC_IS_GPIO != GPIO_NONE)
    #define PAD_HDMIRX_ARC_OEN (PAD_HDMIRX_ARC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HDMIRX_ARC_OUT (PAD_HDMIRX_ARC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25e0, PAD_HDMIRX_ARC_OUT, BIT4),
    _RVM1(0x25e0, PAD_HDMIRX_ARC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_hdmirx_arc_mode
    _RVM1(0x1e16, 0, BIT4),   //reg[101e16]#4 = 0b
    //reg_hdmirx_arc_gpio_en
    _RVM1(0x1e50, BIT0, BIT0),   //reg[101e50]#0 = 1b
    #endif

    #if(PAD_EMMC_IO0_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO0_OEN (PAD_EMMC_IO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO0_OUT (PAD_EMMC_IO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2570, PAD_EMMC_IO0_OUT, BIT4),
    _RVM1(0x2570, PAD_EMMC_IO0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO1_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO1_OEN (PAD_EMMC_IO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO1_OUT (PAD_EMMC_IO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2572, PAD_EMMC_IO1_OUT, BIT4),
    _RVM1(0x2572, PAD_EMMC_IO1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO2_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO2_OEN (PAD_EMMC_IO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO2_OUT (PAD_EMMC_IO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2574, PAD_EMMC_IO2_OUT, BIT4),
    _RVM1(0x2574, PAD_EMMC_IO2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO3_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO3_OEN (PAD_EMMC_IO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO3_OUT (PAD_EMMC_IO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2576, PAD_EMMC_IO3_OUT, BIT4),
    _RVM1(0x2576, PAD_EMMC_IO3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO4_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO4_OEN (PAD_EMMC_IO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO4_OUT (PAD_EMMC_IO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2578, PAD_EMMC_IO4_OUT, BIT4),
    _RVM1(0x2578, PAD_EMMC_IO4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO5_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO5_OEN (PAD_EMMC_IO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO5_OUT (PAD_EMMC_IO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257a, PAD_EMMC_IO5_OUT, BIT4),
    _RVM1(0x257a, PAD_EMMC_IO5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO6_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO6_OEN (PAD_EMMC_IO6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO6_OUT (PAD_EMMC_IO6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257c, PAD_EMMC_IO6_OUT, BIT4),
    _RVM1(0x257c, PAD_EMMC_IO6_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO7_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO7_OEN (PAD_EMMC_IO7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO7_OUT (PAD_EMMC_IO7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257e, PAD_EMMC_IO7_OUT, BIT4),
    _RVM1(0x257e, PAD_EMMC_IO7_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO8_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO8_OEN (PAD_EMMC_IO8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO8_OUT (PAD_EMMC_IO8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2580, PAD_EMMC_IO8_OUT, BIT4),
    _RVM1(0x2580, PAD_EMMC_IO8_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO9_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO9_OEN (PAD_EMMC_IO9_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO9_OUT (PAD_EMMC_IO9_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2582, PAD_EMMC_IO9_OUT, BIT4),
    _RVM1(0x2582, PAD_EMMC_IO9_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO10_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO10_OEN (PAD_EMMC_IO10_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO10_OUT (PAD_EMMC_IO10_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2584, PAD_EMMC_IO10_OUT, BIT4),
    _RVM1(0x2584, PAD_EMMC_IO10_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO11_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO11_OEN (PAD_EMMC_IO11_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO11_OUT (PAD_EMMC_IO11_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2586, PAD_EMMC_IO11_OUT, BIT4),
    _RVM1(0x2586, PAD_EMMC_IO11_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO12_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO12_OEN (PAD_EMMC_IO12_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO12_OUT (PAD_EMMC_IO12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2588, PAD_EMMC_IO12_OUT, BIT4),
    _RVM1(0x2588, PAD_EMMC_IO12_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO13_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO13_OEN (PAD_EMMC_IO13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO13_OUT (PAD_EMMC_IO13_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258a, PAD_EMMC_IO13_OUT, BIT4),
    _RVM1(0x258a, PAD_EMMC_IO13_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO14_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO14_OEN (PAD_EMMC_IO14_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO14_OUT (PAD_EMMC_IO14_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258c, PAD_EMMC_IO14_OUT, BIT4),
    _RVM1(0x258c, PAD_EMMC_IO14_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO15_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO15_OEN (PAD_EMMC_IO15_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO15_OUT (PAD_EMMC_IO15_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258e, PAD_EMMC_IO15_OUT, BIT4),
    _RVM1(0x258e, PAD_EMMC_IO15_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO16_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO16_OEN (PAD_EMMC_IO16_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO16_OUT (PAD_EMMC_IO16_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2590, PAD_EMMC_IO16_OUT, BIT4),
    _RVM1(0x2590, PAD_EMMC_IO16_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_EMMC_IO17_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO17_OEN (PAD_EMMC_IO17_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO17_OUT (PAD_EMMC_IO17_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2592, PAD_EMMC_IO17_OUT, BIT4),
    _RVM1(0x2592, PAD_EMMC_IO17_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_nand_mode[0]
    _RVM1(0x1e10, 0, BIT3),   //reg[101e10]#3 = 0b
    //reg_emmc_config[0]
    _RVM1(0x1e10, 0, BIT0),   //reg[101e10]#0 = 0b
    #endif

    #if(PAD_SDIO_IO0_IS_GPIO != GPIO_NONE)
    #define PAD_SDIO_IO0_OEN (PAD_SDIO_IO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SDIO_IO0_OUT (PAD_SDIO_IO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2594, PAD_SDIO_IO0_OUT, BIT4),
    _RVM1(0x2594, PAD_SDIO_IO0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sdio_mode
    _RVM1(0x1e11, 0, BIT5),   //reg[101e11]#5 = 0b
    #endif

    #if(PAD_SDIO_IO1_IS_GPIO != GPIO_NONE)
    #define PAD_SDIO_IO1_OEN (PAD_SDIO_IO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SDIO_IO1_OUT (PAD_SDIO_IO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2596, PAD_SDIO_IO1_OUT, BIT4),
    _RVM1(0x2596, PAD_SDIO_IO1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sdio_mode
    _RVM1(0x1e11, 0, BIT5),   //reg[101e11]#5 = 0b
    #endif

    #if(PAD_SDIO_IO2_IS_GPIO != GPIO_NONE)
    #define PAD_SDIO_IO2_OEN (PAD_SDIO_IO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SDIO_IO2_OUT (PAD_SDIO_IO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2598, PAD_SDIO_IO2_OUT, BIT4),
    _RVM1(0x2598, PAD_SDIO_IO2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sdio_mode
    _RVM1(0x1e11, 0, BIT5),   //reg[101e11]#5 = 0b
    #endif

    #if(PAD_SDIO_IO3_IS_GPIO != GPIO_NONE)
    #define PAD_SDIO_IO3_OEN (PAD_SDIO_IO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SDIO_IO3_OUT (PAD_SDIO_IO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259a, PAD_SDIO_IO3_OUT, BIT4),
    _RVM1(0x259a, PAD_SDIO_IO3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sdio_mode
    _RVM1(0x1e11, 0, BIT5),   //reg[101e11]#5 = 0b
    #endif

    #if(PAD_SDIO_IO4_IS_GPIO != GPIO_NONE)
    #define PAD_SDIO_IO4_OEN (PAD_SDIO_IO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SDIO_IO4_OUT (PAD_SDIO_IO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259c, PAD_SDIO_IO4_OUT, BIT4),
    _RVM1(0x259c, PAD_SDIO_IO4_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sdio_mode
    _RVM1(0x1e11, 0, BIT5),   //reg[101e11]#5 = 0b
    #endif

    #if(PAD_SDIO_IO5_IS_GPIO != GPIO_NONE)
    #define PAD_SDIO_IO5_OEN (PAD_SDIO_IO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SDIO_IO5_OUT (PAD_SDIO_IO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259e, PAD_SDIO_IO5_OUT, BIT4),
    _RVM1(0x259e, PAD_SDIO_IO5_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_sdio_mode
    _RVM1(0x1e11, 0, BIT5),   //reg[101e11]#5 = 0b
    #endif

    #if(PAD_GT0_MDIO_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_MDIO_OEN (PAD_GT0_MDIO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_MDIO_OUT (PAD_GT0_MDIO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253a, PAD_GT0_MDIO_OUT, BIT4),
    _RVM1(0x253a, PAD_GT0_MDIO_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_MDC_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_MDC_OEN (PAD_GT0_MDC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_MDC_OUT (PAD_GT0_MDC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253c, PAD_GT0_MDC_OUT, BIT4),
    _RVM1(0x253c, PAD_GT0_MDC_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_CLK_OEN (PAD_GT0_RX_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_CLK_OUT (PAD_GT0_RX_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253e, PAD_GT0_RX_CLK_OUT, BIT4),
    _RVM1(0x253e, PAD_GT0_RX_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_CTL_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_CTL_OEN (PAD_GT0_RX_CTL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_CTL_OUT (PAD_GT0_RX_CTL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2540, PAD_GT0_RX_CTL_OUT, BIT4),
    _RVM1(0x2540, PAD_GT0_RX_CTL_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D0_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D0_OEN (PAD_GT0_RX_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D0_OUT (PAD_GT0_RX_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2542, PAD_GT0_RX_D0_OUT, BIT4),
    _RVM1(0x2542, PAD_GT0_RX_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D1_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D1_OEN (PAD_GT0_RX_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D1_OUT (PAD_GT0_RX_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2544, PAD_GT0_RX_D1_OUT, BIT4),
    _RVM1(0x2544, PAD_GT0_RX_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D2_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D2_OEN (PAD_GT0_RX_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D2_OUT (PAD_GT0_RX_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2546, PAD_GT0_RX_D2_OUT, BIT4),
    _RVM1(0x2546, PAD_GT0_RX_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D3_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D3_OEN (PAD_GT0_RX_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D3_OUT (PAD_GT0_RX_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2548, PAD_GT0_RX_D3_OUT, BIT4),
    _RVM1(0x2548, PAD_GT0_RX_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_CLK_OEN (PAD_GT0_TX_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_CLK_OUT (PAD_GT0_TX_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254a, PAD_GT0_TX_CLK_OUT, BIT4),
    _RVM1(0x254a, PAD_GT0_TX_CLK_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_CTL_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_CTL_OEN (PAD_GT0_TX_CTL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_CTL_OUT (PAD_GT0_TX_CTL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254c, PAD_GT0_TX_CTL_OUT, BIT4),
    _RVM1(0x254c, PAD_GT0_TX_CTL_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D0_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D0_OEN (PAD_GT0_TX_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D0_OUT (PAD_GT0_TX_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254e, PAD_GT0_TX_D0_OUT, BIT4),
    _RVM1(0x254e, PAD_GT0_TX_D0_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D1_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D1_OEN (PAD_GT0_TX_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D1_OUT (PAD_GT0_TX_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2550, PAD_GT0_TX_D1_OUT, BIT4),
    _RVM1(0x2550, PAD_GT0_TX_D1_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D2_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D2_OEN (PAD_GT0_TX_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D2_OUT (PAD_GT0_TX_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2552, PAD_GT0_TX_D2_OUT, BIT4),
    _RVM1(0x2552, PAD_GT0_TX_D2_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D3_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D3_OEN (PAD_GT0_TX_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D3_OUT (PAD_GT0_TX_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2554, PAD_GT0_TX_D3_OUT, BIT4),
    _RVM1(0x2554, PAD_GT0_TX_D3_OEN, BIT5),
    //reg_allpad_in
    _RVM1(0x1ea1, 0, BIT7),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    _RVM1(0x1e0c, 0, BIT0),   //reg[101e0c]#0 = 0b
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------
    // For PM PAD MUX
    _MEMMAP_PM_,
    #if (PAD_PM_LED_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x0E50, PAD_PM_LED_MODE << 4, BITMASK(5:4)),
    #endif

    // For nonPM PAD MUX
	_MEMMAP_nonPM_,

	#if (PADS_SM0_OPEN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E18, PADS_SM0_OPEN, BIT0),
    #endif

	#if (PADS_OCP0_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E1C, PADS_OCP0_EN, BIT0),
    #endif

	#if (PADS_SM0_C48!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E18, PADS_SM0_C48 << 4, BITMASK(5:4)),
    #endif

	#if (PADS_SM0_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E19, PADS_SM0_EN, BIT0),
    #endif

	#if (PADS_SM0_IO!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E19, PADS_SM0_IO << 4, BIT4),
    #endif

	#if (PADS_SM1_OPEN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E18, PADS_SM1_OPEN << 1, BIT1),
    #endif

	#if (PADS_OCP1_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E1C, PADS_OCP1_EN << 1, BIT1),
    #endif

	#if (PADS_SM1_C48!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E18, PADS_SM1_C48 << 6, BITMASK(7:6)),
    #endif

	#if (PADS_SM1_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E19, PADS_SM1_EN << 1, BIT1),
    #endif

    #if (PADS_SM1_IO!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E19, PADS_SM1_IO << 5, BIT5),
    #endif

	#if (PADS_I2CM0_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E12, PADS_I2CM0_MODE, BITMASK(1:0)),
    #endif

	#if (PADS_I2CM1_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E12, PADS_I2CM1_MODE << 2, BITMASK(3:2)),
    #endif

	#if (PADS_I2CM2_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E12, PADS_I2CM2_MODE << 4, BITMASK(5:4)),
    #endif

	#if (PADS_I2CM3_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E12, PADS_I2CM3_MODE << 6, BITMASK(7:6)),
    #endif

	#if (PADS_I2CM4_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E13, PADS_I2CM4_MODE << 4, BITMASK(5:4)),
    #endif

	#if (PADS_TS0_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E04, PADS_TS0_MODE, BITMASK(1:0)),
    #endif

	#if (PADS_TS1_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E04, PADS_TS1_MODE << 2, BITMASK(3:2)),
    #endif

	#if (PADS_TS2_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E04, PADS_TS2_MODE << 4, BITMASK(6:4)),
    #endif

	#if (PADS_TS_OUT_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0F, PADS_TS_OUT_MODE << 6, BITMASK(7:6)),
    #endif

	#if (PADS_NAND_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E10, PADS_NAND_MODE << 3, BITMASK(5:3)),
    #endif

	#if (PADS_NAND_CS1_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E10, PADS_NAND_CS1_EN << 6, BIT6),
    #endif

	#if (PADS_CI_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0E, PADS_CI_MODE << 6, BIT6),
    #endif

	#if (PADS_SPDIF_OUT_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0A, PADS_SPDIF_OUT_MODE << 4, BITMASK(5:4)),
    #endif

	#if (PADS_SPDIF_IN_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0A, PADS_SPDIF_IN_MODE, BITMASK(1:0)),
    #endif

	#if (PADS_I2S_IN_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0B, PADS_I2S_IN_MODE, BIT0),
    #endif

	#if (PADS_I2S_TRX_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0B, PADS_I2S_TRX_MODE << 2, BIT2),
    #endif

	#if (PADS_I2S_OUT_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0B, PADS_I2S_OUT_MODE << 4, BIT4),
    #endif

	#if (PADS_I2S_OUT_MODE_2!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0B, PADS_I2S_OUT_MODE_2 << 5, BITMASK(6:5)),
    #endif

	#if (PADS_I2S_OUT_MUTE_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E0B, PADS_I2S_OUT_MUTE_MODE << 7, BIT7),
    #endif

	#if (PADS_SDIO_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E11, PADS_SDIO_MODE << 3, BIT3),
    #endif

	#if (PADS_SD_CONFIG!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E11, PADS_SD_CONFIG, BITMASK(2:0)),
    #endif

	#if (PADS_SECOND_UART_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E06, PADS_SECOND_UART_MODE, BITMASK(1:0)),
    #endif

	#if (PADS_THIRD_UART_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E06, PADS_THIRD_UART_MODE << 4, BITMASK(5:4)),
    #endif

	#if (PADS_FOURTH_UART_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E07, PADS_FOURTH_UART_MODE, BITMASK(1:0)),
    #endif

	#if (PADS_FUART_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E13, PADS_FUART_MODE << 2, BITMASK(3:2)),
    #endif

	#if (PADS_FUART_EMMC_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E13, PADS_FUART_EMMC_MODE << 6, BIT6),
    #endif

    #if (PADS_MSPI1_MODE1!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E08, PADS_MSPI1_MODE1 << 6, BITMASK(7:6)),
    #endif

    #if (PADS_MSPI1_MODE2!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E09, PADS_MSPI1_MODE2, BITMASK(1:0)),
    #endif

    #if (PADS_MSPI1_MODE3!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E09, PADS_MSPI1_MODE3 << 2, BITMASK(3:2)),
    #endif

    #if (PADS_MSPI2_MODE2!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E09, PADS_MSPI2_MODE2 << 6, BITMASK(7:6)),
    #endif

	#if (PADS_EMMC_RSTN_EN!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E6C, PADS_EMMC_RSTN_EN << 5, BIT5),
    #endif

	#if (PADS_EMMC_CONFIG!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E10, PADS_EMMC_CONFIG, BITMASK(2:0)),
    #endif

	#if (PADS_HDMITX_DDC_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E17, PADS_HDMITX_DDC_MODE << 4, BIT4),
    #endif

	#if (PADS_HDMITX_ARC_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E17, PADS_HDMITX_ARC_MODE, BIT0),
    #endif

	#if (PADS_HDMIRX_ARC_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E16, PADS_HDMIRX_ARC_MODE << 4, BIT4),
    #endif

	#if (PADS_SATA_LED_MODE!=PADMUX_MDOE_DEFAULT)
    _RVM1(0x1E4B, PADS_SATA_LED_MODE << 4, BIT4),
    #endif

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

    // Clear all pad in
    _RVM1(0x1EA1, 0, BIT7),
#endif
    _END_OF_TBL_,
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


