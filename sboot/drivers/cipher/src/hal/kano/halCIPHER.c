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
#include "halCHIP.h"

extern void * memcpy ( void * destination, const void * source, size_t num );
extern int printf ( const char * format, ... );
//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define     DELAY_WAITING           1UL
#define     DELAY_RESCHEDULE        10UL
#define     COMQ_AVALIABLE          0x10UL
#define     CIPHER_KEY_SIZE         16UL
#define     CRYPTODMA_DEBUG_SIZE    32UL
#define     HASH_MANUAL_BLOCK_SIZE  64UL
#define     PID_SIZE                2UL
#define     MIU_SELECT_BIT          0x80000000UL

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT               _u32RegBase = 0;
static REG_AESDMACtrl        *_AESDMACtrl  = 0 ;


#define REG32_W(reg, value)     do {    \
                                    (reg)->H = ((value) >> 16); \
                                    (reg)->L = ((value) & 0x0000FFFF);  \
                                } while(0)

#define REG32(addr) (*(volatile MS_U32 *) (addr))
#define REG16(add) *((volatile MS_U16 *)(add))

//--------------------------------------------------------------------------------------------------
//   Local Variable
//--------------------------------------------------------------------------------------------------

static MS_PHY _u32DmaInAddr;
static MS_U32 _u32DmaInNum;
static MS_PHY _u32DmaOut_StartAddr;
static MS_PHY _u32DmaOut_EndAddr;
static MS_PHY _u32ReportAddr;
static MS_U32 _u32OTPHashLengthPadding;

static MS_U8 _pu8CipherKey[CIPHER_KEY_SIZE];
static MS_U8 _pu8CipherKey2[CIPHER_KEY_SIZE];
static MS_U8 _pu8IV[CIPHER_KEY_SIZE];
static MS_U8 _pu8IV2[CIPHER_KEY_SIZE];
static MS_U8 _pu8Data[CIPHER_KEY_SIZE];
static MS_U8 _pu8PID0[PID_SIZE];
static MS_U8 _pu8PID1[PID_SIZE];

MS_U32 _u32CmdLen = 0 ;

// [NOTE] Solve Strict-Aliasing //
static union {
    CMD_DmaFormat0  DmaCtrl0;
    MS_U32 u32DmaCtrl0;
}_uDmaCtrl0;

static union{
    CMD_DmaFormat1  DmaCtrl1;
    MS_U32 u32DmaCtrl1;
}_uDmaCtrl1;

static union{
    CMD_DmaFormat2  DmaCtrl2;
    MS_U32 u32DmaCtrl2;
}_uDmaCtrl2;

static union{
    CMD_DmaFormat3  DmaCtrl3;
    MS_U32 u32DmaCtrl3;
}_uDmaCtrl3;

static union{
    Hash_DmaFormat0  HashCtrl0;
    MS_U32 u32HashCtrl0;
}_uHashCtrl0;

static union{
    Hash_DmaFormat1  HashCtrl1;
    MS_U32 u32HashCtrl1;
}_uHashCtrl1;

static union{
    CMD_HashFormat2  HashCtrl2;
    MS_U32 u32HashCtrl2;
}_uHashCtrl2;

static MS_PHY _u32HashSrcAddr;
static MS_PHY _u32HashDestAddr;
static MS_U32 _u32InitWordCnt;
static MS_U32 _u32HashMsgLen;

static MS_U8  _pu8HashIV[HASH_CMD_IV_SIZE];
static MS_U8  _pu8HashMsg[HASH_CMD_MSG_SIZE];
static MS_U8  _pu8HashHMACHostKey[HASH_CMD_HMAC_HOSTKEY_SIZE];

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
    REG32(&_AESDMACtrl->Cmd_Queue) = (u32Value) ;
}

MS_BOOL _HAL_CIPHER_TransMIUAddr(MS_PHY phyAddr, MS_PHY* phymiuAddr)
{
    CHIP_MIU_ID eMIUSel;
    MS_PHY phyOffset;

    if(NULL == phymiuAddr)
    {
        return FALSE;
    }

    _phy_to_miu_offset(eMIUSel,phyOffset,phyAddr);

    if(E_CHIP_MIU_0 == eMIUSel)
    {
        *phymiuAddr = phyOffset;
    }
    else
    {
        *phymiuAddr = (phyOffset | MIU_SELECT_BIT);
    }

    return TRUE;
}

MS_U32 _HAL_CIPHER_GetCmdLen(HAL_CIPHER_CMDTYPE CmdType)
{

    MS_U32 u32CmdLen = 0 ;


    if ( E_CIPHER_TYPE_DMA == CmdType )
    {
        u32CmdLen = 7;

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
		if ( _uDmaCtrl2.DmaCtrl2.HP == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl3.DmaCtrl3.DK2 == TRUE )
        {
            u32CmdLen += 4 ;
        }
        if ( _uDmaCtrl3.DmaCtrl3.DIV2 == TRUE )
        {
            u32CmdLen += 4 ;
        }

        return u32CmdLen ;
    }


    if ( E_CIPHER_TYPE_SHA == CmdType )
    {

        u32CmdLen = 3 ;

	    if (_uHashCtrl0.HashCtrl0.CMD == CMD_HASH_START_RR )
        {
            u32CmdLen += 1 ;
        }

        if (_uHashCtrl0.HashCtrl0.Src != CMD_HASH_SRC_REG )
        {
            u32CmdLen += 1 ;
        }

		if(_uHashCtrl2.HashCtrl2.RR == TRUE)
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

		if((_uHashCtrl0.HashCtrl0.HMAC_KeySel == CMD_HMAC_KSEL_HK) && (_uHashCtrl0.HashCtrl0.DK == CMD_HMAC_KSEL_DIR_HK))
        {
            u32CmdLen += 4 ;
        }

        return u32CmdLen ;

    }

    if ( E_CIPHER_TYPE_OTPHASH == CmdType )
    {
        u32CmdLen = 7 ;

        if ( _uDmaCtrl1.DmaCtrl1.NL == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl3.DmaCtrl3.AP == TRUE && _uDmaCtrl3.DmaCtrl3.IK == FALSE)
        {
            u32CmdLen += 1 ;
        }

        return u32CmdLen ;
    }


    return 0 ;

}

HAL_CIPHER_KEYSRC _HAL_CIPHER_TransKey(CIPHER_KEY_SRC src, MS_U32 idx)
{
    HAL_CIPHER_KEYSRC KeySrc = E_DMA_KSEL_REGKEY;

    switch(src){
        case E_CIPHER_KSRC_KL:
            KeySrc = E_DMA_KSEL_SK0 + idx;
            break;

        case E_CIPHER_KSRC_OTP:
            KeySrc = E_DMA_KSEL_MK0 + idx;
            break;

        case E_CIPHER_KSRC_CPU:
            KeySrc = E_DMA_KSEL_REGKEY;
            break;

        case E_CIPHER_KSRC_CAIP:
            KeySrc = E_DMA_KSEL_CAIP;
            break;

        case E_CIPHER_KSRC_STR:
            KeySrc = E_DMA_KSEL_STRN;
            break;

        default:
            break;
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
    case E_CIPHER_SUB_CTR_64:
        hal_algo = E_DMA_MODE_CTR_64;
        break;
	case E_CIPHER_SUB_CMAC_Key:
        hal_algo = E_DMA_MODE_CMAC_Key;
        break;
    case E_CIPHER_SUB_CMAC_Algo:
        hal_algo = E_DMA_MODE_CMAC_Algo;
        break;
    case E_CIPHER_SUB_PCBC_ADD:
        hal_algo = E_DMA_MODE_PCBC_ADD;
        break;
    case E_CIPHER_SUB_PCBC_XOR:
        hal_algo = E_DMA_MODE_PCBC_XOR;
        break;
    case E_CIPHER_SUB_OTPHASH:
        hal_algo = E_DMA_MODE_OTPHASH;
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

void HAL_CIPHER_SetBank(MS_VIRT u32BankAddr)
{
    _u32RegBase = u32BankAddr;
    _AESDMACtrl = (REG_AESDMACtrl*)(_u32RegBase + REG_CIPHERCTRL_BASE);

}

void HAL_CIPHER_ResetStatus(MS_BOOL RstDma , MS_BOOL RstHash)
{
    if (RstDma)
    {
        _uDmaCtrl0.u32DmaCtrl0 = 0 ;
        _uDmaCtrl1.u32DmaCtrl1 = 0 ;
        _uDmaCtrl2.u32DmaCtrl2 = 0 ;
        _uDmaCtrl3.u32DmaCtrl3 = 0 ;

        _u32DmaInAddr = 0;
        _u32DmaInNum  = 0 ;
        _u32OTPHashLengthPadding = 0;
        _u32DmaOut_StartAddr = 0;
        _u32DmaOut_EndAddr   = 0;
        _u32ReportAddr = 0 ;
        _memset(&_pu8CipherKey ,   0x0, CIPHER_KEY_SIZE );
        _memset(&_pu8IV        ,   0x0, CIPHER_KEY_SIZE );
		_memset(&_pu8CipherKey2 ,   0x0, CIPHER_KEY_SIZE );
        _memset(&_pu8IV2        ,   0x0, CIPHER_KEY_SIZE );
        _memset(&_pu8Data      ,   0x0, CIPHER_KEY_SIZE );
    }

    if (RstHash)
    {
        (_uHashCtrl0.u32HashCtrl0) = 0 ;
		(_uHashCtrl1.u32HashCtrl1) = 0 ;
		(_uHashCtrl2.u32HashCtrl2) = 0 ;

        _u32HashSrcAddr  = 0 ;
        _u32InitWordCnt  = 0 ;
        _u32HashDestAddr = 0 ;
        _u32HashMsgLen   = 0 ;

        _memset(_pu8HashIV, 0x0 ,HASH_CMD_IV_SIZE);
        _memset(_pu8HashMsg, 0x0 , HASH_CMD_MSG_SIZE);
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

}

void HAL_CIPHER_DMA_Set_InputSrcFrom(CIPHER_MEM_TYPE InputSrcFrom, MS_U8* pu8Data, MS_U32 u32Size)
{
	HAL_CIPHER_DATASRC src = E_DMA_SRC_DRAM;

	switch(InputSrcFrom)
	{
		case E_CIPHER_DRAM:
            _u32DmaInAddr = 0;
			src = E_DMA_SRC_DRAM;
			break;

		case E_CIPHER_DQMEM://only for R2
			_u32DmaInAddr = HAL_CIPHER_BASE_DQMEM;
			src = E_DMA_SRC_DQMEM;
			break;

		case E_CIPHER_IQMEM://only for R2
			_u32DmaInAddr = HAL_CIPHER_BASE_IQMEM;
			src = E_DMA_SRC_IQMEM;
			break;

		// TODO: future work, DIRECT data from pu8Data
		#if 0
		case E_CIPHER_DIRECT:
			HAL_CIPHER_DMA_Set_Data(pu8Data, u32Size);
			src = E_DMA_SRC_DIRECT;
			break;
		#endif

        case E_CIPHER_OTP: //only for OTPHASH
			_u32DmaInAddr = 0;
			src = E_DMA_SRC_HW_INPUT;
			break;

		default:
		    _u32DmaInAddr = 0;
			src = E_DMA_SRC_DRAM;
			break;
	}

    _uDmaCtrl1.DmaCtrl1.SD = (MS_U32)src;
}

MS_BOOL HAL_CIPHER_DMA_Set_OutputDstTo(CIPHER_MEM_TYPE OutputDstTo, MS_U8* pu8Data, MS_U32 u32Size)
{
	HAL_CIPHER_DATADST dst = E_DMA_DST_DRAM;
    MS_BOOL bDstKL = FALSE;

	switch(OutputDstTo)
	{
		case E_CIPHER_DRAM:
            _u32DmaOut_StartAddr = 0;
            _u32DmaOut_EndAddr 	 = 0;
			dst = E_DMA_DST_DRAM;
			break;

		case E_CIPHER_DQMEM: //only for R2
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_DQMEM;
			_u32DmaOut_EndAddr   = HAL_CIPHER_BASE_DQMEM;
			dst = E_DMA_DST_DQMEM;
			break;

		case E_CIPHER_IQMEM: //only for R2
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_IQMEM;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_IQMEM;
			dst = E_DMA_DST_IQMEM;
			break;

		case E_CIPHER_DIRECT: //for ACPU, OTPHASH
			dst = E_DMA_DST_REGFILE;
			break;

		case E_CIPHER_KL_LUT:     //for KL transform algorithm
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_LUT;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_LUT;
			dst = E_DMA_DST_DRAM;
            bDstKL = TRUE;
			break;

		case E_CIPHER_KL_M:       //for KL transform algorithm
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_M;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_M;
			dst = E_DMA_DST_DRAM;
            bDstKL = TRUE;
			break;

		case E_CIPHER_KL_BC:     //for KL transform algorithm
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_BC;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_BC;
			dst = E_DMA_DST_DRAM;
            bDstKL = TRUE;
			break;

		default:
		    _u32DmaOut_StartAddr = 0;
            _u32DmaOut_EndAddr   = 0;
			dst = E_DMA_DST_DRAM;
			break;
	}

    _uDmaCtrl1.DmaCtrl1.Dest= (MS_U32)dst;

    if(bDstKL)
    {
        HAL_CIPHER_DMA_Set_OutputDstKL(TRUE);
    }
    else
    {
    }

    return TRUE;
}

void HAL_CIPHER_DMA_Set_OutputDstKL(MS_BOOL bDstKL)
{
    if(bDstKL)
    {
        _uDmaCtrl1.DmaCtrl1.DestKL = CMD_DMA_OUTPUT_SRAM_KL;
        _uDmaCtrl0.DmaCtrl0.DK = TRUE;
    }
    else
    {
        _uDmaCtrl1.DmaCtrl1.DestKL = CMD_DMA_OUTPUT_DEST;
    }
}

MS_BOOL HAL_CIPHER_DMA_Set_FileinDesc(MS_PHY u32FileinAddr, MS_U32 u32FileinNum)
{
    MS_PHY u32temp = _u32DmaInAddr;

    _u32DmaInAddr += u32FileinAddr;
    _u32DmaInNum = u32FileinNum ;

    // Check overflow
    if(u32temp > _u32DmaInAddr)
    {
        _u32DmaInAddr = 0;
        return FALSE;
    }

    return TRUE;
}

void HAL_CIPHER_DMA_Set_FileoutDesc(MS_PHY u32FileoutSAddr, MS_PHY u32phyFileoutEAddr)
{
    _u32DmaOut_StartAddr += u32FileoutSAddr;
    _u32DmaOut_EndAddr   += u32phyFileoutEAddr;
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
        _memset(_pu8CipherKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        goto SET_KEY_DONE;
    }
    //Check Key Source, Index
    if(((E_CIPHER_KSRC_KL == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)) ||
       ((E_CIPHER_KSRC_OTP == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1)))
    {
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stKey.eKeySrc, stKey.u8KeyIdx);

    // key from CPU
    if(E_DMA_KSEL_REGKEY == KeySrc)
    {
        if ((NULL != stKey.pu8KeyData) && (stKey.u8KeyLen != 0) && (stKey.u8KeyLen <= HAL_CRYPTODMA_KEYLEN_MAX) )
        {
            _memset(_pu8CipherKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            memcpy(_pu8CipherKey, (MS_U8 *)stKey.pu8KeyData , stKey.u8KeyLen);
            bDK = TRUE ;
			KeySrc = 0;
        }
        else
        {
			return FALSE ;
        }

    }
    else if(E_DMA_KSEL_CAIP == KeySrc)
    {
        bDK = FALSE ;
        KeySrc = 0 ;
    }
    else// key from KL or OTP or STR
    {
        bDK = FALSE;
    }

SET_KEY_DONE:

	_uDmaCtrl0.DmaCtrl0.DK = bDK;
    _uDmaCtrl0.DmaCtrl0.KeySel = KeySrc;


    return TRUE ;
}

void HAL_CIPHER_DMA_Set_IV(MS_U8* pu8IV, MS_U32 u32Size)
{
    MS_BOOL bDIV = FALSE;

    if ((NULL != pu8IV) && (u32Size != 0) && (u32Size <= HAL_CRYPTODMA_KEYLEN_MAX) )
    {
        _memset(_pu8IV, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        memcpy(_pu8IV, (MS_U8 *)pu8IV, u32Size);
        bDIV = TRUE;
    }
    else // Data from Dram
    {
        _memset(_pu8IV, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        bDIV = FALSE;
    }

    _uDmaCtrl0.DmaCtrl0.DIV = bDIV ;
}


// Set Direct Data to Register Input, if Dram input, set pu8Data = NULL ;
MS_BOOL HAL_CIPHER_DMA_Set_Data(MS_U8* pu8Data, MS_U32 u32Size)
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
        _memset(_pu8Data, 0x0, HAL_CRYPTODMA_DIRDATA_MAX);
        memcpy(_pu8Data, (MS_U8 *)pu8Data, u32Size);
        eDataSrc = E_DMA_SRC_DIRECT ;
    }
    else // Data from Dram
    {
        _memset(_pu8Data, 0x0 , HAL_CRYPTODMA_DIRDATA_MAX );
        eDataSrc = E_DMA_SRC_DRAM ;
    }

    _uDmaCtrl1.DmaCtrl1.SD = (MS_U32)eDataSrc ;

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

    _uDmaCtrl1.DmaCtrl1.CaVid = u32Tmp ;

    return TRUE;
}

//void HAL_CIPHER_DMA_Set_Algo(CipherAlgo algo , CipherMode subalgo , Cipher_Residue eRes , Cipher_ShortBlock eSB )
void HAL_CIPHER_DMA_Set_Algo(DRV_CIPHER_ALGO stAlgo)
{
    HAL_CIPHER_ALGO algo;
    HAL_CIPHER_MODE subalgo;
    HAL_CIPHER_RESIDUE eRes;
    HAL_CIPHER_SHORTBLOCK eSB;

    if(_uDmaCtrl1.DmaCtrl1.CaVid == E_CIPHER_CAVID1)
    {
        algo    = (HAL_CIPHER_ALGO)stAlgo.eMainAlgo;
        subalgo = (HAL_CIPHER_MODE)stAlgo.eSubAlgo;

        // For OTPHASH in E_CIPHER_CAVID1
        if(stAlgo.eSubAlgo == E_CIPHER_SUB_OTPHASH)
            subalgo = E_DMA_MODE_OTPHASH;
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
}

void HAL_CIPHER_DMA_Set_OTPHash(MS_U32 u32CurrentRound, MS_U32 u32OTPHashRound)
{
    _uDmaCtrl3.DmaCtrl3.AP = 0;
    _uDmaCtrl3.DmaCtrl3.IK = 0;
    // First OTPHASH enable IK, other disable IK
    if(u32CurrentRound == 0)
    {
        _uDmaCtrl3.DmaCtrl3.IK = 1;
    }
    // Final OTPHASH enable AP, other disable AP
    if(u32CurrentRound == u32OTPHashRound)
    {
        _uDmaCtrl3.DmaCtrl3.AP = 1;
    }
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
    MS_PHY u32DmaInAddr = 0;
    MS_PHY u32DmaOut_StartAddr = 0;
    MS_PHY u32DmaOut_EndAddr = 0;

    _uDmaCtrl0.DmaCtrl0.OVT = FALSE ;

    //Set Thread Id
    _uDmaCtrl1.DmaCtrl1.TID  = u16CmdID ;

    _uDmaCtrl1.DmaCtrl1.IntM = IntMode ;

    CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_DMA);

    _uDmaCtrl0.DmaCtrl0.CL  = CmdLen & CMD_LEN_MASK ;
    _uDmaCtrl0.DmaCtrl0.CMD = ((Decrypt?0x3:0x2) & 0xF);

	_uDmaCtrl2.DmaCtrl2.NS = 0;
    _uDmaCtrl1.DmaCtrl1.NL = CMD_NEW_LINE;

    //Parser patch
    if(TRUE == _uDmaCtrl2.DmaCtrl2.HP)
    {
        MS_U32 u32Tmp = 0;
        u32Tmp = _AESDMACtrl->Dma_Ctrl[1];
        u32Tmp |= REG_PARSER_LG_PATH_EN;
        _AESDMACtrl->Dma_Ctrl[1] = u32Tmp;
    }

	//Check STR key valid
    if(CMD_DMA_KSEL_TRNG == _uDmaCtrl0.DmaCtrl0.KeySel)
    {
        MS_U32 u32DMACtrl = REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]);
        if(0 == (REG_DMA_STR_KEY_VALID & u32DMACtrl))
        {
            return FALSE;
        }
    }

    MS_U32 MIUCrossBar = REG32(_u32RegBase + REG_MIU_CROSSBAR);  //MIU CrossBar Bank 0x1613
    REG32(_u32RegBase + REG_MIU_CROSSBAR) = MIUCrossBar | 0xf;  //enable MIU crossbar


    if(_uDmaCtrl1.DmaCtrl1.SD == CMD_DMA_DATA_DRAM)
    {
        if(_HAL_CIPHER_TransMIUAddr(_u32DmaInAddr, &u32DmaInAddr) == FALSE)
        {
            return FALSE;
        }
    }

    if(_uDmaCtrl1.DmaCtrl1.Dest == CMD_DMA_OUTPUT_DRAM)
    {
        if(_HAL_CIPHER_TransMIUAddr(_u32DmaOut_StartAddr, &u32DmaOut_StartAddr) == FALSE)
        {
            return FALSE;
        }
        if(_HAL_CIPHER_TransMIUAddr(_u32DmaOut_EndAddr, &u32DmaOut_EndAddr) == FALSE)
        {
            return FALSE;
        }
    }

DMA_RETRY:
    // [HW] Hardware will check Current spare count first, and others read spare count will be zero!
    u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

    if (u32SpareCnt > CmdLen)
    {
        u32SpareCnt -= CmdLen ;

        WRITE_CMDQ( (_uDmaCtrl0.u32DmaCtrl0)) ;
        WRITE_CMDQ( (_uDmaCtrl1.u32DmaCtrl1)) ;
		WRITE_CMDQ( (_uDmaCtrl2.u32DmaCtrl2)) ;
        WRITE_CMDQ( (_uDmaCtrl3.u32DmaCtrl3)) ;

        WRITE_CMDQ((MS_U32)u32DmaInAddr) ;
        WRITE_CMDQ(_u32DmaInNum) ;
        WRITE_CMDQ((MS_U32)u32DmaOut_StartAddr);
        WRITE_CMDQ((MS_U32)u32DmaOut_EndAddr);

        if (_uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
           WRITE_CMDQ((MS_U32)_u32ReportAddr);
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
        if (_uDmaCtrl1.DmaCtrl1.SD == CMD_DMA_DATA_REG )
        {
			#if 0
            WRITE_CMDQ(_pu8Data[15]<<24 |_pu8Data[14]<<16 | _pu8Data[13]<<8 | _pu8Data[12]);
            WRITE_CMDQ(_pu8Data[11]<<24 |_pu8Data[10]<<16 | _pu8Data[ 9]<<8 | _pu8Data[ 8]);
            WRITE_CMDQ(_pu8Data[ 7]<<24 |_pu8Data[ 6]<<16 | _pu8Data[ 5]<<8 | _pu8Data[ 4]);
            WRITE_CMDQ(_pu8Data[ 3]<<24 |_pu8Data[ 2]<<16 | _pu8Data[ 1]<<8 | _pu8Data[ 0]);
            #else
            WRITE_CMDQ(_pu8Data[ 0]<<24 |_pu8Data[ 1]<<16 | _pu8Data[ 2]<<8 | _pu8Data[ 3]);
            WRITE_CMDQ(_pu8Data[ 4]<<24 |_pu8Data[ 5]<<16 | _pu8Data[ 6]<<8 | _pu8Data[ 7]);
            WRITE_CMDQ(_pu8Data[ 8]<<24 |_pu8Data[ 9]<<16 | _pu8Data[10]<<8 | _pu8Data[11]);
            WRITE_CMDQ(_pu8Data[12]<<24 |_pu8Data[13]<<16 | _pu8Data[14]<<8 | _pu8Data[15]);
            #endif
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

		// PID for HW parser
        if (_uDmaCtrl2.DmaCtrl2.HP == TRUE )
        {
            WRITE_CMDQ(_pu8PID0[ 0]<<27 |_pu8PID0[ 1]<<19 | _pu8PID1[ 0]<<14 | _pu8PID1[ 1]<<6);

        }

        if (_uDmaCtrl3.DmaCtrl3.DIV2 == TRUE )
        {
            WRITE_CMDQ(_pu8IV2[ 0]<<24 |_pu8IV2[ 1]<<16 | _pu8IV2[ 2]<<8 | _pu8IV2[ 3]);
            WRITE_CMDQ(_pu8IV2[ 4]<<24 |_pu8IV2[ 5]<<16 | _pu8IV2[ 6]<<8 | _pu8IV2[ 7]);
            WRITE_CMDQ(_pu8IV2[ 8]<<24 |_pu8IV2[ 9]<<16 | _pu8IV2[10]<<8 | _pu8IV2[11]);
            WRITE_CMDQ(_pu8IV2[12]<<24 |_pu8IV2[13]<<16 | _pu8IV2[14]<<8 | _pu8IV2[15]);
        }

        if (_uDmaCtrl3.DmaCtrl3.DK2 == TRUE )
        {
            WRITE_CMDQ(_pu8CipherKey2[ 0]<<24 |_pu8CipherKey2[ 1]<<16 | _pu8CipherKey2[ 2]<<8 | _pu8CipherKey2[ 3]);
            WRITE_CMDQ(_pu8CipherKey2[ 4]<<24 |_pu8CipherKey2[ 5]<<16 | _pu8CipherKey2[ 6]<<8 | _pu8CipherKey2[ 7]);
            WRITE_CMDQ(_pu8CipherKey2[ 8]<<24 |_pu8CipherKey2[ 9]<<16 | _pu8CipherKey2[10]<<8 | _pu8CipherKey2[11]);
            WRITE_CMDQ(_pu8CipherKey2[12]<<24 |_pu8CipherKey2[13]<<16 | _pu8CipherKey2[14]<<8 | _pu8CipherKey2[15]);
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
            goto DMA_RETRY;
        }
        else
        {
            //  Return Fail, Command abort
            return FALSE ;
        }
    }
//DMA_FAIL:

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
        return FALSE;
    }

    return TRUE;

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
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_NDS_keyoff:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_sha_write_scr:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_read_scr:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_error_flag:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_read_scr:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_key:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_CAVid:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_CAVID;
                    break;
                case E_CIPHER_except_des:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_NDS:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_chain:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_algo:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_key_HID:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_key_cbc_mac:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_file_dqmem:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_hash_dqmem:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_disable_masterkey0:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
                    break;
                case E_CIPHER_disable_masterkey1:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_cck:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_0:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_1:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_2:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_3:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
                    break;
                case E_CIPHER_dma_forbid_qmem2dram:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_dma_forbid_cryptodma_keyslot_qmem:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_sha_forbid_qmem2dram:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_forbid_cryptodma_keyslot_qmem:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
				case E_CIPHER_tdes_key_error:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
				case E_CIPHER_write_address_error:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
				case E_CIPHER_except_str:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
				case E_CIPHER_except_sha_str:
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                default:
                    break;

            }

        }

    }

    return u32ExcOut;
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

}

MS_BOOL HAL_CIPHER_Hash_SetHMACKey(DRV_CIPHER_HMAC_KEY stHMACKey, CIPHER_HMAC_KPAD eKpad, MS_BOOL bClear)
{
	MS_BOOL bHMAC = TRUE;
    MS_U32 u32HMACKeySel = 0;
    MS_U32 u32Kpad = CMD_HASH_IKP;
    HAL_CIPHER_KEYSRC KeySrc;

	 //Check Key Source, Index
    if(((E_CIPHER_KSRC_KL == stHMACKey.eKeySrc) && (stHMACKey.u8KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)) ||
       ((E_CIPHER_KSRC_OTP == stHMACKey.eKeySrc) && (stHMACKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1)))
    {
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stHMACKey.eKeySrc, stHMACKey.u8KeyIdx);

    switch(KeySrc)
    {
        case E_DMA_KSEL_REGKEY:
            u32HMACKeySel = CMD_HMAC_KSEL_HK;
            break;
        case E_DMA_KSEL_STRN:
            u32HMACKeySel = CMD_HMAC_KSEL_STRN;
            break;
        case E_DMA_KSEL_MK0:
            u32HMACKeySel = CMD_HMAC_KSEL_MK0;
            break;
        case E_DMA_KSEL_MK1:
            u32HMACKeySel = CMD_HMAC_KSEL_MK1;
            break;
        case E_DMA_KSEL_CCCK:
            u32HMACKeySel = CMD_HMAC_KSEL_CCCK;
            break;
        case E_DMA_KSEL_SK0:
            u32HMACKeySel = CMD_HMAC_KSEL_SK0;
            break;
        case E_DMA_KSEL_SK1:
            u32HMACKeySel = CMD_HMAC_KSEL_SK1;
            break;
        case E_DMA_KSEL_SK2:
            u32HMACKeySel = CMD_HMAC_KSEL_SK2;
            break;
        case E_DMA_KSEL_SK3:
            u32HMACKeySel = CMD_HMAC_KSEL_SK3;
            break;
        default:
            u32HMACKeySel = 0;
            break;
    }

    if(eKpad == E_CIPHER_HMAC_OKPAD)
    {
        u32Kpad = CMD_HASH_OKP;
    }

    if(bClear)
    {
        u32HMACKeySel += (MS_U32)CMD_HMAC_KSEL_CLR_SK;
    }

    if(KeySrc == E_DMA_KSEL_REGKEY)
    {
		//command mode
        if (0 != stHMACKey.pu8KeyData)
        {
            _memset(_pu8HashHMACHostKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            memcpy(_pu8HashHMACHostKey, (MS_U8 *)stHMACKey.pu8KeyData , stHMACKey.u8KeyLen);
            _uHashCtrl0.HashCtrl0.DK = CMD_HMAC_KSEL_DIR_HK;
        }
        else
        {
            _memset(_pu8HashHMACHostKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            bHMAC = FALSE;
        }
    }

    _uHashCtrl0.HashCtrl0.HMAC = bHMAC;
    _uHashCtrl0.HashCtrl0.KPAD = u32Kpad;
    _uHashCtrl0.HashCtrl0.HMAC_KeySel = u32HMACKeySel;


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

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_IV(MS_U8* pu8IV, MS_U32 u32IVSize ,MS_U32 u32IVSel)
{
    // 256 bits for sha256 , 160 bits for sha1
    _uHashCtrl0.HashCtrl0.InitHashSel = u32IVSel ;


    if(u32IVSel == CMD_HASH_IV_CMD)
    {
        if((pu8IV != NULL) && (u32IVSize > 0))
        {
            _memset(_pu8HashIV, 0x0, HASH_CMD_IV_SIZE);
            memcpy(_pu8HashIV, (MS_U8 *)pu8IV , u32IVSize);

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
    MS_U32 Rpt[2] = {0};

    if(eIWCType == E_CIPHER_HASH_IWC_PRV) //Init Word Count from report
    {
        u32IWCType = CMD_HASH_IWC_RPT;
        HAL_CIPHER_Hash_GetRpt(Rpt, 2); //For debug
    }
    else
    {
        u32IWCType = CMD_HASH_IWC_CMDQ;
        if((u32StartBytes % HASH_MANUAL_BLOCK_SIZE) != 0)
        {
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

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_MsgSrcFrom(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashSrc)
{
    MS_U32 u32SrcTmp = 0;
    switch(eMemType)
    {
        case E_CIPHER_DRAM:
            u32SrcTmp = CMD_HASH_SRC_DRAM;
            _u32HashSrcAddr = 0;
            break;
        case E_CIPHER_DQMEM:
            u32SrcTmp = CMD_HASH_SRC_R2;
            _u32HashSrcAddr = HAL_CIPHER_BASE_DQMEM;
            break;
        case E_CIPHER_IQMEM:
            u32SrcTmp = CMD_HASH_SRC_R2;
            _u32HashSrcAddr = HAL_CIPHER_BASE_IQMEM;
            break;
        case E_CIPHER_DIRECT:
            u32SrcTmp = CMD_HASH_SRC_REG;
            _u32HashSrcAddr = 0;
            break;
        default:
            u32SrcTmp = CMD_HASH_SRC_DRAM;
            _u32HashSrcAddr = 0;
            break;
    }

    *u32HashSrc = u32SrcTmp;

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_OutputDstTo(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashDst)
{
    MS_U32 u32DstTmp = 0;
    switch(eMemType)
    {
        case E_CIPHER_DRAM:
            u32DstTmp = CMD_HASH_OUTPUT_DRAM;
            _u32HashDestAddr = 0;
            break;
        case E_CIPHER_DQMEM:
            u32DstTmp = CMD_HASH_OUTPUT_R2;
            _u32HashDestAddr = HAL_CIPHER_BASE_DQMEM;
            break;
        case E_CIPHER_IQMEM:
            u32DstTmp = CMD_HASH_OUTPUT_R2;
            _u32HashDestAddr = HAL_CIPHER_BASE_IQMEM;
            break;
        case E_CIPHER_DIRECT:
            u32DstTmp = CMD_HASH_OUTPUT_RPT;
            _u32HashDestAddr = 0;
            break;
        default:
            u32DstTmp = CMD_HASH_OUTPUT_RPT;
            _u32HashDestAddr = 0;
            break;
    }

    *u32HashDst = u32DstTmp;
    _uHashCtrl0.HashCtrl0.Dest = u32DstTmp;

	return TRUE;
}

void HAL_CIPHER_Hash_SetMsgLength( MS_U32 u32Size )
{
	_u32HashMsgLen = u32Size;

	return ;
}

void HAL_CIPHER_Hash_SetHOS(MS_BOOL bHos)
{
	_uHashCtrl0.HashCtrl0.HOS = bHos;

	return ;
}


MS_BOOL HAL_CIPHER_Hash_SetMsg(MS_PHY u32MsgPAddr, MS_U32 u32Size ,MS_U32 u32SrcSel )
{
    _uHashCtrl0.HashCtrl0.Src = u32SrcSel;
    _u32HashMsgLen = u32Size;

    if ((CMD_HASH_SRC_DRAM == u32SrcSel) || (CMD_HASH_SRC_R2 == u32SrcSel))
    {
        if (0 != u32MsgPAddr)
        {
            _u32HashSrcAddr += u32MsgPAddr;
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
            return FALSE;
        }
        _u32HashSrcAddr = 0;
        _memset(_pu8HashMsg , 0x0 , HASH_CMD_MSG_SIZE);
        //TODO: fix this
        //memcpy(_pu8HashMsg , (MS_U8 *)(MsOS_PA2KSEG1(u32MsgPAddr)) , u32Size);
    }

    return TRUE;

}


MS_BOOL HAL_CIPHER_Hash_Start(HAL_CIPHER_INTMODE IntMode , MS_BOOL bWaitCmdQ , MS_BOOL bRst, MS_U16 u16CmdID)
{
        MS_U32 u32SpareCnt = 0 , CmdLen = 0 , i = 0 ;
        MS_U32 u32Tmp;

        _uHashCtrl1.HashCtrl1.TID = u16CmdID;
		_uHashCtrl0.HashCtrl0.CMD =  CMD_HASH_START_RR ;

        _uHashCtrl0.HashCtrl0.OVT = FALSE ;
        _uHashCtrl0.HashCtrl0.IntM = IntMode ;

        CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_SHA);

        _uHashCtrl0.HashCtrl0.CL  = CmdLen & CMD_LEN_MASK ;

		_uHashCtrl1.HashCtrl1.NS =  0;

        MS_U32 MIUCrossBar = REG32(_u32RegBase + REG_MIU_CROSSBAR);  //MIU CrossBar Bank 0x1613
        REG32(_u32RegBase + REG_MIU_CROSSBAR) = MIUCrossBar | 0xf;  //enable MIU crossbar


SHA_RETRY:
        u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

        if (u32SpareCnt > CmdLen)
        {
            u32SpareCnt -= CmdLen ;

            WRITE_CMDQ( (_uHashCtrl0.u32HashCtrl0)) ;

            WRITE_CMDQ( (_uHashCtrl1.u32HashCtrl1)) ;

			WRITE_CMDQ( (_uHashCtrl2.u32HashCtrl2)) ;

            WRITE_CMDQ(_u32HashMsgLen) ;

			//SRC
            if (_uHashCtrl0.HashCtrl0.Src != CMD_HASH_SRC_REG)
            {
                WRITE_CMDQ(_u32HashSrcAddr) ;
            }

			//RR address
            if (_uHashCtrl2.HashCtrl2.RR == TRUE)
            {
                WRITE_CMDQ(_u32ReportAddr) ;
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

			if((CMD_HMAC_KSEL_HK == _uHashCtrl0.HashCtrl0.HMAC_KeySel) && (CMD_HMAC_KSEL_DIR_HK == _uHashCtrl0.HashCtrl0.DK))
            {
                for ( i = 0 ; i < (HASH_CMD_HMAC_HOSTKEY_SIZE/4) ; i ++  )
                {
                    u32Tmp = ((_pu8HashHMACHostKey[4*i+0]<<24)|(_pu8HashHMACHostKey[4*i+1]<<16)|(_pu8HashHMACHostKey[4*i+2]<<8)|(_pu8HashHMACHostKey[4*i+3]) );

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
    for(i = E_CIPHER_disable_masterkey0;i<=E_CIPHER_disable_reg_key_3;i++) //check each error flag
    {
        if((u32ExcTmp & (1 << i)) >> i)
        {
            switch(i)
            {
                case E_CIPHER_disable_masterkey0:
                    if(E_DMA_KSEL_MK0 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_masterkey0));
					break;

                case E_CIPHER_disable_masterkey1:
                    if(E_DMA_KSEL_MK1 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_masterkey1));
					break;

                case E_CIPHER_disable_cck:
                    if(E_DMA_KSEL_CCCK != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_cck));
					break;

                 case E_CIPHER_disable_reg_key_0:
                    if(E_DMA_KSEL_SK0 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_0));
					break;

                case E_CIPHER_disable_reg_key_1:
                    if(E_DMA_KSEL_SK1 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_1));
					break;

                case E_CIPHER_disable_reg_key_2:
                    if(E_DMA_KSEL_SK2 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_2));
					break;

                case E_CIPHER_disable_reg_key_3:
                    if(E_DMA_KSEL_SK3 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_3));
                    break;

                default:
                    break;
            }
        }
    }

    *pu32Exception = u32ExcTmp;

    return;

}

// =====================================
//  MISC function implementation
// =====================================
MS_BOOL HAL_CIPHER_Misc_Random(MS_U8 *pu8Buf, MS_U32 u32Size)
{
    MS_U16 u16TRN = 0;
    MS_U16 u16PreTRN = 0;
    MS_U32 u32Count = 0;

    if(NULL == pu8Buf || 0 == u32Size)
        return FALSE;

#if defined (MCU_AEON)

    while(u32Count < u32Size)
    {
        do{
            do{
                //Toggle random number bit
                REG32(_u32RegBase+REG_RNG_TRNG_SCPU) ^= REG_RNG_TRNG_ACK_SCPU;
            }//Read random number valid
            while( !(REG32(_u32RegBase+REG_RNG_TRNG_SCPU) & REG_RNG_TRNG_VALID_SCPU_MASK) );
        }
        while( (u16TRN = (REG32(_u32RegBase+REG_RNG_TRNG_SCPU) & REG_RNG_TRNG_OUT_SCPU_MASK))==u16PreTRN );
        u16PreTRN = u16TRN;

        if(0 == ((u32Size - u32Count)%2UL))
        {
            *(pu8Buf+u32Count)   = u16TRN & 0xFFUL;
            *(pu8Buf+u32Count+1) = (u16TRN & 0xFF00UL) >> 8;
            u32Count += 2;
        }
        else
        {
            *(pu8Buf+u32Count) = u16TRN & 0xFFUL;
            u32Count += 1;
        }
    }

#else

    while(u32Count < u32Size)
    {
        do{
            do{
                //Toggle random number bit
                REG32(_u32RegBase+REG_RNG_TRNG_ACPU) ^= REG_RNG_TRNG_ACK_ACPU;
            }//Read random number valid
            while( !(REG32(_u32RegBase+REG_RNG_TRNG_ACPU) & REG_RNG_TRNG_VALID_ACPU_MASK) );
        }
        while( (u16TRN = (REG32(_u32RegBase+REG_RNG_TRNG_ACPU) & REG_RNG_TRNG_OUT_ACPU_MASK))==u16PreTRN );
        u16PreTRN = u16TRN;

        if(0 == ((u32Size - u32Count)%2UL))
        {
            *(pu8Buf+u32Count)   = u16TRN & 0xFFUL;
            *(pu8Buf+u32Count+1) = (u16TRN & 0xFF00UL) >> 8;
            u32Count += 2;
        }
        else
        {
            *(pu8Buf+u32Count) = u16TRN & 0xFFUL;
            u32Count += 1;
        }
    }
#endif

    return TRUE;
}
