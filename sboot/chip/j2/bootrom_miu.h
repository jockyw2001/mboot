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
#define ENABLE_MIU1 1
#undef  ENABLE_MEM1600MHz
#undef  ENABLE_G3D_PLL

#define ENABLE_MEM1600MHz 0
#define ENABLE_MEM1300MHz 1

#define WREG16(a, v) {CHIPCFG_CMD_BASE, (0xbf000000 + ((a) << 1)) >> 16}, {(0xbf000000 + ((a) << 1)) & 0xffffUL, v}
#define WREG32(a, v) {CHIPCFG_CMD_BASE, (a) >> 16}, {(a) & 0xffffUL, v}

#define DEBUG(v) {CHIPCFG_CMD_DBG, v}
#define WAIT(v)  {CHIPCFG_CMD_WAIT, v}

    DEBUG(0x0020),

    // MIU0 Software reset
    WREG16(0x10121e, 0x0000),   //hold miu_0
    WREG16(0x10121e, 0x0c01),   //miu_sw_rst
    WREG16(0x10121e, 0x0e00),   //
    WREG16(0x101202, 0x0000),   //disable all ddr bus out

    // MIU0 Request mask
    WREG16(0x1012F8, 0x8000),
    WREG16(0x101246, 0xFFFE),   // mask group0
    WREG16(0x101266, 0xFFFF),   // mask group1
    WREG16(0x101286, 0xFFFF),   // mask group2
    WREG16(0x1012A6, 0xFFFF),   // mask group3

#if ( ENABLE_MIU1 == 1 )
    // MIU1 Software reset
    WREG16(0x10061e, 0x0000),   //hold miu_0
    WREG16(0x10061e, 0x0c01),   //
    WREG16(0x10061e, 0x0e00),   //
    WREG16(0x100602, 0x0000),   //

    // MIU1 Request mask
    WREG16(0x1006F8, 0x8000),
    WREG16(0x100646, 0xFFFE),   // mask group0
    WREG16(0x100666, 0xFFFF),   // mask group1
    WREG16(0x100686, 0xFFFF),   // mask group2
    WREG16(0x1006A6, 0xFFFF),   // mask group3
#endif

    // lui     $4,%hi(MIU0_DDR_Init)
    // bal     MDrv_WriteRegTbl32
    // ori     $4,%lo(MIU0_DDR_Init)
#include "miu0.h"

#if (ENABLE_G3D_PLL == 1)
    WREG16(0x1012A0, 0xC004),   // insert
    WREG16(0x1012A2, 0x0002),   // insert

    WREG16(0x1006A0, 0xC004),   // insert
    WREG16(0x1006A2, 0x0002),   // insert
#endif

    DEBUG(0x0021),

    // Enable MIU0
    WREG16(0x10121e, 0x0c01),
    WREG16(0x10121e, 0x0c00),

    WAIT(100),    // delay 100us

    // 8x mode:0xAAAA , 4x mode :0xCCCC
#if (ENABLE_MEM1600MHz)
    WREG16(0x110d00, 0x0401),
#else
    WREG16(0x110d00, 0x0601),
#endif
    WREG16(0x110d02, 0xAAAA),

    DEBUG(0x0023),

#if ( ENABLE_MIU1 == 1 )

#include "miu1.h"
    // lui     $4,%hi(MIU1_DDR_Init)
    // bal     MDrv_WriteRegTbl32
    // ori     $4,%lo(MIU1_DDR_Init)

    // Enable MIU1
    WREG16(0x10061e, 0x0c01),
    WREG16(0x10061e, 0x0c00),

    WAIT(100),    // delay 100us

    // 8x mode:0xAAAA , 4x mode :0xCCCC
#if (ENABLE_MEM1600MHz)
    WREG16(0x110d80, 0x0401),
#else
    WREG16(0x110d80, 0x0601),
#endif
    WREG16(0x110d82, 0xAAAA),
#endif

    DEBUG(0x0024),

    // rx enable
    WREG16(0x110d08, 0x003f),

    // MIU0 INIT Cycle
    WREG16(0x101200, 0x0000), // hold miu_0
    WREG16(0x101200, 0x0008), // DRAM reset
    WREG16(0x101200, 0x000c), // DRAM reset+CS
    WAIT(300),    // delay 300us
    WREG16(0x101200, 0x000e), // DRAM reset+CS+CKE
    WAIT(500),    // delay 500us
    WREG16(0x101200, 0x001f), // DRAM reset+CS+CKE+auto cycle+ODE
    DEBUG(0x0025),

    WAIT(500),    // delay 500us
    DEBUG(0x0026),

#if ( ENABLE_MIU1 == 1 )
    // MIU1 INIT Cycle
    WREG16(0x100600, 0x0000), // hold miu_0
    WREG16(0x100600, 0x0008), // DRAM reset
    WREG16(0x100600, 0x000c), // DRAM reset+CS
    WAIT(300),    // delay 300us
    WREG16(0x100600, 0x000e), // DRAM reset+CS+CKE
    WAIT(500),    // delay 500us
    WREG16(0x100600, 0x001f), // DRAM reset+CS+CKE+auto cycle+ODE
    WAIT(500),    // delay 500us
#endif

    DEBUG(0x0027),

    DEBUG(0x0028),

    // open all MIU0 request mask (All IP can access MIU resource)
    WREG16(0x101246, 0x0000),   //Mask MIU0_group_0
    WREG16(0x101266, 0x0000),   //Mask MIU0_group_1
    WREG16(0x101286, 0x0000),   //Mask MIU0_group_2
    WREG16(0x1012a6, 0x0000),   //Mask MIU0_group_3

#if ( ENABLE_MIU1 == 1 )
    // open all MIU1 request mask (All IP can access MIU resource)
    WREG16(0x100646, 0x0000),   //Mask MIU0_group_0
    WREG16(0x100666, 0x0000),   //Mask MIU0_group_1
    WREG16(0x100686, 0x0000),   //Mask MIU0_group_2
    WREG16(0x1006a6, 0x0000),   //Mask MIU0_group_3

    // all MIU1 IP select MIU0 in bringup
    WREG16(0x1006f0, 0x4088),
    WREG16(0x1006f2, 0x0000),
    WREG16(0x1006f4, 0x0000),
    WREG16(0x1006f6, 0x0000),
#endif

    WREG16(0x10121e, 0x0c08),   // SW initial done and turn on arbitor


    DEBUG(0x0029),
    WREG16(0x1012e0, 0x0000),  // stop bist test

    DEBUG(0x0030),

    WREG32(0xbf203084, 0x0100),        ////Set MIU0 Size of MAU0
    WREG32(0xbf2030c4, 0x0100),        ////Set MIU0 Size of MAU1
