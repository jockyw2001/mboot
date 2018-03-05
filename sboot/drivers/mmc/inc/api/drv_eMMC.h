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
#if defined(CONFIG_MSTAR_BFE_EN) || (defined(CONFIG_MSTAR_NS_UBOOT) && defined(CONFIG_MMC))

#include "datatype.h"
#ifndef __DRV_eMMC_H__
#define __DRV_eMMC_H__

/*=============================================================*/
// Include files
/*=============================================================*/
#include "eMMC_config.h"

/*=============================================================*/
// Extern definition
/*=============================================================*/
typedef struct _eMMC_INFO
{
        U8      au8_Tag[16];
        U8      u8_IDByteCnt;
        U8      au8_ID[15];
        U32     u32_ChkSum;
        U16     u16_SpareByteCnt;
        U16     u16_PageByteCnt;
        U16     u16_BlkPageCnt;
        U16     u16_BlkCnt;
        U32     u32_Config;
        U16     u16_ECCType;
        U16     u16_SeqAccessTime;
        U8      padding[12];
        U8      au8_Vendor[16];
        U8      au8_PartNumber[16];

        U16 u16_ECCCodeByteCnt;
        U16 u16_PageSectorCnt;
        U8  u8_WordMode;

} eMMC_INFO_t;

/*=============================================================*/
// Macro definition
/*=============================================================*/

/*=============================================================*/
// Data type definition
/*=============================================================*/

/*=============================================================*/
// Variable definition
/*=============================================================*/

/*=============================================================*/
// Global function definition
/*=============================================================*/
extern U32  eMMC_Init(void);
extern U32  eMMC_Init_Device(void);
extern U32  eMMC_Init_Device_Ex(void);
extern U32 eMMC_LoadImages(U32 *pu32_Addr, U32 u32_ByteCnt,U32 u32_HashStage);

extern U32  eMMC_GetCID(U8 *pu8_CID); // pu8_CID needs to hold 16 bytes
//--------------------------------------------
extern U32  eMMC_GetID(U8 *pu8IDByteCnt, U8 *pu8ID);

extern U32 eMMC_GetExtCSD(U8* pu8_Ext_CSD);
extern U32 eMMC_SetExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value);

extern U32  eMMC_CheckIfReady(void);
extern void eMMC_ResetReadyFlag(void);
extern U32 eMMC_WriteData(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32 eMMC_ReadData(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr);
extern U32 emmc_read_pdb (U32 part, emmc_partition_t *pdb_p);
extern U32 emmc_write_pdb (U32 part, emmc_partition_t *pdb_p);
extern U32 Search_partition(U8* u8_name, U32* u32_BlkAddr, U32* u32_BlkCnt);
extern U32 eMMC_WriteBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr, U8 u8_PartNo);
extern U32 eMMC_ReadBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr, U8 u8_PartNo);
extern U32  eMMC_ComapreData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt);
extern  void *eMMC_get_RPMBContext_address(void);
//extern  U32  GetHmacKey(U32 u32KeyBufPA, U32 u32key_len);
extern  void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest);
extern  U32  eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt);
extern  U32  eMMC_RPMB_IfKeyWritten(void);
extern  U32  eMMC_RPMB_Read_Result(U16 *pu16_Result);
extern  void eMMC_RPMB_Copy_RESULT(U8 *u8_dest,U8 *u8_src, U32 u32_cnt);
extern  U32  eMMC_RPMB_CheckResultError(void);
extern  void eMMC_RPMB_SWAP_memcpy(U8 *u8_dest,U8 *u8_src, U32 u32_cnt);
extern  U32  eMMC_RPMB_program_auth_key(void);
extern  U32  eMMC_RPMB_Test(void);
extern  U32  eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt);
extern  U32  eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr);
extern  U32  eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr);
extern  U32  eMMC_RPMB_NoDataReq(void);
#endif //__DRV_eMMC_H__

#else

#endif
