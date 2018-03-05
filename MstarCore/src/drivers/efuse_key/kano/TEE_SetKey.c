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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

// [Notice]: This file configuration should customerized per CA project 
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <MsTypes.h>
#include <drvAESDMA.h>
#include <drvDSCMB.h>
//#include <drvSEM.h>

#define YY_CAVID 0x04

#define STB_CHIP

// 3-Level KeyLadder input
static MS_U8 u8KL_AESDMA_Input[48] = {
    //Input Key 2
    0x48, 0xd0, 0xcf, 0xaa, 0x00, 0x23, 0x43, 0x6b, 0x09, 0xd9, 0x4b, 0xe2, 0xe3, 0x6e, 0x07, 0x2f,
    //Input Key 1
    0x95, 0x73, 0x93, 0x5f, 0x96, 0x8d, 0xec, 0xb6, 0x51, 0x85, 0xc3, 0xcd, 0xaa, 0x5b, 0x64, 0x18,
    //Input Key 0 (Encrypted Control Word)
    0xda, 0x1d, 0xe9, 0xb9, 0xff, 0xf1, 0x2b, 0x8a, 0xff, 0xa2, 0x14, 0x0d, 0xfa, 0x99, 0xfc, 0x41};

void TEE_SetKey(void)
{
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};

    MS_BOOL ret = FALSE;

    extern MS_BOOL MDrv_SEM_Init(void);
    extern DLL_PUBLIC MS_BOOL MDrv_DSCMB2_Init(void);
    extern DLL_PUBLIC MS_BOOL MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status );
    extern DLL_PUBLIC MS_BOOL MDrv_DSCMB2_SetDefaultCAVid(MS_U32 u32EngId, MS_U32 u32CAVid);

    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_AES,
        .eSrc = E_DSCMB_KL_SRC_SECRET_2,    // By CA project
        .eDst = E_DSCMB_KL_DST_DMA_SK0,     // Do not modify, only use SK0
        .eOutsize = E_DSCMB_KL_128_BITS,    // Always set 128 Bit
        .eKeyType = 0,                      // Don't care when dst is Crypto DMA
        .u32Level = 3,                      // By CA project
        .u32EngID = 0,                      // Don't care when dst is Crypto DMA
        .u32DscID = 0,                      // Don't care when dst is Crypto DMA
        .u8KeyACPU = 0,                     // If eSrc is not from ACPU, this field should be NULL or zero
        .pu8KeyKLIn = u8KL_AESDMA_Input,    // KL input encrypted data
        .bDecrypt = TRUE,                   // bDecrypt
        .bInverse = FALSE,                  // Don't care
        .u32CAVid = YY_CAVID,               // CAVid for output key, the same of DMA
    };
    MDrv_SEM_Init();
    MDrv_DSCMB2_Init();
	  // Run Key Ladder
    ret = MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!! 0x%x\n",(unsigned int)KL_Status);
        return;
    }

    MDrv_AESDMA_SetDefaultCAVid(YY_CAVID); //By project, AESDMA set cavid,
    MDrv_AESDMA_SetKey_Ex(NULL, 0); //Select key from KL
}

