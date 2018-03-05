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


#ifndef _MS_DRV_VIDEO_
#define _MS_DRV_VIDEO_

/// Stream type
typedef enum
{
    ///Live stream
    E_MSDRV_VIDEO_STREAM_LIVE = 0,
    ///TS file
    E_MSDRV_VIDEO_STREAM_TSFILE,
    ///file
    E_MSDRV_VIDEO_STREAM_FILE,
    ///Stream type number
    E_MSDRV_VIDEO_STREAM_MAX
} MsDrv_VIDEO_StreamType_e;

typedef enum
{
    ///unsupported codec type
    E_MSDRV_VIDEO_CODEC_TYPE_NONE = 0,
    ///MPEG 1/2
    E_MSDRV_VIDEO_CODEC_TYPE_MPEG2,
    ///H263 (short video header)
    E_MSDRV_VIDEO_CODEC_TYPE_H263,
    ///MPEG4 (default)
    E_MSDRV_VIDEO_CODEC_TYPE_MPEG4,
    ///MPEG4 (Divx311)
    E_MSDRV_VIDEO_CODEC_TYPE_DIVX311,
    ///MPEG4 (Divx412)
    E_MSDRV_VIDEO_CODEC_TYPE_DIVX412,
    ///FLV
    E_MSDRV_VIDEO_CODEC_TYPE_FLV,
    ///VC1 advanced profile (VC1)
    E_MSDRV_VIDEO_CODEC_TYPE_VC1_ADV,
    ///VC1 main profile (RCV)
    E_MSDRV_VIDEO_CODEC_TYPE_VC1_MAIN,
    ///Real Video version 8
    E_MSDRV_VIDEO_CODEC_TYPE_RV8,
    ///Real Video version 9 and 10
    E_MSDRV_VIDEO_CODEC_TYPE_RV9,
    ///H264
    E_MSDRV_VIDEO_CODEC_TYPE_H264,
    ///AVS
    E_MSDRV_VIDEO_CODEC_TYPE_AVS,
    ///MJPEG
    E_MSDRV_VIDEO_CODEC_TYPE_MJPEG,
    ///MVC
    E_MSDRV_VIDEO_CODEC_TYPE_MVC,
    ///VP8
    E_MSDRV_VIDEO_CODEC_TYPE_VP8,
    ///HEVC
    E_MSDRV_VIDEO_CODEC_TYPE_HEVC,
    E_MSDRV_VIDEO_CODEC_TYPE_MAX
} MsDrv_VIDEO_CodecType_e;

int MsDrv_VideoStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType);
int MsDrv_IFrameStart(MsDrv_VIDEO_StreamType_e eStreamType, MsDrv_VIDEO_CodecType_e eCodecType, char * FileName);
MS_BOOL MsDrv_VideoSetXcMvopCfg(void);
int MsDrv_VideoStop(void);
MS_U32 MsDrv_VideoShowDecInfo(void);
void MsDrv_VideoRoutinePolling(void);
MS_U32 MsDrv_VideoGetDecFrmCnt(void);
MS_U32 MsDrv_VideoGetPtsAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_U32 *pu32PtsKhzUnit);
MS_BOOL MsDrv_VideoGetFrameCountAddr(MsDrv_VIDEO_CodecType_e eCodecType, MS_U32 *pu32FrameCntAddr, MS_U32 *pu32SkipFrameCntAddr);
MS_BOOL MsDrv_VideoIsSyncOn(void);
MS_BOOL MsDrv_VideoIsReachSync(MS_BOOL bWaitSync);
void MsDrv_VideoUnMute(void);

MS_U32 MsDrv_VideoGetCurPts(void);

MS_BOOL MsDrv_VideoPause(void);
MS_BOOL MsDrv_VideoResume(void);
MS_U32 MsDrv_VideoGetDecFrmCnt(void);
MS_U32 MsDrv_VideoGetDispCnt(void);


#endif  // #ifndef _MS_DRV_VIDEO_

