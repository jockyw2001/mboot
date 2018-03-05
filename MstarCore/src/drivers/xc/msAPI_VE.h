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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   msAPI_XC.h
/// @brief  VE High Layer API Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//****************************************************
// msAPI_VE SW Version: 1.12
// msAPI_VE update date: 2011/05/05 16:10
//****************************************************

#ifndef _MSAPI_VE_H_
#define _MSAPI_VE_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSAPI_VE_H_OFFSET     (9+6)
#define MSAPI_VE_V_OFFSET     (0+0)

//-------------------------------------------------------------------------------------------------
//  Enum Types
//-------------------------------------------------------------------------------------------------
typedef enum
{
    EN_WSS_4x3_FULL                    = 0x08,
    EN_WSS_14x9_LETTERBOX_CENTER       = 0x01,
    EN_WSS_14x9_LETTERBOX_TOP          = 0x02,
    EN_WSS_16x9_LETTERBOX_CENTER       = 0x0B,
    EN_WSS_16x9_LETTERBOX_TOP          = 0x04,
    EN_WSS_ABOVE16x9_LETTERBOX_CENTER  = 0x0D,
    EN_WSS_14x9_FULL_CENTER            = 0x0E,
    EN_WSS_16x9_ANAMORPHIC             = 0x07,
}EN_VE_WSS_TYPE;

/// Define MPEG video AFD (Active Format Description) information
typedef enum
{
    /// Unknow AFD.
    E_MVD_AFD_UNKNOWN           = 0,
    /// LetterBox 16:9, top posistion.
    E_MVD_AFD_BOX_16X9_TOP      = 2,
    /// LetterBox 14:9, top posistion.
    E_MVD_AFD_BOX_14X9_TOP      = 3,
    /// LetterBox 16:9, cnetre posistion.
    E_MVD_AFD_BOX_16X9          = 4,
    /// Full frame.
    E_MVD_AFD_FULL_FRAME        = 8,
    /// Centre 4:3
    E_MVD_AFD_CENTRE_4x3        = 9,
    /// Centre 16:9
    E_MVD_AFD_CENTRE_16X9       = 10,
    /// Centre 14:9
    E_MVD_AFD_CENTRE_14X9       = 11,
    /// 4:3 (with shoot and protect 14:9 centre).
    E_MVD_AFD_CENTRE_4X3_14X9   = 13,
    /// 16:9 (with shoot and protect 14:9 centre).
    E_MVD_AFD_CENTRE_16X9_14X9  = 14,
    /// 16:9 (with shoot and protect 4:3 centre).
    E_MVD_AFD_CENTRE_16X9_4X3   = 15
} MVD_AFD, *pMVD_AFD;

typedef enum
{
    MVD_FORBIDDEN=0,
    MVD_ASP_1TO1,       //    1 : 1
    MVD_ASP_4TO3,       //    4 : 3
    MVD_ASP_16TO9,      //   16 : 9
    MVD_ASP_221TO100,   // 2.21 : 1
    MVD_ASP_MAXNUM,
} MVD_ASPECT_RATIO_CODE;

typedef enum
{
    E_MSAPI_VE_ASPECT_RATIO_NORMAL           = 0,
    E_MSAPI_VE_ASPECT_RATIO_4_3              = 1,
    E_MSAPI_VE_ASPECT_RATIO_16_9             = 2,
    E_MSAPI_VE_ASPECT_RATIO_CUT_4_3          = 3,
    E_MSAPI_VE_ASPECT_RATIO_CUT_16_9         = 4,
    E_MSAPI_VE_ASPECT_RATIO_FULL             = 5,
}E_MSAPI_VE_ASPECT_RATIO_TYPE;

typedef enum
{
    E_MSAPI_VE_SOURCE_AUTO = 0,
    E_MSAPI_VE_SOURCE_SCALER_OP2,
    E_MSAPI_VE_SOURCE_SCALER_DI2VE,
    E_MSAPI_VE_SOURCE_EXT_BT656,
    E_MSAPI_VE_SOURCE_SCALER_DNR2VE,
    E_MSAPI_VE_SOURCE_MAXNUM,
}E_MSAPI_VE_SOURCE_TYPE;

typedef enum
{
    E_MSAPI_VE_DBG_LEVEL_NONE = 0,
    E_MSAPI_VE_DBG_LEVEL_API,
    E_MSAPI_VE_DBG_LEVEL_DRV,
}E_MSAPI_VE_DBG_LEVEL_TYPE;

typedef enum
{
    /// 3D input type is None
    E_MSAPI_VE_3D_INPUT_MODE_NONE,
    /// 3D input type is Frame Packing
    E_MSAPI_VE_3D_INPUT_FRAME_PACKING,
    /// 3D input type is Frame Alternative
    E_MSAPI_VE_3D_INPUT_FRAME_ALTERNATIVE,
    /// 3D input type is Field Alternative
    E_MSAPI_VE_3D_INPUT_FIELD_ALTERNATIVE,
    /// 3D input type is Line Alternativ
    E_MSAPI_VE_3D_INPUT_LINE_ALTERNATIVE,
    /// 3D input type is Side by Side Half
    E_MSAPI_VE_3D_INPUT_SIDE_BY_SIDE_HALF,
    /// 3D input type is Side by Side Full
    E_MSAPI_VE_3D_INPUT_SIDE_BY_SIDE_FULL,
    /// 3D input type is Top and Bottom
    E_MSAPI_VE_3D_INPUT_TOP_BOTTOM,
    /// 3D input type is Check board
    E_MSAPI_VE_3D_INPUT_CHECK_BORAD,
    /// 3D input type is Pixel Alternative
    E_MSAPI_VE_3D_INPUT_PIXEL_ALTERNATIVE,
} E_MSAPI_VE_3D_INPUT_MODE;

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
/// Define the initial data for XC_Sys init
typedef struct
{
    MS_U32 u32VEMemAddress;                                 ///<msAPI VE Memory Address
    MS_U32 u32VEMemSize;                                    ///<msAPI VE Memory Size
    MS_VE_VIDEOSYS eOutputVideoStd;                         ///<msAPI VE Output Standard
    MS_VE_INPUT_SRC_TYPE eInputSrcType;                     ///<msAPI VE Input Source Type
    MS_VE_OUTPUT_DEST_TYPE eOutputDstType;                  ///<msAPI VE Output Destination Type
    MS_BOOL bBootLogoEnable;                                ///<Flag to control if Boot Logo is Enable and to Bypass some init flow
    MS_BOOL bEnableDacSD;                                   ///<Flag to turn on SD DAC after VE initialization
} MSAPI_VE_INITDATA;

typedef struct
{
    //SD
    MS_VE_WINDOW_TYPE SD_SrcWin;
    MS_VE_WINDOW_TYPE SD_DestWin;
    E_MSAPI_VE_ASPECT_RATIO_TYPE SD_AS;
}MSAPI_VE_DISP_INFO;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//init
extern MS_BOOL msAPI_VE_Init(MSAPI_VE_INITDATA stAPI_VE_InitData);

//---------------------------------------------------------------------------------------------
/// @brief: //Enable or Disable SD Output CVBS/S-Video
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_EnableMiscOutput(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: Enable/Disable Bluescreen of SD Output
//---------------------------------------------------------------------------------------------
extern void msAPI_VE_SetVideoMute(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: VE setmode for (1)video path (2)timing change (3)output mode change
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetMode(void);

//---------------------------------------------------------------------------------------------
/// @brief: VE Set Output Standard
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetOutputVideoStd(MS_VE_VIDEOSYS eVideoSystem);

//---------------------------------------------------------------------------------------------
/// @brief: Check if msAPI_VE_Init have been invoked
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_CheckInitStatus(void);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE enable/disable VE Capture Mode
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_EnableCaptureMode(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: VE Set WSS Info
//---------------------------------------------------------------------------------------------
extern void msAPI_VE_SetAFDToVEWSS(MS_BOOL bEn, MVD_AFD u8AFD, MVD_ASPECT_RATIO_CODE u8SrcARC);

//---------------------------------------------------------------------------------------------
/// @brief: //Set VE capture/display window
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetWin(MS_VE_WINDOW_TYPE *pstSrcWin, MS_VE_WINDOW_TYPE *pstDstWin);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE set Debug Message Level
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetDebugLevel(E_MSAPI_VE_DBG_LEVEL_TYPE eDBGLevel);

//---------------------------------------------------------------------------------------------
/// @brief: exit VE
//---------------------------------------------------------------------------------------------
extern void msAPI_VE_Exit(void);

//---------------------------------------------------------------------------------------------
/// @brief: set SD DAC Output Control
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetDACOutCTL(E_DAC_SD_ENABLE_CTRL eDacCtrl);

//---------------------------------------------------------------------------------------------
/// @brief: set VE Source Type
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_TYPE eSrcType);

//---------------------------------------------------------------------------------------------
/// @brief: set information of input CCIR
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetSourceCCIR656Info(MS_BOOL bIsInterlace, MS_U16 u16FrameRate);

//---------------------------------------------------------------------------------------------
/// @brief: //Set VE Output Control to TVEncoder or CCIR656
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_SetOutputCtrl(MS_VE_OUT_TYPE eOutputType);

//---------------------------------------------------------------------------------------------
/// @brief: set VE Source Type
//---------------------------------------------------------------------------------------------
E_MSAPI_VE_SOURCE_TYPE msAPI_VE_GetSourceType(void);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE enable/disable VE Frame Lock
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_EnableFrameLock(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE enable/disable VE Force Set Mode
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_ForceSetMode(MS_BOOL bEnable);

//---------------------------------------------------------------------------------------------
/// @brief: msAPI VE Get XC 3D Mode
//---------------------------------------------------------------------------------------------
extern MS_BOOL msAPI_VE_Get3DMode(E_MSAPI_VE_3D_INPUT_MODE en3DInMode);

#ifdef __cplusplus
}
#endif

#endif
