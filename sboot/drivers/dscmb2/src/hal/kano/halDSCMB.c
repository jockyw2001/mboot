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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   halDSCMB.c
/// @brief  Descrambler (DSCMB) HAL Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

//================================================================
//  Include Headers
//================================================================
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif
#define MSOS_TYPE_NOS
#define DSCMB_NO_PRINTF

#include "drvDSCMB.h"
#include "halDSCMB.h"
#include "regDSCMB.h"

//================================================================
//  Defines
//================================================================

#define REG_BASE_BANK                    _u32RegBase
#define DSCMB_MIU_SHT                     3

#define _AND_(flag, bit)                 ((flag) &   (bit) )
#define _SET_(flag, bit)                 ((flag) |   (bit) )
#define _CLR_(flag, bit)                 ((flag) & (~(bit)))

#define REG16_SET(reg, value);            REG16_W(reg, _SET_(REG16_R(reg), value));
#define REG16_CLR(reg, value);            REG16_W(reg, _CLR_(REG16_R(reg), value));
#define REG16_MSK_W(reg, mask, value);    REG16_W((reg), _CLR_(REG16_R(reg), (mask)) | _AND_((value), (mask)));
#define REG32_R(reg)                     *((volatile MS_U32*)(REG_BASE_BANK+ ((reg)<< 2)))

#if defined (MSOS_TYPE_NOS)
#define DSCMB_HW_LOCK(_a_)
#define DSCMB_HW_UNLOCK(_a_)
#else
#define DSCMB_HW_LOCK(_a_)     { if(MDrv_SEM_Lock((_a_), MSOS_WAIT_FOREVER) == FALSE){ }}
#define DSCMB_HW_UNLOCK(_a_)   { if(MDrv_SEM_Unlock((_a_)) == FALSE){ }}
#endif




//================================================================
//  Variable Declaration
//================================================================
static MS_VIRT _u32RegBase = 0x0;
static MS_U32 _u32CAVid = 0x0F;
#if !defined (MSOS_TYPE_NOS)
//4 HW SEM to protect indirect R/W and some shared register
static eSemId _eHW_SemId = E_SEM_CA;
#endif
static MS_BOOL _bUseKL = FALSE;

//================================================================
//  Private Function Delaration
//================================================================
MS_BOOL _HAL_DSCMB_KTE_Write(MS_U32 u32fltid, DSCMB_Key_Type type, MS_U32 u32Field, MS_U32 u32Cmd, MS_U32 u32CAVid, MS_U32 *u32data);
void    _HAL_DSCMB_PidFlt_Reset(MS_U32 u32FltId);

//================================================================
//  Extern Function Delaration
//================================================================

//================================================================
//  Type Translation
//================================================================
HAL_DSCMB_KL_Dst _HAL_DSCMB_GetKLDst(DSCMB_KLDst eDst)
{
    switch (eDst)
    {
        case E_DSCMB_KL_DST_CPU_ACPU:
            return E_HAL_DSCMB_KL_DST_ACPU;
        case E_DSCMB_KL_DST_CPU_SCPU:
            return E_HAL_DSCMB_KL_DST_SCPU;

        case E_DSCMB_KL_DST_KT_LSAD:
            return E_HAL_DSCMB_KL_DST_KT;
        case E_DSCMB_KL_DST_KT_LSAS:
            return E_HAL_DSCMB_KL_DST_KT;
        case E_DSCMB_KL_DST_KT_ESA:
            return E_HAL_DSCMB_KL_DST_KT;

        case E_DSCMB_KL_DST_DMA_SK0:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SK1:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SK2:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SK3:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPS0:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPS1:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPS2:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPS3:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPD0:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPD1:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPD2:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_DMA_SPD3:
            return E_HAL_DSCMB_KL_DST_DMA;
        case E_DSCMB_KL_DST_PRIVATE_0:
            return E_HAL_DSCMB_KL_DST_PRI0;
        case E_DSCMB_KL_DST_PRIVATE_1:
            return E_HAL_DSCMB_KL_DST_PRI1;
        case E_DSCMB_KL_DST_PRIVATE_2:
            return E_HAL_DSCMB_KL_DST_PRI2;
        case E_DSCMB_KL_DST_PRIVATE_3:
            return E_HAL_DSCMB_KL_DST_PRI3;

        default:
            return 0;
    }
    return 0;
}

HAL_DSCMB_KL_Src _HAL_DSCMB_GetKLSrc(DSCMB_KLSrc eSrc)
{
    switch (eSrc)
    {
        case E_DSCMB_KL_SRC_ACPU:
            return E_HAL_DSCMB_KL_SRC_ACPU;
        case E_DSCMB_KL_SRC_SECRET_1:
            return E_HAL_DSCMB_KL_SRC_SK1;
        case E_DSCMB_KL_SRC_SECRET_2:
            return E_HAL_DSCMB_KL_SRC_SK2;
        case E_DSCMB_KL_SRC_SECRET_3:
            return E_HAL_DSCMB_KL_SRC_SK3;
        case E_DSCMB_KL_SRC_SECRET_4:
            return E_HAL_DSCMB_KL_SRC_SK4;
        case E_DSCMB_KL_SRC_SECRET_5:
            return E_HAL_DSCMB_KL_SRC_SK5;
        case E_DSCMB_KL_SRC_SECRET_6:
            return E_HAL_DSCMB_KL_SRC_SK6;
        case E_DSCMB_KL_SRC_SECRET_7:
            return E_HAL_DSCMB_KL_SRC_SK7;
        case E_DSCMB_KL_SRC_SECRET_8:
            return E_HAL_DSCMB_KL_SRC_SK8;
        case E_DSCMB_KL_SRC_SECRET_9:
            return E_HAL_DSCMB_KL_SRC_SK9;
        case E_DSCMB_KL_SRC_SECRET_10:
            return E_HAL_DSCMB_KL_SRC_SK10;
        case E_DSCMB_KL_SRC_SECRET_11:
            return E_HAL_DSCMB_KL_SRC_SK11;
        case E_DSCMB_KL_SRC_SECRET_12:
            return E_HAL_DSCMB_KL_SRC_SK12;
        case E_DSCMB_KL_SRC_SECRET_13:
            return E_HAL_DSCMB_KL_SRC_SK13;
        case E_DSCMB_KL_SRC_SECRET_14:
            return E_HAL_DSCMB_KL_SRC_SK14;
        case E_DSCMB_KL_SRC_SECRET_15:
            return E_HAL_DSCMB_KL_SRC_SK15;
        default:
            return 0;
    }
    return 0;
}


HAL_DSCMB_KL_Type _HAL_DSCMB_GetKLType(DSCMB_KLType eType)
{
    switch (eType)
    {
        case E_DSCMB_KL_TYPE_LV0:
            return E_HAL_DSCMB_KL_TYPE_0;
        case E_DSCMB_KL_TYPE_LV1:
            return E_HAL_DSCMB_KL_TYPE_1;
        case E_DSCMB_KL_TYPE_LV2:
            return E_HAL_DSCMB_KL_TYPE_2;
        case E_DSCMB_KL_TYPE_LV3:
            return E_HAL_DSCMB_KL_TYPE_3;
        case E_DSCMB_KL_TYPE_LV4:
            return E_HAL_DSCMB_KL_TYPE_4;
        case E_DSCMB_KL_TYPE_LV5:
            return E_HAL_DSCMB_KL_TYPE_5;
        case E_DSCMB_KL_TYPE_INF:
            return E_HAL_DSCMB_KL_TYPE_1_KPR;
        case E_DSCMB_KL_TYPE_S5:
            return E_HAL_DSCMB_KL_TYPE_5_SMI;
        default:
            return E_HAL_DSCMB_KL_TYPE_INVALID;
    }
    return E_HAL_DSCMB_KL_TYPE_INVALID;
}

HAL_DSCMB_KL_DstDma _HAL_DSCMB_GetKLDstDMA(DSCMB_KLDst eDst)
{
    switch (eDst)
    {

        case E_DSCMB_KL_DST_DMA_SK0:
            return E_HAL_DSCMB_KL_DSTDMA_SK0;
        case E_DSCMB_KL_DST_DMA_SK1:
            return E_HAL_DSCMB_KL_DSTDMA_SK1;
        case E_DSCMB_KL_DST_DMA_SK2:
            return E_HAL_DSCMB_KL_DSTDMA_SK2;
        case E_DSCMB_KL_DST_DMA_SK3:
            return E_HAL_DSCMB_KL_DSTDMA_SK3;
        case E_DSCMB_KL_DST_DMA_SPS0:
            return E_HAL_DSCMB_KL_DSTDMA_SPS0;
        case E_DSCMB_KL_DST_DMA_SPS1:
            return E_HAL_DSCMB_KL_DSTDMA_SPS1;
        case E_DSCMB_KL_DST_DMA_SPS2:
            return E_HAL_DSCMB_KL_DSTDMA_SPS2;
        case E_DSCMB_KL_DST_DMA_SPS3:
            return E_HAL_DSCMB_KL_DSTDMA_SPS3;
        case E_DSCMB_KL_DST_DMA_SPD0:
            return E_HAL_DSCMB_KL_DSTDMA_SPD0;
        case E_DSCMB_KL_DST_DMA_SPD1:
            return E_HAL_DSCMB_KL_DSTDMA_SPD1;
        case E_DSCMB_KL_DST_DMA_SPD2:
            return E_HAL_DSCMB_KL_DSTDMA_SPD2;
        case E_DSCMB_KL_DST_DMA_SPD3:
            return E_HAL_DSCMB_KL_DSTDMA_SPD3;
        case E_DSCMB_KL_DST_CPU_ACPU:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_CPU_SCPU:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_KT_LSAD:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_KT_LSAS:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_KT_ESA:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_PRIVATE_0:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_PRIVATE_1:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_PRIVATE_2:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
        case E_DSCMB_KL_DST_PRIVATE_3:
            return E_HAL_DSCMB_KL_DSTDMA_NA;

        default:
            return E_HAL_DSCMB_KL_DSTDMA_NA;
    }
    return E_HAL_DSCMB_KL_DSTDMA_NA;
}

HAL_DSCMB_KL_DstSP _HAL_DSCMB_GetKLDstSP(DSCMB_KLDst eDst)
{
    switch (eDst)
    {
        case E_DSCMB_KL_DST_DMA_SPS0:
            return E_HAL_DSCMB_KL_DST_SP_SPS0;
        case E_DSCMB_KL_DST_DMA_SPS1:
            return E_HAL_DSCMB_KL_DST_SP_SPS1;
        case E_DSCMB_KL_DST_DMA_SPS2:
            return E_HAL_DSCMB_KL_DST_SP_SPS2;
        case E_DSCMB_KL_DST_DMA_SPS3:
            return E_HAL_DSCMB_KL_DST_SP_SPS3;
        case E_DSCMB_KL_DST_DMA_SPD0:
            return E_HAL_DSCMB_KL_DST_SP_SPD0;
        case E_DSCMB_KL_DST_DMA_SPD1:
            return E_HAL_DSCMB_KL_DST_SP_SPD1;
        case E_DSCMB_KL_DST_DMA_SPD2:
            return E_HAL_DSCMB_KL_DST_SP_SPD2;
        case E_DSCMB_KL_DST_DMA_SPD3:
            return E_HAL_DSCMB_KL_DST_SP_SPD3;
        default:
            return E_HAL_DSCMB_KL_DST_SP_NA;
    }
    return E_HAL_DSCMB_KL_DST_SP_NA;
}

HAL_DSCMB_KL_DstKT _HAL_DSCMB_GetKLDstKT(DSCMB_KLDst eDst)
{
    switch (eDst)
    {
        case E_DSCMB_KL_DST_KT_LSAD:
            return E_HAL_DSCMB_KL_DSTKT_LSAD;
        case E_DSCMB_KL_DST_KT_LSAS:
            return E_HAL_DSCMB_KL_DSTKT_LSAS;
        case E_DSCMB_KL_DST_KT_ESA:
            return E_HAL_DSCMB_KL_DSTKT_ESA;

        case E_DSCMB_KL_DST_DMA_SK0:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SK1:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SK2:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SK3:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPS0:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPS1:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPS2:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPS3:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPD0:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPD1:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPD2:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_DMA_SPD3:
            return E_HAL_DSCMB_KL_DSTKT_ESA;

        case E_DSCMB_KL_DST_CPU_ACPU:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_CPU_SCPU:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_PRIVATE_0:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_PRIVATE_1:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_PRIVATE_2:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
        case E_DSCMB_KL_DST_PRIVATE_3:
            return E_HAL_DSCMB_KL_DSTKT_ESA;

        default:
            return E_HAL_DSCMB_KL_DSTKT_ESA;
    }
    return E_HAL_DSCMB_KL_DSTKT_ESA;
}


HAL_DSCMB_FSCB _HAL_DSCMB_GetFSCB(DSCMB_FSCB eFSCB)
{
    switch (eFSCB)
    {
        case DSCMB_FSCB_UNCHG:
            return E_HAL_DSCMB_FSCB_UNCHG;
        case DSCMB_FSCB_CLEAR:
            return E_HAL_DSCMB_FSCB_B00;
        case DSCMB_FSCB_EVEN:
            return E_HAL_DSCMB_FSCB_B10;
        case DSCMB_FSCB_ODD:
            return E_HAL_DSCMB_FSCB_B11;
        default:
            ;
    }

    return E_HAL_DSCMB_FSCB_UNCHG;
}

HAL_DSCMB_KeyType _HAL_DSCMB_GetKeyType(DSCMB_Key_Type eKeyType)
{
    switch (eKeyType)
    {
        case E_DSCMB_KEY_CLEAR:
            return E_HAL_DSCMB_KEY_CLEAR;
        case E_DSCMB_KEY_EVEN:
            return E_HAL_DSCMB_KEY_EVEN;
        case E_DSCMB_KEY_ODD:
            return E_HAL_DSCMB_KEY_ODD;
        default:
            ;
    }

    return E_HAL_DSCMB_KEY_UNDEFINE;
}


HAL_DSCMB_KTE_WriteType _HAL_DSCMB_GetEngType(DSCMB_Eng_Type type)
{
    switch (type)
    {
        case E_DSCMB_ENG_LSAD:
            return E_HAL_DSCMB_ENG_LSAD;
        case E_DSCMB_ENG_ESA:
            return E_HAL_DSCMB_ENG_ESA;
        case E_DSCMB_ENG_LSAS:
            return E_HAL_DSCMB_ENG_LSAS;
        default:
            ;
    }

    return E_HAL_DSCMB_ENG_NUM;
}

HAL_DSCMB_KTE_WriteType _HAL_DSCMB_GetIVType(DSCMB_Eng_Type eType)
{
    HAL_DSCMB_KTE_WriteType wType = E_HAL_DSCMB_ENG_IV_ESA;

    switch (eType)
    {
        case E_DSCMB_ENG_LSAD:
            wType = E_HAL_DSCMB_ENG_IV_LSAD;
            break;

        case E_DSCMB_ENG_ESA:
            wType = E_HAL_DSCMB_ENG_IV_ESA;
            break;

        case E_DSCMB_ENG_LSAS:
            wType = E_HAL_DSCMB_ENG_IV_LSAS;
            break;

        default:
            wType = E_HAL_DSCMB_ENG_IV_ESA;
            break;
    }

    return wType;
}

HAL_DSCMB_MainAlgo_Type _HAL_DSCMB_GetAlgoType(DSCMB_MainAlgo_Type type)
{
    switch (type)
    {
        case E_DSCMB_MAIN_ALGO_AES:
            return E_HAL_DSCMB_MAIN_ALGO_AES;
        case E_DSCMB_MAIN_ALGO_CSA2:
            return E_HAL_DSCMB_MAIN_ALGO_CSA2;
        case E_DSCMB_MAIN_ALGO_DES:
            return E_HAL_DSCMB_MAIN_ALGO_DES;
        case E_DSCMB_MAIN_ALGO_TDES:
            return E_HAL_DSCMB_MAIN_ALGO_TDES;
        case E_DSCMB_MAIN_ALGO_MULTI2:
            return E_HAL_DSCMB_MAIN_ALGO_MULTI2;
        case E_DSCMB_MAIN_ALGO_CSA2_CONF:
            return E_HAL_DSCMB_MAIN_ALGO_CSA2_CONF;
        case E_DSCMB_MAIN_ALGO_CSA3:
            return E_HAL_DSCMB_MAIN_ALGO_CSA3;
        case E_DSCMB_MAIN_ALGO_ASA:
            return E_HAL_DSCMB_MAIN_ALGO_ASA;
        case E_DSCMB_MAIN_ALGO_TCSA3:
            return E_HAL_DSCMB_MAIN_ALGO_TCSA3;
        default:
            // [FIXME] Upper layer should check this parameter .
            ;
    }

    return E_HAL_DSCMB_MAIN_ALGO_NUM;
}


HAL_DSCMB_SubAlgo_Type _HAL_DSCMB_GetSubAlgoType(DSCMB_SubAlgo_Type type)
{
    switch (type)
    {
        case E_DSCMB_SUB_ALGO_MDI_CBC:
            return E_HAL_DSCMB_SUB_ALGO_MDI_CBC;
        case E_DSCMB_SUB_ALGO_MDI_RCBC:
            return E_HAL_DSCMB_SUB_ALGO_MDI_RCBC;
        case E_DSCMB_SUB_ALGO_MDD_CBC:
            return E_HAL_DSCMB_SUB_ALGO_MDD_CBC;
        case E_DSCMB_SUB_ALGO_MDD_RCBC:
            return E_HAL_DSCMB_SUB_ALGO_MDD_RCBC;
        case E_DSCMB_SUB_ALGO_LEADING_CLEAR:
            return E_HAL_DSCMB_SUB_ALGO_LEADING_CLEAR;
        case E_DSCMB_SUB_ALGO_ECB:
            return E_HAL_DSCMB_SUB_ALGO_ECB;
        case E_DSCMB_SUB_ALGO_CBC:
            return E_HAL_DSCMB_SUB_ALGO_CBC;
        case E_DSCMB_SUB_ALGO_CTR:
            return E_HAL_DSCMB_SUB_ALGO_CTR;
        case E_DSCMB_SUB_ALGO_OFB:
            return E_HAL_DSCMB_SUB_ALGO_OFB;
        default:
            // [FIXME] Upper layer should check this parameter .
            ;
    }

    return E_HAL_DSCMB_SUB_ALGO_NUM;
}

HAL_DSCMB_ResSBAlgo_Type _HAL_DSCMB_GetResSBType(DSCMB_ResSBAlgo_Type type)
{
    switch (type)
    {
        case E_DSCMB_RESSB_ALGO_CLR:
            return E_HAL_DSCMB_RESSB_ALGO_CLR;
        case E_DSCMB_RESSB_ALGO_CTS:
            return E_HAL_DSCMB_RESSB_ALGO_CTS;
        case E_DSCMB_RESSB_ALGO_SCTE52:
            return E_HAL_DSCMB_RESSB_ALGO_SCTE52;
        case E_DSCMB_RESSB_ALGO_XORIV1:
            return E_HAL_DSCMB_RESSB_ALGO_XORIV1;
        case E_DSCMB_RESSB_ALGO_OC_M:
            return E_HAL_DSCMB_RESSB_ALGO_OC_M;
        case E_DSCMB_RESSB_ALGO_XORIV2:
            return E_HAL_DSCMB_RESSB_ALGO_XORIV2;
        case E_DSCMB_RESSB_ALGO_CTR:
            return E_HAL_DSCMB_RESSB_ALGO_CTR;
        default:
            ;
    }

    return E_HAL_DSCMB_RESSB_ALGO_NUM;
}

MS_U32 _HAL_DSCMB_GetKLEng(DSCMB_KL_SelEng eKLEng)
{
    switch(eKLEng)
    {
        case E_DSCMB_KL_SEL_CW:
            return DSCMB_CWKL;
        case E_DSCMB_KL_SEL_PVR:
            return DSCMB_PVRKL;
        case E_DSCMB_KL_SEL_DEFAULT:
        default:
            return DSCMB_PVRKL;
    }
}

MS_U32 _HAL_DSCMB_GetKLKdfType(KDF_TYPE eHashSel)
{
    switch(eHashSel)
    {
        case E_KDF_HASH_SHA1:
            return 0;
        case E_KDF_HASH_SHA256:
            return 1;
        default:
            return 0;
    }
}

MS_U32 _HAL_DSCMB_GetKLKdfHWKeyId(KDF_HW_KEY_SEL eHWKeySel)
{
    switch(eHWKeySel)
    {
        case E_KDF_HW_KEY1:
            return 0;
        case E_KDF_HW_KEY2:
            return 1;
        case E_KDF_HW_KEY3:
            return 2;
        case E_KDF_HW_KEY4:
            return 3;
        default:
            return 0;
    }
}

MS_BOOL HAL_DSCMB_OTPEnDSCMB(void)
{
    MS_U32 u32CH0SWCfg = 0;
    u32CH0SWCfg = REG32_R(REG_OTP_CH0_SWICH_COMB);

    return u32CH0SWCfg?TRUE:FALSE;
}

//================================================================
//  Function Implementation
//================================================================
static void REG32_W(MS_VIRT u32Reg, MS_U32 u32Value)
{
    *((volatile MS_U32*)(REG_BASE_BANK+ ((u32Reg)<< 2))) = u32Value;
}

static MS_U16 REG16_R(MS_VIRT u32Reg)
{
    return REG32_R(u32Reg) & 0x0000FFFFUL;
}

static void REG16_W(MS_VIRT u32Reg, MS_U16 u16Value)
{
    MS_U32 u32Value = (REG32_R(u32Reg) & 0xFFFF0000UL) | (u16Value & 0x0000FFFFUL);
    REG32_W(u32Reg, u32Value);
}

static void TSP32_IdrW(MS_U32 u32Addr, MS_U32 u32Value)
{

    REG16_W(REG_TSP_IDR_ADDR_L, u32Addr);
    REG16_W(REG_TSP_IDR_ADDR_H, u32Addr >> 16);

    REG16_W(REG_TSP_IDR_WRITE_L, u32Value);
    REG16_W(REG_TSP_IDR_WRITE_H, u32Value >> 16);

    REG16_W(REG_TSP_IDR_CTRL, TSP_IDR_START | TSP_IDR_WRITE);
}

static MS_U32 TSP32_IdrR(MS_U32 u32Reg)
{
    REG16_W(REG_TSP_IDR_ADDR_L,  u32Reg);
    REG16_W(REG_TSP_IDR_ADDR_H,  u32Reg >> 16);
    REG16_W(REG_TSP_IDR_CTRL, TSP_IDR_START | TSP_IDR_READ);

    return (REG16_R(REG_TSP_IDR_READ_H) << 16) | REG16_R(REG_TSP_IDR_READ_L);
}

MS_BOOL HAL_DSCMB_GetCap(MS_U32 u32EngId, HAL_DSCMB_Query_Type eQueryType, void* pInData, void* pOutData)
{
    MS_BOOL bRet = TRUE;
    *((MS_U32*)pOutData) = 0;

    switch(eQueryType)
    {
    case E_HAL_DSCMB_CAP_ENGINE_NUM:
        *((MS_U32*)pOutData) = HAL_DSCMB_ENG_NUM;
        break;
    case E_HAL_DSCMB_CAP_FLT_NUM:
        *((MS_U32*)pOutData) = HAL_DSCMB_KTE_MAX/3;
        break;
    case E_HAL_DSCMB_CAP_PIDFILTER_MAP_START:
        *((MS_U32*)pOutData) = 1; // pidslotMap 0 is reserved
        break;
    case E_HAL_DSCMB_CAP_PIDFILTER_MAP_END:
        *((MS_U32*)pOutData) = HAL_DSCMB_PIDFLT_NUM-1;
        break;
    case E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM:
        switch (*((DSCMB_Type*)pInData))
        {
            case E_DSCMB_TYPE_CSA:
            case E_DSCMB_TYPE_DES:
            case E_DSCMB_TYPE_AES:
            case E_DSCMB_TYPE_AES_ECB:
            case E_DSCMB_TYPE_AES_SCTE52:
            case E_DSCMB_TYPE_TDES_ECB:
            case E_DSCMB_TYPE_TDES_SCTE52:
            case E_DSCMB_TYPE_SYN_AES:
            case E_DSCMB_TYPE_MULTI2:
            case E_DSCMB_TYPE_CSA3:
            case E_DSCMB_TYPE_CSA_CONF:
            case E_DSCMB_TYPE_AES_CTR:
                *((MS_U32*)pOutData) = (MS_U32)TRUE;
                break;
            case E_DSCMB_TYPE_NSA_AS_ESA:
            case E_DSCMB_TYPE_OC:
                *((MS_U32*)pOutData) = (MS_U32)FALSE;
                break;
            default:
                *((MS_U32*)pOutData) = (MS_U32)FALSE;
                break;
        }
        break;
    default:
        *((MS_U32*)pOutData) = 0xFFFFFFFFUL;
        bRet = FALSE;
        break;
    }
    return bRet;
}


MS_BOOL HAL_DSCMB_SetCAVid(MS_U32 u32CAVid )
{
    _u32CAVid = u32CAVid;
    return TRUE;
}

MS_U32 HAL_DSCMB_GetCAVid(void)
{
    return _u32CAVid;
}

MS_BOOL HAL_DSCMB_SetBank(MS_VIRT u32Bank)
{
    REG_BASE_BANK = u32Bank;

    return TRUE;
}

MS_BOOL _HAL_DSCMB_KTE_Write_Ex(MS_U32 u32fltid, DSCMB_Key_Type type, MS_U32 u32Field, MS_U32 u32Cmd, MS_U32 u32CAVid,MS_U32 *u32data, MS_BOOL bIsKL)
{
    MS_U32 u32Tmp  = 0;
    MS_U32 u32Alog = 0;

    //[NOTE] Type translation //
    HAL_DSCMB_KeyType etype = _HAL_DSCMB_GetKeyType(type);

    if ((u32fltid >= HAL_DSCMB_PIDIDX_MAX) || (~REG_KTE_SEL_MSK & u32Field)  )
    {
        // ivalid pid id
        return FALSE ;
    }


    if(u32Cmd == REG_KTE_CMD_WRITE)
    {
        // file in write data
        REG32_W(REG_KTE_DATA0, u32data[0] );
        REG32_W(REG_KTE_DATA1, u32data[1] );
        REG32_W(REG_KTE_DATA2, u32data[2] );
        REG32_W(REG_KTE_DATA3, u32data[3] );
        REG32_W(REG_KTE_DATA4, u32data[4] );
    }

    //Read destination
    u32Tmp = REG32_R(REG_KTE_CMD_CTRL1);

    //Setting entropy
    if(E_HAL_DSCMB_CAVID2 ==  u32CAVid) //CA will check key entropy in hardware
    {
        u32Tmp &= ~REG_KTE_ETP_MSK;

        if(REG_KTE_SEL_ESA == u32Field)
        {
            u32Alog = (REG32_R(REG_KTE_DATA(1))&REG_KET_SWT_ALG_MSK)>>REG_KET_SWT_ALG_SHT;
            if((E_HAL_DSCMB_MAIN_ALGO_CSA2 != u32Alog) && (E_HAL_DSCMB_MAIN_ALGO_CSA2_CONF != u32Alog)
                && (E_HAL_DSCMB_MAIN_ALGO_MULTI2 != u32Alog))
            {
                u32Alog = REG_KTE_ETP_128;
                u32Tmp |= (u32Alog<<REG_KTE_ETP_SHT);
            }
        }

        if(REG_KTE_SEL_LSAS == u32Field)
        {
            u32Alog = (REG32_R(REG_KTE_DATA(2))&REG_KET_SWT_ALG_MSK)>>REG_KET_SWT_ALG_SHT;
            if((E_HAL_DSCMB_MAIN_ALGO_CSA2 != u32Alog) && (E_HAL_DSCMB_MAIN_ALGO_CSA2_CONF != u32Alog)
                && (E_HAL_DSCMB_MAIN_ALGO_MULTI2 != u32Alog))
            {
                u32Alog = REG_KTE_ETP_128;
                u32Tmp |= (u32Alog<<REG_KTE_ETP_SHT);
            }
        }

        if(REG_KTE_SEL_LSAD == u32Field)
        {
            u32Alog = (REG32_R(REG_KTE_DATA(3))&REG_KET_SWT_ALG_MSK)>>REG_KET_SWT_ALG_SHT;
            if((E_HAL_DSCMB_MAIN_ALGO_CSA2 != u32Alog) && (E_HAL_DSCMB_MAIN_ALGO_CSA2_CONF != u32Alog)
                && (E_HAL_DSCMB_MAIN_ALGO_MULTI2 != u32Alog))
            {
                u32Alog = REG_KTE_ETP_128;
                u32Tmp |= (u32Alog<<REG_KTE_ETP_SHT);
            }
        }

    }

    MS_U32          u32KteField = REG_KTE_FIELD_LSAD;
    HAL_DSCMB_FSCB  _eFSCB      = E_HAL_DSCMB_FSCB_UNCHG;


    // write destination with KeyFSCB
    REG32_W(REG_KTE_CMD_CTRL1 , (u32Tmp & REG_KTE_ETP_MSK) | (u32fltid & REG_KTE_FLTID_MSK) | etype << REG_KTE_SCB_SHT | ((_eFSCB<<REG_KTE_FSCB_SHT)&REG_KTE_FSCB_MSK) );

    // prepare wite command
    u32Tmp = (u32Field & REG_KTE_SEL_MSK) | (u32Cmd) |(u32CAVid << REG_KTE_CAVID_SHT) ;

    if(bIsKL || _bUseKL)    u32Tmp |= REG_KTE_USE_KL;

    // start command
    REG32_W(REG_KTE_CMD_CTRL0 , u32Tmp | REG_KTE_AKI_GO);

    while( (REG32_R(REG_KTE_CMD_CTRL0) & REG_KTE_AKI_GO ) != 0 ) ;

    u32Tmp = REG32_R(REG_KTE_CMD_CTRL0);
    u32Tmp = (MS_U32)((u32Tmp&REG_KTE_RESP_MSK)>>REG_KTE_RESP_SFT);

    return u32Tmp == 0?TRUE:FALSE;
}


MS_BOOL HAL_DSCMB_KTE_Key_Ctrl_Ex(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wType, MS_U32 u32CAVid, MS_BOOL bEnable, MS_BOOL bIsKL)
{
    MS_U32 u32Field = 0;
    MS_U32 u32Cmd = 0;
    HAL_DSCMB_KTE_WriteType ewType = _HAL_DSCMB_GetEngType(wType) ;

    if ((u32FltId >= HAL_DSCMB_PIDIDX_MAX)  )
    {
        // ivalid pid id

        return FALSE ;
    }

    u32Field = ((ewType << REG_KTE_SEL_SFT) & REG_KTE_SEL_MSK);
    if(bEnable == TRUE)
    {
        u32Cmd = REG_KTE_CMD_KEY_EN;
    }
    else
    {
        u32Cmd = REG_KTE_CMD_KEY_DIS;
    }

    return _HAL_DSCMB_KTE_Write_Ex(u32FltId , eType , u32Field, u32Cmd, u32CAVid, NULL, bIsKL);
}


//=======================================
//      Key Ladder Interface
//=======================================
MS_BOOL HAL_DSCMB_KL_Start(DSCMB_KL_SelEng eKLEng)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_DSCMB_KL_CTRL(u32KLEng));
    u32Tmp |=0x1;

    REG32_W(REG_DSCMB_KL_CTRL(u32KLEng),u32Tmp);

    while(1)
    {
        u32Tmp = REG32_R(REG_DSCMB_KL_CTRL(u32KLEng));
        if((u32Tmp & 0x1) == 0x0)   break;
    }

    return HAL_DSCMB_KL_ErrStatus(eKLEng);
}

MS_BOOL HAL_DSCMB_KL_Reset(DSCMB_KL_SelEng eKLEng)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    if(u32KLEng == DSCMB_CWKL)
    {
        u32Tmp = REG32_R(REG_DSCMB_KL_SWRST);
        u32Tmp |= REG_DSCMB_KL_SWRST_CW;
        REG32_W(REG_DSCMB_KL_SWRST, u32Tmp);
        u32Tmp &= ~REG_DSCMB_KL_SWRST_CW;
        REG32_W(REG_DSCMB_KL_SWRST, u32Tmp);
    }
    else if(u32KLEng == DSCMB_PVRKL)
    {
        u32Tmp = REG32_R(REG_DSCMB_KL_SWRST);
        u32Tmp |= REG_DSCMB_KL_SWRST_PVR;

        REG32_W(REG_DSCMB_KL_SWRST, u32Tmp);
        u32Tmp &= ~REG_DSCMB_KL_SWRST_PVR;

        REG32_W(REG_DSCMB_KL_SWRST, u32Tmp);
    }
    //clear keyladder ctrl register
    REG32_W(REG_DSCMB_KL_CTRL(u32KLEng), 0);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_KeyProp(DSCMB_KL_SelEng eKLEng, MS_U32 u32CAVid, MS_U32 u32KeyUsg, MS_U32 u32KeyEtpy)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    if(NULL == u32CAVid)
    {
        u32CAVid = _u32CAVid;
    }

    u32Tmp |= (u32CAVid    <<0);
    u32Tmp |= (u32KeyUsg    <<8);
    u32Tmp |= (u32KeyEtpy   <<16);
    #define KDF_ALGO_VSF_EN  0x00100000
    #define KDF_ALGO_PSM_EN  0x00200000
    #define KDF_ALGO_FKD_EN  0x00400000
    u32Tmp |= (KDF_ALGO_VSF_EN | KDF_ALGO_PSM_EN | KDF_ALGO_FKD_EN);
    #undef KDF_ALGO_VSF_EN
    #undef KDF_ALGO_PSM_EN
    #undef KDF_ALGO_FKD_EN
    REG32_W(REG_DSCMB_KL_KEY_PROP(u32KLEng),u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_KeyBus_Ex(DSCMB_KL_SelEng eKLEng, MS_U32 u32FltId, MS_U32 u32Field, DSCMB_Key_Type eKeyType, DSCMB_KLDst eDst, HAL_DSCMB_FSCB eFSCB)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;
    HAL_DSCMB_KeyType eHalKeyType;
    HAL_DSCMB_KL_DstDma eHalDstDma;
    HAL_DSCMB_KL_DstKT  eHalDstKt;

    eHalKeyType = _HAL_DSCMB_GetKeyType(eKeyType);
    eHalDstKt = _HAL_DSCMB_GetKLDstKT(eDst);

    if(E_DSCMB_KL_DST_DMA_CSSK == eDst)
    {
        eHalDstDma = E_HAL_DSCMB_KL_DSTDMA_CSSK;
    }
    else
    {
        eHalDstDma = _HAL_DSCMB_GetKLDstDMA(eDst);
    }

    u32Tmp |= (eHalDstDma   <<  DSCMB_KL_KEYBUS_DSTDMA_SHT);
    u32Tmp |= (eHalKeyType  <<  DSCMB_KL_KEYBUS_SCB_SHT);
    u32Tmp |= (eFSCB        <<  DSCMB_KL_KEYBUS_FSCB_SHT);
    u32Tmp |= (eHalDstKt    <<  DSCMB_KL_KEYBUS_FIELD_SHT);
    u32Tmp |= (u32FltId     <<  DSCMB_KL_KEYBUS_PIDNO_SHT);

    REG32_W(REG_DSCMB_KL_KEY_BUS(u32KLEng),u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_KeyBus(DSCMB_KL_SelEng eKLEng, MS_U32 u32FltId, MS_U32 u32Field, DSCMB_Key_Type eKeyType, DSCMB_KLDst eDst)
{
    // Set FSCB= E_HAL_DSCMB_FSCB_UNCHG
    return HAL_DSCMB_KL_KeyBus_Ex( eKLEng, u32FltId, u32Field, eKeyType, eDst, E_HAL_DSCMB_FSCB_UNCHG);
}

MS_BOOL HAL_DSCMB_KL_Ctrl(DSCMB_KL_SelEng eKLEng, DSCMB_KLEng eAlgo, DSCMB_KLType eType, DSCMB_KLSrc eSrc, DSCMB_KLDst eDst)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;
    HAL_DSCMB_KL_Type eHalKLType = _HAL_DSCMB_GetKLType(eType);
    HAL_DSCMB_KL_Src eHalKLSrc = _HAL_DSCMB_GetKLSrc(eSrc);
    HAL_DSCMB_KL_Dst eHalKLDst = E_HAL_DSCMB_KL_DST_KT;

    if(E_DSCMB_KL_DST_DMA_CSSK == eDst)
    {
        eHalKLDst = E_HAL_DSCMB_KL_DST_CSSK;
    }
    else
    {
        eHalKLDst = _HAL_DSCMB_GetKLDst(eDst);
    }

    if(E_HAL_DSCMB_KL_TYPE_INVALID == eHalKLType)
    {
        return FALSE;
    }

    u32Tmp = REG32_R(REG_DSCMB_KL_CTRL(u32KLEng));

    if(eAlgo == E_DSCMB_KL_AES)
    {
        u32Tmp |= 0x2;
    }

    u32Tmp &= 0xFFF0F00F; //clean
    u32Tmp |= ((eHalKLType<<4)&0xF0);
    u32Tmp |= ((eHalKLSrc<<8)&0xF00);
    u32Tmp |= ((eHalKLDst<<16)&0xF0000);

    REG32_W(REG_DSCMB_KL_CTRL(u32KLEng),u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_GetACPUOut(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Key, MS_U32 u32Size)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    if(u32Size>16)
    {
        return FALSE;
    }

    u32Tmp = REG32_R(REG_DSCMB_KL_SW_CW(u32KLEng)+3);
    pu8Key[0] =  (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Key[1] =  (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Key[2] =  (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Key[3] =  (MS_U8)((u32Tmp&0x000000ff)>>0);
    u32Tmp = REG32_R(REG_DSCMB_KL_SW_CW(u32KLEng)+2);
    pu8Key[4] =  (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Key[5] =  (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Key[6] =  (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Key[7] =  (MS_U8)((u32Tmp&0x000000ff)>>0);
    u32Tmp = REG32_R(REG_DSCMB_KL_SW_CW(u32KLEng)+1);
    pu8Key[8] =  (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Key[9] =  (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Key[10] = (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Key[11] = (MS_U8)((u32Tmp&0x000000ff)>>0);
    u32Tmp = REG32_R(REG_DSCMB_KL_SW_CW(u32KLEng)+0);
    pu8Key[12] = (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Key[13] = (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Key[14] = (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Key[15] = (MS_U8)((u32Tmp&0x000000ff)>>0);

    return TRUE;
}


MS_BOOL HAL_DSCMB_KL_ACPURootKey(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Key)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);

    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+1,(pu8Key[8]  <<24)|(pu8Key[9]  <<16)|(pu8Key[10] <<8)|(pu8Key[11] <<0));
    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+2,(pu8Key[4]  <<24)|(pu8Key[5]  <<16)|(pu8Key[6]  <<8)|(pu8Key[7]  <<0));
    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+3,(pu8Key[0]  <<24)|(pu8Key[1]  <<16)|(pu8Key[2]  <<8)|(pu8Key[3]  <<0));

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_Input(DSCMB_KL_SelEng eKLEng, MS_U32 u32In, MS_U8 *pu8Key)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);

    if(u32In == 0)
    {
        REG32_W(REG_DSCMB_KL_ENC_CW(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
        REG32_W(REG_DSCMB_KL_ENC_CW(u32KLEng)+1,(pu8Key[8] <<24) |(pu8Key[9] <<16) |(pu8Key[10] <<8)|(pu8Key[11] <<0));
        REG32_W(REG_DSCMB_KL_ENC_CW(u32KLEng)+2,(pu8Key[4]<<24)  |(pu8Key[5]<<16)  |(pu8Key[6] <<8) |(pu8Key[7] <<0));
        REG32_W(REG_DSCMB_KL_ENC_CW(u32KLEng)+3,(pu8Key[0]<<24)  |(pu8Key[1]<<16)  |(pu8Key[2]<<8)  |(pu8Key[3]<<0));
    }

    if(u32In == 1)
    {
        REG32_W(REG_DSCMB_KL_KEY1(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
        REG32_W(REG_DSCMB_KL_KEY1(u32KLEng)+1,(pu8Key[8] <<24) |(pu8Key[9] <<16) |(pu8Key[10] <<8)|(pu8Key[11] <<0));
        REG32_W(REG_DSCMB_KL_KEY1(u32KLEng)+2,(pu8Key[4]<<24)  |(pu8Key[5]<<16)  |(pu8Key[6] <<8) |(pu8Key[7] <<0));
        REG32_W(REG_DSCMB_KL_KEY1(u32KLEng)+3,(pu8Key[0]<<24)  |(pu8Key[1]<<16)  |(pu8Key[2]<<8)  |(pu8Key[3]<<0));
    }
    if(u32In == 2)
    {
        REG32_W(REG_DSCMB_KL_KEY2(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
        REG32_W(REG_DSCMB_KL_KEY2(u32KLEng)+1,(pu8Key[8] <<24) |(pu8Key[9] <<16) |(pu8Key[10] <<8)|(pu8Key[11] <<0));
        REG32_W(REG_DSCMB_KL_KEY2(u32KLEng)+2,(pu8Key[4]<<24)  |(pu8Key[5]<<16)  |(pu8Key[6] <<8) |(pu8Key[7] <<0));
        REG32_W(REG_DSCMB_KL_KEY2(u32KLEng)+3,(pu8Key[0]<<24)  |(pu8Key[1]<<16)  |(pu8Key[2]<<8)  |(pu8Key[3]<<0));
    }
    if(u32In == 3)
    {
        REG32_W(REG_DSCMB_KL_KEY3(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
        REG32_W(REG_DSCMB_KL_KEY3(u32KLEng)+1,(pu8Key[8] <<24) |(pu8Key[9] <<16) |(pu8Key[10] <<8)|(pu8Key[11] <<0));
        REG32_W(REG_DSCMB_KL_KEY3(u32KLEng)+2,(pu8Key[4]<<24)  |(pu8Key[5]<<16)  |(pu8Key[6] <<8) |(pu8Key[7] <<0));
        REG32_W(REG_DSCMB_KL_KEY3(u32KLEng)+3,(pu8Key[0]<<24)  |(pu8Key[1]<<16)  |(pu8Key[2]<<8)  |(pu8Key[3]<<0));
    }
    if(u32In == 4)
    {
        REG32_W(REG_DSCMB_KL_KEY4(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
        REG32_W(REG_DSCMB_KL_KEY4(u32KLEng)+1,(pu8Key[8] <<24) |(pu8Key[9] <<16) |(pu8Key[10] <<8)|(pu8Key[11] <<0));
        REG32_W(REG_DSCMB_KL_KEY4(u32KLEng)+2,(pu8Key[4]<<24)  |(pu8Key[5]<<16)  |(pu8Key[6] <<8) |(pu8Key[7] <<0));
        REG32_W(REG_DSCMB_KL_KEY4(u32KLEng)+3,(pu8Key[0]<<24)  |(pu8Key[1]<<16)  |(pu8Key[2]<<8)  |(pu8Key[3]<<0));
    }

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_ErrMsg(DSCMB_KL_SelEng eKLEng, MS_U32* pu32ErrMsg)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    if(NULL == pu32ErrMsg)
        return  FALSE;

    u32Tmp = REG32_R(REG_DSCMB_KL_ERR_MSG(u32KLEng));
    *pu32ErrMsg = u32Tmp;

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_ErrStatus(DSCMB_KL_SelEng eKLEng)
{
    MS_U32 u32ErrMsg = 0;
    MS_U32 i = 0;
    MS_U32 u32ErrorFlag = 1;
    HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);

    for(i = 0;i<REG_DSCMB_KL_ERR_MSG_SIZE;i++) //check each error flag
    {
        if((u32ErrMsg & (u32ErrorFlag << i)) >> i)
        {
            switch(i)
            {
                case E_HAL_DSCMB_KL_ERR_MSG_KDF:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_KL:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_AES:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_TDES:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_LSA:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_ESA:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_DMA:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_NONCE:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_UACPU:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_SEP:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_LUT:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_TYPE:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_PRV:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_HK:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_DACPU:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_BADRK:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_ILUT:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_RSV1:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_KLTYPE:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_ZERO:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_RSV2:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_RSV3:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_KCV:
                    break;
                case E_HAL_DSCMB_KL_ERR_MSG_BADIK:
                    break;
                default:
                    break;

            }

        }

    }

    if( (u32ErrMsg & ErrStatusMak) == 0)
    {
        //no error flag raised
        return TRUE;
    }
    //err happend
    return FALSE;
}

MS_BOOL HAL_DSCMB_KL_KDF_Busy(DSCMB_KL_SelEng eKLEng, MS_BOOL *pbBusy)
{
    //  MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_DSCMB_KL_KDF_STATUS);
    if(u32Tmp == 0)
    {
        *pbBusy = FALSE;
    }
    else
    {
        *pbBusy = TRUE;
    }

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_ETSI_Nonce(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Nonce)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);

    REG32_W(REG_DSCMB_KL_NONCE(u32KLEng)+0,(pu8Nonce[12] <<24)|(pu8Nonce[13] <<16)|(pu8Nonce[14] <<8)|(pu8Nonce[15] <<0));
    REG32_W(REG_DSCMB_KL_NONCE(u32KLEng)+1,(pu8Nonce[8] <<24) |(pu8Nonce[9] <<16) |(pu8Nonce[10] <<8)|(pu8Nonce[11] <<0));
    REG32_W(REG_DSCMB_KL_NONCE(u32KLEng)+2,(pu8Nonce[4]<<24)  |(pu8Nonce[5]<<16)  |(pu8Nonce[6] <<8) |(pu8Nonce[7] <<0));
    REG32_W(REG_DSCMB_KL_NONCE(u32KLEng)+3,(pu8Nonce[0]<<24)  |(pu8Nonce[1]<<16)  |(pu8Nonce[2]<<8)  |(pu8Nonce[3]<<0));

    MS_U32 u32Tmp = REG32_R(REG_DSCMB_KL_CTRL(u32KLEng));
    u32Tmp |= REG_DSCMB_KL_CTRL_NONCE_EN;
    REG32_W(REG_DSCMB_KL_CTRL(u32KLEng), u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_ETSI_Response(DSCMB_KL_SelEng eKLEng, MS_U8 *pu8Response)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    // get response
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_DSCMB_KL_NONCE_RESP(u32KLEng)+3);
    pu8Response[0] =  (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Response[1] =  (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Response[2] =  (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Response[3] =  (MS_U8)((u32Tmp&0x000000ff)>>0);

    u32Tmp = REG32_R(REG_DSCMB_KL_NONCE_RESP(u32KLEng)+2);
    pu8Response[4] =  (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Response[5] =  (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Response[6] =  (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Response[7] =  (MS_U8)((u32Tmp&0x000000ff)>>0);

    u32Tmp = REG32_R(REG_DSCMB_KL_NONCE_RESP(u32KLEng)+1);
    pu8Response[8] =  (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Response[9] =  (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Response[10] = (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Response[11] = (MS_U8)((u32Tmp&0x000000ff)>>0);

    u32Tmp = REG32_R(REG_DSCMB_KL_NONCE_RESP(u32KLEng)+0);
    pu8Response[12] = (MS_U8)((u32Tmp&0xff000000)>>24);
    pu8Response[13] = (MS_U8)((u32Tmp&0x00ff0000)>>16);
    pu8Response[14] = (MS_U8)((u32Tmp&0x0000ff00)>>8);
    pu8Response[15] = (MS_U8)((u32Tmp&0x000000ff)>>0);

    return TRUE ;

}

void _HAL_DSCMB_KL_KDF_Ctrl(
    DSCMB_KL_SelEng  eKLEng,
    KDF_TYPE eHashSel,
    MS_U16  u16AppId,
    KDF_HW_KEY_SEL  eHWKeyId,
    MS_BOOL bEnable)
{
    //4 u32HWKeyId is 0 in keltic/kaiser, keres/kirin could choose hardwire key
    MS_U32 u32KLEng   = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32HashSel = _HAL_DSCMB_GetKLKdfType(eHashSel);
    MS_U32 u32HWKeyId = _HAL_DSCMB_GetKLKdfHWKeyId(eHWKeyId);

    MS_U32 u32Tmp  = 0;
    if(bEnable)
    {   //The KDF enable is controled by key property
        u32Tmp  = REG_DSCMB_KL_KDF_EN;//only work if root key from ACPU
        u32Tmp |= (u16AppId   <<  REG_DSCMB_KL_KDF_APP_ID_SHT)   & REG_DSCMB_KL_KDF_APP_ID_MSK;
        u32Tmp |= (u32HashSel <<  REG_DSCMB_KL_KDF_HASH_SEL_SHT) & REG_DSCMB_KL_KDF_HASH_SEL_MSK;
        u32Tmp |= (u32HWKeyId <<  REG_DSCMB_KL_KDF_HWKEY_ID_SHT) & REG_DSCMB_KL_KDF_HWKEY_ID_MSK;
    }

    REG32_W(REG_DSCMB_KL_KDF(u32KLEng), u32Tmp);

}

void HAL_DSCMB_KL_KDF_Disable( DSCMB_KL_SelEng  eKLEng)
{
    _HAL_DSCMB_KL_KDF_Ctrl(eKLEng, 0, 0, 0, FALSE);
}

void HAL_DSCMB_KL_KDF_Enable( DSCMB_KL_SelEng  eKLEng, KDF_TYPE eHashSel, MS_U16  u16AppId, KDF_HW_KEY_SEL u32HWKeyId)
{
    _HAL_DSCMB_KL_KDF_Ctrl(eKLEng, eHashSel, u16AppId, u32HWKeyId, TRUE);
}

MS_BOOL HAL_DSCMB_KL_SP_KeyCtrl(DSCMB_KL_SelEng  eKLEng, DSCMB_KLDst eDst)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    HAL_DSCMB_KL_DstSP eHalKLDst = _HAL_DSCMB_GetKLDstSP(eDst);
    MS_U32 u32Tmp = 0;

    if(u32KLEng != DSCMB_PVRKL)
    {
        return FALSE;
    }
    if(eHalKLDst == E_HAL_DSCMB_KL_DST_SP_NA)
    {
        return FALSE;
    }

    u32Tmp |= (eHalKLDst << REG_DSCMB_KL_SP_KEY_DST_SHT) & REG_DSCMB_KL_SP_KEY_DST_MASK; //set sps/spd dest

    //reset forbid
    REG32_W(REG_DSCMB_KL_SP_FORBID(u32KLEng), 0);
    //set sp key dest
    REG32_W(REG_DSCMB_KL_SP_CTRL(u32KLEng), u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_SP_SetIV(DSCMB_KL_SelEng  eKLEng, MS_U8 *pu8IV)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 wData[4] = {0, 0, 0, 0};

    if(u32KLEng != DSCMB_PVRKL)
    {
        return FALSE;
    }

    if(pu8IV == NULL)
    {
        return FALSE;
    }

    wData[0] = pu8IV[12] << 24 | pu8IV[13] << 16 | pu8IV[14] << 8 | pu8IV[15] ;
    wData[1] = pu8IV[8 ] << 24 | pu8IV[9 ] << 16 | pu8IV[10] << 8 | pu8IV[11] ;
    wData[2] = pu8IV[4 ] << 24 | pu8IV[5 ] << 16 | pu8IV[6 ] << 8 | pu8IV[7 ] ;
    wData[3] = pu8IV[0 ] << 24 | pu8IV[1 ] << 16 | pu8IV[2 ] << 8 | pu8IV[3 ] ;

    //set sp iv
    REG32_W(REG_DSCMB_KL_SP_IV(u32KLEng, 0), wData[0]);
    REG32_W(REG_DSCMB_KL_SP_IV(u32KLEng, 1), wData[1]);
    REG32_W(REG_DSCMB_KL_SP_IV(u32KLEng, 2), wData[2]);
    REG32_W(REG_DSCMB_KL_SP_IV(u32KLEng, 3), wData[3]);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_SP_Start(DSCMB_KL_SelEng eKLEng)
{
    MS_U32 u32Tmp = 0;
    MS_U32 u32Resp = 0;
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);

    u32Tmp = REG32_R(REG_DSCMB_KL_SP_CTRL(u32KLEng));
    u32Tmp |= REG_DSCMB_KL_SP_GO;

    REG32_W(REG_DSCMB_KL_SP_CTRL(u32KLEng),u32Tmp);

    while(1)
    {
        u32Tmp = REG32_R(REG_DSCMB_KL_SP_CTRL(u32KLEng));
        if((u32Tmp & REG_DSCMB_KL_SP_GO) == 0x0)   break;
    }

    u32Resp =  (REG32_R(REG_DSCMB_KL_SP_CTRL(u32KLEng)) & REG_DSCMB_KL_SP_KEY_RESP_MASK ) >> REG_DSCMB_KL_SP_KEY_RESP_SHT;

    return  u32Resp > 0 ? FALSE: TRUE;
}
