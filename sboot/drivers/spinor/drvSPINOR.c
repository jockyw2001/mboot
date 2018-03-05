/*
 *#############################################################################
 *
 * Copyright (c) 2006-2011 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * Unless otherwise stipulated in writing, any and all information contained
 * herein regardless in any format shall remain the sole proprietary of
 * MStar Semiconductor Inc. and be kept in strict confidence
 * (¡§MStar Confidential Information¡¨) by the recipient.
 * Any unauthorized act including without limitation unauthorized disclosure,
 * copying, use, reproduction, sale, distribution, modification, disassembling,
 * reverse engineering and compiling of the contents of MStar Confidential
 * Information is unlawful and strictly prohibited. MStar hereby reserves the
 * rights to any and all damages, losses, costs and expenses resulting therefrom.
 *
 *#############################################################################
 */

#ifndef _DRV_SPINOR_H_
#include "drvSPINORCMD.h"
#include "halSPINOR.h"
#endif

//==========================================================================
// Define
//==========================================================================

//==========================================================================
// Global Variable
//==========================================================================

//-------------------------------------------------------------------------------------------------
/// Description : Get flash start block index of a flash address
/// @param  u32FlashAddr    \b IN: flash address
/// @param  pu32BlockIndex    \b IN: poniter to store the returning block index
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
static void MDrv_SERFLASH_AddressToBlock(U32 u32FlashAddr, U32 *pu32BlockIndex)
{
	*pu32BlockIndex = u32FlashAddr / SERFLASH_BLOCK_SIZE;
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors in Serial Flash
/// @param  u32StartBlock    \b IN: start block
/// @param  u32EndBlock    \b IN: end block
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
 BOOL MDrv_SERFLASH_BlockErase(U32 u32StartBlock, U32 u32EndBlock,  BOOL bWait)
{
	return HAL_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, bWait);
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors given starting address and size in Serial Flash
/// @param  u32StartAddr    \b IN: start address at block boundry
/// @param  u32EraseSize    \b IN: size to erase
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
 BOOL MDrv_SERFLASH_AddressErase(U32 u32StartAddr, U32 u32EraseSize, BOOL bWait)
{
		U32  u32StartBlock;
    U32  u32EndBlock;
    MDrv_SERFLASH_AddressToBlock(u32StartAddr, &u32StartBlock);
    MDrv_SERFLASH_AddressToBlock(u32StartAddr + u32EraseSize - 1, &u32EndBlock);
    return MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, bWait);
}


//-------------------------------------------------------------------------------------------------
/// Description : Erase certain 4K sectors in Serial Flash
/// @param  u32StartBlock    \b IN: start address
/// @param  u32EndBlock    \b IN: end address
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
BOOL MDrv_SERFLASH_SectorErase(U32 u32StartAddr, U32 u32EndAddr)
{
    U32 u32I = 0;
    BOOL bRet = FALSE;

#define DRV_SERFLASH_SECTOR_SIZE    0x1000UL
    for( u32I = u32StartAddr; u32I < u32EndAddr; )
    {
        HAL_SERFLASH_SectorErase(u32I);
        if((u32EndAddr-u32I) <= DRV_SERFLASH_SECTOR_SIZE)
        {
            bRet = HAL_SERFLASH_SectorErase(u32EndAddr);
        }
        u32I+=DRV_SERFLASH_SECTOR_SIZE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Description : Write data to Serial Flash
/// @param  u32FlashAddr    \b IN: start address (4-B aligned)
/// @param  u32FlashSize    \b IN: size in Bytes (4-B aligned)
/// @param  user_buffer    \b IN: Virtual Buffer Address ptr to flash write data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
BOOL MDrv_SERFLASH_Write(U32 u32FlashAddr, U32 u32FlashSize, U8 *user_buffer)
{
    return HAL_SERFLASH_Write(u32FlashAddr, u32FlashSize, user_buffer);
}


//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
BOOL MDrv_SERFLASH_Read(U32 u32FlashAddr, U32 u32FlashSize, U8 *user_buffer)
{
    return HAL_SERFLASH_Read(u32FlashAddr, u32FlashSize, user_buffer);
}

//-------------------------------------------------------------------------------------------------
/// Description : Protect/UnProtect All Flash
/// @param  bEnable : switch Protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
BOOL MDrv_SERFLASH_Protect(BOOL bEnable)
{
   return HAL_FSP_WriteProtect(bEnable);
}