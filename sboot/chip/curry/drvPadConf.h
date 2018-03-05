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
#include "hwreg_Curry.h"
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
    //reg_ir_is_gpio[0]=1
    //Reg[000e38]#4=1
    #endif

    #if(PAD_PM_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ_OEN (PAD_PM_SPI_CZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CZ_OUT (PAD_PM_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OUT, BIT1),
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OEN, BIT0),
    //reg_spi_is_gpio[2]=1
    //Reg[000e6a]#2=1
    #endif

    #if(PAD_PM_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CK_OEN (PAD_PM_SPI_CK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CK_OUT (PAD_PM_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f32, PAD_PM_SPI_CK_OUT, BIT1),
    _RVM1(0x0f32, PAD_PM_SPI_CK_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DI_OEN (PAD_PM_SPI_DI_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DI_OUT (PAD_PM_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f34, PAD_PM_SPI_DI_OUT, BIT1),
    _RVM1(0x0f34, PAD_PM_SPI_DI_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DO_OEN (PAD_PM_SPI_DO_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DO_OUT (PAD_PM_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f36, PAD_PM_SPI_DO_OUT, BIT1),
    _RVM1(0x0f36, PAD_PM_SPI_DO_OEN, BIT0),
    //reg_spi_is_gpio[0]=1
    //Reg[000e6a]#0=1
    #endif

    #if(PAD_PM_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO0_OEN (PAD_PM_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO0_OUT (PAD_PM_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f00, PAD_PM_GPIO0_OUT, BIT1),
    _RVM1(0x0f00, PAD_PM_GPIO0_OEN, BIT0),
    //reg_spi_is_gpio[7:6]=0
    //Reg[000e6a]#7~6=0
    //reg_pwm0_mode[1:0]=0
    //Reg[000e50]#1~#0=0
    //reg_vid_mode[1:0]=0
    //Reg[000e51]#5~#4=0
    //reg_gpu_vid_mode[1:0]=0
    //Reg[000e51]#3~#2=0
    //reg_miic_mode[1:0]=0
    //Reg[000e50]#7~#6=0
    #endif

    #if(PAD_PM_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO1_OEN (PAD_PM_GPIO1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO1_OUT (PAD_PM_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f02, PAD_PM_GPIO1_OUT, BIT1),
    _RVM1(0x0f02, PAD_PM_GPIO1_OEN, BIT0),
    //reg_spi_is_gpio[7:6]=0
    //Reg[000e6a]#7~6=0
    //reg_pwm1_mode[1:0]=0
    //Reg[000e50]#3~#2=0
    //reg_uart_is_gpio[2:1]=0
    //Reg[000e6b]#2~#1=0
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
    //reg_vid_mode[1:0]=0
    //Reg[000e51]#5~#4=0
    //reg_gpu_vid_mode[1:0]=0
    //Reg[000e51]#3~#2=0
    //reg_miic_mode[1:0]=0
    //Reg[000e50]#7~#6=0
    #endif

    #if(PAD_PM_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO4_OEN (PAD_PM_GPIO4_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO4_OUT (PAD_PM_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f08, PAD_PM_GPIO4_OUT, BIT1),
    _RVM1(0x0f08, PAD_PM_GPIO4_OEN, BIT0),
    //reg_gpio_pm_lock[15:8]=ba
    //Reg[000e25]#7~#0=ba
    //reg_gpio_pm_lock[7:0]=be
    //Reg[000e24]#7~#0=be
    //reg_link_wkint2gpio4=0
    //Reg[000e38]#3=0
    #endif

    #if(PAD_PM_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO5_OEN (PAD_PM_GPIO5_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO5_OUT (PAD_PM_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0a, PAD_PM_GPIO5_OUT, BIT1),
    _RVM1(0x0f0a, PAD_PM_GPIO5_OEN, BIT0),
    //reg_pwm1_mode[1:0]=0
    //Reg[000e50]#3~#2=0
    #endif

    #if(PAD_PM_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO6_OEN (PAD_PM_GPIO6_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO6_OUT (PAD_PM_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0c, PAD_PM_GPIO6_OUT, BIT1),
    _RVM1(0x0f0c, PAD_PM_GPIO6_OEN, BIT0),
    //reg_pwm0_mode[1:0]=0
    //Reg[000e50]#1~#0=0
    #endif

    #if(PAD_PM_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO7_OEN (PAD_PM_GPIO7_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO7_OUT (PAD_PM_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0e, PAD_PM_GPIO7_OUT, BIT1),
    _RVM1(0x0f0e, PAD_PM_GPIO7_OEN, BIT0),
    #endif

    #if(PAD_PM_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_PM_CEC_OEN (PAD_PM_CEC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_CEC_OUT (PAD_PM_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2c, PAD_PM_CEC_OUT, BIT1),
    _RVM1(0x0f2c, PAD_PM_CEC_OEN, BIT0),
    //reg_cec_is_gpio=1
    //Reg[000e38]#6=1
    #endif

    #if(PAD_HDMITX_HPD_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_HPD_OEN (PAD_HDMITX_HPD_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HDMITX_HPD_OUT (PAD_HDMITX_HPD_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1166, PAD_HDMITX_HPD_OUT, BIT2),
    _RVM1(0x1166, PAD_HDMITX_HPD_OEN, BIT3),
    //reg_hdmi_hpd_bypass=0
    //Reg[000e4f]#7=0
    #endif

    #if(PAD_VID0_IS_GPIO != GPIO_NONE)
    #define PAD_VID0_OEN (PAD_VID0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID0_OUT (PAD_VID0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f90, PAD_VID0_OUT, BIT1),
    _RVM1(0x0f90, PAD_VID0_OEN, BIT0),
    //reg_vid_mode[1:0]=0
    //Reg[000e51]#5~#4=0
    //reg_gpu_vid_mode[1:0]=0
    //Reg[000e51]#3~#2=0
    //reg_miic_mode[1:0]=0
    //Reg[000e50]#7~#6=0
    #endif

    #if(PAD_VID1_IS_GPIO != GPIO_NONE)
    #define PAD_VID1_OEN (PAD_VID1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_VID1_OUT (PAD_VID1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f92, PAD_VID1_OUT, BIT1),
    _RVM1(0x0f92, PAD_VID1_OEN, BIT0),
    //reg_vid_mode[1:0]=0
    //Reg[000e51]#5~#4=0
    //reg_gpu_vid_mode[1:0]=0
    //Reg[000e51]#3~#2=0
    //reg_miic_mode[1:0]=0
    //Reg[000e50]#7~#6=0
    #endif

    #if(PAD_PM_LED0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_LED0_OEN (PAD_PM_LED0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_LED0_OUT (PAD_PM_LED0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f94, PAD_PM_LED0_OUT, BIT1),
    _RVM1(0x0f94, PAD_PM_LED0_OEN, BIT0),
    //reg_led_mode[1:0]=0
    //Reg[000e50]#5~#4=0
    #endif

    #if(PAD_PM_LED1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_LED1_OEN (PAD_PM_LED1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_LED1_OUT (PAD_PM_LED1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f96, PAD_PM_LED1_OUT, BIT1),
    _RVM1(0x0f96, PAD_PM_LED1_OEN, BIT0),
    //reg_led_mode[1:0]=0
    //Reg[000e50]#5~#4=0
    #endif

    #if(PAD_SAR_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO0_OEN (PAD_SAR_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR_GPIO0_OUT (PAD_SAR_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR_GPIO0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR_GPIO0_OEN, BIT0),
    //reg_sar_aisel[0]=0
    //Reg[001422]#0=0
    #endif

    #if(PAD_SAR_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO1_OEN (PAD_SAR_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR_GPIO1_OUT (PAD_SAR_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR_GPIO1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR_GPIO1_OEN, BIT1),
    //reg_sar_aisel[1]=0
    //Reg[001422]#1=0
    #endif

    #if(PAD_SAR_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO2_OEN (PAD_SAR_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR_GPIO2_OUT (PAD_SAR_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR_GPIO2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR_GPIO2_OEN, BIT2),
    //reg_sar_aisel[2]=0
    //Reg[001422]#2=0
    #endif

    #if(PAD_SAR_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO3_OEN (PAD_SAR_GPIO3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_SAR_GPIO3_OUT (PAD_SAR_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_SAR_GPIO3_OUT, BIT3),
    _RVM1(0x1423, PAD_SAR_GPIO3_OEN, BIT3),
    //reg_sar_aisel[3]=0
    //Reg[001422]#3=0
    #endif

    #if(PAD_SAR_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO4_OEN (PAD_SAR_GPIO4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_SAR_GPIO4_OUT (PAD_SAR_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1424, PAD_SAR_GPIO4_OUT, BIT4),
    _RVM1(0x1423, PAD_SAR_GPIO4_OEN, BIT4),
    //reg_sar_aisel[4]=0
    //Reg[001422]#4=0
    #endif

    _MEMMAP_nonPM_,

    #if(PAD_I2S_OUT_BCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_BCK_OEN (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_I2S_OUT_BCK_OUT (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2556, PAD_I2S_OUT_BCK_OUT, BIT4),
    _RVM1(0x2556, PAD_I2S_OUT_BCK_OEN, BIT4),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_in_mode
    //_RVM1(0x1e0b, 0, BIT0 ),   //reg[101e0b]#0 = 0b
    //reg_i2s_trx_mode
    //_RVM1(0x1e0b, 0, BIT2 ),   //reg[101e0b]#2 = 0b
    //reg_i2s_out_mode
    //_RVM1(0x1e0b, 0, BIT4 ),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_MCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MCK_OEN (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MCK_OUT (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2558, PAD_I2S_OUT_MCK_OUT, BIT4),
    _RVM1(0x2558, PAD_I2S_OUT_MCK_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mode
    //_RVM1(0x1e0b, 0, BIT4 ),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_WS_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_WS_OEN (PAD_I2S_OUT_WS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_WS_OUT (PAD_I2S_OUT_WS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255a, PAD_I2S_OUT_WS_OUT, BIT4),
    _RVM1(0x255a, PAD_I2S_OUT_WS_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_in_mode
    //_RVM1(0x1e0b, 0, BIT0 ),   //reg[101e0b]#0 = 0b
    //reg_i2s_out_mode
    //_RVM1(0x1e0b, 0, BIT4 ),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_SD0_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD0_OEN (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD0_OUT (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255c, PAD_I2S_OUT_SD0_OUT, BIT4),
    _RVM1(0x255c, PAD_I2S_OUT_SD0_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_spdif_in_mode
    //_RVM1(0x1e0a, 0, BIT1 ~ BIT0 ),   //reg[101e0a]#1 ~ #0 = 00b
    //reg_i2s_in_mode
    //_RVM1(0x1e0b, 0, BIT0 ),   //reg[101e0b]#0 = 0b
    //reg_i2s_trx_mode
    //_RVM1(0x1e0b, 0, BIT2 ),   //reg[101e0b]#2 = 0b
    //reg_i2s_out_mode
    //_RVM1(0x1e0b, 0, BIT4 ),   //reg[101e0b]#4 = 0b
    #endif

    #if(PAD_I2S_OUT_SD1_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD1_OEN (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD1_OUT (PAD_I2S_OUT_SD1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255e, PAD_I2S_OUT_SD1_OUT, BIT4),
    _RVM1(0x255e, PAD_I2S_OUT_SD1_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_trx_mode
    //_RVM1(0x1e0b, 0, BIT2 ),   //reg[101e0b]#2 = 0b
    //reg_i2s_out_mode2
    //_RVM1(0x1e0b, 0, BIT6 ~ BIT5 ),   //reg[101e0b]#6 ~ #5 = 00b
    #endif

    #if(PAD_I2S_OUT_SD2_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD2_OEN (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD2_OUT (PAD_I2S_OUT_SD2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2560, PAD_I2S_OUT_SD2_OUT, BIT4),
    _RVM1(0x2560, PAD_I2S_OUT_SD2_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mode2
    //_RVM1(0x1e0b, 0, BIT6 ~ BIT5 ),   //reg[101e0b]#6 ~ #5 = 00b
    #endif

    #if(PAD_I2S_OUT_SD3_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD3_OEN (PAD_I2S_OUT_SD3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD3_OUT (PAD_I2S_OUT_SD3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2562, PAD_I2S_OUT_SD3_OUT, BIT4),
    _RVM1(0x2562, PAD_I2S_OUT_SD3_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mode2
    //_RVM1(0x1e0b, 0, BIT6 ~ BIT5 ),   //reg[101e0b]#6 ~ #5 = 00b
    #endif

    #if(PAD_I2S_OUT_MUTE_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MUTE_OEN (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MUTE_OUT (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2564, PAD_I2S_OUT_MUTE_OUT, BIT4),
    _RVM1(0x2564, PAD_I2S_OUT_MUTE_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2s_out_mute_mode
    //_RVM1(0x1e0b, 0, BIT7 ),   //reg[101e0b]#7 = 0b
    #endif

    #if(PAD_SD_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_SD_CLK_OEN (PAD_SD_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_CLK_OUT (PAD_SD_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a4, PAD_SD_CLK_OUT, BIT4),
    _RVM1(0x25a4, PAD_SD_CLK_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    //_RVM1(0x1e12, 0, BIT1 ~ BIT0 ),   //reg[101e12]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    //_RVM1(0x1e05, 0, BIT7 ~ BIT6 ),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    //_RVM1(0x1e11, 0, BIT1 ~ BIT0 ),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_CMD_IS_GPIO != GPIO_NONE)
    #define PAD_SD_CMD_OEN (PAD_SD_CMD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_CMD_OUT (PAD_SD_CMD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a6, PAD_SD_CMD_OUT, BIT4),
    _RVM1(0x25a6, PAD_SD_CMD_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    //_RVM1(0x1e12, 0, BIT1 ~ BIT0 ),   //reg[101e12]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    //_RVM1(0x1e05, 0, BIT7 ~ BIT6 ),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    //_RVM1(0x1e11, 0, BIT1 ~ BIT0 ),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D0_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D0_OEN (PAD_SD_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D0_OUT (PAD_SD_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a8, PAD_SD_D0_OUT, BIT4),
    _RVM1(0x25a8, PAD_SD_D0_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    //_RVM1(0x1e05, 0, BIT7 ~ BIT6 ),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    //_RVM1(0x1e11, 0, BIT1 ~ BIT0 ),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D1_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D1_OEN (PAD_SD_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D1_OUT (PAD_SD_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25aa, PAD_SD_D1_OUT, BIT4),
    _RVM1(0x25aa, PAD_SD_D1_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_mspi2_mode2
    //_RVM1(0x1e05, 0, BIT7 ~ BIT6 ),   //reg[101e05]#7 ~ #6 = 00b
    //reg_sd_config
    //_RVM1(0x1e11, 0, BIT1 ~ BIT0 ),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D2_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D2_OEN (PAD_SD_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D2_OUT (PAD_SD_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ac, PAD_SD_D2_OUT, BIT4),
    _RVM1(0x25ac, PAD_SD_D2_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_sd_config
    //_RVM1(0x1e11, 0, BIT1 ~ BIT0 ),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_SD_D3_IS_GPIO != GPIO_NONE)
    #define PAD_SD_D3_OEN (PAD_SD_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SD_D3_OUT (PAD_SD_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ae, PAD_SD_D3_OUT, BIT4),
    _RVM1(0x25ae, PAD_SD_D3_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_sd_config
    //_RVM1(0x1e11, 0, BIT1 ~ BIT0 ),   //reg[101e11]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO0_OEN (PAD_GPIO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO0_OUT (PAD_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b0, PAD_GPIO0_OUT, BIT4),
    _RVM1(0x25b0, PAD_GPIO0_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b reg_i2cm3_mode
    //_RVM1(0x1e12, 0, BIT7 ~ BIT6 ),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    //_RVM1(0x1e07, 0, BIT1 ~ BIT0 ),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm0_mode
    //_RVM1(0x1e0e, 0, BIT1 ~ BIT0 ),   //reg[101e0e]#1 ~ #0 = 00b
    //reg_usb30vctl_mode
    //_RVM1(0x1e26, 0, BIT1 ~ BIT0 ),   //reg[101e26]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b2, PAD_GPIO1_OUT, BIT4),
    _RVM1(0x25b2, PAD_GPIO1_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm3_mode
    //_RVM1(0x1e12, 0, BIT7 ~ BIT6 ),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    //_RVM1(0x1e07, 0, BIT1 ~ BIT0 ),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm0_mode
    //_RVM1(0x1e0e, 0, BIT1 ~ BIT0 ),   //reg[101e0e]#1 ~ #0 = 00b
    //reg_usb30vctl_mode
    //_RVM1(0x1e26, 0, BIT1 ~ BIT0 ),   //reg[101e26]#1 ~ #0 = 00b
    //reg_usb30vctl_mode1
    //_RVM1(0x1e27, 0, BIT1 ~ BIT0 ),   //reg[101e27]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b4, PAD_GPIO2_OUT, BIT4),
    _RVM1(0x25b4, PAD_GPIO2_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm3_mode
    //_RVM1(0x1e12, 0, BIT7 ~ BIT6 ),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    //_RVM1(0x1e07, 0, BIT1 ~ BIT0 ),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm1_mode
    //_RVM1(0x1e0e, 0, BIT5 ~ BIT4 ),   //reg[101e0e]#5 ~ #4 = 00b
    //reg_usb30vctl_mode
    //_RVM1(0x1e26, 0, BIT1 ~ BIT0 ),   //reg[101e26]#1 ~ #0 = 00b
    //reg_usb30vctl_mode1
    //_RVM1(0x1e27, 0, BIT1 ~ BIT0 ),   //reg[101e27]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b6, PAD_GPIO3_OUT, BIT4),
    _RVM1(0x25b6, PAD_GPIO3_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm3_mode
    //_RVM1(0x1e12, 0, BIT7 ~ BIT6 ),   //reg[101e12]#7 ~ #6 = 00b
    //reg_ForthUARTMode
    //_RVM1(0x1e07, 0, BIT1 ~ BIT0 ),   //reg[101e07]#1 ~ #0 = 00b
    //reg_mspi1_mode2
    //_RVM1(0x1e05, 0, BIT3 ~ BIT2 ),   //reg[101e05]#3 ~ #2 = 00b
    //reg_pwm1_mode
    //_RVM1(0x1e0e, 0, BIT5 ~ BIT4 ),   //reg[101e0e]#5 ~ #4 = 00b
    //reg_usb30vctl_mode1
    //_RVM1(0x1e27, 0, BIT1 ~ BIT0 ),   //reg[101e27]#1 ~ #0 = 00b
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b8, PAD_GPIO4_OUT, BIT4),
    _RVM1(0x25b8, PAD_GPIO4_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm4_mode
    //_RVM1(0x1e13, 0, BIT5 ~ BIT4 ),   //reg[101e13]#5 ~ #4 = 00b
    //reg_fuart_mode
    //_RVM1(0x1e13, 0, BIT3 ~ BIT2 ),   //reg[101e13]#3 ~ #2 = 00b
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ba, PAD_GPIO5_OUT, BIT4),
    _RVM1(0x25ba, PAD_GPIO5_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm4_mode
    //_RVM1(0x1e13, 0, BIT5 ~ BIT4 ),   //reg[101e13]#5 ~ #4 = 00b
    //reg_fuart_mode
    //_RVM1(0x1e13, 0, BIT3 ~ BIT2 ),   //reg[101e13]#3 ~ #2 = 00b
    #endif

    #if(PAD_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO6_OEN (PAD_GPIO6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO6_OUT (PAD_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25bc, PAD_GPIO6_OUT, BIT4),
    _RVM1(0x25bc, PAD_GPIO6_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    #endif

    #if(PAD_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO7_OEN (PAD_GPIO7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO7_OUT (PAD_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25be, PAD_GPIO7_OUT, BIT4),
    _RVM1(0x25be, PAD_GPIO7_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    #endif

    #if(PAD_TS_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS_CLK_OEN (PAD_TS_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_CLK_OUT (PAD_TS_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2524, PAD_TS_CLK_OUT, BIT4),
    _RVM1(0x2524, PAD_TS_CLK_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS_SYNC_OEN (PAD_TS_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_SYNC_OUT (PAD_TS_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2526, PAD_TS_SYNC_OUT, BIT4),
    _RVM1(0x2526, PAD_TS_SYNC_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    //reg_fuart_mode
    //_RVM1(0x1e13, 0, BIT3 ~ BIT2 ),   //reg[101e13]#3 ~ #2 = 00b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS_VLD_OEN (PAD_TS_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_VLD_OUT (PAD_TS_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2528, PAD_TS_VLD_OUT, BIT4),
    _RVM1(0x2528, PAD_TS_VLD_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    //reg_fuart_mode
    //_RVM1(0x1e13, 0, BIT3 ~ BIT2 ),   //reg[101e13]#3 ~ #2 = 00b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D0_OEN (PAD_TS_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D0_OUT (PAD_TS_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252a, PAD_TS_D0_OUT, BIT4),
    _RVM1(0x252a, PAD_TS_D0_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D1_OEN (PAD_TS_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D1_OUT (PAD_TS_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252c, PAD_TS_D1_OUT, BIT4),
    _RVM1(0x252c, PAD_TS_D1_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D2_OEN (PAD_TS_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D2_OUT (PAD_TS_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252e, PAD_TS_D2_OUT, BIT4),
    _RVM1(0x252e, PAD_TS_D2_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D3_OEN (PAD_TS_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D3_OUT (PAD_TS_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2530, PAD_TS_D3_OUT, BIT4),
    _RVM1(0x2530, PAD_TS_D3_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D4_OEN (PAD_TS_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D4_OUT (PAD_TS_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2532, PAD_TS_D4_OUT, BIT4),
    _RVM1(0x2532, PAD_TS_D4_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    //reg_SecondUARTMode
    //_RVM1(0x1e06, 0, BIT1 ~ BIT0 ),   //reg[101e06]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D5_OEN (PAD_TS_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D5_OUT (PAD_TS_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2534, PAD_TS_D5_OUT, BIT4),
    _RVM1(0x2534, PAD_TS_D5_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    //reg_SecondUARTMode
    //_RVM1(0x1e06, 0, BIT1 ~ BIT0 ),   //reg[101e06]#1 ~ #0 = 00b
    #endif

    #if(PAD_TS_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D6_OEN (PAD_TS_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D6_OUT (PAD_TS_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2536, PAD_TS_D6_OUT, BIT4),
    _RVM1(0x2536, PAD_TS_D6_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_TS_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS_D7_OEN (PAD_TS_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS_D7_OUT (PAD_TS_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2538, PAD_TS_D7_OUT, BIT4),
    _RVM1(0x2538, PAD_TS_D7_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_ts0_mode
    //_RVM1(0x1e04, 0, BIT0 ),   //reg[101e04]#0 = 0b
    #endif

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a0, PAD_SPDIF_OUT_OUT, BIT4),
    _RVM1(0x25a0, PAD_SPDIF_OUT_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_spdif_out_mode
    //_RVM1(0x1e0a, 0, BIT5 ~ BIT4 ),   //reg[101e0a]#5 ~ #4 = 00b
    //reg_ts2_mode
    //_RVM1(0x1e04, 0, BIT4 ~ BIT3 ),   //reg[101e04]#4 ~ #3 = 00b
    #endif

    #if(PAD_I2CM0_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SCL_OEN (PAD_I2CM0_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SCL_OUT (PAD_I2CM0_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251c, PAD_I2CM0_SCL_OUT, BIT4),
    _RVM1(0x251c, PAD_I2CM0_SCL_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    //_RVM1(0x1e12, 0, BIT1 ~ BIT0 ),   //reg[101e12]#1 ~ #0 = 00b
    //reg_SecondUARTMode
    //_RVM1(0x1e06, 0, BIT1 ~ BIT0 ),   //reg[101e06]#1 ~ #0 = 00b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2CM0_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SDA_OEN (PAD_I2CM0_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SDA_OUT (PAD_I2CM0_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251e, PAD_I2CM0_SDA_OUT, BIT4),
    _RVM1(0x251e, PAD_I2CM0_SDA_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_test_in_mode
    //_RVM1(0x1e24, 0, BIT1 ~ BIT0 ),   //reg[101e24]#1 ~ #0 = 00b
    //reg_test_out_mode
    //_RVM1(0x1e24, 0, BIT5 ~ BIT4 ),   //reg[101e24]#5 ~ #4 = 00b
    //reg_i2cm0_mode
    //_RVM1(0x1e12, 0, BIT1 ~ BIT0 ),   //reg[101e12]#1 ~ #0 = 00b
    //reg_SecondUARTMode
    //_RVM1(0x1e06, 0, BIT1 ~ BIT0 ),   //reg[101e06]#1 ~ #0 = 00b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2CM1_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SCL_OEN (PAD_I2CM1_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SCL_OUT (PAD_I2CM1_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2520, PAD_I2CM1_SCL_OUT, BIT4),
    _RVM1(0x2520, PAD_I2CM1_SCL_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_i2cm1_mode
    //_RVM1(0x1e12, 0, BIT3 ~ BIT2 ),   //reg[101e12]#3 ~ #2 = 00b
    //reg_ThirdUARTMode
    //_RVM1(0x1e06, 0, BIT5 ~ BIT4 ),   //reg[101e06]#5 ~ #4 = 00b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_I2CM1_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SDA_OEN (PAD_I2CM1_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SDA_OUT (PAD_I2CM1_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2522, PAD_I2CM1_SDA_OUT, BIT4),
    _RVM1(0x2522, PAD_I2CM1_SDA_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_i2cm1_mode
    //_RVM1(0x1e12, 0, BIT3 ~ BIT2 ),   //reg[101e12]#3 ~ #2 = 00b
    //reg_ThirdUARTMode
    //_RVM1(0x1e06, 0, BIT5 ~ BIT4 ),   //reg[101e06]#5 ~ #4 = 00b
    //reg_mspi1_mode1
    //_RVM1(0x1e05, 0, BIT1 ~ BIT0 ),   //reg[101e05]#1 ~ #0 = 00b
    #endif

    #if(PAD_HDMITX_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_SCL_OEN (PAD_HDMITX_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HDMITX_SCL_OUT (PAD_HDMITX_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25da, PAD_HDMITX_SCL_OUT, BIT4),
    _RVM1(0x25da, PAD_HDMITX_SCL_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_i2cm2_mode
    //_RVM1(0x1e12, 0, BIT5 ~ BIT4 ),   //reg[101e12]#5 ~ #4 = 00b
    //reg_ThirdUARTMode
    //_RVM1(0x1e06, 0, BIT5 ~ BIT4 ),   //reg[101e06]#5 ~ #4 = 00b
    //reg_hdmitx_ddc_mode
    //_RVM1(0x1e17, 0, BIT4 ),   //reg[101e17]#4 = 0b
    #endif

    #if(PAD_HDMITX_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_HDMITX_SDA_OEN (PAD_HDMITX_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HDMITX_SDA_OUT (PAD_HDMITX_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25dc, PAD_HDMITX_SDA_OUT, BIT4),
    _RVM1(0x25dc, PAD_HDMITX_SDA_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_i2cm2_mode
    //_RVM1(0x1e12, 0, BIT5 ~ BIT4 ),   //reg[101e12]#5 ~ #4 = 00b
    //reg_ThirdUARTMode
    //_RVM1(0x1e06, 0, BIT5 ~ BIT4 ),   //reg[101e06]#5 ~ #4 = 00b
    //reg_hdmitx_ddc_mode
    //_RVM1(0x1e17, 0, BIT4 ),   //reg[101e17]#4 = 0b
    #endif

    #if(PAD_GT0_MDIO_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_MDIO_OEN (PAD_GT0_MDIO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_MDIO_OUT (PAD_GT0_MDIO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253a, PAD_GT0_MDIO_OUT, BIT4),
    _RVM1(0x253a, PAD_GT0_MDIO_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_MDC_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_MDC_OEN (PAD_GT0_MDC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_MDC_OUT (PAD_GT0_MDC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253c, PAD_GT0_MDC_OUT, BIT4),
    _RVM1(0x253c, PAD_GT0_MDC_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_CLK_OEN (PAD_GT0_RX_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_CLK_OUT (PAD_GT0_RX_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253e, PAD_GT0_RX_CLK_OUT, BIT4),
    _RVM1(0x253e, PAD_GT0_RX_CLK_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_CTL_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_CTL_OEN (PAD_GT0_RX_CTL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_CTL_OUT (PAD_GT0_RX_CTL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2540, PAD_GT0_RX_CTL_OUT, BIT4),
    _RVM1(0x2540, PAD_GT0_RX_CTL_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D0_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D0_OEN (PAD_GT0_RX_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D0_OUT (PAD_GT0_RX_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2542, PAD_GT0_RX_D0_OUT, BIT4),
    _RVM1(0x2542, PAD_GT0_RX_D0_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D1_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D1_OEN (PAD_GT0_RX_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D1_OUT (PAD_GT0_RX_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2544, PAD_GT0_RX_D1_OUT, BIT4),
    _RVM1(0x2544, PAD_GT0_RX_D1_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D2_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D2_OEN (PAD_GT0_RX_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D2_OUT (PAD_GT0_RX_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2546, PAD_GT0_RX_D2_OUT, BIT4),
    _RVM1(0x2546, PAD_GT0_RX_D2_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_RX_D3_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_RX_D3_OEN (PAD_GT0_RX_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_RX_D3_OUT (PAD_GT0_RX_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2548, PAD_GT0_RX_D3_OUT, BIT4),
    _RVM1(0x2548, PAD_GT0_RX_D3_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_CLK_OEN (PAD_GT0_TX_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_CLK_OUT (PAD_GT0_TX_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254a, PAD_GT0_TX_CLK_OUT, BIT4),
    _RVM1(0x254a, PAD_GT0_TX_CLK_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_CTL_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_CTL_OEN (PAD_GT0_TX_CTL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_CTL_OUT (PAD_GT0_TX_CTL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254c, PAD_GT0_TX_CTL_OUT, BIT4),
    _RVM1(0x254c, PAD_GT0_TX_CTL_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D0_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D0_OEN (PAD_GT0_TX_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D0_OUT (PAD_GT0_TX_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254e, PAD_GT0_TX_D0_OUT, BIT4),
    _RVM1(0x254e, PAD_GT0_TX_D0_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D1_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D1_OEN (PAD_GT0_TX_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D1_OUT (PAD_GT0_TX_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2550, PAD_GT0_TX_D1_OUT, BIT4),
    _RVM1(0x2550, PAD_GT0_TX_D1_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D2_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D2_OEN (PAD_GT0_TX_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D2_OUT (PAD_GT0_TX_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2552, PAD_GT0_TX_D2_OUT, BIT4),
    _RVM1(0x2552, PAD_GT0_TX_D2_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_GT0_TX_D3_IS_GPIO != GPIO_NONE)
    #define PAD_GT0_TX_D3_OEN (PAD_GT0_TX_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GT0_TX_D3_OUT (PAD_GT0_TX_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2554, PAD_GT0_TX_D3_OUT, BIT4),
    _RVM1(0x2554, PAD_GT0_TX_D3_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_gt0_mode
    //_RVM1(0x1e0c, 0, BIT0 ),   //reg[101e0c]#0 = 0b
    #endif

    #if(PAD_EMMC_IO12_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO12_OEN (PAD_EMMC_IO12_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO12_OUT (PAD_EMMC_IO12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2588, PAD_EMMC_IO12_OUT, BIT4),
    _RVM1(0x2588, PAD_EMMC_IO12_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_nana_mod
    //_RVM1(0x1e10, 0, BIT5 ~ BIT4 ),   //reg[101e10]#5 ~ #4 = 000b
    //reg_tx_bps_en_0
    //_RVM1(0x3fe2, 0, BIT7 ~ BIT0 ),   //reg[123fe2]#7 ~ #0 = 00000000b
    //reg_tx_bps_en_1
    //_RVM1(0x3fe3, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe3]#7 ~ #0 = 11110000b
    //reg_tx_bps_en_2
    //_RVM1(0x3fe4, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe4]#1 ~ #0 = 11b
    //reg_rx_bps_en_0
    //_RVM1(0x3fe6, 0, BIT7 ~ BIT0 ),   //reg[123fe6]#7 ~ #0 = 00000000b
    //reg_rx_bps_en_1
    //_RVM1(0x3fe7, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe7]#7 ~ #0 = 11110000b
    //reg_rx_bps_en_2
    //_RVM1(0x3fe8, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe8]#1 ~ #0 = 11b
    #endif

    #if(PAD_EMMC_IO14_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO14_OEN (PAD_EMMC_IO14_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO14_OUT (PAD_EMMC_IO14_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258c, PAD_EMMC_IO14_OUT, BIT4),
    _RVM1(0x258c, PAD_EMMC_IO14_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_nana_mod
    //_RVM1(0x1e10, 0, BIT5 ~ BIT4 ),   //reg[101e10]#5 ~ #4 = 000b
    //reg_tx_bps_en_0
    //_RVM1(0x3fe2, 0, BIT7 ~ BIT0 ),   //reg[123fe2]#7 ~ #0 = 00000000b
    //reg_tx_bps_en_1
    //_RVM1(0x3fe3, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe3]#7 ~ #0 = 11110000b
    //reg_tx_bps_en_2
    //_RVM1(0x3fe4, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe4]#1 ~ #0 = 11b
    //reg_rx_bps_en_0
    //_RVM1(0x3fe6, 0, BIT7 ~ BIT0 ),   //reg[123fe6]#7 ~ #0 = 00000000b
    //reg_rx_bps_en_1
    //_RVM1(0x3fe7, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe7]#7 ~ #0 = 11110000b
    //reg_rx_bps_en_2
    //_RVM1(0x3fe8, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe8]#1 ~ #0 = 11b
    #endif

    #if(PAD_EMMC_IO15_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO15_OEN (PAD_EMMC_IO15_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO15_OUT (PAD_EMMC_IO15_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258e, PAD_EMMC_IO15_OUT, BIT4),
    _RVM1(0x258e, PAD_EMMC_IO15_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_nana_mod
    //_RVM1(0x1e10, 0, BIT5 ~ BIT4 ),   //reg[101e10]#5 ~ #4 = 000b
    //reg_tx_bps_en_0
    //_RVM1(0x3fe2, 0, BIT7 ~ BIT0 ),   //reg[123fe2]#7 ~ #0 = 00000000b
    //reg_tx_bps_en_1
    //_RVM1(0x3fe3, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe3]#7 ~ #0 = 11110000b
    //reg_tx_bps_en_2
    //_RVM1(0x3fe4, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe4]#1 ~ #0 = 11b
    //reg_rx_bps_en_0
    //_RVM1(0x3fe6, 0, BIT7 ~ BIT0 ),   //reg[123fe6]#7 ~ #0 = 00000000b
    //reg_rx_bps_en_1
    //_RVM1(0x3fe7, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe7]#7 ~ #0 = 11110000b
    //reg_rx_bps_en_2
    //_RVM1(0x3fe8, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe8]#1 ~ #0 = 11b
    #endif

    #if(PAD_EMMC_IO16_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO16_OEN (PAD_EMMC_IO16_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO16_OUT (PAD_EMMC_IO16_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2590, PAD_EMMC_IO16_OUT, BIT4),
    _RVM1(0x2590, PAD_EMMC_IO16_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_nana_mod
    //_RVM1(0x1e10, 0, BIT5 ~ BIT4 ),   //reg[101e10]#5 ~ #4 = 000b
    //reg_tx_bps_en_0
    //_RVM1(0x3fe2, 0, BIT7 ~ BIT0 ),   //reg[123fe2]#7 ~ #0 = 00000000b
    //reg_tx_bps_en_1
    //_RVM1(0x3fe3, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe3]#7 ~ #0 = 11110000b
    //reg_tx_bps_en_2
    //_RVM1(0x3fe4, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe4]#1 ~ #0 = 11b
    //reg_rx_bps_en_0
    //_RVM1(0x3fe6, 0, BIT7 ~ BIT0 ),   //reg[123fe6]#7 ~ #0 = 00000000b
    //reg_rx_bps_en_1
    //_RVM1(0x3fe7, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe7]#7 ~ #0 = 11110000b
    //reg_rx_bps_en_2
    //_RVM1(0x3fe8, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe8]#1 ~ #0 = 11b
    #endif

    #if(PAD_EMMC_IO17_IS_GPIO != GPIO_NONE)
    #define PAD_EMMC_IO17_OEN (PAD_EMMC_IO17_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_EMMC_IO17_OUT (PAD_EMMC_IO17_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2592, PAD_EMMC_IO17_OUT, BIT4),
    _RVM1(0x2592, PAD_EMMC_IO17_OEN, BIT5),
    //reg_allpad_in
    //_RVM1(0x1ea1, 0, BIT7 ),   //reg[101ea1]#7 = 0b
    //reg_nana_mod
    //_RVM1(0x1e10, 0, BIT5 ~ BIT4 ),   //reg[101e10]#5 ~ #4 = 000b
    //reg_tx_bps_en_0
    //_RVM1(0x3fe2, 0, BIT7 ~ BIT0 ),   //reg[123fe2]#7 ~ #0 = 00000000b
    //reg_tx_bps_en_1
    //_RVM1(0x3fe3, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe3]#7 ~ #0 = 11110000b
    //reg_tx_bps_en_2
    //_RVM1(0x3fe4, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe4]#1 ~ #0 = 11b
    //reg_rx_bps_en_0
    //_RVM1(0x3fe6, 0, BIT7 ~ BIT0 ),   //reg[123fe6]#7 ~ #0 = 00000000b
    //reg_rx_bps_en_1
    //_RVM1(0x3fe7, BIT7 ~ BIT0 , BIT7 ~ BIT0 ),   //reg[123fe7]#7 ~ #0 = 11110000b
    //reg_rx_bps_en_2
    //_RVM1(0x3fe8, BIT1 ~ BIT0 , BIT1 ~ BIT0 ),   //reg[123fe8]#1 ~ #0 = 11b
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------
    _MEMMAP_nonPM_,

    // reg_ej_mode
     #define EJ_MODE ((PADS_EJ_MODE == PAD_GPIO0) ? (BIT0) : 0)
    _RVM1(0x1E16, EJ_MODE, BIT0),

    // reg_spdif_out_mode
    #define SPDIF_OUT_MODE ((PADS_SPDIF_OUT_MODE == PAD_GPIO2) ? (BIT4) : \
                            (PADS_SPDIF_OUT_MODE == PAD_GPIO4) ? (BIT5) : 0)
    _RVM1(0x1E0A, SPDIF_OUT_MODE, BITMASK(5:4)),

    // reg_spdif_in_mode
    #define SPDIF_IN_MODE ((PADS_SPDIF_IN_MODE == PAD_GPIO0) ? (BIT0) : 0)
    _RVM1(0x1E0A, SPDIF_IN_MODE, BITMASK(1:0)),

    // reg_i2s_out_mode
    #define I2S_OUT_MODE ((PADS_I2S_OUT_MODE == PAD_GT0_RX_CLK)  ? (BIT3) : \
                          (PADS_I2S_OUT_MODE == PAD_TS_CLK)      ? (BIT4) : \
                          (PADS_I2S_OUT_MODE == PAD_I2S_OUT_BCK) ? (BIT4|BIT3) : 0)
    _RVM1(0x1E0B, I2S_OUT_MODE, BITMASK(4:3)),

    // reg_i2s_out_mode2
    #define I2S_OUT_MODE_2 ((PADS_I2S_OUT_MODE_2 == PAD_I2S_OUT_SD1) ? (BIT5) : \
                            (PADS_I2S_OUT_MODE_2 == PAD_I2S_OUT_SD2) ? (BIT6) : \
                            (PADS_I2S_OUT_MODE_2 == PAD_I2S_OUT_SD3) ? (BIT6|BIT5) : 0)
    _RVM1(0x1E0B, I2S_OUT_MODE_2, BITMASK(6:5)),

    // reg_i2s_out_mute_mode
    #define I2S_OUT_MUTE_MODE ((PADS_I2S_OUT_MUTE_MODE == PAD_I2S_OUT_MUTE) ? (BIT7) : 0)
    _RVM1(0x1E0B, I2S_OUT_MUTE_MODE, BIT7),

    // reg_sm0_mode
    #define SM0_OPEN ((PADS_SM0_OPEN == PAD_GT0_RX_CLK) ? (BIT2) : \
                      (PADS_SM0_OPEN == PAD_TS_D2)      ? (BIT4) : 0)
    _RVM1(0x1E18, SM0_OPEN, BITMASK(4:2)),

    // reg_ts0_mode
    #define TS0_MODE ((PADS_TS0_MODE == PAD_TS_CLK)  ? (BIT5) : \
                      (PADS_TS0_MODE == PAD_TS_SYNC) ? (BIT6) : 0)
    _RVM1(0x1E04, TS0_MODE, BITMASK(6:5)),

    // reg_ts1_mode
    #define TS1_MODE ((PADS_TS1_MODE == PAD_TS_CLK)  ? (BIT1) : \
                      (PADS_TS1_MODE == PAD_TS_SYNC) ? (BIT2) : 0)
    _RVM1(0x1E04, TS1_MODE, BITMASK(2:1)),

    // reg_ts2_mode
    #define TS2_MODE ((PADS_TS2_MODE == PAD_GT0_MDIO) ? (BIT3) : \
                      (PADS_TS2_MODE == PAD_TS_D1)    ? (BIT4) : 0)
    _RVM1(0x1E04, TS2_MODE, BITMASK(4:3)),

#if 0
    #define TS_OUT_MODE ((PADS_TS_OUT_MODE == PAD_TS1_CLK)  ? (BIT6) : \
                         (PADS_TS_OUT_MODE == PAD_TS1_SYNC) ? (BIT7) : \
                         (PADS_TS_OUT_MODE == PAD_TS1_VLD)  ? (BIT7|BIT6) : 0)

    _RVM1(0x1E0F, TS_OUT_MODE, BITMASK(7:6)),

    // reg_nand_mode
    #define NAND_MODE ((PADS_NAND_MODE == PAD_EMMC_IO0) ? (BIT4) : 0)
    _RVM1(0x1E10, NAND_MODE, BITMASK(5:4)),

    // reg_nand_cs1_en
    #define NAND_CS1_EN ((PADS_NAND_CS1_EN == PAD_EMMC_IO16) ? (BIT6) : 0)
    _RVM1(0x1E10, NAND_CS1_EN, BIT6),
#endif

    // reg_i2cm0_mode
    #define I2CM0_MODE ((PADS_I2CM0_MODE == PAD_I2CM0_SCL) ? (BIT0) : \
                        (PADS_I2CM0_MODE == PAD_TS_CLK)    ? (BIT1) : \
                        (PADS_I2CM0_MODE == PAD_GPIO0)     ? (BIT1|BIT0) : 0)
    _RVM1(0x1E12, I2CM0_MODE, BITMASK(1:0)),

    // reg_i2cm1_mode
    #define I2CM1_MODE ((PADS_I2CM1_MODE == PAD_I2CM1_SCL) ? (BIT2) : \
                        (PADS_I2CM1_MODE == PAD_TS_VLD)    ? (BIT3) : \
                        (PADS_I2CM1_MODE == PAD_GPIO2)     ? (BIT3|BIT2) : 0)
    _RVM1(0x1E12, I2CM1_MODE, BITMASK(3:2)),

    // reg_i2cm2_mode
    #define I2CM2_MODE ((PADS_I2CM2_MODE == PAD_HDMITX_SCL) ? (BIT4) : \
                        (PADS_I2CM2_MODE == PAD_GPIO3)      ? (BIT5) : 0)
    _RVM1(0x1E12, I2CM2_MODE, BITMASK(5:4)),

    // reg_i2cm3_mode
    #define I2CM3_MODE ((PADS_I2CM3_MODE == PAD_GPIO0) ? (BIT6) : \
                        (PADS_I2CM3_MODE == PAD_GPIO2) ? (BIT7) : 0)
    _RVM1(0x1E12, I2CM3_MODE, BITMASK(7:6)),

    // reg_i2cm4_mode
    #define I2CM4_MODE ((PADS_I2CM4_MODE == PAD_TS_CLK) ? (BIT4) : \
                        (PADS_I2CM4_MODE == PAD_TS_D1)  ? (BIT5) : 0)
    _RVM1(0x1E13, I2CM4_MODE, BITMASK(5:4)),

    // reg_SecondUARTMode
    #define SECOND_UART_MODE ((PADS_SECOND_UART_MODE == PAD_GPIO0)          ? (BIT0) : \
                              (PADS_SECOND_UART_MODE == PAD_TS_CLK)         ? (BIT1) : \
                              (PADS_SECOND_UART_MODE == PAD_I2CM0_SCL)      ? (BIT1|BIT0) : 0)
    _RVM1(0x1E06, SECOND_UART_MODE, BITMASK(1:0)),

    // reg_ThirdUARTMode
    #define THIRD_UART_MODE ((PADS_THIRD_UART_MODE == PAD_I2CM1_SCL)  ? (BIT4) : \
                             (PADS_THIRD_UART_MODE == PAD_GPIO3)      ? (BIT5) : \
                             (PADS_THIRD_UART_MODE == PAD_HDMITX_SCL) ? (BIT5|BIT4) : 0)
    _RVM1(0x1E06, THIRD_UART_MODE, BITMASK(5:4)),

    // reg_FourthUARTMode
    #define FOURTH_UART_MODE ((PADS_FOURTH_UART_MODE == PAD_GPIO0) ? (BIT0) : \
                              (PADS_FOURTH_UART_MODE == PAD_TS_D4) ? (BIT1) : \
                              (PADS_FOURTH_UART_MODE == PAD_GPIO2) ? (BIT1|BIT0) : 0)
    _RVM1(0x1E07, FOURTH_UART_MODE, BITMASK(1:0)),

    // reg_fuart_mode
    #define FUART_MODE ((PADS_FUART_MODE == PAD_GPIO2)  ? (BIT2) : \
                        (PADS_FUART_MODE == PAD_TS_CLK) ? (BIT3) : 0)
    _RVM1(0x1E13, FUART_MODE, BITMASK(3:2)),

    // reg_fuart_emmc_mode
    #define FUART_EMMC_MODE ((PADS_FUART_EMMC_MODE == PAD_EMMC_IO14) ? (BIT6) : 0)
    _RVM1(0x1E13, FUART_EMMC_MODE, BIT6),

    // reg_mspi1_mode1
    #define MSPI1_MODE1 ((PADS_MSPI1_MODE1 == PAD_I2CM0_SCL) ? (BIT0) : \
                         (PADS_MSPI1_MODE1 == PAD_TS_CLK)    ? (BIT1) : \
                         (PADS_MSPI1_MODE1 == PAD_TS_SYNC)   ? (BIT1|BIT0) : 0)
    _RVM1(0x1E05, FOURTH_UART_MODE, BITMASK(1:0)),

    // reg_mspi1_mode2
    #define MSPI1_MODE2 ((PADS_MSPI1_MODE2 == PAD_GT0_MDIO) ? (BIT2) : \
                         (PADS_MSPI1_MODE2 == PAD_SD_CLK)   ? (BIT3) : \
                         (PADS_MSPI1_MODE2 == PAD_TS_D1)    ? (BIT3|BIT2) : 0)
    _RVM1(0x1E05, FOURTH_UART_MODE, BITMASK(3:2)),

    // reg_mspi2_mode2
    #define MSPI2_MODE2 ((PADS_MSPI2_MODE2 == PAD_GPIO0) ? (BIT6) : 0)
    _RVM1(0x1E05, FUART_EMMC_MODE, BITMASK(7:6)),

    // reg_sd_config
    //#define SD_CONFIG0 ((PADS_SD_CONFIG0 == PAD_SDIO_IO0)   ? (BIT7) :
    #define SD_CONFIG0 ((PADS_SD_CONFIG0 == PAD_GT0_RX_CLK) ? (BIT7) : \
                        (PADS_SD_CONFIG0 == PAD_TS_CLK)     ? (BIT7) : 0)
    #define SD_CONFIG1 ((PADS_SD_CONFIG1 == PAD_GT0_MDIO)   ? (BIT0) : \
                        (PADS_SD_CONFIG1 == PAD_GT0_RX_CLK) ? (BIT0) : \
                        (PADS_SD_CONFIG1 == PAD_SD_CLK)     ? (BIT1) : \
                        (PADS_SD_CONFIG1 == PAD_TS_CLK)     ? (BIT1) : 0)
    _RVM1(0x1E10, SD_CONFIG0, BIT7),
    _RVM1(0x1E11, SD_CONFIG1, BITMASK(1:0)),

#if 0
    // reg_sdio_mode
    #define SDIO_MODE ((PADS_SDIO_MODE == PAD_SDIO_IO0) ? (BIT2) : 0)
    _RVM1(0x1E11, SDIO_MODE, BITMASK(3:2)),

    // reg_emmc_rstn_en
    #define EMMC_RSTN_EN ((PADS_EMMC_RSTN_EN == PAD_EMMC_IO11) ? (BIT5) : 0)
    _RVM1(0x1E6c, EMMC_RSTN_EN, BIT5),

    // reg_emmc_config
    #define EMMC_CONFIG ((PADS_EMMC_CONFIG == PAD_EMMC_IO0) ? (BIT1) : 0)
    _RVM1(0x1E0F, EMMC_CONFIG, BIT1),
#endif

    // reg_pwm0_mode
    #define PWM0_MODE ((PADS_PWM0_MODE == PAD_GPIO2) ? (BIT0) : \
                       (PADS_PWM0_MODE == PAD_GPIO4) ? (BIT1) : 0)
    _RVM1(0x1E0E, PWM0_MODE, BITMASK(1:0)),

    // reg_pwm1_mode
    #define PWM1_MODE ((PADS_PWM1_MODE == PAD_GPIO0) ? (BIT4) : \
                       (PADS_PWM1_MODE == PAD_GPIO1) ? (BIT5) : 0)
    _RVM1(0x1E0E, PWM1_MODE, BITMASK(5:4)),

    // reg_hdmitx_ddc_mode
    #define HDMITX_DDC_MODE ((PADS_HDMITX_DDC_MODE == PAD_HDMITX_SCL) ? (BIT4) : 0)
    _RVM1(0x1E17, HDMITX_DDC_MODE, BIT4),

//---------------------------------------------------------------------
    // UART_SRC_SEL
    #define UART_INV ((UART0_INV ? BIT0 : 0) | \
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
    _RVM1(0x0E13, 0 , BIT4),     // UART0 -> reg_uart_sel0

    _RVM1(0x0E50, BIT4, BIT4|BIT5),   // LED_Mode

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
    (UART1_SRC_SEL == UART2_SRC_SEL)
    #error "UART src selection conflict"
#endif


