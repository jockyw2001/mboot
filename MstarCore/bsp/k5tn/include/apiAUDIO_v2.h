//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: apiAUDIO_v2.h
//  Description: Audio api header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AUDIO_H_
#define _AUDIO_H_

//#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum {
    //  AUDIO_SYSTEM RELATIONAL API FUNCTION                    //*** Args of API  ***
    MApi_CMD_AUDIO_Initialize = 0,                              // void                                                              #0
    MApi_CMD_AUDIO_SetPowerOn,                                  // pointer to (U32_bEnable)
    MApi_CMD_AUDIO_SetDSPBaseAddr,                              // pointer to (MS_U32)
    MApi_CMD_AUDIO_GetDSPBaseAddr,                              // pointer to (MS_U32)
    MApi_CMD_AUDIO_WriteDecMailBox,                             // pointer to (MS_U32)
    MApi_CMD_AUDIO_TriggerSIFPLL,                               // void
    MApi_CMD_AUDIO_Monitor,                                     // void
    MApi_CMD_AUDIO_SetSystemInfo,                               // pointer to (AUDIO_INIT_INFO*)
    MApi_CMD_AUDIO_ASF_Data_To_DDR,                             // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_R2_DEBUG_MSG,                                // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_R2_DDR_LETENCY_MSG,                          // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_SetOutConnectivity,                          // void
    MApi_CMD_AUDIO_SetPathInfo,                                 // pointer to (AUDIO_PATH_INFO*)
    MApi_CMD_AUDIO_SetOutputInfo,                               // pointer to (AUDIO_OUT_INFO*)
    MApi_CMD_AUDIO_SetSourceInfo,                               // pointer to (AUDIO_SOURCE_INFO_TYPE)
    MApi_CMD_AUDIO_InputSwitch,                                 // pointer to (AUDIO_INPUT_TYPE, AUDIO_SWITCH_GROUP)
    MApi_CMD_AUDIO_Set_Input_Scale,                             // pointer to (int)
    MApi_CMD_AUDIO_Set_Scart_Scale,                             // pointer to (int)
    //MApi_CMD_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE,         // pointer to (int *)
    MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS,                         // pointer to (SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue)

    MApi_CMD_AUDIO_SetDataCaptureSource,                        // pointer to (AUDIO_DEVICE_TYPE, AUDIO_CAPTURE_SOURCE_TYPE)    ##20
    //MApi_CMD_AUDIO_SetOutputSourceInfo,                         // pointer to (OUTPUT_SOURCE_INFO *)                               
    //MApi_CMD_AUDIO_GetOutputSourceInfo,                         // pointer to (OUTPUT_SOURCE_INFO *)
    MApi_CMD_AUDIO_OutputSourceInfo,                            // pointer to (AUDIO_ST_OUTPUT_SOURCE)
    MApi_CMD_AUDIO_ReadMailBox,                                 // pointer to (MS_BOOL, MS_U8)
    MApi_CMD_AUDIO_WriteMailBox,                                // pointer to (MS_BOOL, MS_U8, MS_U16)
    MApi_CMD_AUDIO_FwTriggerDSP,                                // pointer to (MS_U16)
    MApi_CMD_AUDIO_SetFwStatus,                                 // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_ExitAudioSystem,                             // void
    MApi_CMD_AUDIO_RebootDSP,                                   // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_SendIntrupt,                                 // pointer to (MS_BOOL, MS_U8)
    MApi_CMD_AUDIO_I2S_SetMode,                                 // pointer to (AUDIO_I2S_MODE_TYPE, AUDIO_I2S_MODE_VALUE)
    MApi_CMD_AUDIO_WritePreInitTable,                           // void                                                             
    MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage,                  // void
    MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage,               // void

    // AUDIO_SOUND RELATIONAL API FUNCTION
    MApi_CMD_SND_ProcessEnable,                                 // pointer to (Sound_ENABLE_Type, MS_BOOL)                          
    MApi_CMD_SND_SetParam1,                                     // pointer to (Sound_SET_PARAM_Type, MS_U16, MS_U16)
    MApi_CMD_SND_GetParam1,                                     // pointer to (Sound_GET_PARAM_Type, MS_U16)
    MApi_CMD_AUDIO_Sound_SetParam,                              // pointer to (AUDIO_ST_SOUND_SETPARAM*)
    MApi_CMD_AUDIO_SetAbsoluteVolume,                           // pointer to (MS_U8, MS_U8, MS_U8) 
    //MApi_CMD_AUDIO_SetPreScale,                                 // pointer to (MS_U8, MS_U8)
    //MApi_CMD_AUDIO_SetMute,                                     // pointer to (MS_U8, MS_BOOL)
    MApi_CMD_AUDIO_SourceConnect,                               // pointer to (AUDIO_OUTPORT_SOURCE_TYPE, MS_BOOL)                  
    //MApi_CMD_AUDIO_SetBalance,                                  // pointer to (MS_U8)
    //MApi_CMD_AUDIO_EnableEQ,                                    // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableTone,                                  // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableAutoVolume,                            // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableSurround,                              // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_EnableBalance,                               // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_SetDynamicBass,                              // pointer to (MS_U8)
    //MApi_CMD_AUDIO_SetBass,                                     // pointer to (MS_U8)
    //MApi_CMD_AUDIO_AbsoluteBass,                                // pointer to (MS_U8)
    //MApi_CMD_AUDIO_SetTreble,                                   // pointer to (MS_U8)                                               
    MApi_CMD_AUDIO_AbsoluteTreble,                              // pointer to (MS_U8)                                                ##40
    //MApi_CMD_AUDIO_SetEq,                                       // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_SetEq_7band,                                 // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_SetAvcThreshold,                             // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetAvcMode,                                  // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetAvcAT,                                    // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetAvcRT,                                    // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetBufferProcess,                            // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSurroundXA,                               // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSurroundXB,                               // pointer to (MS_U8)                                               
    MApi_CMD_AUDIO_SetSurroundXK,                               // pointer to (MS_U8) 
    MApi_CMD_AUDIO_SetSurroundLPFGain,                           // pointer to (MS_U8) 
    MApi_CMD_AUDIO_ConvertVolumeUnit,                           // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetPEQCoef,                                  // pointer to (AUDIO_PEQ_COEF*)
    //MApi_CMD_AUDIO_EnablePEQ,                                   // pointer to (MS_U8)
    MApi_CMD_AUDIO_EnableDcRemove,                              // pointer to (MS_U8)                                               


    // AUDIO_DTV RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_DecodeSystem,                                // pointer to (AUDIO_ST_SET_DECODESYSTEM)
    MApi_CMD_AUDIO_ReleaseDecodeSystem,                         // pointer to (AUDIO_ST_RELEASE_DECODESYSTEM*)                                        
    MApi_CMD_AUDIO_SetDecodeSystem,                             // pointer to (AUDIO_ST_SET_DECODESYSTEM*)
    MApi_CMD_AUDIO_GetDecodeSystem,                             // pointer to (AUDIO_ST_GET_DECODESYSTEM*)
    MApi_CMD_AUDIO_SetDecodeCmd,                                // pointer to (AUDIO_DEC_ID, En_DVB_decCmdType)                     
    MApi_CMD_AUDIO_OpenDecodeSystem,                            // pointer to (AudioDecStatus_t*)
    MApi_CMD_AUDIO_SetSystem,                                   // pointer to (En_DVB_decSystemType)
    MApi_CMD_AUDIO_GetDecSysSupportStatus,                      // pointer to (En_DVB_decSystemType)                                ##60
    MApi_CMD_AUDIO_IsMadLock,                                   // void
    MApi_CMD_AUDIO_GetDecStatus,                                // void
    MApi_CMD_AUDIO_Decode_Command,                              // pointer to (AUDIO_ST_DECODER_COMMAND)
    //MApi_CMD_AUDIO_GetCommand,                                  // pointer to (En_DVB_AudioDecoder)
    //MApi_CMD_AUDIO_SetCommand,                                  // pointer to (En_DVB_decCmdType)
    MApi_CMD_AUDIO_GetMAD_Lock,                                 // void
    MApi_CMD_AUDIO_SETAD,                                       // pointer to (AUDIO_ST_SET_AD)
    //MApi_CMD_AUDIO_SetADOutputMode,                             // pointer to (MS_U8)                                               
    //MApi_CMD_AUDIO_SetADAbsoluteVolume,                         // pointer to (MS_U8)
    //MApi_CMD_AUDIO_ADSetMute,                                   // pointer to (MS_U8)

    // AUDIO_PVR RELATED FUNCTIONS
    MApi_CMD_AUDIO_SetEncInit,                                  // pointer to (AU_DVB_ENCBR, MS_U8)                                 
    MApi_CMD_AUDIO_SetEncCommand,                               // pointer to (AU_DVB_ENCCMD)
    MApi_CMD_AUDIO_GetEncodeFrameInfo,                          // pointer to (AU_DVB_ENC_FRAME_INFO*)
    MApi_CMD_AUDIO_GetEncodeDoneFlag,                           // void

    // AUDIO_ATV RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_SIF_SetStandard,                             // pointer to (AUDIOSTANDARD_TYPE)                                  
    MApi_CMD_AUDIO_SIF_GetSoundMode,                            // void
    MApi_CMD_AUDIO_SIF_SetSoundMode,                            // pointer to (MS_U8)
    MApi_CMD_AUDIO_SIF_GetAudioStatus,                          // pointer to (AUDIO_ST_SIF_GET_AUDIO_STATUS)                       
    MApi_CMD_AUDIO_SIF_StartAutoStandardDetection,              // void
    MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection,        // void
    MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard,             // pointer to (AUDIOSTANDARD_TYPE)
    MApi_CMD_AUDIO_SIF_SetThreshold,                            // pointer to (THR_TBL_TYPE code*)
    MApi_CMD_AUDIO_SIF_SetPrescale,                             // pointer to (MS_U8, int)
    MApi_CMD_AUDIO_SIF_IsPALType,                               // pointer to (AUDIO_SIF_PAL_TYPE)
    MApi_CMD_AUDIO_SIF_SetPALType,                              // pointer to (AUDIO_SIF_PAL_TYPE)                                  ##80
    MApi_CMD_AUDIO_SIF_SendCmd,                                 // pointer to (En_AUD_SIF_CmdType, MS_U8, MS_U8)
    MApi_CMD_AUDIO_SIF_Shift,                                   // pointer to (En_AUD_VIF_Type)                     

#if 1
    // AUDIO_FM_RADIO RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_FM_RADIO_GetSoundMode,                       // void                                                             #90
    MApi_CMD_AUDIO_FM_RADIO_SetSoundMode,                       // pointer to (MS_U8)
    MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption,                  // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp,                          // void
    MApi_CMD_AUDIO_FM_RADIO_Get_NSR,                            // void
    MApi_CMD_AUDIO_FM_RADIO_Reset,                              // void
#endif

    // AUDIO_SPDIF RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_SPDIF_OP,                                     // pointer to (AUDIO_ST_SPDIF_OP)
    //MApi_CMD_AUDIO_SPDIF_HWEN,                                  // pointer to (MS_BOOL)                                             
    //MApi_CMD_AUDIO_SPDIF_SetMute,                               // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_SPDIF_SetMode,                               // pointer to (MS_BOOL)
    //MApi_CMD_AUDIO_SPDIF_GetMode,                               // void
    MApi_CMD_AUDIO_SPDIF_SCMS,                                  // pointer to (AUDIO_ST_SPDIF_SCMS)
    //MApi_CMD_AUDIO_SPDIF_SetSCMS,                               // pointer to (MS_U8, MS_U8)                                        
    //MApi_CMD_AUDIO_SPDIF_GetSCMS,                               // void
    MApi_CMD_AUDIO_SPDIF_Monitor,                               // void
    MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL,                    // pointer to (AUDIO_SPDIF_CS_TYPE, AUDIO_SPDIF_CS_TYPE_STATUS)
    MApi_CMD_AUDIO_DigitalOut_SetChannelStatus, 
    MApi_CMD_AUDIO_DigitalOut_SetDeviceCapability,              // pointer to (DIGITAL_OUTPUT_TYPE, AUDIO_FORMAT_CODE, Digital_Out_Device_Capability_t*)

    // AUDIO_HDMI RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_HDMI_Tx_SetMute,                             // pointer to (MS_BOOL)                                             
    MApi_CMD_AUDIO_HDMI_Tx_GetStatus,                           // pointer to (MS_BOOL*, AUDIO_FS_TYPE*, HDMI_TX_OUTPUT_TYPE*)
    MApi_CMD_AUDIO_HDMI_Monitor,                                // void
    MApi_CMD_AUDIO_HDMI_GetNonpcmFlag,                          // void
    MApi_CMD_AUDIO_HDMI_SetNonpcm,                              // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_HDMI_Nonpcm_OP,                              // pointer to (AUDIO_ST_HDMI_NONPCM_OP)
    //MApi_CMD_AUDIO_HDMI_Rx_SetNonPcm,                           // pointer to (MS_U8)                                               
    //MApi_CMD_AUDIO_HDMI_RX_GetNonpcm,                           // void
    MApi_CMD_AUDIO_DTV_HDMI_CFG,                                // pointer to (MS_U8)
    MApi_CMD_AUDIO_HDMI_GetSynthFreq,                           // void
    MApi_CMD_AUDIO_HDMI_SetDownSample,                          // pointer to (MS_U8)
    MApi_CMD_AUDIO_HDMI_TX_SetMode,                             // pointer to (HDMI_TX_OUTPUT_TYPE)
    MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus,                // pointer to (AUDIO_ST_RX_HDMI_AUDIO_STATUS*)

    // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_ADVSOUND_ProcessEnable,                      // pointer to (ADVSND_TYPE)                                         
    MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable,                   // pointer to (ADVFUNC, MS_BOOL)                                    ##100
    MApi_CMD_AUDIO_ADVSOUND_SetParam,                           // pointer to (ADVSND_PARAM, MS_U16, MS_U16)
    MApi_CMD_AUDIO_ADVSND_SetParam,                             // pointer to (ADVSND_PARAM, MS_U32, MS_U16)                        
    //MApi_CMD_AUDIO_ADVSOUND_GetParam,                           // pointer to (ADVSND_GET_PARAM)
    MApi_CMD_AUDIO_ADVSOUND_SET_ASF_PARAM,                      // pointer to (ADVSND_PARAM, MS_S16, MS_S16)
    //MApi_CMD_AUDIO_ADVSOUND_GET_ASF_PARAM,                      // pointer to (ADVSND_GET_PARAM)
    MApi_CMD_AUDIO_ADVSOUND_GetInfo,                            // pointer to (AdvSound_GET_Type)
    MApi_CMD_AUDIO_DBXTV_SetMode,                               // pointer to (EN_DBXTV_TotSonMode, EN_DBXTV_TotVolMode , EN_DBXTV_TotSurMode,MS_U32)
    MApi_CMD_AUDIO_SeInit,                                      // void
    MApi_CMD_AUDIO_SetAdvSndSys,                                // pointer to (MS_U8)
    //MApi_CMD_AUDIO_ADV_SOUND_SET,                               // pointer to (AUDIO_ST_HDMI_NONPCM_OP)
    MApi_CMD_AUDIO_SetVDS,                                      // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetVSPK,                                     // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetSRS,                                      // pointer to (MS_U8)
    MApi_CMD_AUDIO_SetBBE,                                      // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_VSPK_WMod,                                   // pointer to (MS_U8)
    MApi_CMD_AUDIO_VSPK_SMod,                                   // pointer to (MS_U8)                                               
    MApi_CMD_AUDIO_SRS_DC,                                      // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRS_TruBass,                                 // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRSTsxtPara,                                 // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_SetSRSTSHD,                                  // pointer to (MS_U8)
    MApi_CMD_AUDIO_TSHD_TruBass,                                // pointer to (MS_U8)
    MApi_CMD_AUDIO_TSHD_Definition,                             // pointer to (MS_U8)
    MApi_CMD_AUDIO_TSHD_Clarity,                                // pointer to (MS_U8)
    MApi_CMD_AUDIO_SRSSetTshdPara,                              // pointer to (MS_U8, MS_U8)
    MApi_CMD_AUDIO_Copy_Parameter,                              // pointer to (AUDIO_ST_COPY_PARAMETER*)        

    // AUDIO_KTV RELATIONAL API FUNCTION
    MApi_CMD_AUDIO_KTV_OP,                                      // pointer to ()
    //MApi_CMD_AUDIO_SetKTVInfo,                                  // pointer to (AUDIO_KTV_MODEL, AUDIO_KTV_PARAMTYPE, MS_U32, MS_U32) 
    //MApi_CMD_AUDIO_GetKTVInfo,                                  // pointer to (AUDIO_KTV_MODEL, AUDIO_KTV_INFOTYPE)
    //MApi_CMD_AUDIO_SetMixModeVolume,                            // pointer to (AUDIO_SOURCE_INFO_TYPE, AUDIO_MIX_VOL_TYPE, MS_U8, MS_U8) 
    //MApi_CMD_AUDIO_SetMixModeMute,                              // pointer to (AUDIO_SOURCE_INFO_TYPE, AUDIO_MIX_VOL_TYPE, MS_BOOL)
    MApi_CMD_AUDIO_PlayMenuSound,                               // void

    // AUDIO_MM RELATIONAL API FUNCTION    
    MApi_CMD_AUDIO_SetCommAudioInfo,                            // pointer to (Audio_COMM_infoType, MS_U32, MS_U32)                 
    MApi_CMD_AUDIO_SetDecodeParam,                              // pointer to (AUDIO_ST_SETDECODEPARAM)
    MApi_CMD_AUDIO_SetMpegInfo,                                 // pointer to (Audio_MPEG_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetAC3Info,                                  // pointer to (Audio_AC3_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetAC3PInfo,                                 // pointer to (Audio_AC3P_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetAACInfo,                                  // pointer to (Audio_AAC_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetWMAInfo,                                  // pointer to (Audio_WMA_infoType, MS_U32, MS_U32)
    MApi_CMD_AUDIO_SetDTSCommonCtrl,                            // pointer to (Audio_DTS_infoType , Audio_DTS_CtrlType )
    MApi_CMD_AUDIO_GetCommAudioInfo,                            // pointer to (Audio_COMM_infoType)                                 ##120
    MApi_CMD_AUDIO_GetMpegInfo,                                 // pointer to (Audio_MPEG_infoType)
    MApi_CMD_AUDIO_GetAC3Info,                                  // pointer to (Audio_AC3P_infoType)
    MApi_CMD_AUDIO_GetAC3PInfo,                                 // pointer to (Audio_AC3P_infoType)
    MApi_CMD_AUDIO_GetAACInfo,                                  // pointer to (Audio_AAC_infoType)
    MApi_CMD_AUDIO_GetWmaInfo,                                  // pointer to (Audio_WMA_infoType)
    MApi_CMD_AUDIO_GetDTSInfo,                                  // pointer to (Audio_DTS_infoType)
    MApi_CMD_AUDIO_XPCM_Param,                                  // pointer to (XPCM_TYPE, MS_U8, MS_U16, MS_U8 , MS_U16, MS_U16)
    MApi_CMD_AUDIO_XPCM2_Param,                                 // pointer to (XPCM_TYPE, MS_U8, MS_U16, MS_U8 , MS_U16, MS_U16)
    MApi_CMD_AUDIO_XPCM2_CheckIntStatus,                        // void
    MApi_CMD_AUDIO_RA8_Param,                                   // pointer to (AUDIO_ST_RA8_Param)                                  
    MApi_CMD_AUDIO_Init,                                        // pointer to (En_DVB_decSystemType)
    MApi_CMD_AUDIO_StartDecode,                                 // void
    MApi_CMD_AUDIO_StartBroswe,                                 // void
    MApi_CMD_AUDIO_StopDecode,                                  // void
    MApi_CMD_AUDIO_PauseDecode,                                 // void
    MApi_CMD_AUDIO_CheckPlayDone,                               // void
    MApi_CMD_AUDIO_GetResidualBufferSize,                       // void
    MApi_CMD_AUDIO_GetPCMBufferSize,                            // pointer to (MS_U16)
    MApi_CMD_AUDIO_GetPCMBufferSize2,                           // pointer to (MS_U16, MS_U16)
    MApi_CMD_AUDIO_GetCurrentFrameNumber,                       // void                                                             
    MApi_CMD_AUDIO_GetSampleRates,                              // void
    MApi_CMD_AUDIO_GetBitRates,                                 // void                                                             ##140
    MApi_CMD_AUDIO_GetLayer,                                    // void
    MApi_CMD_AUDIO_CheckInputRequest,                           // pointer to (AUDIO_ST_CHECK_INPUT_REQ)
    MApi_CMD_AUDIO_SetInput,                                    // void
    MApi_CMD_AUDIO_SetSampleRateIndex,                          // pointer to (MS_U16)
    MApi_CMD_AUDIO_SetXPCMParam,                                // pointer to (XPCM_TYPE, MS_U8, MS_U16, MS_U8, MS_U16, MS_U16)
    MApi_CMD_AUDIO_FileEndNotification,                         // void
    MApi_CMD_AUDIO_FileEndDataHandle,                           // pointer to (MS_U32)
    MApi_CMD_AUDIO_GetPlayTick,                                 // void                                                             
    MApi_CMD_AUDIO_GetEsMEMCnt,                                 // void
    MApi_CMD_AUDIO_SetASFParm,                                  // pointer to (WMA_ASF_PARMTYPE, MS_U32)
    MApi_CMD_AUDIO_MM_SetInput,                                 // pointer to (En_DVB_AudioDecoder, MS_U8)
    MApi_CMD_AUDIO_MM_CheckPlayDone,                            // pointer to (En_DVB_AudioDecoder)
    MApi_CMD_AUDIO_MM_CheckInputRequest,                        // pointer to (AUDIO_ST_MM_CHECK_INPUT_REQ)
    MApi_CMD_AUDIO_DmaReader_Init,                              // pointer to (SAMPLE_RATE)
    MApi_CMD_AUDIO_DmaReader_AllInput_Init,                     // void
    MApi_CMD_AUDIO_DmaReader_WritePCM,                          // pointer to (AUDIO_ST_DMAREADER_WRITEPCM)
    MApi_CMD_AUDIO_DmaWrite_Init,                               // void
    MApi_CMD_AUDIO_USBPCM_Enable,                               // pointer to (MS_BOOL)                                             
    MApi_CMD_AUDIO_USBPCM_SetFlag,                              // pointer to (MS_BOOL)
    MApi_CMD_AUDIO_USBPCM_GetFlag,                              // void
    MApi_CMD_AUDIO_USBPCM_GetMemInfo,                           // pointer to (AUDIO_UPLOAD_INFO*)
    MApi_CMD_AUDIO_PCMCapture_Init,                             // pointer to (const AUDIO_DEVICE_TYPE , const AUDIO_CAPTURE_SOURCE_TYPE)
    MApi_CMD_AUDIO_PCMCapture_Start,                            // pointer to (cAUDIO_ST_PCMCAPTURE_INIT)
    MApi_CMD_AUDIO_PCMCapture_Stop,                             // pointer to (AUDIO_ST_PCMCAPTURE_INIT)
    MApi_CMD_AUDIO_PCMCapture_Read,                             // pointer to (AUDIO_ST_PCMCAPTURE_READ)
    MApi_CMD_AUDIO_VoIP_Config,                                 // pointer to (AUDIO_VoIP_CONFIG_TYPE, MS_U32, MS_U32)
    MApi_CMD_AUDIO_ALSA_Check,                                  // void
    MApi_CMD_AUDIO_ALSA_Enable,                                 // pointer to (MS_BOOL)                                             
    MApi_CMD_AUDIO_UNI_CheckDecodeDone,                         // pointer to (MS_U32*, MS_U32*)
    MApi_CMD_AUDIO_UNI_SetOutput,                               // pointer to (MS_U32)
    MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr,                   // pointer to (MS_U32)
    MApi_CMD_AUDIO_UNI_Get_OutPCMLevel,                         // void
    MApi_CMD_AUDIO_RingTask,                                    // pointer to (audioRingTask*)
    MApi_CMD_AUDIO_Ring_DataTransfer,                           // pointer to (audioRingTask*, audioRingTask*)
    MApi_CMD_AUDIO_MM2_InitAesInfo,                             // pointer to (AUDIO_DEC_ID)
    MApi_CMD_AUDIO_MM2_CheckAesInfo,                            // pointer to (ADUIO_DEC_ID, AES_INFO*)
    MApi_CMD_AUDIO_MM2_InputAesFinished,                        // pointer to (AUDIO_DEC_ID , MS_U32 , MS_BOOL , MS_U64  )
    MApi_CMD_AUDIO_SetAudioParam2,                              // pointer to (AUDIO_DEC_ID , Audio_ParamType, MS_U32 )             
    MApi_CMD_AUDIO_GetAudioInfo2,                               // pointer to (AUDIO_DEC_ID, Audio_InfoType, void* )
    MApi_CMD_AUDIO_GetDDRInfo,                                  // pointer to (AUDIO_DEC_ID , EN_AUDIO_DDRINFO)
    MApi_CMD_AUDIO_DigitalOut_GetChannelStatus,                 //
    MApi_CMD_AUDIO_GetCaps,
    MApi_CMD_AUDIO_PCM_Open,                                    // pointer to (AUDIO_ST_PCM_OPEN)
    MApi_CMD_AUDIO_PCM_Close,                                   // pointer to (AUDIO_ST_PCM_CLOSE)
    MApi_CMD_AUDIO_PCM_Start,                                   // pointer to (AUDIO_ST_PCM_START)
    MApi_CMD_AUDIO_PCM_Stop,                                    // pointer to (AUDIO_ST_PCM_STOP)
    MApi_CMD_AUDIO_PCM_Set,                                     // pointer to (AUDIO_ST_PCM_SET)
    MApi_CMD_AUDIO_PCM_Get,                                     // pointer to (AUDIO_ST_PCM_GET)
    MApi_CMD_AUDIO_PCM_Read,                                    // pointer to (AUDIO_ST_PCM_READ)
    MApi_CMD_AUDIO_PCM_Write,                                   // pointer to (AUDIO_ST_PCM_WRITE)
    MApi_CMD_AUDIO_PCM_Flush,                                   // pointer to (AUDIO_ST_PCM_FLUSH)
} eAudioIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
/// Audio status indicator
typedef enum
{
    AUDIO_STATUS_OK,               ///< Audio status OK
    AUDIO_STATUS_ERROR,            ///< Audio status ERROR
} AUDIO_STATUS;

typedef enum
{
    DECODE_SYS_RELEASE,
    DECODE_SYS_OPEN,
    DECODE_SYS_SET,
    DECODE_SYS_GET,
}AUDIO_DECODE_TYPE;

typedef enum
{
    AD_OUTPUT_MODE,
    AD_ABSOLUTE_VOLUME,
    AD_SET_MUTE,
    AD_SET_MAIN_VOL,
}AUDIO_SET_AD_TYPE;

typedef enum
{
    SPDIF_HWEN,
    SPDIF_SETMUTE,
    SPDIF_SETMODE,
    SPDIF_GETMODE,
}AUDIO_SPDIF_OP;

typedef enum
{
    KTV_SET_INFO,
    KTV_GET_INFO,
    KTV_SET_MIX_VOL,
    KTV_SET_MIX_MUTE,
}AUDIO_KTV_OP;

typedef struct _AUDIO_ST_RX_HDMI_AUDIO_STATUS
{
    MS_BOOL                     bStatus;
    ST_HDMI_AUDIO_STATUS*       pHdmi_status;
}AUDIO_ST_RX_HDMI_AUDIO_STATUS, *PAUDIO_ST_RX_HDMI_AUDIO_STATUS;

typedef struct _AUDIO_ST_SOUND_SETPARAM
{
    MS_BOOL                     bStatus;
    Sound_SET_Type              eType;
    MS_U32*                     pParam;
}AUDIO_ST_SOUND_SETPARAM, *PAUDIO_ST_SOUND_SETPARAM;

typedef struct _AUDIO_ST_DECODESYSTEM
{
    MS_BOOL                     bStatus;
    AUDIO_DECODE_TYPE           eType;
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_DECODESYSTEM, *PAUDIO_ST_DECODESYSTEM;

typedef struct _AUDIO_ST_OPEN_DECODESYSTEM
{
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_OPEN_DECODESYSTEM,  *PAUDIO_ST_OPEN_DECODESYSTEM;

typedef struct _AUDIO_ST_RELEASE_DECODESYSTEM
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
}AUDIO_ST_RELEASE_DECODESYSTEM, *PAUDIO_ST_RELEASE_DECODESYSTEM;

typedef struct _AUDIO_ST_GET_DECODESYSTEM
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_GET_DECODESYSTEM, *PAUDIO_ST_GET_DECODESYSTEM;

typedef struct _AUDIO_ST_SET_DECODESYSTEM
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
    AudioDecStatus_t*           p_AudioDecStatus;
}AUDIO_ST_SET_DECODESYSTEM, *PAUDIO_ST_SET_DECODESYSTEM;

typedef struct _AUDIO_ST_SIF_GET_AUDIO_STATUS
{
    MS_BOOL                     bStatus;
    AUDIOSTATUS*                p_eAudioStatus;
}AUDIO_ST_SIF_GET_AUDIO_STATUS, *PAUDIO_ST_SIF_GET_AUDIO_STATUS;

typedef struct _AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS
{
    MS_BOOL                         bStatus;
    DIGITAL_OUTPUT_TYPE             ePath;
    Digital_Out_Channel_Status_t*   stChannelStatus;
}AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS, *PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS;

typedef struct _AUDIO_ST_DIGITALOUT_SETDEVICECAP
{
    MS_BOOL                         bStatus;
    DIGITAL_OUTPUT_TYPE             eoutType;
    AUDIO_FORMAT_CODE               ecodecType;
    Digital_Out_Device_Capability_t* p_codecCapability;
}AUDIO_ST_DIGITALOUT_SETDEVICECAP, *PAUDIO_ST_DIGITALOUT_SETDEVICECAP;

#if 1 // Richard.Ni add
typedef struct _AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS
{
    DIGITAL_OUTPUT_TYPE             ePath;
    Digital_Out_Channel_Status_t*   stChannelStatus;
}AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS, *PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS;

#endif

typedef struct _AUDIO_HDMI_TX_GETSTATUS
{
    MS_BOOL                     onOff;
    AUDIO_FS_TYPE               hdmi_smpFreq;
    HDMI_TX_OUTPUT_TYPE         outType;
}AUDIO_HDMI_TX_GETSTATUS, *PAUDIO_HDMI_TX_GETSTATUS;

typedef struct _AUDIO_ST_COPY_PARAMETER
{
    ADVSND_PARAM                type;
    void*                       Parameter_ptr;
    MS_U32                      size;
}AUDIO_ST_COPY_PARAMETER, *PAUDIO_ST_COPY_PARAMETER;

typedef struct _AUDIO_ST_RA8_Param
{
    MS_U8                       u8Status;
    MS_U16                      mNumCodecs;
    MS_U16                      mSamples;
    MS_U16                      mSampleRate;
    MS_U16*                     pChannels;
    MS_U16*                     pRegions;
    MS_U16*                     pcplStart;
    MS_U16*                     pcplQbits;
    MS_U16*                     pFrameSize;
}AUDIO_ST_RA8_Param, *PAUDIO_ST_RA8_Param;

typedef struct _AUDIO_ST_CHECK_INPUT_REQ
{
    MS_U8                       u8Status;
    MS_U32*                     pU32WrtAddr;
    MS_U32*                     pU32WrtBytes;
}AUDIO_ST_CHECK_INPUT_REQ, *PAUDIO_ST_CHECK_INPUT_REQ;

typedef struct _AUDIO_ST_MM_CHECK_INPUT_REQ
{
    MS_U8                       u8Status;
    En_DVB_AudioDecoder         AUDDecoderNo;
    MS_U32*                     pU32WrtAddr;
    MS_U32*                     pU32WrtBytes;
}AUDIO_ST_MM_CHECK_INPUT_REQ, *PAUDIO_ST_MM_CHECK_INPUT_REQ;

typedef struct _AUDIO_ST_DMAREADER_WRITEPCM
{
    MS_BOOL                     bStatus;
    void*                       pBuffer;
    MS_U32                      bytes;
}AUDIO_ST_DMAREADER_WRITEPCM, *PAUDIO_ST_DMAREADER_WRITEPCM;

typedef struct _AUDIO_ST_PCMCAPTURE_INIT
{
    MS_BOOL                     bStatus;
    AUDIO_DEVICE_TYPE           eID;
    AUDIO_CAPTURE_SOURCE_TYPE   eSource;
}AUDIO_ST_PCMCAPTURE_INIT, *PAUDIO_ST_PCMCAPTURE_INIT;

typedef struct _AUDIO_ST_PCMCAPTURE_READ
{
    MS_BOOL                     bStatus;
    AUDIO_DEVICE_TYPE           eID;
    void*                       pBuffer;
    MS_U32                      bytes;
}AUDIO_ST_PCMCAPTURE_READ, *PAUDIO_ST_PCMCAPTURE_READ;

typedef struct _AUDIO_ST_UNI_CHECK_DECODEDONE
{
    MS_BOOL                     bStatus;
    MS_U32*                     pU32WrtAddr;
    MS_U32*                     pU32WrtBytes;
}AUDIO_ST_UNI_CHECK_DECODEDONE, *PAUDIO_ST_UNI_CHECK_DECODEDONE;

typedef struct _AUDIO_ST_RING_DATA_TRANS
{
    audioRingTask*              pauRingTask;
    audioRingTask*              pauRingTask_1;
}AUDIO_ST_RING_DATA_TRANS, *PAUDIO_ST_RING_DATA_TRANS;

typedef struct _AUDIO_ST_MM2_CHECKAESINFO
{
    AES_INFO*                   paes_info;
    AUDIO_DEC_ID                dec_id;
}AUDIO_ST_MM2_CHECKAESINFO, *PAUDIO_ST_MM2_CHECKAESINFO;

typedef struct _AUDIO_ST_MM2_INPUTAES_FINISH
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                dec_id; 
    MS_U32                      es_size;
    MS_BOOL                     ptsExist;
    MS_U64                      pts;
}AUDIO_ST_MM2_INPUTAES_FINISH, *PAUDIO_ST_MM2_INPUTAES_FINISH;

typedef struct _AUDIO_ST_GETAUDIOINFO2
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecId;
    Audio_InfoType              infoType;
    void*                       pInfo;
}AUDIO_ST_GETAUDIOINFO2, *PAUDIO_ST_GETAUDIOINFO2;


typedef struct _AUDIO_ST_ADV_SOUND_SETPARAM
{
    AdvSound_SET_Type           eType;
    MS_U32*                     pParam;
}AUDIO_ST_ADV_SOUND_SETPARAM, *PAUDIO_ST_ADV_SOUND_SETPARAM;

typedef struct _AUDIO_ST_OUTPUT_SOURCE
{
    MS_BOOL                     bSet;
    OUTPUT_SOURCE_INFO*         stOutputSourceInfo;          
}AUDIO_ST_OUTPUT_SOURCE, *PAUDIO_ST_OUTPUT_SOURCE;

typedef struct _AUDIO_ST_SET_AD
{
    AUDIO_SET_AD_TYPE           eType;
    MS_U8                       u8Data;
    MS_U8                       u8Data2;
    MS_BOOL                     bMute;
}AUDIO_ST_SET_AD, *PAUDIO_ST_SET_AD;

typedef struct _AUDIO_ST_DECODER_COMMAND
{
    MS_BOOL                     bSet;
    AU_DVB_DECCMD               eDecCmd;
    En_DVB_AudioDecoder         eAdoDecoderNo;
    En_DVB_decCmdType           enCommandType;
}AUDIO_ST_DECODER_COMMAND, *PAUDIO_ST_DECODER_COMMAND;

typedef struct _AUDIO_ST_SPDIF_OP
{
    AUDIO_SPDIF_OP              eType;
    MS_BOOL                     spdif_hwen;
    MS_BOOL                     mute_en;
    MS_U8                       spdif_mode;    
}AUDIO_ST_SPDIF_OP, *PAUDIO_ST_SPDIF_OP;

typedef struct _AUDIO_ST_SPDIF_SCMS
{
    MS_BOOL                     bSetSCMS;
    MS_U8                       C_bit_en;
    MS_U8                       L_bit_en;
    MS_U8                       Ret_bit;
}AUDIO_ST_SPDIF_SCMS, *PAUDIO_ST_SPDIF_SCMS;

typedef struct _AUDIO_ST_HDMI_NONPCM_OP
{
    MS_BOOL                     bSet;
    MS_U8                       nonPCM_en;
    AUDIO_HDMI_RX_TYPE          eRet;
}AUDIO_ST_HDMI_NONPCM_OP, *PAUDIO_ST_HDMI_NONPCM_OP;


typedef struct _AUDIO_ST_KTV_OP
{
    AUDIO_KTV_OP                eType;
    AUDIO_KTV_MODEL             modelType;
    AUDIO_KTV_PARAMTYPE         paramType;
    AUDIO_KTV_INFOTYPE          infoType;
    AUDIO_SOURCE_INFO_TYPE      eSourceType;
    AUDIO_MIX_VOL_TYPE          VolType;
    MS_U8                       u8Vol1;
    MS_U8                       u8Vol2;
    MS_U32                      param1;     
    MS_U32                      param2;
    MS_BOOL                     EnMute;
    MS_BOOL                     bRet;
    MS_U32                      u32Ret;    
}AUDIO_ST_KTV_OP, *PAUDIO_ST_KTV_OP;

typedef struct _AUDIO_ST_SETDECODEPARAM
{
    MS_BOOL                     bStatus;
    AUDIO_DEC_ID                DecID;
    Audio_ParamType             paramType;
    MS_U32*                     Param;
}AUDIO_ST_SETDECODEPARAM, *PAUDIO_ST_SETDECODEPARAM;

#if 0
typedef struct _AUDIO_ST_AUDIO_INIT_INFO
{
    AUDIO_INIT_INFO             *pau_info;
}AUDIO_ST_AUDIO_INIT_INFO, *PAUDIO_ST_AUDIO_INIT_INFO;

typedef struct _AUDIO_ST_AUDIO_PATH_INFO
{
    AUDIO_PATH_INFO             *path_info;
}AUDIO_ST_AUDIO_PATH_INFO, *PAUDIO_ST_AUDIO_PATH_INFO;

typedef struct _AUDIO_ST_AUDIO_OUT_INFO
{
    AUDIO_OUT_INFO              *pout_info;
}AUDIO_ST_AUDIO_OUT_INFO, *PAUDIO_ST_AUDIO_OUT_INFO;

typedef struct _AUDIO_ST_AUDIO_PEQ_COEF
{
    AUDIO_PEQ_COEF              *peq_coef;
}AUDIO_ST_AUDIO_PEQ_COEF, *PAUDIO_ST_AUDIO_PEQ_COEF;
#endif

typedef struct _AUDIO_ST_AU_DVB_ENC_FRAME_INFO
{
    AU_DVB_ENC_FRAME_INFO       *EncFrameInfo;
}AUDIO_ST_AU_DVB_ENC_FRAME_INFO, *PAUDIO_ST_AU_DVB_ENC_FRAME_INFO;

#if 0
typedef struct _AUDIO_ST_THR_TBLL_TYPE
{
    THR_TBL_TYPE code           *ThrTbl;
}AUDIO_ST_THR_TBLL_TYPE, *PAUDIO_ST_THR_TBLL_TYPE;

typedef struct _AUDIO_ST_UPLOAD_INFO
{
    AUDIO_UPLOAD_INFO           *uploadInfo;
}AUDIO_ST_UPLOAD_INFO, *PAUDIO_ST_UPLOAD_INFO;
#endif
typedef struct _AUDIO_ST_AUDIORINGTASK
{
    audioRingTask               *auRingTask;
}AUDIO_ST_AUDIORINGTASK, *PAUDIO_ST_AUDIORINGTASK;

typedef struct _AUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE
{
    MS_BOOL                     bStatus;
    int*                        pthresh_level_table;
}AUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE, *PAUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE;

typedef struct _AUDIO_ST_SPDIF_CHANNELSTATUS
{
    MS_BOOL                     bSet;
    SPDIF_CS_MODE_TYPE          eType;
    SPDIF_CS_MODE_VALUE         eValue;
    MS_U32                      u32Ret;
}AUDIO_ST_SPDIF_CHANNELSTATUS, *PAUDIO_ST_SPDIF_CHANNELSTATUS;

#if 0
typedef struct _AUDIO_ST_GETCAPS
{
    MS_U32                      u32Ret;
    AUDIO_DEC_ID                eDecId;
    MS_U32*                     pu32Caps;
}AUDIO_ST_GETCAPS, *PAUDIO_ST_GETCAPS;
#endif

typedef struct _AUDIO_ST_SETENCCOMMAND
{
    AU_DVB_ENCCMD               enEncComamnd;
    void                        *pCB;
    MS_U8                       u8Param;
}AUDIO_ST_SETENCCOMMAND, *PAUDIO_ST_SETENCCOMMAND;

typedef struct _AUDIO_ST_PCM_OPEN
{
    void                        *pData;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_OPEN, *PAUDIO_ST_PCM_OPEN;

typedef struct _AUDIO_ST_PCM_CLOSE
{
    MS_S32                      s32DeviceId;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_CLOSE, *PAUDIO_ST_PCM_CLOSE;

typedef struct _AUDIO_ST_PCM_START
{
    MS_S32                      s32DeviceId;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_START, *PAUDIO_ST_PCM_START;

typedef struct _AUDIO_ST_PCM_STOP
{
    MS_S32                      s32DeviceId;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_STOP, *PAUDIO_ST_PCM_STOP;

typedef struct _AUDIO_ST_PCM_SET
{
    MS_S32                      s32DeviceId;
    MS_U32                      u32Cmd;
    const void                  *pData;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_SET, *PAUDIO_ST_PCM_SET;

typedef struct _AUDIO_ST_PCM_GET
{
    MS_S32                      s32DeviceId;
    MS_U32                      u32Cmd;
    void                        *pData;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_GET, *PAUDIO_ST_PCM_GET;

typedef struct _AUDIO_ST_PCM_READ
{
    MS_S32                      s32DeviceId;
    void                        *pBuf;
    MS_U32                      u32Size;
    MS_U32                      u32Ret;
}AUDIO_ST_PCM_READ, *PAUDIO_ST_PCM_READ;

typedef struct _AUDIO_ST_PCM_WRITE
{
    MS_S32                      s32DeviceId;
    const void                  *pBuf;
    MS_U32                      u32Size;
    MS_U32                      u32Ret;
}AUDIO_ST_PCM_WRITE, *PAUDIO_ST_PCM_WRITE;

typedef struct _AUDIO_ST_PCM_FLUSH
{
    MS_S32                      s32DeviceId;
    MS_S32                      s32Ret;
}AUDIO_ST_PCM_FLUSH, *PAUDIO_ST_PCM_FLUSH;

#ifdef __cplusplus
}
#endif


#endif //_AUDIO_H_


