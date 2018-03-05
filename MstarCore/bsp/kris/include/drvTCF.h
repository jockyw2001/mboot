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
/// @file   drvTCF.h
/// @brief  TCF Driver
/// @author MStar Semiconductor,Inc.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DRV_TCF_H__
#define __DRV_TCF_H__

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define DRV_TCF_OK                   0x00000000
#define DRV_TCF_FAIL                 0x00000001


/// CF
///
#define CF_NVM_RAW_WORDS       64
#define CF_PERSIST_STATE_WORDS 128
#define CF_REG128_WORDS        4
#define CF_REG64_WORDS         2
#define CF_BITMAP_WORDS        8
#define CF_CMD_WORDS           21
#define CF_CWC_WORDS           4
#define CF_PRODUCT_OR_ENTRIES  8

#define CF_OP_TYPE_MASK             0x0000E000  // Op[0][15:13]
#define CF_OP_TYPE_POS              13
typedef enum
{
    E_CF_OP_TYPE_OP_CWC           = 0x0,
    E_CF_OP_TYPE_OP_PERSO         = 0x1,
    E_CF_OP_TYPE_OP_DIFF          = 0x2,
    E_CF_OP_TYPE_OP_FEATURE       = 0x3,
    E_CF_OP_TYPE_OP_MFR_TEST      = 0x4
} CF_OPERATION_TYPE;



typedef enum
{
    E_CF_TRANS_STATUS_BUSY_INIT             = 0x0,    /**< Reset from power on condition. */
    E_CF_TRANS_STATUS_BUSY_SETUP            = 0x1,    /**< Reset after a command has completed. */
    E_CF_TRANS_STATUS_READY                 = 0x2,    /**< CF ready to receive a new transaction. */
    E_CF_TRANS_STATUS_INPUT                 = 0x3,    /**< CF is receiving a new transaction. */
    E_CF_TRANS_STATUS_BUSY_OP               = 0x4,    /**< CF is processing transaction. */
    E_CF_TRANS_STATUS_DONE_OK               = 0x8,    /**< Transaction was completed successfully. */
    E_CF_TRANS_STATUS_DONE_ERROR            = 0x9,    /**< Transaction has completed with an error. */
    E_CF_TRANS_STATUS_DONE_MFR_TEST         = 0xE     /**< Special case of successful test unlock. */
} CF_TRANS_STATUS_VALUES;

typedef enum
{
    E_CF_NVM_STATUS_READY                   = 0x0,    /**< NVM personalized and ready. */
    E_CF_NVM_STATUS_UNBISTED                = 0x1,    /**< NVM is unbisted.  Currently unsupported state and should be treated as an error. */
    E_CF_NVM_STATUS_UNPERSO                 = 0x2,    /**< NVM is pre-personalized. */
    E_CF_NVM_STATUS_ERROR                   = 0x3     /**< NVM personalized and ready. */
} CF_NVM_STATUS_VALUES;

typedef enum
{
    E_CF_DIFF_STATUS_UNDIFFERENTIATED        = 0x0,    /**< CF is undifferentiated. */
    E_CF_DIFF_STATUS_DIFFERENTIATED          = 0x1     /**< CF is differentiated. */
} CF_DIFF_STATUS_VALUES;

typedef enum
{
    E_CF_OPERATION_TYPE_OP_CWC              = 0x0,    /**< Request an CWC generation operation. */
    E_CF_OPERATION_TYPE_OP_PERSO            = 0x1,    /**< Request a Personalization operation. */
    E_CF_OPERATION_TYPE_OP_DIFF             = 0x2,    /**< Request a Differentiation operation. */
    E_CF_OPERATION_TYPE_OP_FEATURE          = 0x3,    /**< Request a Feature operation. */
    E_CF_OPERATION_TYPE_OP_MFR_TEST         = 0x4,    /**< Request a Manufacturer Test operation. */
    E_CF_OPERATION_TYPE_OP_UNDEFINED1       = 0x5,    /**< Reserved operation. */
    E_CF_OPERATION_TYPE_OP_UNDEFINED2       = 0x6,    /**< Reserved operation. */
    E_CF_OPERATION_TYPE_OP_UNDEFINED3       = 0x7     /**< Reserved operation. */
} CF_OPERATION_TYPE_VALUES;

typedef enum
{
    E_CF_DECM_SOURCE_NONE                   = 0x0,    /**< Invalid DECM source. Should not be used in normal operation. */
    E_CF_DECM_SOURCE_SW                     = 0x1,    /**< DECM source from software. */
    E_CF_DECM_SOURCE_KEY                    = 0x2,    /**< DECM source from key table. */
    E_CF_DECM_SOURCE_MIX                    = 0x3     /**< Combine DECM input from software and key table. */
} CF_DECM_SOURCE_VALUES;

typedef enum
{
    E_CF_OUTPUT_USAGE_SHV                   = 0x0,    /**< Output a secure hash value. */
    E_CF_OUTPUT_USAGE_CWC_DIRECT            = 0x1,    /**< Direct transfer of CWC to key table. */
    E_CF_OUTPUT_USAGE_CWC_XOR               = 0x2,    /**< XOR of key table entry with CWC. */
    E_CF_OUTPUT_USAGE_CWC_AES_KEY           = 0x3     /**< 128-bit AES decrypt of key table entry with CWC. */
} CF_OUTPUT_USAGE_VALUES;



/// CFB
///
typedef enum
{
    E_CFB_USE_CASE_3_1_1 = 0x00,  // 0
    E_CFB_USE_CASE_3_1_2,         // 1
    E_CFB_USE_CASE_3_2_1,         // 2
    E_CFB_USE_CASE_3_2_2,         // 3
    E_CFB_USE_CASE_3_2_3,         // 4
    E_CFB_USE_CASE_3_3_1,
    E_CFB_USE_CASE_3_3_2,
    E_CFB_USE_CASE_3_3_3,
    E_CFB_USE_CASE_3_3_4,
    E_CFB_USE_CASE_3_4_1,
    E_CFB_USE_CASE_3_4_2,
    E_CFB_USE_CASE_3_5_1,
    E_CFB_USE_CASE_3_5_2,
    E_CFB_USE_CASE_3_5_3,
    E_CFB_USE_CASE_3_5_4,
    E_CFB_USE_CASE_3_6,
} CFB_USE_CASE;

typedef enum
{
    E_CFB_NO_OTP_KEY = 0x00,  // 0
    E_CFB_OTP_KEY_1,          // 1
    E_CFB_OTP_KEY_2,          // 2
    E_CFB_OTP_KEY_3,          // 3
    E_CFB_OTP_KEY_4,
    E_CFB_OTP_KEY_5,
    E_CFB_OTP_KEY_6,
    E_CFB_OTP_KEY_7,
    E_CFB_OTP_KEY_8,
} CFB_OTP_KEY_IDX;

//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------
/// TCF DDI return value
typedef MS_U32                    DRV_TCF_RET;


/// CF
///
typedef struct      // MSTAR_CF_VERSION //
{
    MS_U8                         u8VersionEpoch;
    MS_U8                         u8ManufacturerId;
    MS_U8                         u8NetlistVersion;
    MS_U8                         u8VersionBuildId;
} DRV_CF_VERSION;

typedef struct      // MSTAR_CF_FEATURE //
{
    MS_U32                        u32FeatureVector;
} DRV_CF_FEATURE_VECTOR;

typedef struct      // MSTAR_CF_TRANS_STATUS //
{
    MS_U8                         eTransStatus;
    MS_U8                         u8UseNvmKey;
    MS_U8                         eOperationType;
    MS_U8                         eDecmSrc;
    MS_U8                         eOutputUsage;
    MS_U8                         u8ProductRange;
    MS_U8                         u8ProductOffset;
} DRV_CF_TRANS_STATUS;

typedef struct      // MSTAR_CF_CF_STATUS //
{
    MS_U8                         eNvmStatus;
    MS_U8                         eDiffStatus;
    MS_U8                         u8RecentReset;
    MS_U8                         u8CfAlert;
    MS_U8                         u8DevelopmentMode;
    MS_U8                         u8FuseActivate;
    MS_U8                         u8FuseBlock;
} DRV_CF_CF_STATUS;

typedef struct      // MSTAR_CF_DECM_STATUS //
{
    MS_U8                         u8HwDecmValid;
    MS_U8                         u8HwDecmError;
} DRV_CF_DECM_STATUS;

typedef struct      // MSTAR_CF_OPERATION //
{
    MS_U32                        Op[CF_CMD_WORDS];
} DRV_CF_OPERATION;

typedef struct      // MSTAR_CF_RESULT //
{
    MS_U8                         eOperationType;
    MS_U8                         eTransStatus;
    MS_U8                         u8CwcValid;
    MS_U32                        Shv[4];
} DRV_CF_RESULT;



/// CFB
///
typedef struct
{
    MS_U8                         u8CwcPid;
    MS_U8                         u8CwcFld;
    MS_U8                         u8CwcFscb;
    MS_U8                         u8CwcScb;
    MS_U8                         u8CwcTsid;
} DRV_CFB_CWCCFG;

typedef struct
{
    MS_U8                         u8UseCase;
    MS_U8                         u8OtpKeyIdx;
    MS_U32                        CACWC[4];
    MS_U32                        EPK[4];
    MS_U32                        EFUV[4];

    DRV_CFB_CWCCFG                stCwcCfg;
} DRV_CFB_CFBCFG;


//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------
/// CF APIs
///
DLL_PUBLIC DRV_TCF_RET MDrv_TCF_Init(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Version_Info(DRV_CF_VERSION *pstVersion);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Trans_Status(DRV_CF_TRANS_STATUS *pstTransStatus);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Cf_Status(DRV_CF_CF_STATUS *pstCfStatus);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_FeatureVector(DRV_CF_FEATURE_VECTOR *pstCfFeature);

DLL_PUBLIC DRV_TCF_RET MDrv_CF_Issue_Op(DRV_CF_OPERATION *pCfOperations);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_IsFinished(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CF_Read_Op_Result(DRV_CF_RESULT *pCfResult);


/// CFB APIs
///
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Init(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Enable(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Reset(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Setup(DRV_CFB_CFBCFG *pstCFBCfg);

DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Is_Ready(void);
DLL_PUBLIC DRV_TCF_RET MDrv_CFB_Is_Done(void);


/// DBG APIs
///
DLL_PUBLIC void MDrv_CFB_DBG_KT_Response(void);
DLL_PUBLIC MS_U8 MDrv_CFB_DBG_CFB_FSM(void);

#endif /* __DRV_TCF_H__ */
