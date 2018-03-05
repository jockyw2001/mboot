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

#ifndef eMMC_DRIVER_H
#define eMMC_DRIVER_H

//=====================================================================================
#include "eMMC_config.h" // [CAUTION]: edit eMMC_config.h for your platform
//=====================================================================================
#include "eMMC_err_codes.h"
#include "part_emmc.h"

#define eMMC_DRIVER_VERSION   3 // used to sync with other SW stages (e.g. linux kernel)

//===========================================================
// debug macro
//===========================================================
extern U32 gu32_eMMCDrvExtFlag;

#define eMMCDRV_EXTFLAG_DISABLE_LOG   BIT0
#define eMMC_DISABLE_LOG(yes)    \
	do{ \
		if(yes)  gu32_eMMCDrvExtFlag |= eMMCDRV_EXTFLAG_DISABLE_LOG; \
	    else     gu32_eMMCDrvExtFlag &= ~eMMCDRV_EXTFLAG_DISABLE_LOG;\
	}while(0);
#define eMMC_IF_DISABLE_LOG()    \
	(gu32_eMMCDrvExtFlag & eMMCDRV_EXTFLAG_DISABLE_LOG)

#define eMMCDRV_EXTFLAG_DISABLE_RETRY   BIT2
#define eMMC_DISABLE_RETRY(yes)    \
	do{ \
		if(yes)  gu32_eMMCDrvExtFlag |= eMMCDRV_EXTFLAG_DISABLE_RETRY; \
	    else     gu32_eMMCDrvExtFlag &= ~eMMCDRV_EXTFLAG_DISABLE_RETRY;\
	}while(0);
#define eMMC_IF_DISABLE_RETRY()    \
	((gu32_eMMCDrvExtFlag & eMMCDRV_EXTFLAG_DISABLE_RETRY))

//===========================================================
// macro for Spec.
//===========================================================
#define ADDRESSING_MODE_BYTE      1 // 1 byte
#define ADDRESSING_MODE_SECTOR    2 // 512 bytes
#define ADDRESSING_MODE_4KB       3 // 4KB bytes

#define eMMC_DEVTYPE_HS400_1_8V   BIT6 // ECSD[196]
#define eMMC_DEVTYPE_HS200_1_8V   BIT4
#define eMMC_DEVTYPE_DDR          BIT2
#define eMMC_DEVTYPE_ALL          (eMMC_DEVTYPE_HS400_1_8V|eMMC_DEVTYPE_HS200_1_8V|eMMC_DEVTYPE_DDR)

#define eMMC_SPEED_OLD            0 // ECSD[185]
#define eMMC_SPEED_HIGH           1
#define eMMC_SPEED_HS200          2
#define eMMC_SPEED_HS400          3

#define eMMC_DRIVING_TYPE0	      0 // x1
#define eMMC_DRIVING_TYPE1	      1 // x1.5
#define eMMC_DRIVING_TYPE2	      2 // x0.75
#define eMMC_DRIVING_TYPE3	      3 // x0.5
#define eMMC_DRIVING_TYPE4	      4 // x1.2

#define eMMC_FLAG_TRIM            BIT0
#define eMMC_FLAG_HPI_CMD12       BIT1
#define eMMC_FLAG_HPI_CMD13       BIT2

#define eMMC_PwrOffNotif_OFF      0
#define eMMC_PwrOffNotif_ON       1
#define eMMC_PwrOffNotif_SHORT    2
#define eMMC_PwrOffNotif_LONG     3

//-------------------------------------------------------
// Devices has to be in 512B block length mode by default
// after power-on, or software reset.
//-------------------------------------------------------
#define eMMC_SECTOR_512BYTE       0x200
#define eMMC_SECTOR_512BYTE_BITS  9
#define eMMC_SECTOR_512BYTE_MASK  (eMMC_SECTOR_512BYTE-1)

#define eMMC_SECTOR_BUF_16KB      (eMMC_SECTOR_512BYTE * 0x20)

#define eMMC_SECTOR_BYTECNT       eMMC_SECTOR_512BYTE
#define eMMC_SECTOR_BYTECNT_BITS  eMMC_SECTOR_512BYTE_BITS
//-------------------------------------------------------

#define eMMC_ExtCSD_SetBit        1
#define eMMC_ExtCSD_ClrBit        2
#define eMMC_ExtCSD_WByte         3

#define eMMC_CMD_BYTE_CNT         5
#define eMMC_R1_BYTE_CNT          5
#define eMMC_R1b_BYTE_CNT         5
#define eMMC_R2_BYTE_CNT          16
#define eMMC_R3_BYTE_CNT          5
#define eMMC_R4_BYTE_CNT          5
#define eMMC_R5_BYTE_CNT          5
#define eMMC_MAX_RSP_BYTE_CNT     eMMC_R2_BYTE_CNT

// ExtCSD[171]
#define PERM_WP_EN    BIT2
#define PWR_WP_EN     BIT0

// ExtCSD[173]
#define SEC_WP_SEL   BIT7
#define PWR_WP_DIS   BIT6
#define PERM_WP_DIS  BIT4
#define PERM_WP_SEL  BIT3
#define PERM_WP_EN   BIT2
#define PWR_WP_SEL   BIT1
#define PWR_WP_EN    BIT0

//===========================================================
// Partition Info parameters
//===========================================================

// internal data Sector Address
#define eMMC_ID_BYTE_CNT           15
#define eMMC_ID_FROM_CID_BYTE_CNT  10
#define eMMC_ID_DEFAULT_BYTE_CNT   11 // last byte means n GB


//===========================================================
// driver flag (u32_DrvFlag)
//===========================================================
#define DRV_FLAG_INIT_DONE       BIT0 // include eMMC identify done

#define DRV_FLAG_GET_PART_INFO   BIT1
#define DRV_FLAG_RSP_WAIT_D0H    BIT2 // currently only R1b

#define DRV_FLAG_SPEED_MASK      (BIT7|BIT6|BIT5)
#define DRV_FLAG_SPEED_HIGH      BIT5

#define eMMC_IF_NORMAL_SDR()   (DRV_FLAG_SPEED_HIGH==(g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_MASK))
#define eMMC_SPEED_MODE()      (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_MASK)

#define DRV_FLAG_PwrOffNotif_MASK   (BIT8|BIT9)
#define DRV_FLAG_PwrOffNotif_OFF    0
#define DRV_FLAG_PwrOffNotif_ON     BIT8
#define DRV_FLAG_PwrOffNotif_SHORT  BIT9
#define DRV_FLAG_PwrOffNotif_LONG   (BIT8|BIT9)

#define DRV_FLAG_RSPFROMRAM_SAVE    BIT10
#define DRV_FLAG_ERROR_RETRY        BIT11
#define DRV_FLAG_GEN_TTABLE         BIT12

#define DRV_FLAG_SPEED_HS400_DETECT BIT13


#define EMMC_MBOOT_PATITION_SIZE 6144
#define EMMC_MPOOL_PATITION_SIZE 4096


typedef struct _eMMC_DRIVER
{
	U32 u32_ChkSum; // [8th ~ last-512] bytes
	U8 au8_Sig[4];  // 'e','M','M','C'

	// ----------------------------------------
    // FCIE
    // ----------------------------------------
	U16 u16_RCA;
	U32 u32_DrvFlag, u32_LastErrCode;
	U8  au8_Rsp[eMMC_MAX_RSP_BYTE_CNT];
	U8  au8_CSD[eMMC_MAX_RSP_BYTE_CNT];
	U8  au8_CID[eMMC_MAX_RSP_BYTE_CNT];
    U8  u8_PadType;
	U16 u16_Reg10_Mode;
	U32 u32_ClkKHz;
	U16 u16_ClkRegVal;

    // ----------------------------------------
    // eMMC
    // ----------------------------------------
	// CSD
	U8  u8_SPEC_VERS;
	U8  u8_R_BL_LEN, u8_W_BL_LEN; // supported max blk len
	U16 u16_C_SIZE;
	U8  u8_TAAC, u8_NSAC, u8_Tran_Speed;
	U8  u8_C_SIZE_MULT;
	U8  u8_ERASE_GRP_SIZE, u8_ERASE_GRP_MULT;
	U8  u8_R2W_FACTOR;

	U8  u8_IfSectorMode;
	U32 u32_eMMCFlag;
	U32 u32_EraseUnitSize;

	// ExtCSD
	U32 u32_SEC_COUNT;
	U32 u32_BOOT_SEC_COUNT;

	U8  u8_BUS_WIDTH, u8_ErasedMemContent;
	U16 u16_ReliableWBlkCnt;
	U8  u8_ECSD185_HsTiming, u8_ECSD192_Ver, u8_ECSD196_DevType, u8_ECSD197_DriverStrength;
	U8  u8_ECSD248_CMD6TO, u8_ECSD247_PwrOffLongTO, u8_ECSD34_PwrOffCtrl;
	U8	u8_ECSD160_PartSupField, u8_ECSD224_HCEraseGRPSize, u8_ECSD221_HCWpGRPSize;
	U8	u8_ECSD159_MaxEnhSize_2, u8_ECSD158_MaxEnhSize_1, u8_ECSD157_MaxEnhSize_0;
	U8	u8_u8_ECSD155_PartSetComplete, u8_ECSD166_WrRelParam;
    U8  u8_BOOT_SIZE_MULT;

    // ----------------------------------------
    // CIS
    // ----------------------------------------
	// nni
	U8 u8_IDByteCnt, au8_ID[eMMC_ID_BYTE_CNT];
	U8 au8_Vendor[16], au8_PartNumber[16];

} eMMC_DRIVER, *P_eMMC_DRIVER;


extern eMMC_DRIVER g_eMMCDrv;

typedef eMMC_PACK0 struct _eMMC_RPMB_DATA
{
   U16 u16_req_rsp;
   U16 u16_result;
   U16 u16_blk_cnt;
   U16 u16_addr;
   U32 u32_writecnt;
   U8  u8_nonce[16];
   U8  u8_data[256];
   U8  u8_auth_key[32];
   U8  u8_stuff[196];
}eMMC_PACK1 eMMC_RPMB_DATA;

typedef eMMC_PACK0 struct _eMMC_TEST_ALIGN_PACK {

	U8	u8_0;
	U16	u16_0;
	U32	u32_0, u32_1;

} eMMC_PACK1 eMMC_TEST_ALIGN_PACK_t;

#define eMMC_RPMB_REQ_AUTH_KEY         0x01
#define eMMC_RPMB_REQ_WRITE_CNT_VAL    0x02
#define eMMC_RPMB_REQ_AUTH_DATA_W      0x03
#define eMMC_RPMB_REQ_AUTH_DATA_R      0x04
#define eMMC_RPMB_REQ_RESULT_R         0x05


//===========================================================
// exposed APIs
//===========================================================
#include "drv_eMMC.h"

//===========================================================
// internal used functions
//===========================================================
#include "eMMC_hal.h"

extern void eMMC_FCIE_GetCMDFIFO(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf);
extern U32 	eMMC_WaitCIFD_Event(U16 u16_WaitEvent, U32  u32_MicroSec);
extern U32 	eMMC_WaitSetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt);
extern U32 	eMMC_WaitGetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt);

#endif // eMMC_DRIVER_H
#else

#endif

