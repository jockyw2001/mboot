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

#include "CusSataUsbTest.h"

// SSC setup
static MI_SysWriteReg_t aSATA_ENABLE_SSC[7] = {
	//{0x103964, 0x08},
	//{0x103965, 0x30},
	//{0x103960, 0x2f},
	//{0x103961, 0xdd},
	//{0x103962, 0x24},
	//{0x103966, 0x0d},
	//{0x103967, 0x03},
	{0x103964, 0x07}, 	// STEP1[7:0]
	{0x103965, 0x30},	// STEP1[10:8]
	{0x103960, 0x0a},	// SET[7:0]
	{0x103961, 0xdf},	// SET[15:8]
	{0x103962, 0x24},	// SET[23:16]
	{0x103966, 0xe8},	// SPAN[7:0]
	{0x103967, 0x02}	// SPAN[14:8]
};

//>>> Program pattern as HFTP
static MI_SysWriteReg_t aSATA_HFTP[8] = {
	{0x103934, 0x4a},
	{0x103935, 0x4a},
	{0x103936, 0x4a},
	{0x103937, 0x4a},
	{0x103938, 0x4a},
	{0x103939, 0x4a},
	{0x10393a, 0x4a},
	{0x10393b, 0x4a}
};

//>>> Program pattern as LBP
static MI_SysWriteReg_t aSATA_LBP[8] = {
	{0x103934, 0x0c},
	{0x103935, 0x8b},
	{0x103936, 0x0c},
	{0x103937, 0x6b},
	{0x103938, 0x0c},
	{0x103939, 0x8b},
	{0x10393a, 0x0c},
	{0x10393b, 0x6b}
};

//>>> Program pattern as LFTP
static MI_SysWriteReg_t aSATA_LFTP[8] = {
	{0x103934, 0x7e},
	{0x103935, 0x7e},
	{0x103936, 0x7e},
	{0x103937, 0x7e},
	{0x103938, 0x7e},
	{0x103939, 0x7e},
	{0x10393a, 0x7e},
	{0x10393b, 0x7e}
};

//>>> Program pattern as MFTP
static MI_SysWriteReg_t aSATA_MFTP[8] = {
	{0x103934, 0x78},
	{0x103935, 0x78},
	{0x103936, 0x78},
	{0x103937, 0x78},
	{0x103938, 0x78},
	{0x103939, 0x78},
	{0x10393a, 0x78},
	{0x10393b, 0x78}
};

//>>> Program pattern as SSOP
static MI_SysWriteReg_t aSATA_SSOP[8] = {
	{0x103934, 0x7f},
	{0x103935, 0x7f},
	{0x103936, 0x7f},
	{0x103937, 0x7f},
	{0x103938, 0x7f},
	{0x103939, 0x7f},
	{0x10393a, 0x7f},
	{0x10393b, 0x7f}
};

static MI_SysWriteReg_t aSATA_TX_PATTERN_GEN1[67] = {
	// Initial
	{0x103922, 0x00},
	{0x103923, 0x00},
	{0x103924, 0x00},
	{0x103925, 0x00},
	{0x10390c, 0x04},
	{0x10390d, 0x83},
	{0x10390e, 0x00},
	{0x10390f, 0x02},
	{0x103910, 0x00},
	{0x103911, 0x00},
	{0x10390a, 0xb8},
	{0x10390b, 0x08},
	{0x103902, 0x87},
	{0x103903, 0x80},
	{0x103902, 0x8e},
	{0x103903, 0x80},
	{0x103919, 0x20},
	{0x10397c, 0x40},
	{0x10392d, 0xb9},
	{0x103920, 0x02},
	{0x103925, 0x00},
	{0x10397c, 0xe9},	//enable test mode
	// Wait TXPLL lock
	//>>> Program pattern as HFTP
	{0x103934, 0x4a},
	{0x103935, 0x4a},
	{0x103936, 0x4a},
	{0x103937, 0x4a},
	{0x103938, 0x4a},
	{0x103939, 0x4a},
	{0x10393a, 0x4a},
	{0x10393b, 0x4a},
	//>>> Program pattern as MFTP
	{0x103934, 0x78},
	{0x103935, 0x78},
	{0x103936, 0x78},
	{0x103937, 0x78},
	{0x103938, 0x78},
	{0x103939, 0x78},
	{0x10393a, 0x78},
	{0x10393b, 0x78},
	//>>> Program pattern as LFTP
	{0x103934, 0x7e},
	{0x103935, 0x7e},
	{0x103936, 0x7e},
	{0x103937, 0x7e},
	{0x103938, 0x7e},
	{0x103939, 0x7e},
	{0x10393a, 0x7e},
	{0x10393b, 0x7e},
	//>>> Program pattern as LBP
	{0x103934, 0x0c},
	{0x103935, 0x8b},
	{0x103936, 0x0c},
	{0x103937, 0x6b},
	{0x103938, 0x0c},
	{0x103939, 0x8b},
	{0x10393a, 0x0c},
	{0x10393b, 0x6b},
	//>>> Program pattern as SSOP
	{0x103934, 0x7f},
	{0x103935, 0x7f},
	{0x103936, 0x7f},
	{0x103937, 0x7f},
	{0x103938, 0x7f},
	{0x103939, 0x7f},
	{0x10393a, 0x7f},
	{0x10393b, 0x7f},
	//add by chinglin
	//TX swing
	//{0x103994 0x31},
	//{0x103995 0x20},
	//{0x103996 0x21},
	//{0x103997 0x38},
	//TX swing     1.9db
	//{0x103994 0x31}, => For socketboard_Neil
	{0x103994, 0x2B},	//For 15x15 modification_Neil
	{0x103995, 0x20},
	{0x103996, 0x03},
	{0x103997, 0x38},
	//TX rterm
	{0x103928, 0x1c}
};

static MI_SysWriteReg_t aSATA_TX_PATTERN_GEN2[67] = {
	// Initial
	{0x103922, 0x00},
	{0x103923, 0x00},
	{0x103924, 0x00},
	{0x103925, 0x00},
	{0x10390c, 0x04},
	{0x10390d, 0x83},
	{0x10390e, 0x00},
	{0x10390f, 0x02},
	{0x103910, 0x00},
	{0x103911, 0x00},
	{0x10390a, 0xb8},
	{0x10390b, 0x08},
	{0x103902, 0x87},
	{0x103903, 0x80},
	{0x103902, 0x8e},
	{0x103903, 0x80},
	{0x103919, 0x20},
	{0x10397c, 0x40},
	{0x10392d, 0xb9},
	{0x103920, 0x02},
	{0x103925, 0x40},
	{0x10397c, 0xe9},	//enable test mode
	// Wait TXPLL lock
	//>>> Program pattern as HFTP
	{0x103934, 0x4a},
	{0x103935, 0x4a},
	{0x103936, 0x4a},
	{0x103937, 0x4a},
	{0x103938, 0x4a},
	{0x103939, 0x4a},
	{0x10393a, 0x4a},
	{0x10393b, 0x4a},
	//>>> Program pattern as MFTP
	{0x103934, 0x78},
	{0x103935, 0x78},
	{0x103936, 0x78},
	{0x103937, 0x78},
	{0x103938, 0x78},
	{0x103939, 0x78},
	{0x10393a, 0x78},
	{0x10393b, 0x78},
	//>>> Program pattern as LFTP
	{0x103934, 0x7e},
	{0x103935, 0x7e},
	{0x103936, 0x7e},
	{0x103937, 0x7e},
	{0x103938, 0x7e},
	{0x103939, 0x7e},
	{0x10393a, 0x7e},
	{0x10393b, 0x7e},
	//>>> Program pattern as LBP
	{0x103934, 0x0c},
	{0x103935, 0x8b},
	{0x103936, 0x0c},
	{0x103937, 0x6b},
	{0x103938, 0x0c},
	{0x103939, 0x8b},
	{0x10393a, 0x0c},
	{0x10393b, 0x6b},
	//>>> Program pattern as SSOP
	{0x103934, 0x7f},
	{0x103935, 0x7f},
	{0x103936, 0x7f},
	{0x103937, 0x7f},
	{0x103938, 0x7f},
	{0x103939, 0x7f},
	{0x10393a, 0x7f},
	{0x10393b, 0x7f},
	//add by chinglin
	//TX swing
	//{0x103994 0x31},
	//{0x103995 0x20},
	//{0x103996 0x21},
	//{0x103997 0x38},
	//TX swing     1.9db
	//{0x103994 0x35}, => For socketboard_Neil
	{0x103994, 0x33},	//For 15x15 modification_Neil
	{0x103995, 0x39},
	{0x103996, 0x20},
	{0x103997, 0x39},
	//TX rterm
	{0x103928, 0x1c}
};

static MI_SysWriteReg_t aUSB_TSTPG_HS[121] = {
	{0x0010080e, 0x01},
	{0x00113700, 0xff},
	{0x00113702, 0x00},
	{0x00113720, 0xff},
	{0x00113722, 0x00},
	{0x00113740, 0xff},
	{0x00113742, 0x00},
	{0x00100800, 0xb0},
	{0x00100801, 0x10},
	{0x00100820, 0xb0},
	{0x00100821, 0x10},
	{0x00100804, 0x10},
	{0x00100805, 0x01},
	{0x00100824, 0x10},
	{0x00100825, 0x01},
	{0x00103a88, 0x0f},
	{0x00103a89, 0x04},
	{0x00103a80, 0x05},
	{0x00103a81, 0x7f},
	{0x00103a08, 0x0f},
	{0x00103a09, 0x04},
	{0x00103a00, 0x05},
	{0x00103a01, 0x7f},
	{0x00103808, 0x0f},
	{0x00103809, 0x04},
	{0x00103800, 0x05},
	{0x00103801, 0x7f},
	{0x001012fe, 0xe1},
	{0x001012ff, 0x08},
	{0x00103a88, 0x0f},
	{0x00103a89, 0x04},
	{0x00103aa0, 0xa1},
	{0x00103aa1, 0x80},
	{0x00103aa2, 0x88},
	{0x00103aa3, 0x20},
	{0x00103a80, 0x03},
	{0x00103a81, 0x6b},
	{0x00103a08, 0x0f},
	{0x00103a09, 0x04},
	{0x00103a20, 0xa1},
	{0x00103a21, 0x80},
	{0x00103a22, 0x88},
	{0x00103a23, 0x20},
	{0x00103a00, 0x03},
	{0x00103a01, 0x6b},
	{0x00103a2c, 0x2f},
	{0x00103aac, 0x2f},
	{0x00103808, 0x0f},
	{0x00103809, 0x04},
	{0x00103820, 0xa1},
	{0x00103821, 0x80},
	{0x00103822, 0x88},
	{0x00103823, 0x20},
	{0x00103800, 0x03},
	{0x00103801, 0x6b},
	//TE WAIT 1ms
	{0x00103a80, 0xc3},
	{0x00103a81, 0x69},
	{0x00103a00, 0xc3},
	{0x00103a01, 0x69},
	{0x00103800, 0xc3},
	{0x00103801, 0x69},
	//TE WAIT 2ms
	{0x00103a80, 0x01},
	{0x00103a81, 0x00},
	{0x00103abc, 0x01},
	{0x00103abd, 0x00},
	{0x00103abc, 0x00},
	{0x00103abd, 0x00},
	{0x00103a00, 0x01},
	{0x00103a01, 0x00},
	{0x00103a3c, 0x01},
	{0x00103a3d, 0x00},
	{0x00103a3c, 0x00},
	{0x00103a3d, 0x00},
	{0x00103800, 0x01},
	{0x00103801, 0x00},
	{0x0010383c, 0x01},
	{0x0010383d, 0x00},
	{0x0010383c, 0x00},
	{0x0010383d, 0x00},
	//TE WAIT 2ms
	{0x00103a90, 0x78},
	{0x00103a91, 0x00},
	{0x00103a86, 0x43},
	{0x00103a87, 0x00},
	{0x00103a86, 0x40},
	{0x00103a87, 0x00},
	{0x00103a80, 0x01},
	{0x00103a81, 0x00},
	{0x00103a94, 0x00},
	{0x00103a95, 0x06},
	{0x00103ab4, 0x00},
	{0x00103ab5, 0x00},
	{0x00103ab2, 0xfe},
	{0x00103ab3, 0x0b},
	{0x00103a10, 0x78},
	{0x00103a11, 0x00},
	{0x00103a06, 0x43},
	{0x00103a07, 0x00},
	{0x00103a06, 0x40},
	{0x00103a07, 0x00},
	{0x00103a00, 0x01},
	{0x00103a01, 0x00},
	{0x00103a14, 0x00},
	{0x00103a15, 0x06},
	{0x00103a34, 0x00},
	{0x00103a35, 0x00},
	{0x00103a32, 0xfe},
	{0x00103a33, 0x0b},
	{0x00103810, 0x78},
	{0x00103811, 0x00},
	{0x00103806, 0x43},
	{0x00103807, 0x00},
	{0x00103806, 0x40},
	{0x00103807, 0x00},
	{0x00103800, 0x01},
	{0x00103801, 0x00},
	{0x00103814, 0x00},
	{0x00103815, 0x06},
	{0x00103834, 0x00},
	{0x00103835, 0x00},
	{0x00103832, 0xfe},
	{0x00103833, 0x0b}
};

static MI_SysWriteReg_t aUSB_TSTPG_FS[125] = {
	{0x0010080e, 0x01},
	{0x00113700, 0xff},
	{0x00113702, 0x00},
	{0x00113720, 0xff},
	{0x00113722, 0x00},
	{0x00113740, 0xff},
	{0x00113742, 0x00},
	{0x00100800, 0xb0},
	{0x00100801, 0x10},
	{0x00100820, 0xb0},
	{0x00100821, 0x10},
	{0x00100804, 0x10},
	{0x00100805, 0x01},
	{0x00100824, 0x10},
	{0x00100825, 0x01},
	{0x00103a88, 0x0f},
	{0x00103a89, 0x04},
	{0x00103a80, 0x05},
	{0x00103a81, 0x7f},
	{0x00103a08, 0x0f},
	{0x00103a09, 0x04},
	{0x00103a00, 0x05},
	{0x00103a01, 0x7f},
	{0x00103808, 0x0f},
	{0x00103809, 0x04},
	{0x00103800, 0x05},
	{0x00103801, 0x7f},
	{0x001012fe, 0xe1},
	{0x001012ff, 0x08},
	{0x00103a88, 0x0f},
	{0x00103a89, 0x04},
	{0x00103aa0, 0xa1},
	{0x00103aa1, 0x80},
	{0x00103aa2, 0x88},
	{0x00103aa3, 0x20},
	{0x00103a80, 0xc3},
	{0x00103a81, 0x69},
	{0x00103a82, 0x84},
	{0x00103a83, 0x90},
	{0x00103a80, 0x01},
	{0x00103a81, 0x02},
	{0x00103a80, 0x01},
	{0x00103a81, 0x00},
	{0x00103a08, 0x0f},
	{0x00103a09, 0x04},
	{0x00103a20, 0xa1},
	{0x00103a21, 0x80},
	{0x00103a22, 0x88},
	{0x00103a23, 0x20},
	{0x00103a00, 0xc3},
	{0x00103a01, 0x69},
	{0x00103a02, 0x84},
	{0x00103a03, 0x90},
	{0x00103a00, 0x01},
	{0x00103a01, 0x02},
	{0x00103a00, 0x01},
	{0x00103a01, 0x00},
	{0x00103808, 0x0f},
	{0x00103809, 0x04},
	{0x00103820, 0xa1},
	{0x00103821, 0x80},
	{0x00103822, 0x88},
	{0x00103823, 0x20},
	{0x00103800, 0xc3},
	{0x00103801, 0x69},
	{0x00103802, 0x84},
	{0x00103803, 0x90},
	{0x00103800, 0x01},
	{0x00103801, 0x02},
	{0x00103800, 0x01},
	{0x00103801, 0x00},
	//TE WAIT 1ms
	{0x00103abc, 0x01},
	{0x00103abd, 0x00},
	{0x00103abc, 0x00},
	{0x00103abd, 0x00},
	{0x00103a3c, 0x01},
	{0x00103a3d, 0x00},
	{0x00103a3c, 0x00},
	{0x00103a3d, 0x00},
	{0x0010383c, 0x01},
	{0x0010383d, 0x00},
	{0x0010383c, 0x00},
	{0x0010383d, 0x00},
	//TE WAIT 5ms
	{0x00103a90, 0x78},
	{0x00103a91, 0x00},
	{0x00103a86, 0x43},
	{0x00103a87, 0x00},
	{0x00103a86, 0x40},
	{0x00103a87, 0x00},
	{0x00103a80, 0xeb},
	{0x00103a81, 0x00},
	{0x00103a94, 0x40},
	{0x00103a95, 0x07},
	{0x00103ab4, 0x00},
	{0x00103ab5, 0x00},
	{0x00103ab2, 0xfe},
	{0x00103ab3, 0x0b},
	{0x00103a10, 0x78},
	{0x00103a11, 0x00},
	{0x00103a06, 0x43},
	{0x00103a07, 0x00},
	{0x00103a06, 0x40},
	{0x00103a07, 0x00},
	{0x00103a00, 0xeb},
	{0x00103a01, 0x00},
	{0x00103a14, 0x40},
	{0x00103a15, 0x07},
	{0x00103a34, 0x00},
	{0x00103a35, 0x00},
	{0x00103a32, 0xfe},
	{0x00103a33, 0x0b},
	{0x00103810, 0x78},
	{0x00103811, 0x00},
	{0x00103806, 0x43},
	{0x00103807, 0x00},
	{0x00103806, 0x40},
	{0x00103807, 0x00},
	{0x00103800, 0xeb},
	{0x00103801, 0x00},
	{0x00103814, 0x40},
	{0x00103815, 0x07},
	{0x00103834, 0x00},
	{0x00103835, 0x00},
	{0x00103832, 0xfe},
	{0x00103833, 0x0b}
};

void MI_DEMO_Help(char *str)
{
	printf("\tUsage: %s [sata sataPattern] [usb usbPattern] [help]\n", str);
	printf("\tFor example: %s sata 0\n\n", str);
    printf("\t help: Display help info\n");
    printf("\t sata: SATA TEST \n");
	printf("\t\t 0: SATA_ENABLE_SSC \n");
	printf("\t\t 1: SATA_HFTP \n");
	printf("\t\t 2: SATA_LBP \n");
	printf("\t\t 3: SATA_LFTP \n");
	printf("\t\t 4: SATA_MFTP \n");
	printf("\t\t 5: SATA_SSOP \n");
	printf("\t\t 6: SATA_TX_PATTERN_GEN1 \n");
	printf("\t\t 7: SATA_TX_PATTERN_GEN2 \n");
    printf("\t usb: USB TEST \n");
	printf("\t\t 0: USB_TSTPG_HS \n");
	printf("\t\t 1: USB_TSTPG_FS \n");

	return;
}



int MI_DEMO_WRIU(MI_SysWriteReg_t *pwrReg, int n)
{
	int i;

	for(i = 0; i < n; i++) {

		WriteByte(pwrReg[i].u32WriteRegAddr, pwrReg[i].u8WriteRegValue);
        UBOOT_INFO("[%x]:0x%2x\n",(unsigned int)pwrReg[i].u32WriteRegAddr,(unsigned int)ReadByte(pwrReg[i].u32WriteRegAddr));

        //usleep(100000);
	}

	return 0;
}

int MI_DEMO_RDIU(MI_SysReadReg_t *prdReg, int n)
{
	int i;

	for(i = 0; i < n; i++) {
		UBOOT_INFO("[%x]:0x%2x\n",(unsigned int)prdReg[i].u32ReadRegAddr,(unsigned int)ReadByte(prdReg[i].u32ReadRegAddr));
        //usleep(100000);
	}

	return 0;
}

void MI_DEMO_SATATest(MI_SataPattern_e sataPattern)
{
    printf("[%s:%d] sataPattern: %d\n", __FUNCTION__, __LINE__, sataPattern);

	switch(sataPattern) {
		case SATA_ENABLE_SSC:
			MI_DEMO_WRIU(aSATA_ENABLE_SSC, 7);
			break;
		case SATA_HFTP:
			MI_DEMO_WRIU(aSATA_HFTP, 8);
			break;
		case SATA_LBP:
			MI_DEMO_WRIU(aSATA_LBP, 8);
			break;
		case SATA_LFTP:
			MI_DEMO_WRIU(aSATA_LFTP, 8);
			break;
		case SATA_MFTP:
			MI_DEMO_WRIU(aSATA_MFTP, 8);
			break;
		case SATA_SSOP:
			MI_DEMO_WRIU(aSATA_SSOP, 8);
			break;
		case SATA_TX_PATTERN_GEN1:
			MI_DEMO_WRIU(aSATA_TX_PATTERN_GEN1, 67);
			break;
		case SATA_TX_PATTERN_GEN2:
			MI_DEMO_WRIU(aSATA_TX_PATTERN_GEN2, 67);
			break;
		default:
			printf("WARN: invalid parameter!\n");
			break;
	}

	return;
}

void MI_DEMO_USBTest(MI_USBPattern_e usbPattern)
{
    printf("[%s:%d] usbPattern: %d\n", __FUNCTION__, __LINE__, usbPattern);

	switch(usbPattern) {
		case USB_TSTPG_HS:
			MI_DEMO_WRIU(aUSB_TSTPG_HS, 121);
			break;
		case USB_TSTPG_FS:
			MI_DEMO_WRIU(aUSB_TSTPG_FS, 125);
			break;
		default:
			printf("\n");
			break;
	}

	return;
}

int do_sata_usb_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int pattern =0;

    if (argc < 2)
    {
        MI_DEMO_Help(argv[0]);
        return -1;
    }

	if(strncmp(argv[1], "sata", 5) == 0) // sata test
	{
		pattern = simple_strtoul(argv[2], NULL, 16);
		MI_DEMO_SATATest(pattern);
	}
    else if(strncmp(argv[1], "usb", 1) == 0) //usb test
    {
		pattern = simple_strtoul(argv[2], NULL, 16);
		MI_DEMO_USBTest(pattern);
    }
    else if(strncmp(argv[1], "help", 1) == 0) //usb test
    {
		MI_DEMO_Help(argv[0]);
    }
    else
    {
		MI_DEMO_Help(argv[0]);
    }

    return 0;
}
