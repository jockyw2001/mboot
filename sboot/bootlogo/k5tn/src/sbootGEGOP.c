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

#include "MsTypes.h"
#include "sbootTypes.h"
#include "sbootGEGOP.h"

#define REG_ADDR_BASE 0xBF000000
#define WRITEREG16(_u32Reg, _u16Val ) { *((volatile MS_U16*)(MS_U32)(REG_ADDR_BASE + _u32Reg)) = (_u16Val); }
#define WRITEREG8(_u32Reg, _u16Val ) { *((volatile MS_U8*)(MS_U32)(REG_ADDR_BASE + _u32Reg)) = (_u16Val); }
#define WRITEREG8_MASK(_u32Reg, _u16Mask, _u16Val ) { *((volatile MS_U8*)(MS_U32)(REG_ADDR_BASE + _u32Reg)) = ((*((volatile MS_U8*)(MS_U32)(REG_ADDR_BASE + _u32Reg)) & ~(_u16Mask)) | ((_u16Val) & (_u16Mask))); }

void _Sboot_GEGOP_GetDstInfo(const EN_OUTPUT_TIMING_TYPE enOutputTiming, MS_U16 *u16Width, MS_U16 *u16Height, MS_U16 *u16PipeDelay)
{
    switch(enOutputTiming)
    {
        case E_OUTPUT_TIMING_720X480_60I:
        case E_OUTPUT_TIMING_720X480_60P:
            *u16Width = 720;
            *u16Height = 480;
            *u16PipeDelay = 0x38;
            break;
        case E_OUTPUT_TIMING_720X576_50I:
        case E_OUTPUT_TIMING_720X576_50P:
            *u16Width = 720;
            *u16Height = 576;
            *u16PipeDelay = 0x41;
            break;
        case E_OUTPUT_TIMING_1280X720_50P:
        case E_OUTPUT_TIMING_1280X720_60P:
            *u16Width = 1280;
            *u16Height = 720;
            *u16PipeDelay = 0xD6;
            break;
        case E_OUTPUT_TIMING_1920X1080_50I:
        case E_OUTPUT_TIMING_1920X1080_60I:
        case E_OUTPUT_TIMING_1920X1080_50P:
        case E_OUTPUT_TIMING_1920X1080_60P:
            *u16Width = 1920;
            *u16Height = 1080;
            *u16PipeDelay = 0x84;
            break;
        default:
            *u16Width = 1920;
            *u16Height = 1080;
            *u16PipeDelay = 0x84;
            break;
    }
}

MS_BOOL Sboot_GEGOP_Init(const MS_U32 u32SrcAddr, const RectInfo stRectSrcInfo, const EN_OUTPUT_TIMING_TYPE enOutputTiming)
{
    // this src is YUV 422
    MS_U32 u32GEInputAddr = u32SrcAddr;
    MS_U32 u32GEOutputAddr = CONFIG_GOP_OUTPUT_ADDR;
    MS_U16 u16SrcW = stRectSrcInfo.u16Width;
    MS_U16 u16SrcH = stRectSrcInfo.u16Height;
    MS_U16 u16SrcPitch = stRectSrcInfo.u16Pitch;
    MS_U16 u16DstW = 0;
    MS_U16 u16DstH = 0;
    MS_U16 u16GOPPipeDelay = 0;

    MS_U32 WordUnit = 0x10;
    MS_U32 u32GwinInputAddr = u32GEOutputAddr / WordUnit;

    _Sboot_GEGOP_GetDstInfo(enOutputTiming, &u16DstW, &u16DstH, &u16GOPPipeDelay);

    // GE
   WRITEREG16((0x102804 << 1), 0x3F7F);
   WRITEREG16((0x102800 << 1), 0xC001);
   WRITEREG16((0x102802 << 1), 0x4011);
   WRITEREG16((0x10283E << 1), 0x0040);
   WRITEREG16((0x102840 << 1), (u32GEInputAddr & 0x0000FFFF));          // Src addr => byte address [3:0]
   WRITEREG16((0x102842 << 1), ((u32GEInputAddr >> 16) & 0x0000FFFF));  // Src addr => byte address [7:4]
   WRITEREG16((0x10284C << 1), (u32GEOutputAddr & 0x0000FFFF));         // Dst addr=> byte address [3:0]
   WRITEREG16((0x10284E << 1), ((u32GEOutputAddr >> 16) & 0x0000FFFF)); // Dst addr=> byte address [7:4]
   WRITEREG16((0x102860 << 1), (u16SrcPitch * 2));                     // Src pitch=>(ARGB8888)Src pitch*4 ¡B(YUV422)Src pitch*2
   WRITEREG16((0x102864 << 1), 0x0001);
   WRITEREG16((0x102866 << 1), (u16DstW * 2));                          // Dst pitch=>(ARGB8888)Dst width*4 ¡B(YUV422)Dst width*2
   WRITEREG16((0x102868 << 1), 0x0E0E);                                 // Color format=>(ARGB8888)0x0F0F¡B(YUV422)0x0E0E
   WRITEREG16((0x1028AC << 1), 0x0780);
   WRITEREG16((0x1028B0 << 1), 0x0438);
   WRITEREG16((0x1028B2 << 1), 0x0040);
   WRITEREG16((0x1028BC << 1), 0x1B00);
   WRITEREG16((0x1028BE << 1), 0x1B8F);
   WRITEREG16((0x1028C8 << 1), ((u16SrcW * 4096) / u16DstW));          // H ratio=>(src width*2^12)/(dst width)
   WRITEREG16((0x1028CA << 1), ((u16SrcH * 4096) / u16DstH));           // V ratio=>(src height*2^12)/(dst height)
   WRITEREG16((0x1028D4 << 1), (u16DstW - 1));                          // X1=>dst width -1
   WRITEREG16((0x1028D6 << 1), (u16DstH - 1));                          // Y1=>dst height -1
   WRITEREG16((0x1028DC << 1), u16SrcW);                               // src width=> src width
   WRITEREG16((0x1028DE << 1), u16SrcH);                                // src height=> src height
   WRITEREG16((0x102804 << 1), 0x3F3F);
   WRITEREG16((0x1028C0 << 1), 0x8040);

    // GOP
   WRITEREG16((0x120800 << 1), 0xC408);
   WRITEREG16((0x120802 << 1), 0x4102);
   WRITEREG16((0x120804 << 1), 0x0018);
   WRITEREG16((0x120806 << 1), 0x0000);
   WRITEREG16((0x120808 << 1), 0x0000);
   WRITEREG16((0x12080A << 1), 0x80FF);
   WRITEREG16((0x12080C << 1), 0x01FF);
   WRITEREG16((0x120810 << 1), 0x403F);
   WRITEREG16((0x120812 << 1), 0x5804);
   WRITEREG16((0x12081C << 1), 0x03C1);
   WRITEREG16((0x12081E << 1), u16GOPPipeDelay);                            // pipe delay
   WRITEREG16((0x120832 << 1), 0x47D0);
   WRITEREG16((0x120836 << 1), 0x200C);
   WRITEREG16((0x120860 << 1), (u16DstW / 2));                              // stretch win H size = width / 2
   WRITEREG16((0x120862 << 1), u16DstH);                                    // stretch win V size = height
   WRITEREG16((0x12086A << 1), 0x1000);
   WRITEREG16((0x12086C << 1), 0x1000);
   WRITEREG16((0x120864 << 1), 0x0000);
   WRITEREG16((0x120900 << 1), 0x3F91);                                     // color format=>(ARGB8888)0x3F51¡B(YUV422)0x3F91
   WRITEREG16((0x120902 << 1), (u32GwinInputAddr & 0x0000FFFF));            // addr=>(GE dst addr /Word Unit) [3:0]
   WRITEREG16((0x120904 << 1), ((u32GwinInputAddr >> 16) & 0x0000FFFF));    // addr=>(GE dst addr /Word Unit) [7:4]
   WRITEREG16((0x12090A << 1), ((u16DstW * 2) / WordUnit));                 // width=>ARGB8888(width*4/Word Unit) ¡BYUV422(width*2/Word Unit)
   WRITEREG16((0x120910 << 1), u16DstH);                                    // height=> height
   WRITEREG16((0x120912 << 1), ((u16DstW * 2) / WordUnit));                 // pitch=>(GE dst pitch/ Word Unit)
   WRITEREG16((0x120914 << 1), 0x0003);
   WRITEREG16((0x120920 << 1), 0x0000);
   WRITEREG16((0x120922 << 1), 0x0000);
   WRITEREG16((0x101FFC << 1), 0x060A);
   WRITEREG16((0x101FFE << 1), 0x0206);
   WRITEREG16((0x101FFE << 1), 0x0006);
   WRITEREG16((0x13000C << 1), 0x8000);

//    WRITEREG16((0x1208FC << 1), 0x0688);
//    WRITEREG16((0x13000C << 1), 0x2000);
//    WRITEREG16((0x130F8C << 1), 0x0000);

}

MS_BOOL Sboot_GOP2VE(const MS_U32 u32SrcAddr, const RectInfo stRectSrcInfo, const EN_OUTPUT_TIMING_TYPE enOutputTiming)
{
   WRITEREG16((0x102804 << 1), 0x3F7F);
   WRITEREG16((0x102800 << 1), 0xC001);
   WRITEREG16((0x102802 << 1), 0x4011);
   WRITEREG16((0x102840 << 1), 0x2000);
   WRITEREG16((0x102842 << 1), 0x0399);
   WRITEREG16((0x10284C << 1), 0x0000);
   WRITEREG16((0x10284E << 1), 0x10F0);
   WRITEREG16((0x102860 << 1), 0x0140);
   WRITEREG16((0x102864 << 1), 0x0001);
   WRITEREG16((0x102866 << 1), 0x05A0);
   WRITEREG16((0x102868 << 1), 0x0E0E);
   WRITEREG16((0x1028AC << 1), 0x0780);
   WRITEREG16((0x1028B0 << 1), 0x0438);
   WRITEREG16((0x1028B2 << 1), 0x0040);
   WRITEREG16((0x1028BC << 1), 0x1B00);
   WRITEREG16((0x1028BE << 1), 0x1B8F);
   WRITEREG16((0x1028C8 << 1), 0x0371);
   WRITEREG16((0x1028CA << 1), 0x03CE);
   WRITEREG16((0x1028D4 << 1), 0x02CF);
   WRITEREG16((0x1028D6 << 1), 0x023F);
   WRITEREG16((0x1028DC << 1), 0x009B);
   WRITEREG16((0x1028DE << 1), 0x0089);
   WRITEREG16((0x102804 << 1), 0x3f3f);
   WRITEREG16((0x1028C0 << 1), 0x8040);
   WRITEREG16((0x100B80 << 1), 0x0018);
   WRITEREG16((0x103BAA << 1), 0x0D05);
   WRITEREG16((0x120F60 << 1), 0x0000);
   WRITEREG16((0x120200 << 1), 0x4406);
   WRITEREG16((0x120202 << 1), 0x4103);
   WRITEREG16((0x120204 << 1), 0x0018);
   WRITEREG16((0x120206 << 1), 0x0000);
   WRITEREG16((0x120208 << 1), 0x0000);
   WRITEREG16((0x12020A << 1), 0x80FF);
   WRITEREG16((0x12020C << 1), 0x01FF);
   WRITEREG16((0x120210 << 1), 0x403F);
   WRITEREG16((0x120212 << 1), 0x5804);
   WRITEREG16((0x12021C << 1), 0x03C1);
   WRITEREG16((0x12021E << 1), 0x0084);
   WRITEREG16((0x120232 << 1), 0x47D0);
   WRITEREG16((0x120236 << 1), 0x200C);
   WRITEREG16((0x120260 << 1), 0x0168);
   WRITEREG16((0x120262 << 1), 0x0240);
   WRITEREG16((0x120268 << 1), 0x0013);
   WRITEREG16((0x12026A << 1), 0x1000);
   WRITEREG16((0x12026C << 1), 0x1000);
   WRITEREG16((0x120264 << 1), 0x0000);
   WRITEREG16((0x120300 << 1), 0x3F91);
   WRITEREG16((0x120302 << 1), 0x0000);
   WRITEREG16((0x120304 << 1), 0x010F);
   WRITEREG16((0x12030A << 1), 0x005A);
   WRITEREG16((0x120310 << 1), 0x0120);
   WRITEREG16((0x120312 << 1), 0x005A);
   WRITEREG16((0x120314 << 1), 0x0003);
   WRITEREG16((0x120320 << 1), 0x0000);
   WRITEREG16((0x120322 << 1), 0x0000);
   WRITEREG16((0x101FFE << 1), 0x0206);
   WRITEREG16((0x101FFE << 1), 0x0006);
   WRITEREG16((0x13000C << 1), 0x8000);
}
