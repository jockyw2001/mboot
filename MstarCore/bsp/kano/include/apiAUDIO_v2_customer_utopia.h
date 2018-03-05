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
// Copyright (c) 2013-2015 MStar Semiconductor, Inc.
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
//  File name:   apiAUDIO_v2_customer_utopia.h
//  Description: apiAUDIO_v2_customer_utopia.h
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ROLLS_ROYCE

#ifndef _AUDIO_V2_CUSTOMER_H_
#define _AUDIO_V2_CUSTOMER_H_

#include "MsTypes.h"
#include "drvAUDIO_if.h"
#include "apiAUDIO_v2_customer_config.h"
#include "../../drv/audio/internal/drvAUDIO_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Enum / Struct
//-------------------------------------------------------------------------------------------------
///< Select AD output type
enum
{
    AD_OUT_SPEAKER, ///< AD out in speaker only
    AD_OUT_HP,      ///< AD out in HP only
    AD_OUT_BOTH,    ///< AD out both in speaker & HP
    AD_OUT_NONE     ///< no AD out
};

///< Audio source switch group
typedef enum
{
    E_AUDIO_GROUP_MAIN,     ///< Main speaker, HP, Line-out , S/PDIF PCM mode
    E_AUDIO_GROUP_SCART,    ///< SCART
    E_AUDIO_GROUP_SPDIF,    ///< SPDIF non-pcm
    E_AUDIO_GROUP_SUB,      ///< SUB source for PIP application
    E_AUDIO_GROUP_INVALID
} AUDIO_SWITCH_GROUP;

///< Decoder command type
typedef enum
{
    MSAPI_AUD_DVB_DECCMD_STOP = 0x0,        ///< 1st DSP stop
    MSAPI_AUD_DVB_DECCMD_PLAY,              ///< 1st DSP play
    MSAPI_AUD_DVB_DECCMD_RESYNC,
    MSAPI_AUD_DVB_DECCMD_FREE_RUN,
    MSAPI_AUD_DVB_DECCMD_AVSYNC,
    MSAPI_AUD_DVB_DECCMD_PLAYFILE,          ///< 1st DSP play file
    MSAPI_AUD_DVB_DECCMD_PAUSE,             ///< 1st DSP pause
    MSAPI_AUD_DVB_DECCMD_PLAYFILETSP,       ///< 1st DSP play TS file
    MSAPI_AUD_DVB_DECCMD_STARTBROWSE,
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME,         ///<1st DSP play MM (for uniplayer/Mstreamer)
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE,   ////<1st DSP play MM (for stagecraft)
    MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS,      ///<1st DSP play MM (for uniplayer/Gstreamer ES copy)
    MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2,
    MSAPI_AUD_DVB_DECCMD_PLAY_AD,
    MSAPI_AUD_DVB_DECCMD_STOP_AD,
    // SE DSP
    MSAPI_AUD_DVB2_DECCMD_STOP=0x10,        ///< 2nd DSP stop
    MSAPI_AUD_DVB2_DECCMD_PLAY,             ///< 2nd DSP play
    MSAPI_AUD_DVB2_DECCMD_RESYNC,
    MSAPI_AUD_DVB2_DECCMD_FREE_RUN,
    MSAPI_AUD_DVB2_DECCMD_AVSYNC,
    MSAPI_AUD_DVB2_DECCMD_PLAYFILE,         ///< 2nd DSP play file
    MSAPI_AUD_DVB2_DECCMD_PAUSE,            ///< 2nd DSP pause
    MSAPI_AUD_DVB2_DECCMD_PLAYFILETSP       ///< 2nd DSP play TS file

} En_DVB_decCmdType;

///< ATV SIF Command
typedef enum
{
    MSAPI_AUD_SIF_CMD_SET_STOP                  = 0x00,
    MSAPI_AUD_SIF_CMD_SET_PLAY                  = 0x01,
    MSAPI_AUD_SIF_CMD_ENABLE_HIDEV              = 0x02,
    MSAPI_AUD_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL = 0x03,
    MSAPI_AUD_SIF_CMD_RESET_FC_TRACKING         = 0x04,
    MSAPI_AUD_SIF_CMD_ENABLE_FC_TRACKING        = 0x05,
    MSAPI_AUD_SIF_CMD_SET_ADC_FROM_VIF_PATH     = 0x06,
    MSAPI_AUD_SIF_CMD_ENABLE_AUTO_MUTE          = 0x07,
    MSAPI_AUD_SIF_CMD_ENABLE_BEEPER_FUNCTION    = 0x08,
    MSAPI_AUD_SIF_CMD_SET_BEEPER_TONE           = 0x09,
    MSAPI_AUD_SIF_CMD_ENABLE_AGC                = 0x0A,
    MSAPI_AUD_SIF_CMD_RESET_AGC                 = 0x0B,
    MSAPI_AUD_SIF_STANDARD_RESET                = 0x0C,
    MSAPI_AUD_SIF_CMD_DETECT_MAIN_STD_ONLY      = 0x0D,
    MSAPI_AUD_SIF_CMD_ENABLE_AUTO_SOUNDMODE     = 0x0E,
    MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP           = 0x0F,
    MSAPI_AUD_SIF_CMD_SET_ENC_CMD               = 0x10,
    MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation = 0x11,
    MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling    = 0x12,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain       = 0x13,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain       = 0x14,
    MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain     = 0x15,

    MSAPI_AUD_SIF_CMD2_ADC_VIA_VIF_PIN          = 0x01
} En_AUD_SIF_CmdType;

///< Audio SIF Standard Type
typedef enum
{
    E_AUDIOSTANDARD_BG          = 0x00,         ///< Audio standard BG
    E_AUDIOSTANDARD_BG_A2       = 0x01,         ///< Audio standard BG A2
    E_AUDIOSTANDARD_BG_NICAM    = 0x02,         ///< Audio standard BG NICAM
    E_AUDIOSTANDARD_I           = 0x03,         ///< Audio standard I
    E_AUDIOSTANDARD_DK          = 0x04,         ///< Audio standard DK
    E_AUDIOSTANDARD_DK1_A2      = 0x05,         ///< Audio standard DK1 A2
    E_AUDIOSTANDARD_DK2_A2      = 0x06,         ///< Audio standard DK2 A2
    E_AUDIOSTANDARD_DK3_A2      = 0x07,         ///< Audio standard DK3 A2
    E_AUDIOSTANDARD_DK_NICAM    = 0x08,         ///< Audio standard DK NICAM
    E_AUDIOSTANDARD_L           = 0x09,         ///< Audio standard L
    E_AUDIOSTANDARD_M           = 0x0A,         ///< Audio standard M
    E_AUDIOSTANDARD_M_BTSC      = 0x0B,         ///< Audio standard M BTSC
    E_AUDIOSTANDARD_M_A2        = 0x0C,         ///< Audio standard M A2
    E_AUDIOSTANDARD_M_EIA_J     = 0x0D,         ///< Audio standard M EIA J
    E_AUDIOSTANDARD_NOTSTANDARD = 0x0F          ///< Not Audio standard
} AUDIOSTANDARD_TYPE;

///< sif carrier status define
typedef enum
{
    // Never change the value of enumeration. Because each bit has weight.
    E_STATE_AUDIO_NO_CARRIER           = 0x00, ///< No carrier detect
    E_STATE_AUDIO_PRIMARY_CARRIER      = 0x01, ///< Carrier 1 exist
    E_STATE_AUDIO_SECONDARY_CARRIER    = 0x02, ///< Carrier 2 exist
    E_STATE_AUDIO_NICAM                = 0x04, ///< Nicam lock state
    E_STATE_AUDIO_STEREO               = 0x08, ///< A2 Stereo exist
    E_STATE_AUDIO_BILINGUAL            = 0x10, ///< A2 Dual exist
    E_STATE_AUDIO_PILOT                = 0x20, ///< A2 Pilot exist
    E_STATE_AUDIO_DK2                  = 0x40, ///< Sound standard is DK2
    E_STATE_AUDIO_DK3                  = 0x80  ///< Sound standard is DK3
} AUDIOSTATUS;

///< Audio mode type
typedef enum
{
    E_AUDIOMODE_INVALID       = 0x00,           ///< Audio Mode Invalid
    E_AUDIOMODE_MONO          = 0x01,           ///< Audio Mode MONO
    E_AUDIOMODE_FORCED_MONO   = 0x02,           ///< Audio Mode Forced MONO
    E_AUDIOMODE_G_STEREO      = 0x03,           ///< Audio Mode G Stereo
    E_AUDIOMODE_K_STEREO      = 0x04,           ///< Audio Mode  K Stereo
    E_AUDIOMODE_MONO_SAP      = 0x05,           ///< Audio Mode MONO SAP
    E_AUDIOMODE_STEREO_SAP    = 0x06,           ///< Audio Mode Stereo SAP
    E_AUDIOMODE_DUAL_A        = 0x07,           ///< Audio Mode Dual A
    E_AUDIOMODE_DUAL_B        = 0x08,           ///< Audio Mode Dual B
    E_AUDIOMODE_DUAL_AB       = 0x09,           ///< Audio Mode Dual AB
    E_AUDIOMODE_NICAM_MONO    = 0x0A,           ///< Audio Mode NICAM MONO
    E_AUDIOMODE_NICAM_STEREO  = 0x0B,           ///< Audio Mode NICAM Stereo
    E_AUDIOMODE_NICAM_DUAL_A  = 0x0C,           ///< Audio Mode NICAM DUAL A
    E_AUDIOMODE_NICAM_DUAL_B  = 0x0D,           ///< Audio Mode NICAM DUAL B
    E_AUDIOMODE_NICAM_DUAL_AB = 0x0E,           ///< Audio Mode NICAM DUAL AB
    E_AUDIOMODE_HIDEV_MONO    = 0x0F,           ///< Audio Mode HIDEV MONO
    E_AUDIOMODE_LEFT_LEFT     = 0x10,           ///< Audio Mode Left left
    E_AUDIOMODE_RIGHT_RIGHT   = 0x11,           ///< Audio Mode right right
    E_AUDIOMODE_LEFT_RIGHT    = 0x12            ///< Audio Mode left right
} AUDIOMODE_TYPE;


typedef enum
{
    HDMITX_AUDIO_FREQ_NO_SIG  = 0,
    HDMITX_AUDIO_32K          = 1,
    HDMITX_AUDIO_44K          = 2,
    HDMITX_AUDIO_48K          = 3,
    HDMITX_AUDIO_88K          = 4,
    HDMITX_AUDIO_96K          = 5,
    HDMITX_AUDIO_176K         = 6,
    HDMITX_AUDIO_192K         = 7,
    HDMITX_AUDIO_FREQ_MAX_NUM = 8,
} HDMITX_AUDIO_FREQUENCY;

typedef enum
{
    HDMITX_AUDIO_PCM        = 0, // PCM
    HDMITX_AUDIO_NONPCM     = 1, // non-PCM
} HDMITX_AUDIO_CODING_TYPE;

typedef enum
{
    HDMITX_AUDIO_CH_2  = 2, // 2 channels
    HDMITX_AUDIO_CH_8  = 8, // 8 channels
} HDMITX_AUDIO_CHANNEL_COUNT;

typedef enum
{
    HDMITX_AUDIO_FORMAT_PCM   = 0,
    HDMITX_AUDIO_FORMAT_DSD   = 1,
    HDMITX_AUDIO_FORMAT_HBR   = 2,
    HDMITX_AUDIO_FORMAT_NA    = 3,
} HDMITX_AUDIO_SOURCE_FORMAT;

typedef enum
{
    E_HDCP_DISABLE  = 0, // HDCP disable
    E_HDCP_FAIL     = 1, // HDCP fail
    E_HDCP_PASS     = 2, // HDCP pass
} HDMITX_HDCP_STATUS;

//-------------------------------------------------------------------------------------------------
//  Functions
//-------------------------------------------------------------------------------------------------

//============================================================
// System
//============================================================
void MApi_AUDIO_WritePreInitTable(void);
void MApi_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr);
void MApi_AUDIO_Initialize(void);
void MApi_Audio_Monitor(void);
MS_U32 MApi_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);

//============================================================
// path
//============================================================
void MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info);
void MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup);
void MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType);

//=============================================================
// volume
//=============================================================
void MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute);
void MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2);
void MApi_AUDIO_SetAbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2);
void MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute );
void MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en);

//============================================================
// Decoder
//============================================================
AUDIO_DEC_ID MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId);
MS_BOOL MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType infoType, MS_U32 param1, MS_U32 param2);
void    MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd);
void    MApi_AUDIO_SetADOutputMode(MS_U8 out_mode);
void    MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value);

//============================================================
// Common Decoder
//============================================================
#if(UTPA_SUPPORT_COMMON_DECODER_API)
MS_S32 MApi_AUDIO_DECODER_Open(void * pData);
MS_S32 MApi_AUDIO_DECODER_Close(MS_S32 s32DeviceID);
MS_S32 MApi_AUDIO_DECODER_Start(MS_S32 s32DeviceID);
MS_S32 MApi_AUDIO_DECODER_Stop(MS_S32 s32DeviceID);
MS_S32 MApi_AUDIO_DECODER_Set(MS_S32 s32DeviceID, AUDIO_DECODER_Command_t Cmd, void* pData);
MS_S32 MApi_AUDIO_DECODER_Get(MS_S32 s32DeviceID, AUDIO_DECODER_Command_t Cmd, void* pData);
MS_U32 MApi_AUDIO_DECODER_Read(MS_S32 s32DeviceID, void* pBuf, MS_U32 u32Size);
MS_U32 MApi_AUDIO_DECODER_Write(MS_S32 s32DeviceID, void* pBuf, MS_U32 u32Size);
MS_S32 MApi_AUDIO_DECODER_Flush(MS_S32 s32DeviceID);
#endif //#if(UTPA_SUPPORT_COMMON_DECODER_API)

//============================================================
// Encode
//============================================================


//=============================================================
// Common Info
//=============================================================
MS_BOOL   MApi_AUDIO_SetCommAudioInfo(Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2);
long long MApi_AUDIO_GetCommAudioInfo(Audio_COMM_infoType infoType);
MS_BOOL   MApi_AUDIO_SetAudioParam2(AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param);
MS_BOOL   MApi_AUDIO_GetAudioInfo2(AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo);

//=============================================================
// Capture
//=============================================================
MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes);
MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);

//============================================================
// ATV
//============================================================
void    MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2);
MS_BOOL MApi_AUDIO_SIF_SetBandSetup(MS_U8 SifBand);
void    MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type);
MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type);
void    MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type);
MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode);
void    MApi_AUDIO_TriggerSifPLL(void);
MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus);
MS_U8   MApi_AUDIO_SIF_GetSoundMode(void);
void    MApi_AUDIO_SIF_StartAutoStandardDetection(void);
AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void);

//=============================================================
// SPDIF
//=============================================================
void    MApi_Audio_SPDIF_Monitor(void);
void    MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode);
void    MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
void    MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en);
MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);

//=============================================================
// HDMI
//=============================================================
#ifdef CONFIG_API_HDMITX //TV dont have HDMI TX
MS_BOOL MApi_HDMITx_Init(void);
void    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_FREQUENCY freq, HDMITX_AUDIO_CHANNEL_COUNT ch, HDMITX_AUDIO_CODING_TYPE type);
void    MApi_HDMITx_SetAudioOnOff(MS_BOOL state);
void    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_FREQUENCY freq);
void    MApi_HDMITx_SetAudioSourceFormat(HDMITX_AUDIO_SOURCE_FORMAT fmt);
HDMITX_HDCP_STATUS MApi_HDMITx_GetHDCPStatus(void);
#endif

MS_BOOL MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability);
MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void);
void    MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType );
void    MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType);
AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en);
AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_GetNonPCM(void);

//=============================================================
// PCM IO
//=============================================================
MS_S32 MApi_AUDIO_PCM_Open(void *pData);
MS_S32 MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId);
MS_S32 MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId);
MS_S32 MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
MS_S32 MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32 MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32 MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32 MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32 MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId);

//=============================================================
// Basic Sound Effect
//=============================================================
MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);
MS_BOOL MApi_SND_SetParam1(Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);
void    MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);

//=============================================================
// ADV Sound Effect
//=============================================================
MS_BOOL MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type);
MS_BOOL MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);
MS_BOOL MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2);

//=============================================================
// MM
//=============================================================
MS_PHY  MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);
MS_BOOL MApi_AUDIO_MM2_initAesInfo(AUDIO_DEC_ID dec_id);
MS_BOOL MApi_AUDIO_MM2_checkAesInfo(AUDIO_DEC_ID  dec_id, AES_INFO *aes_info);
MS_BOOL MApi_AUDIO_MM2_inputAesFinished(AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts);


#ifdef __cplusplus
}
#endif

#endif //_AUDIO_V2_CUSTOMER_H_

#endif // #ifdef ROLLS_ROYCE
