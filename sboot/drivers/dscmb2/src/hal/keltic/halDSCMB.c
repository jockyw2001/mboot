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
#ifdef UTOPIA_FULL_VERSION
#include "MsCommon.h"
#include "MsTypes.h"
#include "MsDevice.h"
#include "MsIRQ.h"
#include "drvSYS.h"
#include "drvSEM.h"
#else
#define MSOS_TYPE_NOS
#define DSCMB_NO_PRINTF
#endif /* UTOPIA_FULL_VERSION */

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
#define REG32_R(reg)            *((volatile MS_U32*)(REG_BASE_BANK+ ((reg)<< 2)))

#ifdef DSCMB_NO_PRINTF
#define HALDSCMB2_DBG(lv, x, args...)
#else
#define HALDSCMB2_DBG(lv, x, args...) \
    if (lv <= _u32HalDSCMB2DbgLv ) \
    {   \
        if(lv == DSCMB2_DBGLV_ERR){ printf("[ERROR]"); } \
        else if(lv == DSCMB2_DBGLV_WARNING){ printf("[WARNING]"); }\
        else{ printf("[DEBUG]"); }\
        if(_u32HalDSCMB2DbgLv >= DSCMB2_DBGLV_DEBUG){ printf("[%05d][  %-33s]", __LINE__, __FUNCTION__); }\
        printf(" ");\
        printf(x, ##args); \
    }
#endif

#if defined (MSOS_TYPE_NOS)
#define DSCMB_HW_LOCK(_a_)
#define DSCMB_HW_UNLOCK(_a_)
#else
#define DSCMB_HW_LOCK(_a_)     { if(MDrv_SEM_Lock((_a_), MSOS_WAIT_FOREVER) == FALSE){HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "HW Lock Fail!\n");}}
#define DSCMB_HW_UNLOCK(_a_)   { if(MDrv_SEM_Unlock((_a_)) == FALSE){ HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "HW Lock Fail!\n");}}
#endif

typedef struct
{
    MS_U32 SCB_LSAS;
    MS_U32 SCB_LSAD;
    MS_U32 SCB_ESA;
} stSCBFix;


//================================================================
//  Variable Declaration
//================================================================
static MS_U32 _u32RegBase = 0x0;
static stSCBFix SCBFix_Tsid[REG_TSP_TSID_CNT];
static MS_U32 _u32CAVid = 0x0F; // TODO:  K3S forbid cavid== 0
static MS_U32 _u32HalDSCMB2DbgLv = DSCMB2_DBGLV_WARNING;

#if !defined (MSOS_TYPE_NOS)
//4 HW SEM to protect indirect R/W and some shared register
static eSemId _eHW_SemId = E_SEM_CA;
#endif

MS_U32  _u32SwitchData[5]= {0};

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
        case E_DSCMB_KL_TYPE_INF:
            return E_HAL_DSCMB_KL_TYPE_1_KPR;
        case E_DSCMB_KL_TYPE_S5:
            return E_HAL_DSCMB_KL_TYPE_5;
        default:
            return 0;
    }
    return 0;
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

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Invalid FSCB Input.\n");
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

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Invalid Key Type.\n");
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
            //[FIXME] Check this return value
            ;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Invalid Engine Type.\n");
    return E_HAL_DSCMB_ENG_NUM;
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
        default:
            // [FIXME] Upper layer should check this parameter .
            ;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Invalid Algorithm Type.\n");
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

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Invalid Sub Algorithm Type.\n");
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
        default:
            ;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "Invalid Residue/Short Block Algorithm Type.\n");
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

MS_BOOL HAL_DSCMB_OTPEnDSCMB(void)
{
    MS_U32 u32CH0SWCfg = 0;
    u32CH0SWCfg = REG32_R(REG_OTP_CH0_SWICH_COMB);

    return u32CH0SWCfg?TRUE:FALSE;
}

//================================================================
//  Function Implementation
//================================================================
static void * _memset(void * s,char c,int count)
{
    char *xs = (char *) s;

    while (count--)
    *xs++ = c;

    return s;
}

static void REG32_W(MS_U32 u32Reg, MS_U32 u32Value)
{
    *((volatile MS_U32*)(REG_BASE_BANK+ ((u32Reg)<< 2))) = u32Value;
    HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "REG32_W(0x%08lX, 0x%08lX)\n", REG_BASE_BANK+ ((u32Reg)<< 2), u32Value);
}

static MS_U16 REG16_R(MS_U32 u32Reg)
{
    return REG32_R(u32Reg) & 0x0000FFFF;
}

static void REG16_W(MS_U32 u32Reg, MS_U16 u16Value)
{
    MS_U32 u32Value = (REG32_R(u32Reg) & 0xFFFF0000) | (u16Value & 0x0000FFFF);
    REG32_W(u32Reg, u32Value);
}

static void TSP32_IdrW(MS_U32 u32Addr, MS_U32 u32Value)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "TSP32_IdrW(0x%08lX, 0x%08lX)\n", u32Addr, u32Value);

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

MS_BOOL HAL_DSCMB_GetCap(void)
{
    return TRUE ;
}

MS_BOOL HAL_DSCMB_SetCAVid(MS_U32 u32CAVid )
{
    _u32CAVid = u32CAVid;
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set CAVid: 0x%X\n", (unsigned int)_u32CAVid);
    return TRUE;
}

MS_U32 HAL_DSCMB_GetCAVid(void)
{
    return _u32CAVid;
}

MS_BOOL HAL_DSCMB_SetBank(MS_U32 u32Bank)
{
    REG_BASE_BANK = 0xBF200000;
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set bank 0x%08x\n", (int)REG_BASE_BANK);

    return TRUE;
}

MS_BOOL HAL_DSCMB_GetBank(MS_U32 *u32Bank)
{
    if(u32Bank == 0)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Get bank FAIL\n");
        return FALSE;
    }

    *u32Bank = REG_BASE_BANK;

    return TRUE ;
}

void _HAL_DSCMB_HWPatch(void)
{
    // HWPatch for PktParser //
//   REG32_W(REG_CIPHER_REG1(0) , 0xFFFFFFFF );
//   REG32_W(REG_CIPHER_REG1(1) , 0xFFFFFFFF );

    return ;
}

MS_BOOL HAL_DSCMB_BypassMode(MS_U32 u32CAVid, MS_U32 u32TSid)
{
    MS_U32 i = 0;
    MS_BOOL ret = TRUE;

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "By Pass clear packet CAVid: %d\n", (unsigned int)u32CAVid);
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "By Pass set switch and FSCB\n");

    // Set CAVid
    ret &= HAL_DSCMB_PidIdx_SetCAVid(i,u32CAVid);
    // ClearKey idx points to the last slot for bypass packet
    ret &= HAL_DSCMB_PidIdx_SetSlotKeyIdx(i, E_DSCMB_KEY_CLEAR, HAL_DSCMB_KTE_ID_NULL);

    //Open switch E for bypass packet
    ret &= HAL_DSCMB_KTE_Write_Switch(i, E_DSCMB_KEY_CLEAR, u32CAVid,REG_KTE_SW4_E_EN, REG_KTE_SW4_E_EN);
    ret &= HAL_DSCMB_KTE_Write_FSCB(i, E_DSCMB_KEY_CLEAR, u32CAVid, DSCMB_FSCB_UNCHG);
    //Destination
    ret &= HAL_DSCMB_KTE_Write_SrcDst(i,E_DSCMB_KEY_CLEAR,u32CAVid,0,0,0);

    if(ret == FALSE)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Bypass Mode fail!!!!\n");
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_DSCMB_Init(void)
{
    MS_U32 u32Tmp , i ;

    /// turn on TSCE0 clock
    u32Tmp = REG32_R((MS_U32)REG_TSCE_CTRL);
    u32Tmp |= REG_TSCE_CTRL_CLK_EN;
    REG32_W((MS_U32)REG_TSCE_CTRL, (MS_U32)u32Tmp);

    // Init TSP filter and DSCMB PidSlotMap
    HAL_DSCMB_PidIdx_Enable(0,FALSE);    //disable pidslotMap 0 for default path
    for (i = 1; i< HAL_DSCMB_PIDIDX_MAX; i++)
    {
        HAL_DSCMB_PidIdx_Enable(i,TRUE) ;//enable all pidslotMap, except pidslotmap 1
    }

     for (i = 0; i< HAL_DSCMB_PIDIDX_MAX; i++)
    {
        _HAL_DSCMB_PidFlt_Reset(i);
        HAL_DSCMB_SetTSPCADst(i,0,0);
        HAL_DSCMB_PidIdx_DualPath(i,FALSE); //disable all dual path
    }

    // Default SCBFix setting
    for (i = 0 ; i < HAL_DSCMB_TSID_MAX ; i ++)
    {
        HAL_DSCMB_Cipher_Set_SCBFix(0, i, E_HAL_DSCMB_SCBFIX_DMXL , E_HAL_DSCMB_SCBFIX_USESCB );
        HAL_DSCMB_Cipher_Set_SCBFix(0, i, E_HAL_DSCMB_SCBFIX_DMXU , E_HAL_DSCMB_SCBFIX_USESCB );
        HAL_DSCMB_Cipher_Set_SCBFix(0, i, E_HAL_DSCMB_SCBFIX_ESA  , E_HAL_DSCMB_SCBFIX_USESCB );
        HAL_DSCMB_Cipher_Set_SCBFix(0, i, E_HAL_DSCMB_SCBFIX_LSAS , E_HAL_DSCMB_SCBFIX_USESCB );
        HAL_DSCMB_Cipher_Set_SCBFix(0, i, E_HAL_DSCMB_SCBFIX_LSAD , E_HAL_DSCMB_SCBFIX_USESCB );
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Connect Tsid[X] to Clear[X]\n");
    for (i = 0 ; i < REG_TSP_TSID_CNT ; i++)
    {
        // set TSID to Clear output 1-to-1 mapping
        HAL_DSCMB_Cipher_Output_Ctrl(E_HAL_DSCMB_CIPHER_OUTPUT_CLR0+i,
                                     0           /*cipher0*/ ,
                                     TRUE        /*Enable*/,
                                     FALSE       /*No Encrypt*/ ,
                                     i           /*tsid*/ ,
                                     _u32CAVid   /*cavid*/);
    }

    //Default enable Cipher Channel Dual Path
    for (i = 0 ; i < REG_DSCMB_CIPHER_CNT; i++)
    {
        HAL_DSCMB_Cipher_DualPath(i, TRUE);
    }

    for(i = 0 ; i < HAL_DSCMB_TSID_MAX ; i++)
    {
        HAL_DSCMB_ClearPidPairStatus(i, 1);
        HAL_DSCMB_ClearPidPairStatus(i, 2);
        HAL_DSCMB_ClearPidPairStatus(i, 3);
        HAL_DSCMB_ClearPidPairStatus(i, 4);

        HAL_DSCMB_CC_Respin(0, i, TRUE);

        // Set TSP CAVid
        HAL_DSCMB_SetTSPCAVid(i, _u32CAVid);
    }

    _memset(SCBFix_Tsid , 0x0 ,sizeof(SCBFix_Tsid));

    //enable csa2 descramble clear stream type with clear key
    //enable keep ts scb
    //cipher channel 0
    u32Tmp = REG32_R(REG_CIPHER_CSA2_CTRL(0));
    u32Tmp |= REG_CIPHER_CSA2_SCB0_PKT;
    u32Tmp |= REG_CIPHER_CSA2_KEEP_TS_SCB;
    REG32_W(REG_CIPHER_CSA2_CTRL(0), u32Tmp);

    // [FIXME] HW Patch //
    //  _HAL_DSCMB_HWPatch();
    return TRUE;

}

//---------------------------------------
//  Pid Slot Index interface
//---------------------------------------
MS_BOOL _HAL_DSCMB_PidIdx_MuxWrite(MS_U32 u32fltid , MS_U32 u32wmux, MS_U32 u32wdata )
{
    MS_U32 u32Tmp = 0  ;
    if ((u32wmux & ~REG_PIDIDX_WMUX_MSK) || (u32fltid >= HAL_DSCMB_PIDIDX_MAX))
    {
        // undefined write field
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }
    // prepare wite command
    u32Tmp = REG_PIDIDX_DIR_WIRTE | u32wmux | ((u32fltid << REG_PIDIDX_FLTID_SHT ) & REG_PIDIDX_FLTID_MSK) ;
    // fill in data
    REG32_W(REG_PIDIDX_DATA , REG_PIDIDX_WDATA_MSK & u32wdata );
    // start command
    REG32_W(REG_PIDIDX_CTRL , u32Tmp | REG_PIDIDX_CMD_GO );

    u32Tmp = (REG32_R(REG_PIDIDX_CTRL) & 0xF0000000) >> 28;
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "u32fltid=%lu mtn_resp = %s\n",u32fltid, u32Tmp == 0?"OK":(u32Tmp==1?"NOP":(u32Tmp==2?"NULL CAVid":(u32Tmp == 3? "PidSlotMapInUse":"KeySlotInUse"))));

    return TRUE;
}


MS_BOOL _HAL_DSCMB_PidIdx_MuxRead(MS_U32 u32fltid, MS_U32 *u32rdata )
{
    MS_U32 u32Tmp= 0;
    if ((u32fltid >= HAL_DSCMB_PIDIDX_MAX) || ( NULL == u32rdata ))
    {
        // ivalid pid id

        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }
    // prepare wite command
    u32Tmp = REG_PIDIDX_DIR_READ| ((u32fltid << REG_PIDIDX_FLTID_SHT ) & REG_PIDIDX_FLTID_MSK) ;
    // start command
    REG32_W(REG_PIDIDX_CTRL , u32Tmp | REG_PIDIDX_CMD_GO );
    // read  data
    *u32rdata = REG32_R(REG_PIDIDX_DATA);
    return TRUE ;
}

MS_BOOL HAL_DSCMB_PidIdx_Write(MS_U32 u32fltid , DSCMB_Key_Type type , MS_U32 u32idx , MS_BOOL bEnable)
{
    MS_U32 u32wmux = 0 ;

    if (u32idx >= HAL_DSCMB_KTE_MAX || (u32fltid >= HAL_DSCMB_PIDIDX_MAX))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    switch (type)
    {
        case E_DSCMB_KEY_CLEAR:
            u32wmux= REG_PIDIDX_WMUX_CLR_EN;
            break;
        case E_DSCMB_KEY_EVEN:
            u32wmux= REG_PIDIDX_WMUX_EVEN_EN;
            break;
        case E_DSCMB_KEY_ODD:
            u32wmux= REG_PIDIDX_WMUX_ODD_EN;
            break;
        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            return FALSE;
    }

    return _HAL_DSCMB_PidIdx_MuxWrite( u32fltid , u32wmux , (bEnable<<7)|u32idx );
}

MS_BOOL HAL_DSCMB_PidIdx_DualPath(MS_U32 u32fltid , MS_BOOL bDual )
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Enable PidSlotMap Entry=%lu, bDual=%d\n",u32fltid, bDual);
    return _HAL_DSCMB_PidIdx_MuxWrite(u32fltid , REG_PIDIDX_WMUX_DUAL_EN , bDual);
}

MS_BOOL HAL_DSCMB_PidIdx_Enable(MS_U32 u32fltid , MS_BOOL bEnable)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Enable PidSlotMap Entry=%lu, bEnable=%d\n",u32fltid, bEnable);
    return _HAL_DSCMB_PidIdx_MuxWrite(u32fltid , REG_PIDIDX_WMUX_EN , bEnable);
}

MS_BOOL HAL_DSCMB_PidIdx_SetCAVid(MS_U32 u32fltid , MS_U32 u32CAVid )
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set PidSlotMap Entry=%lu CAVid=%lu\n",u32fltid, u32CAVid);
    return _HAL_DSCMB_PidIdx_MuxWrite(u32fltid , REG_PIDIDX_WMUX_CAVID , u32CAVid );
}

MS_BOOL HAL_DSCMB_PidIdx_SetSlotKeyIdx(MS_U32 u32FltId,DSCMB_Key_Type key, MS_U32 keyIdx)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set/Enable fltid=%lu key=%d keyIdx=%lu\n", u32FltId, key, keyIdx);
    return HAL_DSCMB_PidIdx_Write(u32FltId , key  , keyIdx , TRUE  );
}

MS_BOOL HAL_DSCMB_PidIdx_ClearSlotKeyIdx(MS_U32 u32FltId,DSCMB_Key_Type key)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Reset/Disable fltid=%lu key=%d\n", u32FltId, key);
    return HAL_DSCMB_PidIdx_Write(u32FltId , key  , PID_SLOT_INDEX_NULL , FALSE  );
}

MS_BOOL HAL_DSCMB_PidIdx_EnableSlot(MS_U32 u32FltId)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Enalbe fltid=%lu from TSP to DSCMB\n", u32FltId);
    HAL_DSCMB_PidFlt_EnableKey(u32FltId, TRUE);
    return TRUE;
}

MS_BOOL HAL_DSCMB_PidIdx_DisableSlot(MS_U32 u32FltId)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Disable fltid=%lu from TSP to DSCMB\n", u32FltId);
    HAL_DSCMB_PidFlt_EnableKey(u32FltId, FALSE);
    return TRUE;
}

MS_BOOL HAL_DSCMB_PidIdx_SetTsId(MS_U32 u32fltid , MS_U32 u32TsId )
{
    //  printf("\033[31m u32fltid = %u u32TsId = %u \n\033[m", u32fltid, u32TsId);
    return _HAL_DSCMB_PidIdx_MuxWrite(u32fltid , REG_PIDIDX_WMUX_TSID , u32TsId );
}

//---------------------------------------
//  Key Table entry interface
//---------------------------------------
MS_BOOL _HAL_DSCMB_KTE_Write_Ex(MS_U32 u32fltid, DSCMB_Key_Type type, MS_U32 u32Field, MS_U32 u32Cmd, MS_U32 u32CAVid,MS_U32 *u32data, MS_BOOL bIsKL)
{
    MS_U32 u32Tmp = 0;

    //[NOTE] Type translation //
    HAL_DSCMB_KeyType etype = _HAL_DSCMB_GetKeyType(type);

    if ((u32fltid >= HAL_DSCMB_PIDIDX_MAX) || (~REG_KTE_SEL_MSK & u32Field)  )
    {
        // ivalid pid id
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d] %lu %lu FAIL\n",__func__, __LINE__, u32fltid, u32Field);
        return FALSE ;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "[%s][%d] %lu %lu \n",__func__, __LINE__, u32fltid, u32Field);

    if(u32Cmd == REG_KTE_CMD_WRITE)
    {
        //for CAVID1 read switch hw bug
        if(u32Field == REG_KTE_SEL_SWITCH)
        {
            _u32SwitchData[0] = u32data[0];
            _u32SwitchData[1] = u32data[1];
            _u32SwitchData[2] = u32data[2];
            _u32SwitchData[3] = u32data[3];
            _u32SwitchData[4] = u32data[4];
        }

        // file in write data
        REG32_W(REG_KTE_DATA0, u32data[0] );
        REG32_W(REG_KTE_DATA1, u32data[1] );
        REG32_W(REG_KTE_DATA2, u32data[2] );
        REG32_W(REG_KTE_DATA3, u32data[3] );
        REG32_W(REG_KTE_DATA4, u32data[4] );
    }

    // write destination
    REG32_W(REG_KTE_CMD_CTRL1 , (u32fltid & REG_KTE_FLTID_MSK) | etype << REG_KTE_SCB_SHT );
    // prepare wite command
    u32Tmp = (u32Field & REG_KTE_SEL_MSK) | (u32Cmd) |(u32CAVid << REG_KTE_CAVID_SHT) ;

    if(bIsKL) u32Tmp |= 0x40;

    // start command
    REG32_W(REG_KTE_CMD_CTRL0 , u32Tmp);
    REG32_W(REG_KTE_CMD_CTRL0 , u32Tmp | REG_KTE_AKI_GO);

    // For DEBUG
    if((u32Cmd == REG_KTE_CMD_WRITE) && (u32Field == REG_KTE_SEL_ESA))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "SWITCH WRITE filter %d Key %d cavid %d ===========\n", (int)u32fltid, etype, (int)u32CAVid);
        HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "SWITCH Data0: 0x%X \n",       (unsigned int)u32data[0]);
        HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "SWITCH Data1: 0x%X \n",       (unsigned int)u32data[1]);
        HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "SWITCH Data2: 0x%X \n",       (unsigned int)u32data[2]);
        HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "SWITCH Data3: 0x%X \n",       (unsigned int)u32data[3]);
        HALDSCMB2_DBG(DSCMB2_DBGLV_ARRAY, "SWITCH Data4: 0x%X \n", (unsigned int)u32data[4]);

        HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "SWITCH WRITE value 0x%x ===========\n", (unsigned int)u32Tmp);
        u32Tmp = REG32_R(REG_KTE_CMD_CTRL0);
        HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "[%s][%d]  KeyArray_RESP = 0x%X\n",__func__, __LINE__, (unsigned int)((u32Tmp&0x0000F800)>>11) );
    }
#if 0
    if((u32Cmd == REG_KTE_CMD_KEY_EN) )
    {
        DSCMB_SWITCH_DBG("\033[33mSWITCH WRITE filter %d Key %d cavid %d ===========\n", (int)u32fltid, etype, (int)_u32CAVid);

        DSCMB_SWITCH_DBG("\033[33mSWITCH WRITE value 0x%x ===========\n", (unsigned int)u32Tmp);
        u32Tmp = REG32_R(REG_KTE_CMD_CTRL0);
        DSCMB_SWITCH_DBG("[%s][%d]  KeyArray_RESP = 0x%X\n",__func__, __LINE__, (unsigned int)((u32Tmp&0x0000F800)>>11) );
    }
    if((u32Cmd == REG_KTE_CMD_KEY_DIS) )
    {
        DSCMB_SWITCH_DBG("\033[33mSWITCH WRITE filter %d Key %d cavid %d ===========\n", (int)u32fltid, etype, (int)_u32CAVid);

        DSCMB_SWITCH_DBG("\033[33mSWITCH WRITE value 0x%x ===========\n", (unsigned int)u32Tmp);
        u32Tmp = REG32_R(REG_KTE_CMD_CTRL0);
        DSCMB_SWITCH_DBG("[%s][%d]  KeyArray_RESP = 0x%X\n",__func__, __LINE__, (unsigned int)((u32Tmp&0x0000F800)>>11) );
    }
#endif
    // modify by efang
    u32Tmp = REG32_R(REG_KTE_CMD_CTRL0);
    u32Tmp = (MS_U32)((u32Tmp&REG_KTE_RESP_MSK)>>REG_KTE_RESP_SFT);

    if(u32Tmp == REG_KTE_RESP_NO_SLOT)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "No slot.\n");
    }
    else if(u32Tmp == REG_KTE_RESP_NOT_ALLOW_WRITE_KEY)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Not allow to write key.\n");
    }
    else
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Write Ok, %ld\n", u32Tmp);
    }

    return u32Tmp == 0?TRUE:FALSE;
}

MS_BOOL _HAL_DSCMB_KTE_Write(MS_U32 u32fltid, DSCMB_Key_Type type, MS_U32 u32Field, MS_U32 u32Cmd, MS_U32 u32CAVid,MS_U32 *u32data)
{
    return _HAL_DSCMB_KTE_Write_Ex(u32fltid, type, u32Field, u32Cmd, u32CAVid, u32data, FALSE);
}

MS_BOOL HAL_DSCMB_KTE_Read_Switch(MS_U32 u32fltid, DSCMB_Key_Type type, MS_U32 u32CAVid, MS_U32 *u32data)
{
    // [NOTE] type tanslation //
    HAL_DSCMB_KeyType etype = _HAL_DSCMB_GetKeyType(type);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "type: %d\n", type);
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "DRV type: %d HAL type: %d\n", type, etype);

    // write destination
    REG32_W(REG_KTE_CMD_CTRL1 , (u32fltid & REG_KTE_FLTID_MSK) | etype << REG_KTE_SCB_SHT );
    // wite command
    REG32_W(REG_KTE_CMD_CTRL0 , REG_KTE_CMD_READSW | REG_KTE_AKI_GO | ((u32CAVid<<REG_KTE_CAVID_SHT)&REG_KTE_CAVID_MSK ));

    // for CAVID1 read hw switch bug
    if(u32CAVid == E_HAL_DSCMB_CAVID1)
    {   // TODO: FIXME, if support multiple CA in the future
        u32data[0] = _u32SwitchData[0];
        u32data[1] = _u32SwitchData[1];
        u32data[2] = _u32SwitchData[2];
        u32data[3] = _u32SwitchData[3];
        u32data[4] = _u32SwitchData[4];
    }
    else
    {
        u32data[0] = REG32_R(REG_KTE_DATA0);
        u32data[1] = REG32_R(REG_KTE_DATA1);
        u32data[2] = REG32_R(REG_KTE_DATA2);
        u32data[3] = REG32_R(REG_KTE_DATA3);
        u32data[4] = REG32_R(REG_KTE_DATA4);
    }

    return TRUE ;
}


MS_BOOL HAL_DSCMB_KTE_Write_Key(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wtype, MS_U32 u32CAVid, MS_U8* u8key)
{
    MS_U32 u32Field = 0;
    MS_U32 wData[5];
    HAL_DSCMB_KTE_WriteType ewtype = _HAL_DSCMB_GetEngType(wtype) ;


    if ((u32FltId >= HAL_DSCMB_PIDIDX_MAX) || (u8key==NULL))
    {
        // ivalid pid id

        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    // file in write data
    wData[0] = u8key[12] << 24 | u8key[13] << 16 | u8key[14] << 8 | u8key[15] ;
    wData[1] = u8key[8 ] << 24 | u8key[9 ] << 16 | u8key[10] << 8 | u8key[11] ;
    wData[2] = u8key[4 ] << 24 | u8key[5 ] << 16 | u8key[6 ] << 8 | u8key[7 ] ;
    wData[3] = u8key[0 ] << 24 | u8key[1 ] << 16 | u8key[2 ] << 8 | u8key[3 ] ;
    u32Field = ((ewtype << REG_KTE_SEL_SFT) & REG_KTE_SEL_MSK);

    return _HAL_DSCMB_KTE_Write(u32FltId, eType, u32Field ,REG_KTE_CMD_WRITE , u32CAVid, wData);
}

MS_BOOL HAL_DSCMB_KTE_Write_IV(MS_U32 u32FltId, DSCMB_Key_Type eType, MS_U32 u32CAVid, MS_U8* u8key )
{
    MS_U32 u32Field = 0;
    MS_U32 wData[5];
    HAL_DSCMB_KTE_WriteType ewType = E_HAL_DSCMB_ENG_IV;


    if ((u32FltId >= HAL_DSCMB_PIDIDX_MAX) || (u8key==NULL))
    {
        // ivalid pid id

        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }
    // file in write data
    wData[0] = u8key[12] << 24 | u8key[13] << 16 | u8key[14] << 8 | u8key[15] ;
    wData[1] = u8key[8 ] << 24 | u8key[9 ] << 16 | u8key[10] << 8 | u8key[11] ;
    wData[2] = u8key[4 ] << 24 | u8key[5 ] << 16 | u8key[6 ] << 8 | u8key[7 ] ;
    wData[3] = u8key[0 ] << 24 | u8key[1 ] << 16 | u8key[2 ] << 8 | u8key[3 ] ;
    u32Field = ((ewType << REG_KTE_SEL_SFT) & REG_KTE_SEL_MSK);

    return _HAL_DSCMB_KTE_Write(u32FltId, eType , u32Field ,REG_KTE_CMD_WRITE, u32CAVid ,wData);
}

MS_BOOL HAL_DSCMB_KTE_Key_Ctrl_Ex(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wType, MS_U32 u32CAVid, MS_BOOL bEnable, MS_BOOL bIsKL)
{
    MS_U32 u32Field = 0;
    MS_U32 u32Cmd = 0;
    HAL_DSCMB_KTE_WriteType ewType = _HAL_DSCMB_GetEngType(wType) ;

    if ((u32FltId >= HAL_DSCMB_PIDIDX_MAX)  )
    {
        // ivalid pid id

        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
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

MS_BOOL HAL_DSCMB_KTE_Key_Ctrl(MS_U32 u32FltId, DSCMB_Key_Type eType, DSCMB_Eng_Type wType, MS_U32 u32CAVid, MS_BOOL bEnable )
{
    return HAL_DSCMB_KTE_Key_Ctrl_Ex(u32FltId , eType , wType, u32CAVid, bEnable, FALSE);
}

MS_BOOL HAL_DSCMB_KTE_IV_Ctrl(MS_U32 u32FltId, DSCMB_Key_Type eType, MS_U32 u32CAVid, MS_BOOL bEnable )
{
    MS_U32 u32Field = 0;
    MS_U32 u32Cmd = 0;
    HAL_DSCMB_KTE_WriteType ewType = E_HAL_DSCMB_ENG_IV;

    if ((u32FltId >= HAL_DSCMB_PIDIDX_MAX)  )
    {
        // ivalid pid id

        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
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

    return _HAL_DSCMB_KTE_Write(u32FltId , eType , u32Field, u32Cmd, u32CAVid, NULL);
}

// Write algorithm settion to specific Dscmb engine block//
MS_BOOL HAL_DSCMB_KTE_Write_Algo(MS_U32 u32fltid, DSCMB_Key_Type eKeyType, DSCMB_Eng_Type eEngType , MS_U32 u32CAVid,
                                 DSCMB_MainAlgo_Type eAlgoType, DSCMB_SubAlgo_Type eSubAlgo, DSCMB_ResSBAlgo_Type eResType,
                                 DSCMB_ResSBAlgo_Type eSBType, MS_BOOL bDecrypt )
{
    MS_U32 wData[5] = {0,0,0,0,0};
    MS_U32 u32Tmp = 0 ;

    // Type translation //
    HAL_DSCMB_MainAlgo_Type eAlgo   = _HAL_DSCMB_GetAlgoType(eAlgoType);
    HAL_DSCMB_SubAlgo_Type eSub     = _HAL_DSCMB_GetSubAlgoType(eSubAlgo);
    HAL_DSCMB_ResSBAlgo_Type eRes   = _HAL_DSCMB_GetResSBType(eResType);
    HAL_DSCMB_ResSBAlgo_Type eSB    = _HAL_DSCMB_GetResSBType(eSBType);


    if (!HAL_DSCMB_KTE_Read_Switch(u32fltid , eKeyType, u32CAVid, wData))
    {
        // read switch Fail
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }
    // prepare write command
    // u32Cmd = (eEngType << REG_KTE_SEL_SFT);
    // prepare algorithm data
    u32Tmp  = (eAlgo << REG_KET_SWT_ALG_SHT ) | (eSub << REG_KET_SWT_SUBALG_SHT);
    u32Tmp |= (eRes << REG_KET_SWT_RES_SHT) | ( eSB << REG_KET_SWT_SB_SHT) ;

    if (bDecrypt)
        u32Tmp |= REG_KET_SWT_DECRYPT ;

    switch (eEngType)
    {
        case E_DSCMB_ENG_ESA:
            wData[1] = u32Tmp ;
            break;
        case E_DSCMB_ENG_LSAS:
            wData[2] = u32Tmp ;
            break;
        case E_DSCMB_ENG_LSAD:
            wData[3] = u32Tmp ;
            break;
        default:

            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            return FALSE ;
    }

    return _HAL_DSCMB_KTE_Write(u32fltid, eKeyType, REG_KTE_SEL_SWITCH , REG_KTE_CMD_WRITE, u32CAVid ,wData) ;
}

MS_BOOL HAL_DSCMB_KTE_Write_Switch(MS_U32 u32FltId , DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 u32UppSwitch, MS_U32 u32LowSwitch)
{
    MS_U32 wData[5];
    MS_U32 u32Tmp = 0;

    if (!HAL_DSCMB_KTE_Read_Switch(u32FltId ,eKeyType, u32CAVid, wData ))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    // update switch value
    u32Tmp = (( u32UppSwitch & REG_KTE_SW4_SWT_MSK) << REG_KTE_SW4_UPPSWT_SHT )
             |(( u32LowSwitch & REG_KTE_SW4_SWT_MSK) << REG_KTE_SW4_LOWSWT_SHT );

    u32Tmp |=  wData[4] & ~(REG_KTE_SW4_SWT_MSK|(REG_KTE_SW4_SWT_MSK<<REG_KTE_SW4_UPPSWT_SHT));

    wData[4] = u32Tmp ;

    return _HAL_DSCMB_KTE_Write(u32FltId, eKeyType, REG_KTE_SEL_SWITCH, REG_KTE_CMD_WRITE, u32CAVid ,wData) ;

}

MS_BOOL HAL_DSCMB_KTE_Write_PacketSwitch(MS_U32 u32FltId ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, DSCMB_Eng_Type eUppSwitch, DSCMB_Eng_Type eLowSwitch)
{
    MS_U32 u32UppSwitch = REG_KTE_SW4_E_EN;
    MS_U32 u32LowSwitch = REG_KTE_SW4_E_EN;

    //esa
    if((eLowSwitch & E_DSCMB_ENG_ESA) != 0)
    {
        u32LowSwitch |= REG_KTE_SW4_B_EN;
    }

    if((eUppSwitch & E_DSCMB_ENG_ESA) != 0)
    {
        u32UppSwitch |= REG_KTE_SW4_B_EN;
    }

    //lsas
    if((eLowSwitch & E_DSCMB_ENG_LSAS) != 0)
    {
        u32LowSwitch |= REG_KTE_SW4_D_EN;
    }

    if((eUppSwitch & E_DSCMB_ENG_LSAS) != 0)
    {
        u32UppSwitch |= REG_KTE_SW4_D_EN;
    }

    //lsad
    if((eLowSwitch & E_DSCMB_ENG_LSAD) != 0)
    {
        u32LowSwitch |= REG_KTE_SW4_A_EN;
    }

    if((eUppSwitch & E_DSCMB_ENG_LSAD) != 0)
    {
        u32UppSwitch |= REG_KTE_SW4_A_EN;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "u32FltId=%lu eKeyType=%d u32CAVid=%lu u32UppSwitch=%lu u32LowSwitch=%lu\n", u32FltId, eKeyType, u32CAVid, u32UppSwitch, u32LowSwitch);

    return HAL_DSCMB_KTE_Write_Switch(u32FltId, eKeyType, u32CAVid, u32UppSwitch, u32LowSwitch );
}


MS_BOOL HAL_DSCMB_KTE_Write_FSCB(MS_U32 u32FltId ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, DSCMB_FSCB eForceSCB)
{
    MS_U32 wData[5] = {0};
    HAL_DSCMB_FSCB eFSCB = _HAL_DSCMB_GetFSCB(eForceSCB);

    if (!HAL_DSCMB_KTE_Read_Switch(u32FltId, eKeyType, u32CAVid, wData ))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    // Force SCB rule //
    wData[0] &=  ~(REG_KTE_SW0_FORCESCB_MSK);
    wData[0] |= REG_KTE_SW0_FORCESCB_MSK & eFSCB;

    return _HAL_DSCMB_KTE_Write(u32FltId, eKeyType, REG_KTE_SEL_SWITCH, REG_KTE_CMD_WRITE, u32CAVid ,wData) ;
}

MS_BOOL HAL_DSCMB_KTE_Write_SBOX(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 u32Sbox, MS_BOOL bCSA2Mode)
{
    MS_U32 wData[5];
    MS_U32 u32Tmp = 0 ;

    if (!HAL_DSCMB_KTE_Read_Switch(u32fltid, eKeyType, u32CAVid, wData))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }
    u32Tmp =  wData[4] & ~(REG_KTE_SW4_CSA2_SBOX_MSK) ;
    u32Tmp |= ( u32Sbox << REG_KTE_SW4_CSA2_SBOX_SHT ) & REG_KTE_SW4_CSA2_SBOX_MSK ;

#if 0
    if (bCSA2Mode)
    {
        u32Tmp |= REG_KTE_SW4_CSA2MOD_EN;
    }
#endif

    wData[4] = u32Tmp ;

    return _HAL_DSCMB_KTE_Write(u32fltid, eKeyType, REG_KTE_SEL_SWITCH , REG_KTE_CMD_WRITE, u32CAVid, wData) ;
}


MS_BOOL HAL_DSCMB_KTE_Write_Permu(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 u32per)
{
    MS_U32 wData[5];
    MS_U32 u32Tmp = 0 ;

    if (!HAL_DSCMB_KTE_Read_Switch(u32fltid , eKeyType, u32CAVid, wData ))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    u32Tmp =  wData[4] & ~(REG_KTE_SW4_PERMUTATION_MSK) ;
    u32Tmp |= ( u32per << REG_KTE_SW4_PERMUTATION_SHT ) & REG_KTE_SW4_PERMUTATION_MSK ;
    wData[4] = u32Tmp ;

    return _HAL_DSCMB_KTE_Write(u32fltid, eKeyType, REG_KTE_SEL_SWITCH , REG_KTE_CMD_WRITE, u32CAVid, wData) ;
}

//  MULTI2 Algorithm Related  //
MS_BOOL HAL_DSCMB_KTE_Write_MULTI2_SysKey(MS_U8* Key , MS_U32 u32Len)
{

    MS_U8 Data[HAL_DSCMB_MULTI2_SYSKEY_MAX];
    MS_U32 i , tmp ;

    _memset(Data , 0x0 , sizeof(Data));

    memcpy(Data,Key,u32Len);


    for ( i = 0 ; i < (HAL_DSCMB_MULTI2_SYSKEY_MAX/4 ) ; i ++ )
    {
        tmp = i * 4  ;
        REG32_W(REG_TSCE_MULTI2_SYSKEY(i) , Data[tmp+0] << 24 | Data[tmp+1] << 16 | Data[tmp+2] << 8 | Data[tmp+3] );
    }

    return TRUE ;
}

MS_BOOL HAL_DSCMB_KTE_Write_MULTI2_Round(MS_U32 u32Round)
{

    REG32_W(REG_TSCE_MULTI2_ROUND , u32Round );

    return TRUE ;
}

// Get KTE Slot status //
MS_BOOL HAL_DSCMB_KTE_GetStatus(MS_U32 u32FltId ,DSCMB_Key_Type eKeyType, MS_U32 *KeyStatus)
{
    MS_U32 u32KeyIdx = 0 ;
    MS_U32 u32Status = 0 ;
    MS_U32 u32Tmp = 0 ;
    MS_U32 u32StatNum = 0;

    PidSlotMapRead_t ReadData = {0};
    HAL_DSCMB_ReadPidSlotMap(u32FltId, &ReadData);

    // Filter specific key slot info //
    switch (eKeyType)
    {
        case E_DSCMB_KEY_CLEAR:
            u32KeyIdx = ReadData.ClrSlotIndex;
            break;
        case E_DSCMB_KEY_EVEN:
            u32KeyIdx = ReadData.EvenSlotIndex;
            break;
        case E_DSCMB_KEY_ODD:
            u32KeyIdx = ReadData.OddSlotIndex;
            break;
        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            return FALSE ;
    }

    u32Status |= ReadData.DualSloEn?DSCMB_KTE_DUAL_EN:0;
    u32Status |= ReadData.PidSlotEn?DSCMB_PIDIDX_EN:0;

    u32StatNum = (u32KeyIdx >> 5);

    u32Tmp =  REG32_R(REG_KTE_LSAD_STAT(u32StatNum)) & (1 << (u32KeyIdx % 32 ));
    u32Status |= (u32Tmp)?DSCMB_KTE_LSAD_EN:0;

    u32Tmp =  REG32_R(REG_KTE_ESA_STAT(u32StatNum)) & (1 << (u32KeyIdx % 32 )) ;
    u32Status |= (u32Tmp)?DSCMB_KTE_ESA_EN:0;

    u32Tmp =  REG32_R(REG_KTE_LSAS_STAT(u32StatNum)) &(1 << (u32KeyIdx % 32 ));
    u32Status |= (u32Tmp)?DSCMB_KTE_LSAS_EN:0;

    u32Tmp =  REG32_R(REG_KTE_IV_STAT(u32StatNum)) &(1 << (u32KeyIdx % 32 ));
    u32Status |= (u32Tmp)?DSCMB_KTE_IV_EN:0;

    u32Tmp = REG32_R(REG_PIDIDX_STAT(u32StatNum)) & (1 << (u32KeyIdx % 32 ));
    u32Status |= (u32Tmp)?DSCMB_KTE_VALID:0;

    *KeyStatus = u32Status ;

    return  TRUE ;

}

MS_BOOL HAL_DSCMB_KTE_Write_SrcDst(MS_U32 u32fltid ,DSCMB_Key_Type eKeyType, MS_U32 u32CAVid, MS_U32 Src, MS_U32 UppDst, MS_U32 LowDst)
{
    MS_U32 wData[5];
    MS_U32 u32Tmp = 0 ;

    if (!HAL_DSCMB_KTE_Read_Switch(u32fltid, eKeyType, u32CAVid , wData))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    u32Tmp = wData[0] & ~(REG_KTE_SW0_SOURCE_MSK|REG_KTE_SW0_UPPDEST_MSK|REG_KTE_SW0_LOWDEST_MSK);

    u32Tmp |= ((Src<<REG_KTE_SW0_SOURCE_SHT)     &REG_KTE_SW0_SOURCE_MSK);
    u32Tmp |= ((UppDst<<REG_KTE_SW0_UPPDEST_SHT) &REG_KTE_SW0_UPPDEST_MSK);
    u32Tmp |= ((LowDst<<REG_KTE_SW0_LOWDEST_SHT) &REG_KTE_SW0_LOWDEST_MSK);

    wData[0] = u32Tmp;

    return _HAL_DSCMB_KTE_Write(u32fltid, eKeyType, REG_KTE_SEL_SWITCH, REG_KTE_CMD_WRITE, u32CAVid, wData);

}


//---------------------------------------
//  Stream Parser interface
//---------------------------------------
void HAL_DSCMB_PktParser_PrintEvent(MS_U32 event)
{
    enum
    {
        eNoSlot               = 31,
        eLSADKeyInvalid       = 30,
        eESAKeyInvalid        = 29,
        eLSASKeyInvalid       = 28,
        eIVInvalid            = 27,
        eCAVidUnMatched       = 26,
        eSwitchInvalid        = 25,
        eAFOnlySCBNotClear    = 24,
        eLSASMDDSCBNotClear   = 23,
        eLSASMDDto2ndPID      = 22,
        eNDSCSAV3VarForbid    = 21,
        eNDSCSAV2VarForbid    = 20,
        eNDSSourceForbid      = 19,
        eUpperDestForbid      = 18,
        eLowerDestForbid      = 17,
        eUnknowKTResp         = 16,
        eLSAD_tDESBasedBadKey = 15,
        eESA_tDESBasedBadKey  = 14,
        eLSAS_tDESBasedBadKey = 13,
        etDESBasedBadKey      = 12,
        eESA_algo_key         = 11,
        eLSAD_algo_key        = 10,
        eLSAS_algo_key        = 9,
        ePES_except           = 8,
    };

    if((event>>eNoSlot)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eNoSlot\n");
    }

    if((event>>eLSADKeyInvalid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSADKeyInvalid\n");
    }

    if((event>>eESAKeyInvalid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eESAKeyInvalid\n");
    }

    if((event>>eLSASKeyInvalid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSASKeyInvalid\n");
    }

    if((event>>eIVInvalid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eIVInvalid\n");
    }

    if((event>>eCAVidUnMatched)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eCAVidUnMatched\n");
    }

    if((event>>eSwitchInvalid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eSwitchInvalid\n");
    }

    if((event>>eAFOnlySCBNotClear)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eAFOnlySCBNotClear\n");
    }

    if((event>>eLSASMDDSCBNotClear)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSASMDDSCBNotClear\n");
    }

    if((event>>eLSASMDDto2ndPID)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSASMDDto2ndPID\n");
    }

    if((event>>eNDSCSAV3VarForbid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eNDSCSAV3VarForbid\n");
    }

    if((event>>eNDSCSAV2VarForbid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eNDSCSAV2VarForbid\n");
    }

    if((event>>eNDSSourceForbid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eNDSSourceForbid\n");
    }

    if((event>>eUpperDestForbid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eUpperDestForbid\n");
    }

    if((event>>eLowerDestForbid)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLowerDestForbid\n");
    }

    if((event>>eUnknowKTResp)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eUnknowKTResp\n");
    }

    if((event>>eLSAD_tDESBasedBadKey)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSAD_tDESBasedBadKey\n");
    }

    if((event>>eESA_tDESBasedBadKey)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eESA_tDESBasedBadKey\n");
    }

    if((event>>eLSAS_tDESBasedBadKey)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSAS_tDESBasedBadKey\n");
    }

    if((event>>etDESBasedBadKey)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event etDESBasedBadKey\n");
    }

    if((event>>eESA_algo_key)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eESA_algo_key\n");
    }

    if((event>>eLSAD_algo_key)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "pktparser_event eLSAD_algo_key\n");
    }

    if((event>>eLSAS_algo_key)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING,"pktparser_event eLSAS_algo_key\n");
    }

    if((event>>ePES_except)&0x1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING,"pktparser_event ePES_except\n");
    }
}

MS_U32 HAL_DSCMB_PktParser_Ctrl(MS_U32 u32Eng ,MS_U32 u32tsif ,MS_BOOL bRst, MS_U32 u32Range ,HAL_DSCMB_PktParser_Mode eViewer, MS_BOOL bEnable )
{
    MS_U32 u32Tmp = 0 ;

    u32Tmp  = ( u32tsif << REG_CIPHER_REG1_TSID_SHT ) & REG_CIPHER_REG1_TSID_MSK ;

    if (bEnable) //enable snap mode, stop when pktcnt is finished
    {
        u32Tmp |=  (REG_CIPHER_REG1_SNAPSHOT | (u32Range & REG_CIPHER_REG1_PARSER_SAMPLE_MSK ));
    }

    if (bRst)
    {
        REG32_W(REG_CIPHER_REG1(u32Eng) , u32Tmp |  REG_CIPHER_REG1_CLEARALL );
        REG32_W(REG_CIPHER_REG1(u32Eng) , u32Tmp & ~REG_CIPHER_REG1_CLEARALL );
        return 0;
    }

    u32Tmp |= ( (eViewer << REG_CIPHER_REG1_VIEWER_SHT) & REG_CIPHER_REG1_VIEWER_MSK );

    REG32_W(REG_CIPHER_REG1(u32Eng) , u32Tmp ) ;

    return REG32_R(REG_CIPHER_PKTVIEW_STATUS(u32Eng));
}


//---------------------------------------
//   Cipher channel control interface
//---------------------------------------
MS_BOOL HAL_DSCMB_Cipher_TCSA3(MS_U32 u32CHNum, MS_BOOL bEnable)
{
    MS_U32 u32Tmp = 0;

    if(u32CHNum >= REG_DSCMB_CIPHER_CNT)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    u32Tmp = REG32_R(REG_CIPHER_REG0(u32CHNum));
    if(bEnable)
    {
        u32Tmp |= REG_CIPHER_REG0_IRD_TCSA3_EN;
    }
    else
    {
        u32Tmp &= ~REG_CIPHER_REG0_IRD_TCSA3_EN;
    }

    REG32_W(REG_CIPHER_REG0(u32CHNum),u32Tmp);

    return TRUE;

}

MS_BOOL HAL_DSCMB_Cipher_DualPath(MS_U32 u32CHNum, MS_BOOL bEnable)
{
    MS_U32 u32Tmp = 0;

    if(u32CHNum >= REG_DSCMB_CIPHER_CNT)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    u32Tmp = REG32_R(REG_CIPHER_REG0(u32CHNum));
    if(bEnable)
    {
        u32Tmp |= REG_CIPHER_REG0_DUALPATH_EN;
    }
    else
    {
        u32Tmp &= ~REG_CIPHER_REG0_DUALPATH_EN;
    }

    REG32_W(REG_CIPHER_REG0(u32CHNum),u32Tmp);

    return TRUE;
}

MS_U32 HAL_DSCMB_Cipher_Output_Ctrl(HAL_DSCMB_OutPut_Eng_Sel EngId, MS_U32 u32ChlSel , MS_BOOL bEnable , MS_BOOL bEncrypt ,
                                    MS_U32 u32TsId , MS_U32 u32CaVid)
{
    MS_U32 u32Tmp;

    u32Tmp  = (1 == u32ChlSel)? REG_TSCE_SRC_CH1:REG_TSCE_SRC_CH0;
    u32Tmp |= (bEnable)?REG_TSCE_SRC_EN:0;
    u32Tmp |=  (u32TsId << REG_TSCE_SRC_TSID_SHT) & REG_TSCE_SRC_TSID_MSK ;
    u32Tmp |=  (u32CaVid << REG_TSCE_SRC_CAVID_SHT) & REG_TSCE_SRC_CAVID_MSK ;

    switch (EngId)
    {
        case E_HAL_DSCMB_CIPHER_OUTPUT_SPS0:
        case E_HAL_DSCMB_CIPHER_OUTPUT_SPS1:
        case E_HAL_DSCMB_CIPHER_OUTPUT_SPS2:
        case E_HAL_DSCMB_CIPHER_OUTPUT_SPS3:
            u32Tmp |= (bEncrypt)?REG_TSCE_SRC_ENC_EN:0;
            REG32_W(REG_TSCE_SRC_SPS_SEL(EngId) , u32Tmp);
            break;

        case E_HAL_DSCMB_CIPHER_OUTPUT_CLR0:
        case E_HAL_DSCMB_CIPHER_OUTPUT_CLR1:
        case E_HAL_DSCMB_CIPHER_OUTPUT_CLR2:
        case E_HAL_DSCMB_CIPHER_OUTPUT_CLR3:
        case E_HAL_DSCMB_CIPHER_OUTPUT_CLR4:
        case E_HAL_DSCMB_CIPHER_OUTPUT_CLR5:
            REG32_W(REG_TSCE_SRC_CLR_SEL(EngId-E_HAL_DSCMB_CIPHER_OUTPUT_CLR0), u32Tmp) ;
            break;
        case E_HAL_DSCMB_CIPHER_OUTPUT_TSO0:
        case E_HAL_DSCMB_CIPHER_OUTPUT_TSO1:
            REG32_W(REG_TSCE_SRC_TSO_SEL(EngId-E_HAL_DSCMB_CIPHER_OUTPUT_TSO0), u32Tmp) ;
            break;
    }
    return u32Tmp;
}

MS_BOOL HAL_DSCMB_Cipher_SPS_Enable(HAL_DSCMB_OutPut_Eng_Sel EngId, MS_BOOL bEncrypt)
{
    MS_U32 u32Tmp = REG32_R(REG_TSCE_SRC_SPS_SEL(EngId));

    if(EngId >= E_HAL_DSCMB_CIPHER_OUTPUT_SPS2)
    {
        return FALSE;
    }

    if(bEncrypt)
    {
        u32Tmp |= REG_TSCE_SRC_ENC_EN;
    }
    else
    {
        u32Tmp &= (~REG_TSCE_SRC_ENC_EN);
    }

    REG32_W(REG_TSCE_SRC_SPS_SEL(EngId) , u32Tmp);

    return TRUE;
}



MS_BOOL HAL_DSCMB_Cipher_Set_SCBFix(MS_U32 EngId ,MS_U32 u32tsid ,HAL_DSCMB_SCBFix_EngSel eEngSel ,
                                    HAL_DSCMB_SCBFix eSCBFix  )
{
    MS_U32 u32Tmp = 0   ;
    // for DMXL, this value will be updated in switch case.
    u32Tmp = eSCBFix << REG_CIPHER_SCBFIX_SHT(u32tsid);

    switch (eEngSel)
    {
        case E_HAL_DSCMB_SCBFIX_LSAD:
            u32Tmp |= SCBFix_Tsid[u32tsid].SCB_LSAD & ~REG_CIPHER_SCBFIX_MSK(u32tsid);
            SCBFix_Tsid[u32tsid].SCB_LSAD = u32Tmp ;
            REG32_W( REG_CIPHER_LSAD_SCB(EngId), u32Tmp);
            break;
        case E_HAL_DSCMB_SCBFIX_ESA :
            u32Tmp |= SCBFix_Tsid[u32tsid].SCB_ESA & ~REG_CIPHER_SCBFIX_MSK(u32tsid);
            SCBFix_Tsid[u32tsid].SCB_ESA = u32Tmp ;
            REG32_W( REG_CIPHER_ESA_SCB(EngId), u32Tmp ) ;
            break;
        case E_HAL_DSCMB_SCBFIX_LSAS:
            SCBFix_Tsid[u32tsid].SCB_LSAS = u32Tmp ;
            u32Tmp |= SCBFix_Tsid[u32tsid].SCB_LSAS & ~REG_CIPHER_SCBFIX_MSK(u32tsid);
            REG32_W( REG_CIPHER_LSAS_SCB(EngId), u32Tmp);
            break;
        case E_HAL_DSCMB_SCBFIX_DMXU:
            u32Tmp |= REG32_R(REG_CIPHER_DMX_SCB(EngId)) & ~REG_CIPHER_DMXU_SCBFIX_MSK(u32tsid);
            REG32_W( REG_CIPHER_DMX_SCB(EngId), u32Tmp);
            break;
        case E_HAL_DSCMB_SCBFIX_DMXL:
            u32Tmp = eSCBFix << REG_CIPHER_DMXL_SCBFIX_SHT(u32tsid);
            if (0 == u32tsid)
            {
                u32Tmp |=  REG32_R(REG_CIPHER_DMX_SCB(EngId)) & ~REG_CIPHER_DMXL_SCBFIX_MSK(u32tsid);
                REG32_W( REG_CIPHER_DMX_SCB(EngId), u32Tmp );
            }
            else
            {
                u32Tmp |=  REG32_R(REG_CIPHER_EKEY_INVALID(EngId)) & ~REG_CIPHER_DMXL_SCBFIX_MSK(u32tsid);
                REG32_W( REG_CIPHER_EKEY_INVALID(EngId),  u32Tmp );
            }
            break;

        default:

            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            return FALSE ;
    }

    return TRUE;

}

MS_BOOL HAL_DSCMB_Cipher_Set_Default_Algo
( MS_U32 u32CHNum ,
  DSCMB_MainAlgo_Type eAlgoType,
  DSCMB_SubAlgo_Type eSubAlgo,
  DSCMB_ResSBAlgo_Type eResType,
  DSCMB_ResSBAlgo_Type eSBType)
{
    HAL_DSCMB_MainAlgo_Type  eAlgo = _HAL_DSCMB_GetAlgoType(eAlgoType);
    HAL_DSCMB_SubAlgo_Type   eSub  = _HAL_DSCMB_GetSubAlgoType(eSubAlgo);
    HAL_DSCMB_ResSBAlgo_Type eRes  = _HAL_DSCMB_GetResSBType(eResType);
    HAL_DSCMB_ResSBAlgo_Type eSB   = _HAL_DSCMB_GetResSBType(eSBType);


    if(u32CHNum >= REG_DSCMB_CIPHER_CNT)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }
    MS_U32 u32Tmp = 0x00000264;
    u32Tmp  = ((eAlgo << REG_CIPHER_ESA_Algo_SHT   ) & REG_CIPHER_ESA_Algo_MSK   );
    u32Tmp |= ((eSub  << REG_CIPHER_ESA_SubAlgo_SHT) & REG_CIPHER_ESA_SubAlgo_MSK);
    u32Tmp |= ((eRes  << REG_CIPHER_ESA_RES_SHT    ) & REG_CIPHER_ESA_RES_MSK    );
    u32Tmp |= ((eSB   << REG_CIPHER_ESA_SB_SHT     ) & REG_CIPHER_ESA_SB_MSK     );

    REG32_W(REG_CIPHER_ESA_DEFAULT_ALGO(u32CHNum), u32Tmp);
    //printf("[%s] Default algo = %08x\n", __FUNCTION__, u32Tmp);
    return TRUE;
}

MS_BOOL HAL_DSCMB_Cipher_Set_CSA2_CTRL(MS_U32 EngId )
{
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_CIPHER_CSA2_CTRL(EngId));
    u32Tmp |= REG_CIPHER_CSA2_KEEP_TS_SCB;
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "[%s] u32Tmp enable CSA2_KEEP_TS_SCB = %ld\n", __FUNCTION__, u32Tmp);

    REG32_W(REG_CIPHER_CSA2_CTRL(EngId)  , u32Tmp);
    return TRUE;
}

//---------------------------------------
//   TSCE SPD SPS-PVR interface
//---------------------------------------
MS_U32 HAL_DSCMB_FltSrc2TSIF(DSCMB_TSIF ePidFltSrc)
{
    switch (ePidFltSrc)
    {
        case E_DSCMB_TSIF_LIVE0:
            return E_HAL_DSCMB_TSIF0;

        case E_DSCMB_TSIF_FILE0:
            return E_HAL_DSCMB_TSIF1;

        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d] Wrong filter source!!!\n",__FUNCTION__, __LINE__);
            break;
    }

    return E_HAL_DSCMB_TSIF_NUM;
}

MS_U32 HAL_DSCMB_FltSrc2PktDmx(DSCMB_TSIF ePidFltSrc)
{
    switch (ePidFltSrc)
    {
        case E_DSCMB_TSIF_LIVE0:
            return E_HAL_DSCMB_TSID0;

        case E_DSCMB_TSIF_FILE0:
            return E_HAL_DSCMB_TSID1;

        default:
            break;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "[%s][%d] Wrong filter source!!!\n",__FUNCTION__, __LINE__);
    return E_HAL_DSCMB_TSID_NUM;
}

MS_U32 HAL_DSCMB_GetChannlId(DSCMB_TSIF ePidFltSrc)
{
    return 0;
}


MS_U32 HAL_DSCMB_GetProgId(DSCMB_CAPVR_MODE eCaMode)
{
    return 0;
}

MS_BOOL _HAL_DSCMB_SPD_Bypass_Enable(MS_U32 u32Tsif, MS_BOOL bByPassEn)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    if(u32Tsif >= HAL_DSCMB_TSIF_MAX)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        DSCMB_HW_UNLOCK(_eHW_SemId);
        return FALSE;
    }

    MS_U32 u32Tmp = REG_TSP_SPD_TSIF0_BYPASS_EN << u32Tsif;

    if(bByPassEn)
    {
        REG16_SET(REG_TSP_SPD_BYPASS, u32Tmp);
    }
    else
    {
        REG16_CLR(REG_TSP_SPD_BYPASS, u32Tmp);
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "SPD bypass = [%d]  \n",REG16_R(REG_TSP_SPD_BYPASS));

    DSCMB_HW_UNLOCK(_eHW_SemId);

    return TRUE;
}

MS_BOOL HAL_DSCMB_SPD_Enable(MS_U32 u32Tsif)
{
    if(u32Tsif >= HAL_DSCMB_TSIF_MAX)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    _HAL_DSCMB_SPD_Bypass_Enable(u32Tsif, FALSE);

    return TRUE;
}


MS_BOOL HAL_DSCMB_SPD_Disable(MS_U32 u32Tsif)
{
    if(u32Tsif >= HAL_DSCMB_TSIF_MAX)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    _HAL_DSCMB_SPD_Bypass_Enable(u32Tsif, TRUE);

    return TRUE;
}

MS_BOOL HAL_DSCMB_SPD_Reset(MS_U32 u32Tsif)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    if(u32Tsif >= HAL_DSCMB_TSIF_MAX)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        DSCMB_HW_UNLOCK(_eHW_SemId);
        return FALSE;
    }

    switch (u32Tsif)
    {
        case 0:
            REG16_SET(REG_TSP_SPD_RESET, REG_TSP_SPD_TSIF0_RESET);
            REG16_CLR(REG_TSP_SPD_RESET, REG_TSP_SPD_TSIF0_RESET);
            break;

        case 1:
            REG16_SET(REG_TSP_SPD_RESET, REG_TSP_SPD_TSIF1_RESET);
            REG16_CLR(REG_TSP_SPD_RESET, REG_TSP_SPD_TSIF1_RESET);
            break;

        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            DSCMB_HW_UNLOCK(_eHW_SemId);
            return FALSE;
    }

    DSCMB_HW_UNLOCK(_eHW_SemId);

    return TRUE;
}

MS_BOOL HAL_DSCMB_SPSPVR_Reset( MS_U32 EngId)
{
    MS_U32 u32Tmp = REG32_R((MS_U32)REG_TSCE_CTRL);

    if(EngId >= HAL_DSCMB_SPSPVR_ENG_NUM)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }
/*
    if( MDrv_SYS_GetChipRev() == 0x1)
    {
        /// keltic U02

        // Set Low Reset
        u32Tmp &= ~(REG_TSCE_SPSPVR_RST_EN(EngId));
        REG32_W((MS_U32)REG_TSCE_CTRL, (MS_U32)u32Tmp);

        // Set High Enable
        u32Tmp |=  (REG_TSCE_SPSPVR_RST_EN(EngId));
        REG32_W((MS_U32)REG_TSCE_CTRL, (MS_U32)u32Tmp);
    }
    else
    {
        /// keltic U01
        MS_U32 u32Tmp = REG32_R((MS_U32)REG_TSCE_SPSPVR_CTRL);
        u32Tmp &= ~(REG_TSCE_SPSPVR_EN(EngId));
        REG32_W((MS_U32)REG_TSCE_SPSPVR_CTRL, (MS_U32)u32Tmp);
    }
*/
    return TRUE;
}

MS_BOOL HAL_DSCMB_SPSPVR_SetBuffer( MS_U32 EngId , MS_U32 BufSel ,MS_U32 u32Buf0 , MS_U32 u32Size0 ,
                                    MS_U32 u32Buf1 , MS_U32 u32Size1 )
{
    if(EngId >= HAL_DSCMB_SPSPVR_ENG_NUM || BufSel >= HAL_DSCMB_SPSPVR_BUF_NUM)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    REG32_W(REG_TSCE_SPSPVR_BUF_HEAD1(EngId,BufSel), u32Buf0 >> DSCMB_MIU_SHT );
    REG32_W(REG_TSCE_SPSPVR_BUF_TAIL1(EngId,BufSel), (u32Buf0 + u32Size0) >> DSCMB_MIU_SHT );
    REG32_W(REG_TSCE_SPSPVR_BUF_MID1 (EngId,BufSel), u32Buf0 >> DSCMB_MIU_SHT );

    REG32_W(REG_TSCE_SPSPVR_BUF_HEAD2(EngId,BufSel), u32Buf1 >> DSCMB_MIU_SHT);
    REG32_W(REG_TSCE_SPSPVR_BUF_TAIL2(EngId,BufSel), (u32Buf1 + u32Size1) >> DSCMB_MIU_SHT );
    REG32_W(REG_TSCE_SPSPVR_BUF_MID2 (EngId,BufSel), u32Buf1 >> DSCMB_MIU_SHT);
    return TRUE;
}

MS_BOOL _HAL_DSCMB_SPSPVR_Start( MS_U32 EngId , MS_BOOL bEnable , MS_BOOL bDataSwap )
{
    //DataSwap function is no longer avaliable
    if(EngId >= HAL_DSCMB_SPSPVR_ENG_NUM)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    MS_U32 u32Tmp = 0 ;

    u32Tmp  = (bEnable)?(REG_TSCE_SPSPVR_EN(EngId)):0 ;
    //u32Tmp |= (bDataSwap)?(REG_TSCE_SPSPVR_SWP(EngId)):0 ;
    REG32_W(REG_TSCE_SPSPVR_CTRL, (REG32_R(REG_TSCE_SPSPVR_CTRL) & ~(REG_TSCE_SPSPVR_EN(EngId)|REG_TSCE_SPSPVR_SWP(EngId)))
            |u32Tmp);
    return TRUE;
}

MS_BOOL HAL_DSCMB_SPSPVR_Stop( MS_U32 EngId)
{
    if(EngId >= HAL_DSCMB_SPSPVR_ENG_NUM)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    _HAL_DSCMB_SPSPVR_Start(EngId, FALSE, FALSE);
    return TRUE;
}

MS_BOOL HAL_DSCMB_SPSPVR_Start( MS_U32 EngId)
{
    if(EngId >= HAL_DSCMB_SPSPVR_ENG_NUM)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    _HAL_DSCMB_SPSPVR_Start(EngId, TRUE, FALSE);
    return TRUE;
}

MS_BOOL HAL_DSCMB_SPSPVR_SetPid( MS_U32 EngId , MS_U32 u32SPSPVRFltId , MS_U32 U32Pid , MS_BOOL bBuf0, MS_BOOL bBuf1 )
{
    if ((EngId >= HAL_DSCMB_SPSPVR_ENG_NUM) || ( u32SPSPVRFltId >= HAL_DSCMB_SPSPVR_FLT_NUM ))
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    MS_U32 u32Tmp = 0 ;

    u32Tmp = U32Pid & REG_DSCMB_SPSPVR_PID_MSK ;
    u32Tmp |= (bBuf0)?REG_DSCMB_SPSPVR_BUF0_EN:0 ;
    u32Tmp |= (bBuf1)?REG_DSCMB_SPSPVR_BUF1_EN:0 ;
    REG32_W(REG_DSCMB_SPSPVR_FLT(EngId,u32SPSPVRFltId) , u32Tmp );

    return TRUE ;
}

MS_BOOL HAL_DSCMB_SPSPVR_GetWPtr( MS_U32 EngId , MS_U32 *pu32WPtr0, MS_U32 *pu32WPtr1 )
{
    if (EngId >= HAL_DSCMB_SPSPVR_ENG_NUM || pu32WPtr0 == NULL || pu32WPtr1 == NULL)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE;
    }

    *pu32WPtr0 = REG32_R(REG_TSCE_SPSPVR_WPTR(EngId, 0));
    *pu32WPtr1 = REG32_R(REG_TSCE_SPSPVR_WPTR(EngId, 1));

    return TRUE;
}

//=======================================
//      Key Ladder Interface
//=======================================
MS_BOOL HAL_DSCMB_KL_Start( DSCMB_KL_SelEng eKLEng )
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_DSCMB_KL_CTRL(u32KLEng));
    u32Tmp |=0x1;

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
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

        HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
        REG32_W(REG_DSCMB_KL_SWRST, u32Tmp);
        u32Tmp &= ~REG_DSCMB_KL_SWRST_PVR;

        HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
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
#if 0
    u32Tmp |= 0x000700000;
#endif /* 1 */

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
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
    eHalDstDma = _HAL_DSCMB_GetKLDstDMA(eDst);
    eHalDstKt = _HAL_DSCMB_GetKLDstKT(eDst);

    u32Tmp |= (eHalDstDma   <<  DSCMB_KL_KEYBUS_DSTDMA_SHT);
    u32Tmp |= (eHalKeyType  <<  DSCMB_KL_KEYBUS_SCB_SHT);
    u32Tmp |= (eHalDstKt    <<  DSCMB_KL_KEYBUS_FIELD_SHT);
    u32Tmp |= (u32FltId     <<  DSCMB_KL_KEYBUS_PIDNO_SHT);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
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
    HAL_DSCMB_KL_Dst eHalKLDst = _HAL_DSCMB_GetKLDst(eDst);

    u32Tmp = REG32_R(REG_DSCMB_KL_CTRL(u32KLEng));

    if(eAlgo == E_DSCMB_KL_AES)
    {
        u32Tmp |= 0x2;
    }

    u32Tmp |= ((eHalKLType<<4)&0xF0);
    u32Tmp |= ((eHalKLSrc<<8)&0xF00);
    u32Tmp |= ((eHalKLDst<<16)&0xF0000);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
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
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set\n");

    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+0,(pu8Key[12] <<24)|(pu8Key[13] <<16)|(pu8Key[14] <<8)|(pu8Key[15] <<0));
    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+1,(pu8Key[8]  <<24)|(pu8Key[9]  <<16)|(pu8Key[10] <<8)|(pu8Key[11] <<0));
    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+2,(pu8Key[4]  <<24)|(pu8Key[5]  <<16)|(pu8Key[6]  <<8)|(pu8Key[7]  <<0));
    REG32_W(REG_DSCMB_KL_SW_KEY(u32KLEng)+3,(pu8Key[0]  <<24)|(pu8Key[1]  <<16)|(pu8Key[2]  <<8)|(pu8Key[3]  <<0));

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_Input(DSCMB_KL_SelEng eKLEng, MS_U32 u32In, MS_U8 *pu8Key)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set u32In: 0x%X\n", (unsigned int)u32In);

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

MS_BOOL HAL_DSCMB_KL_ErrMsg(DSCMB_KL_SelEng eKLEng, MS_U32 *pu32ErrMsg)
{
    MS_U32 u32KLEng = _HAL_DSCMB_GetKLEng(eKLEng);
    MS_U32 u32Tmp = 0;
    u32Tmp = REG32_R(REG_DSCMB_KL_ERR_MSG(u32KLEng));
    *pu32ErrMsg = u32Tmp;

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_ErrStatus(DSCMB_KL_SelEng eKLEng)
{
    MS_U32 u32ErrMsg = 0;
    HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);

    if( (u32ErrMsg & ErrStatusMak) == 0)
    {
        //no error flag raised
        return TRUE;
    }
    //err happend
    HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Status: 0x%X\n", (unsigned int)u32ErrMsg);
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

// Assume the parameter pu8Code is MSB first
MS_BOOL HAL_DSCMB_KL_TCSA3_ActCode(MS_U8 *pu8Code, MS_U32 u32Size)
{
    MS_U32 u32Tmp = 0;
    if(u32Size != 16)  return FALSE;

    u32Tmp = (pu8Code[0] <<24)| (pu8Code[1] <<16)| (pu8Code[2] <<8)| (pu8Code[3] <<0);
    REG32_W(REG_DSCMB_KL_TCSA3_ACT_CODE + 3, u32Tmp);

    u32Tmp = (pu8Code[4] <<24)| (pu8Code[5] <<16)| (pu8Code[6] <<8)| (pu8Code[7] <<0);
    REG32_W(REG_DSCMB_KL_TCSA3_ACT_CODE + 2, u32Tmp);

    u32Tmp = (pu8Code[8] <<24)| (pu8Code[9] <<16)| (pu8Code[10] <<8)|(pu8Code[11] <<0);
    REG32_W(REG_DSCMB_KL_TCSA3_ACT_CODE + 1, u32Tmp);

    u32Tmp = (pu8Code[12] <<24)|(pu8Code[13] <<16)|(pu8Code[14] <<8)|(pu8Code[15] <<0);
    REG32_W(REG_DSCMB_KL_TCSA3_ACT_CODE + 0, u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_TCSA3_CHSel(MS_U32 u32Ch)
{
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_DSCMB_KL_TCSA3_CTRL);

    switch(u32Ch)
    {
        case 0:
            u32Tmp |= (REG_DSCMB_KL_TCSA3_CHSEL0<<REG_DSCMB_KL_TCSA3_CHSEL_SHT);
            break;
        case 1:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Wrong DSCMB Channel select\n");
            return FALSE;
        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "Wrong DSCMB Channel select\n");
            return FALSE;
    }

    REG32_W(REG_DSCMB_KL_TCSA3_CTRL, u32Tmp);

    return TRUE;
}

MS_BOOL HAL_DSCMB_KL_TCSA3_Start(void)
{
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_DSCMB_KL_TCSA3_CTRL);
    u32Tmp |= (REG_DSCMB_KL_TCSA3_START);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set 0x%X\n",(unsigned int)u32Tmp);
    REG32_W(REG_DSCMB_KL_TCSA3_CTRL,u32Tmp);

    while(1)
    {
        u32Tmp = REG32_R(REG_DSCMB_KL_TCSA3_CTRL);
        if((u32Tmp & 0x1) == 0x0)   break;
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
    //MS_U32 u32HWKeyId = _HAL_DSCMB_GetKLKdfHWKeyId(eHWKeyId);

    MS_U32 u32Tmp  = 0;
    if(bEnable)
    {
        u32Tmp  = REG_DSCMB_KL_KDF_EN;
        u32Tmp |= (u16AppId   <<  REG_DSCMB_KL_KDF_APP_ID_SHT)   & REG_DSCMB_KL_KDF_APP_ID_MSK;
        u32Tmp |= (u32HashSel <<  REG_DSCMB_KL_KDF_HASH_SEL_SHT) & REG_DSCMB_KL_KDF_HASH_SEL_MSK;
//        u32Tmp |= (u32HWKeyId <<  REG_DSCMB_KL_KDF_HWKEY_ID_SHT) & REG_DSCMB_KL_KDF_HWKEY_ID_MSK;
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Set u32KLEng: %ld, KDF:0x%08lX\n", u32KLEng, u32Tmp);

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

static void HAL_TSP_Set_RcvBuf_Src(MS_U32 bufIdx, MS_U32 inputSrc)
{
    switch (bufIdx)
    {
        case 0:
            REG16_MSK_W(REG_TSP_RCV_BUF_SRC, REG_TSP_RCV_BUF0_SRC_MSK, inputSrc<<(bufIdx * 2));
            break;
        case 1:
            REG16_MSK_W(REG_TSP_RCV_BUF_SRC, REG_TSP_RCV_BUF1_SRC_MSK, inputSrc<<(bufIdx * 2));
            break;
    }
}

MS_BOOL HAL_DSCMB_KL_SP_KeyCtrl(DSCMB_KL_SelEng  eKLEng, DSCMB_KLDst eDst)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d] FAIL, No support!\n", __FUNCTION__, __LINE__);
    return FALSE;
}

MS_BOOL HAL_DSCMB_KL_SP_Start(DSCMB_KL_SelEng eKLEng)
{
    HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d] FAIL, No support!\n", __FUNCTION__, __LINE__);
    return FALSE;
}

//========================================
//    Pid Filter
//========================================
MS_BOOL HAL_DSCMB_PidFlt_SetPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec)
{
    MS_U32 u32PairSet = 1;//only open one pair set
    MS_U32 u32TsidPri = 0;
    MS_U32 u32TsidSec = 0;

    HAL_DSCMB_Get_TsidInput(u32FltIdPri, &u32TsidPri);
    HAL_DSCMB_Get_TsidInput(u32FltIdSec, &u32TsidSec);

    if(u32TsidPri != u32TsidSec)
    {
        //error pid filter source
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d] FAIL, Invaild TS source \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    HAL_DSCMB_PidFlt_2ndPid( u32FltIdSec, TRUE);
    HAL_DSCMB_PidFlt_PIDPair(u32FltIdPri, u32PairSet, TRUE);
    HAL_DSCMB_PidFlt_PIDPair(u32FltIdSec, u32PairSet, TRUE);

    return TRUE;
}

MS_BOOL HAL_DSCMB_PidFlt_ClearPidPair(MS_U32 u32FltIdPri, MS_U32 u32FltIdSec)
{
    MS_U32 u32PairSet = 1;//only open one pair set
    MS_U32 u32TsidPri = 0;
    MS_U32 u32TsidSec = 0;

    HAL_DSCMB_Get_TsidInput(u32FltIdPri, &u32TsidPri);
    HAL_DSCMB_Get_TsidInput(u32FltIdSec, &u32TsidSec);

    if(u32TsidPri != u32TsidSec)
    {
        //error pid filter source
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d] FAIL, Invaild TS source \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    HAL_DSCMB_ClearPidPairStatus(u32TsidPri, u32PairSet);

    HAL_DSCMB_PidFlt_2ndPid( u32FltIdSec, FALSE);
    HAL_DSCMB_PidFlt_PIDPair(u32FltIdPri, u32PairSet, FALSE);
    HAL_DSCMB_PidFlt_PIDPair(u32FltIdSec, u32PairSet, FALSE);

    return TRUE;
}

void _HAL_DSCMB_PidFlt_Reset(MS_U32 u32FltId)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt = REG_PIDFLT0(u32FltId);

    //Set CC to 0xFF
    TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) | REG_TSP_PIDFLT_CC_MSK);

    //Set   Key_en    to 0
    //Set   2ndPid     to 0
    //Set   PP[x]       to 0
    TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) & ~(REG_TSP_PIDFLT_PP_MSK|REG_TSP_PIDFLT_KEYEN|REG_TSP_PIDFLT_2NDPID));

    DSCMB_HW_UNLOCK(_eHW_SemId);
}

void HAL_DSCMB_PidFlt_PIDPair(MS_U32 u32FltId, MS_U32 u32PPNo, MS_BOOL bEnable)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt = REG_PIDFLT0(u32FltId);
    MS_U32 u32Tmp;

    switch(u32PPNo)
    {
        case 1:
            u32Tmp = REG_TSP_PIDFLT_PP1;
            break;
        case 2:
            u32Tmp = REG_TSP_PIDFLT_PP2;
            break;
        case 3:
            u32Tmp = REG_TSP_PIDFLT_PP3;
            break;
        case 4:
            u32Tmp = REG_TSP_PIDFLT_PP4;
            break;
        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            DSCMB_HW_UNLOCK(_eHW_SemId);
            return;
    }

    if (bEnable)
    {
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) | u32Tmp );
    }
    else
    {
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) & ~u32Tmp );
    }

    DSCMB_HW_UNLOCK(_eHW_SemId);

}


void HAL_DSCMB_PidFlt_2ndPid(MS_U32 u32FltId, MS_BOOL bEnable)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt = REG_PIDFLT0(u32FltId);

    if (bEnable)
    {
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) | REG_TSP_PIDFLT_2NDPID );
    }
    else
    {
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) & ~REG_TSP_PIDFLT_2NDPID );
    }

    DSCMB_HW_UNLOCK(_eHW_SemId);
}


void HAL_DSCMB_PidFlt_EnableKey(MS_U32 u32FltId, MS_BOOL bEnable)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt = REG_PIDFLT0(u32FltId);

    if (bEnable)
    {
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) | REG_TSP_PIDFLT_KEYEN );
    }
    else
    {
        TSP32_IdrW(pPidFlt, TSP32_IdrR(pPidFlt) & ~REG_TSP_PIDFLT_KEYEN );
    }

    DSCMB_HW_UNLOCK(_eHW_SemId);
}

MS_U32 HAL_DSCMB_PidFlt_GetPid(MS_U32 u32FltId)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt  = REG_PIDFLT0(u32FltId);
    MS_U32 u32Tmp   = TSP32_IdrR(pPidFlt);
    MS_U32 u32Value = u32Tmp & REG_TSP_PIDFLT_PID_MSK;

    DSCMB_HW_UNLOCK(_eHW_SemId);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "FltId%ld=0x%04lx\n", u32FltId, u32Value );

    return u32Value;
}

void HAL_DSCMB_Get_TsidInput(MS_U32 u32FltId, MS_U32 *u32Tsid)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt = REG_PIDFLT0(u32FltId);
    MS_U32 u32Tmp  = TSP32_IdrR(pPidFlt);

    *u32Tsid = (u32Tmp & REG_TSP_PIDFLT_TS_SRC_MSK) >> REG_TSP_PIDFLT_TS_SRC_SHT;

    DSCMB_HW_UNLOCK(_eHW_SemId);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "Get TSID %lu !!!!!!!!!!!!!\n", *u32Tsid);
}

// CA PVR Destination
void HAL_DSCMB_SetTSPCADst(MS_U32 u32FltId, MS_U32 u32UpDst, MS_U32 u32LowDst)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    MS_U32 pPidFlt = REG_PIDFLT1(u32FltId);
    MS_U32 u32tmp = 0;

    u32tmp |= (u32UpDst  << REG_TSP_PIDFLT_UPDST_SHT) & REG_TSP_PIDFLT_UPDST_MSK;
    u32tmp |= (u32LowDst << REG_TSP_PIDFLT_LODST_SHT) & REG_TSP_PIDFLT_LODST_MSK;

    TSP32_IdrW(pPidFlt, (TSP32_IdrR(pPidFlt) & ~(REG_TSP_PIDFLT_UPDST_MSK|REG_TSP_PIDFLT_LODST_MSK)) | u32tmp);

    DSCMB_HW_UNLOCK(_eHW_SemId);
}

void HAL_DSCMB_SetTSPCAVid(MS_U32 u32Tsid, MS_U32 u32CAVid)
{
    DSCMB_HW_LOCK(_eHW_SemId);
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "u32Tsid: 0x%lu, Set CAVid: 0x%lu\n", u32Tsid, u32CAVid);

    switch(u32Tsid)
    {
        case 0:
            REG16_MSK_W(REG_TSP_CAVID0, REG_TSP_PIDFLT_BF0_CAVID_MSK , u32CAVid);
            HAL_DSCMB_Cipher_Output_Ctrl(E_HAL_DSCMB_CIPHER_OUTPUT_CLR0, 0, TRUE, FALSE, 0, u32CAVid);
            break;
        case 1:
            REG16_MSK_W(REG_TSP_CAVID0, REG_TSP_PIDFLT_BF1_CAVID_MSK , u32CAVid<<5);
            HAL_DSCMB_Cipher_Output_Ctrl(E_HAL_DSCMB_CIPHER_OUTPUT_CLR1, 0, TRUE, FALSE, 1, u32CAVid);
            break;
        default:
            ;
    }
    DSCMB_HW_UNLOCK(_eHW_SemId);
}

MS_BOOL HAL_DSCMB_ClearPidPairStatus(MS_U32 u32Tsid, MS_U32 u32PidPairNum)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "u32Tsid: 0x%lX, reset pid pair: 0x%lX\n", u32Tsid, u32PidPairNum);
#define CFG3_0D_REG_PIDFLT0_CC_STATUS_SHIFT                             0x8
#define CFG3_0D_REG_PIDFLT1_CC_STATUS_SHIFT                             0x12

    if(u32PidPairNum > REG_TSP_PIDFLT_PP_CNT ||  u32PidPairNum<= 0)
    {
        DSCMB_HW_UNLOCK(_eHW_SemId);
        return FALSE;  //1~4
    }

    MS_U32 u32Tmp = 0;
    switch(u32Tsid)
    {
        case 0:
            u32Tmp = 1 << ( u32PidPairNum - 1 ) << CFG3_0D_REG_PIDFLT0_CC_STATUS_SHIFT;
            break;
        case 1:
            u32Tmp = 1 << ( u32PidPairNum - 1 ) << CFG3_0D_REG_PIDFLT1_CC_STATUS_SHIFT;
            break;
        default:
            HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
            DSCMB_HW_UNLOCK(_eHW_SemId);
            return FALSE;
    }

    REG16_MSK_W(REG_TSP_CLEAR_PIDPAIR_STATUS0, u32Tmp , u32Tmp);
    REG16_MSK_W(REG_TSP_CLEAR_PIDPAIR_STATUS0, u32Tmp , 0);

    DSCMB_HW_UNLOCK(_eHW_SemId);

    return TRUE;
}



void HAL_DSCMB_ConnectPath(MS_U32 u32Idx ,MS_BOOL bEnable)
{
    DSCMB_HW_LOCK(_eHW_SemId);

    if(bEnable)
    {
        HAL_TSP_Set_RcvBuf_Src(u32Idx, 2); //data from DSCMB
    }
    else
    {
        HAL_TSP_Set_RcvBuf_Src(u32Idx, 0); //data from Merge 0
    }

    DSCMB_HW_UNLOCK(_eHW_SemId);
}

void HAL_DSCMB_ReadPidSlotMap(MS_U32 PidFltNo,PidSlotMapRead_t *pReadData)
{
    _HAL_DSCMB_PidIdx_MuxRead(PidFltNo,(MS_U32*)pReadData);
}

MS_BOOL HAL_DSCMB_CC_Respin(MS_U32 u32CHNum, MS_U32 u32Tsid, MS_BOOL bEnable)
{
    MS_U32 u32Tmp = 0;

    u32Tmp = REG32_R(REG_CIPHER_REG0(u32CHNum));
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "[%s] u32Tmp = 0x%p\n", __FUNCTION__,(MS_U32*) u32Tmp);
    if(bEnable)
    {
        u32Tmp |= (REG_CIPHER_REG0_TSID0_CC_RESPIN_EN << u32Tsid);
    }
    else
    {
        u32Tmp &= ~(REG_CIPHER_REG0_TSID0_CC_RESPIN_EN << u32Tsid);
    }
    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "[%s] after u32Tmp = 0x%p\n", __FUNCTION__, (MS_U32*)u32Tmp);

    REG32_W(REG_CIPHER_REG0(u32CHNum),u32Tmp);

    return TRUE;
}


void HAL_DSCMB_LowRecord(MS_BOOL bEnable)
{
    MS_U32 u32Tmp = REG32_R(REG_TSCE_SRC_SPS_SEL(0));
    if(bEnable)
    {
        u32Tmp |= 0x8;
    }
    else
    {
        u32Tmp &= (~0x8);
    }

    HALDSCMB2_DBG(DSCMB2_DBGLV_DEBUG, "[%s] 0x%lx", __FUNCTION__, u32Tmp);
    REG32_W(REG_TSCE_SRC_SPS_SEL(0) , u32Tmp);
}

MS_U32 _HAL_KTE_REG0_Merge(MS_U32 field, MS_U32 CMD, MS_U32 CAVid)
{
    MS_U32 u32Reg0 = 0;

    // Prepare Reg0 write data
    u32Reg0 = (REG_KTE_AKI_GO)                                 //Go
              |( (field<<REG_KTE_SEL_SFT)  &REG_KTE_SEL_MSK )   //Field  (0:LSAD  1:ESA   2:LSAS   3:SW   4:IV)
              |( (CMD  <<REG_KTE_CMD_SFT)  &REG_KTE_CMD_MSK )   //CMD  (1:WriteKey  2:InvalidateKey  3:ReadSW   4:EnableKey)
              |( (CAVid<<REG_KTE_CAVID_SHT)&REG_KTE_CAVID_MSK); //CAVID

    return u32Reg0;
}

MS_U32 _HAL_KTE_REG1_Merge(MS_U32 pid_no, MS_U32 SCB, MS_U32 Entropy, MS_U32 Usage)
{
    MS_U32 u32Reg1 = 0;

    u32Reg1 = (pid_no & REG_KTE_FLTID_MSK)                         //PID No
              |( (SCB     << REG_KTE_SCB_SHT)   & REG_KTE_SCB_MSK)    //SCB
              |( (Entropy << REG_KTE_ETPY_SHT)  & REG_KTE_ETPY_MSK)   //Key Entropy
              |( (Usage   << REG_KTE_USAGE_SHT) & REG_KTE_USAGE_MSK); //Key Usage

    return u32Reg1;
}

MS_BOOL _HAL_KTE_ReadSwitch(MS_U32 u32fltid, MS_U32 u32CAVid,
                            DSCMB_Key_Type type,  MS_U32 *u32data)
{
    MS_U32 u32Reg0= 0;
    MS_U32 u32Reg1= 0;
    HAL_DSCMB_KeyType etype;

    if (u32fltid >= HAL_DSCMB_PIDIDX_MAX  )
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    //[NOTE] Type translation //
    etype = _HAL_DSCMB_GetKeyType(type);

    // Prepare Reg1 write data
    u32Reg1 = _HAL_KTE_REG1_Merge(u32fltid,etype,0,0);

    u32Reg0 = _HAL_KTE_REG0_Merge( REG_KTE_FIELD_SWITCH, REG_KTE_CMD_READSWITCH /*ReadSW*/,u32CAVid);

    //write reg 1
    REG32_W(REG_KTE_CMD_CTRL1 , u32Reg1);
    //write reg 0
    REG32_W(REG_KTE_CMD_CTRL0 , u32Reg0);

    // Read switch data (reg2~6)

    u32data[0] = REG32_R(REG_KTE_DATA0);
    u32data[1] = REG32_R(REG_KTE_DATA1);
    u32data[2] = REG32_R(REG_KTE_DATA2);
    u32data[3] = REG32_R(REG_KTE_DATA3);
    u32data[4] = REG32_R(REG_KTE_DATA4);

    return TRUE ;
}

MS_BOOL _HAL_KTE_WriteSwitch(MS_U32 u32fltid, MS_U32 u32CAVid,
                             DSCMB_Key_Type type,  MS_U32 *u32data)
{
    MS_U32 u32Reg0= 0;
    MS_U32 u32Reg1= 0;
    HAL_DSCMB_KeyType etype;

    if (u32fltid >= HAL_DSCMB_PIDIDX_MAX )
    {
        // ivalid pid id
        HALDSCMB2_DBG(DSCMB2_DBGLV_ERR, "[%s][%d]  FAIL\n",__func__, __LINE__);
        return FALSE ;
    }

    //[NOTE] Type translation //
    etype     = _HAL_DSCMB_GetKeyType(type);


    u32Reg1 = _HAL_KTE_REG1_Merge(u32fltid,etype,0,0);

    u32Reg0 = _HAL_KTE_REG0_Merge(REG_KTE_FIELD_SWITCH, REG_KTE_CMD_WRITE_KEY /*WriteKey*/,u32CAVid);

    // write switch data (reg2~6)
    REG32_W(REG_KTE_DATA0, u32data[0] );
    REG32_W(REG_KTE_DATA1, u32data[1] );
    REG32_W(REG_KTE_DATA2, u32data[2] );
    REG32_W(REG_KTE_DATA3, u32data[3] );
    REG32_W(REG_KTE_DATA4, u32data[4] );

    //write reg 1
    REG32_W(REG_KTE_CMD_CTRL1 , u32Reg1);
    //write reg 0
    REG32_W(REG_KTE_CMD_CTRL0 , u32Reg0);


    return TRUE ;
}

// TODO: FIXME, this should by different CA
void HAL_DSCMB_PrintSwitchSetup(MS_U32 u32FltId, DSCMB_Key_Type type, MS_U32 u32CAVid)
{
    MS_U32 rswitch[5]={0,0,0,0,0};

    _HAL_KTE_ReadSwitch(u32FltId, u32CAVid, type,(MS_U32 *) &rswitch[0]);

    HALDSCMB2_DBG("u32CAVid = %lu key type      = %d\n",u32CAVid, type);

    if(u32CAVid == E_HAL_DSCMB_CAVID1)
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "forcescb      = %lx\n",  rswitch[0]>>0   & 0x3);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "input_source  = %lx\n", (rswitch[0]>>14) & 0xff);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "low_dest      = %lx\n", (rswitch[0]>>22) & 0xff);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "upp_dest      = %lx\n", ((rswitch[0]>>30) & 0x3) | (((rswitch[4]>>8)  & 0x1f)<<2) | (((rswitch[4]>>16) & 0x1) <<7));

        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "dvbcsa_var    = %lx\n", ((rswitch[4]>>17) & 0x3) | (((rswitch[4]>>15) & 0x1) <<2) | (((rswitch[0]>>2)  & 0x3) <<3));
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "permutation   = %lx\n", ((rswitch[1]>>0)  & 0x1) | (((rswitch[1]>>4)  & 0x1) <<1) | (((rswitch[1]>>8)  & 0x1) <<2));
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "modifyxrc     = %lx\n", (rswitch[1]>>9)  & 0x1);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "low_switch    = %lx\n", (rswitch[2]>>23) & 0xf);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "upp_switch    = %lx\n", (rswitch[3]>>9)  & 0xf);

        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_select    = %lx\n",   (rswitch[1]>>22) & 0xf);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_subselect = %lx\n", ((rswitch[1]>>18) & 0x1) | (((rswitch[1]>>20) & 0x3) <<1));
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "localselect_d = %lx\n", ((rswitch[2]>>13) & 0x3) | (((rswitch[2]>>16) & 0x3)) <<2);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "localselect_s = %lx\n", ((rswitch[1]>>27) & 0x1) | (((rswitch[2]>>1)  & 0x1) <<1) | (((rswitch[2]>>4)  & 0x1) <<2) | (((rswitch[2]>>8)  & 0x1) <<3));
    }
    else
    {
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "forcescb      = %lx\n",  rswitch[0]>>0   & 0x3);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "input_source  = %lx\n", (rswitch[0]>>8)  & 0xff);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "low_dest      = %lx\n", (rswitch[0]>>16) & 0xff);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "upp_dest      = %lx\n", (rswitch[0]>>24) & 0xff);

        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "dvbcsa_var    = %x\n", (rswitch[4]>>8)  & 0x1f);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "permutation   = %x\n", (rswitch[4]>>16) & 0x7);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "modifyxrc     = %x\n", (rswitch[4]>>15) & 0x1);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "low_switch    = %x\n", (rswitch[4]>>0)  & 0xf);
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "upp_switch    = %x\n", (rswitch[4]>>4)  & 0xf);

        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_decrypt   = %x\n", (rswitch[1]>>0)  & 0x1 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_main_algo = %x\n", (rswitch[1]>>24) & 0xf );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_sub_algo  = %x\n", (rswitch[1]>>20) & 0xf );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_res_algo  = %x\n", (rswitch[1]>>16) & 0x7 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "esa_sb_algo   = %x\n", (rswitch[1]>>12) & 0x7 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsas_decrypt  = %x\n", (rswitch[2]>>0)  & 0x1 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsas_main_algo= %x\n", (rswitch[2]>>24) & 0xf );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsas_sub_algo = %x\n", (rswitch[2]>>20) & 0xf );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsas_res_algo = %x\n", (rswitch[2]>>16) & 0x7 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsas_sb_algo  = %x\n", (rswitch[2]>>12) & 0x7 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsad_decrypt  = %x\n", (rswitch[3]>>0)  & 0x1 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsad_main_algo= %x\n", (rswitch[3]>>24) & 0xf );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsad_sub_algo = %x\n", (rswitch[3]>>20) & 0xf );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsad_res_algo = %x\n", (rswitch[3]>>16) & 0x7 );
        HALDSCMB2_DBG(DSCMB2_DBGLV_WARNING, "lsad_sb_algo  = %x\n\n",(rswitch[3]>>12) & 0x7 );
    }
}

void HAL_DSCMB_SetDBGLevel(MS_U32 u32Level)
{
    _u32HalDSCMB2DbgLv = u32Level;
    HALDSCMB2_DBG(DSCMB2_DBGLV_INFO, "%s level: %lx\n", __FUNCTION__, u32Level);
    return;
}

void HAL_DSCMB_PidFlt_Dump(void)
{
    MS_U32 i = 0;
    MS_U32 pPidFlt;
    MS_U32 u32Value;
    for(i = 0 ; i < 256 ; i++)
    {
        pPidFlt = REG_PIDFLT0(i);
        u32Value = TSP32_IdrR(pPidFlt);
        HALDSCMB2_DBG("[Set0]PidFlt_addr=0x%08lX, %ld, value = 0x%08lX\n",pPidFlt, i, u32Value);
    }

    for(i = 0 ; i < 256 ; i++)
    {
        pPidFlt = REG_PIDFLT1(i);
        u32Value = TSP32_IdrR(pPidFlt);
        HALDSCMB2_DBG("[Set1]PidFlt_addr=0x%08lX, %ld, value = 0x%08lX\n",pPidFlt, i, u32Value);
    }
}

