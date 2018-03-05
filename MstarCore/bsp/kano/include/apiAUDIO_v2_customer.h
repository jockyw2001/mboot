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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiAUDIO_v2_customer.h
/// @brief  AUDIO API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_AUDIO_CUSTOMER_H_
#define _API_AUDIO_CUSTOMER_H_

#include "MsCommon.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------------------------------------------
/* Initialize */
//-------------------------------------------------------------------------------------------------------------------------------------
//need to be consistent with AUDIO_OUTPUT_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_OUTPUT_TYPE_INVALID = -1,
    API_AUDIO_CUSTOMER_LINEOUT0_OUTPUT,              ///< 0: DAC0 OUTPUT
    API_AUDIO_CUSTOMER_LINEOUT1_OUTPUT,              ///< 1: T2=>AA0 OUTPUT    T3=>HP OUTPUT
    API_AUDIO_CUSTOMER_LINEOUT2_OUTPUT,              ///< 2: T2=>AA1 OUTPUT    T3=>DAC2 OUTPUT
    API_AUDIO_CUSTOMER_LINEOUT3_OUTPUT,              ///< 3: T2=>NULL          T3=>AA OUTPUT
    API_AUDIO_CUSTOMER_SPDIF_OUTPUT,                 ///< 4: SPDIF OUTPUT
    API_AUDIO_CUSTOMER_I2S_OUTPUT,                   ///< 5: I2S OUTPUT
    API_AUDIO_CUSTOMER_HP_OUTPUT,                    ///< 6: OUTPUT with headphone driver
    API_AUDIO_CUSTOMER_I2S2_OUTPUT,                  ///< 7: I2S2 OUTPUT
    API_AUDIO_CUSTOMER_HDMI_ARC_OUTPUT,              ///< 8: HDMI ARC OUTPUT(HDMI_Rx,TV)
    API_AUDIO_CUSTOMER_HDMI_OUTPUT,                  ///< 9: HDMI Tx OUTPUT(HDMI_Tx,STB)
    API_AUDIO_CUSTOMER_NULL_OUTPUT = 0xFF,           ///< 0xFF: Null output
    API_AUDIO_CUSTOMER_OUTPUT_MAX,
} API_AUDIO_CUSTOMER_OUTPUT_TYPE;

//need to be consistent with AUDIO_OUT_INFO of drvAudio_if.h
typedef struct
{
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   SpeakerOut;  ///< Audio output port for Speaker
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   HpOut;       ///< Audio output port for HP
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   MonitorOut;  ///< Audio output port for Monitor out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   ScartOut;    ///< Audio output port for Scart out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   SpdifOut;    ///< Audio output port for S/PDIF out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   ArcOut;      ///< Audio output port for ARC(HDMI_Rx) out
    API_AUDIO_CUSTOMER_OUTPUT_TYPE   HDMIOut;     ///< Audio output port for HDMI_Tx out
} API_AUDIO_CUSTOMER_OUTPUT_INFO;

typedef enum
{
    API_AUDIO_CUSTOMER_CHIP_PLATFORM_INVALID = -1,
    API_AUDIO_CUSTOMER_CHIP_PLATFORM_TV,
    API_AUDIO_CUSTOMER_CHIP_PLATFORM_STB,
} API_AUDIO_CUSTOMER_CHIP_PLATFORM;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Connect & Disconnect */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_PARSER_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_PARSER_MODE_0,
    API_AUDIO_CUSTOMER_PARSER_MODE_1,
    API_AUDIO_CUSTOMER_PARSER_MODE_2,
    API_AUDIO_CUSTOMER_PARSER_MODE_3,
    API_AUDIO_CUSTOMER_PARSER_MODE_MAX,
} API_AUDIO_CUSTOMER_PARSER_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_PARSER_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO0,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO1,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO2,
    API_AUDIO_CUSTOMER_PARSER_INPUT_AFIFO3,
    API_AUDIO_CUSTOMER_PARSER_INPUT_MAX,
} API_AUDIO_CUSTOMER_PARSER_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_INDEX_INVALID = -1,
    API_AUDIO_CUSTOMER_ADEC0,
    API_AUDIO_CUSTOMER_ADEC1,
    API_AUDIO_CUSTOMER_ADEC_ATV,
    API_AUDIO_CUSTOMER_ADEC_MAX,
} API_AUDIO_CUSTOMER_ADEC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_ADEC_INPUT_DTV,
    API_AUDIO_CUSTOMER_ADEC_INPUT_HDMI,
    API_AUDIO_CUSTOMER_ADEC_INPUT_MM,
    API_AUDIO_CUSTOMER_ADEC_INPUT_CLIP,
    API_AUDIO_CUSTOMER_ADEC_INPUT_ATV,
    API_AUDIO_CUSTOMER_ADEC_INPUT_NONE,
    API_AUDIO_CUSTOMER_ADEC_INPUT_MAX,
} API_AUDIO_CUSTOMER_ADEC_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_INVALID = -1,
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_DSP_SRC,
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_MCU,
    API_AUDIO_CUSTOMER_ADEC_PCM_PATH_MAX,
} API_AUDIO_CUSTOMER_ADEC_PCM_PATH;

typedef enum
{
    API_AUDIO_CUSTOMER_ADC_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_ADC0,
    API_AUDIO_CUSTOMER_ADC1,
    API_AUDIO_CUSTOMER_ADC_MAX,
} API_AUDIO_CUSTOMER_ADC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_ADC_IN_PORT_INVALID  = -1,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_0       = 0x02,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_1       = 0x12,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_2       = 0x22,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_3       = 0x32,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_4       = 0xA2,
    API_AUDIO_CUSTOMER_ADC0_IN_PORT_5       = 0xB2,
    API_AUDIO_CUSTOMER_ADC0_MIC_IN          = 0x72,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_0       = 0x09,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_1       = 0x19,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_2       = 0x29,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_3       = 0x39,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_4       = 0xA9,
    API_AUDIO_CUSTOMER_ADC1_IN_PORT_5       = 0xB9,
    API_AUDIO_CUSTOMER_ADC1_MIC_IN          = 0x79,
} API_AUDIO_CUSTOMER_ADC_IN_PORT;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_MIXER_INDEX_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_MIXER0,
    API_AUDIO_CUSTOMER_PCM_MIXER_MAX,
} API_AUDIO_CUSTOMER_PCM_MIXER_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX0,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX1,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX2,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX3,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX4,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX5,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX6,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_AMIX7,
    API_AUDIO_CUSTOMER_PCM_MIXER_INPUT_MAX,
} API_AUDIO_CUSTOMER_PCM_MIXER_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_CH_SOUND_INVALID = -1,
    API_AUDIO_CUSTOMER_CH5_SOUND,
    API_AUDIO_CUSTOMER_CH6_SOUND,
    API_AUDIO_CUSTOMER_CH7_SOUND,
    API_AUDIO_CUSTOMER_CH8_SOUND,
    API_AUDIO_CUSTOMER_CH_SOUND_MAX,
} API_AUDIO_CUSTOMER_CH_SOUND;

typedef enum
{
    API_AUDIO_CUSTOMER_CH_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC0,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC1,
    API_AUDIO_CUSTOMER_CH_INPUT_ADEC_ATV,
    API_AUDIO_CUSTOMER_CH_INPUT_HDMI,
    API_AUDIO_CUSTOMER_CH_INPUT_ADC0,
    API_AUDIO_CUSTOMER_CH_INPUT_ADC1,
    API_AUDIO_CUSTOMER_CH_INPUT_SCART,
    API_AUDIO_CUSTOMER_CH_INPUT_R2DMA_DSP1,
    API_AUDIO_CUSTOMER_CH_INPUT_R2DMA_DSP3,
    API_AUDIO_CUSTOMER_CH_INPUT_SWDMA_DSP3,
    API_AUDIO_CUSTOMER_CH_INPUT_HWDMA,
    API_AUDIO_CUSTOMER_CH_INPUT_NONE,
    API_AUDIO_CUSTOMER_CH_INPUT_MAX,
} API_AUDIO_CUSTOMER_CH_INPUT;

typedef  enum
{
    API_AUDIO_CUSTOMER_FWM_INVALID = -1,
    API_AUDIO_CUSTOMER_FWM0,
    API_AUDIO_CUSTOMER_FWM1,
    API_AUDIO_CUSTOMER_FWM2,
    API_AUDIO_CUSTOMER_FWM_MAX,
} API_AUDIO_CUSTOMER_FWM_INDEX;

typedef  enum
{
    API_AUDIO_CUSTOMER_FWM_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH5,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH6,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH7,
    API_AUDIO_CUSTOMER_FWM_INPUT_CH8,
    API_AUDIO_CUSTOMER_FWM_INPUT_MAX,
} API_AUDIO_CUSTOMER_FWM_INPUT;

typedef  enum
{
    API_AUDIO_CUSTOMER_SE_INVALID = -1,
    API_AUDIO_CUSTOMER_SE_PRER2,
    API_AUDIO_CUSTOMER_SE_DSPSE,
    API_AUDIO_CUSTOMER_SE_POSTR2,
    API_AUDIO_CUSTOMER_SE_MAX,
} API_AUDIO_CUSTOMER_SE_INDEX;

typedef  enum
{
    API_AUDIO_CUSTOMER_SE_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_SE_INPUT_DLY0,
    API_AUDIO_CUSTOMER_SE_INPUT_DLY1,
    API_AUDIO_CUSTOMER_SE_INPUT_MAX,
} API_AUDIO_CUSTOMER_SE_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_SOUNDOUT_INVALID = -1,
    API_AUDIO_CUSTOMER_SOUNDOUT_I2S,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT0,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT1,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT2,
    API_AUDIO_CUSTOMER_SOUNDOUT_LINEOUT3,
    API_AUDIO_CUSTOMER_SOUNDOUT_SPDIF,
    API_AUDIO_CUSTOMER_SOUNDOUT_HDMI_ARC,
    API_AUDIO_CUSTOMER_SOUNDOUT_HDMI_TX,
    API_AUDIO_CUSTOMER_SOUNDOUT_MAX,
} API_AUDIO_CUSTOMER_SOUNDOUT_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_NULL,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_CH7,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_FWM0,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DLY0,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_FWM1,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DLY1,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_FWM2,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DLY2,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_PRER2,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_DSPSE,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_POSTR2,
    API_AUDIO_CUSTOMER_SOUNDOUT_INPUT_MAX,
} API_AUDIO_CUSTOMER_SOUNDOUT_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE0,
    API_AUDIO_CUSTOMER_PCM_CAPTURE1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE2,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_MAX,
} API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH5,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH6,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH7,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_CH8,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_PCM,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_PCM_DELAY,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_PCM_SE,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_MIXER,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_FWM0,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_FWM1,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_FWM2,
    API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT_MAX,
} API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_MP3_ENC_INVALID = -1,
    API_AUDIO_CUSTOMER_MP3_ENC0,
    API_AUDIO_CUSTOMER_MP3_ENC_MAX,
} API_AUDIO_CUSTOMER_MP3_ENC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_CH5,
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_CH6,
    API_AUDIO_CUSTOMER_MP3_ENC_INPUT_MAX,
} API_AUDIO_CUSTOMER_MP3_ENC_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_AAC_ENC_INVALID = -1,
    API_AUDIO_CUSTOMER_AAC_ENC0,
    API_AUDIO_CUSTOMER_AAC_ENC_MAX,
} API_AUDIO_CUSTOMER_AAC_ENC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_INVALID = -1,
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_PCM_CAPTURE0,
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_PCM_CAPTURE1,
    API_AUDIO_CUSTOMER_AAC_ENC_INPUT_MAX,
} API_AUDIO_CUSTOMER_AAC_ENC_INPUT;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Start & Stop */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_CODEC_TYPE_INVALID           = -1,
    API_AUDIO_CUSTOMER_CODEC_TYPE_UNKNOWN           = 0,
    API_AUDIO_CUSTOMER_CODEC_TYPE_PCM               = 1, // for HDMI PCM
    API_AUDIO_CUSTOMER_CODEC_TYPE_AC3               = 2,
    API_AUDIO_CUSTOMER_CODEC_TYPE_EAC3              = 3,
    API_AUDIO_CUSTOMER_CODEC_TYPE_MPEG              = 4,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AAC               = 5,
    API_AUDIO_CUSTOMER_CODEC_TYPE_HEAAC             = 6,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DRA               = 7,
    API_AUDIO_CUSTOMER_CODEC_TYPE_MP3               = 8,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS               = 9,
    API_AUDIO_CUSTOMER_CODEC_TYPE_SIF               = 10,
    API_AUDIO_CUSTOMER_CODEC_TYPE_SIF_BTSC          = 11,
    API_AUDIO_CUSTOMER_CODEC_TYPE_SIF_A2            = 12,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DEFAULT           = 13,
    API_AUDIO_CUSTOMER_CODEC_TYPE_NONE              = 14,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS_HD_MA         = 15,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS_EXPRESS       = 16,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DTS_CD            = 17,
    API_AUDIO_CUSTOMER_CODEC_TYPE_WMA               = 18,
    API_AUDIO_CUSTOMER_CODEC_TYPE_WMA_PRO           = 19,
    API_AUDIO_CUSTOMER_CODEC_TYPE_XPCM              = 20,
    API_AUDIO_CUSTOMER_CODEC_TYPE_RA8LBR            = 21,
    API_AUDIO_CUSTOMER_CODEC_TYPE_FLAC              = 22,
    API_AUDIO_CUSTOMER_CODEC_TYPE_VORBIS            = 23,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AMR_NB            = 24,
    API_AUDIO_CUSTOMER_CODEC_TYPE_AMR_WB            = 25,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DolbyTrueHDBypass = 26,
    API_AUDIO_CUSTOMER_CODEC_TYPE_DVI               = 27, // for HDMI DVI
    API_AUDIO_CUSTOMER_CODEC_TYPE_ESBypass          = 28,
} API_AUDIO_CUSTOMER_CODEC_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_UPPER_PLAYCMD_INVALID = -1,
    API_AUDIO_CUSTOMER_UPPER_PLAY            = 0,
    API_AUDIO_CUSTOMER_UPPER_STOP            = 1,
    API_AUDIO_CUSTOMER_UPPER_PAUSE           = 2,
} API_AUDIO_CUSTOMER_UPPER_PLAYCMD;

//-------------------------------------------------------------------------------------------------------------------------------------
/* SPDIF */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_TX_MODE_INVALID   = -1,
    API_AUDIO_CUSTOMER_SPDIF_TX_NONE           = 0,
    API_AUDIO_CUSTOMER_SPDIF_TX_PCM            = 1,
    API_AUDIO_CUSTOMER_SPDIF_TX_AUTO           = 2,
    API_AUDIO_CUSTOMER_SPDIF_TX_BYPASS         = 3,
    API_AUDIO_CUSTOMER_SPDIF_TX_TRANSCODE      = 4,        /* Netflix */
    API_AUDIO_CUSTOMER_SPDIF_TX_DD             = 5,
} API_AUDIO_CUSTOMER_SPDIF_TX_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT_INVALID  = -1,
    API_AUDIO_CUSTOMER_SPDIF_COPY_FREE          = 0,    /* cp-bit : 1, L-bit : 0 */
    API_AUDIO_CUSTOMER_SPDIF_COPY_NO_MORE       = 1,    /* cp-bit : 0, L-bit : 1 */
    API_AUDIO_CUSTOMER_SPDIF_COPY_ONCE          = 2,    /* cp-bit : 0, L-bit : 0 */
    API_AUDIO_CUSTOMER_SPDIF_COPY_NEVER         = 3,    /* cp-bit : 0, L-bit : 1 */
} API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT;

//====== SPDIF OUTPUT CS TYPE =========
typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_CS_CategoryCode               = 0x0,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyRight                  = 0x1,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber               = 0x2,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber              = 0x3,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq                    = 0x4,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc                   = 0x5,      ///<
    API_AUDIO_CUSTOMER_SPDIF_ChannelAB                     = 0x6,      ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect                = 0x7,      ///<
} API_AUDIO_CUSTOMER_SPDIF_CS_TYPE;

//====== SPDIF OUTPUT CS Status =======
typedef enum
{
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast         = 0x00,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_General           = 0x01,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast_JP      = 0x02,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast_EU      = 0x03,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_BroadCast_USA     = 0x04,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_Rev4              = 0x05,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_Rev5              = 0x06,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_Category_Rev6              = 0x07,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyRight_CP               = 0x08,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyRight_NonCP            = 0x09,     ///<SPDIF_CS_ChannelNumber_Right
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_2             = 0x0A,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_5             = 0x0B,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_6             = 0x0C,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_Rev1          = 0x0D,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_Rev2          = 0x0E,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SourceNumber_Rev3          = 0x0F,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_A_Left       = 0x10,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_B_Right      = 0x11,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_C            = 0x12,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_D            = 0x13,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_E            = 0x14,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_F            = 0x15,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_G            = 0x16,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_H            = 0x17,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_I            = 0x18,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_J            = 0x19,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ChannelNumber_K            = 0x1A,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_192                = 0x1B,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_176                = 0x1C,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_128                = 0x1D,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_96                 = 0x1E,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_88                 = 0x1F,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_64                 = 0x20,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_48                 = 0x21,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_44                 = 0x22,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_32                 = 0x23,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_24                 = 0x24,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_22                 = 0x25,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_16                 = 0x26,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev1               = 0x27,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev2               = 0x28,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev3               = 0x29,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev4               = 0x2A,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_SmpFreq_Rev5               = 0x2B,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_Lev1              = 0x2C,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_Lev2              = 0x2D,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_Lev3              = 0x2E,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_ClockAcc_NotMatch          = 0x2F,     ///<
    API_AUDIO_CUSTOMER_SPDIF_ChannelAB_Invalid_ON          = 0x30,     ///<
    API_AUDIO_CUSTOMER_SPDIF_ChannelAB_Invalid_OFF         = 0x31,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect_CopyFree       = 0x32,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect_CopyOnce       = 0x33,     ///<
    API_AUDIO_CUSTOMER_SPDIF_CS_CopyProtect_CopyNever      = 0x34,     ///<
} API_AUDIO_CUSTOMER_SPDIF_CS_TYPE_STATUS;

//-------------------------------------------------------------------------------------------------------------------------------------
/* HDMI */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_INVALID    = -1,
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_PCM,                       ///< HDMI OUTPUT PCM
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_NONPCM,                    ///< HDMI OUTPUT non-PCM
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_8CH_NONPCM,                ///< HDMI OUTPUT non-PCM with 8 Channel output
    API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_EAC3,                      ///< HDMI OUTPUT DDP (MS12)
} API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* ATV */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_SIF_INPUT_INVALID    = -1,
    API_AUDIO_CUSTOMER_SIF_INPUT_EXTERNAL   = 0,
    API_AUDIO_CUSTOMER_SIF_INPUT_INTERNAL   = 1,
} API_AUDIO_CUSTOMER_SIF_INPUT;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_TYPE_INVALID         = -1,
    API_AUDIO_CUSTOMER_SIF_TYPE_NONE            = 0x0000,       ///< INIT TYPE : NONE
    API_AUDIO_CUSTOMER_SIF_ATSC_SELECT          = 0x0001,       ///< INIT TYPE : TV Systems for A2 enabled in default ATSC system
    API_AUDIO_CUSTOMER_SIF_KOREA_A2_SELECT      = 0x0002,       ///< INIT TYPE : TV Systems for A2 enabled in Korea A2 system
    API_AUDIO_CUSTOMER_SIF_BTSC_SELECT          = 0x0004,       ///< INIT TYPE : TV Systems for BTSC enabled in ATSC(CO, CF) or DVB(Taiwan) system
    API_AUDIO_CUSTOMER_SIF_BTSC_BR_SELECT       = 0x0008,       ///< INIT TYPE : TV Systems for BTSC enabled in ATSC(Brazil) system
    API_AUDIO_CUSTOMER_SIF_BTSC_US_SELECT       = 0x0010,       ///< INIT TYPE : TV Systems for BTSC enabled in ATSC(US) system
    API_AUDIO_CUSTOMER_SIF_DVB_SELECT           = 0x0020,       ///< INIT TYPE : TV Systems for EU in default DVB system
    API_AUDIO_CUSTOMER_SIF_DVB_ID_SELECT        = 0x0040,       ///< INIT TYPE : TV Systems for ID(Indonesia) in DVB(PAL B/G) system
    API_AUDIO_CUSTOMER_SIF_DVB_IN_SELECT        = 0x0080,       ///< INIT TYPE : TV Systems for IN(India) in DVB(PAL B) system
    API_AUDIO_CUSTOMER_SIF_DVB_CN_SELECT        = 0x0100,       ///< INIT TYPE : TV Systems for CN(China, Hong Kone) in DVB system
    API_AUDIO_CUSTOMER_SIF_DVB_AJJA_SELECT      = 0x0200,       ///< INIT TYPE : TV Systems for AJ(Asia JooDong), JA(JooAang Asia) in DVB system
    API_AUDIO_CUSTOMER_SIF_TYPE_MAX             = 0x0FFF,       ///< INIT TYPE : MAX
} API_AUDIO_CUSTOMER_SIF_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_STANDARD_INVALID = -1,
    API_AUDIO_CUSTOMER_SIF_BG               = 0x00,         ///< Audio standard BG
    API_AUDIO_CUSTOMER_SIF_BG_A2            = 0x01,         ///< Audio standard BG A2
    API_AUDIO_CUSTOMER_SIF_BG_NICAM         = 0x02,         ///< Audio standard BG NICAM
    API_AUDIO_CUSTOMER_SIF_I                = 0x03,         ///< Audio standard I
    API_AUDIO_CUSTOMER_SIF_DK               = 0x04,         ///< Audio standard DK
    API_AUDIO_CUSTOMER_SIF_DK1_A2           = 0x05,         ///< Audio standard DK1 A2
    API_AUDIO_CUSTOMER_SIF_DK2_A2           = 0x06,         ///< Audio standard DK2 A2
    API_AUDIO_CUSTOMER_SIF_DK3_A2           = 0x07,         ///< Audio standard DK3 A2
    API_AUDIO_CUSTOMER_SIF_DK_NICAM         = 0x08,         ///< Audio standard DK NICAM
    API_AUDIO_CUSTOMER_SIF_L                = 0x09,         ///< Audio standard L
    API_AUDIO_CUSTOMER_SIF_M                = 0x0A,         ///< Audio standard M
    API_AUDIO_CUSTOMER_SIF_M_BTSC           = 0x0B,         ///< Audio standard M BTSC
    API_AUDIO_CUSTOMER_SIF_M_A2             = 0x0C,         ///< Audio standard M A2
    API_AUDIO_CUSTOMER_SIF_M_EIA_J          = 0x0D,         ///< Audio standard M EIA J
    API_AUDIO_CUSTOMER_SIF_NOTSTANDARD      = 0x0F          ///< Not Audio standard
} API_AUDIO_CUSTOMER_SIF_STANDARD;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_MODE_SET_INVALID             =     -1,
    API_AUDIO_CUSTOMER_SIF_SET_PAL_MONO                 =   0x00,   // PAL Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_MONO_FORCED          =   0x01,   // PAL Mono Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_STEREO               =   0x02,   // PAL Stereo
    API_AUDIO_CUSTOMER_SIF_SET_PAL_STEREO_FORCED        =   0x03,   // PAL Stereo Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_DUALI                =   0x04,   // PAL Dual I
    API_AUDIO_CUSTOMER_SIF_SET_PAL_DUALII               =   0x05,   // PAL Dual II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_DUALI_II             =   0x06,   // PAL Dual I+II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_MONO           =   0x07,   // PAL NICAM Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_MONO_FORCED    =   0x08,   // PAL NICAM Mono Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_STEREO         =   0x09,   // PAL NICAM Stereo
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_STEREO_FORCED  =   0x0A,   // PAL NICAM Stereo Force Mono
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUALI          =   0x0B,   // PAL NICAM Dual I
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUALII         =   0x0C,   // PAL NICAM Dual II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUALI_II       =   0x0D,   // PAL NICAM Dual I+II
    API_AUDIO_CUSTOMER_SIF_SET_PAL_NICAM_DUAL_FORCED    =   0x0E,   // PAL NICAM Dual Forced Mono(Not Supported)
    API_AUDIO_CUSTOMER_SIF_SET_PAL_UNKNOWN              =   0x0F,   // PAL Unkown State
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_MONO             =   0x10,   // NTSC(A2) Mono
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_STEREO           =   0x11,   // NTSC(A2) Stereo
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_SAP              =   0x12,   // NTSC(A2) SAP
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_A2_UNKNOWN          =   0x13,   // NTSC(A2) Unkown State
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_MONO           =   0x14,   // NTSC(BTSC) Mono
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_STEREO         =   0x15,   // NTSC(BTSC) Stereo
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_SAP_MONO       =   0x16,   // NTSC(BTSC) SAP Mono
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_SAP_STEREO     =   0x17,   // NTSC(BTSC) SAP Stereo
    API_AUDIO_CUSTOMER_SIF_SET_NTSC_BTSC_UNKNOWN        =   0x18,   // NTSC(BTSC) Unkown State
} API_AUDIO_CUSTOMER_SIF_MODE_SET;

typedef enum
{
    API_AUDIO_CUSTOMER_SIF_HHIDEV_FILTER_BW1        = 0x10, // 220kHz~315kHz
    API_AUDIO_CUSTOMER_SIF_HHIDEV_FILTER_BW2        = 0x20, // 320kHz~415kHz
    API_AUDIO_CUSTOMER_SIF_HHIDEV_FILTER_BW3        = 0x30, // 400kHz~495kHz
} API_AUDIO_CUSTOMER_SIF_HIDEV_BW;

///< Audio mode type
typedef enum
{
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_INVALID        = 0x00,           ///< Audio Mode Invalid
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_MONO           = 0x01,           ///< Audio Mode MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_FORCED_MONO    = 0x02,           ///< Audio Mode Forced MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_G_STEREO       = 0x03,           ///< Audio Mode G Stereo
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_K_STEREO       = 0x04,           ///< Audio Mode  K Stereo
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_MONO_SAP       = 0x05,           ///< Audio Mode MONO SAP
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_STEREO_SAP     = 0x06,           ///< Audio Mode Stereo SAP
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_DUAL_A         = 0x07,           ///< Audio Mode Dual A
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_DUAL_B         = 0x08,           ///< Audio Mode Dual B
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_DUAL_AB        = 0x09,           ///< Audio Mode Dual AB
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_MONO     = 0x0A,           ///< Audio Mode NICAM MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_STEREO   = 0x0B,           ///< Audio Mode NICAM Stereo
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_DUAL_A   = 0x0C,           ///< Audio Mode NICAM DUAL A
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_DUAL_B   = 0x0D,           ///< Audio Mode NICAM DUAL B
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_NICAM_DUAL_AB  = 0x0E,           ///< Audio Mode NICAM DUAL AB
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_HIDEV_MONO     = 0x0F,           ///< Audio Mode HIDEV MONO
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_LEFT_LEFT      = 0x10,           ///< Audio Mode Left left
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_RIGHT_RIGHT    = 0x11,           ///< Audio Mode right right
    API_AUDIO_CUSTOMER_SIF_AUDIOMODE_LEFT_RIGHT     = 0x12            ///< Audio Mode left right
} API_AUDIO_CUSTOMER_SIF_AUDIOMODE_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Decoder */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_DOLBY_DRC_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_DOLBY_LINE_MODE,
    API_AUDIO_CUSTOMER_DOLBY_RF_MODE,
} API_AUDIO_CUSTOMER_DOLBY_DRC_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_DOWNMIX_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_LTRT_MODE,
    API_AUDIO_CUSTOMER_LORO_MODE,
} API_AUDIO_CUSTOMER_DOWNMIX_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_DUALMONO_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_LR,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_LL,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_RR,
    API_AUDIO_CUSTOMER_DUALMONO_MODE_MIX,
} API_AUDIO_CUSTOMER_DUALMONO_MODE;

typedef enum
{
    API_AUDIO_CUSTOMER_IN_PORT_INVALID  = -1,
    API_AUDIO_CUSTOMER_IN_PORT_NONE     =  0,
    API_AUDIO_CUSTOMER_IN_PORT_TP       =  1,   // From TPA Stream Input
    API_AUDIO_CUSTOMER_IN_PORT_SPDIF    =  2,   // From SERIAL INTERFACE 0
    API_AUDIO_CUSTOMER_IN_PORT_SIF      =  3,   // From Analog Front End (SIF)
    API_AUDIO_CUSTOMER_IN_PORT_ADC      =  4,   // Fron ADC Input
    API_AUDIO_CUSTOMER_IN_PORT_HDMI     =  5,   // From HDMI
    API_AUDIO_CUSTOMER_IN_PORT_I2S      =  6,   // From I2S
    API_AUDIO_CUSTOMER_IN_PORT_SYSTEM   =  7,   // From System
} API_AUDIO_CUSTOMER_IN_PORT;

typedef enum
{
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_INVALID    =       -1,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_NONE       =        0,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_4_KHZ      =     4000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_8_KHZ      =     8000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_11_025KHZ  =    11025,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_12_KHZ     =    12000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_16_KHZ     =    16000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_22_05KHZ   =    22050,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_24_KHZ     =    24000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_32_KHZ     =    32000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_44_1KHZ    =    44100,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_48_KHZ     =    48000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_64_KHZ     =    64000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_88_2KHZ    =    88200,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_96_KHZ     =    96000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_128_KHZ    =   128000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_176_4KHZ   =   176400,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_192_KHZ    =   192000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_768_KHZ    =   768000,
    API_AUDIO_CUSTOMER_SAMPLING_FREQ_DEFAULT    =   999000,
} API_AUDIO_CUSTOMER_SAMPLING_FREQ;

typedef enum
{
    API_AUDIO_CUSTOMER_HDMI_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_HDMI0                = 0,
    API_AUDIO_CUSTOMER_HDMI1                = 1,
    API_AUDIO_CUSTOMER_HDMI2                = 2,
    API_AUDIO_CUSTOMER_HDMI3                = 3,
    API_AUDIO_CUSTOMER_HDMI_SWITCH          = 4,
    API_AUDIO_CUSTOMER_HDMI_MAX             = API_AUDIO_CUSTOMER_HDMI_SWITCH,
} API_AUDIO_CUSTOMER_HDMI_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_CHANNEL_MODE_INVALID             = -1,
    API_AUDIO_CUSTOMER_CH_MODE_MONO                     = 0,
    API_AUDIO_CUSTOMER_CH_MODE_JOINT_STEREO             = 1,
    API_AUDIO_CUSTOMER_CH_MODE_STEREO                   = 2,
    API_AUDIO_CUSTOMER_CH_MODE_DUAL_MONO                = 3,
    API_AUDIO_CUSTOMER_CH_MODE_MULTI                    = 4,
    API_AUDIO_CUSTOMER_CH_MODE_UNKNOWN                  = 5,
    API_AUDIO_CUSTOMER_CH_MODE_2_1_FL_FR_LFE            = 6,
    API_AUDIO_CUSTOMER_CH_MODE_3_0_FL_FR_RC             = 7,
    API_AUDIO_CUSTOMER_CH_MODE_3_1_FL_FR_RC_LFE         = 8,
    API_AUDIO_CUSTOMER_CH_MODE_4_0_FL_FR_RL_RR          = 9,
    API_AUDIO_CUSTOMER_CH_MODE_4_1_FL_FR_RL_RR_LFE      = 10,
    API_AUDIO_CUSTOMER_CH_MODE_5_0_FL_FR_FC_RL_RR       = 11,
    API_AUDIO_CUSTOMER_CH_MODE_5_1_FL_FR_FC_RL_RR_LFE   = 12,
} API_AUDIO_CUSTOMER_CHANNEL_MODE;

typedef struct
{
    MS_U8 version;              /* AAC = 0x0, HE-AACv1 = 0x1, HE-AACv2 = 0x2 */
    MS_U8 transmissionformat;   /* LOAS/LATM = 0x0, ADTS = 0x1*/
    MS_U8 channelNum;
    API_AUDIO_CUSTOMER_CHANNEL_MODE channeMode;
} API_AUDIO_CUSTOMER_HEAAC_ES_INFO;

typedef struct
{
    MS_U8 bitRate;
    MS_U8 sampleRate;
    MS_U8 layer;
    MS_U8 channelNum;
    API_AUDIO_CUSTOMER_CHANNEL_MODE channeMode;
} API_AUDIO_CUSTOMER_MPEG_ES_INFO;

typedef struct
{
    MS_U8 bitRate;
    MS_U8 sampleRate;
    MS_U8 channelNum;
    MS_U8 EAC3; /* AC3 0x0, EAC3 0x1*/
    API_AUDIO_CUSTOMER_CHANNEL_MODE channeMode;
} API_AUDIO_CUSTOMER_AC3_ES_INFO;

//need to be consistent with XPCM_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_XPCM_LPCM            = 1,
    API_AUDIO_CUSTOMER_XPCM_MS_ADPCM        = 2,
    API_AUDIO_CUSTOMER_XPCM_G711_A_LAW      = 6,
    API_AUDIO_CUSTOMER_XPCM_G711_u_LAW      = 7,
    API_AUDIO_CUSTOMER_XPCM_DVD_LPCM        = 8,
    API_AUDIO_CUSTOMER_XPCM_PRIVATE1_LPCM   = 9,
    API_AUDIO_CUSTOMER_XPCM_IMA_ADPCM       = 17,
    API_AUDIO_CUSTOMER_XPCM_IMA_ADPCM_APPLE = 18, // apple IMA4
} API_AUDIO_CUSTOMER_XPCM_TYPE;

//need to be consistent with Audio_XPCM_Param of drvAudio_if.h
typedef struct
{
    API_AUDIO_CUSTOMER_XPCM_TYPE audioType;
    MS_U32  sampleRate;
    MS_U16  blockSize;
    MS_U16  samplePerBlock;
    MS_U8   channels;
    MS_U8   bitsPerSample;
} API_AUDIO_CUSTOMER_XPCM_PARAM;

//need to be consistent with Audio_COOK_Param of drvAudio_if.h
typedef struct
{
    #define COOK_MAX_NUM_CODECS 5
    MS_U16 Channels[COOK_MAX_NUM_CODECS];
    MS_U16 Regions[COOK_MAX_NUM_CODECS];
    MS_U16 cplStart[COOK_MAX_NUM_CODECS];
    MS_U16 cplQbits[COOK_MAX_NUM_CODECS];
    MS_U16 FrameSize[COOK_MAX_NUM_CODECS];
    MS_U16 mNumCodecs;
    MS_U16 mSamples;
    MS_U16 mSampleRate;
} API_AUDIO_CUSTOMER_COOK_PARM;

typedef enum
{
    API_AUDIO_CUSTOMER_TRICK_MODE_INVALID       = -1,
    API_AUDIO_CUSTOMER_TRICK_NONE               = 0,    ///<  rate : None, TP Live Play
    API_AUDIO_CUSTOMER_TRICK_PAUSE              = 1,    ///<  rate : Pause, DVR Play
    API_AUDIO_CUSTOMER_TRICK_NORMAL_PLAY        = 2,    ///<  rate : Normal Play, DVR Play
    API_AUDIO_CUSTOMER_TRICK_SLOW_MOTION_0P25X  = 3,    ///<  rate : 0.25 Play
    API_AUDIO_CUSTOMER_TRICK_SLOW_MOTION_0P50X  = 4,    ///<  rate : 0.50 Play
    API_AUDIO_CUSTOMER_TRICK_SLOW_MOTION_0P80X  = 5,    ///<  rate : 0.80 Play
    API_AUDIO_CUSTOMER_TRICK_FAST_FORWARD_1P20X = 6,    ///<  rate : 1.20 Play
    API_AUDIO_CUSTOMER_TRICK_FAST_FORWARD_1P50X = 7,    ///<  rate : 1.50 Play
    API_AUDIO_CUSTOMER_TRICK_FAST_FORWARD_2P00X = 8,    ///<  rate : 2.00 Play
    API_AUDIO_CUSTOMER_TRICK_ONE_FRAME_DECODE   = 9,    ///<  rate : one frame decode
} API_AUDIO_CUSTOMER_TRICK_MODE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Common Decoder */
//-------------------------------------------------------------------------------------------------------------------------------------
//need to be consistent with AUDIO_DECODER_INFO_VERSION of drvAudio_if.h
#define API_AUDIO_CUSTOMER_COMMON_DECODER_INFO_VERSION 0x00010001

//need to be consistent with AUDIO_DECODER_Command_t of drvAudio_if.h
typedef struct
{
    MS_U8  CmdName[64];
    MS_U32 CmdLen;
} API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND;

//need to be consistent with AUDIO_DECODER_SOURCE_TYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_NONE,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_DTV,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_ATV,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_HDMI,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_ADC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_MM,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE_SPDIF,
} API_AUDIO_CUSTOMER_COMMON_DECODER_SOURCE;

//need to be consistent with AUDIO_DECODER_PLAY_MODE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_FRAME_HAND_SHAKE,   // Gstreamer , OMX
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_FILE_TS,            // Send data to HW demux
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_ES_WITH_TS,         // MM new mode
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_ES,                 // MM old mode
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_DTV,                //
    API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE_OTHER,              // HDMI
} API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE;

//need to be consistent with AUDIO_DECODER_AV_SYNC_MODE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE_AV_SYNC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE_FREE_RUN,
} API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE;

//need to be consistent with AUDIO_DECODER_CODECTYPE of drvAudio_if.h
typedef enum
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_INVALID = -1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_PCM,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_XPCM,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_MPEG,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_MP3,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AC3,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AC3P,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AAC_LC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_HEAAC_V1,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_HEAAC_V2,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DOLBYPULSE,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_WMA,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_WMAPRO,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_RA8,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DTS,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_FLAC,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_VORBIS,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AMR_NB,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_AMR_WB,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DRA,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DTSLBR,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_DTSXLL,
    API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE_NULL = 0xFFFFFFFF
} API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE;

//need to be consistent with Audio_Ringbuf of drvAudio_if.h
typedef struct
{
    MS_U32 u32BufStartAddr;
    MS_U32 u32BufEndAddr;
    MS_U32 u32BufWrPtr;
    MS_U32 u32BufRdPtr;
    MS_U32 u32BufTotalSize;
    MS_U32 u32BufOffset;
    MS_U32 u32Size;
    MS_U32 u32CurLevel;
} API_AUDIO_CUSTOMER_COMMON_DECODER_RINGBUFFER;

//need to be consistent with AUDIO_DECODER_MM_Info_t of drvAudio_if.h
typedef struct
{
    API_AUDIO_CUSTOMER_COMMON_DECODER_RINGBUFFER  EsCtrl;  /* ES  buf control */
    API_AUDIO_CUSTOMER_COMMON_DECODER_RINGBUFFER  PcmCtrl; /* PCM buf control */

    /*Input ES related*/
    MS_U32 u32NeedDecodeFrmCnt;  /* Need decode frame count */
    MS_U64 u64TimeStamp;         /* Time stamp */

    /*Output PCM related*/
    MS_U32 u32OutputFrameCnt;    /* Decoder decode done frame count */
    MS_U32 u32OutputFrameCntAck; /* Inform Decoder output frame count */
} API_AUDIO_CUSTOMER_COMMON_DECODER_MM_INFO;

//need to be consistent with AUDIO_DECODER_Info_t of drvAudio_if.h
typedef struct
{
    char u8Name[32];            /* The DECODER name requested (only 32 bytes, if change this length, must change u8Name of API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND as well!) */
    MS_U32 u32StructVersion;    /* version number of this data structure */
    MS_U32 u32StructSize;       /* size of this data structure */

    MS_U32 u32Connect;          /* current connect status*/
    MS_U32 u32CodecType;        /* Set Decoder format ex, AC3 MP3 AAC DTS etc. please refer to API_AUDIO_CUSTOMER_COMMON_DECODER_CODECTYPE */
    MS_U32 u32SourceType;       /* Set Audio Source, ex, DTV / HDMI /MM etc. */
    MS_U32 u32OutputGroup;      /* Set Decoder Output Group */
    MS_U32 u32PlayMode;         /* Decoder Playback mode, please refer to API_AUDIO_CUSTOMER_COMMON_DECODER_PLAY_MODE */
    MS_U32 u32AfifoSource;      /* Set AUDIO FIFO */
    MS_U32 u32AdEnable;         /* Enable AD */
    MS_U32 u32AvSyncMode;       /* DECODER AV SYNC MODE, please refer to API_AUDIO_CUSTOMER_COMMON_DECODER_SYNC_MODE */

    MS_U32 u32MadBaseAddr;
    MS_U32 u32Endian;
    MS_U32 u32Channels;
    MS_U32 u32BitsPerSample;
    MS_U32 u32SamplingRate;
    MS_U32 u32BitRate;

    API_AUDIO_CUSTOMER_COMMON_DECODER_MM_INFO tDecoderMMInfo;
    MS_U32 *pCodecExtraData;        /* Extra data pointer if necessary */
    MS_U32 u32CodecExtraDataSize;   /* Extra data size */
} API_AUDIO_CUSTOMER_COMMON_DECODER_INFO;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Common */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_PARAM_TYPE_INVALID                       = -1,

    //--------------------------------------------------------------------------------------
    // Decoder (API_AUDIO_CUSTOMER_"ADEC"_PARAM_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---MPEG---
    API_AUDIO_CUSTOMER_ADEC_PARAM_MPEG_SOUND_MODE               = 0,

    //---Dolby AC3P---
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DRC_MODE                 = 1,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DOWNMIX_MODE             = 2,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DOLBY_BULLETIN11         = 3,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_HDMITX_BYPASS_ENABLE     = 4,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DRC_HIGHCUT_SCALE        = 5,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AC3P_DRC_LOWBOOT_SCALE        = 6,

    //---AAC---
    API_AUDIO_CUSTOMER_ADEC_PARAM_AAC_DRC_MODE                  = 7,
    API_AUDIO_CUSTOMER_ADEC_PARAM_AAC_DOWNMIX_MODE              = 8,

    //---WMA---
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_VERSION                   = 9,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_CHANNELS                  = 10,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_SAMPLERATE                = 11,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_BYTERATE                  = 12,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_BLOCKALIGN                = 13,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_ENCOPT                    = 14,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_PARSINGBYAPP              = 15,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_BITS_PER_SAMPLE           = 16,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_CHANNELMASK               = 17,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_PARAM_EXIST           = 18,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_RMS_AMP_REF           = 19,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_RMS_AMP_TARGET        = 20,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_PEAK_AMP_REF          = 21,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_DRC_PEAK_AMP_TARGET       = 22,
    API_AUDIO_CUSTOMER_ADEC_PARAM_WMA_MAX_PACKET_SIZE           = 23,

    //---XPCM---
    API_AUDIO_CUSTOMER_ADEC_PARAM_XPCM                          = 24,

    //---Underrun threshold in ms, Debounce Cnt---
    API_AUDIO_CUSTOMER_ADEC_UNDERRUN_THRESHOLD_IN_MS            = 26,
    API_AUDIO_CUSTOMER_ADEC_UNDERRUN_DEBOUNCE_CNT               = 27, //<--MAX is here!!!

    //--------------------------------------------------------------------------------------
    // Common (API_AUDIO_CUSTOMER_"COMMON"_PARAM_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---RESERVE---
    API_AUDIO_CUSTOMER_COMMON_PARAM_RESERVE0                    = 25,

    //---MAX---
    API_AUDIO_CUSTOMER_PARAM_MAX = API_AUDIO_CUSTOMER_ADEC_UNDERRUN_DEBOUNCE_CNT,
} API_AUDIO_CUSTOMER_PARAM_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMER_ADEC_INFO_TYPE_INVALID                   = -1,

    //--------------------------------------------------------------------------------------
    // Decoder (API_AUDIO_CUSTOMER_"ADEC"_INFO_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---MPEG---
    API_AUDIO_CUSTOMER_ADEC_INFO_MPEG_SOUND_MODE                    = 0,

    //---Dolby AC3P---
    API_AUDIO_CUSTOMER_ADEC_INFO_AC3P_DRC_MODE                      = 1,
    API_AUDIO_CUSTOMER_ADEC_INFO_AC3P_DOWNMIX_MODE                  = 2,

    //---AAC---
    API_AUDIO_CUSTOMER_ADEC_INFO_AAC_DRC_MODE                       = 3,
    API_AUDIO_CUSTOMER_ADEC_INFO_AAC_DOWNMIX_MODE                   = 4,

    //---Decode Info---
    API_AUDIO_CUSTOMER_ADEC_INFO_SAMPLERATE                         = 9,
    API_AUDIO_CUSTOMER_ADEC_INFO_ACMODE                             = 10,
    API_AUDIO_CUSTOMER_ADEC_INFO_AVSYNC_STATE                       = 11,

    //--------------------------------------------------------------------------------------
    // Common (API_AUDIO_CUSTOMER_"COMMON"_INFO_XXXXXXXX)
    //--------------------------------------------------------------------------------------
    //---ES---
    API_AUDIO_CUSTOMER_COMMON_INFO_ES_BUFFER_BASE                   = 5,
    API_AUDIO_CUSTOMER_COMMON_INFO_ES_BUFFER_SIZE                   = 6,

    //---PCM---
    API_AUDIO_CUSTOMER_COMMON_INFO_PCM_BUFFER_CURRENT_LEVEL         = 7,
    API_AUDIO_CUSTOMER_COMMON_INFO_PCM_BUFFER_CURRENT_LEVEL_IN_MS   = 12, //<--MAX is here!!!

    //---PTS---
    API_AUDIO_CUSTOMER_COMMON_INFO_33BIT_PTS                        = 8,

    //---MAX---
    API_AUDIO_CUSTOMER_INFO_MAX = API_AUDIO_CUSTOMER_COMMON_INFO_PCM_BUFFER_CURRENT_LEVEL_IN_MS,
} API_AUDIO_CUSTOMER_INFO_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Customized patch */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_TYPE_INVALID = -1,

    //--------------------------------------------------------------------------------------
    // (API_AUDIO_"CUSTOMIZED_PATCH"_PARAM_XXXXXXXX), PARAM: Set
    //--------------------------------------------------------------------------------------
    //---ES Passthrough---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_DELAY_SAMPLE        = 0,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_PCM_SAMPLERATE      = 1,
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_HDMI_SAMPLERATE     = 2, //<--MAX is here!!!

    //---MAX---
    API_AUDIO_CUSTOMIZED_PATCH_PARAM_MAX = API_AUDIO_CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH_HDMI_SAMPLERATE,
} API_AUDIO_CUSTOMIZED_PATCH_PARAM_TYPE;

typedef enum
{
    API_AUDIO_CUSTOMIZED_PATCH_INFO_TYPE_INVALID = -1,

    //--------------------------------------------------------------------------------------
    // (API_AUDIO_"CUSTOMIZED_PATCH"_INFO_XXXXXXXX), INFO: Get
    //--------------------------------------------------------------------------------------
    //---RESERVE---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_RESERVE0        = 0,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_RESERVE1        = 1,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_RESERVE2        = 2,
    API_AUDIO_CUSTOMIZED_PATCH_INFO_RESERVE3        = 3, //<--MAX is here!!!

    //---MAX---
    API_AUDIO_CUSTOMIZED_PATCH_INFO_MAX = API_AUDIO_CUSTOMIZED_PATCH_INFO_RESERVE3,
} API_AUDIO_CUSTOMIZED_PATCH_INFO_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Clip Play for ES */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    API_AUDIO_CUSTOMER_CODEC_TYPE   clipType;
    MS_U32                          repeatNumber;       /* The play number of audio clip. */
} API_AUDIO_CUSTOMER_CLIP_DEC_PARAM;

typedef enum
{
    API_AUDIO_CUSTOMER_CLIP_STATUS_INVALID  = -1,
    API_AUDIO_CUSTOMER_CLIP_NONE            = 0,
    API_AUDIO_CUSTOMER_CLIP_PLAY            = 1,
    API_AUDIO_CUSTOMER_CLIP_STOP            = 2,
    API_AUDIO_CUSTOMER_CLIP_RESUME          = 3,
    API_AUDIO_CUSTOMER_CLIP_PAUSE           = 4,
    API_AUDIO_CUSTOMER_CLIP_DONE            = 5,
} API_AUDIO_CUSTOMER_CLIP_STATUS;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Clip Play for PCM */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_MIXER_INDEX_INVALID  = -1,
    API_AUDIO_CUSTOMER_MIXER0               = 0,
    API_AUDIO_CUSTOMER_MIXER1               = 1,
    API_AUDIO_CUSTOMER_MIXER2               = 2,
    API_AUDIO_CUSTOMER_MIXER3               = 3,
    API_AUDIO_CUSTOMER_MIXER4               = 4,
    API_AUDIO_CUSTOMER_MIXER5               = 5,
    API_AUDIO_CUSTOMER_MIXER6               = 6,
    API_AUDIO_CUSTOMER_MIXER7               = 7,
    API_AUDIO_CUSTOMER_MIXER_MAX            = API_AUDIO_CUSTOMER_MIXER7,
} API_AUDIO_CUSTOMER_MIXER_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_ENDIAN_INVALID   = -1,
    API_AUDIO_CUSTOMER_PCM_LITTLE_ENDIAN    = 0,
    API_AUDIO_CUSTOMER_PCM_BIG_ENDIAN       = 1,
} API_AUDIO_CUSTOMER_PCM_ENDIAN;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_SIGNED_INVALID   = -1,
    API_AUDIO_CUSTOMER_PCM_SIGNED           = 0,
    API_AUDIO_CUSTOMER_PCM_UNSIGNED         = 1,
} API_AUDIO_CUSTOMER_PCM_SIGNED_STATUS;

typedef struct
{
    MS_U32                                  numOfChannel;       /* 2  : stereo, 1 : mono,  8 : 8 channel */
    MS_U32                                  bitPerSample;       /* 16 : 16 bit, 8 : 8 bit 24 : 24bit */
    API_AUDIO_CUSTOMER_SAMPLING_FREQ        samplingFreq;       /* 48000 : 48Khz, 44100 : 44.1Khz */
    API_AUDIO_CUSTOMER_PCM_ENDIAN           endianType;         /* 0  : little endian, 1 : big endian */
    API_AUDIO_CUSTOMER_PCM_SIGNED_STATUS    signedType;         /* 0  : signed PCM, 1 : unsigned PCM */
    MS_U32                                  repeatNumber;       /* The play number of audio clip. */
} API_AUDIO_CUSTOMER_CLIP_MIX_PARAM;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Gain, Mute & Delay */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_LINE_OUT_INVALID    = -1,
    API_AUDIO_CUSTOMER_LINE_OUT_0          = 0,
    API_AUDIO_CUSTOMER_LINE_OUT_1          = 1,
    API_AUDIO_CUSTOMER_LINE_OUT_2          = 2,
    API_AUDIO_CUSTOMER_LINE_OUT_3          = 3,
    API_AUDIO_CUSTOMER_LINE_OUT_MAX        = API_AUDIO_CUSTOMER_LINE_OUT_3,
} API_AUDIO_CUSTOMER_LINE_OUT_IDX;

//-------------------------------------------------------------------------------------------------------------------------------------
/* SPK Out LR Mode */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_INVALID    = -1,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_LR         = 0,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_LL         = 1,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_RR         = 2,
    API_AUDIO_CUSTOMER_SNDOUT_LRMODE_MIX        = 3,
} API_AUDIO_CUSTOMER_SNDOUT_LRMODE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* AENC */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_AENC_INDEX_INVALID   = -1,
    API_AUDIO_CUSTOMER_AENC0                = 0,
    API_AUDIO_CUSTOMER_AENC1                = 1,
    API_AUDIO_CUSTOMER_AENC_MAX             = API_AUDIO_CUSTOMER_AENC1,
} API_AUDIO_CUSTOMER_AENC_INDEX;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_STATUS_INVALID      = -1,
    API_AUDIO_CUSTOMER_AENC_STATUS_STOP         = 0,
    API_AUDIO_CUSTOMER_AENC_STATUS_PLAY         = 1,
    API_AUDIO_CUSTOMER_AENC_STATUS_ABNORMAL     = 2,
} API_AUDIO_CUSTOMER_AENC_STATUS;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT_INVALID = -1,
    API_AUDIO_CUSTOMER_AENC_ENCODE_MP3              = 0,    /* Encode MP3 format */
    API_AUDIO_CUSTOMER_AENC_ENCODE_AAC              = 1,    /* Encode AAC format */
} API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_CHANNEL_INVALID = -1,
    API_AUDIO_CUSTOMER_AENC_MONO            = 0,
    API_AUDIO_CUSTOMER_AENC_STEREO          = 1,
} API_AUDIO_CUSTOMER_AENC_CHANNEL;

typedef enum
{
    API_AUDIO_CUSTOMER_AENC_BITRATE_INVALID = -1,
    API_AUDIO_CUSTOMER_AENC_BIT_48K         = 0,
    API_AUDIO_CUSTOMER_AENC_BIT_56K         = 1,
    API_AUDIO_CUSTOMER_AENC_BIT_64K         = 2,
    API_AUDIO_CUSTOMER_AENC_BIT_80K         = 3,
    API_AUDIO_CUSTOMER_AENC_BIT_112K        = 4,
    API_AUDIO_CUSTOMER_AENC_BIT_128K        = 5,
    API_AUDIO_CUSTOMER_AENC_BIT_160K        = 6,
    API_AUDIO_CUSTOMER_AENC_BIT_192K        = 7,
    API_AUDIO_CUSTOMER_AENC_BIT_224K        = 8,
    API_AUDIO_CUSTOMER_AENC_BIT_256K        = 9,
    API_AUDIO_CUSTOMER_AENC_BIT_320K        = 10,
} API_AUDIO_CUSTOMER_AENC_BITRATE;

typedef struct
{
    //Get Info for debugging on DDI
    API_AUDIO_CUSTOMER_AENC_STATUS            status;   // current ENC Status
    API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT   codec;    // current ENC Codec

    MS_U32  errorCount;                                 // current ENC error counter
    MS_U32  inputCount;                                 // current ENC input counter - we distinguish whether or not enter data from input.
    MS_U32  underflowCount;                             // current ENC underflowCnt in kernel space - we distinguish which module have problem between muxer and encdoer
    MS_U32  overflowCount;                              // current ENC overflowCnt - we distinguish

    //Set Info - it is applied realtime, no matter stop&start
    API_AUDIO_CUSTOMER_AENC_CHANNEL   channel;          // number of channel
    API_AUDIO_CUSTOMER_AENC_BITRATE   bitrate;          // bitrate
} API_AUDIO_CUSTOMER_AENC_INFO;

//-------------------------------------------------------------------------------------------------------------------------------------
/* PCM(Sound Bar Buletooth, PCM Capture) */
//-------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------
/* PCM IO */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Name[32];           /* The PCM name requested (only 32 bytes, if change this length, must change u8Name of AUDIO_PCM_INFO_t as well!) */
    MS_U8 u8MultiChFlag;        /* it is TRUE when request for a multi-channel PCM IO */
    MS_U32 u32BufferDuration;   /* PCM buffer size in mini-second unit, and up to a maximun size */
    MS_U32 u32Channel;          /* PCM channel number */
    MS_U32 u32SampleRate;       /* PCM sample rate */
    MS_U32 u32BitWidth;         /* PCM sample's bit width of each channel */
    MS_U32 u32BigEndian;        /* PCM endien, TRUE means Big Endien, FALSE means Little Endien */
} API_AUDIO_CUSTOMER_PCMIO_PARAM;

typedef enum
{
    API_AUDIO_CUSTOMER_PCM_CMD_ALL,             /*       GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_NONBLOCKING,     /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_MULTICH,         /*       GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_MIXING,          /*       GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_MIXINGGROUP,     /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFER,          /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFERDURATION,  /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_READPTR,         /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_WRITEPTR,        /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_CHANNEL,         /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_SAMPLERATE,      /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BITWIDTH,        /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BIGENDIEN,       /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_TIMESTAMP,       /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_WEIGHTING,       /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_VOLUME,          /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFERLEVEL,     /*       GET only */
    API_AUDIO_CUSTOMER_PCM_CMD_MUTE,            /* SET / GET      */
    API_AUDIO_CUSTOMER_PCM_CMD_BUFFERSIZE,      /*       GET only */
} API_AUDIO_CUSTOMER_PCM_COMMAND;

//-------------------------------------------------------------------------------------------------------------------------------------
/* MM New Mode */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_MEMORY_BASE = 0x0, ///< for audio DDR Base
    API_AUDIO_CUSTOMER_MEMORY_SIZE = 0x1, ///< for audio DDR Size
} API_AUDIO_CUSTOMER_DDRINFO;

typedef struct
{
    MS_U32 aes_base_addr;       ///<  base address of ES buffer
    MS_U32 aes_end_addr;        ///<  end address of ES buffer
    MS_U32 aes_BufSize;         ///<  ES buffer size
    MS_U32 aes_write_addr;      ///<  write pointer
    MS_U32 aes_read_addr;       ///<  read pointer
    MS_U32 aes_freeSpace;       ///<  free space of ES buffer
    MS_U32 aes_level;           ///<  level of ES buffer
    MS_U32 aes_ptsCnt;          ///<  count of total received PTS
    MS_U64 aes_lastPTS;         ///<  last received PTS
} API_AUDIO_CUSTOMER_AES_INFO;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Mstar Sound Effect */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_GEQ,
    API_AUDIO_CUSTOMER_PEQ,
    API_AUDIO_CUSTOMER_HPF,
    API_AUDIO_CUSTOMER_DRC,
    API_AUDIO_CUSTOMER_AVC,
    API_AUDIO_CUSTOMER_BALANCE,
} API_AUDIO_CUSTOMER_MSTAR_SE_TYPE;

typedef struct
{
    MS_U8   band;   ///< band
    MS_U32  a0;     ///< a0
    MS_U32  a1;     ///< a1
    MS_U32  a2;     ///< a2
    MS_U32  b1;     ///< b1
    MS_U32  b2;     ///< b2
    MS_BOOL enable; /// PEQ band enable
} API_AUDIO_CUSTOMER_MSTAR_PEQ_COEF;

typedef struct
{
    MS_U32  a0;     ///< a0
    MS_U32  a1;     ///< a1
    MS_U32  a2;     ///< a2
    MS_U32  b1;     ///< b1
    MS_U32  b2;     ///< b2
} API_AUDIO_CUSTOMER_MSTAR_HPF_COEF;

typedef enum
{
    API_AUDIO_CUSTOMER_AVC_LINEAR,
    API_AUDIO_CUSTOMER_AVC_SHIFT,
    API_AUDIO_CUSTOMER_AVC_NORMAL,
} API_AUDIO_CUSTOMER_MSTAR_AVC_MODE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* Advanced Sound Effect */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    API_AUDIO_CUSTOMER_TSXT,
    API_AUDIO_CUSTOMER_TSHD,
    API_AUDIO_CUSTOMER_THEATERSOUND,
    API_AUDIO_CUSTOMER_ULTRATV,
    API_AUDIO_CUSTOMER_PURESND,
    API_AUDIO_CUSTOMER_STUDIOSOUND_3D,
} API_AUDIO_CUSTOMER_DTS_SE_TYPE;

typedef enum
{
    // TSHD
    API_AUDIO_CUSTOMER_THEATERSOUND_MAIN,
    API_AUDIO_CUSTOMER_THEATERSOUND_TSHD,
    API_AUDIO_CUSTOMER_THEATERSOUND_TSHD_SURR,
    API_AUDIO_CUSTOMER_THEATERSOUND_TRUBASS,
    API_AUDIO_CUSTOMER_THEATERSOUND_DEFINITION,
    API_AUDIO_CUSTOMER_THEATERSOUND_DIALOG_CLARITY,
    API_AUDIO_CUSTOMER_THEATERSOUND_LEVEL_INDEPENDENT,

    // PURE SOUND
    API_AUDIO_CUSTOMER_PURESOUND_HL,
    API_AUDIO_CUSTOMER_PURESOUND_AEQ,
    API_AUDIO_CUSTOMER_PURESOUND_HPF,
} API_AUDIO_CUSTOMER_DTS_SE_UNIT_TYPE;

typedef enum
{
    // TSHD
    API_AUDIO_CUSTOMER_THEATERSOUND_TSHD_INPUT_GAIN_FXP24,             // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_THEATERSOUND_DEFINITION_CONTROL_FXP24,          // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_THEATERSOUND_DIALOG_CLARITY_CONTROL_FXP24,      // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_THEATERSOUND_SURR_LEVEL_CONTROL_FXP24,          // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_THEATERSOUND_TRUBASS_CONTROL_FXP24,             // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_THEATERSOUND_TRUBASS_COMPRESSOR_CONTROL_FXP24,  // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_THEATERSOUND_TRUBASS_PROCESS_MODE,              // 0:mono  1:stereo
    API_AUDIO_CUSTOMER_THEATERSOUND_SPEAKERSIZE,                       // 0:40Hz, 1:60Hz, 2:100Hz, 3:150Hz, 4:200Hz, 5:250Hz, 6:300Hz, 7:400Hz, 8: Custom (default)
    API_AUDIO_CUSTOMER_THEATERSOUND_TRUBASS_SPEAKER_AUDIO,             // Range: 0 ~ 38 (20Hz ~ 400 Hz, step:10Hz)
    API_AUDIO_CUSTOMER_THEATERSOUND_TRUBASS_SPEAKER_ANALYSIS,          // Range: 0 ~ 38 (20Hz ~ 400 Hz, step:10Hz)

    // PURE SOUND
    API_AUDIO_CUSTOMER_PURESOUND_HL_INPUT_GAIN_FXP24,                  // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_PURESOUND_HL_OUTPUT_GAIN_FXP24,                 // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_PURESOUND_HL_BYPASS_GAIN_FXP24,                 // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_PURESOUND_HL_LIMITERBOOST_FXP24,                // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_PURESOUND_HL_HARDLIMIT_CONTROL_FXP24,           // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_PURESOUND_HL_DELAY,                             // Range: 6 ~ 48
    API_AUDIO_CUSTOMER_PURESOUND_HPF_FREQUENCY,                        // Range: 0~3 (60, 80, 100, 120 Hz)
    API_AUDIO_CUSTOMER_PURESOUND_INPUT_GAIN_FXP24,                     // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
    API_AUDIO_CUSTOMER_PURESOUND_OUTPUT_GAIN_FXP24,                    // Range: 0x0 ~ 0x7FFFFF (0.0 ~ 1.0)
} API_AUDIO_CUSTOMER_DTS_SE_PARAM_TYPE;

//-------------------------------------------------------------------------------------------------------------------------------------
/* callback functions */
//-------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8   index;      // PCM index
    MS_U32  pts;        // PTS(unit : 90Khz clock base, max value : 0xFFFFFFFF)
    MS_U8   *pData;     // pointer to Audio Data
    MS_U32  dataLen;    // Audio Data Length
} API_AUDIO_CUSTOMER_PCM_DATA;

typedef struct
{
    MS_U32  encFormat;  // Encode format, 0:MP3, 1:AAC
    MS_U64  pts;        // PTS
    MS_U8   *pData;     // pointer to Audio Data
    MS_U32  dataLen;    // Audio Data Length
    MS_U8   *pRStart;   // start pointer of buffer
    MS_U8   *pREnd;     // end pointer of buffer
} API_AUDIO_CUSTOMER_AENC_DATA;

typedef MS_BOOL (*pfnAudioCustomerAdecoderClipDone)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
typedef MS_BOOL (*pfnAudioCustomerAmixerClipDone)(API_AUDIO_CUSTOMER_MIXER_INDEX amixIndex);
typedef MS_BOOL (*pfnAudioCustomerAENCDataHandling)(API_AUDIO_CUSTOMER_AENC_DATA *pMsg);
typedef MS_BOOL (*pfnAudioCustomerPCMDataHandling)(API_AUDIO_CUSTOMER_PCM_DATA *pMsg);
typedef MS_BOOL (*pfnAudioCustomerPCMSending)(MS_U8 *pBuf, MS_U16 length);
typedef MS_BOOL (*pfnAudioCustomerMP3EncodeDone)(MS_U8 *pCopyBuffer, MS_U16 copyLenth);
typedef MS_BOOL (*pfnAudioCustomerAdecDecodeDone)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 copyLenth);
typedef MS_BOOL (*pfnAudioCustomerAdecEndOfStream)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
typedef MS_BOOL (*pfnAudioCustomerAdecUnderrun)(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);

/******************************************************************************
    (Function Declaration)
******************************************************************************/
/* Initialize */
MS_BOOL API_AUDIO_CUSTOMER_InitializeModule(MS_U32 u32MiuNo, MS_U32 mad_va, API_AUDIO_CUSTOMER_OUTPUT_INFO OutputInfo, API_AUDIO_CUSTOMER_CHIP_PLATFORM chipPlatform);

/* Connect & Disconnect */
MS_BOOL API_AUDIO_CUSTOMER_SetParserMode(API_AUDIO_CUSTOMER_PARSER_MODE ParserMode);
MS_BOOL API_AUDIO_CUSTOMER_ADEC_Connect(API_AUDIO_CUSTOMER_ADEC_INDEX currentConnect, API_AUDIO_CUSTOMER_ADEC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_ADEC_Disconnect(API_AUDIO_CUSTOMER_ADEC_INDEX currentConnect);
MS_BOOL API_AUDIO_CUSTOMER_ADC_Connect(API_AUDIO_CUSTOMER_ADC_INDEX currentConnect, API_AUDIO_CUSTOMER_ADC_IN_PORT portNum);
MS_BOOL API_AUDIO_CUSTOMER_ADC_Disconnect(API_AUDIO_CUSTOMER_ADC_INDEX currentConnect, API_AUDIO_CUSTOMER_ADC_IN_PORT portNum);
MS_BOOL API_AUDIO_CUSTOMER_PCM_Mixer_Connect(API_AUDIO_CUSTOMER_PCM_MIXER_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_MIXER_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_Mixer_Disconnect(API_AUDIO_CUSTOMER_PCM_MIXER_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_MIXER_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_CH_Sound_Connect(API_AUDIO_CUSTOMER_CH_SOUND currentConnect, API_AUDIO_CUSTOMER_CH_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_CH_Sound_Disconnect(API_AUDIO_CUSTOMER_CH_SOUND currentConnect, API_AUDIO_CUSTOMER_CH_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_FW_MIXER_Connect(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_FW_MIXER_Disconnect(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SE_Connect(API_AUDIO_CUSTOMER_SE_INDEX currentConnect, API_AUDIO_CUSTOMER_SE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SE_Disconnect(API_AUDIO_CUSTOMER_SE_INDEX currentConnect, API_AUDIO_CUSTOMER_SE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SNDOUT_Connect(API_AUDIO_CUSTOMER_SOUNDOUT_INDEX currentConnect, API_AUDIO_CUSTOMER_SOUNDOUT_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_SNDOUT_Disconnect(API_AUDIO_CUSTOMER_SOUNDOUT_INDEX currentConnect, API_AUDIO_CUSTOMER_SOUNDOUT_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_CAPTURE_Connect(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_CAPTURE_Disconnect(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, API_AUDIO_CUSTOMER_PCM_CAPTURE_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_MP3_ENC_Connect(API_AUDIO_CUSTOMER_MP3_ENC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_MP3_ENC_Disconnect(API_AUDIO_CUSTOMER_MP3_ENC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_AAC_ENC_Connect(API_AUDIO_CUSTOMER_AAC_ENC_INPUT inputConnect);
MS_BOOL API_AUDIO_CUSTOMER_AAC_ENC_Disconnect(API_AUDIO_CUSTOMER_AAC_ENC_INPUT inputConnect);

/* Start & Stop */
MS_BOOL API_AUDIO_CUSTOMER_SetCodecType(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_CODEC_TYPE audioType);
MS_BOOL API_AUDIO_CUSTOMER_StartDecoding(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_StopDecoding(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_PauseDecoding(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_SetMainDecoderOutput(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);

/* SPDIF */
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetOutputType(API_AUDIO_CUSTOMER_SPDIF_TX_MODE eSPDIFMode);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetCopyInfo(API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT copyInfo);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetCategoryCode(MS_U8 categoryCode);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetLightOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_SetMonitorOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SPDIF_TX_ChannelStatus_CTRL(API_AUDIO_CUSTOMER_SPDIF_CS_TYPE cs_mode, API_AUDIO_CUSTOMER_SPDIF_CS_TYPE_STATUS status);

/* HDMI */
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_GetAudioMode(API_AUDIO_CUSTOMER_CODEC_TYPE *pHDMIMode);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_GetCopyInfo(API_AUDIO_CUSTOMER_SPDIF_COPYRIGHT *pCopyInfo);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_SetAudioReturnChannel(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_RX_SetMonitorOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_TX_SetOutputType(API_AUDIO_CUSTOMER_HDMI_TX_OUTPUT_TYPE eHDMIMode);
MS_BOOL API_AUDIO_CUSTOMER_HDMI_TX_SetMonitorOnOff(MS_BOOL bOnOff);

/* ATV */
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetInputSource(API_AUDIO_CUSTOMER_SIF_INPUT sifSource);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetHighDevMode(API_AUDIO_CUSTOMER_SIF_HIDEV_BW bandwidth, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetAudioStandard(API_AUDIO_CUSTOMER_SIF_STANDARD sifStandard);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetSoundMode(API_AUDIO_CUSTOMER_SIF_AUDIOMODE_TYPE u8SifSoundMode);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetMonitorOnOff(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SIF_SetAutoMute(MS_BOOL bOnOff);
API_AUDIO_CUSTOMER_SIF_STANDARD API_AUDIO_CUSTOMER_SIF_StartAutoStandardDetection(void);

/* Decoder */
MS_BOOL API_AUDIO_CUSTOMER_SetSyncMode(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetDecodingType(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_CODEC_TYPE *pAudioType);
MS_BOOL API_AUDIO_CUSTOMER_SetDualMonoOutMode(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_DUALMONO_MODE outputMode);
MS_BOOL API_AUDIO_CUSTOMER_GetESInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, void * pAudioESInfo);
MS_BOOL API_AUDIO_CUSTOMER_IsESExist(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_SetAudioDescription(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetTrickMode(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_TRICK_MODE eTrickMode);
MS_BOOL API_AUDIO_CUSTOMER_GetBufferStatus(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 *pMaxSize, MS_U32 *pFreeSize);
MS_BOOL API_AUDIO_CUSTOMER_SetAdecPcmPath(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_ADEC_PCM_PATH pcmPath);
MS_BOOL API_AUDIO_CUSTOMER_AdecPcmReady(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_U32  API_AUDIO_CUSTOMER_AdecPcmGet(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, void * pOutPcm, MS_U32 u32Size);
MS_BOOL API_AUDIO_CUSTOMER_AdecRegisterDecodeDoneCallback(pfnAudioCustomerAdecDecodeDone pfnDecodeDoneCallBack);
MS_BOOL API_AUDIO_CUSTOMER_AdecRegisterEndOfStreamCallback(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, pfnAudioCustomerAdecEndOfStream pfnEndOfStreamCallBack);
MS_BOOL API_AUDIO_CUSTOMER_AdecRegisterUnderrunCallback(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, pfnAudioCustomerAdecUnderrun pfnUnderrunCallBack);
MS_BOOL API_AUDIO_CUSTOMER_Auto_Recovery_SetMonitorOnOff(MS_BOOL bOnOff);

/* Common Decoder */
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Open(void * pData);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Close(MS_S32 s32DeviceID);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Start(MS_S32 s32DeviceID);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Stop(MS_S32 s32DeviceID);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Set(MS_S32 s32DeviceID, API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND Cmd, void* pData);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Get(MS_S32 s32DeviceID, API_AUDIO_CUSTOMER_COMMON_DECODER_COMMAND Cmd, void* pData);
MS_U32 API_AUDIO_CUSTOMER_COMMON_DECODER_Read(MS_S32 s32DeviceID, void* pBuf, MS_U32 u32Size);
MS_U32 API_AUDIO_CUSTOMER_COMMON_DECODER_Write(MS_S32 s32DeviceID, void* pBuf, MS_U32 u32Size);
MS_S32 API_AUDIO_CUSTOMER_COMMON_DECODER_Flush(MS_S32 s32DeviceID);

/* Common */
MS_BOOL API_AUDIO_CUSTOMER_SetAudioParam(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_PARAM_TYPE paramType, void *pParam);
MS_BOOL API_AUDIO_CUSTOMER_GetAudioInfo(API_AUDIO_CUSTOMER_ADEC_INDEX  adecIndex, API_AUDIO_CUSTOMER_INFO_TYPE  infoType,  void *pInfo);

/* Customized patch */
MS_BOOL API_AUDIO_CUSTOMIZED_PATCH_SetAudioParam(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMIZED_PATCH_PARAM_TYPE paramType, void *pParam);
MS_BOOL API_AUDIO_CUSTOMIZED_PATCH_GetAudioInfo(API_AUDIO_CUSTOMER_ADEC_INDEX  adecIndex, API_AUDIO_CUSTOMIZED_PATCH_INFO_TYPE  infoType,  void *pInfo);

/* Clip Play for ES */
MS_BOOL API_AUDIO_CUSTOMER_PlayClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_CLIP_DEC_PARAM clipInfo, MS_U32 bufSize, void *pBufClip, pfnAudioCustomerAdecoderClipDone pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_StopClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_PauseClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_ResumeClipDecoder(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);

/* Clip Play for PCM */
MS_BOOL API_AUDIO_CUSTOMER_PlayClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex, API_AUDIO_CUSTOMER_CLIP_MIX_PARAM clipInfo, MS_U32 bufSize, void *pBufClip, pfnAudioCustomerAmixerClipDone pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_StopClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex);
MS_BOOL API_AUDIO_CUSTOMER_PauseClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex);
MS_BOOL API_AUDIO_CUSTOMER_ResumeClipMixer(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex);

/* Gain, Mute & Delay */
MS_BOOL API_AUDIO_CUSTOMER_SetPCMMixerInputGain(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetI2SOutGain(MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetSPDIFOutGain(MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetLineOutGain(API_AUDIO_CUSTOMER_LINE_OUT_IDX lineIndex, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetHDMIOutGain(MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetAudioDescriptionGain(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetFWMixerChannelMute(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetFWMixerChannelGain(API_AUDIO_CUSTOMER_FWM_INDEX currentConnect, API_AUDIO_CUSTOMER_FWM_INPUT inputConnect, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_SetPCMMixerInputMute(API_AUDIO_CUSTOMER_MIXER_INDEX mixerIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetI2SOutMute(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetLineOutMute(API_AUDIO_CUSTOMER_LINE_OUT_IDX lineIndex, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetSPDIFOutMute(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_SetHDMIOutMute(MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetI2SOutMuteStatus(MS_BOOL *pOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetSPDIFOutMuteStatus(MS_BOOL *pOnOff);
MS_BOOL API_AUDIO_CUSTOMER_GetLineOutMuteStatus(API_AUDIO_CUSTOMER_LINE_OUT_IDX lineIndex, MS_BOOL *pOnOff);
MS_BOOL API_AUDIO_CUSTOMER_MuteDuringLimitedTime_Input(MS_U32 per_50ms, API_AUDIO_CUSTOMER_FWM_INDEX eFWM, API_AUDIO_CUSTOMER_FWM_INPUT eCh);
MS_BOOL API_AUDIO_CUSTOMER_MuteDuringLimitedTime_Output(MS_U32 per_50ms, API_AUDIO_CUSTOMER_OUTPUT_TYPE ePort);
MS_BOOL API_AUDIO_CUSTOMER_SetAudioDelay(MS_U32 delay);
MS_BOOL API_AUDIO_CUSTOMER_SetSpdifDelay(MS_U32 delay);
MS_BOOL API_AUDIO_CUSTOMER_SetHdmiDelay(MS_U32 delay);

/* AENC */
MS_BOOL API_AUDIO_CUSTOMER_AENC_Start(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat);
MS_BOOL API_AUDIO_CUSTOMER_AENC_Stop(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat);
MS_BOOL API_AUDIO_CUSTOMER_AENC_RegisterCallback(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, pfnAudioCustomerAENCDataHandling pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_AENC_SetInfo(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, API_AUDIO_CUSTOMER_AENC_INFO info);
MS_BOOL API_AUDIO_CUSTOMER_AENC_GetInfo(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, API_AUDIO_CUSTOMER_AENC_INFO *pInfo);
MS_BOOL API_AUDIO_CUSTOMER_AENC_SetGain(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_AENC_CopyData(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, MS_U8 *pDest, MS_U8 *pBufAddr, MS_U32 datasize, MS_U8 *pRStart, MS_U8 *pREnd);
MS_BOOL API_AUDIO_CUSTOMER_AENC_ReleaseData(API_AUDIO_CUSTOMER_AENC_ENCODING_FORMAT encFormat, MS_U8 *pBufAddr, MS_U32 datasize);

/* PCM Capture */
MS_BOOL API_AUDIO_CUSTOMER_PCM_StartUpload(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_StopUpload(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect);
MS_BOOL API_AUDIO_CUSTOMER_PCM_RegisterSendPCMCallback(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, pfnAudioCustomerPCMSending pfnCallBack);
MS_BOOL API_AUDIO_CUSTOMER_PCM_SetGain(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, MS_U32 gain);
MS_BOOL API_AUDIO_CUSTOMER_PCM_SetMute(API_AUDIO_CUSTOMER_PCM_CAPTURE_INDEX currentConnect, MS_BOOL bOnOff);
MS_BOOL API_AUDIO_CUSTOMER_PCM_SetRequestSizeInMs(MS_U32 request_ms);

/* PCM IO Control */
MS_S32  API_AUDIO_CUSTOMER_PCM_Open(API_AUDIO_CUSTOMER_PCMIO_PARAM *pData);
MS_S32  API_AUDIO_CUSTOMER_PCM_Close(MS_S32 s32DeviceId);
MS_S32  API_AUDIO_CUSTOMER_PCM_Start(MS_S32 s32DeviceId);
MS_S32  API_AUDIO_CUSTOMER_PCM_Stop(MS_S32 s32DeviceId);
MS_S32  API_AUDIO_CUSTOMER_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32  API_AUDIO_CUSTOMER_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32  API_AUDIO_CUSTOMER_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32  API_AUDIO_CUSTOMER_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32  API_AUDIO_CUSTOMER_PCM_Flush(MS_S32 s32DeviceId);

/* MM New Mode */
MS_PHY  API_AUDIO_CUSTOMER_GetDDRInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_DDRINFO DDRInfo);
MS_BOOL API_AUDIO_CUSTOMER_MM2_initAesInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex);
MS_BOOL API_AUDIO_CUSTOMER_MM2_checkAesInfo(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, API_AUDIO_CUSTOMER_AES_INFO *aes_info);
MS_BOOL API_AUDIO_CUSTOMER_MM2_inputAesFinished(API_AUDIO_CUSTOMER_ADEC_INDEX adecIndex, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts);

/* Mstar Sound Effect */
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_Enable(API_AUDIO_CUSTOMER_MSTAR_SE_TYPE Type, MS_BOOL enable);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_GEQ()
///@brief \b Function \b Description: This routine is used to set the the 5-band GEQ u8Level
///@param <IN> \b u8band    :  GEQ band  0~4 // 0:120Hz, 1:500Hz, 2:1500Hz, 3:5000Hz, 4:10000Hz
///@param <IN> \b s8level   :  GEQ level -48~48(-12dB~12dB, step 0.25dB)
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_GEQ(MS_U8 u8band, MS_S8 s8level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_PEQCoef()
///@brief \b Function \b Description: This routine is used to set the PEQ parameters
///@param <IN> \b peq_coef->band    :  PEQ band select ( 0 ~ 7 )
///@param <IN> \b peq_coef->enable  :  PEQ band enable/disable ( 0:disable  1:enable )
///@param <IN> \b peq_coef->a0      :  PEQ coefficient a0 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->a1      :  PEQ coefficient a1 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->a2      :  PEQ coefficient a2 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->b1      :  PEQ coefficient b1 (check below sampele code to calculate coefficient)
///@param <IN> \b peq_coef->b2      :  PEQ coefficient b2 (check below sampele code to calculate coefficient)
///
/// @@@ Here are sample codes to calculate PEQ Coefficents @@@
/// @param <IN> \b Gain: -12dB ~ 12dB
/// @param <IN> \b Fc (Hz)
/// @param <IN> \b Q : 0.5~16.0 (band width: wide to narrow)
///void PEQ_Coefficent_Calculation(const float Gain, const int Fc, const float Q)
///{
///    float coef;
///    float G, fc, Q;
///    float fb,d,v0,H0,aBC,fm,fz,kb;
///    unsigned int a0, a1, a2, b1, b2;
///
///    fb = Fc / Q;
///    d = - cos( 2 * 3.1415926 * Fc / 48000);
///    v0 = powf(10.0, (Gain / 20.0));
///    H0 = v0 -1;
///    aBC=0;
///    fm=0;
///    fz=1;
///    kb = tan ( 3.1415926 * fb / 48000);
///
///    if (Gain >=0)
///    {
///        fz = kb - 1;
///        fm = kb + 1;
///    }
///    else
///    {
///        fz = kb - v0;
///        fm = kb + v0;
///    }
///
///    aBC = fz / fm ;
///    coef = 1 + (1 + aBC) * H0 / 2;
///    a0 = (long)(coef * 4194304);
///    coef = d * (1 - aBC);
///    a1 = (long)(coef * 4194304);
///    coef = -aBC - (1 + aBC) * H0 /2;
///    a2 = (long)(coef * 4194304);
///    coef = d * (1 - aBC);
///    b1 = (long)(-coef * 4194304);
///    coef = -aBC;
///    b2 = (long)(-coef * 4194304);
///
///    printf("PEQ Coefficient:\n");
///    printf("a0:%lx \n",a0));
///    printf("a1:%lx \n",a1));
///    printf("a2:%lx \n",a2));
///    printf("b1:%lx \n",b1));
///    printf("b2:%lx \n",b2));
///}
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_PEQCoef(API_AUDIO_CUSTOMER_MSTAR_PEQ_COEF *peq_coef);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_HPFCoef()
///@brief \b Function \b Description: This routine is used to set the HPF parameters
///@param <IN> \b hpf_coef->a0      :  HPF coefficient a0 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->a1      :  HPF coefficient a1 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->a2      :  HPF coefficient a2 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->b1      :  HPF coefficient b1 (check below sampele code to calculate coefficient)
///@param <IN> \b hpf_coef->b2      :  HPF coefficient b2 (check below sampele code to calculate coefficient)
///
/// @@@ Here are sample codes to calculate HPF Coefficents @@@
/// @param <IN> \b Fc (Hz): 50Hz~200Hz, step 1Hz
///void HPF_Coefficent_Calculation( const int Fc )
///{
///    float coef[5];
///    float dem;
///    float Q;
///    float k;
///    float kpow2;
///    unsigned int a0, a1, a2, b1, b2;
///
///    k = tan((3.1415926*Fc)/48000);
///    Q = 1/sqrt(2);
///
///    kpow2 = k*k;
///
///    dem = 1 + k/Q + kpow2;
///    coef[0] = 1/dem;
///    coef[1] = -2/dem;
///    coef[2] = 1/dem;
///    coef[3] = 2*(kpow2-1)/dem;
///    coef[4] = (1 - k/Q + kpow2)/dem;
///
///    a0 = (long)(coef[0] * 8388608/2);
///    a1 = (long)(coef[1] * 8388608/2);
///    a2 = (long)(coef[2] * 8388608/2);
///    b1 = (long)(-coef[3] * 8388608/2);
///    b2 = (long)(-coef[4] * 8388608/2);
///    printf("HPF Coefficient:\n");
///    printf("a0:%lx \n",a0));
///    printf("a1:%lx \n",a1));
///    printf("a2:%lx \n",a2));
///    printf("b1:%lx \n",b1));
///    printf("b2:%lx \n",b2));
///}
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_HPFCoef(API_AUDIO_CUSTOMER_MSTAR_HPF_COEF *hpf_coef);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_LEVEL()
///@brief \b Function \b Description: This routine is used to set the AVC clipping level
///@param <IN> \b u16level    :  AVC clipping u16level
///                              0x00--  0      dBFS
///                              0x01--  -0.5   dBFS
///                              0x20--  -16    dBFS
///                              0x50--  -40    dBFS
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_LEVEL(MS_U16 u16level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_S_MODE_OFFSET()
///@brief \b Function \b Description: This routine is used to set the AVC L_Mode offset (min value)
///@param <IN> \b u16offset    : range: +0 ~ +12 dB
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_S_MODE_OFFSET(MS_U16 u16offset);

MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_AVC_MODE(API_AUDIO_CUSTOMER_MSTAR_AVC_MODE mode);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_DRC_LEVEL()
///@brief \b Function \b Description: This routine is used to set the DRC clipping level
///@param <IN> \b u16level    :  DRC clipping u16level
///                              0x00--  0      dBFS
///                              0x01--  -0.5   dBFS
///                              0x20--  -16    dBFS
///                              0x50--  -40    dBFS
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_DRC_LEVEL(MS_U16 u16level);

///@brief \b Function \b Name: API_AUDIO_CUSTOMER_MSTAR_SE_SET_BALANCE()
///@brief \b Function \b Description: This routine is used to set the volume balance
///@param <IN> \b Lbalance    :  balance L  0x00(0dB)~0xFF(mute)  step: -0.25dB
///@param <IN> \b Lbalance    :  balance R  0x00(0dB)~0xFF(mute)  step: -0.25dB
MS_BOOL API_AUDIO_CUSTOMER_MSTAR_SE_SET_BALANCE(MS_U16 Lbalance, MS_U16 Rbalance);

/* Advanced Sound Effect */
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_Enable(API_AUDIO_CUSTOMER_DTS_SE_TYPE seType);
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_ProcessUnit_Enable(API_AUDIO_CUSTOMER_DTS_SE_UNIT_TYPE seUnit, MS_BOOL bEn);
MS_BOOL API_AUDIO_CUSTOMER_DTS_SE_SetParam(API_AUDIO_CUSTOMER_DTS_SE_PARAM_TYPE param, MS_U32 u32value);

/* Debug */
void API_AUDIO_CUSTOMER_DebugMenu(void * pVoid, MS_BOOL scanf_bSupport);
void API_AUDIO_CUSTOMER_DebugMenu_Non_Scanf_ParseCommand(char *paramsStr);


//-----------------------------------------------------------------------------------
// [用途] 主要是用來 Set
// 需與 hal_audio.c 的 HAL_AUDIO_CUSTOMER_DEBUG_PARAM 對齊
//-----------------------------------------------------------------------------------
typedef struct
{
    MS_U32 Dbg_Param;
    MS_U32 Dbg_Param2;
    MS_U32 Dbg_Param3;
    MS_U32 Dbg_Param4;
    MS_U32 Dbg_Param5;
    MS_U32 Dbg_Param6;
    MS_U32 Dbg_Param7;
    MS_U32 Dbg_Param8;
    MS_U32 Dbg_Param9;
    MS_U32 Dbg_Param10;
} API_AUDIO_CUSTOMER_DEBUG_PARAM;

//-----------------------------------------------------------------------------------
// [用途] 主要是用來 Get
// 需與 hal_audio.c 的 HAL_AUDIO_CUSTOMER_DEBUG_INFO 對齊
//-----------------------------------------------------------------------------------
typedef struct
{
    MS_U32 Dbg_Info;
    MS_U32 Dbg_Info2;
    MS_U32 Dbg_Info3;
    MS_U32 Dbg_Info4;
    MS_U32 Dbg_Info5;
    MS_U32 Dbg_Info6;
    MS_U32 Dbg_Info7;
    MS_U32 Dbg_Info8;
    MS_U32 Dbg_Info9;
    MS_U32 Dbg_Info10;
} API_AUDIO_CUSTOMER_DEBUG_INFO;

//Header-free api structure
typedef struct
{
    const char *name;
    void* pdata;
} audio_tunnel;

MS_BOOL API_AUDIO_CUSTOMER_Testing_7202(void * pVoid);


#ifdef __cplusplus
}
#endif

#endif // _API_AUDIO_CUSTOMER_H_
