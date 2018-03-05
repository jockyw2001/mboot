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

#ifndef __DRVMIU__
#define __DRVMIU__

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <configs/uboot_board_config.h>
#include <ShareType.h>
#include <drvMIU.h>
#include <MsStr.h>
typedef enum
{
    E_MIU_ID_0=0x00,
    E_MIU_ID_1,
    E_MIU_ID_2,
    E_MIU_ID_MAX
}EN_MIUID;

#define	MIU_MAX_GP_NUM		        (16)         //Max MIU Group number
#define MIU_MAX_GP_PRIORITY_NUM     (16)         //Max MIU Group Priority number

typedef struct
{
    MIU_ID miu_id;
    MS_U8 u8Group;
    char *c_Mmap_Id;
    MS_U32 u32Start;
    MS_U32 u32End;
    MS_U8 clientID[16];
} stMIU_PortectInfo;


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Global Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);
MS_BOOL MsDrv_MIU_Protect(MS_U8  u8Blockx, MS_U8  *pu8ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag);
MS_BOOL MsDrv_MIU_ProtectEx(MS_U8  u8Blockx, MS_U32  *pu32ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag);
void MsDrv_kernelProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End);
void MsDrv_PMProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End);
void MsDrv_NuttxProtect(MS_U8  u8Blockx,MS_U64 u64NuttxStart, MS_U64 u64NuttxEnd);
void MsDrv_MiuInit(void);
MIU_DDR_SIZE dram_size_to_enum(MS_U32 u32DramSize);
char* MsDrv_GetMIUSize(MS_U8 MiuID);
MS_U64 BA2PA(MS_U64 u64usAddr);
MS_U64 PA2BA(MS_U64 u64PhyAddr);
void MSDrv_FSTR_MiuBackup(ST_MIU_INFO * stMiuInfo);
//-------------------------------------------------------------------------------------------------
/// get MIU_ID by input uPhyAddrs
/// @INPUT:  uPhyAddrs                 \b OUT: MIU_ID
//-------------------------------------------------------------------------------------------------
EN_MIUID MsApi_Miu_GetID(MS_U64 uPhyAddrs);


#endif	//__DRVMIU__

