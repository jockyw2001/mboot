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


#include "boot.h"

const Integer2048 _sig0 _SECTION_SIG_0_ = {0};
const Integer2048 _sig1 _SECTION_SIG_1_ = {0};
const Integer2048 _sig2 _SECTION_SIG_2_ = {0};

typedef struct _EXT_Key
{
    // 0x0000
    Integer2048 rsa_n_ext;    // max 2048 bit
    // 0x0100
    Integer32   rsa_e_ext;    // max 32   bit
    // 0x0104-0x010F
    MS_U32      reserved[3];
} EXT_Key;


typedef struct _EXT_Param
{
    struct
    {
        MS_U16 addr;
        MS_U16 value;
    } chipcfg[1023];
} EXT_Param;


const EXT_Key _hash0 _SECTION_HASH_0_ =
{
    {                                                                   // rsa_public_n_ext
        0xA5, 0x8C, 0x1A, 0xBF, 0x7D, 0xD1, 0xAF, 0xF7, 0xAB, 0xE3, 0xFF, 0xCF, 0x45, 0x63, 0xE5, 0x2E,
        0x41, 0x09, 0xC6, 0x31, 0xBC, 0x34, 0xCB, 0x13, 0xEA, 0x3E, 0xA4, 0x18, 0xAB, 0xE9, 0x67, 0xC6,
        0xC6, 0x9E, 0xE7, 0xEC, 0xC7, 0xF0, 0x5B, 0xFC, 0x54, 0x5A, 0xE7, 0x63, 0x37, 0x8F, 0x8D, 0x49,
        0x5F, 0x2F, 0x5D, 0xA5, 0x09, 0xE2, 0x70, 0x21, 0x44, 0x50, 0x01, 0x01, 0xA4, 0xC2, 0xB4, 0x1F,
        0x0E, 0x06, 0xE9, 0x70, 0xD8, 0xB7, 0xAF, 0xAB, 0x30, 0xDD, 0x72, 0xAB, 0x3D, 0x46, 0x66, 0xAA,
        0x1A, 0x8F, 0x7C, 0x94, 0xA8, 0x11, 0xED, 0x66, 0x44, 0x0C, 0x41, 0x55, 0x90, 0x05, 0x7A, 0xC6,
        0x91, 0x45, 0xB6, 0x3A, 0xD5, 0x10, 0x1A, 0x18, 0x61, 0x43, 0x67, 0x21, 0xDD, 0x5A, 0x2E, 0x27,
        0x1C, 0x17, 0x60, 0xC3, 0x4B, 0x4E, 0x75, 0xAD, 0x76, 0xDB, 0x3C, 0x33, 0x43, 0x52, 0x6A, 0x56,
        0x53, 0x30, 0xDD, 0x92, 0xF1, 0x1C, 0x49, 0x3E, 0x4B, 0xE8, 0xBF, 0xF0, 0xC7, 0x88, 0x8E, 0xBC,
        0x00, 0x78, 0xEB, 0xAD, 0x04, 0x77, 0xB4, 0xA5, 0x79, 0x2F, 0x4D, 0xAB, 0x6E, 0x2B, 0x31, 0x79,
        0x2F, 0x20, 0x77, 0xC8, 0xAE, 0x15, 0x96, 0xAF, 0xCF, 0x1F, 0x6F, 0xE2, 0xD1, 0xBF, 0x08, 0x76,
        0x13, 0xDD, 0x17, 0xDB, 0xD3, 0x13, 0x98, 0x4C, 0x7D, 0x9C, 0xF8, 0xAD, 0x1F, 0xB2, 0x75, 0x71,
        0x6F, 0xA3, 0x57, 0x93, 0xA9, 0xC9, 0xCE, 0xD8, 0xDC, 0xBA, 0x6C, 0x6A, 0xC9, 0xED, 0x34, 0xC9,
        0xDF, 0xDB, 0x98, 0xF0, 0x78, 0xCF, 0x05, 0x3F, 0x67, 0xE4, 0x43, 0x3E, 0xE3, 0xA3, 0xFD, 0xE2,
        0x73, 0x51, 0x75, 0x96, 0xE4, 0x34, 0x4D, 0x41, 0xDF, 0x67, 0x38, 0x84, 0x63, 0x64, 0xB3, 0xDA,
        0x28, 0x50, 0xEA, 0x8C, 0xB9, 0xC3, 0x58, 0xBA, 0x6D, 0xBF, 0x68, 0xAC, 0x7F, 0x9C, 0x83, 0x61,
    },

    {                                                                   // rsa_public_e_ext
        0x00, 0x01, 0x00, 0x01,
    },

    0x00000000, // Market segment ID
    0x00000000, 0x00000000                                  // reserved
};


// {CHIPCFG_CMD_DBG,  0xF010},
// {CHIPCFG_CMD_BASE, 0xbf20},
// {CHIPCFG_CMD_WAIT, 50},
// {CHIPCFG_CMD_END,  0xFFFF},
const EXT_Param _hash1 _SECTION_HASH_1_ =
{
    {
#include "bootrom_miu.h"
        {CHIPCFG_CMD_WAIT,  50},
        {CHIPCFG_CMD_END,  0xFFFF}
    }
};
