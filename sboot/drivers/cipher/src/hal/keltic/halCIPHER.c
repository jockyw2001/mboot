////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2013 MStar Semiconductor, Inc.
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
// file   halCIPHER.c
// @brief  CIPHER HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
//#include "MsTypes.h"
//#include "MsOS.h"
#include "regCIPHER.h"
#include "halCIPHER.h"
#include "drvCIPHER.h"

extern void * memset ( void * ptr, int value, size_t num );
extern void * memcpy ( void * destination, const void * source, size_t num );
extern int printf ( const char * format, ... );
//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define     DELAY_WAITING           1
#define     DELAY_RESCHEDULE        10
#define     COMQ_AVALIABLE          0x10
#define     CIPHER_KEY_SIZE         16
#define     CRYPTODMA_DEBUG_SIZE    19
#define     HASH_MANUAL_BLOCK_SIZE  64

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_U32              _u32RegBase         = 0;
static REG_AESDMACtrl      *_AESDMACtrl        = 0;
static CIPHER_DBGMSG_LEVEL _HAL_CIPHERDBGLevel = CIPHER_DBG_ERR;

// Algorithm Table for AES/TDES/DES
static MS_BOOL _CryptodMA_AlgoTable[4][E_DMA_MODE_NUM][E_DMA_RESIDUE_NUM][E_DMA_SB_NUM];


#define REG32_W(reg, value)     do {    \
                                    (reg)->H = ((value) >> 16); \
                                    (reg)->L = ((value) & 0x0000FFFF);  \
                                } while(0)

#define REG32(addr) (*(volatile unsigned long *) (addr))
#define REG16(add) *((volatile unsigned short *)(add))
#define CIPHER_NO_PRINTF

#ifdef CIPHER_NO_PRINTF
#define HAL_DEBUG_CIPHER(debug_level, x)
#else
#define HAL_DEBUG_CIPHER(debug_level, x)     do { if (_HAL_CIPHERDBGLevel >= (debug_level)) (x); } while(0)
#endif
//--------------------------------------------------------------------------------------------------
//   Local Variable
//--------------------------------------------------------------------------------------------------

MS_U32 _u32DmaInAddr , _u32DmaInNum ;
MS_U32 _u32DmaOut_StartAddr , _u32DmaOut_EndAddr ;
MS_U32 _u32ReportAddr;

MS_U8 _pu8CipherKey[CIPHER_KEY_SIZE];
MS_U8 _pu8IV[CIPHER_KEY_SIZE];
MS_U8 _pu8Data[CIPHER_KEY_SIZE];

MS_U32 _u32CmdLen = 0 ;

// [NOTE] Solve Strict-Aliasing //
union {
    CMD_DmaFormat0  DmaCtrl0;
    MS_U32 u32DmaCtrl0;
}_uDmaCtrl0;

union{
    CMD_DmaFormat1  DmaCtrl1;
    MS_U32 u32DmaCtrl1;
}_uDmaCtrl1;

union{
    Hash_DmaFormat0  HashCtrl0;
    MS_U32 u32HashCtrl0;
}_uHashCtrl0;

union{
    Hash_DmaFormat1  HashCtrl1;
    MS_U32 u32HashCtrl1;
}_uHashCtrl1;


MS_U32 _u32HashSrcAddr, _u32InitWordCnt, _u32HashDestAddr, _u32HashMsgLen ;

MS_U8  _pu8HashIV[HASH_CMD_IV_SIZE];
MS_U8  _pu8HashMsg[HASH_CMD_MSG_SIZE];

static MS_U32 _CIPHER_CAVid = 0x0F;

//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------
void * _memset(void * s,char c,int count)
{
    char *xs = (char *) s;

    while (count--)
    *xs++ = c;

    return s;
}

void WRITE_CMDQ(MS_U32 u32Value)
{
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s] = 0x%08X \n",__func__, (unsigned int)(u32Value)));
    REG32(&_AESDMACtrl->Cmd_Queue) = (u32Value) ;
}

MS_U32 _HAL_CIPHER_GetCmdLen(HAL_CIPHER_CMDTYPE CmdType)
{

    MS_U32 u32CmdLen = 0 ;


    if ( E_CIPHER_TYPE_DMA == CmdType )
    {
        u32CmdLen = 6;

        if ( _uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl0.DmaCtrl0.DIV == TRUE )
        {
            u32CmdLen += 4 ;
        }
        if ( _uDmaCtrl1.DmaCtrl1.SD == E_DMA_SRC_DIRECT )
        {
            u32CmdLen += 4 ;
        }
        if ( _uDmaCtrl0.DmaCtrl0.DK == TRUE )
        {
            u32CmdLen += 4 ;
        }

        return u32CmdLen ;
    }


    if ( E_CIPHER_TYPE_SHA == CmdType )
    {

        u32CmdLen = 3 ;

        if (_uHashCtrl0.HashCtrl0.Src != CMD_HASH_SRC_REG )
        {
            u32CmdLen += 1 ;
        }

        if (_uHashCtrl0.HashCtrl0.IWC_Sel == CMD_HASH_IWC_CMDQ )
        {
            u32CmdLen += 1 ;
        }

        if ((_uHashCtrl0.HashCtrl0.Dest == CMD_HASH_OUTPUT_DRAM ) || (_uHashCtrl0.HashCtrl0.Dest == CMD_HASH_OUTPUT_R2 ))
        {
            u32CmdLen += 1 ;
        }

        if (_uHashCtrl0.HashCtrl0.InitHashSel == CMD_HASH_IV_CMD )
        {
            u32CmdLen += 8 ;
        }

        if (_uHashCtrl0.HashCtrl0.Src == CMD_HASH_SRC_REG )
        {
            u32CmdLen += 16 ;
        }

        return u32CmdLen ;

    }


    return 0 ;

}

HAL_CIPHER_KEYSRC _HAL_CIPHER_TransKey(CIPHER_KEY_SRC src, MS_U32 idx)
{
    HAL_CIPHER_KEYSRC KeySrc = E_DMA_KSEL_REGKEY;

    if(src == E_CIPHER_KSRC_KL)
    {
        KeySrc = E_DMA_KSEL_SK0 + idx;
    }
    else if(src == E_CIPHER_KSRC_OTP)
    {
        KeySrc = E_DMA_KSEL_MK0 + idx;
    }
    else if(src == E_CIPHER_KSRC_CPU)
    {
        KeySrc = E_DMA_KSEL_REGKEY;
    }
    else if(src == E_CIPHER_KSRC_CAIP)
    {
        KeySrc = E_DMA_KSEL_CAIP;
    }

    return KeySrc;
}

HAL_CIPHER_ALGO _HAL_CIPHER_TransMainAlgo(CIPHER_MAIN_ALGO algo)
{
    HAL_CIPHER_ALGO hal_algo;
    switch(algo)
    {
    case E_CIPHER_MAIN_AES:
        hal_algo = E_DMA_ALGO_AES;
        break;
    case E_CIPHER_MAIN_DES:
        hal_algo = E_DMA_ALGO_DES;
        break;
    case E_CIPHER_MAIN_TDES:
        hal_algo = E_DMA_ALGO_TDES;
        break;
    case E_CIPHER_MAIN_M6_S56_CCBC:
        hal_algo = E_DMA_ALGO_M6_S56_CCBC;
        break;
    case E_CIPHER_MAIN_M6_S56:
        hal_algo = E_DMA_ALGO_M6_S56;
        break;
    case E_CIPHER_MAIN_M6_KE56:
        hal_algo = E_DMA_ALGO_M6_KE56;
        break;
    case E_CIPHER_MAIN_RC4:
        hal_algo = E_DMA_ALGO_RC4;
        break;
    default:
        hal_algo = E_DMA_ALGO_NONE;
    }

    return hal_algo;
}

HAL_CIPHER_MODE _HAL_CIPHER_TransSubAlgo(CIPHER_SUB_ALGO algo)
{
    HAL_CIPHER_MODE hal_algo;
    switch(algo)
    {
    case E_CIPHER_SUB_ECB:
        hal_algo = E_DMA_MODE_ECB;
        break;
    case E_CIPHER_SUB_CBC:
        hal_algo = E_DMA_MODE_CBC;
        break;
    case E_CIPHER_SUB_CTR:
        hal_algo = E_DMA_MODE_CTR;
        break;
    case E_CIPHER_SUB_CBC_MAC:
        hal_algo = E_DMA_MODE_CBC_MAC;
        break;
    default:
        hal_algo = E_DMA_MODE_NONE;
    }

    return hal_algo;

}

HAL_CIPHER_RESIDUE _HAL_CIPHER_TransResAlgo(CIPHER_RES_ALGO algo)
{
    HAL_CIPHER_RESIDUE hal_algo;
    switch(algo)
    {
    case E_CIPHER_RES_CLR:
        hal_algo = E_DMA_RESIDUE_CLR;
        break;
    case E_CIPHER_RES_CTS:
        hal_algo = E_DMA_RESIDUE_CTS;
        break;
    case E_CIPHER_RES_SCTE52:
        hal_algo = E_DMA_RESIDUE_SCTE52;
        break;
    default:
        hal_algo = E_DMA_RESIDUE_NONE;
    }

    return hal_algo;
}

HAL_CIPHER_SHORTBLOCK _HAL_CIPHER_TransSBAlgo(CIPHER_SB_ALGO algo)
{
    HAL_CIPHER_SHORTBLOCK hal_algo;
    switch(algo)
    {
    case E_CIPHER_SB_CLR:
        hal_algo = E_DMA_SB_CLR;
        break;
    case E_CIPHER_SB_IV1:
        hal_algo = E_DMA_SB_IV1;
        break;
    case E_CIPHER_SB_IV2:
        hal_algo = E_DMA_SB_IV2;
        break;
    default:
        hal_algo = E_DMA_SB_NONE;
    }

    return hal_algo;

}

HAL_CIPHER_HASHMODE _HAL_CIPHER_TransHASHAlgo(CIPHER_HASH_ALGO algo)
{
    HAL_CIPHER_HASHMODE hal_algo;
    switch(algo)
    {
    case E_CIPHER_HASH_ALGO_SHA1:
        hal_algo = E_HASH_SHA1;
        break;
    case E_CIPHER_HASH_ALGO_SHA256:
        hal_algo = E_HASH_SHA256;
        break;
    case E_CIPHER_HASH_ALGO_MD5:
        hal_algo = E_HASH_MD5;
        break;
    default:
        hal_algo = E_HASH_SHA256;
    }

    return hal_algo;
}

void HAL_CIPHER_DMA_AlgoTable_Init(void)
{

    memset(_CryptodMA_AlgoTable , 0x0 , sizeof(_CryptodMA_AlgoTable));

    // AES algorithms //
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC_MAC][E_DMA_RESIDUE_CLR][E_DMA_RESIDUE_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CTR][E_DMA_RESIDUE_NONE][E_DMA_SB_NONE] = TRUE ; // residue & sb don't care

    // DES Algorithms //
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV2] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CTR][E_DMA_RESIDUE_NONE][E_DMA_SB_NONE] = TRUE ; // residue & sb don't care

    // TDES Algorithm //
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CTR][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV2] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_CLR] = TRUE ;

}


MS_BOOL HAL_CIPHER_DMA_CheckAlgo(HAL_CIPHER_ALGO eAlgo, HAL_CIPHER_MODE eMode, HAL_CIPHER_RESIDUE eRes, HAL_CIPHER_SHORTBLOCK eSB)
{
    // Special Cases //
    if (eAlgo > E_DMA_ALGO_TDES )
    {
        if ((E_DMA_ALGO_RC4==eAlgo)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR == eSB)&&(E_DMA_MODE_NONE==eMode))
        {   //RC4/none/none/none
            return TRUE ;
        }
        if ((E_DMA_ALGO_M6_KE56==eAlgo)&&(E_DMA_MODE_ECB==eMode)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR==eSB))
        {   //M6_KE56/ecb/none/clr
            return TRUE;
        }
        if ((E_DMA_ALGO_M6_S56==eAlgo)&&(E_DMA_MODE_ECB==eMode)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR==eSB))
        {   //M6_S56/ecb/none/clr
            return TRUE;
        }
        if ((E_DMA_ALGO_M6_S56_CCBC==eAlgo)&&(E_DMA_MODE_NONE==eMode)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR==eSB))
        {   //M6_S56_CCBC/none/none/clr
            return TRUE;
        }
        return FALSE ;
    }

    // Reference table for Algorithm AES/TDES/DES //
    return _CryptodMA_AlgoTable[eAlgo][eMode][eRes][eSB] ;
}

void HAL_CIPHER_SetBank( MS_U32 u32BankAddr)
{
    MS_U8 u8Tmp = 0;
    _u32RegBase = 0xbf200000;
    _AESDMACtrl = (REG_AESDMACtrl*)(_u32RegBase + REG_CIPHERCTRL_BASE);

    //TSP software reset
    u8Tmp = *((volatile MS_U8*)((_u32RegBase + REG_TSPCTRL_BASE)+ ((0x7A)<< 2)));
    u8Tmp |= REG_TSP0_nRST;
    *((volatile MS_U8*)((_u32RegBase + REG_TSPCTRL_BASE)+ ((0x7A)<< 2))) = u8Tmp;
}

void HAL_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel)
{
    _HAL_CIPHERDBGLevel = eDBGMsgLevel;
}

void HAL_CIPHER_EnableSEPInterface(MS_BOOL bEnable)
{
    #if defined (MCU_AEON)
        if(bEnable) {
            _AESDMACtrl = (REG_AESDMACtrl*)(0x81000000);
        } else {
            _AESDMACtrl = (REG_AESDMACtrl*)(_u32RegBase + REG_CIPHERCTRL_BASE);
        }
    #else
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Not support, only for SEP\n", __func__, __LINE__));

    #endif
}

void HAL_CIPHER_ResetStatus(MS_BOOL RstDma , MS_BOOL RstHash)
{
    if (RstDma)
    {
        _uDmaCtrl0.u32DmaCtrl0 = 0 ;
        _uDmaCtrl1.u32DmaCtrl1 = 0 ;
        _u32DmaInAddr = 0;
        _u32DmaInNum  = 0 ;
        _u32DmaOut_StartAddr = 0;
        _u32DmaOut_EndAddr   = 0;
        _u32ReportAddr = 0 ;
        memset(&_pu8CipherKey ,   0x0, CIPHER_KEY_SIZE );
        memset(&_pu8IV        ,   0x0, CIPHER_KEY_SIZE );
        memset(&_pu8Data      ,   0x0, CIPHER_KEY_SIZE );
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Reset DMA value\n", __FUNCTION__, __LINE__));

    }

    if (RstHash)
    {
        (_uHashCtrl0.u32HashCtrl0) = 0 ;

        _u32HashSrcAddr  = 0 ;
        _u32InitWordCnt  = 0 ;
        _u32HashDestAddr = 0 ;
        _u32HashMsgLen   = 0 ;

        memset(_pu8HashIV, 0x0 ,HASH_CMD_IV_SIZE);
        memset(_pu8HashMsg, 0x0 , HASH_CMD_MSG_SIZE);
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Reset HASH value\n", __FUNCTION__, __LINE__));
    }

}

void HAL_CIPHER_ResetException(void)
{
    MS_U32 u32DMACtrl = REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]);
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = u32DMACtrl | REG_EXCEPT_FLAG_CLEAR;  //clear all except flag
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = u32DMACtrl & ~REG_EXCEPT_FLAG_CLEAR;  //enable acpu except flag
}

MS_BOOL HAL_CIPHER_ResetKey(MS_U32 u32KeyIdx)
{
    HAL_CIPHER_KEYSRC KeySrc;

    //Check Key Index
    if(u32KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid Key Index[%lu]\n",
            __func__, __LINE__, u32KeyIdx));
        return FALSE;
    }

    KeySrc = _HAL_CIPHER_TransKey(E_CIPHER_KSRC_KL, u32KeyIdx);

    _uDmaCtrl0.DmaCtrl0.DK = FALSE;
    _uDmaCtrl0.DmaCtrl0.KeySel = KeySrc|CMD_DMA_KSEL_CLR_SK;

    return TRUE;

}

void HAL_CIPHER_SWReset(void)
{
    MS_U32 DMA_CTRL = REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]);
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = DMA_CTRL | REG_DMA_SW_RESET;  //reset CryptoDMA
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = DMA_CTRL & ~REG_DMA_SW_RESET; //enable CryptoDMA

    //Enable SW IV, ECO
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = DMA_CTRL | REG_DMA_NDS_SW_IV;
}

void HAL_CIPHER_DMA_Set_InputSrcFrom(CIPHER_MEM_TYPE InputSrcFrom, MS_U8* pu8Data, MS_U32 u32Size)
{
	HAL_CIPHER_DATASRC src = E_DMA_SRC_DRAM;

	switch(InputSrcFrom)
	{
		case E_CIPHER_DRAM:
			src = E_DMA_SRC_DRAM;
			break;
		case E_CIPHER_DIRECT:
			src = E_DMA_SRC_DIRECT;
			break;
		default:
			src = E_DMA_SRC_DRAM;
	}

    _uDmaCtrl1.DmaCtrl1.SD = (MS_U32)src;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set input source from [%d]\n", __func__, __LINE__, src));
}

MS_BOOL HAL_CIPHER_DMA_Set_OutputDstTo(CIPHER_MEM_TYPE OutputDstTo, MS_U8* pu8Data, MS_U32 u32Size)
{
    HAL_CIPHER_DATADST dst = E_DMA_DST_DRAM;

	switch(OutputDstTo)
       {
        case E_CIPHER_DRAM:
            dst = E_DMA_DST_DRAM;
            break;
        case E_CIPHER_DIRECT://only for ACPU
            dst = E_DMA_DST_REGFILE;
            break;
        default:
            dst = E_DMA_DST_DRAM;
        }
    _uDmaCtrl1.DmaCtrl1.Dest= (MS_U32)dst;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set output destination to [%d]\n", __func__, __LINE__, dst));

    return TRUE;
}

MS_BOOL HAL_CIPHER_DMA_Set_FileinDesc( MS_PHY u32FileinAddr , MS_U32 u32FileinNum)
{
    MS_PHY u32temp = _u32DmaInAddr;

    _u32DmaInAddr = u32FileinAddr ;
    _u32DmaInNum = u32FileinNum ;

    // Check overflow
    if(u32temp > _u32DmaInAddr)
    {
        _u32DmaInAddr = 0;
        HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] _u32DmaInAddr overflow\n", __func__, __LINE__));
        return FALSE;
    }

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]_u32DmaInAddr = %x _u32DmaInNum = %x\n", __func__, __LINE__, (unsigned int)_u32DmaInAddr, (unsigned int)_u32DmaInNum));
    return TRUE;
}

void HAL_CIPHER_DMA_Set_FileoutDesc(MS_PHY u32FileoutSAddr, MS_PHY u32phyFileoutEAddr)
{
    _u32DmaOut_StartAddr = u32FileoutSAddr;
    _u32DmaOut_EndAddr   = u32phyFileoutEAddr;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d] _u32DmaOut_StartAddr = %x _u32DmaOut_EndAddr = %x\n", __func__, __LINE__, (unsigned int)_u32DmaOut_StartAddr, (unsigned int)_u32DmaOut_EndAddr));
}

void HAL_CIPHER_OTPHash_Set_FileinDesc(MS_PHY u32FileinAddr, MS_U32 u32FileinNum, MS_U32 u32CurrentRound, CIPHER_MEM_TYPE eInputSrcFrom)
{

}

MS_BOOL HAL_CIPHER_DMA_Set_Key(DRV_CIPHER_KEY stKey)
{
    HAL_CIPHER_KEYSRC KeySrc = 0;
    HAL_CIPHER_ALGO algo = 0;
    MS_BOOL bDK = FALSE;

    algo = _uDmaCtrl0.DmaCtrl0.Algo;

    if(E_DMA_ALGO_NONE == algo)
    {
        //No key
        bDK = TRUE;
        memset(_pu8CipherKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        goto SET_KEY_DONE;
    }

    //Check Key Source, Index
    if(((E_CIPHER_KSRC_KL == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)) ||
       ((E_CIPHER_KSRC_OTP == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1)))
    {
       HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid KeySrc[%d], Index[%d]\n",
           __func__, __LINE__, stKey.eKeySrc, stKey.u8KeyIdx));
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stKey.eKeySrc, stKey.u8KeyIdx);

    // key from CPU
    if(E_DMA_KSEL_REGKEY == KeySrc)
    {
        if ((NULL != stKey.pu8KeyData) && (stKey.u8KeyLen != 0) && (stKey.u8KeyLen <= HAL_CRYPTODMA_KEYLEN_MAX) )
        {
            memset(_pu8CipherKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            memcpy(_pu8CipherKey, stKey.pu8KeyData , stKey.u8KeyLen);
            bDK = TRUE;
            KeySrc = 0;
        }
        else
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Key is NULL\n", __func__, __LINE__));
            return FALSE ;
        }
    }
    else if(E_DMA_KSEL_CAIP == KeySrc)
    {
        bDK = FALSE ;
        KeySrc = 0 ;
    }
    else// key from KL or OTP
    {
        bDK = FALSE;
    }

SET_KEY_DONE:
	_uDmaCtrl0.DmaCtrl0.DK = bDK;
    _uDmaCtrl0.DmaCtrl0.KeySel = KeySrc;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set DK[%d], KeySel[%d]\n",
        __func__, __LINE__, bDK, KeySrc));

    return TRUE ;
}

void HAL_CIPHER_DMA_Set_IV(const MS_U8 *pu8IV, MS_U32 u32Size)
{
    MS_BOOL bDIV = FALSE;

    if ((NULL != pu8IV) && (u32Size != 0) && (u32Size <= HAL_CRYPTODMA_KEYLEN_MAX))
    {
        memset(_pu8IV, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        memcpy(_pu8IV, pu8IV, u32Size);
        bDIV = TRUE ;
    }
    else // Data from Dram
    {
        memset(_pu8IV, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
        bDIV = FALSE ;
    }

    _uDmaCtrl0.DmaCtrl0.DIV = bDIV ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set DIV[%d]\n",
        __func__, __LINE__, bDIV));
}


// Set Direct Data to Register Input, if Dram input, set pu8Data = NULL ;
MS_BOOL HAL_CIPHER_DMA_Set_Data(MS_U8 *pu8Data, MS_U32 u32Size)
{
    HAL_CIPHER_DATASRC eDataSrc = E_DMA_SRC_DRAM;

    // Data size can't be zero //
    if (( 0 == u32Size ) && (NULL != pu8Data ))
    {
        return FALSE ;
    }

    if (NULL != pu8Data )  // Data from CMDQ
    {
        _u32DmaInNum = u32Size ;
        memset(_pu8Data, 0x0, HAL_CRYPTODMA_DIRDATA_MAX);
        memcpy(_pu8Data, pu8Data, u32Size);
        eDataSrc = E_DMA_SRC_DIRECT ;
    }
    else // Data from Dram
    {
        memset(_pu8Data, 0x0 , HAL_CRYPTODMA_DIRDATA_MAX );
        eDataSrc = E_DMA_SRC_DRAM ;
    }

    _uDmaCtrl1.DmaCtrl1.SD = eDataSrc ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set SD[%d]\n",
        __func__, __LINE__, eDataSrc));

    return TRUE ;
}

MS_BOOL HAL_CIPHER_DMA_Set_CaVid(MS_U32 u32CAVid)
{
    MS_U32 u32Tmp = u32CAVid;

    if(u32Tmp > HAL_CRYPTODMA_CAVID_MAX)
    {
        return FALSE;
    }

    if(u32Tmp == 0)
    {
        u32Tmp = _CIPHER_CAVid ;
    }
    else
    {
        u32Tmp = u32CAVid ;
    }

    //Enable SW IV
    if(u32CAVid == E_CIPHER_CAVID1)
    {
        MS_U32 u32DMACtrl = REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]);
        REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = u32DMACtrl | REG_DMA_NDS_SW_IV;
    }

    _uDmaCtrl1.DmaCtrl1.CaVid = u32Tmp ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n",
        __func__, __LINE__, (unsigned int)u32Tmp));

    return TRUE;
}

void HAL_CIPHER_DMA_Set_Config(MS_BOOL OutputReg)
{
    _uDmaCtrl1.DmaCtrl1.Dest= OutputReg ;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set dest[%d]\n",
        __func__, __LINE__, OutputReg));
}


void HAL_CIPHER_DMA_Set_ReportMode(MS_BOOL RptInDram, MS_U32 u32DramAddr)
{
    MS_BOOL bRR = FALSE;

    if (RptInDram)
    {
        bRR = TRUE ;
        _u32ReportAddr = u32DramAddr;

    }
    else
    {
        bRR = FALSE ;
        _u32ReportAddr = (MS_U32)NULL;

    }

    _uDmaCtrl1.DmaCtrl1.RR = bRR;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set report mode[%d], report address[0x%08x]\n",
        __func__, __LINE__, bRR, (unsigned int)_u32ReportAddr));
}

// [NOTE] HW Bug, the DMA can't support DRAM Data Swap, Data Error for input and output
void HAL_CIPHER_DMA_Set_DataSwap(MS_BOOL InDataSwap , MS_BOOL OutDataSwap,
                                MS_BOOL DInByteSwap, MS_BOOL DOutByteSwap )
{
    _uDmaCtrl1.DmaCtrl1.CIS = InDataSwap ;
    _uDmaCtrl1.DmaCtrl1.COS = OutDataSwap ;
    //_uDmaCtrl1.DmaCtrl1.DODS = DOutBitSwap ;
    _uDmaCtrl1.DmaCtrl1.DOBS = DOutByteSwap ;
    _uDmaCtrl1.DmaCtrl1.DIBS = DInByteSwap;

}


void HAL_CIPHER_DMA_Set_Algo(DRV_CIPHER_ALGO stAlgo)
{
    HAL_CIPHER_ALGO algo;
    HAL_CIPHER_MODE subalgo;
    HAL_CIPHER_RESIDUE eRes;
    HAL_CIPHER_SHORTBLOCK eSB;

    if(_uDmaCtrl1.DmaCtrl1.CaVid == E_CIPHER_CAVID1)
    {
        algo    = stAlgo.eMainAlgo;
        subalgo = stAlgo.eSubAlgo;
    }
    else
    {
        algo    = _HAL_CIPHER_TransMainAlgo(stAlgo.eMainAlgo);
        subalgo = _HAL_CIPHER_TransSubAlgo(stAlgo.eSubAlgo);
    }

    eRes    = _HAL_CIPHER_TransResAlgo(stAlgo.eResAlgo);
    eSB     = _HAL_CIPHER_TransSBAlgo(stAlgo.eSBAlgo);

    _uDmaCtrl0.DmaCtrl0.Algo= algo ;
    _uDmaCtrl0.DmaCtrl0.SubAlgo= subalgo ;
    _uDmaCtrl0.DmaCtrl0.Residue = eRes ;
    _uDmaCtrl0.DmaCtrl0.SB = eSB ;

     HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set algorithm, MAIN[%d], SUB[%d], RES[%d], SB[%d]\n",
        __func__, __LINE__, algo, subalgo, eRes, eSB));
}

void HAL_CIPHER_DMA_Set_OTPHash(MS_U32 u32CurrentRound, MS_U32 u32OTPHashRound)
{

}

void HAL_CIPHER_DMA_WriteCMDQ(MS_U32 u32Cmd)
{
    WRITE_CMDQ(u32Cmd) ;
}

void HAL_CIPHER_DMA_GetData(MS_U8 *u8Data)
{

    MS_U32 tmp ;

    // [Note] for u8Data is not always 4 bytes alignment.
    tmp = REG32(&_AESDMACtrl->Dma_Out[0]);
    u8Data[0] = (tmp >> 0)  & 0xFF ;
    u8Data[1] = (tmp >> 8)  & 0xFF ;
    u8Data[2] = (tmp >> 16) & 0xFF ;
    u8Data[3] = (tmp >> 24) & 0xFF ;

    tmp = REG32(&_AESDMACtrl->Dma_Out[1]);
    u8Data[4] = (tmp >> 0)  & 0xFF ;
    u8Data[5] = (tmp >> 8)  & 0xFF ;
    u8Data[6] = (tmp >> 16) & 0xFF ;
    u8Data[7] = (tmp >> 24) & 0xFF ;

    tmp = REG32(&_AESDMACtrl->Dma_Out[2]);
    u8Data[8] = (tmp >> 0)  & 0xFF ;
    u8Data[9] = (tmp >> 8)  & 0xFF ;
    u8Data[10] = (tmp >> 16) & 0xFF ;
    u8Data[11] = (tmp >> 24) & 0xFF ;

    tmp = REG32(&_AESDMACtrl->Dma_Out[3]);
    u8Data[12] = (tmp >> 0)  & 0xFF ;
    u8Data[13] = (tmp >> 8)  & 0xFF ;
    u8Data[14] = (tmp >> 16) & 0xFF ;
    u8Data[15] = (tmp >> 24) & 0xFF ;

}

void HAL_CIPHER_DMA_GetRpt(MS_U32 *DmaRpt)
{
    MS_U32 i ;

    for (i = 0 ; i < DMA_RPT_REG_SIZE ; i++)
    {
        DmaRpt[i] = REG32(&_AESDMACtrl->Dma_Reportp[i]);
    }
}

MS_BOOL HAL_CIPHER_DMA_Start(MS_BOOL Decrypt, HAL_CIPHER_INTMODE IntMode, MS_BOOL bWaitCmdQ, MS_U16 u16CmdID)
{
    MS_U32 u32SpareCnt = 0 , CmdLen = 0 ;

    _uDmaCtrl0.DmaCtrl0.OVT = FALSE ;

    //Set Thread Id
    _uDmaCtrl1.DmaCtrl1.TID  = u16CmdID ;

    _uDmaCtrl1.DmaCtrl1.IntM = IntMode ;

    CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_DMA);

    _uDmaCtrl0.DmaCtrl0.CL  = CmdLen & CMD_LEN_MASK ;
    _uDmaCtrl0.DmaCtrl0.CMD = ((Decrypt?0x1:0x2) & 0xF);

DMA_RETRY:
    // [HW] Hardware will check Current spare count first, and others read spare count will be zero!
    u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

    if (u32SpareCnt > CmdLen)
    {
        u32SpareCnt -= CmdLen ;
        WRITE_CMDQ( (_uDmaCtrl0.u32DmaCtrl0)) ;
        WRITE_CMDQ( (_uDmaCtrl1.u32DmaCtrl1)) ;
        WRITE_CMDQ(_u32DmaInAddr) ;
        WRITE_CMDQ(_u32DmaInNum) ;
        WRITE_CMDQ(_u32DmaOut_StartAddr);
        WRITE_CMDQ(_u32DmaOut_EndAddr);

        if (_uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
           WRITE_CMDQ(_u32ReportAddr);
        }

        // DMA Initial Vector from CmdQ
        if (_uDmaCtrl0.DmaCtrl0.DIV == TRUE )
        {
            #if 0
            WRITE_CMDQ(_pu8IV[15]<<24 |_pu8IV[14]<<16 | _pu8IV[13]<<8 | _pu8IV[12]);
            WRITE_CMDQ(_pu8IV[11]<<24 |_pu8IV[10]<<16 | _pu8IV[ 9]<<8 | _pu8IV[ 8]);
            WRITE_CMDQ(_pu8IV[ 3]<<24 |_pu8IV[ 2]<<16 | _pu8IV[ 1]<<8 | _pu8IV[ 0]);
            WRITE_CMDQ(_pu8IV[ 7]<<24 |_pu8IV[ 6]<<16 | _pu8IV[ 5]<<8 | _pu8IV[ 4]);
            #else
            WRITE_CMDQ(_pu8IV[ 0]<<24 |_pu8IV[ 1]<<16 | _pu8IV[ 2]<<8 | _pu8IV[ 3]);
            WRITE_CMDQ(_pu8IV[ 4]<<24 |_pu8IV[ 5]<<16 | _pu8IV[ 6]<<8 | _pu8IV[ 7]);
            WRITE_CMDQ(_pu8IV[ 8]<<24 |_pu8IV[ 9]<<16 | _pu8IV[10]<<8 | _pu8IV[11]);
            WRITE_CMDQ(_pu8IV[12]<<24 |_pu8IV[13]<<16 | _pu8IV[14]<<8 | _pu8IV[15]);
            #endif
        }

        // DMA Data from CmdQ
        if (_uDmaCtrl1.DmaCtrl1.SD == E_DMA_SRC_DIRECT )
        {
            WRITE_CMDQ(_pu8Data[15]<<24 |_pu8Data[14]<<16 | _pu8Data[13]<<8 | _pu8Data[12]);
            WRITE_CMDQ(_pu8Data[11]<<24 |_pu8Data[10]<<16 | _pu8Data[ 9]<<8 | _pu8Data[ 8]);
            WRITE_CMDQ(_pu8Data[ 7]<<24 |_pu8Data[ 6]<<16 | _pu8Data[ 5]<<8 | _pu8Data[ 4]);
            WRITE_CMDQ(_pu8Data[ 3]<<24 |_pu8Data[ 2]<<16 | _pu8Data[ 1]<<8 | _pu8Data[ 0]);
        }

        // DMA Key from CmdQ
        if (_uDmaCtrl0.DmaCtrl0.DK == TRUE )
        {
            #if 0
            WRITE_CMDQ(_pu8CipherKey[15]<<24 |_pu8CipherKey[14]<<16 | _pu8CipherKey[13]<<8 | _pu8CipherKey[12]);
            WRITE_CMDQ(_pu8CipherKey[11]<<24 |_pu8CipherKey[10]<<16 | _pu8CipherKey[ 9]<<8 | _pu8CipherKey[ 8]);
            WRITE_CMDQ(_pu8CipherKey[ 7]<<24 |_pu8CipherKey[ 6]<<16 | _pu8CipherKey[ 5]<<8 | _pu8CipherKey[ 4]);
            WRITE_CMDQ(_pu8CipherKey[ 3]<<24 |_pu8CipherKey[ 2]<<16 | _pu8CipherKey[ 1]<<8 | _pu8CipherKey[ 0]);
            #else
            WRITE_CMDQ(_pu8CipherKey[ 0]<<24 |_pu8CipherKey[ 1]<<16 | _pu8CipherKey[ 2]<<8 | _pu8CipherKey[ 3]);
            WRITE_CMDQ(_pu8CipherKey[ 4]<<24 |_pu8CipherKey[ 5]<<16 | _pu8CipherKey[ 6]<<8 | _pu8CipherKey[ 7]);
            WRITE_CMDQ(_pu8CipherKey[ 8]<<24 |_pu8CipherKey[ 9]<<16 | _pu8CipherKey[10]<<8 | _pu8CipherKey[11]);
            WRITE_CMDQ(_pu8CipherKey[12]<<24 |_pu8CipherKey[13]<<16 | _pu8CipherKey[14]<<8 | _pu8CipherKey[15]);
            #endif
        }

        // [HW] Hardware will check Current spare count != Write back Spare count to start operation.
        REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
        return TRUE ;

    }
    else
    {
        // [HW] Check Current spare count == Write back Spare count to start wait cmd
        REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
        if (bWaitCmdQ)
        {
            //  Retry until CMDQ avaliable
            HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA retry!\n",__func__, __LINE__));
            goto DMA_RETRY;
        }
        else
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA Fail!\n",__func__, __LINE__));
            //  Return Fail, Command abort
            return FALSE ;
        }
    }
//DMA_FAIL:

}

MS_BOOL HAL_CIPHER_OTPHash_Start(HAL_CIPHER_INTMODE IntMode, MS_BOOL bWaitCmdQ, MS_U16 u16CmdID)
{
    return FALSE;
}


MS_BOOL HAL_CIPHER_DMA_CmdDone(MS_U32 u32CmdID, MS_U32 *u32Ret)
{
	MS_U32 u32ExcTmp = 0;
    MS_U32 u32Rpt   = REG32(&_AESDMACtrl->Dma_Reportp[0]) ;
    MS_U32 u32RptId = u32Rpt & REG_CIPHER_RPT_THREAD_MSK;

    if(u32RptId == u32CmdID)
    {
        u32ExcTmp = (MS_U32) REG32(&_AESDMACtrl->Dma_Ctrl[REG_EXCEPT_FLAG]);
        *u32Ret = u32ExcTmp;
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA Cmd[%lu] Not done!ReportId[%lu]\n",
            __func__, __LINE__, u32CmdID, u32RptId));
        return FALSE;
    }

    return TRUE;

}

void HAL_CIPHER_GetException(MS_U32 *pu32ExcFlag)
{
    MS_U32 u32ExcTmp = (MS_U32) REG32(&_AESDMACtrl->Dma_Ctrl[REG_EXCEPT_FLAG]);
    *pu32ExcFlag = u32ExcTmp;
}

MS_U32 HAL_CIPHER_ReadException(MS_U32 u32ExcTmp)
{
    MS_U32 i = 0;
    MS_U32 u32ExcOut = 0;

    for(i = 0;i<CRYPTODMA_DEBUG_SIZE;i++) //check each error flag
    {
        if((u32ExcTmp & (1 << i)) >> i)
        {
            switch(i)
            {
                case E_CIPHER_NDS_Region_fail:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_NDS_Region_fail\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_NDS_keyoff:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_NDS_keyoff\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_sha_write_scr:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_write_scr\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_read_scr:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_read_scr\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_error_flag:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_error_flag\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_read_scr:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_read_scr\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_key:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_key\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_CAVid:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_CAVid\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_CAVID;
                    break;
                case E_CIPHER_except_des:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_des\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_NDS:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_NDS\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_chain:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_chain\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_algo:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_algo\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_key_HID:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_key_HID\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_key_cbc_mac:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_key_cbc_mac\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_disable_masterkey1:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_masterkey1\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_masterkey0:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_masterkey0\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                default:
                    break;

            }

        }

    }

    HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Exception out [0x%08x]\n", __func__, __LINE__, (unsigned int)u32ExcOut));

    return u32ExcOut;

}

void HAL_CIPHER_IntEnable(void)
{
    MS_U16 u16MaskTmp = REG16(_u32RegBase + REG_HST0_FIQ_MASK_63_48);
    u16MaskTmp &= REG_HTS0_FIQ_MASK_CRYPTODMA;  //FIQ 56 in reg_hst0_fiq_mask_63_48_
    REG16(_u32RegBase + REG_HST0_FIQ_MASK_63_48) = u16MaskTmp;
}

void HAL_CIPHER_IntClear(void)
{
    REG16(_u32RegBase + REG_HST0_FIQ_STATUS_63_48) = REG_HTS0_FIQ_CRYPTODMA; //set 1 to clear interrupt
}

//======================================================================

void HAL_CIPHER_Hash_GetRpt(MS_U32 *HashRpt, MS_U32 u32Size)
{
    MS_U32 i , tmp;

    if (u32Size < HASH_RPT_REG_SIZE )
    {
        tmp = u32Size ;
    }
    else
    {
        tmp = HASH_RPT_REG_SIZE ;
    }

    for (i = 0 ; i < tmp ; i++)
    {
        HashRpt[i] = REG32(&_AESDMACtrl->Hash_Reportp[i]);
    }
}

void HAL_CIPHER_Hash_Set_Config(CIPHER_HASH_ALGO algo , MS_BOOL bAutoPad , MS_BOOL bInv16)
{
    HAL_CIPHER_HASHMODE hash_mode = E_HASH_SHA1;
    hash_mode = _HAL_CIPHER_TransHASHAlgo(algo);

    _uHashCtrl0.HashCtrl0.HashSel = hash_mode ;
    _uHashCtrl0.HashCtrl0.AutoPad = bAutoPad ;
    _uHashCtrl0.HashCtrl0.Inv16 = bInv16; //Inverse the byte order per 16 bytes

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH Mode[%d], AutoPad[%d], Inverse16[%d]!\n",
        __func__, __LINE__, hash_mode, bAutoPad, bInv16));
}

MS_BOOL HAL_CIPHER_Hash_SetHMACKey(DRV_CIPHER_HMAC_KEY stHMACKey, CIPHER_HMAC_KPAD eKpad, MS_BOOL bClear)
{
    MS_U32 u32HMACKeySel = 0;
    MS_U8  u8KeyPad      = 0;
    HAL_CIPHER_KEYSRC KeySrc = 0;

	//Check Key Source, Index
    if((E_CIPHER_KSRC_KL == stHMACKey.eKeySrc) ||
       ((E_CIPHER_KSRC_OTP == stHMACKey.eKeySrc) && (stHMACKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1))||
           E_CIPHER_KSRC_CPU== stHMACKey.eKeySrc)
    {
       HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid KeySrc[%d], Index[%d]\n",
           __func__, __LINE__, stHMACKey.eKeySrc, stHMACKey.u8KeyIdx));
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stHMACKey.eKeySrc, stHMACKey.u8KeyIdx);

    switch(KeySrc)
    {
        case E_DMA_KSEL_MK0:
            u32HMACKeySel = CMD_HASH_HMAC_IKEY_MK0;
            u8KeyPad = 2;  //for spec
            break;
        case E_DMA_KSEL_MK1:
            u32HMACKeySel = CMD_HASH_HMAC_IKEY_MK1;
            u8KeyPad = 2;
            break;
        default:
            u32HMACKeySel = 0;
            u8KeyPad = 0;
            break;
    }

    if(eKpad == E_CIPHER_HMAC_OKPAD)
    {
        u32HMACKeySel += u8KeyPad;
    }

    _uHashCtrl0.HashCtrl0.HMAC_KeySel= u32HMACKeySel;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HMAC KeySel[%lu]!\n",
        __func__, __LINE__, u32HMACKeySel));

	return TRUE;

}

MS_BOOL HAL_CIPHER_Hash_Set_CaVid(MS_U32 u32CAVid)
{
    MS_U32 u32Tmp = u32CAVid;

    if(u32Tmp > HAL_CRYPTODMA_CAVID_MAX)
    {
        return FALSE;
    }

    if(u32Tmp == 0)
    {
        u32Tmp = _CIPHER_CAVid ;
    }
    else
    {
        u32Tmp = u32CAVid;
    }

    _uHashCtrl1.HashCtrl1.CaVid = u32Tmp;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n",
        __func__, __LINE__, (unsigned int)u32Tmp));

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_IV(MS_U8 *pu8IV, MS_U32 u32IVSize ,MS_U32 u32IVSel)
{
    // 256 bits for sha256 , 160 bits for sha1
    _uHashCtrl0.HashCtrl0.InitHashSel = u32IVSel ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH IVSel [%lu]\n",
        __func__, __LINE__, u32IVSel));

    if(u32IVSel == CMD_HASH_IV_CMD)
    {
        if((pu8IV != NULL) && (u32IVSize > 0))
        {
            memset(_pu8HashIV, 0x0, HASH_CMD_IV_SIZE);
            memcpy(_pu8HashIV, pu8IV , u32IVSize);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}


MS_BOOL HAL_CIPHER_Hash_Set_InitWordCnt(HAL_CIPHER_IWCTYPE eIWCType, MS_U32 u32StartBytes)
{
	MS_U32 u32IWCType = 0;
    MS_U32 u32IWC = 0;
    MS_U32 Rpt[2];

    if(eIWCType == E_CIPHER_HASH_IWC_PRV) //Init Word Count from report
    {
        u32IWCType = CMD_HASH_IWC_RPT;
        HAL_CIPHER_Hash_GetRpt(Rpt, 2); //For debug
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IWC from report is [%lu]\n",
        __func__, __LINE__, Rpt[1]>>2));
	}
    else
    {
        u32IWCType = CMD_HASH_IWC_CMDQ;
        if((u32StartBytes % HASH_MANUAL_BLOCK_SIZE) != 0)
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init word count is 64 bytes alignment!\n",
                __func__, __LINE__));
            return FALSE;
        }

        if(u32StartBytes != 0)
        {
            u32IWC = u32StartBytes/4; //Init Word Count
        }
        else
        {
            u32IWC = 0;
        }
    }

    _u32InitWordCnt = u32IWC;
    _uHashCtrl0.HashCtrl0.IWC_Sel = u32IWCType;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IWC type[%lu], IWC[%lu]\n",
        __func__, __LINE__, u32IWCType, _u32InitWordCnt));

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_MsgSrcFrom(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashSrc)
{
    MS_U32 u32SrcTmp = 0;
    if((eMemType == E_CIPHER_DQMEM) || (eMemType == E_CIPHER_IQMEM))
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Set HASH source from DQMEM, IQMEM is not support!\n",
        __func__, __LINE__));
        return FALSE;
    }

    switch(eMemType)
    {
    	case E_CIPHER_DRAM:
    		u32SrcTmp = CMD_HASH_SRC_DRAM;
    		break;
        case E_CIPHER_DIRECT:
            u32SrcTmp = CMD_HASH_SRC_REG;
            break;
        default:
            u32SrcTmp = CMD_HASH_SRC_DRAM;
            break;
    }

    *u32HashSrc = u32SrcTmp;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH source from [%lu]\n",
        __func__, __LINE__, u32SrcTmp));

	return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_OutputDstTo(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashDst)
{
    MS_U32 u32DstTmp = 0;

    if((eMemType == E_CIPHER_DQMEM) || (eMemType == E_CIPHER_IQMEM))
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH destination to DQMEM, IQMEM is not support!\n",
        __func__, __LINE__));
        return FALSE;
    }

    switch(eMemType)
    {
        case E_CIPHER_DRAM:
            u32DstTmp = CMD_HASH_OUTPUT_DRAM;

    		break;
        case E_CIPHER_DIRECT:
            u32DstTmp = CMD_HASH_OUTPUT_RPT;

            break;
        default:
            u32DstTmp = CMD_HASH_OUTPUT_RPT;

            break;
    }

    *u32HashDst = u32DstTmp;
    _uHashCtrl0.HashCtrl0.Dest = u32DstTmp;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH destination[%lu]\n",
        __func__, __LINE__, u32DstTmp));

	return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_OuputAddr(MS_U32 u32OutputPAddr, MS_U32 u32DstSel)
{
    if(CMD_HASH_OUTPUT_DRAM == u32DstSel)
    {
        if ((MS_U32)NULL != u32OutputPAddr)
        {
            _u32HashDestAddr = u32OutputPAddr;
        }
        else
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Hash output address is NULL\n",__func__, __LINE__));
            return FALSE;
        }
    }
    return TRUE;

}

MS_BOOL HAL_CIPHER_Hash_SetMsgLength( MS_U32 u32Size )
{
	_u32HashMsgLen = u32Size;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH message length[%lu]\n",
        __func__, __LINE__, u32Size));

	return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_SetHOS(MS_BOOL bHos)
{
	_uHashCtrl0.HashCtrl0.HOS = bHos;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HOS[%d]\n",
        __func__, __LINE__, bHos));

	return TRUE;
}


MS_BOOL HAL_CIPHER_Hash_SetMsg(MS_PHY u32MsgPAddr, MS_U32 u32Size ,MS_U32 u32SrcSel )
{
    _uHashCtrl0.HashCtrl0.Src = u32SrcSel;
    _u32HashMsgLen = u32Size;

    if (CMD_HASH_SRC_DRAM == u32SrcSel)
    {
        if ((MS_U32)NULL != u32MsgPAddr)
        {
            _u32HashSrcAddr = u32MsgPAddr;
        }
        else
        {
            return FALSE;
        }
    }
    else if (CMD_HASH_SRC_REG == u32SrcSel)
    {
        if((u32MsgPAddr == 0) || (u32Size > HASH_CMD_MSG_SIZE))
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set HASH direct message size[%lu] over 64 bytes\n",
                __func__, __LINE__, u32Size));
            return FALSE;
        }
        _u32HashSrcAddr = (MS_U32)NULL;
        memset(_pu8HashMsg , 0x0 , HASH_CMD_MSG_SIZE);
        memcpy(_pu8HashMsg , (MS_U8*)(u32MsgPAddr) , u32Size);
    }

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH message source[%lu], size[%lu]\n",
        __func__, __LINE__, u32SrcSel, _u32HashMsgLen));

    return TRUE;

}


MS_BOOL HAL_CIPHER_Hash_Start(HAL_CIPHER_INTMODE IntMode , MS_BOOL bWaitCmdQ , MS_BOOL bRst, MS_U16 u16CmdID)
{
        MS_U32 u32SpareCnt = 0 , CmdLen = 0 , i = 0 ;
        MS_U32 u32Tmp;

        _uHashCtrl1.HashCtrl1.TID = u16CmdID;

        _uHashCtrl0.HashCtrl0.OVT = FALSE ;
        _uHashCtrl0.HashCtrl0.IntM = IntMode ;
        _uHashCtrl0.HashCtrl0.RstHash = bRst ;

        CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_SHA);

        _uHashCtrl0.HashCtrl0.CL  = CmdLen & CMD_LEN_MASK ;
        _uHashCtrl0.HashCtrl0.CMD = CMD_HASH_START ;

SHA_RETRY:
        u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

        if (u32SpareCnt > CmdLen)
        {
            u32SpareCnt -= CmdLen ;

            WRITE_CMDQ( (_uHashCtrl0.u32HashCtrl0)) ;

            WRITE_CMDQ((_uHashCtrl1.u32HashCtrl1));

            WRITE_CMDQ(_u32HashMsgLen) ;

			//SRC
            if (_uHashCtrl0.HashCtrl0.Src != CMD_HASH_SRC_REG)
            {
                WRITE_CMDQ(_u32HashSrcAddr) ;
            }

			// IWC
            if (CMD_HASH_IWC_CMDQ == _uHashCtrl0.HashCtrl0.IWC_Sel)
            {
               WRITE_CMDQ(_u32InitWordCnt);
            }
            else
            {
                REG32(&_AESDMACtrl->Hash_Reportp[1]) = _u32InitWordCnt;
            }

			// DST
            if ((CMD_HASH_OUTPUT_DRAM == _uHashCtrl0.HashCtrl0.Dest) || (_uHashCtrl0.HashCtrl0.Dest == CMD_HASH_OUTPUT_R2 ))
            {
                WRITE_CMDQ(_u32HashDestAddr);
            }

            // HASH Initial Vector from CmdQ
            if (CMD_HASH_IV_CMD == _uHashCtrl0.HashCtrl0.InitHashSel )
            {
                for ( i = 0 ; i < (HASH_CMD_IV_SIZE/4) ; i ++  )
                {
                    u32Tmp = ((_pu8HashIV[4*i+0]<<24)|(_pu8HashIV[4*i+1]<<16)|(_pu8HashIV[4*i+2]<<8)|(_pu8HashIV[4*i+3]) );

                    WRITE_CMDQ(u32Tmp);
                }
            }

            // HASH Data from CmdQ
            if ( CMD_HASH_SRC_REG == _uHashCtrl0.HashCtrl0.Src )
            {
                for ( i = 0 ; i < (HASH_CMD_MSG_SIZE/4) ; i ++  )
                {
					 u32Tmp = (  (_pu8HashMsg[i*4+0]<<24)
					 			|(_pu8HashMsg[i*4+1]<<16)
					 			|(_pu8HashMsg[i*4+2]<<8)
					 			|(_pu8HashMsg[i*4+3]<<0)
					 		  );

                     WRITE_CMDQ(u32Tmp);
				}
            }

            // [HW] Hardware will check Current spare count != Write back Spare count to start operation.
            REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;

            return TRUE ;

        }
        else
        {
            // [HW] Check Current spare count == Write back Spare count to start wait cmd
            REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;

            if (bWaitCmdQ)
            {
                //  Retry until CMDQ avaliable
                HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]HASH cmd retry!\n",
                    __func__, __LINE__));
                goto SHA_RETRY;
            }
            else
            {
                //  Return Fail, Command abort
                return FALSE ;
            }
        }
}

MS_BOOL HAL_CIPHER_Hash_CmdDone(MS_U32 u32CmdID, MS_U32 *u32Ret)
{
    MS_U32 u32ExcTmp = 0;

    MS_U32 u32Tmp = REG32(&_AESDMACtrl->Hash_Reportp[0]) ;
    MS_U32 u32RptId = u32Tmp & REG_CIPHER_RPT_THREAD_MSK;

    if(u32RptId == u32CmdID)
    {
    	u32ExcTmp = (MS_U32)REG32(&_AESDMACtrl->Dma_Ctrl[REG_EXCEPT_FLAG]);
        *u32Ret = u32ExcTmp;
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]HASH Cmd[%lu] Not done!ReportId[%lu]\n",
                    __func__, __LINE__, u32CmdID, u32RptId));
        return FALSE;
    }

    return TRUE;
}

void HAL_CIPHER_Hash_ExceptFilter(MS_U32 *pu32Exception, CIPHER_KEY_SRC eKeySrc, MS_U8 u8KeyIdx)
{
    //Because CryptoDMA will check every key slot usage for HMAC
    //We need to filter out the correct key exception
    MS_U32 u32ExcTmp = 0;
    HAL_CIPHER_KEYSRC KeySrc = 0;
    MS_U32 i = 0;

    u32ExcTmp = *pu32Exception;

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(eKeySrc, u8KeyIdx);

    //Only check HMAC key exception
    for(i = E_CIPHER_disable_masterkey1;i<=E_CIPHER_disable_masterkey0;i++) //check each error flag
    {
        if((u32ExcTmp & (1 << i)) >> i)
        {
            switch(i)
            {
                case E_CIPHER_disable_masterkey1:
                    if(E_DMA_KSEL_MK1 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_masterkey1));
					break;

                case E_CIPHER_disable_masterkey0:
                    if(E_DMA_KSEL_MK0 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_masterkey0));
					break;

                default:
                    break;
            }
        }
    }

    *pu32Exception = u32ExcTmp;

    return;

}

MS_BOOL HAL_CIPHER_BGC_CheckId(MS_U32 u32EngId)
{
    if(u32EngId < BGC_ENG_SIZE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_CIPHER_BGC_Algo(MS_U32 u32BGC_idx, MS_U32 u32BGC_algo)
{
    MS_U32 u32Algo;

    if(u32BGC_algo < BGC_SUPPORT_ALGO)
    {
        u32Algo = REG32(&_AESDMACtrl->BGC_Algo);

        if (u32BGC_algo == 0)
        {
            u32Algo = u32Algo & (~(1<<u32BGC_idx));
        }
        else
        {
            u32Algo = u32Algo | (1<<u32BGC_idx);
        }

        REG32(&_AESDMACtrl->BGC_Algo) = u32Algo ;

        return TRUE;
    }

    return FALSE;
}

MS_BOOL HAL_CIPHER_BGC_SetKey(MS_U8 *pu8Key)
{
    if(pu8Key != NULL)
    {
        REG32(&_AESDMACtrl->BGC_Key[0]) = (pu8Key[ 0]<<24 |pu8Key[ 1]<<16 | pu8Key[ 2]<<8 | pu8Key[ 3]);
        REG32(&_AESDMACtrl->BGC_Key[1]) = (pu8Key[ 4]<<24 |pu8Key[ 5]<<16 | pu8Key[ 6]<<8 | pu8Key[ 7]);
        REG32(&_AESDMACtrl->BGC_Key[2]) = (pu8Key[ 8]<<24 |pu8Key[ 9]<<16 | pu8Key[10]<<8 | pu8Key[11]);
        REG32(&_AESDMACtrl->BGC_Key[3]) = (pu8Key[12]<<24 |pu8Key[13]<<16 | pu8Key[14]<<8 | pu8Key[15]);

    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_CIPHER_BGC_SetIV(MS_U8 *pu8IV)
{
    if(pu8IV != NULL)
    {
        REG32(&_AESDMACtrl->BGC_IV[0]) = (pu8IV[ 0]<<24 |pu8IV[ 1]<<16 | pu8IV[ 2]<<8 | pu8IV[ 3]);
        REG32(&_AESDMACtrl->BGC_IV[1]) = (pu8IV[ 4]<<24 |pu8IV[ 5]<<16 | pu8IV[ 6]<<8 | pu8IV[ 7]);
        REG32(&_AESDMACtrl->BGC_IV[2]) = (pu8IV[ 8]<<24 |pu8IV[ 9]<<16 | pu8IV[10]<<8 | pu8IV[11]);
        REG32(&_AESDMACtrl->BGC_IV[3]) = (pu8IV[12]<<24 |pu8IV[13]<<16 | pu8IV[14]<<8 | pu8IV[15]);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


MS_BOOL HAL_CIPHER_BGC_Period(MS_U32 u32BGC_idx, HAL_CIPHER_BGC_PERIODTIME ePrd)
{
    MS_U32 u32Tmp;

    switch(ePrd)
    {
    case E_CIPHER_BGC_PERIOD_1MS:       u32Tmp = 90;    break;
    case E_CIPHER_BGC_PERIOD_10MS:      u32Tmp = 900;   break;
    case E_CIPHER_BGC_PERIOD_100MS:     u32Tmp = 9000;  break;
    case E_CIPHER_BGC_PERIOD_1000MS:    u32Tmp = 90000; break;
    default:                            u32Tmp = 90;    break;
    }

    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_Period) = u32Tmp;

    return TRUE;
}


MS_BOOL HAL_CIPHER_BGC_Range(MS_U32 u32BGC_idx, MS_U32 u32SAddr, MS_U32 u32EAddr)
{
    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_SAddr) = u32SAddr;
    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_EAddr) = u32EAddr;
    return TRUE;
}

MS_BOOL HAL_CIPHER_BGC_Golden(MS_U32 u32BGC_idx, MS_U8 *pu8Golden, MS_U32 u32Algo)
{
    MS_U32 u32Tmp;
    MS_U8  u8BGCGolden[20];
    memset(u8BGCGolden, 0x0, 20);

    if(pu8Golden == NULL)
    {
        return FALSE;
    }

    if(u32Algo == 0) //SHA1
    {
        memcpy(u8BGCGolden, pu8Golden, 20);
    }
    else
    {
        memcpy(u8BGCGolden, pu8Golden, 16);
    }

    u32Tmp = (u8BGCGolden[0] << 24 )
            |(u8BGCGolden[1] << 16 )
            |(u8BGCGolden[2] << 8  )
            |(u8BGCGolden[3] << 0  );

    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_GOLDEN4) = u32Tmp;

    u32Tmp = (u8BGCGolden[4] << 24 )
            |(u8BGCGolden[5] << 16 )
            |(u8BGCGolden[6] << 8  )
            |(u8BGCGolden[7] << 0  );

    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_GOLDEN3) = u32Tmp;

    u32Tmp = (u8BGCGolden[8] << 24 )
            |(u8BGCGolden[9] << 16 )
            |(u8BGCGolden[10] << 8  )
            |(u8BGCGolden[11] << 0  );


    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_GOLDEN2) = u32Tmp;

    u32Tmp = (u8BGCGolden[12] << 24 )
            |(u8BGCGolden[13] << 16 )
            |(u8BGCGolden[14] << 8  )
            |(u8BGCGolden[15] << 0  );

    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_GOLDEN1) = u32Tmp;

    u32Tmp = (u8BGCGolden[16] << 24 )
            |(u8BGCGolden[17] << 16 )
            |(u8BGCGolden[18] << 8  )
            |(u8BGCGolden[19] << 0  );

    REG32(&_AESDMACtrl->BGC_Ctrl[u32BGC_idx].BGC_GOLDEN0) = u32Tmp;

    return TRUE;

}

MS_BOOL HAL_CIPHER_BGC_Start(MS_U32 u32BGC_idx, MS_BOOL bEn)
{
    MS_U32 u32En;
    MS_U32 u32Fail;

    u32Fail = REG32(&_AESDMACtrl->BGC_Fail);

    if( 0 != (u32Fail & (1<<u32BGC_idx)))
    {
        REG32(&_AESDMACtrl->BGC_Fail) = (1<<u32BGC_idx); //Wrtite 1 to clear fail flag
        REG32(&_AESDMACtrl->BGC_Fail) = 0;               //Write 0 to enable flag, enable all
    }

    u32En = REG32(&_AESDMACtrl->BGC_Enable);

    if(bEn)
    {
        u32En = u32En | (1<<u32BGC_idx);
    }
    else
    {
        u32En = u32En & (~(1<<u32BGC_idx));
    }

    REG32(&_AESDMACtrl->BGC_Enable) = u32En;
    return TRUE;
}

MS_BOOL HAL_CIPHER_BGC_Check(MS_U32 u32BGC_idx)
{
    MS_U32 u32Tmp;
    u32Tmp = REG32(&_AESDMACtrl->BGC_Fail);

    if( 0 != (u32Tmp & (1<<u32BGC_idx)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


// =====================================
//  MISC function implementation
// =====================================
MS_BOOL HAL_CIPHER_Misc_Random(MS_U8 *pu8Buf, MS_U32 u32Size)
{

    MS_U32 i = 0;

    for(i = 0; i < u32Size; i++)
    {
        *(pu8Buf+i) = (MS_U8)(0xFF & REG32(_u32RegBase + REG_RANDOM_NUMBER));
    }

    return TRUE;
}



