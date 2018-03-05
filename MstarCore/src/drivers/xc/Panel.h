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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _PANEL_H_
#define _PANEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "apiDAC.h"
#include "apiHDMITx.h"

#include "apiPNL.h"


#define INTERFACE extern

#define ENABLE_OD      0
#define ENABLE_MFC     0
////////////////////////////////////////////////////
// Include the real header file
////////////////////////////////////////////////////


////////////////////////////////////////////////////
//      Type define
////////////////////////////////////////////////////

typedef enum{
    PANEL_RES_MIN = 0,

    PANEL_SEC32_LE32A_FULLHD = PANEL_RES_MIN,
    // For Normal LVDS panel
    PANEL_RES_SXGA          = 1,            // 1280x1024, Pnl_AU17_EN05_SXGA
    PANEL_RES_WXGA          = 2,            // 1366x768, Pnl_AU20_T200XW02_WXGA,
    PANEL_RES_WXGA_PLUS     = 3,            // 1440x900, Pnl_CMO19_M190A1_WXGA, Pnl_AU19PW01_WXGA
    PANEL_RES_WSXGA         = 4,            // 1680x1050, Pnl_AU20_M201EW01_WSXGA,
    PANEL_RES_FULL_HD       = 5,            // 1920x1080, Pnl_AU37_T370HW01_HD, Pnl_CMO216H1_L01_HD.h

    // For DAC/HDMI Tx output
    PANEL_DACOUT_480I       = 6,            // just for U3 Dac output 480i timing usage
    PANEL_DACOUT_480P       = 7,            // just for U3 Dac output 480p timing usage
    PANEL_DACOUT_576I       = 8,            // just for U3 Dac output 576i timing usage
    PANEL_DACOUT_576P       = 9,            // just for U3 Dac output 576p timing usage
    PANEL_DACOUT_720P_50    = 10,           // just for U3 Dac output 720p timing usage
    PANEL_DACOUT_720P_60    = 11,           // just for U3 Dac output 720p timing usage
    PANEL_DACOUT_1080I_50   = 12,           // just for U3 Dac output 1080i timing usage
    PANEL_DACOUT_1080I_60   = 13,           // just for U3 Dac output 1080i timing usage
    PANEL_DACOUT_1080P_24   = 14,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_25   = 15,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_30   = 16,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_50   = 17,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_1080P_60   = 18,           // just for U3 Dac output 1080p timing usage
    PANEL_DACOUT_640X480P   = 19,

    Panel_CMO_CMO260J2_WUXGA,           // 1920*1200
#if ENABLE_MFC
    PANEL_RES_URSA,
    PANEL_URSA_RES_FULL_HD_120Hz = PANEL_RES_URSA,
    PANEL_URSA_RES_FULL_HD_60Hz,
#endif
    PANEL_RES_MAX_NUM,
}PANEL_RESOLUTION_TYPE;

////////////////////////////////////////////////////
//      global variable
////////////////////////////////////////////////////
#define PANEL_POWER_VCC         0x01
#define PANEL_POWER_LIGHT_ON    0x02
#define PANEL_POWER_BLUESCREEN  0x04

#define PANEL_DEFAULT_TYPE_SEL  PANEL_DACOUT_720P_50
//#define PANEL_DEFAULT_TYPE_SEL  PANEL_DACOUT_480I
INTERFACE PANEL_RESOLUTION_TYPE g_PNL_TypeSel ;
////////////////////////////////////////////////////
//      Functions
////////////////////////////////////////////////////

/*
*   below are customized functions,
*
*
*
*/


/*
*   Transfer panel select value into a PanelType structure,
*   which informed XC_PNL_OBJ which panel used.
*
*   used it with MApi_PNL_Init to initialzed a panel.
*/
INTERFACE PanelType* MApi_XC_GetPanelSpec(PANEL_RESOLUTION_TYPE enResolutionType);

extern E_OUTPUT_VIDEO_TIMING_TYPE MApi_XC_GetDACType(PANEL_RESOLUTION_TYPE enResolutionType);
extern HDMITX_VIDEO_TIMING MApi_XC_GetHDMIType(PANEL_RESOLUTION_TYPE enResolutionType);
//extern MS_COMPONENT_OUTPUT_TYPE MApi_XC_GetComponentType(PANEL_RESOLUTION_TYPE enResolutionType)

////////////////////////////////////////////////////
//      need to be refined
////////////////////////////////////////////////////

#if  ( ENABLE_OD )
    #include "MsOD_LIB.h"
#endif

#ifndef PANEL_SYNC_MODE_1
#define PANEL_SYNC_MODE_1            0
#endif

#ifndef PANEL_ANALOG_TCON
#define PANEL_ANALOG_TCON           0
#endif

#ifndef ENABLE_DPWM_FUNCTION
#define ENABLE_DPWM_FUNCTION        0
#endif

#ifndef DPWM_BURST_MODE
#define DPWM_BURST_MODE             0
#endif

#ifndef DISABLE_SHORT_FRAME_TUNE
#define DISABLE_SHORT_FRAME_TUNE    1
#endif

#ifndef DISABLE_SHORT_LINE_TUNE
#define DISABLE_SHORT_LINE_TUNE     0
#endif

#ifndef SET_SSC_SPAN
#define SET_SSC_SPAN                0x00
#endif

#ifndef SET_SSC_STEP
#define SET_SSC_STEP                0x00
#endif

#ifndef PANEL_DOT_HEIGHT
#define PANEL_DOT_HEIGHT            100
#endif

#ifndef PANEL_DOT_WIDTH
#define PANEL_DOT_WIDTH            PANEL_DOT_HEIGHT
#endif

#ifndef CRT_INTERLACE_DOUBLOUT
#define CRT_INTERLACE_DOUBLOUT  0
#endif

#define WIDE_PANEL  0
#define _4_3_PANEL  1
#define OTHER_PANEL 2
#define _5_4_PANEL  3
#define _16_9_PANEL 4

////////////////////////////////////////////////////
// Panel common constant
////////////////////////////////////////////////////
#define PANEL_PWM_BRIGHT        1

////////////////////////////////////////////////////
// Panel power sequence
////////////////////////////////////////////////////
//Truman temp
//#define pnlSetFPVCCOn()     SetPinFpVcc()
//#define pnlSetFPVCCOff()    ClrPinFpVcc()

////////////////////////////////////////////////////
// Ouput Clock PLL
////////////////////////////////////////////////////

// Output clock = XTAL * MM * ((524288 * LM * K) / (SET * A))
//  where XTAL = external XTAL frequency
//        MM = PLL divider MM, SREG D2[4:0]
//        LM = PLL divider LM, SREG D3[4:0]
//        K = PLL divider K, SREG D4[4:3]
//        SET = PLL divider SET, SREG D5, D6, D7
//        A = PLL divider A, LVDS single channel = 7
//                           LVDS dual channel = 3.5
//                           RSDS/TTL = 4

//
//  TCON setting, not every panel have it,
//  so we add redundant define here to let compile ok
//
#ifndef SET_PTC_MODE1
#define SET_PTC_MODE1           0
#endif
#ifndef SET_PTC_MODE2
#define SET_PTC_MODE2           0
#endif
#ifndef SET_PTC_MODE3
#define SET_PTC_MODE3           0
#endif
#ifndef SET_FRP_TRAN
#define SET_FRP_TRAN            0
#endif
#ifndef SET_STH_START
#define SET_STH_START           0
#endif
#ifndef SET_STH_WIDTH
#define SET_STH_WIDTH           0
#endif
#ifndef SET_OEH_START
#define SET_OEH_START           0
#endif
#ifndef SET_OEH_WIDTH
#define SET_OEH_WIDTH           0
#endif
#ifndef SET_OEV_START
#define SET_OEV_START           0
#endif
#ifndef SET_OEV_WIDTH
#define SET_OEV_WIDTH           0
#endif
#ifndef SET_CKV_START
#define SET_CKV_START           0
#endif
#ifndef SET_CKV_START2
#define SET_CKV_START2          0
#endif
#ifndef SET_CKV_WIDTH
#define SET_CKV_WIDTH           0
#endif
#ifndef SET_STV_LINE_TH
#define SET_STV_LINE_TH         0
#endif
#ifndef SET_STV_START
#define SET_STV_START           0
#endif
#ifndef SET_STV_WIDTH
#define SET_STV_WIDTH           0
#endif
#ifndef SET_OEV2_START
#define SET_OEV2_START          0
#endif
#ifndef SET_OEV3_START
#define SET_OEV3_START          0
#endif
#ifndef SET_H_ST_DLY_L
#define SET_H_ST_DLY_L          0
#endif
#ifndef SET_H_ST_DLY_H
#define SET_H_ST_DLY_H          0
#endif
#ifndef SET_CLK_DLY_SYNC_OUT
#define SET_CLK_DLY_SYNC_OUT    0
#endif
#ifndef SET_CKV_END2
#define SET_CKV_END2            0
#endif
#ifndef SET_Q1H
#define SET_Q1H                 0
#endif
#ifndef SET_OEV2_WIDTH
#define SET_OEV2_WIDTH          0
#endif
#ifndef SET_OEV3_WIDTH
#define SET_OEV3_WIDTH          0
#endif
#ifndef SET_OEV_DELTA
#define SET_OEV_DELTA           0
#endif
// Every panel must have his own PanelFineTune()
// The function will be called after mode change successfully
//extern void PanelFineTune();

#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif


