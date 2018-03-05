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
#include "Board.h"

#define REG_ADDR_BASE 0xbf000000
#define WREG(addr, value) {CHIPCFG_CMD_BASE, (addr) >> 16}, {(addr) & 0xffff, value}
#define WREG_B(addr, value) {CHIPCFG_CMD_BASE_8BITS, (addr) >> 16}, {(addr) & 0xffff, ((value & 0xff) << 8) | 0xff}

typedef struct _EXT_Param
{
    struct
    {
        MS_U16 addr;
        MS_U16 value;
    } chipcfg[1022];
} EXT_Param;


// {CHIPCFG_CMD_DBG,  0xF010},
// {CHIPCFG_CMD_BASE, 0xbf20},
// {CHIPCFG_CMD_WAIT, 50},
// {CHIPCFG_CMD_END,  0xFFFF},
#define SCMB_64_BWP 0
#define SCMB_64_MIU 1
const EXT_Param _hash1 _SECTION_HASH_1_ =
{
    {

    #if (ENABLE_MSTAR_MCM_ENABLE)
        {CHIPCFG_CMD_BASE, 0xBF2e},
        {0x1f80<<1,0x0000},     // LB: TSP(NA)
        {0x1f20<<1,0x8080},     // HB:VD_R2, LB:MOBF
        {0x1f00<<1,0x8000},     // HB:URDMA, LB:BDMA(NA)
        {0x1f02<<1,0x0080},     // LB:PM51
        {0x1fa2<<1,0x8080},     // HB:AU_R2, LB:AU_R2
        {0x1fc0<<1,0x8020},     // HB:MVOP, LB:SC
        {0x1fc2<<1,0x0000},     // HB:GOP2(NA), LB:GOP3(NA)
        {0x1fc4<<1,0x0000},     // HB:GOP0(NA), LB:GOP1(NA)
        {0x1fc6<<1,0x8000},     // HB:VE, LB:GOPD(NA)
        {0x1fa0<<1,0xf0f0},     // HB:Vivaldi LB:Vivaldi
        {0x1f26<<1,0x0080},     // LB:MVD
        {0x1f24<<1,0x8080},     // HB:HVD/EVD, LB:JPD
        {0x1f44<<1,0x8080},     // HB:GE, LB:GPD
        {0x1f22<<1,0x8080},     // HB:USB0, LB:USB1
        {0x1f40<<1,0x8000},     // HB:DEMOD
        {0x1f42<<1,0x8080},     // HB:FCIE, LB:GMAC
    #endif


#if ( MIU_INTERFACE == DDR2_INTERFACE_FPGA)
    #include "script_MIU_init_ddr2_FPGA.h"
#else
#include "script_MainPll_MIPSPLL_UPLL_flash.h"

        {CHIPCFG_CMD_WAIT, 1},
#include "script_clk_gen.h"
        {CHIPCFG_CMD_WAIT, 1},
        // MSTV_TOOL access dram enable
        {CHIPCFG_CMD_BASE, 0xBF00},
        {0x578C, 0x3C11},
        {0x5794, 0x403C},
        {0x5788, 0x0004},
        // L2 cache enable
        {CHIPCFG_CMD_BASE_8BITS, 0xBF20},
        {CHIPCFG_CMD_BASE, 0xBF20},
        {0x3124, 0x0001},      // 0x101849[0]; L2_gatclk_on = 1 for cache enable
        {0x3100, 0x31da},      // 0x101840[1]:
                               // [1]: reg_cca_buf_off = 1
                               // [3]: reg_cca_size = 1
                               // [4]: reg_cca_line = 1
                               // [5]: reg_cca_size_256k = 0
                               // [6]: reg_uca_line = 1
                               // [7]: reg_uca_buf_on = 1
                               // [8]: reg_wfull_16b = 1
                               // [12]: reg_cca_pf_on = 1
                               // [13]: reg_uca_pf_on = 1
                               // [14]: reg_c_resetmcu = 0
                               // [15]: reg_c_resetbridge = 0
        {0x3104, 0x0010},      // 0x101841[4]: Start to cache operation
        {CHIPCFG_CMD_WAIT, 1}, // wait while for //0x101841[8]: L2 cache active
        {0x3104, 0x0000},      // 0x101841[4]: Stop to cache operation
#ifdef CONFIG_DRAM_MOBF_64BIT
        {0x3100, 0x31D8},      // 0x101840[1]: reg_cca_buf_off = 0
#else
        {0x3100, 0x30D8},      // 0x101840[1]: reg_cca_buf_off = 0
#endif
        {CHIPCFG_CMD_WAIT, 1},

#if ( MS_BOARD_TYPE_SEL == BD_MST227C_D01A)
    #include "script_MIU_init_ddr3_1866_MCP15x15.h"
#elif (MS_BOARD_TYPE_SEL == BD_MST227D_D01A)
    #include "script_MIU_init_ddr3_1600_BGA13x13.h"
#elif  (MS_BOARD_TYPE_SEL == BD_MST227A_D01A)
    #include "script_MIU_init_ddr3_1866_BGA.h"
#elif  (MS_BOARD_TYPE_SEL == BD_MST227B_D01A) || (MS_BOARD_TYPE_SEL == BD_MST227B_D01A_S)
    #include "script_MIU_init_ddr3_1866_BGA_8Gbits.h"
#endif

//set reg_dram_size
        {CHIPCFG_CMD_BASE_8BITS, 0xBF20},
#if defined(CONFIG_MSTAR_DDR_1GB)
        {(0x12D2<<1)+1, 0xA0F0},
#elif defined(CONFIG_MSTAR_DDR_512MB)
        {(0x12D2<<1)+1, 0x90F0},
#elif defined(CONFIG_MSTAR_DDR_256MB)
        {(0x12D2<<1)+1, 0x80F0},
#elif defined(CONFIG_MSTAR_DDR_128MB)
        {(0x12D2<<1)+1, 0x70F0},
#elif defined(CONFIG_MSTAR_DDR_64MB)
        {(0x12D2<<1)+1, 0x60F0},
#else
#error "please check dram size range"
#endif

// DRAMOBF key table
#if !defined(CONFIG_TEE)
// Secure Range key different with MOBF, can't enable both
#ifdef CONFIG_DRAM_MOBF_8BIT
        {CHIPCFG_CMD_BASE, 0xBF20},
        {0x25EC, 0x0008},
        {CHIPCFG_CMD_BASE, 0xBF2C},
        {0x11D8, 0xC800}, // bypass key index

        {0x1E00, 0x2000},
        {0x1E04, 0x2000},
        {0x1E08, 0x2000},
        {0x1E0C, 0x2000},
        {0x1E10, 0x2000},
        {0x1E14, 0x2000},

        {0x1E18, 0x2000},
        {0x1E1C, 0x2000},
        {0x1E20, 0x2000},

        {0x1E24, 0x2000},
        {0x1E28, 0x2000},
        {0x1E2C, 0x2000},
        {0x1E30, 0x2000},

        {0x1E34, 0x2000},
        {0x1E38, 0x2000},

        {0x1E3C, 0x2000},
        {0x1E40, 0x2000},
        {0x1E44, 0x2000},
        {0x1E48, 0x2000},
        {0x1E4C, 0x2000},
        {0x1E50, 0x2000},
        {0x1E54, 0x2000},
        {0x1E58, 0x2000},
        {0x1E5C, 0x2000},
        {CHIPCFG_CMD_BASE, 0xBF2C},
        {0x11D8, 0xC001},
        {0x11D0, 0x0001},
        {0x11D4, 0x0302},

        {0x11D8, 0x0002},
        {0x11D0, 0x0504},
        {0x11D4, 0x0706},

        {0x11D8, 0x0003},
        {0x11D0, 0x0908},
        {0x11D4, 0x0b0a},

        {0x11D8, 0x0004},
        {0x11D0, 0x0d0c},
        {0x11D4, 0x0f0e},

        {0x11D8, 0x0005},
        {0x11D0, 0x1001},
        {0x11D4, 0x1302},

        {0x11D8, 0x0006},
        {0x11D0, 0x1504},
        {0x11D4, 0x1706},

        {0x11D8, 0x0007},
        {0x11D0, 0x1908},
        {0x11D4, 0x1b0a},

        {0x11D8, 0x0008},
        {0x11D0, 0x1d0c},
        {0x11D4, 0x1f0e},

        {0x11D8, 0x0009},
        {0x11D0, 0x00f1},
        {0x11D4, 0x03f2},

        {0x11D8, 0x000a},
        {0x11D0, 0x05f4},
        {0x11D4, 0x07f6},

        {0x11D8, 0x000b},
        {0x11D0, 0x09f8},
        {0x11D4, 0x0bfa},

        {0x11D8, 0x000c},
        {0x11D0, 0x0dfc},
        {0x11D4, 0x0ffe},

        {0x11D8, 0x000d},
        {0x11D0, 0x7001},
        {0x11D4, 0x7302},

        {0x11D8, 0x0004},
        {0x11D0, 0x7504},
        {0x11D4, 0x7706},

        {0x11D8, 0x000f},
        {0x11D0, 0x7908},
        {0x11D4, 0x7b0a},

        {0x11D8, 0x0010},
        {0x11D0, 0x7d0c},
        {0x11D4, 0x7f0e},

        {0x11D8, 0x0011},
        {0x11D0, 0x0081},
        {0x11D4, 0x0382},

        {0x11D8, 0x0012},
        {0x11D0, 0x0584},
        {0x11D4, 0x0786},
#endif // CONFIG_DRAM_MOBF_8BIT

#ifdef CONFIG_DRAM_MOBF_64BIT
#if SCMB_64_BWP // 64_BWP
        {CHIPCFG_CMD_BASE, 0xBF20},
        {0x25EC, 0x0004},
        {CHIPCFG_CMD_BASE, 0xBF2C},
        {0x11D8, 0xC000},

        {0x1E00, 0x2008},
        {0x1E04, 0x4008},
        {0x1E08, 0x4008},
        {0x1E0C, 0x4008},
        {0x1E10, 0x4008},
        {0x1E14, 0x4008},

        {0x1E18, 0x4000},
        {0x1E1C, 0x2000},
        {0x1E20, 0x4000},

        {0x1E24, 0x4000},
        {0x1E28, 0x4000},
        {0x1E2C, 0x4000},
        {0x1E30, 0x4000},

        {0x1E34, 0x4000},
        {0x1E38, 0x4000},

        {0x1E3C, 0x4000},
        {0x1E40, 0x4000},
        {0x1E44, 0x4000},
        {0x1E48, 0x4000},
        {0x1E4C, 0x4000},
        {0x1E50, 0x4000},
        {0x1E54, 0x4000},
        {0x1E58, 0x4008},
        {0x1E5C, 0x4000},

#endif // 64bit SCMB BWP

#if SCMB_64_MIU
        {CHIPCFG_CMD_BASE, 0xBF20},
        {0x25EC, 0x0005},
        {CHIPCFG_CMD_BASE, 0xBF2C},
        {0x11D8, 0xC000},

        {0x1E00, 0x2000},
        {0x1E04, 0x2000},
        {0x1E08, 0x2000},
        {0x1E0C, 0x2000},
        {0x1E10, 0x2000},
        {0x1E14, 0x2000},

        {0x1E18, 0x2000},
        {0x1E1C, 0x2000},
        {0x1E20, 0x2000},

        {0x1E24, 0x2000},
        {0x1E28, 0x2000},
        {0x1E2C, 0x2000},
        {0x1E30, 0x2000},

        {0x1E34, 0x2000},
        {0x1E38, 0x2000},

        {0x1E3C, 0x2000},
        {0x1E40, 0x2000},
        {0x1E44, 0x2000},
        {0x1E48, 0x2000},
        {0x1E4C, 0x2000},
        {0x1E50, 0x2000},
        {0x1E54, 0x2000},
        {0x1E58, 0x2000},
        {0x1E5C, 0x2000},

#endif // 64bit SCMB MIU
#endif // CONFIG_DRAM_MOBF_64BIT
#endif // !defined CONFIG_TEE
#endif



#if 1  // STR script move to Hash 1
        {CHIPCFG_INIT_TRIM,0x0000},
        {CHIPCFG_CMD_WAIT, 1}, //WAIT 0.001s	// delay 1ms
        {CHIPCFG_STR_IFAC,0x0},//CHIPCFG_STR_AC_SCRIPT start
        {CHIPCFG_CMD_BASE_8BITS, 0xBF22},
        _RV32_2(0x0D00, 0x0008),
        {CHIPCFG_CMD_BASE_8BITS, 0xBF20}, //_Disable_ATOP_SW start
        {((0x1202<<1)+1),0x00F0},
        {CHIPCFG_CMD_BASE_8BITS, 0xBF22},
        _RV32_2(0x0D54, 0x0070),
        _RV32_2(0x0D08, 0xffff),          //_Disable_ATOP_SW end
        {CHIPCFG_STR_IFAC_END,0x0},
        {CHIPCFG_MIU_INIT_CYCLE,0x0000},
        {CHIPCFG_STR_IFAC,0x0},     //CHIPCFG_STR_AC_SCRIPT start
        {CHIPCFG_CMD_BASE,0xbf22},
         #if (ENABLE_MEM1866MHz)
        _RV32_2(0x0d0e, 0x00e9),
        _RV32_2(0x0d0e, 0x00a9),
        _RV32_2(0x0d0e, 0x00e9),
        _RV32_2(0x0d0e, 0x00a9),
        #elif (ENABLE_MEM1600MHz)
        _RV32_2(0x0d0e, 0x00e7),
        _RV32_2(0x0d0e, 0x00a7),
        _RV32_2(0x0d0e, 0x00e7),
        _RV32_2(0x0d0e, 0x00a7),
        #endif
        {CHIPCFG_CMD_BASE,0xbf20},
        _RV32_2(0x1218, 0x0400),
        _RV32_2(0x1200, 0x043f),
        _RV32_2(0x1200, 0x053f),
        _RV32_2(0x1200, 0x003f),
        _RV32_2(0x1200, 0x023f),
        _RV32_2(0x1200, 0x033f),
        _RV32_2(0x1200, 0x001f),     //CHIPCFG_STR_AC_SCRIPT end
        {CHIPCFG_STR_IFAC_END,0x0},
#if CONFIG_MSTAR_STR_ENABLE
        {CHIPCFG_STR_IFDC,0x0},  //CHIPCFG_STR_DC_SCRIPT start
        {CHIPCFG_STR_DC_BEFORE_REFRESH,0x0000},
        {CHIPCFG_CMD_BASE,0xbf20}, //Enter refresh mode
        _RV32_2(0x12e0, 0x0000),
        _RV32_2(0x1246, 0xFFFE),
        _RV32_2(0x1266, 0xFFFF),
        _RV32_2(0x1286, 0xFFFF),
        _RV32_2(0x12A6, 0xFFFF),
        _RV32_2(0x1218, 0x0400),
        _RV32_2(0x1200, 0x002F),
        _RV32_2(0x1200, 0x042F),
        _RV32_2(0x1200, 0x052F),
        _RV32_2(0x1200, 0x002F),
        {CHIPCFG_CMD_BASE_8BITS, 0xBF20},
        {((0x1206<<1)+1),0x0808},
        {CHIPCFG_CMD_BASE,0xbf20},
        _RV32_2(0x1246, 0xFFFF),
        _RV32_2(0x1200, 0x202F),  //Enter refresh mode end
        {CHIPCFG_CMD_BASE_8BITS, 0xBF20}, //_Disable_ATOP_SW start
        {((0x1202<<1)+1),0x00F0},
        {CHIPCFG_CMD_BASE_8BITS, 0xBF22},
        _RV32_2(0x0D54, 0x0070),
        _RV32_2(0x0D08, 0xffff),          //_Disable_ATOP_SW start end
        {CHIPCFG_CMD_BASE_8BITS, 0xBF20},//_Exit_Refresh_Mode start
        {((0x1206<<1)+1),0x0008},
        {CHIPCFG_CMD_BASE,0xbf20},
        _RV32_2(0x1200, 0x202F),
        _RV32_2(0x1246, 0xFFFF),
        _RV32_2(0x1200, 0x022F),
        _RV32_2(0x1200, 0x032F),
        _RV32_2(0x1200, 0x002F),
        _RV32_2(0x1200, 0x000F),
        {CHIPCFG_CMD_WAIT, 50},
        _RV32_2(0x1200, 0x001F),
        {CHIPCFG_CMD_WAIT, 5},
        _RV32_2(0x1246, 0x7ffe),
        _RV32_2(0x1266, 0xffff),
        _RV32_2(0x1286, 0xffff),
        _RV32_2(0x12A6, 0xffff),
        _RV32_2(0x121e, 0x0c08),
        _RV32_2(0x12e0, 0x0000),        //_Exit_Refresh_Mode end
         #if (ENABLE_MEM1866MHz) //_MIU0_Push_Dram start
        {CHIPCFG_CMD_BASE,0xbf22},
        _RV32_2(0x0d0e, 0x00e9),
        _RV32_2(0x0d0e, 0x00a9),
        _RV32_2(0x0d0e, 0x00e9),
        _RV32_2(0x0d0e, 0x00a9),
        #elif (ENABLE_MEM1600MHz)
        {CHIPCFG_CMD_BASE,0xbf22},
        _RV32_2(0x0d0e, 0x00e7),
        _RV32_2(0x0d0e, 0x00a7),
        _RV32_2(0x0d0e, 0x00e7),
        _RV32_2(0x0d0e, 0x00a7),
        #endif
        _RV32_2(0x1218, 0x0400),
        _RV32_2(0x1200, 0x043f),
        _RV32_2(0x1200, 0x053f),
        _RV32_2(0x1200, 0x003f),
        _RV32_2(0x1200, 0x023f),
        _RV32_2(0x1200, 0x033f),
        _RV32_2(0x1200, 0x001f), //_MIU0_Push_Dram start end
        {CHIPCFG_STR_IFDC_END,0x0},
#endif //CHIPCFG_STR_DC_SCRIPT end
        {CHIPCFG_CMD_BASE,0xbf20}, //_MIU_ClearIPMask start
        _RV32_2(0x1246, 0x0000),
        _RV32_2(0x1266, 0x0000),
        _RV32_2(0x1286, 0x0000),
        _RV32_2(0x12a6, 0x0000),
        _RV32_2(0x121e, 0x0c08),  //_MIU_ClearIPMask end // SW initial done and turn on arbitor
#endif //END notes "STR scripts move to H1"
        {CHIPCFG_CMD_WAIT, 1},
        {CHIPCFG_CMD_END,  0xFFFF}
    }
};

