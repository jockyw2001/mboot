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
/// file   drvDSCMB.c
/// @brief  Descrambler (DSCMB) Driver Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "drvDSCMB.h"
#include "regDSCMB.h"
#include "halDSCMB.h"

#define MsOS_ObtainMutex(a,b)  {}
#define MsOS_CreateMutex(a,b,c) 0
#define MsOS_ReleaseMutex(a) {}
#define MS_MODULE_HW                0x0000

//=========================================================================================
//  Data Structure Declaration
//=========================================================================================
#define DSCMB_LOCK(_a_)      MsOS_ObtainMutex ( (_a_) , MSOS_WAIT_FOREVER )
#define DSCMB_UNLOCK(_a_)    MsOS_ReleaseMutex( (_a_) )

static MS_BOOL _bDSCMB2Drv_Inited = FALSE;
static MS_BOOL _bKLDrv_Inited = FALSE;

#define DSCMB_ASSERT(x, p, ret)  if (!(x)){return ret;}
#define DSCMB_INIT_CHECK(ret)    DSCMB_ASSERT(_bDSCMB2Drv_Inited == TRUE, "The drvDSCMB2 is not initialized\n", ret)
#define KL_INIT_CHECK(ret)    DSCMB_ASSERT(_bKLDrv_Inited == TRUE, "The Key Ladder is not initialized\n", ret)
#define OBSOLETE_FUNCTION(x, args...)



// TODO: Fix me, need to put to resource private?
static MS_S32 _s32DscMutexId = -1 ;
static MS_S32 _s32KLMutexId = -1 ;

// TODO: Does it nessarary?
MS_U8  _u8KLPreLevel = 0;



#define SLOTUSED 1
#define SLOTEMPTY 0

extern void * memcpy ( void * destination, const void * source, size_t num );

//--------------------------------------------------------------------------------------------------
static void * _memset (void *s, int c, unsigned int n)
{
    unsigned char *t = s;

    while(n-- > 0)
        *t++ = c;

    return s;
}

MS_BOOL _MDrv_DSCMB2_KLadder_Init(void)
{
    MS_VIRT u32Bank;
    MS_PHY u32BankSize;

    _bKLDrv_Inited = FALSE;
    if(_bKLDrv_Inited == TRUE)
    {
        return TRUE;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        return FALSE;
    }

    HAL_DSCMB_SetBank(u32Bank);

    _s32KLMutexId  = MsOS_CreateMutex(E_MSOS_FIFO,"KeyLadder_Mutex", MSOS_PROCESS_SHARED);
    _bKLDrv_Inited = TRUE;
    return TRUE;
}


//------------------------------------- ------------------//
//----------------------Keyladder API -----------------------//
//-------------------------------------------------------//
//--------------------------------------------------------------------------------------------------
/// An atomic Key ladder function for Key generation, including common KL use cases.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status )
{
    DSCMB_ASSERT(KLCfg != NULL, "KLCfg input error", FALSE);
    DSCMB_ASSERT(u32Status != NULL, "u32Status input error", FALSE);

    MS_U32 i = 0;
    MS_U32 u32ErrMsg = 0;
    //  MS_BOOL bKeepRound=FALSE;
    MS_U32 u32FltId = 0;
    MS_U32 u32Field = 0;
    MS_U32 u32CAVid = KLCfg->u32CAVid;
    MS_U8   u8TempBuffer[16];

    DSCMB_KL_SelEng eKLEng = KLCfg->eKLSel;
    DSCMB_KLDst eKLDst = KLCfg->eDst & (~E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_KLSrc eKLSrc = KLCfg->eSrc & (~E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnableKDF = ((KLCfg->eSrc & E_DSCMB_KL_SRC_KDF_ENABLE) == E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnablePidslotmap = ((KLCfg->eDst & E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE) == E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_Key_Type eKeyType = KLCfg->eKeyType & (~E_DSCMB_KEY_FSCB_ENABLE);
    MS_BOOL bEnableKeyFscb = ((KLCfg->eKeyType & E_DSCMB_KEY_FSCB_ENABLE) == E_DSCMB_KEY_FSCB_ENABLE);

    if(_MDrv_DSCMB2_KLadder_Init () == FALSE)
    {
        return FALSE;
    }

    MsOS_ObtainMutex(_s32KLMutexId, MSOS_WAIT_FOREVER);

    *u32Status = KL_STATUS_KEY_OK ;

    ///translate cavid to Default CAVID if user set input cavid to 0x0
    if(u32CAVid == 0)
    {
        u32CAVid = DEFAULT_CAVID;
    }

    /////////////// Check Source and acpu data ////////////////////
    if ((E_DSCMB_KL_SRC_ACPU == eKLSrc) && (NULL == KLCfg->u8KeyACPU))
    {
        *u32Status = KL_STATUS_ACPU_KEY_NULL;
        goto KL_Fail;
    }

    /////////////// Check KL level ////////////////////
    if((KLCfg->u32Level >5) && (KLCfg->u32Level != 9) && (KLCfg->u32Level != 0xD))
    {
        //bKeepRound = TRUE ;
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }
    else if(( 0 == KLCfg->u32Level ) && (0==(DSCMB_KL_DST_CLASS_DMA & eKLDst)))
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    ////////////// check Key destination ///////////////
    /// Compatible and translate old version to new version
    if( ((eKLDst & 0xF0) == 0) && ( (eKLDst & 0x0F) > 0 )) //OLD Destination
    {
        switch(eKLDst)
        {
            case E_DSCMB_KL_DST_ACPU:
                eKLDst = E_DSCMB_KL_DST_CPU_ACPU;
                break;
            case E_DSCMB_KL_DST_KT_NSA:
                eKLDst = E_DSCMB_KL_DST_KT_ESA;
                break;
            case E_DSCMB_KL_DST_DMA_AES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_DMA_TDES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_PRIVATE0:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_0;
                break;
            case E_DSCMB_KL_DST_PRIVATE1:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_1;
                break;
            case E_DSCMB_KL_DST_PRIVATE2:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_2;
                break;
            default:
                ;
        }
    }

    // If destination is key table, and use dscmbId to get pidslotmap entry,
    // need to check descrambler driver init
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        if(bEnablePidslotmap == FALSE)
        {
            DSCMB_INIT_CHECK(FALSE);
        }
    }

    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        //KL to ACPU
        if(NULL == ACPU_Out)
        {
            *u32Status = KL_STATUS_ACPU_OUTKEY_NULL;
            goto KL_Fail;
        }
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        //KL to KT
        if(bEnablePidslotmap)
        {
            u32FltId = KLCfg->u32DscID;
        }
        else
        {
            *u32Status = KL_STATUS_IVALID_DSCMB_ID;
            goto KL_Fail;
        }
        // Get Field

    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        //KL to DMA
    }
    else if(DSCMB_KL_DST_CLASS_PVT &eKLDst)
    {
        //KL to PVT
    }
    else
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    //Check KeyLadder Input
    if ((KLCfg->u32Level != 0) && (NULL == KLCfg->pu8KeyKLIn))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    // TODO: Here need to check key property if support KDF or not

    if((E_DSCMB_KL_TYPE_INF == _u8KLPreLevel) || ((E_DSCMB_KL_TYPE_INF == _u8KLPreLevel) && (E_DSCMB_KL_TYPE_LV1 == KLCfg->u32Level)))
    {
        _u8KLPreLevel = KLCfg->u32Level;
    }
    else
    {
        _u8KLPreLevel = KLCfg->u32Level;
        HAL_DSCMB_KL_Reset(eKLEng);
        HAL_DSCMB_KL_KDF_Disable(eKLEng);
    }

    if(E_DSCMB_KL_SRC_ACPU == eKLSrc)
    {
        HAL_DSCMB_KL_ACPURootKey(eKLEng, KLCfg->u8KeyACPU);
    }

    //FIXME: If Key from OTP, should we set usage & entropy ?
    HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 0 /*Entropy*/);

    //Enable KDF
    if(bEnableKDF)
    {
        HAL_DSCMB_KL_KDF_Enable( eKLEng, KLCfg->stKDF.eKDFType, KLCfg->stKDF.u16AppId, KLCfg->stKDF.eHWKeyId);
    }

    // Set KL Level input //
    if(E_DSCMB_KL_TYPE_INF == KLCfg->u32Level) //Keep round, 1 stage
    {
        HAL_DSCMB_KL_Input(eKLEng, 0, KLCfg->pu8KeyKLIn);
    }
    else if(E_DSCMB_KL_TYPE_S5 == KLCfg->u32Level) //SMI Keyladder
    {
        for( i = 0 ; i < 5 ; i ++ )
        {
            HAL_DSCMB_KL_Input(eKLEng, (4 - i), KLCfg->pu8KeyKLIn+(16*i));
        }
    }
    else if(KLCfg->u32Level <= 5) //0~5 level keyladder
    {
        for( i = 0 ; i < KLCfg->u32Level ; i ++ )
        {
            HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
        }
    }
    else
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    // Set KL Ctrl
    if(FALSE == HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo, KLCfg->u32Level, eKLSrc, eKLDst))
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;

    }

    if (DSCMB_KL_DST_CLASS_CPU & eKLDst)
    {
        // KL flow for ACPU output //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0);               /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        if(bEnableKeyFscb){
            // KL flow for output  to KTE (with KeyFSCB) //
            HAL_DSCMB_KL_KeyBus_Ex(eKLEng,
                                  u32FltId,               /*For Dst KT*/
                                  u32Field,               /*For Dst KT*/
                                  eKeyType,               /*For Dst KT*/
                                  eKLDst,                 /*For Dst KT*/
                                  KLCfg->eFSCB);          /*For Dst KT*/
        }
        else{
        // KL flow for output  to KTE //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst KT*/
                            u32Field,               /*For Dst KT*/
                            eKeyType,               /*For Dst KT*/
                            eKLDst);                /*For Dst KT*/
        }
    }
    else if (DSCMB_KL_DST_CLASS_PVT & eKLDst)
    {
        // KL flow for output  to PVT //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0);              /*For Dst DMA*/
    }

    if(HAL_DSCMB_KL_Start(eKLEng) == FALSE)
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        goto KL_Fail;
    }

    if ((DSCMB_KL_DST_CLASS_SPSSPD & eKLDst) == DSCMB_KL_DST_CLASS_SPSSPD)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_SP_KeyCtrl(eKLEng, eKLDst);
        if(HAL_DSCMB_KL_SP_Start(eKLEng) == FALSE)
        {
            goto KL_Fail;
        }
    }

    // enable key
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        DSCMB_Eng_Type eEngType = E_DSCMB_ENG_ESA;
        if(eKLDst == E_DSCMB_KL_DST_KT_ESA )
        {
            eEngType = E_DSCMB_ENG_ESA;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAS )
        {
            eEngType = E_DSCMB_ENG_LSAS;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAD )
        {
            eEngType = E_DSCMB_ENG_LSAD;
        }

        if (FALSE == HAL_DSCMB_KTE_Key_Ctrl_Ex(u32FltId,
                                            eKeyType,
                                            eEngType,
                                            u32CAVid,
                                            TRUE,
                                            TRUE))
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //Output to ACPU
    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        MS_U32 u32OutKeySize = 0;
        _memset(u8TempBuffer , 0x0 , 16);
        HAL_DSCMB_KL_GetACPUOut(eKLEng, u8TempBuffer, 16);

        if(KLCfg->eOutsize == E_DSCMB_KL_64_BITS)
        {
            u32OutKeySize = 8;
        }
        else if(KLCfg->eOutsize == E_DSCMB_KL_128_BITS)
        {
            u32OutKeySize = 16;
        }
        memcpy(ACPU_Out , u8TempBuffer , u32OutKeySize );
    }

    MsOS_ReleaseMutex(_s32KLMutexId);
    return TRUE;

KL_Fail:

    MsOS_ReleaseMutex(_s32KLMutexId);
    DSCMB_ASSERT_AND_RETURN(FALSE);

    return FALSE;
}

//--------------------------------------------------------------------------------------------------
/// An ETSI Key ladder function for Key generation, including common KL use cases.
/// Thread safe for cuncurrent usage.
/// @param KLCfg\b IN: KL execution setting for target key generation
/// @param ACPU_Out\b IN: A 16 bytes array pointer for Acpu key output.
/// @param pu8Nonce\b IN: A 16 bytes array pointer for Nonce input
/// @param pu8Response\n OUT: A 16 byte array pointer for Response output
/// @param u32Status\b IN: Key ladder running status
/// @return TRUE - Success
/// @return FALSE - Failure
//--------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_DSCMB2_KLadder_ETSI(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, MS_U8 *pu8Nonce, MS_U8 *pu8Response, DSCMB_KL_Status* u32Status )
{
    DSCMB_INIT_CHECK(FALSE);
    DSCMB_ASSERT(KLCfg != NULL, "KLCfg input error", FALSE);
    DSCMB_ASSERT(u32Status != NULL, "u32Status input error", FALSE);
    DSCMB_ASSERT(pu8Nonce != NULL, "pu8Nonce input error", FALSE);

    MS_U32 i = 0;
    MS_U32 u32ErrMsg = 0;
    MS_U32 u32FltId = 0;
    MS_U32 u32Field = 0;
    MS_U32 u32CAVid = KLCfg->u32CAVid;
    MS_U8   u8TempBuffer[16];

    DSCMB_KL_SelEng eKLEng = KLCfg->eKLSel;
    DSCMB_KLDst eKLDst = KLCfg->eDst & (~E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_KLSrc eKLSrc = KLCfg->eSrc & (~E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnableKDF = ((KLCfg->eSrc & E_DSCMB_KL_SRC_KDF_ENABLE) == E_DSCMB_KL_SRC_KDF_ENABLE);
    MS_BOOL bEnablePidslotmap = ((KLCfg->eDst & E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE) == E_DSCMB_KL_DST_KT_PIDSLOTMAP_ENABLE);
    DSCMB_Key_Type eKeyType = KLCfg->eKeyType & (~E_DSCMB_KEY_FSCB_ENABLE);
    MS_BOOL bEnableKeyFscb = ((KLCfg->eKeyType & E_DSCMB_KEY_FSCB_ENABLE) == E_DSCMB_KEY_FSCB_ENABLE);

    if(_MDrv_DSCMB2_KLadder_Init () == FALSE)
    {
        return FALSE;
    }

    MsOS_ObtainMutex(_s32KLMutexId, MSOS_WAIT_FOREVER);

    *u32Status = KL_STATUS_KEY_OK ;

    ///translate cavid to Default CAVID if user set input cavid to 0x0
    if(u32CAVid == 0)
    {
        u32CAVid = DEFAULT_CAVID;
    }

    /////////////// Check Source and acpu data ////////////////////
    if ((E_DSCMB_KL_SRC_ACPU == eKLSrc) && (NULL == KLCfg->u8KeyACPU))
    {
        *u32Status = KL_STATUS_ACPU_KEY_NULL;
        goto KL_Fail;
    }

    /////////////// Check KL level ////////////////////
    if(KLCfg->u32Level != 3) //ETSI keyladder, must be 3 level!
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;
    }

    ////////////// check Key destination ///////////////
    /// Compatible and translate old version to new version
    if( ((eKLDst & 0xF0) == 0) && ( (eKLDst & 0x0F) > 0 )) //OLD Destination
    {
        switch(eKLDst)
        {
            case E_DSCMB_KL_DST_ACPU:
                eKLDst = E_DSCMB_KL_DST_CPU_ACPU;
                break;
            case E_DSCMB_KL_DST_KT_NSA:
                eKLDst = E_DSCMB_KL_DST_KT_ESA;
                break;
            case E_DSCMB_KL_DST_DMA_AES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_DMA_TDES:
                eKLDst = E_DSCMB_KL_DST_DMA_SK0;
                break;
            case E_DSCMB_KL_DST_PRIVATE0:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_0;
                break;
            case E_DSCMB_KL_DST_PRIVATE1:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_1;
                break;
            case E_DSCMB_KL_DST_PRIVATE2:
                eKLDst = E_DSCMB_KL_DST_PRIVATE_2;
                break;
            default:
                ;
        }
    }

    // If destination is key table, and use dscmbId to get pidslotmap entry,
    // need to check descrambler driver init
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        if(bEnablePidslotmap == FALSE)
        {
            DSCMB_INIT_CHECK(FALSE);
        }
    }

    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        //KL to ACPU
        if(NULL == ACPU_Out)
        {
            *u32Status = KL_STATUS_ACPU_OUTKEY_NULL;
            goto KL_Fail;
        }
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        //KL to KT
        if(bEnablePidslotmap)
        {
            u32FltId = KLCfg->u32DscID;
        }
        else
        {
            *u32Status = KL_STATUS_IVALID_DSCMB_ID;
            goto KL_Fail;
        }
        // Get Field

    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        //KL to DMA
    }
    else if(DSCMB_KL_DST_CLASS_PVT &eKLDst)
    {
        //KL to PVT
    }
    else
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    //Check KeyLadder Input
    if ((KLCfg->u32Level != 0) && (NULL == KLCfg->pu8KeyKLIn))
    {
        *u32Status = KL_STATUS_KL_INPUT_NULL;
        goto KL_Fail;
    }

    // TODO: Here need to check key property if support KDF or not

    HAL_DSCMB_KL_Reset(eKLEng);
    HAL_DSCMB_KL_KDF_Disable(eKLEng);


    if(E_DSCMB_KL_SRC_ACPU == eKLSrc)
    {
        HAL_DSCMB_KL_ACPURootKey(eKLEng, KLCfg->u8KeyACPU);
    }

    //FIXME: If Key from OTP, should we set usage & entropy ?
    HAL_DSCMB_KL_KeyProp( eKLEng, u32CAVid, 0 , /*Usage*/ 0 /*Entropy*/);

    //Enable KDF
    if(bEnableKDF)
    {
        HAL_DSCMB_KL_KDF_Enable( eKLEng, KLCfg->stKDF.eKDFType, KLCfg->stKDF.u16AppId, KLCfg->stKDF.eHWKeyId);
    }

    // Set KL Level input //
    for( i = 0 ; i < KLCfg->u32Level ; i ++ )
    {
        HAL_DSCMB_KL_Input(eKLEng, (KLCfg->u32Level-i-1), KLCfg->pu8KeyKLIn+(16*i));
    }


    //Set ETSI KL Nonce input
    HAL_DSCMB_KL_ETSI_Nonce(eKLEng, pu8Nonce);

    // Set KL Ctrl
    if(FALSE == HAL_DSCMB_KL_Ctrl(eKLEng, KLCfg->eAlgo, KLCfg->u32Level, eKLSrc, eKLDst))
    {
        *u32Status = KL_STATUS_INVALID_INPUT_LEVEL;
        goto KL_Fail;

    }

    if (DSCMB_KL_DST_CLASS_CPU & eKLDst)
    {
        // KL flow for ACPU output //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0);     /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_DMA & eKLDst)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            0,                /*For Dst KT*/
                            eKLDst);          /*For Dst DMA*/
    }
    else if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        if(bEnableKeyFscb){
            // KL flow for output  to KTE (with KeyFSCB) //
            HAL_DSCMB_KL_KeyBus_Ex(eKLEng,
                                  u32FltId,               /*For Dst KT*/
                                  u32Field,               /*For Dst KT*/
                                  eKeyType,               /*For Dst KT*/
                                  eKLDst,                 /*For Dst KT*/
                                  KLCfg->eFSCB);          /*For Dst KT*/
        }
        else{
        // KL flow for output  to KTE //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            u32FltId,               /*For Dst KT*/
                            u32Field,               /*For Dst KT*/
                            eKeyType,               /*For Dst KT*/
                            eKLDst);                 /*For Dst KT*/
        }
    }
    else if (DSCMB_KL_DST_CLASS_PVT & eKLDst)
    {
        // KL flow for output  to PVT //
        HAL_DSCMB_KL_KeyBus(eKLEng,
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0,               /*For Dst KT*/
                            0);              /*For Dst DMA*/
    }

    if(HAL_DSCMB_KL_Start(eKLEng) == FALSE)
    {
        HAL_DSCMB_KL_ErrMsg(eKLEng, &u32ErrMsg);
        goto KL_Fail;
    }

    if ((DSCMB_KL_DST_CLASS_SPSSPD & eKLDst) == DSCMB_KL_DST_CLASS_SPSSPD)
    {
        // KL flow for output  to CryptoDMA  //
        HAL_DSCMB_KL_SP_KeyCtrl(eKLEng, eKLDst);
        if(HAL_DSCMB_KL_SP_Start(eKLEng) == FALSE)
        {
            goto KL_Fail;
        }
    }

    // enable key
    if (DSCMB_KL_DST_CLASS_KT & eKLDst)
    {
        DSCMB_Eng_Type eEngType = E_DSCMB_ENG_ESA;
        if(eKLDst == E_DSCMB_KL_DST_KT_ESA )
        {
            eEngType = E_DSCMB_ENG_ESA;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAS )
        {
            eEngType = E_DSCMB_ENG_LSAS;
        }
        else if(eKLDst == E_DSCMB_KL_DST_KT_LSAD )
        {
            eEngType = E_DSCMB_ENG_LSAD;
        }

        if (FALSE == HAL_DSCMB_KTE_Key_Ctrl_Ex(u32FltId,
                                            eKeyType,
                                            eEngType,
                                            u32CAVid,
                                            TRUE,
                                            TRUE))
        {
            *u32Status = KL_STATUS_KL_INPUT_NULL;
            goto KL_Fail;
        }
    }

    //Output to ACPU
    if (E_DSCMB_KL_DST_CPU_ACPU == eKLDst)
    {
        MS_U32 u32OutKeySize = 0;
        _memset(u8TempBuffer , 0x0 , 16);
        HAL_DSCMB_KL_GetACPUOut(eKLEng,u8TempBuffer,16);

        if(KLCfg->eOutsize == E_DSCMB_KL_64_BITS)
        {
            u32OutKeySize = 8;
        }
        else if(KLCfg->eOutsize == E_DSCMB_KL_128_BITS)
        {
            u32OutKeySize = 16;
        }
        memcpy(ACPU_Out , u8TempBuffer , u32OutKeySize );
    }

    HAL_DSCMB_KL_ETSI_Response(eKLEng, pu8Response);

    MsOS_ReleaseMutex(_s32KLMutexId);
    return TRUE;

KL_Fail:

    MsOS_ReleaseMutex(_s32KLMutexId);
    DSCMB_ASSERT_AND_RETURN(FALSE);

    return FALSE;
}


//----------------------------------------------------------------------//
//                   New Interface for K3 Series                        //
//----------------------------------------------------------------------//
MS_BOOL MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status)
{
    return _MDrv_DSCMB2_KLadder_AtomicExec(KLCfg, ACPU_Out, u32Status);
}

MS_BOOL MDrv_DSCMB2_KLadder_ETSI(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, MS_U8 *pu8Nonce, MS_U8 *pu8Response, DSCMB_KL_Status* u32Status )
{
    return _MDrv_DSCMB2_KLadder_ETSI(KLCfg, ACPU_Out, pu8Nonce, pu8Response, u32Status);
}
