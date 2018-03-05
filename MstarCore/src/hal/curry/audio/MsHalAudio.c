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
#include <command.h>
#include <config.h>
#include <MsTypes.h>
#include <MsOS.h>
#include <MsDebug.h>
#include <MsSystem.h>

#include <apiAUDIO.h>
#include <drvAUDIO_if.h>
#include <drvAUDIO.h>

#define AUDIO_DELAY1MS(x)    mdelay(x)

//=============================================
// Set audio related input & output port connectivity
//=============================================
void MsHalAudio_SetInternalPath(void)
{
        MDrv_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT, AUDIO_PATH_MAIN);      //Set input source to decoder 1

        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_I2S_OUTPUT);       // TV:2R1D use internal path D
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT0_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT1_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT2_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT3_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_SPDIF_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_HDMI_OUTPUT);       
}

//=============================================
// Set volume for all output port
//=============================================
void MsHalAudio_SetOutputVolume(MS_U16 u16Vol)
{
    MS_U8 u8VolHi = 0 , u8VolLo = 0 ;

    u8VolHi = (MS_U8)(u16Vol>>8);    
    u8VolLo = (MS_U8)(u16Vol&0xFF);

    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_I2S, u8VolHi, u8VolLo );
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT0, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT1, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT2, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT3, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_SPDIF, u8VolHi, u8VolLo );   //Set volume//AUDIO_T3_PATH_I2S    
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_HDMI, u8VolHi, u8VolLo );      //Set  HDMI volume
}

//=============================================
// Play Melody in this function 
//=============================================
void MsHalAudio_MelodyPlayStart(void)
{
  //======================================================
  // New SRC arch chip , play Music with MM new mode
  //======================================================
    MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MP3);   // Set Decoder type = MP3
    MApi_AUDIO_MM2_initAesInfo(AU_DEC_ID1);
    AUDIO_DELAY1MS(5);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
    AUDIO_DELAY1MS(5);
    MApi_AUDIO_MM2_inputAesFinished(AU_DEC_ID1, 0x10000-0x20, 0, 0);
}

//=============================================
// Mute/unmute all audio output port
//=============================================
void MsHalAudio_SetOutputMute(MS_BOOL bMute)
{
    if(bMute == 1)
    {
        MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT0, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT1, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT3, 1);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_I2S, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 1);  
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, 1);
    }  
    else
    {
        MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT0, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT1, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2, 0);                    
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT3, 0);                     
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_I2S, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 0);  
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_HDMI, 0 );
    }
}

//=============================================
// Get ES buffer abs start address
//=============================================
MS_U32 MsHalAudio_GetEsStartAddr(void)
{
    MS_U32 u32EsAddress = 0;

    u32EsAddress = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_UNI_ES_Base);

    return u32EsAddress;
}


