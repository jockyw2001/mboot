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


#ifndef _CHIP_H_
#define _CHIP_H_


//------------------------------------------------------------------------------------------------
// Type Definition and Structure
//------------------------------------------------------------------------------------------------
// Debug register
#define R_DEBUG                     0x1F206700 // mailbox 0


// CA Autoload RIU
#define B_NP1_MCU                   0x1F227400
#define R_OTPCFG_BOOTTYPE           (B_NP1_MCU + (0x000e<<2))
    #define OTPCFG_BOOT_MASK                    0x03F0
    #define OTPCFG_BOOT_OTP                     0x03F0
    #define OTPCFG_BOOT_NOR_ACTIVE              0x3C00
#define R_OTPCFG_SBOOT              (B_NP1_MCU + (0x0017<<2))
    #define OTPCFG_SBOOT_ACTIVE                 0xF000

#define U_OTP_MSID                  (0xb0001f4c + 4 * 0x0b)


// UART
#define REG_UART_BASE     (0x1F201300)
#define UART_LSR_THRE     (0x20)
#define UART_LSR_TEMT     (0x40)
#define REG_UART_RX       (REG_UART_BASE + (0 * 8))
#define REG_UART_TX       (REG_UART_BASE + (0 * 8))
#define REG_UART_LSR      (REG_UART_BASE + (5 * 8))

// UART
#define REG_MOBF_MIU0_BASE              (0x1F202400)
#define REG_MOBF_MIU1_BASE              (0x1F200C00)
#define REG_MOBF_CONTROL_BASE           (0x1F201800)
#define REG_CHIP_TOP_BASE               (0x1F203C00)

// MIU BIST CONFIG
#define MIU_BIST_ADDRESS_UNIT 13 // 8KB 
#define MIU_BIST_LENGTH_UNIT 3 // 16 Bytes


#define REG_WRITE_CACHE_AU_R2_BASE      (0x1F225800)
#define REG_WRITE_CACHE_USB20_BASE      (0x1F220400)
#define REG_WRITE_CACHE_USB30_BASE      (0x1F244000)
#define REG_WRITE_CACHE_USB20_0_BASE    (0x1F200E00)
#define REG_WRITE_CACHE_USB20_1_BASE    (0x1F200E00)
#define REG_WRITE_CACHE_USB20_1_BASE    (0x1F200E00)

// HASH1 TABLE LENGTH
#define HASH1_PARAM_LEN   1022




//-------------------------------------------------------------------------------------------------
// Global Variable
//-------------------------------------------------------------------------------------------------


#endif // _CHIP_H_

