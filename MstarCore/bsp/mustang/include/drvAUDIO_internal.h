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
//  Macro and Define
//=====================================================
#define AUDIO_DO_ALIGNMENT(Value, AlignmentSize) \
    do { \
        Value = (Value /AlignmentSize) * AlignmentSize; \
    } while(0)

#define AUDIO_INT16_MAX    ((MS_S32)0x7fff)
#define AUDIO_INT16_MIN    ((MS_S32)-0x8000)
#define AUDIO_INT32_MAX    ((MS_S32)0x7fffffff)
#define AUDIO_INT32_MIN    ((MS_S32)-0x80000000)

#define AUDIO_FIND_MIN(a,b)    ((a) < (b) ? (a) : (b))
#define AUDIO_FIND_MAX(a,b)    ((a) > (b) ? (a) : (b))


//=====================================================
// Define
//=====================================================
#define MAX_AUDIO_BASE_ADDRESS_NUM2     3
#define AUDIO_PCM_MAXIMUM                20
#define AUDIO_PCM_SWMIXER_SERVER_MAXIMUM    2
#define AUDIO_PCM_SWMIXER_CLIENT_MAXIMUM    8
#define AUDIO_PCM_SWMIXER_CHANNEL    2
#define AUDIO_PCM_SWMIXER_GROUP_MAX    AUDIO_PCM_SWMIXER_GROUP_2
/*
 * must costant output size and various input size, 128 will output size error in 32 sample rate
 * 3840 =640*6 mini product
 * 1920, no 11.025
 * 960 , no 11.025 22.05
 */
#define AUDIO_PCM_SWMIXER_OUTPUT_SIZE_MAXIMUM    3840

#ifdef MSOS_TYPE_LINUX_KERNEL
#define AUDIO_MEM_ALLOC(size)    kzalloc(size, GFP_KERNEL)
#define AUDIO_MEM_FREE(ptr)    kfree(ptr)
#define AUDIO_SLEEP_MS(ms)    msleep(ms)
#define AUDIO_GET_SYSTEM_TIME_MS()    jiffies_to_msecs(jiffies)
#else
#define AUDIO_MEM_ALLOC(size)    calloc(size, 1)
#define AUDIO_MEM_FREE(ptr)    free(ptr)
#define AUDIO_SLEEP_MS(ms)    MsOS_DelayTask(ms)
#define AUDIO_GET_SYSTEM_TIME_MS()    MsOS_GetSystemTime()
#endif

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
#define  AU_DVB2_AUTHBIT_SONICMOTION_ABS3D  92
#define  AU_DVB2_AUTHBIT_BONGIOVI_DPS   93
#define  AU_DVB2_AUTHBIT_NULL           126
#define  AU_DVB_AUTHBIT_DEMOMODE        127
#define  AU_DVB2_AUTHBIT_DOLBY_MS11_WO_AAC       115
#define  AU_DVB2_AUTHBIT_DOLBY_MS12_D_WO_AAC     116
#define  AU_DVB2_AUTHBIT_DOLBY_MS12_B_WO_AAC     117

//=====================================================
// Structure
//=====================================================


//=====================================================
// Enum
//=====================================================
//====== DSP code Type========
typedef enum
{
    AU_DVB_STANDARD_INVALID             = 0xFF,
    AU_DVB_STANDARD_DEC_DSP             = 0x00,    // define for dsp load code
    AU_DVB_STANDARD_DEC_DSP_EXT         = 0x10,    // define for dsp load code
    AU_DVB_NONE                         = 0x00,     //0x00
    AU_DVB_DEC_NONE,                    //0x01
    AU_DVB_ENC_NONE,                    //0x02
    AU_DVB_STANDARD_MPEG,               //0x03
    AU_DVB_STANDARD_AC3,                //0x04
    AU_DVB_STANDARD_AC3P,               //0x05
    AU_DVB_STANDARD_AAC,                //0x06
    AU_DVB_STANDARD_MP3,                //0x07
    AU_DVB_STANDARD_WMA,                //0x08
    AU_DVB_STANDARD_RESERVE3,           //0x09
    AU_DVB_STANDARD_RA8LBR,             //0x0A
    AU_DVB_STANDARD_XPCM,               //0x0B
    AU_DVB_STANDARD_TONE,               //0x0C
    AU_DVB_STANDARD_DTS,                //0x0D
    AU_DVB_STANDARD_MS10_DDT,           //0x0E
    AU_DVB_STANDARD_MS10_DDC,           //0x0F
    AU_DVB_STANDARD_WMA_PRO,            //0x10
    AU_DVB_STANDARD_FLAC,               //0x11
    AU_DVB_STANDARD_VORBIS,             //0x12
    AU_DVB_STANDARD_DTSLBR,             //0x13
    AU_DVB_STANDARD_AMR_NB,             //0x14
    AU_DVB_STANDARD_AMR_WB,             //0x15
    AU_DVB_STANDARD_DRA,                //0x16
    AU_DVB_STANDARD_MPEG_AD,            //0x17
    AU_DVB_STANDARD_DolbyTrueHD_Bypass, //0x18
    AU_DVB_STANDARD_DTSHD_ADO,          //0x19
    AU_DVB_STANDARD_DTSXLL,             //0x1A
    AU_DVB_STANDARD_MAX,

    AU_DVB_STANDARD_ENC                 = 0x20,    // define for dsp load code
    AU_DVB_STANDARD_MPEG_EN             = 0x20,

    AU_DVB2_STANDARD_SE_DSP             = 0x30,    // define for dsp load code
    //AU_DVB2_STANDARD_SE_DSP_EXT         = 0x40,    // need to refine ADV judgement first.
    AU_DVB2_NONE                        = 0x30,
    AU_DVB2_STANDARD_MPEG_AD ,
    AU_DVB2_STANDARD_AC3_AD,
    AU_DVB2_STANDARD_AC3P_AD,
    AU_DVB2_STANDARD_AAC_AD,
    AU_DVB2_STANDARD_DDE,
    AU_DVB2_STANDARD_GAAC_AD,
    AU_DVB2_STANDARD_MP3,
    AU_DVB2_STANDARD_MPEG_EN,
    AU_DVB2_STANDARD_XPCM,
    AU_DVB2_STANDARD_KTV,
    AU_DVB2_STANDARD_KTV2,
    AU_DVB2_STANDARD_DTS,
    AU_DVB2_STANDARD_TONEGEN,
    AU_DVB2_STANDARD_DolbyTrueHD_Bypass,
    AU_DVB2_STANDARD_DTSHD_ADO,
    AU_DVB2_STANDARD_MAX,

    AU_DVB2_ADVSND                      = 0x40,    // define for dsp load code
    AU_DVB2_ADVSND_PL2                  = 0x40,
    AU_DVB2_ADVSND_BBE,
    AU_DVB2_ADVSND_SRS,
    AU_DVB2_ADVSND_VDS,
    AU_DVB2_ADVSND_VSPK,
    AU_DVB2_ADVSND_SUPV,
    AU_DVB2_ADVSND_TSHD,
    AU_DVB2_ADVSND_DTS_ULTRA_TV,
    AU_DVB2_ADVSND_TSHDVIQ,
    AU_DVB2_ADVSND_AUDSY,
    AU_DVB2_ADVSND_DBX,
    AU_DVB2_ADVSND_THEATERSOUND,
    AU_DVB2_ADVSND_PURESND,
    AU_DVB2_ADVSND_STUDIOSOUND_3D,
    AU_DVB2_ADVSND_BONGIOVI_DPS,
    AU_DVB2_ADVSND_CV3,
    AU_DVB2_ADVSND_SONICMOTION_ABS3D,
    AU_DVB2_ADVSND_MAX,

    AU_SE_SYSTEM                        = 0x60,

    AU_KTV_ENC                          = 0x70,
    AU_KTV_FUNC                         = 0x80,
    AU_KTV_SYSTEM                       = 0x90,

    AU_SND_EFFECT                       = 0xa0,

    AU_STANDARD_SIF_TYPE                = 0xb0,
    AU_SIF_NONE                         = 0xb0,
    AU_SIF_BTSC ,
    AU_SIF_EIAJ,
    AU_SIF_PALSUM,
    AU_SIF_FM_RADIO,
    AU_SIF_MAX,

    AU_ATVENC_NONE                      = 0xc0,
    AU_ATVENC_FMTx,
    AU_ATVENC_BTSC,

    AU_DVB2_ENCODE_NONE                 = 0xd0,    // define for snd_dsp encode code for because 0x30 has no free enum
    AU_DVB2_ENCODE_DTSE        = 0xd1,
    AU_DVB2_ENCODE_MS10_DDE    = 0xd2,
    AU_DVB2_ENCODE_DDE    = 0xd3,
    AU_DVB2_ENCODE_MAX,

    AU_DEC_SYSTEM                       = 0xe0,

}AUDIO_DSP_CODE_TYPE;

#define IS_AUDIO_DSP_CODE_TYPE_SIF(dspCodeType)          ((dspCodeType >= AU_SIF_NONE) && (dspCodeType < AU_SIF_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_SE_ENC(dspCodeType)       ((dspCodeType >= AU_DVB2_ENCODE_NONE) && (dspCodeType < AU_DVB2_ENCODE_MAX))

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
    AU_CHIP_MULAN,
    AU_CHIP_MILAN,
    AU_CHIP_MUSTANG,
    AU_CHIP_MARLON,
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

typedef enum
{
    AUDIO_DTV_DVB,
    AUDIO_DTV_ATSC,
    AUDIO_DTV_ISDB,
    AUDIO_DTV_DMBT,
    AUDIO_DTV_OTHER = 15
}AUDIO_DTV_SYSTEM;

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

typedef enum
{
    AUDIO_MULTICH_PCM_DMXL,
    AUDIO_MULTICH_PCM_DMXR,
    AUDIO_MULTICH_PCM_L,
    AUDIO_MULTICH_PCM_C,
    AUDIO_MULTICH_PCM_R,
    AUDIO_MULTICH_PCM_LS,
    AUDIO_MULTICH_PCM_RS,
    AUDIO_MULTICH_PCM_LFE,
    AUDIO_MULTICH_PCM_LRS,
    AUDIO_MULTICH_PCM_RRS,
}AUDIO_MULTICH_PCM_TYPE;

typedef struct
{
    MS_S32 (*open)(void *pData);
    MS_S32 (*close)(void);
    MS_S32 (*start)(void);
    MS_S32 (*stop)(void);
    MS_S32 (*set)(MS_U32 u32Cmd, const void *pData);
    MS_S32 (*get)(MS_U32 u32Cmd, void *pData);
    MS_U32 (*read)(void *pBuf, MS_U32 u32Size);
    MS_U32 (*write)(const void *pBuf, MS_U32 u32Size);
    MS_S32 (*flush)(void);
} AUDIO_PCM_OPS_t;

typedef struct
{
    MS_U8 u8MultiChFlag;
    MS_U8 u8MixingFlag;
    MS_U8 u8CaptureFlag;
    MS_U32 u32Channel[16];
    MS_U32 u32SampleRate[32];
    MS_U32 u32BitWidth[8];
} AUDIO_PCM_CAPS_t;

typedef struct
{
    MS_S8 *pPhyBuffer;
    MS_U32 u32ReadOffset;
    MS_U32 u32WriteOffset;
    MS_U32 u32BufferSize;
    MS_U32 u32BufferHighThreshold;
    MS_U32 u32RemainSize;
} AUDIO_PCM_BUFFER_INFO_t;

typedef struct
{
    MS_S8 *pInputBuffer;
    MS_S32 s32InputSize;
    MS_U8 u8WaitFirstPcmData;
} AUDIO_PCM_SWMIXER_CLIENT_t;

typedef struct
{
    MS_U8 u8ServerId;
    MS_U8 u8ThreadRunning;
    MS_U8 u8ThreadExit;
    MS_S8 *pHandleRoot;
    MS_S8 *pHandle;
    MS_S8 *pBuffer;
    MS_S32 s32OutputId;
    AUDIO_PCM_SWMIXER_CLIENT_t tPcmSwMixerClient[AUDIO_PCM_MAXIMUM];
} AUDIO_PCM_SWMIXER_SERVER_t;

typedef struct
{
    MS_U8 u8Name[AUDIO_COMMON_PCM_IO_NAME_LENGTH]; /* PCM name */
    AUDIO_PCM_OPS_t tPcmOps;  /* PCM operators */
    AUDIO_PCM_CAPS_t tPcmCaps;  /* PCM capabilities */
    AUDIO_PCM_INFO_t *pPcmInfo; /* PCM information */
    AUDIO_PCM_BUFFER_INFO_t *pPcmBufferInfo;  /* PCM buffer information */
    MS_U8 u8SettingChangeFlag;
} AUDIO_PCM_t;

typedef struct
{
    MS_BOOL          bDecInUsed;
    AUDIO_USAGE_TYPE eUsageType;
    MMA_AudioType    eAudioType;
} AUDIO_DSP_USAGE;

typedef struct
{
    MS_U32  g_ShmID;

    MS_BOOL g_bDecPlayFileFlag;   // DEC MM interrupt flag
    MS_BOOL g_bSePlayFileFlag;    // SE  MM interrupt flag
    MS_BOOL g_bMMPlayFlag;        // MM  play flag
    MS_BOOL g_Audio_PreInitFlag;
    MS_BOOL g_Audio_InitFlag;
    MS_BOOL g_SPDIF_MuteStatus;
    AUDIO_SPDIF_OUTPUT_TYPE g_eSpdifOutputType;
    MS_U8   g_SpdifNpcmPath;
    MS_U8   g_PreMainDecodeId;

    MS_BOOL g_dolbyLoudness_Enable;
    MS_U8   g_dolbyLoudness_DTVsystem;

    MS_U8   g_u8DspCodeType;
    MS_U8   g_u8Dsp2CodeType;

    MS_U8   g_u8DecR2Adec1DecType;
    MS_U8   g_u8DecR2Adec2DecType;

    MS_U8   g_u8MADSkipResetFlag;                         //add for STB PVR skip-> play reset use
    MS_U8   g_u8MAD2SkipResetFlag;                       //add for STB PVR skip-> play reset use

    MS_U16  u16DecodeFrmCnt[AU_DEC_MAX];

    MS_U8   g_u8AdvSndEffDspCodeType;
    MS_U8   g_u8AdvSndEffType;              //store ADV sound type
    MS_U8   g_u8DspAliveFlag;
    MS_U8   g_u8AudioCrashFlag;

    MS_U8   g_u8SifDspType;
    MS_U16  sif_gain_0[6];
    MS_U16  sif_shift_0[6];

    MS_U8   g_u8IntTag;   // DEC MM interrupt flag
    MS_U16  int_mb_cnt;

    MS_U8   CompressInfo;
    MS_U8   g_DSPMadMIUBank[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_U32  g_DSPBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_U32  g_DSPMadBaseBufferAdr[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_U32  g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];

    MS_U32  g_u32ClientCounter;

    AUDIO_SOURCE_INFO_TYPE eMainSourceType;
    AUDIO_SOURCE_INFO_TYPE ePrevSourceType;
    AUDIO_DSP_USAGE        DspUsage[AU_DEC_MAX];
    AudioDecStatus_t            AudioDecStatus[AU_DEC_MAX];

    AUDIO_SOURCE_INFO_TYPE eAudioSource;
    AUDIO_INPUT_TYPE       eMainAudioSource;
    AUDIO_INPUT_TYPE       eSubAudioSource;
    AUDIO_INPUT_TYPE       eScartAudioSource;

    AUDIO_INIT_INFO        AudioSystemInfo;
    AUDIO_OUT_INFO         AudioOutputInfo;             // Store the connectivity of audio output device
    AUDIO_PATH_INFO        AudioPathInfo;               // Store the connectivity of audio DSP & output path
    OUTPUT_SOURCE_INFO     AudioOutputSourceInfo;       // Store the output port source setting

    DigitalOutSetting_t stDigitalOutSetting[DIGITAL_MAX_OUTPUT];
    Digital_Out_Channel_Status_t stDigitalOutChannelStatus[DIGITAL_MAX_OUTPUT];

    MS_U16  mainAudioDelay;
    MS_U8   ChipRevision;
    MS_U8   g_dolby_truehd_enable;

    AUDIO_CAPABILITIES AudioCaps[AU_DEC_MAX];

    Digital_Out_Device_Capability_t stDigitalOutCodecCapability[2][CODEC_MAX];

    MS_U8 g_PreMainDspCodeType;
    MS_BOOL DolbyAACFlag;
    MS_BOOL DolbyEncFlag;

    MS_U8   g_R2_NonPcm_Sel;
    MS_U8   g_DolbyDAPEnable;

    AUDIO_SPECIFIED_DIGITAL_OUTPUT_t g_AudioSpecifiedDigitalOutput;

    MS_BOOL  g_hbr_bypass_enable;

    MS_BOOL  g_R2ShmInitFlag;
} AUDIO_SHARED_VARS2;

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
 MS_BOOL MDrv_AUDIO_CheckBootOnInitState(void);
 MS_BOOL MDrv_AUDIO_Check_AsndR2_SupportStatus(void);
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
MS_S32 MDrv_AUDIO_InputSwitch(void);

//================================================================
// Audio VoIP Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2);

//================================================================
// Audio ALSA Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_ALSA_Check(void);


MS_U32 MDrv_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

//================================================================
// Audio Common PCM Relational Function
//================================================================
MS_S32 MDrv_AUDIO_PCM_Init(void);
MS_S32 MDrv_AUDIO_PCM_DeInit(void);
MS_S32 MDrv_AUDIO_PCM_Open(void *pData);
MS_S32 MDrv_AUDIO_PCM_Close(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Start(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32 MDrv_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32 MDrv_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32 MDrv_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32 MDrv_AUDIO_PCM_Flush(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_SwMixer_Server_Core(MS_S32 s32MixerServerId);
void * MDrv_AUDIO_PCM_SwMixer_Server_Thread(void *arg);


#ifdef __cplusplus
}
#endif

#endif // _DRV_AUDIO_IF_H_
