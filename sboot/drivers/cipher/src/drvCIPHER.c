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
////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvCIPHER.c
// @brief  CIPHER Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "drvCIPHER.h"
#include "halRSA.h"
#include "halCIPHER.h"

#undef CIPHER_UTOPIA_20

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define CIPHERIntEnable()  {}
#define CIPHERIntDisable() {}
#define CIPHERIntAttach()  {}
#define CIPHERIntDetach()  {}
#define MsOS_ObtainMutex(a,b)  {}
#define MsOS_CreateMutex(a,b,c) 0
#define MsOS_ObtainSemaphore(a, b) {}
#define MsOS_CreateSemaphore(a,b,c) 0
#define MsOS_ReleaseMutex(a) {}
#define MsOS_ReleaseSemaphore(a) {}
#define MsOS_FlushMemory() {}
#define MsOS_DelayTaskUs(a) {}
#define MsOS_DelayTask(a) {}
#define MsOS_ReadMemory() {}
#define MS_MODULE_HW                0x0000

#define AESDMAPA2KSEG1(addr)                    ((void *)(((MS_U32)addr) | 0xa0000000UL)) //physical -> unchched


#define RSAInitChk()                               if(_bRSA_Init == FALSE){ return DRVAESDMA_FAIL; }
//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#define CIPHER_TASK_STACK_SIZE   (4096UL)
#define CIPHER_CAVID             (0x0FUL)
#define CIPHER_KEYLEN_MAX        (16UL)
#define HASH_HMAC_KEYPAD_SIZE    (64UL)
#define HASH_RPT_SIZE_ALL        (10UL)
#define HASH_RPT_SIZE_STATUS     (2UL)

#define HASH_MD5_DIGEST_SIZE     (16UL)
#define HASH_SHA1_DIGEST_SIZE    (20UL)
#define HASH_SHA256_DIGEST_SIZE  (32UL)
#define HASH_BLOCK_SIZE          (64UL)

#define DRV_CIPHER_TA_SHA_CHECK_SIZE    (8UL)
#define DRV_CIPHER_TA_LUT3_BITMAP_LUT   (0x1UL)
#define DRV_CIPHER_TA_LUT3_BITMAP_M     (0x2UL)
#define DRV_CIPHER_TA_LUT3_BITMAP_BC    (0x4UL)
#define DRV_CIPHER_TA_KCV_SIZE          (16UL)   //HW limitation, need padding to 16 bytes
#define DRV_CIPHER_TA_LUT_SIZE          (256UL)
#define DRV_CIPHER_TA_M_SIZE            (128UL)
#define DRV_CIPHER_TA_BC_SIZE           (16UL)

#define DRV_CIPHER_TA_SHA_FAIL          (0x00000010UL)
#define DRV_CIPHER_TA_KCV_FAIL          (0x00000020UL)
#define DRV_CIPHER_TA_LUT_FAIL          (0x00000030UL)
#define DRV_CIPHER_TA_M_FAIL            (0x00000040UL)
#define DRV_CIPHER_TA_BC_FAIL           (0x00000050UL)

#define DRV_CIPHER_NO_REPORT_BUFFER     (0x00000003UL)

//Interrupt
#define DRV_CIPHER_DMA_DATA_INIT        (0x00000000UL)
#define DRV_CIPHER_DONE                 (0x00000100UL)
#define DRV_CIPHER_CALLBACK             (0x0000000FUL)
#define DRV_CIPHER_CLOSE                (0x80000000UL)

//Report status
#define DRV_CIPHER_REPORT_OK            (0x80000000UL)

typedef struct
{
    MS_U32 u32LUT;  //LUT3 fill in 512 bytes table, each has 9 bits to describe
    MS_U32 u32M;
    MS_U32 u32BC;
}DRV_CIPHER_TA_LUT3;


//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
static MS_BOOL _bCIPHERDrv_Inited = FALSE;
static MS_BOOL _bCIPHERDrv_KeyCtrlEn = FALSE;

#define DRV_CIPHER_ASSERT( _bool_, _msg_, ret) if(!(_bool_)){return (ret);}
#define DRV_CIPHER_INIT_CHECK(ret) DRV_CIPHER_ASSERT(TRUE == _bCIPHERDrv_Inited, "drvCIPHER is not initialized\n", ret)
#define DRV_CIPHER_KEYCTRL_CHECK(ret) DRV_CIPHER_ASSERT(TRUE == _bCIPHERDrv_KeyCtrlEn, "drvCIPHER key control mode is not enable\n", ret)

static MS_S32  _CipherMtx = 0;
static MS_S32  _CipherRSAMtx;
static MS_S32  _CipherSemaphore;

static MS_S32  _AESDMAMtx = 0;

MS_BOOL bDES_Key = FALSE;


static CIPHER_DBGMSG_LEVEL _CIPHERDBGLevel = CIPHER_DBG_ERR;

#define CIPHER_REPORT_BUFFER_MAX 2UL

typedef struct
{
    MS_U32  u32Report;
    MS_U32  u32DoneBytes;
    MS_U32  u32ExcMsg;
    MS_U8*  pu8Digest_Buf;
    MS_U32  u32Digest_Buf_Size;
    P_DrvCIPHER_EvtCallback pfCallback;
    CIPHER_KEY_SRC  eKeySrc;
    MS_U8   u8KeyIdx;
    MS_BOOL bUsed;
}CIPHER_REPORT;

typedef struct
{
    DRV_CIPHER_DMACFG stConfig;
    MS_U8           u8Key[CIPHER_KEYLEN_MAX];
    MS_U8           u8IV[CIPHER_KEYLEN_MAX];
    MS_BOOL         bUsed;
}CIPHER_RESOURCE;

static CIPHER_REPORT _CIPHER_ReportBuffer[CIPHER_REPORT_BUFFER_MAX];
static CIPHER_RESOURCE _CIPHER_Resource[HAL_CRYPTODMA_DMA_KEY_SLOT];

static MS_U32 _u32CallbackID = 1;

static MS_U16 _u16ThreadID = 1;
//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

static MS_BOOL                        _bRSA_Init = FALSE;

//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MMIO_GetBASE(MS_VIRT *ptrBaseaddr, MS_PHY *u32Basesize, MS_U32 u32Module)
{
#define RIU_BASE    0x1f200000
    *ptrBaseaddr = RIU_BASE;
    return TRUE;
}

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsDMADone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
    MS_U32  u32Ret     = DRV_CIPHER_OK;
    MS_U32  u32Rpt[2]  = {0};
    MS_U8   u8BufNum   = 0;
    MS_BOOL bDone      = FALSE;

    DRV_CIPHER_INIT_CHECK(FALSE);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    u8BufNum = u32CmdId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    //Check register report
    if(FALSE == HAL_CIPHER_DMA_CmdDone(u32CmdId, &u32Ret))
    {
        bDone = FALSE;
        goto ISDMADONE_FINISH;
    }

    HAL_CIPHER_DMA_GetRpt(u32Rpt);

    if((u32Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
		if(u32Ret != 0)
        {
            *pu32Exception = HAL_CIPHER_ReadException(u32Ret);
            HAL_CIPHER_ResetException();
        }
    }

    bDone = TRUE;

    _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

ISDMADONE_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(bDone)
    {
        MsOS_ReadMemory();
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return bDone;
}

DLL_PUBLIC MS_BOOL MDrv_CIPHER_IsHASHDone(MS_U32 u32CmdId, MS_U32 *pu32Exception)
{
    MS_U32  u32Ret         = DRV_CIPHER_OK;
    MS_U32  u32Rpt[10]     = {0};
    MS_U8   u8BufNum       = 0;
    MS_BOOL bDone          = FALSE;

    MS_U8*  pu8Digest_Buf = NULL;
    MS_U32  u32Digest_Buf_Size = 0;
    CIPHER_KEY_SRC  eKeySrc = 0;
    MS_U8   u8KeyIdx        = 0;

    DRV_CIPHER_INIT_CHECK(FALSE);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    u8BufNum = u32CmdId % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    if(FALSE == HAL_CIPHER_Hash_CmdDone(u32CmdId, &u32Ret))
    {
        bDone = FALSE;
        goto ISHASHDONE_FINISH;
    }

    //Get all register report, include status and digest
    HAL_CIPHER_Hash_GetRpt(u32Rpt, HASH_RPT_SIZE_ALL);

    pu8Digest_Buf      = _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf;
    u32Digest_Buf_Size = _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size;
    eKeySrc            = _CIPHER_ReportBuffer[u8BufNum].eKeySrc;
    u8KeyIdx           = _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx;

    //Avoid status report, only copy digest to output address
    if((pu8Digest_Buf != NULL) && (u32Digest_Buf_Size > 0))
    {
        memcpy((MS_U8 *)pu8Digest_Buf, u32Rpt+HASH_RPT_SIZE_STATUS, u32Digest_Buf_Size);
    }

    //Reset exception flag for polling mode
    if(u32Ret != 0)
    {
        HAL_CIPHER_Hash_ExceptFilter(&u32Ret, eKeySrc, u8KeyIdx);
        *pu32Exception = HAL_CIPHER_ReadException(u32Ret);
        HAL_CIPHER_ResetException();
    }

    if((u32Rpt[0] & DRV_CIPHER_REPORT_OK) == 0)
    {
    }

    bDone = TRUE;

    _CIPHER_ReportBuffer[u8BufNum].u32Report          = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes       = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32ExcMsg          = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc            = 0;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx           = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed              = FALSE;

ISHASHDONE_FINISH:

    MsOS_ReleaseMutex(_CipherMtx);

    if(bDone)
    {
        MsOS_FlushMemory();
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return bDone;
}


//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Init(void)
{
    MS_VIRT u32Bank;
    MS_PHY  u32BankSize;
    MS_U32 i = 0;

    _bCIPHERDrv_Inited = FALSE;
    if(_bCIPHERDrv_Inited == TRUE)
    {
        return DRV_CIPHER_OK;
    }

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        return DRV_CIPHER_FAIL;
    }

    HAL_CIPHER_SetBank(u32Bank);
    HAL_CIPHER_ResetStatus(TRUE, TRUE);

    HAL_CIPHER_SWReset();

    _u16ThreadID   = 1;
    _u32CallbackID = 1;

    for(i=0;i<CIPHER_REPORT_BUFFER_MAX;i++)
    {
        _CIPHER_ReportBuffer[i].u32Report          = 0;
        _CIPHER_ReportBuffer[i].u32DoneBytes       = 0;
        _CIPHER_ReportBuffer[i].u32ExcMsg          = 0;
        _CIPHER_ReportBuffer[i].pu8Digest_Buf      = NULL;
        _CIPHER_ReportBuffer[i].u32Digest_Buf_Size = 0;
        _CIPHER_ReportBuffer[i].eKeySrc     = 0;
        _CIPHER_ReportBuffer[i].u8KeyIdx    = 0;
        _CIPHER_ReportBuffer[i].bUsed              = FALSE;
    }

    _CipherMtx = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"CIPHER Mutex", MSOS_PROCESS_SHARED);
	_AESDMAMtx = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"AESDMA Mutex", MSOS_PROCESS_SHARED);
    _CipherSemaphore = MsOS_CreateSemaphore(1, E_MSOS_FIFO, (char *)"CIPHER Semaphore");

	_bCIPHERDrv_Inited = TRUE;

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_Reset(void)
{
    MS_U32 i = 0;
    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

	MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE, TRUE);
    HAL_CIPHER_SWReset();
    HAL_CIPHER_ResetException();

    _u16ThreadID   = 1;
    _u32CallbackID = 1;

    for(i=0;i<CIPHER_REPORT_BUFFER_MAX;i++)
    {
        _CIPHER_ReportBuffer[i].u32Report          = 0;
        _CIPHER_ReportBuffer[i].u32DoneBytes       = 0;
        _CIPHER_ReportBuffer[i].u32ExcMsg          = 0;
        _CIPHER_ReportBuffer[i].pu8Digest_Buf      = NULL;
        _CIPHER_ReportBuffer[i].u32Digest_Buf_Size = 0;
        _CIPHER_ReportBuffer[i].pfCallback         = NULL;
        _CIPHER_ReportBuffer[i].eKeySrc            = 0;
        _CIPHER_ReportBuffer[i].u8KeyIdx           = 0;
        _CIPHER_ReportBuffer[i].bUsed              = FALSE;
    }

    if(TRUE == _bCIPHERDrv_KeyCtrlEn)
    {
        for(i = 0;i < HAL_CRYPTODMA_DMA_KEY_SLOT;i++)
        {
            _memset(&_CIPHER_Resource[i], 0, sizeof(CIPHER_RESOURCE));
        }
    }

    MsOS_ReleaseMutex(_CipherMtx);

    return DRV_CIPHER_OK;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_ResetKey(MS_U32 u32CAVid, MS_U32 u32KeyIdx)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    DRV_CIPHER_ALGO algo = {0};
    MS_U32 u32Ret = 0;
    MS_U32 u32WaitCnt = 0;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);
    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE , FALSE );

    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto RESETKEY_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo(algo);

    if(FALSE == HAL_CIPHER_ResetKey(u32KeyIdx))
    {
       ret =  DRV_CIPHER_BAD_PARAM;
       goto RESETKEY_FINISH;
    }

    HAL_CIPHER_DMA_Set_InputSrcFrom(E_CIPHER_DIRECT, NULL, 0);
    if(FALSE == HAL_CIPHER_DMA_Set_OutputDstTo(E_CIPHER_DIRECT, NULL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto RESETKEY_FINISH;
    }
    if(FALSE == HAL_CIPHER_DMA_Set_FileinDesc ( 0, HAL_CRYPTODMA_DIRDATA_MAX))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto RESETKEY_FINISH;
    }
    HAL_CIPHER_DMA_Set_FileoutDesc( 0, (HAL_CRYPTODMA_DIRDATA_MAX - 1));

    if(FALSE == HAL_CIPHER_DMA_Start(FALSE, E_DMA_INT_NONE, TRUE, 0))
    {
        ret =  DRV_CIPHER_FAIL;
        goto RESETKEY_FINISH;
    }

    //Check register report
    while((FALSE == HAL_CIPHER_DMA_CmdDone(0, &u32Ret)) && (u32WaitCnt < HAL_CIPHER_RESETKEY_TIMEOUT_VALUE))
    {
        u32WaitCnt++;
    }

    if(u32WaitCnt >= HAL_CIPHER_RESETKEY_TIMEOUT_VALUE)
    {
        ret = DRV_CIPHER_FAIL;
        goto RESETKEY_FINISH;
    }

    if(u32Ret != 0)
    {
        //Ignore exception here, HW still clear session key
        HAL_CIPHER_ResetException();
    }

RESETKEY_FINISH:

    //Reset all
    HAL_CIPHER_ResetStatus(TRUE, FALSE);

    MsOS_ReleaseMutex(_CipherMtx);

    MsOS_ReleaseSemaphore(_CipherSemaphore);


    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_DMACalc(DRV_CIPHER_DMACFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U8 u8BufNum = 0;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    if(TRUE == _bCIPHERDrv_KeyCtrlEn)
    {
        if(HAL_CRYPTODMA_DMA_KEY_SLOT <= stCfg.stKey.u8KeyIdx)
        {
            return DRV_CIPHER_BAD_PARAM;
        }

        if(E_CIPHER_KSRC_KL == stCfg.stKey.eKeySrc && FALSE == _CIPHER_Resource[stCfg.stKey.u8KeyIdx % HAL_CRYPTODMA_DMA_KEY_SLOT].bUsed)
        {
           return DRV_CIPHER_BAD_PARAM;
        }
    }

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.stOutput.u32Size == 0))
    {
        return DRV_CIPHER_BAD_PARAM;
    }

    //Check Key length
    if((stCfg.stKey.u8KeyLen > CIPHER_KEYLEN_MAX) || (stCfg.stKey.u8IVLen > CIPHER_KEYLEN_MAX))
    {
        return DRV_CIPHER_BAD_PARAM;
    }

    if(pu32CmdId == NULL)
    {
        return DRV_CIPHER_BAD_PARAM;
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    HAL_CIPHER_ResetStatus(TRUE , FALSE ) ;


    if(FALSE == HAL_CIPHER_DMA_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto DMA_FINISH;
    }

    HAL_CIPHER_DMA_Set_Algo( stCfg.stAlgo);

    if(FALSE == HAL_CIPHER_DMA_Set_Key(stCfg.stKey))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto DMA_FINISH;
    }

    HAL_CIPHER_DMA_Set_IV(stCfg.stKey.pu8IVData, stCfg.stKey.u8IVLen);


	// need to call DMA_Set_InputSrcFrom and DMA_Set_OutputDstTo
	// before call  DMA_Set_FileinDesc and DMA_Set_FileoutDesc
    HAL_CIPHER_DMA_Set_InputSrcFrom(stCfg.stInput.eMemoryType, NULL, 0);
    if(FALSE == HAL_CIPHER_DMA_Set_OutputDstTo(stCfg.stOutput.eMemoryType, NULL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto DMA_FINISH;
    }

    // Set File-in Addr //
    if(FALSE == HAL_CIPHER_DMA_Set_FileinDesc ( stCfg.stInput.u32Addr, stCfg.stInput.u32Size))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto DMA_FINISH;
    }
    HAL_CIPHER_DMA_Set_FileoutDesc( stCfg.stOutput.u32Addr, stCfg.stOutput.u32Addr + stCfg.stOutput.u32Size - 1);


    //Check report buffer
    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto DMA_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].bUsed        = TRUE;


    if(FALSE == HAL_CIPHER_DMA_Start(stCfg.bDecrypt, eIntMode , TRUE , _u16ThreadID))
    {
        ret =  DRV_CIPHER_FAIL;
        goto DMA_FINISH;
    }


    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

DMA_FINISH:

    //Reset all
    HAL_CIPHER_ResetStatus(TRUE, FALSE);

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;

}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASH(DRV_CIPHER_HASHCFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET ret  = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE eIntMode = E_DMA_INT_NONE;
    MS_U32 u32OutLen    = 32;
    MS_U32 u32HashSrc   = 0;
    MS_U32 u32HashDst   = 0;
    MS_U8  u8BufNum    = 0;


    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.u32Digest_Buf_Size == 0))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    if(stCfg.u32Digest_Buf_Size != u32OutLen)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    //Set CAVid

    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE );


    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS))
     {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHAUTO_FINISH;
    }


	//Set HOS = 1 , Output will be MSB First
	HAL_CIPHER_Hash_SetHOS(TRUE);

    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
        ret =  DRV_CIPHER_NO_REPORT_BUFFER;
        goto HASHAUTO_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;


    MsOS_FlushMemory();

    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHAUTO_FINISH;
    }

    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

HASHAUTO_FINISH:

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HASHManual(DRV_CIPHER_HASHCFG_MANUAL stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET      ret      = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE  eIntMode = E_DMA_INT_NONE;
    HAL_CIPHER_IWCTYPE  eIWCType = E_CIPHER_HASH_IWC_MANUAL; //Init Word Count
    MS_U32  u32IVSel    = CMD_HASH_IV_FIPS;
    MS_U8* pu8IV       = NULL;
    MS_U32  u32DataSize = 0;
    MS_U32  u32IVSize   = 0;
    MS_BOOL bAutoPad    = FALSE;
    MS_U32  u32OutLen   = 32;
    MS_U32  u32HashSrc  = 0;
    MS_U32  u32HashDst  = 0;
    MS_U8   u8BufNum    = 0;

    DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    u32DataSize = stCfg.stInput.u32Size;

    if((0 == u32DataSize) && (E_CIPHER_HASH_STAGE_LAST != stCfg.eStage))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    //Check buffer and size
    if((stCfg.pu8Digest_Buf != NULL) && (stCfg.u32Digest_Buf_Size != u32OutLen))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    //Check IV setting
    if(stCfg.pu8SetIV != NULL)
    {
        pu8IV = stCfg.pu8SetIV;
        u32IVSel = CMD_HASH_IV_CMD;

        if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)
        {
            u32IVSize = HASH_SHA1_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)
        {
            u32IVSize = HASH_SHA256_DIGEST_SIZE;
        }
        else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)
        {
            u32IVSize = HASH_MD5_DIGEST_SIZE;
        }
    }
    else
    {
        //NULL IV
        if(stCfg.eIVMode == E_CIPHER_HASH_IV_CMD)
        {
            ret = DRV_CIPHER_BAD_PARAM;
            goto HASHMANUAL_FINISH;
        }
    }

    //IV and IWC is from previous report
    if(stCfg.eIVMode == E_CIPHER_HASH_IV_PRV)
    {
        u32IVSel = CMD_HASH_IV_PRV_RPT;
        eIWCType = E_CIPHER_HASH_IWC_PRV;
    }

    //AutoPad only works on last stage, previous stage is 64 bytes alignment
    if(stCfg.eStage == E_CIPHER_HASH_STAGE_LAST)
    {
        bAutoPad = TRUE;
    }
    else
    {
        if((u32DataSize % (MS_U32)HASH_BLOCK_SIZE) != 0 )
        {
            ret = DRV_CIPHER_BAD_PARAM;
            goto HASHMANUAL_FINISH;
        }
    }

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    //Set CAVid

    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, bAutoPad, FALSE);


    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(pu8IV, u32IVSize , u32IVSel))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }


    //Set Init Word Count
    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(eIWCType, stCfg.u32StartByte))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }


    //Set HMAC Key
    if(FALSE == HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, stCfg.eHMACKPad, FALSE))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHMANUAL_FINISH;
    }
	//Set HOS = 1, Output will be MSB First
	//IV auto mode use previous report digest as IV, report must be LSB first
    if(stCfg.pu8Digest_Buf == NULL)
    {
        HAL_CIPHER_Hash_SetHOS(FALSE);
    }
    else
    {
        HAL_CIPHER_Hash_SetHOS(TRUE);
    }

    //Set total size if last command
    if(stCfg.eStage == E_CIPHER_HASH_STAGE_LAST)
    {
        HAL_CIPHER_Hash_SetMsgLength(stCfg.u32StartByte + stCfg.stInput.u32Size);
    }



    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    //Report buffer already used
    if(_CIPHER_ReportBuffer[u8BufNum].bUsed == TRUE)
    {
       ret =  DRV_CIPHER_NO_REPORT_BUFFER;
       goto HASHMANUAL_FINISH;
    }

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc  = stCfg.stHMACKey.eKeySrc;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx = stCfg.stHMACKey.u8KeyIdx;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;



    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHMANUAL_FINISH;
    }

    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

HASHMANUAL_FINISH:

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

DLL_PUBLIC DRV_CIPHER_RET MDrv_CIPHER_HMAC(DRV_CIPHER_HMACCFG stCfg, MS_U32 *pu32CmdId)
{
    DRV_CIPHER_RET      ret      = DRV_CIPHER_OK;
    HAL_CIPHER_INTMODE  eIntMode = E_DMA_INT_NONE;
    MS_U32 u32OutLen   = 0;
    MS_U32 u32HashSrc  = 0;
    MS_U32 u32HashDst  = 0;
    MS_U8  u8BufNum    = 0;


	DRV_CIPHER_INIT_CHECK(DRV_CIPHER_FAIL);

    if(TRUE == _bCIPHERDrv_KeyCtrlEn)
    {
        if(E_CIPHER_KSRC_KL == stCfg.stHMACKey.eKeySrc)
        {
           return DRV_CIPHER_FAIL;
        }
    }

    MsOS_FlushMemory();

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    //Check data size
    if((stCfg.stInput.u32Size == 0) || (stCfg.u32Digest_Buf_Size == 0))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(pu32CmdId == NULL)
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA1)          u32OutLen = HASH_SHA1_DIGEST_SIZE;    //160 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_SHA256)   u32OutLen = HASH_SHA256_DIGEST_SIZE;    //256 bits
    else if(stCfg.eAlgo == E_CIPHER_HASH_ALGO_MD5)      u32OutLen = HASH_MD5_DIGEST_SIZE;    //128 bits
    else u32OutLen = HASH_SHA256_DIGEST_SIZE;

    if(stCfg.u32Digest_Buf_Size != u32OutLen)
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }
    //------------Inner hash----------------------------
    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);
    //Set CAVid
    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }
    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE);


    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    //Set HMAC Key
    if(FALSE == HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, E_CIPHER_HMAC_IKPAD, FALSE))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    //Set Hash message source
    if(FALSE == HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }
    //Set input message address
    if(FALSE == HAL_CIPHER_Hash_SetMsg(stCfg.stInput.u32Addr, stCfg.stInput.u32Size, u32HashSrc))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }


    //Set output destination, HMAC inner hash output to register
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

	//Set HOS = 0, HW limitation
	HAL_CIPHER_Hash_SetHOS(FALSE);

    HAL_CIPHER_Hash_SetMsgLength(stCfg.stInput.u32Size + HASH_HMAC_KEYPAD_SIZE);

    u8BufNum = (MS_U8)(_u16ThreadID % (MS_U16)CIPHER_REPORT_BUFFER_MAX);

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = NULL;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = 0;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc  = stCfg.stHMACKey.eKeySrc;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx = stCfg.stHMACKey.u8KeyIdx;


    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

    MsOS_ReleaseMutex(_CipherMtx);

    //----------------Outer hash------------------------------

    //==============Obtain Semaphore==============
    MsOS_ObtainSemaphore(_CipherSemaphore, MSOS_WAIT_FOREVER);

    MsOS_ObtainMutex(_CipherMtx, MSOS_WAIT_FOREVER);

    eIntMode = E_DMA_INT_NONE;

    //Set CAVid
    if(FALSE == HAL_CIPHER_Hash_Set_CaVid(stCfg.u32CAVid))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    //Set Algo
    HAL_CIPHER_Hash_Set_Config(stCfg.eAlgo, TRUE, FALSE );

    //Set IV to FIPS
    if(FALSE == HAL_CIPHER_Hash_Set_IV(NULL, 0 , CMD_HASH_IV_FIPS))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    if(FALSE == HAL_CIPHER_Hash_Set_InitWordCnt(E_CIPHER_HASH_IWC_MANUAL, 0))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }


    //Set HMAC Key,
    if(FALSE == HAL_CIPHER_Hash_SetHMACKey(stCfg.stHMACKey, E_CIPHER_HMAC_OKPAD, FALSE))
    {
        ret =  DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    //No need to set Hash message source
    //HAL_CIPHER_Hash_Set_MsgSrcFrom(stCfg.stInput.eMemoryType, &u32HashSrc);

    //Set input message address, outer hash from previous report
    if(FALSE == HAL_CIPHER_Hash_SetMsg(NULL, 0,  CMD_HASH_SRC_PRV_RPT))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }

    //Set output destination
    if(FALSE == HAL_CIPHER_Hash_Set_OutputDstTo(E_CIPHER_DIRECT, &u32HashDst))
    {
        ret = DRV_CIPHER_BAD_PARAM;
        goto HASHHMAC_FINISH;
    }


	//Set HOS = 1
	HAL_CIPHER_Hash_SetHOS(TRUE);

    HAL_CIPHER_Hash_SetMsgLength(u32OutLen + HASH_HMAC_KEYPAD_SIZE); //Hash output size + O_keypad_size

    u8BufNum = _u16ThreadID % (MS_U32)CIPHER_REPORT_BUFFER_MAX;

    _CIPHER_ReportBuffer[u8BufNum].u32Report    = 0;
    _CIPHER_ReportBuffer[u8BufNum].u32DoneBytes = 0;
    _CIPHER_ReportBuffer[u8BufNum].pu8Digest_Buf      = stCfg.pu8Digest_Buf;
    _CIPHER_ReportBuffer[u8BufNum].u32Digest_Buf_Size = stCfg.u32Digest_Buf_Size;
    _CIPHER_ReportBuffer[u8BufNum].eKeySrc  = stCfg.stHMACKey.eKeySrc;
    _CIPHER_ReportBuffer[u8BufNum].u8KeyIdx = stCfg.stHMACKey.u8KeyIdx;
    _CIPHER_ReportBuffer[u8BufNum].bUsed = TRUE;


    //Start hash
    if(FALSE == HAL_CIPHER_Hash_Start(eIntMode , TRUE, FALSE, _u16ThreadID))
    {
        ret = DRV_CIPHER_FAIL;
        goto HASHHMAC_FINISH;
    }


    *pu32CmdId = (MS_U32)_u16ThreadID;

    //Check thread id max
    if(_u16ThreadID == HAL_CRYPTODMA_THREAD_ID_MAX)
    {
        _u16ThreadID = 1;
    }
    else
    {
        _u16ThreadID++;
    }

HASHHMAC_FINISH:

    //Reset all
    HAL_CIPHER_ResetStatus(FALSE , TRUE);

    MsOS_ReleaseMutex(_CipherMtx);

    if(ret != DRV_CIPHER_OK)
    {
        MsOS_ReleaseSemaphore(_CipherSemaphore);
    }

    return ret;
}

MS_BOOL _MDrv_RSA_Init(void)
{
    MS_VIRT u32Bank;
    MS_PHY  u32BankSize;
    _bRSA_Init = FALSE;
    if(_bRSA_Init == FALSE)
    {
        if(FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
        {
            return FALSE;
        }
        HAL_RSA_SetBank(u32Bank);

        _CipherRSAMtx = MsOS_CreateMutex( E_MSOS_FIFO, (char*)"CIPHER RSA mutex", MSOS_PROCESS_SHARED);
        _bRSA_Init = TRUE;
    }
    return TRUE;
}

DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U32 u32KeyLen = 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = NULL;

    if( pstSign==NULL || pstKey==NULL )
    {
        ret = DRVAESDMA_INVALID_PARAM;
        goto RSA_CAL_FINISH;
    }
    pu32Sig = (MS_U32 *)(pstSign);

    if( _MDrv_RSA_Init()==FALSE )
    {
        ret = DRVAESDMA_FAIL;
        goto RSA_CAL_FINISH;
    }

    MsOS_ObtainMutex( _CipherRSAMtx, MSOS_WAIT_FOREVER);
    HAL_RSA_Reset();
    HAL_RSA_Ind32Ctrl(RAM_WRITE);

    switch (eMode)
    {
        case E_DRVAESDMA_RSA1024_PUBLIC:
        {
            u32KeyLen = 128;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA2048_PUBLIC:
        {
            u32KeyLen = 256;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA256_PUBLIC:
        {
            u32KeyLen = 32;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_CAL_FINISH;
        }
    }

    HAL_RSA_LoadSram((pstKey->u32KeyE), E_RSA_ADDRESS_E);
    HAL_RSA_LoadSram((pstKey->u32KeyN), E_RSA_ADDRESS_N);
    HAL_RSA_LoadSram(pu32Sig, E_RSA_ADDRESS_A);
    HAL_RSA_SetKeyLength((u32KeyLen/RSA_UNIT_SIZE)-1);
    HAL_RSA_SetKeyType(E_RSA_USE_SW_KEY, u8Public); //sw key, public key
    HAL_RSA_ExponetialStart();
RSA_CAL_FINISH:
    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
    return _MDrv_RSA_Calculate(pstSign, pstKey, eMode);
}

DRVAESDMA_RESULT MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U8  u8KeySrc= 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = NULL;

    if( pstSign==NULL )
    {
        ret = DRVAESDMA_INVALID_PARAM;
        goto RSA_CAL_HW_FINISH;
    }
    pu32Sig = (MS_U32 *)(pstSign);

    if( _MDrv_RSA_Init()==FALSE )
    {
        ret = DRVAESDMA_FAIL;
        goto RSA_CAL_HW_FINISH;
    }

    MsOS_ObtainMutex( _CipherRSAMtx, MSOS_WAIT_FOREVER);

    HAL_RSA_Reset();
    HAL_RSA_Ind32Ctrl(RAM_WRITE);

    switch (eMode)
    {
        case E_DRVAESDMA_RSA_HWKEY_PRIVATE:
        {
            u8KeySrc = E_RSA_HW_PRI_KEY;
            u8Public = E_RSA_USE_PRIV_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_PUBLIC1:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY1;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        case E_DRVAESDMA_RSA_HWKEY_PUBLIC2:
        {
            u8KeySrc = E_RSA_HW_PUB_KEY2;
            u8Public = E_RSA_USE_PUB_KEY;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_CAL_HW_FINISH;
        }
    }

    HAL_RSA_LoadSram(pu32Sig, E_RSA_ADDRESS_A);

    HAL_RSA_SetKeyType(E_RSA_USE_HW_KEY, u8Public);
    HAL_RSA_Rootkey_Sel(u8KeySrc);
    HAL_RSA_ExponetialStart();

RSA_CAL_HW_FINISH:
    return ret;
}

DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void)
{
    RSAInitChk();

    if(HAL_RSA_Get_RSA_IsFinished()==0)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_FAIL;
    }
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_IsFinished(void)
{
    return _MDrv_RSA_IsFinished();
}

DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
    MS_U32 ret = DRVAESDMA_OK;
    MS_U32 i = 0;
    MS_U32 u32KeyLen = 0;
    MS_U32 *pu32RSAOut = (MS_U32 *)(pstRSAOut->u32RSAOut);

    RSAInitChk();

    switch (eMode)
    {
        case E_DRVAESDMA_RSA1024_PUBLIC:
        case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            break;
        }
        case E_DRVAESDMA_RSA2048_PUBLIC:
        case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            break;
        }
        case E_DRVAESDMA_RSA256_PUBLIC:
        case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            break;
        }
        default:
        {
            ret = DRVAESDMA_INVALID_PARAM;
            goto RSA_OUTPUT_FINISH;
        }
    }

    HAL_RSA_SetFileOutAddr(64-(u32KeyLen/sizeof(MS_U32)));
    HAL_RSA_Ind32Ctrl(RAM_READ);
    HAL_RSA_FileOutStart();
	for( i = 0; i<(u32KeyLen/RSA_UNIT_SIZE); i++)
	{
		pu32RSAOut[(u32KeyLen/RSA_UNIT_SIZE)-1-i] = HAL_RSA_FileOut();
	}

RSA_OUTPUT_FINISH:
    MsOS_ReleaseMutex(_CipherRSAMtx);
    return ret;
}

DLL_PUBLIC DRVAESDMA_RESULT MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
    return _MDrv_RSA_Output(eMode, pstRSAOut);
}
