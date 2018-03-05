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
#include "hwreg_Keltic.h"
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
    _MEMMAP_PM_,


    // IRIN

    // GPIO_PM0~GPIO_PM12
    #if(PAD_GPIO_PM4_IS_GPIO != GPIO_NONE)
        _RVM2(0x0e24, 0xBABE, 0xFFFF),
    #endif

    #define GPIO_PM6_IS_GPIO    (PAD_GPIO_PM6_IS_GPIO ? BIT2: 0)

    #if(PAD_GPIO_PM6_IS_GPIO != GPIO_NONE)
        _RVM1(0x0E6A, GPIO_PM6_IS_GPIO, BIT2),
    #endif



    // SAR
    // SAR
    #if(PAD_SAR_GPIO0_IS_GPIO != GPIO_NONE)
    _RVM1(0x1422, 0, BIT0),
    #endif
    #if(PAD_SAR_GPIO1_IS_GPIO != GPIO_NONE)
    _RVM1(0x1422, 0, BIT1),
    #endif
    #if(PAD_SAR_GPIO2_IS_GPIO != GPIO_NONE)
    _RVM1(0x1422, 0, BIT2),
    #endif
    #if(PAD_SAR_GPIO3_IS_GPIO != GPIO_NONE)
    _RVM1(0x1422, 0, BIT3),
    #endif

    #define SAR_OEN         ((PAD_SAR_GPIO0_IS_GPIO == GPIO_IN ? (1UL <<  0) : 0) | \
                             (PAD_SAR_GPIO1_IS_GPIO == GPIO_IN ? (1UL <<  1) : 0) | \
                             (PAD_SAR_GPIO2_IS_GPIO == GPIO_IN ? (1UL <<  2) : 0) | \
                             (PAD_SAR_GPIO3_IS_GPIO == GPIO_IN ? (1UL <<  3) : 0))

    #define SAR_OUT         ((PAD_SAR_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? (1UL <<  0) : 0) | \
                             (PAD_SAR_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? (1UL <<  1) : 0) | \
                             (PAD_SAR_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? (1UL <<  2) : 0) | \
                             (PAD_SAR_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? (1UL <<  3) : 0))

    _RVM1(0x1424, SAR_OUT, BITMASK(3:0)),
    _RVM1(0x1423, SAR_OEN, BITMASK(3:0)),


    _MEMMAP_nonPM_,

// Script Generated GPIO

    #if(PAD_CI_A14_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A14_OEN (PAD_CI_A14_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A14_OUT (PAD_CI_A14_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255a, PAD_CI_A14_OUT, BIT4),
    _RVM1(0x255a, PAD_CI_A14_OEN, BIT5),
    #endif

    #if(PAD_CI_A13_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A13_OEN (PAD_CI_A13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A13_OUT (PAD_CI_A13_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255c, PAD_CI_A13_OUT, BIT4),
    _RVM1(0x255c, PAD_CI_A13_OEN, BIT5),
    #endif

    #if(PAD_CI_A12_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A12_OEN (PAD_CI_A12_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A12_OUT (PAD_CI_A12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x255e, PAD_CI_A12_OUT, BIT4),
    _RVM1(0x255e, PAD_CI_A12_OEN, BIT5),
    #endif

    #if(PAD_CI_A11_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A11_OEN (PAD_CI_A11_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A11_OUT (PAD_CI_A11_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2560, PAD_CI_A11_OUT, BIT4),
    _RVM1(0x2560, PAD_CI_A11_OEN, BIT5),
    #endif

    #if(PAD_CI_A10_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A10_OEN (PAD_CI_A10_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A10_OUT (PAD_CI_A10_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2562, PAD_CI_A10_OUT, BIT4),
    _RVM1(0x2562, PAD_CI_A10_OEN, BIT5),
    #endif

    #if(PAD_CI_A9_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A9_OEN (PAD_CI_A9_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A9_OUT (PAD_CI_A9_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2564, PAD_CI_A9_OUT, BIT4),
    _RVM1(0x2564, PAD_CI_A9_OEN, BIT5),
    #endif

    #if(PAD_CI_A8_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A8_OEN (PAD_CI_A8_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A8_OUT (PAD_CI_A8_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2566, PAD_CI_A8_OUT, BIT4),
    _RVM1(0x2566, PAD_CI_A8_OEN, BIT5),
    #endif

    #if(PAD_CI_A7_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A7_OEN (PAD_CI_A7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A7_OUT (PAD_CI_A7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2568, PAD_CI_A7_OUT, BIT4),
    _RVM1(0x2568, PAD_CI_A7_OEN, BIT5),
    #endif

    #if(PAD_CI_A6_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A6_OEN (PAD_CI_A6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A6_OUT (PAD_CI_A6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x256a, PAD_CI_A6_OUT, BIT4),
    _RVM1(0x256a, PAD_CI_A6_OEN, BIT5),
    #endif

    #if(PAD_CI_A5_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A5_OEN (PAD_CI_A5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A5_OUT (PAD_CI_A5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x256c, PAD_CI_A5_OUT, BIT4),
    _RVM1(0x256c, PAD_CI_A5_OEN, BIT5),
    #endif

    #if(PAD_CI_A4_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A4_OEN (PAD_CI_A4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A4_OUT (PAD_CI_A4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x256e, PAD_CI_A4_OUT, BIT4),
    _RVM1(0x256e, PAD_CI_A4_OEN, BIT5),
    #endif

    #if(PAD_CI_A3_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A3_OEN (PAD_CI_A3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A3_OUT (PAD_CI_A3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2570, PAD_CI_A3_OUT, BIT4),
    _RVM1(0x2570, PAD_CI_A3_OEN, BIT5),
    #endif

    #if(PAD_CI_A2_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A2_OEN (PAD_CI_A2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A2_OUT (PAD_CI_A2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2572, PAD_CI_A2_OUT, BIT4),
    _RVM1(0x2572, PAD_CI_A2_OEN, BIT5),
    #endif

    #if(PAD_CI_A1_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A1_OEN (PAD_CI_A1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A1_OUT (PAD_CI_A1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2574, PAD_CI_A1_OUT, BIT4),
    _RVM1(0x2574, PAD_CI_A1_OEN, BIT5),
    #endif

    #if(PAD_CI_A0_IS_GPIO != GPIO_NONE)
    #define PAD_CI_A0_OEN (PAD_CI_A0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_A0_OUT (PAD_CI_A0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2576, PAD_CI_A0_OUT, BIT4),
    _RVM1(0x2576, PAD_CI_A0_OEN, BIT5),
    #endif

    #if(PAD_CI_D7_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D7_OEN (PAD_CI_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D7_OUT (PAD_CI_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2578, PAD_CI_D7_OUT, BIT4),
    _RVM1(0x2578, PAD_CI_D7_OEN, BIT5),
    #endif

    #if(PAD_CI_D6_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D6_OEN (PAD_CI_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D6_OUT (PAD_CI_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257a, PAD_CI_D6_OUT, BIT4),
    _RVM1(0x257a, PAD_CI_D6_OEN, BIT5),
    #endif

    #if(PAD_CI_D5_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D5_OEN (PAD_CI_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D5_OUT (PAD_CI_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257c, PAD_CI_D5_OUT, BIT4),
    _RVM1(0x257c, PAD_CI_D5_OEN, BIT5),
    #endif

    #if(PAD_CI_D4_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D4_OEN (PAD_CI_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D4_OUT (PAD_CI_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x257e, PAD_CI_D4_OUT, BIT4),
    _RVM1(0x257e, PAD_CI_D4_OEN, BIT5),
    #endif

    #if(PAD_CI_D3_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D3_OEN (PAD_CI_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D3_OUT (PAD_CI_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2580, PAD_CI_D3_OUT, BIT4),
    _RVM1(0x2580, PAD_CI_D3_OEN, BIT5),
    #endif

    #if(PAD_CI_D2_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D2_OEN (PAD_CI_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D2_OUT (PAD_CI_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2582, PAD_CI_D2_OUT, BIT4),
    _RVM1(0x2582, PAD_CI_D2_OEN, BIT5),
    #endif

    #if(PAD_CI_D1_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D1_OEN (PAD_CI_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D1_OUT (PAD_CI_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2584, PAD_CI_D1_OUT, BIT4),
    _RVM1(0x2584, PAD_CI_D1_OEN, BIT5),
    #endif

    #if(PAD_CI_D0_IS_GPIO != GPIO_NONE)
    #define PAD_CI_D0_OEN (PAD_CI_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_D0_OUT (PAD_CI_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2586, PAD_CI_D0_OUT, BIT4),
    _RVM1(0x2586, PAD_CI_D0_OEN, BIT5),
    #endif

    #if(PAD_CIORDZ_IS_GPIO != GPIO_NONE)
    #define PAD_CIORDZ_OEN (PAD_CIORDZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CIORDZ_OUT (PAD_CIORDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2588, PAD_CIORDZ_OUT, BIT4),
    _RVM1(0x2588, PAD_CIORDZ_OEN, BIT5),
    #endif

    #if(PAD_CIOWRZ_IS_GPIO != GPIO_NONE)
    #define PAD_CIOWRZ_OEN (PAD_CIOWRZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CIOWRZ_OUT (PAD_CIOWRZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258a, PAD_CIOWRZ_OUT, BIT4),
    _RVM1(0x258a, PAD_CIOWRZ_OEN, BIT5),
    #endif

    #if(PAD_CI_OEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_OEZ_OEN (PAD_CI_OEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_OEZ_OUT (PAD_CI_OEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258c, PAD_CI_OEZ_OUT, BIT4),
    _RVM1(0x258c, PAD_CI_OEZ_OEN, BIT5),
    #endif

    #if(PAD_CI_WEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_WEZ_OEN (PAD_CI_WEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_WEZ_OUT (PAD_CI_WEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x258e, PAD_CI_WEZ_OUT, BIT4),
    _RVM1(0x258e, PAD_CI_WEZ_OEN, BIT5),
    #endif

    #if(PAD_CI_REGZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_REGZ_OEN (PAD_CI_REGZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_REGZ_OUT (PAD_CI_REGZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2590, PAD_CI_REGZ_OUT, BIT4),
    _RVM1(0x2590, PAD_CI_REGZ_OEN, BIT5),
    #endif

    #if(PAD_CI_CEZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_CEZ_OEN (PAD_CI_CEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_CEZ_OUT (PAD_CI_CEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2592, PAD_CI_CEZ_OUT, BIT4),
    _RVM1(0x2592, PAD_CI_CEZ_OEN, BIT5),
    #endif

    #if(PAD_CIRQAZ_IS_GPIO != GPIO_NONE)
    #define PAD_CIRQAZ_OEN (PAD_CIRQAZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CIRQAZ_OUT (PAD_CIRQAZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2594, PAD_CIRQAZ_OUT, BIT4),
    _RVM1(0x2594, PAD_CIRQAZ_OEN, BIT5),
    #endif

    #if(PAD_CI_WAITZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_WAITZ_OEN (PAD_CI_WAITZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_WAITZ_OUT (PAD_CI_WAITZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2596, PAD_CI_WAITZ_OUT, BIT4),
    _RVM1(0x2596, PAD_CI_WAITZ_OEN, BIT5),
    #endif

    #if(PAD_CI_RST_IS_GPIO != GPIO_NONE)
    #define PAD_CI_RST_OEN (PAD_CI_RST_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_RST_OUT (PAD_CI_RST_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2598, PAD_CI_RST_OUT, BIT4),
    _RVM1(0x2598, PAD_CI_RST_OEN, BIT5),
    #endif

    #if(PAD_CI_CDZ_IS_GPIO != GPIO_NONE)
    #define PAD_CI_CDZ_OEN (PAD_CI_CDZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_CI_CDZ_OUT (PAD_CI_CDZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259a, PAD_CI_CDZ_OUT, BIT4),
    _RVM1(0x259a, PAD_CI_CDZ_OEN, BIT5),
    #endif

    #if(PAD_DISEQC_IN_IS_GPIO != GPIO_NONE)
    #define PAD_DISEQC_IN_OEN (PAD_DISEQC_IN_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DISEQC_IN_OUT (PAD_DISEQC_IN_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259c, PAD_DISEQC_IN_OUT, BIT4),
    _RVM1(0x259c, PAD_DISEQC_IN_OEN, BIT5),
    #endif

    #if(PAD_DISEQC_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_DISEQC_OUT_OEN (PAD_DISEQC_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DISEQC_OUT_OUT (PAD_DISEQC_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x259e, PAD_DISEQC_OUT_OUT, BIT4),
    _RVM1(0x259e, PAD_DISEQC_OUT_OEN, BIT5),
    #endif

    #if(PAD_DISEQC_HV_IS_GPIO != GPIO_NONE)
    #define PAD_DISEQC_HV_OEN (PAD_DISEQC_HV_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DISEQC_HV_OUT (PAD_DISEQC_HV_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a0, PAD_DISEQC_HV_OUT, BIT4),
    _RVM1(0x25a0, PAD_DISEQC_HV_OEN, BIT5),
    #endif

    #if(PAD_FUART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_RX_OEN (PAD_FUART_RX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_RX_OUT (PAD_FUART_RX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a8, PAD_FUART_RX_OUT, BIT4),
    _RVM1(0x25a8, PAD_FUART_RX_OEN, BIT5),
    #endif

    #if(PAD_FUART_TX_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_TX_OEN (PAD_FUART_TX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_TX_OUT (PAD_FUART_TX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25aa, PAD_FUART_TX_OUT, BIT4),
    _RVM1(0x25aa, PAD_FUART_TX_OEN, BIT5),
    #endif

    #if(PAD_FUART_CTS_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_CTS_OEN (PAD_FUART_CTS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_CTS_OUT (PAD_FUART_CTS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ac, PAD_FUART_CTS_OUT, BIT4),
    _RVM1(0x25ac, PAD_FUART_CTS_OEN, BIT5),
    #endif

    #if(PAD_FUART_RTS_IS_GPIO != GPIO_NONE)
    #define PAD_FUART_RTS_OEN (PAD_FUART_RTS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_FUART_RTS_OUT (PAD_FUART_RTS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ae, PAD_FUART_RTS_OUT, BIT4),
    _RVM1(0x25ae, PAD_FUART_RTS_OEN, BIT5),
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

    #if(PAD_I2CM1_SDA_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SDA_OEN (PAD_I2CM1_SDA_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SDA_OUT (PAD_I2CM1_SDA_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2520, PAD_I2CM1_SDA_OUT, BIT4),
    _RVM1(0x2520, PAD_I2CM1_SDA_OEN, BIT5),
    #endif

    #if(PAD_I2CM1_SCL_IS_GPIO != GPIO_NONE)
    #define PAD_I2CM1_SCL_OEN (PAD_I2CM1_SCL_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2CM1_SCL_OUT (PAD_I2CM1_SCL_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2522, PAD_I2CM1_SCL_OUT, BIT4),
    _RVM1(0x2522, PAD_I2CM1_SCL_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_MUTE_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MUTE_OEN (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MUTE_OUT (PAD_I2S_OUT_MUTE_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x262e, PAD_I2S_OUT_MUTE_OUT, BIT4),
    _RVM1(0x262e, PAD_I2S_OUT_MUTE_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_BCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_BCK_OEN (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_BCK_OUT (PAD_I2S_OUT_BCK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2630, PAD_I2S_OUT_BCK_OUT, BIT4),
    _RVM1(0x2630, PAD_I2S_OUT_BCK_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_WS_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_WS_OEN (PAD_I2S_OUT_WS_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_WS_OUT (PAD_I2S_OUT_WS_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2632, PAD_I2S_OUT_WS_OUT, BIT4),
    _RVM1(0x2632, PAD_I2S_OUT_WS_OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_MCK_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_MCK _OEN (PAD_I2S_OUT_MCK _IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_MCK _OUT (PAD_I2S_OUT_MCK _IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2634, PAD_I2S_OUT_MCK _OUT, BIT4),
    _RVM1(0x2634, PAD_I2S_OUT_MCK _OEN, BIT5),
    #endif

    #if(PAD_I2S_OUT_SDO_IS_GPIO != GPIO_NONE)
    #define PAD_I2S_OUT_SDO_OEN (PAD_I2S_OUT_SDO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_I2S_OUT_SDO_OUT (PAD_I2S_OUT_SDO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2636, PAD_I2S_OUT_SDO_OUT, BIT4),
    _RVM1(0x2636, PAD_I2S_OUT_SDO_OEN, BIT5),
    #endif

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

    #if(PAD_SM0_GPIO_IS_GPIO != GPIO_NONE)
    #define PAD_SM0_GPIO_OEN (PAD_SM0_GPIO_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SM0_GPIO_OUT (PAD_SM0_GPIO_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x250d, PAD_SM0_GPIO_OUT, BIT4),
    _RVM1(0x250d, PAD_SM0_GPIO_OEN, BIT5),
    #endif

    #if(PAD_SPDIF_OUT_IS_GPIO != GPIO_NONE)
    #define PAD_SPDIF_OUT_OEN (PAD_SPDIF_OUT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_SPDIF_OUT_OUT (PAD_SPDIF_OUT_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a2, PAD_SPDIF_OUT_OUT, BIT4),
    _RVM1(0x25a2, PAD_SPDIF_OUT_OEN, BIT5),
    #endif

    #if(PAD_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO0_OEN (PAD_GPIO0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO0_OUT (PAD_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b0, PAD_GPIO0_OUT, BIT4),
    _RVM1(0x25b0, PAD_GPIO0_OEN, BIT5),
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b2, PAD_GPIO1_OUT, BIT4),
    _RVM1(0x25b2, PAD_GPIO1_OEN, BIT5),
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b4, PAD_GPIO2_OUT, BIT4),
    _RVM1(0x25b4, PAD_GPIO2_OEN, BIT5),
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b6, PAD_GPIO3_OUT, BIT4),
    _RVM1(0x25b6, PAD_GPIO3_OEN, BIT5),
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25b8, PAD_GPIO4_OUT, BIT4),
    _RVM1(0x25b8, PAD_GPIO4_OEN, BIT5),
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25ba, PAD_GPIO5_OUT, BIT4),
    _RVM1(0x25ba, PAD_GPIO5_OEN, BIT5),
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

    #if(PAD_TS1_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_CLK_OEN (PAD_TS1_CLK_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_CLK_OUT (PAD_TS1_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253a, PAD_TS1_CLK_OUT, BIT4),
    _RVM1(0x253a, PAD_TS1_CLK_OEN, BIT5),
    #endif

    #if(PAD_TS1_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_SYNC_OEN (PAD_TS1_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_SYNC_OUT (PAD_TS1_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253c, PAD_TS1_SYNC_OUT, BIT4),
    _RVM1(0x253c, PAD_TS1_SYNC_OEN, BIT5),
    #endif

    #if(PAD_TS1_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_VLD_OEN (PAD_TS1_VLD_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_VLD_OUT (PAD_TS1_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253e, PAD_TS1_VLD_OUT, BIT4),
    _RVM1(0x253e, PAD_TS1_VLD_OEN, BIT5),
    #endif

    #if(PAD_TS1_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D7_OEN (PAD_TS1_D7_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D7_OUT (PAD_TS1_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2540, PAD_TS1_D7_OUT, BIT4),
    _RVM1(0x2540, PAD_TS1_D7_OEN, BIT5),
    #endif

    #if(PAD_TS1_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D6_OEN (PAD_TS1_D6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D6_OUT (PAD_TS1_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2542, PAD_TS1_D6_OUT, BIT4),
    _RVM1(0x2542, PAD_TS1_D6_OEN, BIT5),
    #endif

    #if(PAD_TS1_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D5_OEN (PAD_TS1_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D5_OUT (PAD_TS1_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2544, PAD_TS1_D5_OUT, BIT4),
    _RVM1(0x2544, PAD_TS1_D5_OEN, BIT5),
    #endif

    #if(PAD_TS1_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D4_OEN (PAD_TS1_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D4_OUT (PAD_TS1_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2546, PAD_TS1_D4_OUT, BIT4),
    _RVM1(0x2546, PAD_TS1_D4_OEN, BIT5),
    #endif

    #if(PAD_TS1_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D3_OEN (PAD_TS1_D3_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D3_OUT (PAD_TS1_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2548, PAD_TS1_D3_OUT, BIT4),
    _RVM1(0x2548, PAD_TS1_D3_OEN, BIT5),
    #endif

    #if(PAD_TS1_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D2_OEN (PAD_TS1_D2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D2_OUT (PAD_TS1_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254a, PAD_TS1_D2_OUT, BIT4),
    _RVM1(0x254a, PAD_TS1_D2_OEN, BIT5),
    #endif

    #if(PAD_TS1_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D1_OEN (PAD_TS1_D1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D1_OUT (PAD_TS1_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254c, PAD_TS1_D1_OUT, BIT4),
    _RVM1(0x254c, PAD_TS1_D1_OEN, BIT5),
    #endif

    #if(PAD_TS1_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D0_OEN (PAD_TS1_D0_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D0_OUT (PAD_TS1_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x254e, PAD_TS1_D0_OUT, BIT4),
    _RVM1(0x254e, PAD_TS1_D0_OEN, BIT5),
    #endif

    #if(PAD_UART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_UART_RX_OEN (PAD_UART_RX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_UART_RX_OUT (PAD_UART_RX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a4, PAD_UART_RX_OUT, BIT4),
    _RVM1(0x25a4, PAD_UART_RX_OEN, BIT5),
    #endif

    #if(PAD_UART_TX_IS_GPIO != GPIO_NONE)
    #define PAD_UART_TX_OEN (PAD_UART_TX_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_UART_TX_OUT (PAD_UART_TX_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x25a6, PAD_UART_TX_OUT, BIT4),
    _RVM1(0x25a6, PAD_UART_TX_OEN, BIT5),
    #endif

    _MEMMAP_PM_,
    
    #if(PAD_PM_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO0_OEN (PAD_PM_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO0_OUT (PAD_PM_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f00, PAD_PM_GPIO0_OUT, BIT1),
    _RVM1(0x0f00, PAD_PM_GPIO0_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO1_OEN (PAD_PM_GPIO1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO1_OUT (PAD_PM_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f02, PAD_PM_GPIO1_OUT, BIT1),
    _RVM1(0x0f02, PAD_PM_GPIO1_OEN, BIT0),
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
    #endif

    #if(PAD_PM_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO4_OEN (PAD_PM_GPIO4_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO4_OUT (PAD_PM_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f08, PAD_PM_GPIO4_OUT, BIT1),
    _RVM1(0x0f08, PAD_PM_GPIO4_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO5_OEN (PAD_PM_GPIO5_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO5_OUT (PAD_PM_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0a, PAD_PM_GPIO5_OUT, BIT1),
    _RVM1(0x0f0a, PAD_PM_GPIO5_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO6_OEN (PAD_PM_GPIO6_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO6_OUT (PAD_PM_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f0c, PAD_PM_GPIO6_OUT, BIT1),
    _RVM1(0x0f0c, PAD_PM_GPIO6_OEN, BIT0),
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
    #endif

    #if(PAD_PM_GPIO13_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO13_OEN (PAD_PM_GPIO13_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO13_OUT (PAD_PM_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1a, PAD_PM_GPIO13_OUT, BIT1),
    _RVM1(0x0f1a, PAD_PM_GPIO13_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO14_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO14_OEN (PAD_PM_GPIO14_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO14_OUT (PAD_PM_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1c, PAD_PM_GPIO14_OUT, BIT1),
    _RVM1(0x0f1c, PAD_PM_GPIO14_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO15_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO15_OEN (PAD_PM_GPIO15_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO15_OUT (PAD_PM_GPIO15_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f1e, PAD_PM_GPIO15_OUT, BIT1),
    _RVM1(0x0f1e, PAD_PM_GPIO15_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO16_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO16_OEN (PAD_PM_GPIO16_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO16_OUT (PAD_PM_GPIO16_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f20, PAD_PM_GPIO16_OUT, BIT1),
    _RVM1(0x0f20, PAD_PM_GPIO16_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO17_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO17_OEN (PAD_PM_GPIO17_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO17_OUT (PAD_PM_GPIO17_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f22, PAD_PM_GPIO17_OUT, BIT1),
    _RVM1(0x0f22, PAD_PM_GPIO17_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO18_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO18_OEN (PAD_PM_GPIO18_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO18_OUT (PAD_PM_GPIO18_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f24, PAD_PM_GPIO18_OUT, BIT1),
    _RVM1(0x0f24, PAD_PM_GPIO18_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO19_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO19_OEN (PAD_PM_GPIO19_IS_GPIO == GPIO_IN ? BIT0   : 0)
    #define PAD_PM_GPIO19_OUT (PAD_PM_GPIO19_IS_GPIO == GPIO_OUT_HIGH ? BIT1  : 0)
    _RVM1(0x0f26, PAD_PM_GPIO19_OUT, BIT1  ),
    _RVM1(0x0f26, PAD_PM_GPIO19_OEN, BIT0   ),
    #endif

    #if(PAD_PM_GPIO20_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO20_OEN (PAD_PM_GPIO20_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO20_OUT (PAD_PM_GPIO20_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f38, PAD_PM_GPIO20_OUT, BIT1),
    _RVM1(0x0f38, PAD_PM_GPIO20_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO21_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO21_OEN (PAD_PM_GPIO21_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO21_OUT (PAD_PM_GPIO21_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f3a, PAD_PM_GPIO21_OUT, BIT1),
    _RVM1(0x0f3a, PAD_PM_GPIO21_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO22_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO22_OEN (PAD_PM_GPIO22_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO22_OUT (PAD_PM_GPIO22_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f3c, PAD_PM_GPIO22_OUT, BIT1),
    _RVM1(0x0f3c, PAD_PM_GPIO22_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO23_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO23_OEN (PAD_PM_GPIO23_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO23_OUT (PAD_PM_GPIO23_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f3e, PAD_PM_GPIO23_OUT, BIT1),
    _RVM1(0x0f3e, PAD_PM_GPIO23_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO24_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO24_OEN (PAD_PM_GPIO24_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO24_OUT (PAD_PM_GPIO24_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f40, PAD_PM_GPIO24_OUT, BIT1),
    _RVM1(0x0f40, PAD_PM_GPIO24_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO25_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO25_OEN (PAD_PM_GPIO25_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO25_OUT (PAD_PM_GPIO25_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f42, PAD_PM_GPIO25_OUT, BIT1),
    _RVM1(0x0f42, PAD_PM_GPIO25_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO26_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO26_OEN (PAD_PM_GPIO26_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO26_OUT (PAD_PM_GPIO26_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f44, PAD_PM_GPIO26_OUT, BIT1),
    _RVM1(0x0f44, PAD_PM_GPIO26_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO27_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO27_OEN (PAD_PM_GPIO27_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO27_OUT (PAD_PM_GPIO27_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f46, PAD_PM_GPIO27_OUT, BIT1),
    _RVM1(0x0f46, PAD_PM_GPIO27_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO28_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO28_OEN (PAD_PM_GPIO28_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO28_OUT (PAD_PM_GPIO28_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f48, PAD_PM_GPIO28_OUT, BIT1),
    _RVM1(0x0f48, PAD_PM_GPIO28_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO29_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO29_OEN (PAD_PM_GPIO29_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_GPIO29_OUT (PAD_PM_GPIO29_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f4a, PAD_PM_GPIO29_OUT, BIT1),
    _RVM1(0x0f4a, PAD_PM_GPIO29_OEN, BIT0),
    #endif

    #if(PAD_PM_GPIO30_IS_GPIO != GPIO_NONE)
    #define PAD_PM_GPIO30_OEN (PAD_PM_GPIO30_IS_GPIO == GPIO_IN ? BIT0 : 0)
    #define PAD_PM_GPIO30_OUT (PAD_PM_GPIO30_IS_GPIO == GPIO_OUT_HIGH ? BIT1  : 0)
    _RVM1(0x0f4c, PAD_PM_GPIO30_OUT, BIT1  ),
    _RVM1(0x0f4c, PAD_PM_GPIO30_OEN, BIT0 ),
    #endif

    #if(PAD_PM_IRIN_IS_GPIO != GPIO_NONE)
    #define PAD_PM_IRIN_OEN (PAD_PM_IRIN_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_IRIN_OUT (PAD_PM_IRIN_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f28, PAD_PM_IRIN_OUT, BIT1),
    _RVM1(0x0f28, PAD_PM_IRIN_OEN, BIT0),
    #endif

    #if(PAD_PM_PWM0_IS_GPIO != GPIO_NONE)
    #define PAD_PM_PWM0_OEN (PAD_PM_PWM0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_PWM0_OUT (PAD_PM_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2c, PAD_PM_PWM0_OUT, BIT1),
    _RVM1(0x0f2c, PAD_PM_PWM0_OEN, BIT0),
    #endif

    #if(PAD_PM_PWM1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_PWM1_OEN (PAD_PM_PWM1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_PWM1_OUT (PAD_PM_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0f2e, PAD_PM_PWM1_OUT, BIT1),
    _RVM1(0x0f2e, PAD_PM_PWM1_OEN, BIT0),
    #endif

    #if(PAD_PM_SPI_CZ_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ_OEN (PAD_PM_SPI_CZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CZ_OUT (PAD_PM_SPI_CZ_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e6a, BIT1, BIT1),  // Configuration
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OUT, BIT1),
    _RVM1(0x0f30, PAD_PM_SPI_CZ_OEN, BIT0),
    #endif

    #if(PAD_PM_SPI_CK_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CK_OEN (PAD_PM_SPI_CK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CK_OUT (PAD_PM_SPI_CK_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e6a, BIT0, BIT0),  // Configuration
    _RVM1(0x0f32, PAD_PM_SPI_CK_OUT, BIT1),
    _RVM1(0x0f32, PAD_PM_SPI_CK_OEN, BIT0),
    #endif

    #if(PAD_PM_SPI_DI_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DI_OEN (PAD_PM_SPI_DI_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DI_OUT (PAD_PM_SPI_DI_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e6a, BIT0, BIT0),  // Configuration
    _RVM1(0x0f34, PAD_PM_SPI_DI_OUT, BIT1),
    _RVM1(0x0f34, PAD_PM_SPI_DI_OEN, BIT0),
    #endif

    #if(PAD_PM_SPI_DO_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_DO_OEN (PAD_PM_SPI_DO_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_DO_OUT (PAD_PM_SPI_DO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e6a, BIT0, BIT0),  // Configuration
    _RVM1(0x0f36, PAD_PM_SPI_DO_OUT, BIT1),
    _RVM1(0x0f36, PAD_PM_SPI_DO_OEN, BIT0),
    #endif

    #if(PAD_PM_SPI_WPB_M_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_WPB_M_OEN (PAD_PM_SPI_WPB_M_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_WPB_M_OUT (PAD_PM_SPI_WPB_M_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0ee4, PAD_PM_SPI_WPB_M_OUT, BIT1),
    _RVM1(0x0ee4, PAD_PM_SPI_WPB_M_OEN, BIT0),
    #endif

    _MEMMAP_nonPM_,
    
    #if(PAD_PM_UART_RX_IS_GPIO != GPIO_NONE)
    #define PAD_PM_UART_RX_OEN (PAD_PM_UART_RX_IS_GPIO == GPIO_IN ? 0 : BIT0)   // (0:input / 1:output): 0)
    #define PAD_PM_UART_RX_OUT (PAD_PM_UART_RX_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1047, PAD_PM_UART_RX_OUT, BIT0),
    _RVM1(0x105c, PAD_PM_UART_RX_OEN, BIT0),
    #endif

    #if(PAD_SAR_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO0_OEN (PAD_SAR_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR_GPIO0_OUT (PAD_SAR_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR_GPIO0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR_GPIO0_OEN, BIT0),
    #endif

    #if(PAD_SAR_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO1_OEN (PAD_SAR_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR_GPIO1_OUT (PAD_SAR_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR_GPIO1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR_GPIO1_OEN, BIT1),
    #endif

    #if(PAD_SAR_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO2_OEN (PAD_SAR_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR_GPIO2_OUT (PAD_SAR_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR_GPIO2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR_GPIO2_OEN, BIT2),
    #endif

    #if(PAD_SAR_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR_GPIO3_OEN (PAD_SAR_GPIO3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_SAR_GPIO3_OUT (PAD_SAR_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_SAR_GPIO3_OUT, BIT3),
    _RVM1(0x1423, PAD_SAR_GPIO3_OEN, BIT3),
    #endif


//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------
    _MEMMAP_PM_,

    #define IRIN_USE_PM5 ((PADS_PM5_IS_IRIN==1)? BIT0 : 0) //reg_qfp156_mode

    _RVM1(0x0E50, IRIN_USE_PM5, BIT0),

    _MEMMAP_nonPM_,

    #define SM0_OPEN ((PADS_SM0_OPEN == PAD_SM0_CLK) ? (BIT0) : \
                      (PADS_SM0_OPEN == PAD_SM0_RST ) ? (BIT0) : 0)

    _RVM1(0x1E18, SM0_OPEN, BIT0), 

    #define SM0_C48 ((PADS_SM0_C48 == PAD_SM0_C4) ? (BIT4) : \
                     (PADS_SM0_C48 == PAD_SM0_C8 ) ? (BIT5) : 0)

    _RVM1(0x1E18, SM0_C48, BIT4),

    #define SM0_EN  ((PADS_SM0_EN == PAD_SM0_CLK) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_RST) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_CD) ? (BIT0) : \
                     (PADS_SM0_EN == PAD_SM0_VCC) ? (BIT0) : 0)

    _RVM1(0x1E19, SM0_EN, BIT0),



    #define SM0_IO  ((PADS_SM0_IO == PAD_SM0_IO) ? (BIT4) : 0)

    _RVM1(0x1E19, SM0_IO, BIT4),



    #define I2CM0_MODE ((PADS_I2CM0_MODE == PAD_I2CM0_SDA) ? (BIT0) : \
                      (PADS_I2CM0_MODE == PAD_I2CM0_SCL) ? (BIT0) : \
                      (PADS_I2CM0_MODE == PAD_UART_RX) ? (BIT1) : \
                      (PADS_I2CM0_MODE == PAD_UART_TX) ? (BIT1) : \
                      (PADS_I2CM0_MODE == PAD_FUART_CTS) ? (BIT1|BIT0) : \
                      (PADS_I2CM0_MODE == PAD_FUART_RTS) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E12, I2CM0_MODE, BIT1|BIT0),

    #define TS0_MODE ((PADS_TS0_MODE == PAD_TS0_CLK) ? (BIT0) : \
                      (PADS_TS0_MODE == PAD_TS0_D7) ? (BIT1) : \
                      (PADS_TS0_MODE == PAD_TS0_D0) ? (BIT1|BIT0) : \
                      (PADS_TS0_MODE == PAD_TS0_D3) ? (BIT2) : 0)

    _RVM1(0x1E04, TS0_MODE, BITMASK(2:0)),

    #define TS1_MODE ((PADS_TS1_MODE == PAD_TS1_CLK) ? (BIT4) : \
                      (PADS_TS1_MODE == PAD_TS1_D7) ? (BIT5) : \
                      (PADS_TS1_MODE == PAD_TS1_D0) ? (BIT5|BIT4) : \
                      (PADS_TS1_MODE == PAD_TS1_D6) ? (BIT6) : 0)

    _RVM1(0x1E04, TS1_MODE, BITMASK(6:4)),


    #define CI_MODE ((PADS_CI_MODE == PAD_CI_A14) ? (BIT0) : 0)

    _RVM1(0x1E0E, CI_MODE, BIT0),

    #define SPDIF_OUT_MODE ((PADS_SPDIF_OUT_MODE == PAD_SPDIF_OUT) ? (BIT4) : 0)

    _RVM1(0x1E0A, SPDIF_OUT_MODE, BITMASK(6:4)),

    #define SPDIF_IN_MODE ((PADS_SPDIF_IN_MODE == PAD_I2S_OUT_MUTE) ? (BIT0) : 0)

    _RVM1(0x1E0A, SPDIF_IN_MODE, BITMASK(2:0)),

    #define I2S_IN_MODE ((PADS_I2S_IN_MODE == PAD_FUART_CTS) ? BIT0 : \
                         (PADS_I2S_IN_MODE == PAD_FUART_RTS) ? BIT0 : \
                         (PADS_I2S_IN_MODE == PAD_FUART_TX) ? BIT0 : 0)


    _RVM1(0x1E0B, I2S_IN_MODE, BITMASK(1:0)),



    // UART MUX
    #define SECOND_UART_MODE ((PADS_SECOND_UART_MODE == PAD_UART_RX) ? (BIT0) : \
                              (PADS_SECOND_UART_MODE == PAD_UART_TX) ? (BIT0) : \
	                          (PADS_SECOND_UART_MODE == PAD_FUART_RX) ? (BIT1) : \
	                          (PADS_SECOND_UART_MODE == PAD_FUART_TX) ? (BIT1) : \
	                          (PADS_SECOND_UART_MODE == PAD_I2CM1_SCL) ? (BIT0|BIT1) : \
	                          (PADS_SECOND_UART_MODE == PAD_I2CM1_SDA) ? (BIT0|BIT1) : 0)

    _RVM1(0x1E06, SECOND_UART_MODE, BITMASK(2:0)),

    #define THIRD_UART_MODE ((PADS_THIRD_UART_MODE == PAD_UART_RX) ? (BIT4) : \
                             (PADS_THIRD_UART_MODE == PAD_UART_TX) ? (BIT4) : \
                             (PADS_THIRD_UART_MODE == PAD_FUART_RX) ? (BIT5) : \
                             (PADS_THIRD_UART_MODE == PAD_FUART_TX) ? (BIT5) : \
                             (PADS_THIRD_UART_MODE == PAD_I2CM1_SCL) ? (BIT4|BIT5) : \
                             (PADS_THIRD_UART_MODE == PAD_I2CM1_SDA) ? (BIT4|BIT5) :0)


    _RVM1(0x1E06, THIRD_UART_MODE, BITMASK(5:4)),

    #define FOURTH_UART_MODE ((PADS_FOURTH_UART_MODE == PAD_UART_RX) ? (BIT0) : \
                           (PADS_FOURTH_UART_MODE == PAD_UART_TX) ? (BIT0) : \
                           (PADS_FOURTH_UART_MODE == PAD_FUART_RX) ? (BIT1) : \
                           (PADS_FOURTH_UART_MODE == PAD_FUART_TX) ? (BIT1) : \
                           (PADS_FOURTH_UART_MODE == PAD_I2CM1_SCL) ? (BIT1|BIT0) : \
                           (PADS_FOURTH_UART_MODE == PAD_I2CM1_SDA) ? (BIT1|BIT0) : 0)

    _RVM1(0x1E07, FOURTH_UART_MODE, BITMASK(1:0)),
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


    _MEMMAP_nonPM_,

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
    (UART1_SRC_SEL == UART2_SRC_SEL)
    #error "UART src selection conflict"
#endif



