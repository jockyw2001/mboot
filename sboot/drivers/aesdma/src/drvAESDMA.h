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

#ifndef _DRV_AESDMA_H_
#define _DRV_AESDMA_H_

#ifdef CONFIG_MSTAR_KIWI
#define AESDMA_HAL_KIWI       1
#endif


//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(AESDMA_HAL_KIWI) && AESDMA_HAL_KIWI
  #include "hal/kiwi/halAESDMA.h"
#else
  #error "Error! no platform selected."
#endif



void MDrv_AESDMA_SecureAuthen(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U32 u32SramSel);
U32 MDrv_AESDMA_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U32 u32SramSel);


#endif
