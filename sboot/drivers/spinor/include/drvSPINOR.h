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
#define _DRV_SPINOR_H_
#include "halSPINOR.h"



BOOL MDrv_SERFLASH_Read(U32 u32FlashAddr, U32 u32FlashSize, U8 *user_buffer);
BOOL MDrv_SERFLASH_AddressErase(U32 u32StartAddr, U32 u32EraseSize, BOOL bWait);
BOOL MDrv_SERFLASH_SectorErase(U32 u32StartAddr, U32 u32EndAddr);
BOOL MDrv_SERFLASH_Write(U32 u32FlashAddr, U32 u32FlashSize, U8 *user_buffer);
BOOL MDrv_SERFLASH_Protect(BOOL bEnable);
#endif

