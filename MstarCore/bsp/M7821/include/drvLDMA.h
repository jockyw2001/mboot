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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVLDMA_PRIV_H_
#define _DRVLDMA_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvMSPI_private.h
/// @author MStar Semiconductor Inc.
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

typedef enum
{
    E_LDMA_DBGLV_NONE,    //disable all the debug message
    E_LDMA_DBGLV_INFO,    //information
    E_LDMA_DBGLV_NOTICE,  //normal but significant condition
    E_LDMA_DBGLV_WARNING, //warning conditions
    E_LDMA_DBGLV_ERR,     //error conditions
    E_LDMA_DBGLV_CRIT,    //critical conditions
    E_LDMA_DBGLV_ALERT,   //action must be taken immediately
    E_LDMA_DBGLV_EMERG,   //system is unusable
    E_LDMA_DBGLV_DEBUG,   //debug-level messages
} LDMA_DbgLv;

//------------------------------------------------------------------------------
/// Description : Set detailed level of LDMA driver debug message
/// @param u8DbgLevel    \b IN  debug level for local dimming LDMA
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_LDMA_SetDbgLevel(MS_U8 u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Description : Set Menuload Number [if need transfer 100 data, then u32MenuldNum =100 ]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u32MenuldNum \b IN: set Dma load to mspi data number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetMenuloadNumber(MS_U8 u8HWNum, MS_U32 u32MenuldNum);
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI Command Format
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8CmdLen \b IN: set mspi spec cmd length
/// @param pu16CmdBuf \b IN: mspi spec cmds
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetSPICommandFormat( MS_U8 u8HWNum,MS_U8 u8CmdLen, MS_U16* pu16CmdBuf);
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI check sum mode
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8SumMode \b IN: set spi data check summode [000: All (start + command + data) 001: Command + data 010: Data only]
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetCheckSumMode(MS_U8 u8HWNum,MS_U8 u8SumMode);
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI trigger mode
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8TriggerMode \b IN: set DMA trigger mspi mode [0:trigger stop 1: one shot real time trigger 2: HW real time trigger]
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetSpiTriggerMode( MS_U8 u8HWNum, MS_U8 u8TriggerMode);
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI Trigger delay time
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param pu16TrigDelay \b IN: set spi clk delay time while trigger done
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetTrigDelay(MS_U8 u8HWNum,MS_U16 *pu16TrigDelay);
//-------------------------------------------------------------------------------------------------
/// Description : Enable ldma cs
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param bEnable \b IN: set DMA for local dimming enable or disable cs
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_EnableCS(MS_U8 u8HWNum, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Description : LDMA Init function to set iobase [should call first]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8ClkHz \b IN: select local dimming period for control LD bank
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_Init(MS_U8 u8HWNum,MS_U8 u8ClkHz);
//-------------------------------------------------------------------------------------------------
/// Description : set LDF base address
/// @param u8AddrIndex \b IN:  doublebuffer for  LDF index
/// @param u32LDFAddr_l \b IN: LDF base Addr for left
/// @param u32LDFAddr_r \b IN: LDF base Addr for right
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetLDFAddr(MS_U8 u8AddrIndex, MS_U32 u32LDFAddr_l,MS_U32 u32LDFAddr_r);
//-------------------------------------------------------------------------------------------------
/// Description : set LDB base address
/// @param u8AddrIndex \b IN:  doublebuffer for  LDB index
/// @param u32LDFAddr_l \b IN: LDB base Addr for left
/// @param u32LDFAddr_r \b IN: LDB base Addr for right
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetLDBAddr(MS_U8 u8AddrIndex, MS_U32 u32LDBAddr_l,MS_U32 u32LDBAddr_r);
//-------------------------------------------------------------------------------------------------
/// Description : set Edge2D base address
/// @param u32Edge2DAddr \b IN:  Edge2DAddr baseaddr
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetEdge2DAddr(MS_U32 u32Edge2DAddr);
//-------------------------------------------------------------------------------------------------
/// Description : set LEDBuf BaseOffset [spi data addr = LDB addr +LEDBuf BaseOffset]
/// @param u32Edge2DAddr \b IN:  LEDBuf BaseOffset
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetLEDBufBaseOffset(MS_U32 u32DataOffset);
//-------------------------------------------------------------------------------------------------
/// Description : Set MIU PackOffset [DMA load mem offset data ,ps:always offset=0]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8PackOffset \b IN: u8PackOffset
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetMIUPackOffset(MS_U8 u8HWNum,MS_U8 u8PackOffset);
//-------------------------------------------------------------------------------------------------
/// Description : Set MIU Pack length
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8PackLength \b IN: DMA load mem data pack format size
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetMIUPackLength(MS_U8 u8HWNum,MS_U8 u8PackLength);
//-------------------------------------------------------------------------------------------------
/// Description : Set YoffEnd
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8YoffEnd \b IN: u8YoffEnd always =led height
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetYoffEnd(MS_U8 u8HWNum,MS_U8 u8YoffEnd);
//-------------------------------------------------------------------------------------------------
/// Description : Set Back light height number
/// @param u8Height \b IN: LDMA height = led height number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetBlHeightDMA(MS_U8 u8Height);
//-------------------------------------------------------------------------------------------------
/// Description : Set Back light Width number
/// @param u8Width \b IN: LDMA Width = led Width number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetBlWidthDMA(MS_U8 u8Width);
//-------------------------------------------------------------------------------------------------
/// Description : Enable ldma for LD
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param bEnable \b IN: set DMA for local dimming enable or disable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_SetDmaEnable(MS_U8 u8HWNum, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Description : Enable LD [last step action]
/// @param bEnable \b IN: set local dimming enable or disable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_LD_Enable(MS_BOOL bEnable);

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
