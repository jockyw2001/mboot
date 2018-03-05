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
//#include "c_riubase.h"
#include "hwreg_Kappa.h"
#include "chip/bond.h"

#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high

#if defined (MIPS_CHAKRA) || defined(MSOS_TYPE_LINUX)
#define _MapBase_nonPM_KAPPA (RIU_MAP + 0x200000)
#define _MapBase_PM_KAPPA    RIU_MAP
#else
#define _MapBase_nonPM_KAPPA 0xA0200000
#define _MapBase_PM_KAPPA   0xA0000000
#endif
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
    _MEMMAP_PM_,


// Script Generated GPIO

    #if(PAD_PM_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ_OEN (PAD_PM_SPI_CZ_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PM_SPI_CZ_OUT (PAD_PM_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0e6a, BIT1, BIT1), // Configuratioin
    _RVM1(0x0e3a, PAD_PM_SPI_CZ_OUT, BIT4),
    _RVM1(0x0e3c, PAD_PM_SPI_CZ_OEN, BIT4),
    #endif

    #if(PAD_PM_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CK_OEN (PAD_PM_SPI_CK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PM_SPI_CK_OUT (PAD_PM_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x0e6a, BIT0, BIT0), // Configuratioin
    _RVM1(0x0e3a, PAD_PM_SPI_CK_OUT, BIT5),
    _RVM1(0x0e3c, PAD_PM_SPI_CK_OEN, BIT5),
    #endif

    #if(PAD_PM_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DI_OEN (PAD_PM_SPI_DI_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_PM_SPI_DI_OUT (PAD_PM_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0e6a, BIT0, BIT0), // Configuratioin
    _RVM1(0x0e3a, PAD_PM_SPI_DI_OUT, BIT6),
    _RVM1(0x0e3c, PAD_PM_SPI_DI_OEN, BIT6),
    #endif

    #if(PAD_PM_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DO_OEN (PAD_PM_SPI_DO_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_PM_SPI_DO_OUT (PAD_PM_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x0e6a, BIT0, BIT0), // Configuratioin
    _RVM1(0x0e3a, PAD_PM_SPI_DO_OUT, BIT7),
    _RVM1(0x0e3c, PAD_PM_SPI_DO_OEN, BIT7),
    #endif

    #if(PAD_IRIN_IS_GPIO != GPIO_NONE)
    #define PAD_IRIN_OEN (PAD_IRIN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_IRIN_OUT (PAD_IRIN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e38, BIT4, BIT4), // Configuratioin
    _RVM1(0x0f28, PAD_IRIN_OUT, BIT1),
    _RVM1(0x0f28, PAD_IRIN_OEN, BIT0),
    #endif

    #if(PAD_HDMI_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_HDMI_CEC_OEN (PAD_HDMI_CEC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_HDMI_CEC_OUT (PAD_HDMI_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e38, BIT6, BIT6), // Configuratioin
    _RVM1(0x0f24, PAD_HDMI_CEC_OUT, BIT1),
    _RVM1(0x0f24, PAD_HDMI_CEC_OEN, BIT0),
    #endif

    #if(PAD_HDMI_HPD_IS_GPIO != GPIO_NONE)
    #define PAD_HDMI_HPD_OEN (PAD_HDMI_HPD_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HDMI_HPD_OUT (PAD_HDMI_HPD_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1166, PAD_HDMI_HPD_OUT, BIT2),
    _RVM1(0x1166, PAD_HDMI_HPD_OEN, BIT3),
    #endif

    #if(PAD_UART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_UART_RX_OEN (PAD_UART_RX_IS_GPIO == GPIO_IN ? BITNA: 0)
    #define PAD_UART_RX_OUT (PAD_UART_RX_IS_GPIO == GPIO_OUT_HIGH ? BITNA: 0)
    _RVM1(0x, PAD_UART_RX_OUT, BITNA),
    _RVM1(0x, PAD_UART_RX_OEN, BITNA),
    #endif

    #if(PAD_UART_TX_IS_GPIO != GPIO_NONE)
    #define PAD_UART_TX_OEN (PAD_UART_TX_IS_GPIO == GPIO_IN ? BITNA: 0)
    #define PAD_UART_TX_OUT (PAD_UART_TX_IS_GPIO == GPIO_OUT_HIGH ? BITNA: 0)
    _RVM1(0x, PAD_UART_TX_OUT, BITNA),
    _RVM1(0x, PAD_UART_TX_OEN, BITNA),
    #endif

    #if(PAD_PWM0_IS_GPIO != GPIO_NONE)
    #define PWM0_IS_GPIO    (PAD_PWM0_IS_GPIO ? BIT2: 0)
    #define PAD_PWM0_OEN (PAD_PWM0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PWM0_OUT (PAD_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e38, PWM0_IS_GPIO, BIT2),
    _RVM1(0x0f2c, PAD_PWM0_OUT, BIT1),
    _RVM1(0x0f2c, PAD_PWM0_OEN, BIT0),
    #endif

    #if(PAD_PWM1_IS_GPIO != GPIO_NONE)
    #define PWM1_IS_GPIO    (PAD_PWM1_IS_GPIO ? BIT3: 0)
    #define PAD_PWM1_OEN (PAD_PWM1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PWM1_OUT (PAD_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e38, PWM1_IS_GPIO, BIT3),
    _RVM1(0x0f2e, PAD_PWM1_OUT, BIT1),
    _RVM1(0x0f2e, PAD_PWM1_OEN, BIT0),
    #endif

    #if(PAD_PWM2_IS_GPIO != GPIO_NONE)
    #define PWM2_IS_GPIO    (PAD_PWM2_IS_GPIO ? BIT1: 0)
    #define PAD_PWM2_OEN (PAD_PWM2_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PWM2_OUT (PAD_PWM2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e39, PWM2_IS_GPIO, BIT1),
    _RVM1(0x0f26, PAD_PWM2_OUT, BIT1),
    _RVM1(0x0f26, PAD_PWM2_OEN, BIT0),
    #endif

    #if(PAD_SAR0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_IS_GPIO BIT3|BIT2|BIT1|BIT0
    #define PAD_SAR0_OEN (PAD_SAR0_IS_GPIO == GPIO_IN ? BIT8: 0)
    #define PAD_SAR0_OUT (PAD_SAR0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1422, 0, PAD_SAR_IS_GPIO), // [3:0] to zero
    _RVM1(0x1424, PAD_SAR0_OUT, BIT0),
    _RVM1(0x1422, PAD_SAR0_OEN, BIT8),
    #endif

    #if(PAD_SAR1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_IS_GPIO BIT3|BIT2|BIT1|BIT0
    #define PAD_SAR1_OEN (PAD_SAR1_IS_GPIO == GPIO_IN ? BIT9: 0)
    #define PAD_SAR1_OUT (PAD_SAR1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1422, 0, PAD_SAR_IS_GPIO), // [3:0] to zero
    _RVM1(0x1424, PAD_SAR1_OUT, BIT1),
    _RVM1(0x1422, PAD_SAR1_OEN, BIT9),
    #endif

    #if(PAD_SAR2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_IS_GPIO BIT3|BIT2|BIT1|BIT0
    #define PAD_SAR2_OEN (PAD_SAR2_IS_GPIO == GPIO_IN ? BIT10: 0)
    #define PAD_SAR2_OUT (PAD_SAR2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1422, 0, PAD_SAR_IS_GPIO), // [3:0] to zero
    _RVM1(0x1424, PAD_SAR2_OUT, BIT2),
    _RVM1(0x1422, PAD_SAR2_OEN, BIT10),
    #endif

    #if(PAD_SAR3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_IS_GPIO BIT3|BIT2|BIT1|BIT0
    #define PAD_SAR3_OEN (PAD_SAR3_IS_GPIO == GPIO_IN ? BIT11: 0)
    #define PAD_SAR3_OUT (PAD_SAR3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1422, 0, PAD_SAR_IS_GPIO),// [3:0] to zero
    _RVM1(0x1424, PAD_SAR3_OUT, BIT3),
    _RVM1(0x1422, PAD_SAR3_OEN, BIT11),
    #endif

    _MEMMAP_nonPM_,

    #if(PAD_SM0_IO_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_IO_OEN (PAD_SM0_IO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_IO_OUT (PAD_SM0_IO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2500, PAD_SM0_IO_OUT, BIT4),
    _RVM1(0x2500, PAD_SM0_IO_OEN, BIT5),
    #endif

    #if(PAD_SM0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CLK_OEN (PAD_SM0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CLK_OUT (PAD_SM0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2502, PAD_SM0_CLK_OUT, BIT4),
    _RVM1(0x2502, PAD_SM0_CLK_OEN, BIT5),
    #endif

    #if(PAD_SM0_RST_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_RST_OEN (PAD_SM0_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_RST_OUT (PAD_SM0_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2504, PAD_SM0_RST_OUT, BIT4),
    _RVM1(0x2504, PAD_SM0_RST_OEN, BIT5),
    #endif

    #if(PAD_SM0_CD_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_CD_OEN (PAD_SM0_CD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_CD_OUT (PAD_SM0_CD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2506, PAD_SM0_CD_OUT, BIT4),
    _RVM1(0x2506, PAD_SM0_CD_OEN, BIT5),
    #endif

    #if(PAD_SM0_VCC_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_VCC_OEN (PAD_SM0_VCC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_VCC_OUT (PAD_SM0_VCC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2508, PAD_SM0_VCC_OUT, BIT4),
    _RVM1(0x2508, PAD_SM0_VCC_OEN, BIT5),
    #endif

    #if(PAD_SM0_C4_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_C4_OEN (PAD_SM0_C4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_C4_OUT (PAD_SM0_C4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250a, PAD_SM0_C4_OUT, BIT4),
    _RVM1(0x250a, PAD_SM0_C4_OEN, BIT5),
    #endif

    #if(PAD_SM0_C8_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_C8_OEN (PAD_SM0_C8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_C8_OUT (PAD_SM0_C8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250c, PAD_SM0_C8_OUT, BIT4),
    _RVM1(0x250c, PAD_SM0_C8_OEN, BIT5),
    #endif

    #if(PAD_SM0_SWITCH_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_SWITCH_OEN (PAD_SM0_SWITCH_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_SWITCH_OUT (PAD_SM0_SWITCH_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250e, PAD_SM0_SWITCH_OUT, BIT4),
    _RVM1(0x250e, PAD_SM0_SWITCH_OEN, BIT5),
    #endif

    #if(PAD_I2CM0_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SDA_OEN (PAD_I2CM0_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SDA_OUT (PAD_I2CM0_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251c, PAD_I2CM0_SDA_OUT, BIT4),
    _RVM1(0x251c, PAD_I2CM0_SDA_OEN, BIT5),
    #endif

    #if(PAD_I2CM0_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM0_SCL_OEN (PAD_I2CM0_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM0_SCL_OUT (PAD_I2CM0_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x251e, PAD_I2CM0_SCL_OUT, BIT4),
    _RVM1(0x251e, PAD_I2CM0_SCL_OEN, BIT5),
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2524, PAD_TS0_CLK_OUT, BIT4),
    _RVM1(0x2524, PAD_TS0_CLK_OEN, BIT5),
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2526, PAD_TS0_VLD_OUT, BIT4),
    _RVM1(0x2526, PAD_TS0_VLD_OEN, BIT5),
    #endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2528, PAD_TS0_SYNC_OUT, BIT4),
    _RVM1(0x2528, PAD_TS0_SYNC_OEN, BIT5),
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252a, PAD_TS0_D7_OUT, BIT4),
    _RVM1(0x252a, PAD_TS0_D7_OEN, BIT5),
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252c, PAD_TS0_D6_OUT, BIT4),
    _RVM1(0x252c, PAD_TS0_D6_OEN, BIT5),
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x252e, PAD_TS0_D5_OUT, BIT4),
    _RVM1(0x252e, PAD_TS0_D5_OEN, BIT5),
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2530, PAD_TS0_D4_OUT, BIT4),
    _RVM1(0x2530, PAD_TS0_D4_OEN, BIT5),
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2532, PAD_TS0_D3_OUT, BIT4),
    _RVM1(0x2532, PAD_TS0_D3_OEN, BIT5),
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2534, PAD_TS0_D2_OUT, BIT4),
    _RVM1(0x2534, PAD_TS0_D2_OEN, BIT5),
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2536, PAD_TS0_D1_OUT, BIT4),
    _RVM1(0x2536, PAD_TS0_D1_OEN, BIT5),
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2538, PAD_TS0_D0_OUT, BIT4),
    _RVM1(0x2538, PAD_TS0_D0_OEN, BIT5),
    #endif

    #if(PAD_FUART_CTS_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_CTS_OEN (PAD_FUART_CTS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_CTS_OUT (PAD_FUART_CTS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253a, PAD_FUART_CTS_OUT, BIT4),
    _RVM1(0x253a, PAD_FUART_CTS_OEN, BIT5),
    #endif

    #if(PAD_FUART_RTS_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_RTS_OEN (PAD_FUART_RTS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_RTS_OUT (PAD_FUART_RTS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2540, PAD_FUART_RTS_OUT, BIT4),
    _RVM1(0x2540, PAD_FUART_RTS_OEN, BIT5),
    #endif

    #if(PAD_FUART_TX_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_TX_OEN (PAD_FUART_TX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_TX_OUT (PAD_FUART_TX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2542, PAD_FUART_TX_OUT, BIT4),
    _RVM1(0x2542, PAD_FUART_TX_OEN, BIT5),
    #endif

    #if(PAD_FUART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_RX_OEN (PAD_FUART_RX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_RX_OUT (PAD_FUART_RX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2544, PAD_FUART_RX_OUT, BIT4),
    _RVM1(0x2544, PAD_FUART_RX_OEN, BIT5),
    #endif

    #if(PAD_PWR_CTRL_IS_GPIO != GPIO_NONE)
    #define PAD_PWR_CTRL_OEN (PAD_PWR_CTRL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PWR_CTRL_OUT (PAD_PWR_CTRL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253e, PAD_PWR_CTRL_OUT, BIT4),
    _RVM1(0x253e, PAD_PWR_CTRL_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_MCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MCK_OEN (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MCK_OUT (PAD_I2S_OUT_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253c, PAD_I2S_OUT_MCK_OUT, BIT4),
    _RVM1(0x253c, PAD_I2S_OUT_MCK_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_MUTE_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MUTE_OEN (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MUTE_OUT (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2546, PAD_I2S_OUT_MUTE_OUT, BIT4),
    _RVM1(0x2546, PAD_I2S_OUT_MUTE_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_BCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_BCK_OEN (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_BCK_OUT (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2548, PAD_I2S_OUT_BCK_OUT, BIT4),
    _RVM1(0x2548, PAD_I2S_OUT_BCK_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_SD0_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SD0_OEN (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SD0_OUT (PAD_I2S_OUT_SD0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254a, PAD_I2S_OUT_SD0_OUT, BIT4),
    _RVM1(0x254a, PAD_I2S_OUT_SD0_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_WS_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_WS_OEN (PAD_I2S_OUT_WS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_WS_OUT (PAD_I2S_OUT_WS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254c, PAD_I2S_OUT_WS_OUT, BIT4),
    _RVM1(0x254c, PAD_I2S_OUT_WS_OEN, BIT5),
    #endif

    #if(PAD_SPDIF_IN_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_IN_OEN (PAD_SPDIF_IN_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SPDIF_IN_OUT (PAD_SPDIF_IN_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254e, PAD_SPDIF_IN_OUT, BIT4),
    _RVM1(0x254e, PAD_SPDIF_IN_OEN, BIT5),
    #endif

    #if(PAD_SPI_WP_IS_GPIO != GPIO_NONE)
    #define PAD_SPI_WP_OEN (PAD_SPI_WP_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SPI_WP_OUT (PAD_SPI_WP_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2600, PAD_SPI_WP_OUT, BIT4),
    _RVM1(0x2600, PAD_SPI_WP_OEN, BIT5),
    #endif

    #if(PAD_GPIO_MCK_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_MCK_OEN (PAD_GPIO_MCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO_MCK_OUT (PAD_GPIO_MCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2602, PAD_GPIO_MCK_OUT, BIT4),
    _RVM1(0x2602, PAD_GPIO_MCK_OEN, BIT5),
    #endif

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x262c, PAD_SPDIF_OUT_OUT, BIT4),
    _RVM1(0x262d, PAD_SPDIF_OUT_OEN, BIT0),
    #endif

    #if(PAD_HSYNC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_HSYNC_OUT_OEN (PAD_HSYNC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HSYNC_OUT_OUT (PAD_HSYNC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263c, PAD_HSYNC_OUT_OUT, BIT4),
    _RVM1(0x263c, PAD_HSYNC_OUT_OEN, BIT5),
    #endif

    #if(PAD_VSYNC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_VSYNC_OUT_OEN (PAD_VSYNC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_VSYNC_OUT_OUT (PAD_VSYNC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x263e, PAD_VSYNC_OUT_OUT, BIT4),
    _RVM1(0x263e, PAD_VSYNC_OUT_OEN, BIT5),
    #endif

    #if(PAD_S_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO0_OEN (PAD_S_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_S_GPIO0_OUT (PAD_S_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e40, PAD_S_GPIO0_OUT, BIT0),
    _RVM1(0x1e41, PAD_S_GPIO0_OEN, BIT0),
    #endif

    #if(PAD_S_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO1_OEN (PAD_S_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_S_GPIO1_OUT (PAD_S_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e40, PAD_S_GPIO1_OUT, BIT1),
    _RVM1(0x1e41, PAD_S_GPIO1_OEN, BIT1),
    #endif

    #if(PAD_S_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO2_OEN (PAD_S_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_S_GPIO2_OUT (PAD_S_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e40, PAD_S_GPIO2_OUT, BIT2),
    _RVM1(0x1e41, PAD_S_GPIO2_OEN, BIT2),
    #endif

    #if(PAD_S_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO3_OEN (PAD_S_GPIO3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_S_GPIO3_OUT (PAD_S_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e40, PAD_S_GPIO3_OUT, BIT3),
    _RVM1(0x1e41, PAD_S_GPIO3_OEN, BIT3),
    #endif

    #if(PAD_S_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO4_OEN (PAD_S_GPIO4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_S_GPIO4_OUT (PAD_S_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e40, PAD_S_GPIO4_OUT, BIT4),
    _RVM1(0x1e41, PAD_S_GPIO4_OEN, BIT4),
    #endif

    #if(PAD_S_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_S_GPIO5_OEN (PAD_S_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_S_GPIO5_OUT (PAD_S_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e40, PAD_S_GPIO5_OUT, BIT5),
    _RVM1(0x1e41, PAD_S_GPIO5_OEN, BIT5),
    #endif


//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------

    _MEMMAP_nonPM_,

    #define SM0_OPEN ((PADS_SM0_OPEN == PAD_SM0_CLK) ? (BIT0) : \
                      (PADS_SM0_OPEN == PAD_SM0_RST ) ? (BIT0) : 0)

    _RVM1(0x1E18, SM0_OPEN, BIT0),


    #define SM0_EN  ((PADS_SM0_EN == PAD_SM0_IO) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_CLK) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_RST) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_CD) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_VCC) ? (BIT0) : 0)

    _RVM1(0x1E19, SM0_EN, BIT0),


    #define I2CM0_MODE ((PADS_I2CM0_MODE == PAD_I2CM0_SDA  ) ? (BIT0) : \
                      (PADS_I2CM0_MODE == PAD_I2CM0_SCL) ? (BIT0) : \
                      (PADS_I2CM0_MODE == PAD_SM0_IO) ? (BIT1) : \
                      (PADS_I2CM0_MODE == PAD_SM0_CLK) ? (BIT1) : \
                      (PADS_I2CM0_MODE == PAD_I2S_OUT_WS) ? (BIT1|BIT0) : \
                      (PADS_I2CM0_MODE == PAD_I2S_OUT_SD0) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E12, I2CM0_MODE, BIT1|BIT0),


    #define TS0_MODE ((PADS_TS0_MODE == PAD_TS0_CLK) ? (BIT0) : \
                      (PADS_TS0_MODE == PAD_TS0_D7) ? (BIT1) : \
                      (PADS_TS0_MODE == PAD_TS0_D0) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E04, TS0_MODE, BITMASK(2:0)),


    #define CCIR_OUT_MODE ((PADS_CCIR_OUT_MODE == PAD_TS0_CLK) ? (BIT4) : 0)

    _RVM1(0x1E10, CCIR_OUT_MODE, BIT4),


    #define CCIR_IN_MODE ((PADS_CCIR_IN_MODE == PAD_TS0_CLK) ? (BIT0) : 0)

    _RVM1(0x1E10, CCIR_OUT_MODE, BIT0),


    #define SPDIF_OUT_MODE ((PADS_SPDIF_OUT_MODE == PAD_SM0_SWITCH) ? (BIT4) : \
                            (PADS_SPDIF_OUT_MODE == PAD_TS0_CLK) ? (BIT5) : \
                            (PADS_SPDIF_OUT_MODE == PAD_SPDIF_OUT) ? (BIT5|BIT4) : \
                            (PADS_SPDIF_OUT_MODE == PAD_SPDIF_IN) ? (BIT6) : \
                            (PADS_SPDIF_OUT_MODE == PAD_S_GPIO0) ? (BIT6|BIT4) : 0)

    _RVM1(0x1E0A, SPDIF_OUT_MODE, BITMASK(6:4)),


    #define SPDIF_IN_MODE ((PADS_SPDIF_IN_MODE == PAD_SM0_SWITCH) ? (BIT0) : \
                            (PADS_SPDIF_IN_MODE == PAD_TS0_CLK) ? (BIT1) : \
                            (PADS_SPDIF_IN_MODE == PAD_SPDIF_IN) ? (BIT1|BIT0) : \
                            (PADS_SPDIF_IN_MODE == PAD_SPDIF_OUT) ? (BIT2) : \
                            (PADS_SPDIF_IN_MODE == PAD_SPI_WP) ? (BIT2|BIT0) : 0)

    _RVM1(0x1E0A, SPDIF_IN_MODE, BITMASK(2:0)),


    #define I2S_IN_MODE ((PADS_I2S_IN_MODE == PAD_FUART_TX) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E0B, I2S_IN_MODE, BITMASK(1:0)),


    #define PWM_MODE ((PADS_PWM_MODE == PAD_HSYNC_OUT) ? (BIT0) : \
                        (PADS_PWM_MODE == PAD_VSYNC_OUT) ? (BIT0) : 0)

    _RVM1(0x1E13, PWM_MODE, BIT0),


    #define HSYNC_EN ((PADS_HSYNC_EN == PAD_HSYNC_OUT) ? (BIT0) : 0)

    _RVM1(0x1E16, HSYNC_EN, BIT0),

    #define VSYNC_EN ((PADS_VSYNC_EN == PAD_VSYNC_OUT) ? (BIT4) : 0)

    _RVM1(0x1E16, VSYNC_EN, BIT4),

    // UART MUX
    #define SECOND_UART_MODE ((PADS_SECOND_UART_MODE == PAD_HSYNC_OUT) ? (BIT0) : \
                        (PADS_SECOND_UART_MODE == PAD_VSYNC_OUT) ? (BIT0) : \
                        (PADS_SECOND_UART_MODE == PAD_SM0_IO) ? (BIT1) : \
                        (PADS_SECOND_UART_MODE == PAD_SM0_CLK) ? (BIT1) : \
                        (PADS_SECOND_UART_MODE == PAD_FUART_RX) ? (BIT1|BIT0) : \
                        (PADS_SECOND_UART_MODE == PAD_FUART_TX) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E06, SECOND_UART_MODE, BITMASK(2:0)),


    #define THIRD_UART_MODE ((PADS_THIRD_UART_MODE == PAD_FUART_RTS) ? (BIT4) : \
                           (PADS_THIRD_UART_MODE == PAD_FUART_CTS) ? (BIT4) : 0)

    _RVM1(0x1E06, THIRD_UART_MODE, BITMASK(5:4)),


    #define FOURTH_UART_MODE ((PADS_FOURTH_UART_MODE == PAD_FUART_RX) ? (BIT0) : \
                           (PADS_FOURTH_UART_MODE == PAD_FUART_TX) ? (BIT0) : \
                           (PADS_FOURTH_UART_MODE == PAD_TS0_D1) ? (BIT1) : \
                           (PADS_FOURTH_UART_MODE == PAD_TS0_D2) ? (BIT1) : \
                           (PADS_FOURTH_UART_MODE == PAD_HSYNC_OUT) ? (BIT1|BIT0) : \
                           (PADS_FOURTH_UART_MODE == PAD_VSYNC_OUT) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E07, FOURTH_UART_MODE, BITMASK(1:0)),

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

    _MEMMAP_nonPM_,

    // Clear all pad in
    _RVM1(0x1EA1, 0, BIT7),
    _END_OF_TBL2_,

//---------------------------------------------------------------------
// ISP_TOOL Write Protect (Need to check ????)
//---------------------------------------------------------------------

    //external flash #1
    _MEMMAP_PM_,
    _RVM1(0x0E6A, 0x00, BIT0), //reg_spi_gpio set to 0 (swtch to spi_do, spi_di, spi_ck)
    _RVM1(0x0E6A, 0x00, BIT1), //reg_spicsz0_gpio set to 0 (enable spi_cz0)
    _RVM1(0x08F4, 0x00, 0xFF), //reg_chip_select set to 0 (select embedded SPI Flash)
    #if (PIN_FLASH_WP0)
    _RVM1(0x0E21, BIT0, BIT0),
    _RVM1(0x0E1F,    0, BIT0),
    #endif
    _END_OF_TBL2_,

    //external flash #2
    _MEMMAP_PM_,
    _RVM1(0x0E6A, 0x00, BIT0), //reg_spi_gpio set to 0 (swtch to spi_do, spi_di, spi_ck)
    #if (PIN_SPI_CZ1)
    _RVM1(0x0E6A, 0x00, BIT2), //reg_spicsz1_gpio set to 0 (enable spi_cz1)
    _RVM1(0x08F4, 0x01, 0xFF), //reg_chip_select set to 1 (select external flash #1)
    #if (PIN_FLASH_WP1)
    _RVM1(0x0E21, BIT0, BIT0),
    _RVM1(0x0E1F,    0, BIT0),
    #endif
    _END_OF_TBL2_,
    #endif

};

//-----------------------------------------------------------------
// Sanity check for PAD_TOP setting (TODO)
//-----------------------------------------------------------------

#if (UART0_SRC_SEL == UART1_SRC_SEL) || \
    (UART0_SRC_SEL == UART2_SRC_SEL) || \
    (UART1_SRC_SEL == UART2_SRC_SEL)
    #error "UART src selection conflict"
#endif



