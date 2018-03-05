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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

#ifndef _DRV_AUDIO_INTERNAL_H_
#define _DRV_AUDIO_INTERNAL_H_

#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

//=====================================================
// Define
//=====================================================

//==============================
//  IP Security Define
//==============================
#define  AU_DVB2_AUTHBIT_BBE            0
#define  AU_DVB2_AUTHBIT_SRS            2
#define  AU_DVB2_AUTHBIT_TSHD           3
#define  AU_DVB2_AUTHBIT_VIQ            4
#define  AU_DVB2_AUTHBIT_VDS            5
#define  AU_DVB2_AUTHBIT_VSPK           6
#define  AU_DVB2_AUTHBIT_AUDSY          9
// Dolby DD
#define  AU_DVB_AUTHBIT_AC3             11
#define  AU_DVB2_AUTHBIT_AC3_AD         11
// Dolby DDP
#define  AU_DVB_AUTHBIT_AC3P            12
#define  AU_DVB2_AUTHBIT_AC3P_AD        12
// Dolby DDCO
#define  AU_DVB2_AUTHBIT_DDE            13
// Dolby Pulse(DDT)
#define  AU_DVB_AUTHBIT_MS10_DDT        14
// DTS DMP (Core 2.0)
#define  AU_DVB_AUTHBIT_DTSDEC          15
#define  AU_DVB2_AUTHBIT_DTSDEC         15
// DTS HD
#define  AU_DVB_AUTHBIT_DTSHD           18
// COOK(RA8-LBR) same with RM video
#define  AU_DVB_AUTHBIT_COOK            28
// WMA
#define  AU_DVB_AUTHBIT_WMA             30
#define  AU_DVB2_AUTHBIT_WMA            30
// SRS StudioSound
#define  AU_DVB2_AUTHBIT_SRS_StudioSND  55
// Dolby Volume
#define  AU_DVB2_AUTHBIT_DolbyVolume    56
// Dolby DD Plus Encode
#define  AU_DVB2_AUTHBIT_DDP_ENC        57
// DTS LBR
#define  AU_DVB_AUTHBIT_DTS_LBR         58
#define  AU_DVB2_AUTHBIT_DTS_LBR        58
// DTS XLL
#define  AU_DVB_AUTHBIT_DTS_XLL         59
#define  AU_DVB2_AUTHBIT_DTS_XLL        59
#define  AU_DVB2_AUTHBIT_DBX            64
#define  AU_DVB_AUTHBIT_DRA             65
// SRS PureSound
#define  AU_DVB2_AUTHBIT_SRS_PURESND    66
//DTS StudioSound 3D(=TheaterSound3D)
#define  AU_DVB2_AUTHBIT_DTS_StudioSound3D  69
#define  AU_DVB_AUTHBIT_GAAC            70
#define  AU_DVB_AUTHBIT_DTSNeoUltra     72
#define  AU_DVB2_AUTHBIT_DTSNeoUltra    72
#define  AU_DVB_AUTHBIT_DTSE            73
#define  AU_DVB2_AUTHBIT_DTSE           73
#define  AU_DVB_AUTHBIT_DOLBY_MS11      80
#define  AU_DVB2_AUTHBIT_DOLBY_MS11     80
#define  AU_DVB_AUTHBIT_DOLBY_MS12_B    81
#define  AU_DVB_AUTHBIT_DOLBY_MS12_C    82
#define  AU_DVB_AUTHBIT_DOLBY_MS12_LC   83
#define  AU_DVB2_AUTHBIT_NULL           126
#define  AU_DVB_AUTHBIT_DEMOMODE        127


//=====================================================
// Structure
//=====================================================


//=====================================================
// Enum
//=====================================================
typedef enum
{
    AU_CHIP_T2,
    AU_CHIP_U3,
    AU_CHIP_T3,
    AU_CHIP_T4,
    AU_CHIP_T7,                         //single DSP
    AU_CHIP_JANUS,                      //single DSP
    AU_CHIP_U4,
    AU_CHIP_T8,
    AU_CHIP_T9,
    AU_CHIP_T10,
    AU_CHIP_T11,
    AU_CHIP_T13,
    AU_CHIP_T12,
    AU_CHIP_J2,
    AU_CHIP_K1,
    AU_CHIP_K2,
    AU_CHIP_A1,
    AU_CHIP_A5,
    AU_CHIP_A6,
    AU_CHIP_A7,
    AU_CHIP_A3,
    AU_CHIP_A5P,
    AU_CHIP_AMETHYST,
    AU_CHIP_EAGLE,
    AU_CHIP_EDISON,
    AU_CHIP_EMERALD,
    AU_CHIP_M12,
    AU_CHIP_EINSTEIN,
    AU_CHIP_EIFFEL,
    AU_CHIP_NIKE,
    AU_CHIP_Euler,
    AU_CHIP_NUGGET,
    AU_CHIP_NIKON,
    AU_CHIP_EINSTEIN3,
    AU_CHIP_NAPOLI,
    AU_CHIP_MONACO,
    AU_CHIP_MADISON,
    AU_CHIP_MIAMI,
    AU_CHIP_CLIPPERS,
    AU_CHIP_MUJI,
    AU_CHIP_KERES,
    AU_CHIP_MUNICH,
}AU_CHIP_TYPE;

typedef enum
{
    AUDIO_ETMODE_DTV,     // DTV Mode
    AUDIO_ETMODE_KTV,     // KTV Mode 1
    AUDIO_ETMODE_GAME,  // GAME Mode
    AUDIO_ETMODE_KTV2,   // KTV Mode 2
    AUDIO_ETMODE_VOIP_ON,
    AUDIO_ETMODE_VOIP_OFF,
    AUDIO_ETMODE_KTV_STB, // STB KTV Mode, forward mix ch.6 and ch.7
}AUDIO_ENTERTAINMENT_MODE;

// for dvb2 decoder command
typedef enum
{
    AU_DVB2_DECCMD_STOP,                //0
    AU_DVB2_DECCMD_PLAY,                //1
    AU_DVB2_DECCMD_PLAYFILETSP          = 2,
    AU_DVB2_DECCMD_RESYNC,
    AU_DVB2_DECCMD_PLAYFILE             = 4,
    AU_DVB2_DECCMD_PAUSE                = 6
}AU_DVB2_DECCMD;

// for dvb2 free run mode
typedef enum
{
    AU_DVB2_FreeRunMode_AVsync,         //0
    AU_DVB2_FreeRunMode_FreeRun,        //1
}AU_DVB2_FreeRunMode;

typedef enum
{
    DSP_MEM_TYPE_PM,                    // DSP PM memory
    DSP_MEM_TYPE_DM                     // DSP DM memory
}AUDIO_DSP_MEMORY_TYPE;


//=====================================================

void MDrv_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE u16Mode);
void MDrv_AUDIO_SPDIF_SetOutputType (AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType);
MS_BOOL MDrv_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);
AUDIO_DEC_ID MDrv_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MDrv_AUDIO_CheckPlayDone(void);
//================================================================
//  For Audio initialize
//================================================================
void    MDrv_AUDIO_SetPowerOn(MS_BOOL bPowerOn);
void    MDrv_AUDIO_RebootDSP(MS_BOOL bDspType);
MS_BOOL MDrv_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);
MS_BOOL MDrv_AUDIO_GetEncodeDoneFlag(void);
//================================================================
// Audio I/O Config. function
//================================================================
void    MDrv_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);
void    MDrv_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd);
void    MDrv_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8 u8Value);
void    MDrv_AUDIO_USBPCM_Enable(MS_BOOL bEnable);
void    MDrv_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);
MS_BOOL MDrv_AUDIO_USBPCM_GetFlag(void);
void    MDrv_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);
MS_BOOL MDrv_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL MDrv_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MDrv_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MDrv_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes);

//=====================================================
// AUDIO SPDIF Relational Hal Function
//=====================================================
void    MDrv_AUDIO_SPDIF_HWEN(MS_U8 spdif_hwen);
MS_U8   MDrv_AUDIO_SPDIF_GetMode(void);
void    MDrv_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
MS_U8   MDrv_AUDIO_SPDIF_GetSCMS(void);
MS_BOOL MDrv_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);
MS_BOOL Mdrv_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
//=====================================================
// AUDIO HDMI Relational Hal Function
//=====================================================
MS_BOOL MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);
void    MDrv_AUDIO_HDMI_Tx_SetMute(MS_U8 bEnMute);
AUDIO_HDMI_RX_TYPE MDrv_AUDIO_HDMI_SetNonpcm(MS_U8 u8NonPCM_en);
MS_U16  MDrv_AUDIO_HDMI_GetSynthFreq(void);
void    MDrv_AUDIO_HDMI_SetDownSample(MS_U8 ratio);
MS_BOOL MDrv_AUDIO_HDMI_NonpcmMonitor(void);
//================================================================
// for audio driver internal use function
//================================================================
void    MDrv_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);
void    MDrv_AUDIO_DmaReader_AllInput_Init(void);
MS_BOOL MDrv_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);
void    MDrv_AUDIO_DmaWriter_Init(void);

//================================================================
// Audio VoIP Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2);

//================================================================
// Audio ALSA Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_ALSA_Check(void);


MS_U32 MDrv_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

MS_S32 MDrv_AUDIO_PCM_Open(VOID *pData);
MS_S32 MDrv_AUDIO_PCM_Close(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Start(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const VOID *pData);
MS_S32 MDrv_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, VOID *pData);
MS_U32 MDrv_AUDIO_PCM_Read(MS_S32 s32DeviceId, const VOID *pBuf, MS_U32 u32Size);
MS_U32 MDrv_AUDIO_PCM_Write(MS_S32 s32DeviceId, const VOID *pBuf, MS_U32 u32Size);
MS_S32 MDrv_AUDIO_PCM_Flush(MS_S32 s32DeviceId);

#ifdef __cplusplus
}
#endif

#endif // _DRV_AUDIO_IF_H_

