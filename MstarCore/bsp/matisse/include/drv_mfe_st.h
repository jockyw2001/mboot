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
// (uMStar Confidential Informationv) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _DRV_MFE_ST_H_
#define _DRV_MFE_ST_H_

#ifdef __cplusplus
extern "C"
{
#endif


#define MFE_SUPPORT_REDUCE_MODE
// This version of MFE driver supports "reduce mode" which includes
// two flags that can be turned on/off seperately. Refer to
// 'enableReduceBW' and 'enableFastMode' in struct PVR_Info below.


typedef enum {
    MFE_Invalid            = 0x00,
    MFE_Init                 = 0x20,
    MFE_Idle                =0x30,
    MFE_Busy               =0x40,
    MFE_DeInit             =0x50,
    MFE_Error               =0x60
 } MFE_STATE;

typedef enum
{
MFE_CLK_VERY_SLOW = 0,
MFE_CLK_SLOW = 1,
MFE_CLK_MEDIUM = 2,
MFE_CLK_FAST = 3,
} MFE_CLK_LEVEL;


typedef enum
{
BITS_INFO_ZERO = 0,
BITS_INFO_UNKNOW,
BITS_INFO_FRAMEDONE,
BITS_INFO_BUFFULL,
BITS_INFO_IMG_BUFFULL,
} BITS_STATUS_INFO;

typedef enum
{
GETBITS_NOT_READY=0,
GETBITS_READY=1,
} MFE_HW_STATUS;

typedef enum
{
PROGRESSIVE = 0,
INTERLACE_SPLITED,
INTERLACE_NONSPLITED,
} MFE_INTERLACE_INFO;

typedef enum
{
YUVTILE = 0,
MSTTILE = 1,
MFE_YUYV = 2,
MFE_YVYU = 3,
MFE_UYVY = 4,
MFE_VYUY = 5,
} MFE_COLOR_FORMAT_INFO;

#define MAX_OUTBUFFER 10

typedef struct {
    unsigned long miuAddress;    //hardware physical
    unsigned long miuPointer;    //need to OR 0xA0000000
    unsigned long virtual_addr;
    long size;
    BITS_STATUS_INFO status;    // 1: frame_done. 2: buffer full,3: image buffer full.
    int is_more_bits; //1: there are more bitstream packet;  0: this is the last bitstream packet
    unsigned char  voptype;
    unsigned long IVOP_address;
    int stuffing_bytes;     //CABAC stuffing byte
} BITSFRAME_INFO;

/* Linear memory area descriptor */
typedef struct MEMMAP
{
    //U32 *miuPointer;
    unsigned char *miuPointer;
    unsigned int miuAddress;
    unsigned int size;
} MEMMAP_t;

typedef struct MEMMAP_CUR
{
    MEMMAP_t Cur_Y0;
    MEMMAP_t Cur_C0;
    MEMMAP_t Cur_Y1;
    MEMMAP_t Cur_C1;
} MEMMAP_CUR_t;

/* madp_mfe crop info */
typedef struct _MFE_CROP_INFO
{
    MS_BOOL crop_en;
    unsigned int crop_top;
    unsigned int crop_bottom;
    unsigned int crop_left;
    unsigned int crop_right;
} MFE_CROP_INFO_t;

/* madp_mfe api info */
typedef struct _MFE_API_INFO
{
    unsigned int miu_offset;
    unsigned int mfe_phy_buf_addr;
    unsigned int mfe_vir_buf_ptr;
    unsigned int mfe_phy_buf_size;
    unsigned int Encode_stop;
} MFE_API_INFO_t;

typedef struct _MFE_ADV_INFO
{
    unsigned int low_bandwidth_en;
    unsigned int input_imi_en;
    MEMMAP_t imi_buf_addr;
} MFE_ADV_INFO_t;

typedef struct{
    unsigned int is_off;
    MFE_CLK_LEVEL clock_level;
} POWERON_t;

typedef struct _SPSPPS_INFO
{
    unsigned char* pHeaderSPS;
    unsigned int SPSLen;
    unsigned char* pHeaderPPS;
    unsigned int PPSLen;
} SPSPPS_INFO_t;

typedef struct _VOL_INFO
{
    unsigned char* pHeader;
    unsigned int Len;
} VOL_INFO_t;

// bFirst_or_last_byte_of_Frame: 0x00 error state, 0x10 first , 0x01 last at frame, 0x11 one frame
typedef void (*StreamCallback)(void *pCtx, MS_S32 s32FrameType, void *pStreamData, MS_U32 u32DataLen, MS_U32 bFirstOrLastByteOfFrame);

typedef void (*CleanCallback)(MS_U32 u32FrameCoded, MS_U32 u32YUVPlane);


// MADP-layer "simplified" encoder configuration
// NOTE: "Advanced" configurations are hidden inside DRV-layer.
typedef struct{
    // MFE-compliant
    int width;
    int height;
    unsigned long BitsBuffStart;
    unsigned long BitsBuffSize;
    int nOutBuffer;
    int OutBufferSize;
    MEMMAP_t InBuffer;
    MEMMAP_t BitstreamOutBuffer;
    int BitstreamOutBufferSize;

    // MFE new settings
    //MFE_COLOR_FORMAT_INFO nColorFormat;
    int nCodecType;
    int nDispWidth, nDispHeight;    // Display resolution
    int nBufWidth, nBufHeight;        // Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
    MFE_INTERLACE_INFO bInterlace;
    MFE_COLOR_FORMAT_INFO bColorFormat;
    // GOP
    int nPbetweenI;
    int nBbetweenP;
    int nP_is_infinite;
    // Rate control
    int FrameRatex100;
    int nBitrate;
    int m_bFrameMode;
    int out_frametag_top;
    // Video length
    int nTotalFrames;    // -1 means infinite. 0 is not allowed.
    int TimeIncreamentRes;              // [IN] frame rate
    int VopTimeIncreament;              // [IN] frame rate
    //jpeg parameter
    int quality;
    int fdc_mode;
    int exif_enable;
    MFE_CLK_LEVEL MFEClk;

    MFE_API_INFO_t MfeApiInfo;
    MFE_ADV_INFO_t MfeAdvInfo;

    unsigned int nDropFlag;
    unsigned int nSkipFlag;
    unsigned int enableVTRateControl;
    unsigned int enableSecurityMode;
    unsigned int enableISR;
    unsigned int enableCABAC; //main profile

    MFE_CROP_INFO_t MfeCropInfo;
    unsigned int enableReduceBW; //Memory BW reduced mode
    unsigned int enableFastMode; //intra mode inly 0~2, drop i16 and disable fme.
    MFE_STATE mfe_state;
    void* g_pCustomCtx;
    StreamCallback g_StreamCallBackFunc;
    CleanCallback g_CleanCallBackFunc;

    void* mfe_drvHandle;

    MS_S32 reserved0;
    MS_S32 reserved1;
    MS_S32 reserved2;
    MS_S32 reserved3;
    MS_S32 reserved4;
    MS_S32 reserved5;
}PVR_Info;

#define API_HWCAP_VERSION0 1

typedef struct{
    MS_U32 u32ApiHW_Version;   //<Version of current structure>
    MS_U16 u16ApiHW_Length;    //<Length of this structure>
    MS_BOOL bIsSupportEncoder;
    MS_U8 u8MaxEncodeFrame;
    MS_U16 u16MaxEncodeWidth;
    MS_U16 u16MaxEncodeHeight;
    MS_BOOL bSupportSubVidWin;
    MS_BOOL bSupportMpeg4;
    MS_BOOL bSupportH264;
    /*New Cap add in struct add at the end*/
}MFE_HwCapV0;

#ifdef __cplusplus
}
#endif

#endif
