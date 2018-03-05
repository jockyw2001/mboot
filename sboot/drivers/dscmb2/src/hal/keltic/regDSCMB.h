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
/// file   drvDscmb.h
/// @brief  Descrambler (Dscmb) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REG_DSCMB_H__
#define __REG_DSCMB_H__


/////////////////////////////////////////////////
//   Special Register Definition
/////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// rsa
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_RSA_BANK                      0x38900         // 0x1712               // 0xBF2E2400
#define REG_SYS_CTRL                            (REG_DSCMB_RSA_BANK + 0x0001)
    #define REG_SYS_NSK_CLK                     16


////////////////////////////////////////////////////////////////////////////////////////////////
// cipher 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_CIPHER0_BANK             0x38380      // 0x1707              // 0xBF2E0E00
#define REG_DSCMB_CIPHER1_BANK             0x38400      // 0x1708              // 0xBF2E1000
#define REG_DSCMB_CIPHER_CNT                1

#define REG_DSCMB_CIPHER_BANK(_a_)              (REG_DSCMB_CIPHER0_BANK + ((_a_)*0x80)) // 0 & 1 only

#define REG_CIPHER_REG0(_a_)                    (REG_DSCMB_CIPHER_BANK(_a_) + 0x0000)
    #define REG_CIPHER_REG0_BLK_BADPKT_EN          0x00000001
    #define REG_CIPHER_REG0_DUALPATH_EN            0x00000002
    #define REG_CIPHER_REG0_3KEYTDES_EN            0x00000004
    #define REG_CIPHER_REG0_REDUCE_U_PKT_EN        0x00000008
    #define REG_CIPHER_REG0_IRD_TCSA3_EN           0x00000010
    #define REG_CIPHER_REG0_TSID0_CC_RESPIN_EN     0x00000100
    #define REG_CIPHER_REG0_CCRESPIN_EN_TSID(_a_)  (0x00000100 << (_a_))

/// trouble register
#define REG_CIPHER_REG1(_a_)                        (REG_DSCMB_CIPHER_BANK(_a_) + 0x0001)
    /// write only, read is NOT allowed
    #define REG_CIPHER_REG1_PARSER_SAMPLE_MSK  0x000001FF     /// ??   write only, read is allowed
    #define REG_CIPHER_REG1_VIEWER_MSK         0x00000E00     /// ??
    #define REG_CIPHER_REG1_VIEWER_SHT         9
    #define REG_CIPHER_REG1_CLEARALL           0x00004000     /// ??
    #define REG_CIPHER_REG1_SNAPSHOT           0x00008000     /// ??
    #define REG_CIPHER_REG1_TSID_MSK           0x000F0000     /// ??
    #define REG_CIPHER_REG1_TSID_SHT           16
    // read only, write is NOT allowed
    #define REG_CIPHER_REG1_PARSERINFO_MSK     0xFFFFFFFF     /// ??  depends on how viewer is set. REG_CIPHER0_REG1_VIEWER_MSK

#define REG_CIPHER_LSAD_SCB(_a_)                    (REG_DSCMB_CIPHER_BANK(_a_) + 0x0002)
    /// write only, read is NOT allowed
    #define REG_CIPHER_LSAD_TROUBLE_CLR        0x00000008
    // read only, write is NOT allowed
    #define REG_CIPHER_LSAD_CIPHER_DEBUG       0x00FFFFFF
    #define REG_CIPHER_LSAD_TROUBLE            0xFF000000

#define REG_CIPHER_ESA_SCB(_a_)                (REG_DSCMB_CIPHER_BANK(_a_) + 0x0003)
    #define REG_CIPHER_SCBFIX_SHT(_a_)         ((_a_)?(13 + ((_a_)* 3) ):0)
    #define REG_CIPHER_SCBFIX_MSK(_a_)         (0x00000007 << REG_CIPHER_SCBFIX_SHT(_a_))
    /// write only, read is NOT allowed
    #define REG_CIPHER_ESA_TROUBLE_CLR         0x00000008
    // read only, write is NOT allowed
    #define REG_CIPHER_ESA_CIPHER_DEBUG        0x00FFFFFF
    #define REG_CIPHER_ESA_TROUBLE             0xFF000000

#define REG_CIPHER_LSAS_SCB(_a_)              (REG_DSCMB_CIPHER_BANK(_a_) + 0x0004)
    /// write only, read is NOT allowed
    #define REG_CIPHER_LSAS_TROUBLE_CLR        0x00000008
    // read only, write is NOT allowed
    #define REG_CIPHER_LSAS_CIPHER_DEBUG       0x00FFFFFF
    #define REG_CIPHER_LSAS_TROUBLE            0xFF000000

#define REG_CIPHER_DMX_SCB(_a_)                 (REG_DSCMB_CIPHER_BANK(_a_) + 0x0005)
    #define REG_CIPHER_DMXU_SCBFIX_SHT(_a_)     ((_a_)?(13 + ((_a_)* 3) ):0)
    #define REG_CIPHER_DMXU_SCBFIX_MSK(_a_)     (0x00000007 << REG_CIPHER_DMXU_SCBFIX_SHT(_a_))

    #define REG_CIPHER_DMXL_SCBFIX_TSID0_MSK   0x00000070
    #define REG_CIPHER_DMXL_SCBFIX_TSID0_SHT   4

#define REG_CIPHER_EKEY_INVALID(_a_)            (REG_DSCMB_CIPHER_BANK(_a_) + 0x0006)
    #define REG_CIPHER_DMXL_SCBFIX_SHT(_a_)     ((_a_)?(13 + ((_a_)* 3) ):4)  // a = 0 ==> REG_DSCMB_CIPHER_BANK + 0x0005
    #define REG_CIPHER_DMXL_SCBFIX_MSK(_a_)     (0x00000007 << REG_CIPHER_DMXL_SCBFIX_SHT(_a_))

    #define REG_CIPHER_EKEY_INVALID_EN         0x00000001
    #define REG_CIPHER_EKEY_INVALID_TSID_MSK   0x000000F0
    #define REG_CIPHER_EKEY_INVALID_TSID_SHT   4

#define REG_CIPHER_CSA2_CTRL(_a_)             (REG_DSCMB_CIPHER_BANK(_a_) + 0x000b)
    #define REG_CIPHER_CSA2_KEEP_TS_SCB        0x00000001
    #define REG_CIPHER_CSA2_KEEP_PES_SCB       0x00000002
    #define REG_CIPHER_CSA2_BYPASS_SCP         0x00000004
    #define REG_CIPHER_CSA2_BYPASS_SID         0x00000008
    #define REG_CIPHER_CSA2_FIX_SCB            0x00000010
    #define REG_CIPHER_CSA2_DIS_PKEND          0x00000020
    #define REG_CIPHER_CSA2_CLROVF             0x00000040
    #define REG_CIPHER_CSA2_SCB0_PKT           0x00000080 // enable CSA2 descramble clear packet

#define REG_CIPHER_ESA_DEFAULT_ALGO(_a_)	  REG_DSCMB_CIPHER_BANK(_a_) + 0x000C
	#define REG_CIPHER_ESA_MSK				  0x00007FFF
	#define REG_CIPHER_ESA_Algo_MSK           0x0000000F
	#define REG_CIPHER_ESA_Algo_SHT           0
	#define REG_CIPHER_ESA_SubAlgo_MSK        0x000000F0
	#define REG_CIPHER_ESA_SubAlgo_SHT        4
	#define REG_CIPHER_ESA_RES_MSK            0x00000700
	#define REG_CIPHER_ESA_RES_SHT            8
	#define REG_CIPHER_ESA_SB_MSK			  0x00007000
	#define REG_CIPHER_ESA_SB_SHT			  12

#define REG_CIPHER_PKTVIEW_STATUS(_a_)         (REG_DSCMB_CIPHER_BANK(_a_) + 0x001C)

////////////////////////////////////////////////////////////////////////////////////////////////
// cipher 1
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// tsce 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_TSCE0_BANK                   0x38480         // 0x1709               // 0xBF2E1200
#define REG_TSCE_CTRL                          (REG_DSCMB_TSCE0_BANK + 0x0000)
    #define REG_TSCE_CTRL_CLK_EN               0x00000001   // 0: disable, 1 : enable,
    #define REG_TSCE_SPS_RST_EN(_a_)           ( 0x10 << (_a_))  // a = 0 ~ 3
    #define REG_TSCE_SPSPVR_RST_EN(_a_)        ( 0x100 << (_a_))  // a = 0 ~ 3

// TSCE PVR engine control
#define REG_TSCE_SRC_SPS_SEL(_a_)              (REG_DSCMB_TSCE0_BANK + 0x0001 + (_a_)) // a = 0 ~ 3
#define REG_TSCE_SRC_CLR_SEL(_a_)              (REG_DSCMB_TSCE0_BANK + 0x0005 + (_a_)) // a = 0 ~ 5
#define REG_TSCE_SRC_TSO_SEL(_a_)              (REG_DSCMB_TSCE0_BANK + 0x000b + (_a_)) // a = 0 ~ 1
    #define REG_TSCE_SRC_EN                0x00000001
    #define REG_TSCE_SRC_MSK               0x00000004
    #define REG_TSCE_SRC_CH0               0x00000000
    #define REG_TSCE_SRC_CH1               0x00000004
    #define REG_TSCE_SRC_TSID_MSK          0x000000F0
    #define REG_TSCE_SRC_TSID_SHT          4
    #define REG_TSCE_SRC_CAVID_MSK         0x00001F00
    #define REG_TSCE_SRC_CAVID_SHT         8
    #define REG_TSCE_SRC_ENC_EN            0x00008000  // only used in SPS Encoding engine

#define REG_TSCE_MULTI2_SYSKEY(_a_)            (REG_DSCMB_TSCE0_BANK + 0x10 + (_a_)) // a = 0 ~ 7
#define REG_TSCE_MULTI2_ROUND                  (REG_DSCMB_TSCE0_BANK + 0x18 )


////////////////////////////////////////////////////////////////////////////////////////////////
// tsce 1
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_TSCE1_BANK               0x38500         // 0x170A               // 0xBF2E1400
#define REG_TSCE_SPSPVR_CTRL                    (REG_DSCMB_TSCE1_BANK + 0x0000)
    #define REG_TSCE_SPSPVR_EN(_a_)             (0x01 << (_a_)) // a = 0 ~ 3
    #define REG_TSCE_SPSPVR_SWP(_a_)            (0x10 << (_a_)) // a = 0 ~ 3

/***************************************************
    PVR0 BUF0 >> 0x01 ~ 0x06 , BUF1 >> 0x07 ~ 0x0c
    PVR1 BUF0 >> 0x0d ~ 0x12 , BUF1 >> 0x13 ~ 0x18
    PVR2 BUF0 >> 0x19 ~ 0x1e , BUF1 >> 0x1f ~ 0x24
    PVR3 BUF0 >> 0x25 ~ 0x2a , BUF1 >> 0x2b ~ 0x30
****************************************************/
/// SPSPVR[_eng_]  buffer [_sel_]
#define REG_TSCE_SPSPVR_BUF_HEAD1(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 1 + (12 * (_eng_)) + (6 * (_sel_))) // eng = 0 ~ 3
#define REG_TSCE_SPSPVR_BUF_TAIL1(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 2 + (12 * (_eng_)) + (6 * (_sel_))) // sel = 0 ~ 1
#define REG_TSCE_SPSPVR_BUF_MID1( _eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 3 + (12 * (_eng_)) + (6 * (_sel_)))
#define REG_TSCE_SPSPVR_BUF_HEAD2(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 4 + (12 * (_eng_)) + (6 * (_sel_)))
#define REG_TSCE_SPSPVR_BUF_TAIL2(_eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 5 + (12 * (_eng_)) + (6 * (_sel_)))
#define REG_TSCE_SPSPVR_BUF_MID2( _eng_ , _sel_)  (REG_DSCMB_TSCE1_BANK + 6 + (12 * (_eng_)) + (6 * (_sel_)))

#define REG_TSCE_SPSPVR_INT_CTRL                 (REG_DSCMB_TSCE1_BANK + 0x0031) // 32 bits
    #define REG_TSCE_SPSPVR_INT_CLR(_eng_ , _sel_)        (0x001 << (((_eng_)*2 )+(_sel_)) )  // eng = 0 ~ 3 , sel = 0 ~ 1
    #define REG_TSCE_SPSPVR_INT_MSK(_eng_ , _sel_)        (0x100 << (((_eng_)*2 )+(_sel_)) )  // eng = 0 ~ 3 , sel = 0 ~ 1

#define REG_TSCE_SPSPVR_WPTR_BASE (REG_DSCMB_TSCE1_BANK + 0x40)
#define REG_TSCE_SPSPVR_WPTR(_pvreng_, _bufsel_)  (REG_TSCE_SPSPVR_WPTR_BASE +( 2 * ( _pvreng_)) + (_bufsel_)) //pvreng = 0~3 ,  bufsel = 0~1

////////////////////////////////////////////////////////////////////////////////////////////////
// KTE
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_KTE_BANK                      0x38600         // 0x170C              // 0xBF2E1800

    // Command interface 1
#define REG_KTE_CMD_CTRL0                    (REG_DSCMB_KTE_BANK + 0x0000)
    // command go
    #define REG_KTE_AKI_GO                      0x00000001
    // field selection
    #define REG_KTE_SEL_MSK        0x0000003E
    #define REG_KTE_SEL_SFT        1
    #define REG_KTE_SEL_LSAD       0x00000000
    #define REG_KTE_SEL_ESA        0x00000002
    #define REG_KTE_SEL_LSAS       0x00000004
    #define REG_KTE_SEL_SWITCH     0x00000006
    #define REG_KTE_SEL_IV         0x00000008
    #define REG_KTE_FIELD_LSAD       0x00000000
    #define REG_KTE_FIELD_ESA        0x00000001
    #define REG_KTE_FIELD_LSAS       0x00000002
    #define REG_KTE_FIELD_SWITCH     0x00000003
    #define REG_KTE_FIELD_IV         0x00000004
    // Enable Key after write //
    #define REG_KTE_AUTO_EN               0x00000080
    // command type
    #define REG_KTE_CMD_MSK         0x00000700
    #define REG_KTE_CMD_SFT         8
    #define REG_KTE_CMD_NOP         0x00000000
    #define REG_KTE_CMD_WRITE       0x00000100
    #define REG_KTE_CMD_KEY_DIS     0x00000200
    #define REG_KTE_CMD_READSW      0x00000300
    #define REG_KTE_CMD_KEY_EN      0x00000400
    #define REG_KTE_CMD_WRITE_KEY         0x1
    #define REG_KTE_CMD_INVALID_KEY       0x2
    #define REG_KTE_CMD_READSWITCH        0x3
    #define REG_KTE_CMD_ENABLE_KEY        0x4

    // Response
    #define REG_KTE_RESP_MSK        0x0000F800
    #define REG_KTE_RESP_SFT                11
        #define REG_KTE_RESP_NO_SLOT              0x1
	    #define REG_KTE_RESP_NOT_ALLOW_WRITE_KEY  0x2
    // CAVID
    #define REG_KTE_CAVID_MSK                 0x001F0000
    #define REG_KTE_CAVID_SHT                 16

    // CMD interface 2
#define REG_KTE_CMD_CTRL1                    (REG_DSCMB_KTE_BANK + 0x0001)
    #define REG_KTE_FLTID_MSK                 0x000000FF
    #define REG_KTE_SCB_MSK                   0x00030000
    #define REG_KTE_SCB_SHT                   16

    // Key Entropy
#define REG_KTE_ETPY_MSK                  0x00300000
#define REG_KTE_ETPY_SHT                  20
#define REG_KTE_ETPY_64                   (0x0<<REG_KTE_ETPY_SHT)
#define REG_KTE_ETPY_128                  (0x1<<REG_KTE_ETPY_SHT)
#define REG_KTE_ETPY_192                  (0x2<<REG_KTE_ETPY_SHT)
#define REG_KTE_ETPY_256                  (0x3<<REG_KTE_ETPY_SHT)
    // Key Usage
#define REG_KTE_USAGE_MSK                  0x7F000000
#define REG_KTE_USAGE_SHT                  24
#define REG_KTE_USAGE_NO_CSA2              (1<<24)
#define REG_KTE_USAGE_NO_CSA3              (1<<25)
#define REG_KTE_USAGE_NO_MULTI2            (1<<26)
#define REG_KTE_USAGE_NO_AES               (1<<27)
#define REG_KTE_USAGE_NO_TDES              (1<<28)
#define REG_KTE_USAGE_NO_DES               (1<<29)

#define REG_KTE_DATA0                    (REG_DSCMB_KTE_BANK + 0x0002) /// trouble register

    // [NonNDS] All Default Set to 0
    // [NDS mode Only]
#define REG_KTE_SWT_DEST                     (REG_DSCMB_KTE_BANK + 0x0002) /// trouble register
    #define REG_KTE_SW0_FORCESCB_MSK      0x00000003
    #define REG_KTE_SW0_SOURCE_MSK        0x0000FF00
    #define REG_KTE_SW0_SOURCE_SHT        8
    #define REG_KTE_SW0_LOWDEST_MSK       0x00FF0000
    #define REG_KTE_SW0_LOWDEST_SHT       16
    #define REG_KTE_SW0_UPPDEST_MSK       0xFF000000
    #define REG_KTE_SW0_UPPDEST_SHT       24
    #define REG_KTE_SW0_NDS_AESMODE_EN    0x00000008
    #define REG_KTE_SW0_NDS_AESMODE_PI    0x00000004


#define REG_KTE_DATA1                    (REG_DSCMB_KTE_BANK + 0x0003)
#define REG_KTE_DATA2                    (REG_DSCMB_KTE_BANK + 0x0004)
#define REG_KTE_DATA3                    (REG_DSCMB_KTE_BANK + 0x0005)

#define REG_KTE_SWT_ESA                      (REG_DSCMB_KTE_BANK + 0x0003) /// ESA SWITCH INTERFACE
#define REG_KTE_SWT_LSAS                     (REG_DSCMB_KTE_BANK + 0x0004) /// LSAS SWITCH INTERFACE
#define REG_KTE_SWT_LSAD                     (REG_DSCMB_KTE_BANK + 0x0005) /// LSAD SWITCH INTERFACE
    #define REG_KET_SWT_DECRYPT           0x00000001
 //   #define REG_KET_SWT_KEYENTROPY        0x00000010  //K3U01  //[NOTE] if Key size > 128 ==> Set this bit (according to algorithm)
 //   #define REG_KET_SWT_KEYUSAGE_MSK      0x00000F00  //K3U01
 //   #define REG_KET_SWT_KEYUSAGE_SHT      8           //K3U01
    #define REG_KET_SWT_SB_MSK            0x00007000
    #define REG_KET_SWT_SB_SHT            12
    #define REG_KET_SWT_RES_MSK           0x00070000
    #define REG_KET_SWT_RES_SHT           16
    #define REG_KET_SWT_SUBALG_MSK        0x00F00000
    #define REG_KET_SWT_SUBALG_SHT        20
    #define REG_KET_SWT_ALG_MSK           0x0F000000
    #define REG_KET_SWT_ALG_SHT           24


#define REG_KTE_DATA4                    (REG_DSCMB_KTE_BANK + 0x0006) /// trouble register
#define REG_KTE_SWT_SELECT                   (REG_DSCMB_KTE_BANK + 0x0006) /// trouble register
    // lower switch
    // upper switch
    #define REG_KTE_SW4_SWT_MSK            0x0000000F
    #define REG_KTE_SW4_LOWSWT_SHT         0
    #define REG_KTE_SW4_UPPSWT_SHT         4
    #define REG_KTE_SW4_A_EN               0x00000008
    #define REG_KTE_SW4_B_EN               0x00000004
    #define REG_KTE_SW4_D_EN               0x00000002
    #define REG_KTE_SW4_E_EN               0x00000001

    // MISC setting
    #define REG_KTE_SW4_CSA2_SBOX_MSK     0x00001F00
    #define REG_KTE_SW4_CSA2_SBOX_SHT     8
    #define REG_KTE_SW4_CSA2MOD_EN        0x00080000
    #define REG_KTE_SW4_PERMUTATION_MSK   0x00070000
    #define REG_KTE_SW4_PERMUTATION_SHT   16




#define REG_PIDIDX_CTRL                          (REG_DSCMB_KTE_BANK + 0x0008)
    // pid slot command start
    #define REG_PIDIDX_CMD_GO                    0x00000001
    // pid slot command : read/write
    #define REG_PIDIDX_DIR_MSK                   0x00000008
    #define REG_PIDIDX_DIR_READ                  0x00000000
    #define REG_PIDIDX_DIR_WIRTE                 0x00000008
    // pid slot command
    #define REG_PIDIDX_WMUX_MSK                   0x00000F00
        #define REG_PIDIDX_WMUX_EN                0x00000000
        #define REG_PIDIDX_WMUX_CAVID             0x00000100
        #define REG_PIDIDX_WMUX_TSID              0x00000200
        #define REG_PIDIDX_WMUX_EVEN_EN           0x00000300
        #define REG_PIDIDX_WMUX_ODD_EN            0x00000400
        #define REG_PIDIDX_WMUX_CLR_EN            0x00000500
        #define REG_PIDIDX_WMUX_DUAL_EN           0x00000600
        #define REG_PIDIDX_WMUX_EVEN_IDX          0x00000700
        #define REG_PIDIDX_WMUX_ODD_IDX           0x00000800
        #define REG_PIDIDX_WMUX_CLR_IDX           0x00000900
    // pid slot number
    #define REG_PIDIDX_FLTID_MSK                    0x00FF0000
    #define REG_PIDIDX_FLTID_SHT                    16

/// trouble register
#define REG_PIDIDX_DATA                          (REG_DSCMB_KTE_BANK + 0x0009)
    // write only, read is not allowed
    #define REG_PIDIDX_WDATA_MSK                 0x000000FF
    // read only, write is not allowed
    #define REG_PIDIDX_RDATA_MSK                 0xFFFFFFFF

    #define REG_PIDIDX_CLR_MSK                   0x0000007F
    #define REG_PIDIDX_CLR_SHT                   0
    #define REG_PIDIDX_ODD_MSK                   0x00007F00
    #define REG_PIDIDX_ODD_SHT                   8
    #define REG_PIDIDX_EVEN_MSK                  0x007F0000
    #define REG_PIDIDX_EVEN_SHT                  16

    #define REG_PIDIDX_DUAL_EN                   0x00800000
    #define REG_PIDIDX_CLR_EN                    0x01000000
    #define REG_PIDIDX_ODD_EN                    0x02000000
    #define REG_PIDIDX_EVEN_EN                   0x04000000

    #define REG_PIDIDX_CAVID_MSK                 0xF8000000
    #define REG_PIDIDX_CAVID_SHT                 27



// key array status register : LSAD // read only
#define REG_KTE_LSAD_STAT(_a_)                 (REG_DSCMB_KTE_BANK + 0x0040 + (_a_) )
#define REG_KTE_LSAD_STAT_0                    (REG_DSCMB_KTE_BANK + 0x0040)
#define REG_KTE_LSAD_STAT_1                    (REG_DSCMB_KTE_BANK + 0x0041)
#define REG_KTE_LSAD_STAT_2                    (REG_DSCMB_KTE_BANK + 0x0042)
#define REG_KTE_LSAD_STAT_3                    (REG_DSCMB_KTE_BANK + 0x0043)

// key array status register : LSAS // read only
#define REG_KTE_LSAS_STAT(_a_)                 (REG_DSCMB_KTE_BANK + 0x0044 + (_a_) )
#define REG_KTE_LSAS_STAT_0                    (REG_DSCMB_KTE_BANK + 0x0044)
#define REG_KTE_LSAS_STAT_1                    (REG_DSCMB_KTE_BANK + 0x0045)
#define REG_KTE_LSAS_STAT_2                    (REG_DSCMB_KTE_BANK + 0x0046)
#define REG_KTE_LSAS_STAT_3                    (REG_DSCMB_KTE_BANK + 0x0047)

// key array status register : ESA // read only
#define REG_KTE_ESA_STAT(_a_)                  (REG_DSCMB_KTE_BANK + 0x0048 + (_a_) )
#define REG_KTE_ESA_STAT_0                     (REG_DSCMB_KTE_BANK + 0x0048)
#define REG_KTE_ESA_STAT_1                     (REG_DSCMB_KTE_BANK + 0x0049)
#define REG_KTE_ESA_STAT_2                     (REG_DSCMB_KTE_BANK + 0x004a)
#define REG_KTE_ESA_STAT_3                     (REG_DSCMB_KTE_BANK + 0x004b)

// key array status register : IV // read only
#define REG_KTE_IV_STAT(_a_)                   (REG_DSCMB_KTE_BANK + 0x004c + (_a_) )
#define REG_KTE_IV_STAT_0                      (REG_DSCMB_KTE_BANK + 0x004c)
#define REG_KTE_IV_STAT_1                      (REG_DSCMB_KTE_BANK + 0x004d)
#define REG_KTE_IV_STAT_2                      (REG_DSCMB_KTE_BANK + 0x004e)
#define REG_KTE_IV_STAT_3                      (REG_DSCMB_KTE_BANK + 0x004f)

// pid slot valid status register
#define REG_PIDIDX_STAT(_a_)                     (REG_DSCMB_KTE_BANK + 0x0050 + (_a_) )
#define REG_PIDIDX_STAT_0                        (REG_DSCMB_KTE_BANK + 0x0050)
#define REG_PIDIDX_STAT_1                        (REG_DSCMB_KTE_BANK + 0x0051)
#define REG_PIDIDX_STAT_2                        (REG_DSCMB_KTE_BANK + 0x0052)
#define REG_PIDIDX_STAT_3                        (REG_DSCMB_KTE_BANK + 0x0053)
#define REG_PIDIDX_STAT_4                        (REG_DSCMB_KTE_BANK + 0x0054)
#define REG_PIDIDX_STAT_5                        (REG_DSCMB_KTE_BANK + 0x0055)
#define REG_PIDIDX_STAT_6                        (REG_DSCMB_KTE_BANK + 0x0056)
#define REG_PIDIDX_STAT_7                        (REG_DSCMB_KTE_BANK + 0x0057)


////////////////////////////////////////////////////////////////////////////////////////////////
//  Key Ladder
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_KL_BANK         (0x38680)         // 0x170D               // 0xBF2E1A00
#define DSCMB_CWKL                (0)
#define DSCMB_PVRKL               (1)

#define REG_DSCMB_KL_SWRST                (REG_DSCMB_KL_BANK + 0x0B)
    #define REG_DSCMB_KL_SWRST_CW         (0x100)
    #define REG_DSCMB_KL_SWRST_PVR        (0x200)

#define REG_DSCMB_KL_TCSA3_ACT_CODE         (REG_DSCMB_KL_BANK + 0x24)    //CWKL only
#define REG_DSCMB_KL_TCSA3_CTRL             (REG_DSCMB_KL_BANK + 0x28)    //CWKL only
    #define REG_DSCMB_KL_TCSA3_START        (0x1)
    #define REG_DSCMB_KL_TCSA3_CHSEL_SHT    (1)
    #define REG_DSCMB_KL_TCSA3_CHSEL0       (0)
    #define REG_DSCMB_KL_TCSA3_CHSEL1       (1)

#define REG_DSCMB_KL_KDF_STATUS               (REG_DSCMB_KL_BANK + 0x4B )
    #define REG_DSCMB_KL_KDF_BUSY             (1)
    #define REG_DSCMB_KL_KDF_DONE             (0)

#define REG_DSCMB_KL_SW_CW(_a_)           (REG_DSCMB_KL_BANK + 0x00 +((_a_)*(0x40)))
#define REG_DSCMB_KL_NONCE_RESP(_a_)      (REG_DSCMB_KL_BANK + 0x04 +((_a_)*(0x40)))
#define REG_DSCMB_KL_ERR_MSG(_a_)         (REG_DSCMB_KL_BANK + 0x0A +((_a_)*(0x40)))
#define REG_DSCMB_KL_KEY_PROP(_a_)        (REG_DSCMB_KL_BANK + 0x0C +((_a_)*(0x40)))

#define REG_DSCMB_KL_KEY_BUS(_a_)         (REG_DSCMB_KL_BANK + 0x0D +((_a_)*(0x40)))
    #define     DSCMB_KL_KEYBUS_DSTDMA_SHT     (0)
    #define     DSCMB_KL_KEYBUS_TSID_SHT       (8)
    #define     DSCMB_KL_KEYBUS_SCB_SHT        (16)
    #define     DSCMB_KL_KEYBUS_FIELD_SHT      (20)
    #define     DSCMB_KL_KEYBUS_PIDNO_SHT      (24)

#define REG_DSCMB_KL_KDF(_a_)             (REG_DSCMB_KL_BANK + 0x0E +((_a_)*(0x40)))
    #define REG_DSCMB_KL_KDF_EN            0x00100000
    #define REG_DSCMB_KL_KDF_APP_ID_MSK    0x0000FFFF
    #define REG_DSCMB_KL_KDF_APP_ID_SHT    0
    #define REG_DSCMB_KL_KDF_HASH_SEL_MSK  0x00010000
    #define REG_DSCMB_KL_KDF_HASH_SEL_SHT  16
//    #define REG_DSCMB_KL_KDF_HWKEY_ID_MSK  0x000C0000
//    #define REG_DSCMB_KL_KDF_HWKEY_ID_SHT  18

#define REG_DSCMB_KL_CTRL(_a_)            (REG_DSCMB_KL_BANK + 0x0F +((_a_)*(0x40)))
    #define REG_DSCMB_KL_CTRL_NONCE_EN         (0x4)
#define REG_DSCMB_KL_ENC_CW(_a_)          (REG_DSCMB_KL_BANK + 0x10 +((_a_)*(0x40)))
#define REG_DSCMB_KL_KEY1(_a_)            (REG_DSCMB_KL_BANK + 0x14 +((_a_)*(0x40)))
#define REG_DSCMB_KL_KEY2(_a_)            (REG_DSCMB_KL_BANK + 0x18 +((_a_)*(0x40)))
#define REG_DSCMB_KL_KEY3(_a_)            (REG_DSCMB_KL_BANK + 0x1C +((_a_)*(0x40)))
#define REG_DSCMB_KL_KEY4(_a_)            (REG_DSCMB_KL_BANK + 0x20 +((_a_)*(0x40)))
#define REG_DSCMB_KL_PRED_NUM(_a_)        (REG_DSCMB_KL_BANK + 0x29 +((_a_)*(0x40)))
#define REG_DSCMB_KL_SW_KEY(_a_)          (REG_DSCMB_KL_BANK + 0x30 +((_a_)*(0x40)))
#define REG_DSCMB_KL_NONCE(_a_)           (REG_DSCMB_KL_BANK + 0x34 +((_a_)*(0x40)))
#define REG_DSCMB_KL_TF_ENC_CW(_a_)       (REG_DSCMB_KL_BANK + 0x38 +((_a_)*(0x40)))


////////////////////////////////////////////////////////////////////////////////////////////////
//  ProgPVR Pid Filter Setting
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_SPSPVR_BANK                        0x38880         // 0x1711              // 0xBF2E2200
#define REG_DSCMB_SPSPVR_FLT(_Eng_,_a_)           (REG_DSCMB_SPSPVR_BANK + ((_Eng_) * 32) + (_a_)) // Eng = 0 ~ 3 , a = 0 ~ 31
    #define REG_DSCMB_SPSPVR_PID_MSK              0x1FFF
    #define REG_DSCMB_SPSPVR_BUF0_EN              0x2000
    #define REG_DSCMB_SPSPVR_BUF1_EN              0x4000


///////////////////////////OTP//////////////////////////////////////////////
#define REG_OTP_BANK  0x48000      //0x1900       //0xBF320000

#define REG_OTP_CH0_SWICH_COMB      (REG_OTP_BANK + 3951)


////////////////////////////////////////////////////////////////////////////////////////////////
// TSP Pid Filter
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_PIDFLT_BASE0                 (0x00240000)
#define REG_PIDFLT_BASE1                 (0x00241000)
#define REG_PIDFLT0(_a_)                 (REG_PIDFLT_BASE0 + ((_a_) * 4))
#define REG_PIDFLT1(_a_)                 (REG_PIDFLT_BASE1 + ((_a_) * 4))

//PID Information
#define REG_TSP_PIDFLT_PID_MSK             0x00001FFF
#define REG_TSP_PIDFLT_PID_SHT             0
#define REG_TSP_PIDFLT_TS_SRC_MSK          0x0000E000
#define REG_TSP_PIDFLT_TS_SRC_SHT          13
#define REG_TSP_PIDFLT_KEYEN               0x00010000
#define REG_TSP_PIDFLT_2NDPID              0x00020000
#define REG_TSP_PIDFLT_PP1                 0x00040000
#define REG_TSP_PIDFLT_PP2                 0x00080000
#define REG_TSP_PIDFLT_PP3                 0x00100000
#define REG_TSP_PIDFLT_PP4                 0x00200000
#define REG_TSP_PIDFLT_PP_MSK              0x003C0000
#define REG_TSP_PIDFLT_CC_MSK              0xFF000000

//PID CA Destination
#define REG_TSP_PIDFLT_UPDST_MSK           0x03C00000
#define REG_TSP_PIDFLT_UPDST_SHT           22
#define REG_TSP_PIDFLT_LODST_MSK           0x003C0000
#define REG_TSP_PIDFLT_LODST_SHT           18


#define REG_TSP_PIDFLT_PP_CNT             4
#define REG_TSP_RCVBUF_CNT                2
#define REG_TSP_TSID_CNT                  2
#define REG_DSCMB_PATH_CNT                REG_TSP_RCVBUF_CNT




////////////////////////////////////////////////////////////////////////////////////////////////
// TSP 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP0_BANK      0x00A80      //0x1015       //0x1F202A00
#define REG_TSP_IDR_CTRL              (REG_TSP0_BANK + 0x0048)
    #define TSP_IDR_READ               0x0000
    #define TSP_IDR_START              0x0001
    #define TSP_IDR_WRITE              0x0002
#define REG_TSP_IDR_ADDR_L            (REG_TSP0_BANK + 0x0049)
#define REG_TSP_IDR_ADDR_H            (REG_TSP0_BANK + 0x004a)
#define REG_TSP_IDR_WRITE_L           (REG_TSP0_BANK + 0x004b)
#define REG_TSP_IDR_WRITE_H           (REG_TSP0_BANK + 0x004c)
#define REG_TSP_IDR_READ_L            (REG_TSP0_BANK + 0x004d)
#define REG_TSP_IDR_READ_H            (REG_TSP0_BANK + 0x004e)

////////////////////////////////////////////////////////////////////////////////////////////////
// TSP 1
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP1_BANK    0x00B00 //TSP1 0x1016

////////////////////////////////////////////////////////////////////////////////////////////////
// TSP 3
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP3_BANK      0x38100      //0x1702       //0x0xBF2E0400
#define REG_TSP_SPD_BYPASS               (REG_TSP3_BANK + 0x0004)
    #define REG_TSP_SPD_TSIF0_BYPASS_EN   0x0010
    #define REG_TSP_SPD_TSIF1_BYPASS_EN   0x0020
#define REG_TSP_RCV_BUF_SRC              (REG_TSP3_BANK + 0x0011)
    #define REG_TSP_RCV_BUF0_SRC_MSK      0x0003    //Receive BUF0 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb
    #define REG_TSP_RCV_BUF1_SRC_MSK      0x000c    //Receive BUF1 input Stream source selection 00: pkt_merge0 01: pkt_merge1 10: Dscrmb


////////////////////////////////////////////////////////////////////////////////////////////////
// TSP 4
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP4_BANK      0x38180      //0x1703       //0xBF2E0600
#define REG_TSP_CAVID0                      (REG_TSP4_BANK + 0x000a)
    #define REG_TSP_PIDFLT_BF0_CAVID_MSK     0x001F
    #define REG_TSP_PIDFLT_BF1_CAVID_MSK     0x03E0
#define REG_TSP_CLEAR_PIDPAIR_STATUS0       (REG_TSP4_BANK + 0x000d)
#define REG_TSP_SPD_RESET                   (REG_TSP4_BANK + 0x001E)
    #define REG_TSP_SPD_TSIF0_RESET          0x1000
    #define REG_TSP_SPD_TSIF1_RESET          0x2000


#endif // #ifndef __REG_DSCMB_H__
