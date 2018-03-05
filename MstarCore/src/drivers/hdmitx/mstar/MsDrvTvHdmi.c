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

#include <common.h>
#include <MsCommon.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <panel/MsDrvPanel.h>
#include <apiHDMITx.h>
#include <MsEnvironment.h>
#include <malloc.h>


typedef enum
{
    E_HDMITX_VIDEO_SI_NoData       = 0,
    E_HDMITX_VIDEO_SI_Overscanned  = 1,
    E_HDMITX_VIDEO_SI_Underscanned = 2,
    E_HDMITX_VIDEO_SI_Reserved     = 3,
} MsHDMITX_VIDEO_SCAN_INFO;

typedef enum
{
    E_HDMITX_VIDEO_AFD_SameAsPictureAR    = 8, // 1000
    E_HDMITX_VIDEO_AFD_4_3_Center         = 9, // 1001
    E_HDMITX_VIDEO_AFD_16_9_Center        = 10, // 1010
    E_HDMITX_VIDEO_AFD_14_9_Center        = 11, // 1011
    E_HDMITx_VIDEO_AFD_Others = 15, // 0000~ 0111, 1100 ~ 1111
} MsHDMITX_VIDEO_AFD_RATIO;


static HDMITX_VIDEO_TIMING getHDMITxVideoTiming()
{
    HDMITX_VIDEO_TIMING mode =HDMITX_RES_640x480p;
    int u8ResolutionEnv = -1;
    char * p_str = NULL;

    p_str = getenv ("resolution");
    if(NULL != p_str)
    {
        u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
    }
     else
    {
        //set env normal resolution. HDMITX_RES_1920x1080p_60Hz  = 13,
        setenv("resolution", "13");
        saveenv();
    }

    if((HDMITX_RES_MAX <= u8ResolutionEnv) || (0 > u8ResolutionEnv))
    {
        printf("error, resolution env is out of [1--%d]!\n",HDMITX_RES_MAX);
        return FALSE;
    }

    return u8ResolutionEnv;
}
int MsDrvTvHdmiTxInit(void)
{
    MS_BOOL _bModeNTSC = FALSE;
    MS_BOOL bRet = FALSE;
    HDMITX_VIDEO_TIMING mode =HDMITX_RES_640x480p;

    UBOOT_TRACE("IN\n");


    MApi_HDMITx_SetDbgLevel(0);
    MApi_HDMITx_SetHPDGpioPin(HDMITX_HPD_PM_GPIO);
    MApi_HDMITx_Init();
    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);

    HDMITX_VIDEO_COLOR_FORMAT eInColorFmt  = HDMITX_VIDEO_COLOR_RGB444;
    HDMITX_OUTPUT_MODE    eOutMode = HDMITX_HDMI;
    HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth = HDMITX_VIDEO_CD_NoID;
    MApi_HDMITx_EdidChecking();
    HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;

    MApi_HDMITx_SetHDMITxMode_CD(eOutMode, eEDIDColorDepth);
    MApi_HDMITx_SetHDMITxMode(eOutMode);
    MApi_HDMITx_SetColorFormat(eInColorFmt, eOutColorFmt);
#if(ENABLE_POWER_MUSIC==1)
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif

    //6. Post HDMI setting
    MApi_HDMITx_SetVideoOutputTiming(getHDMITxVideoTiming());
    //setHDMITxAnalogTuning(enPanelType);
    // MApi_HDMITx_Exhibit();
    if(getHDMITxVideoTiming() <= HDMITX_RES_720x576p)
    {
        //MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center);
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center, 0x1);
    }
    else
    {
        //MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center);
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center, 0x1);
    }
    //back ground active need disable output (video and audio)
    if(FALSE == pm_check_back_ground_active())
    {
        MApi_HDMITx_SetTMDSOnOff(TRUE);
        MsOS_DelayTask(20);
        MApi_HDMITx_SetAVMUTE(FALSE);
    }
    else
    {
        MApi_HDMITx_SetTMDSOnOff(FALSE);
    }
    UBOOT_TRACE("ON\n");
    return 0;
}
