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
//swch 4

// [Notice]
// 1. GE clock is OFF !
// 2. ZDEC clock is OFF !
// 3. 1st_GMAC AHB clock is ON / 216MHz !
// 4. 2nd_GMAC AHB clock is ON / 144MHz !
// 5. clk_usb20_p0/p1/p2_240m is OFF !
// 6. TSP* clock is OFF !
// 7. VE clock is OFF !
// 8. HDGEN(YPbPr) clock is OFF !
// 9. Vivalid9 mcu clock is ON / 216MHz.
// 10. SCx clock is OFF.
// 11. Periperial Ips setting
// => IICM* clock is OFF !
// => fuar0 clock is ON/fuart_syn(432MHz)
// => uart0 clock is ON/123MHz
// => uart1 clock is ON/123MHz
// => mspi* clock is OFF !

// fuart
WREG(REG_ADDR_BASE + (0x100b2e << 1), 0x0300),

// uart0 @ 144MHz
//WREG_B((REG_ADDR_BASE + (0x100b26 <<1)+1) , 0x0c),
WREG_B((REG_ADDR_BASE + (0x100b26 <<1)+1) , 0x08),

// uart1 @ 144MHz
//WREG_B((REG_ADDR_BASE + (0x100b28 <<1)) , 0x0c),
WREG_B((REG_ADDR_BASE + (0x100b28 <<1)) , 0x08),

// select clk_miu_p from miupll
WREG(REG_ADDR_BASE + (0x100b3e << 1), 0x0008),

// select clk_mcu_p1 = 216MHz
WREG(REG_ADDR_BASE + (0x100b20 << 1), 0x0000),

// select clk_mcu from xtal to clk_mcu_p1
WREG(REG_ADDR_BASE + (0x100b20 << 1), 0x0001),

// set clk_ddr_syn @ 432MHz
WREG_B((REG_ADDR_BASE + (0x100bfa <<1)) , 0x04),

// set PM 8051 clock @ 170MHz
//WREG(REG_ADDR_BASE + (0x000e40 << 1), 0x8000),

// set clk_tsp_p @ 192MHz; [0]=0 => ON; [4:2]=0 => 192MHz
WREG_B((REG_ADDR_BASE + (0x100b54 <<1)) , 0x00),

// turn on clk_dac1 & clk_dac2; [0]=0 => ON; [3:2]=2 => xtal clock (12MHz)
WREG(REG_ADDR_BASE + (0x100bb2 << 1), 0x0808),

// set clk_spi @ 86MHz; [0]=0 => ON; [5:2]=6 => 86MHz; [6]=1 => switch from xtal to 86MHz
WREG_B((REG_ADDR_BASE + (0x100b2c <<1)) , 0x10),
WREG_B((REG_ADDR_BASE + (0x100b2c <<1)) , 0x58),

// set clk_r2_secure_p @ 288MHz; [0]=0 => ON; [4:2]=3 => 288MHz; [5]=1 => switch from xtal to 288MHz
WREG_B((REG_ADDR_BASE + (0x100b62 <<1)) , 0x0c),
WREG_B((REG_ADDR_BASE + (0x100b62 <<1)) , 0x2c),

// turn off un-necessary clocks while init; [0]=1 => stc0/2 OFF; [4]=1 => stc1/3 OFF
// turn off clk_stc0_p & clk_stc1_p
WREG_B((REG_ADDR_BASE + (0x100b54 <<1)+1) , 0x11),
// turn off clk_stc2_p & clk_stc3_p
WREG_B((REG_ADDR_BASE + (0x100a9e <<1)+1) , 0x11),

//BDMA
WREG(REG_ADDR_BASE + (0x100bcc << 1), 0x0004),

//reg_ckg_mpll_syn@001d[4:2] ; 432 MHz
WREG(REG_ADDR_BASE + (0x100b3a << 1) , 0x0404),

//secgmac
WREG_B((REG_ADDR_BASE + (0x103364 <<1)) , 0x08),
