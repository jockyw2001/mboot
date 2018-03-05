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
#include "chip.h"

#define REG_ADDR_BASE 0x1f000000
#define WREG(addr, value) {CHIPCFG_CMD_BASE, (addr) >> 16}, {(addr) & 0xffff, value}
#define WREG_B(addr, value) {CHIPCFG_CMD_BASE_8BITS, (addr) >> 16}, {(addr) & 0xffff, (((value) & 0xff) << 8) | 0xff}
#define WREG_B_MASK(addr, value, mask) {CHIPCFG_CMD_BASE_8BITS, (addr) >> 16}, {(addr) & 0xffff, (((value) & 0xff) << 8) | mask}

#define WREG_NO_BASE(addr, value) {(addr) & 0xffff, value}
#define WREG_B_NO_BASE(addr,value) {(addr) & 0xffff, (((value) & 0xff) << 8) | 0xff}
#define WREG_B_MASK_NO_BASE(addr,value,mask)  {(addr) & 0xffff, (((value) & 0xff) << 8) | mask}

#define MIU_PROTECT_DDR_32MB                    (0x50)
#define MIU_PROTECT_DDR_64MB                    (0x60)
#define MIU_PROTECT_DDR_128MB                   (0x70)
#define MIU_PROTECT_DDR_256MB                   (0x80)
#define MIU_PROTECT_DDR_512MB                   (0x90)
#define MIU_PROTECT_DDR_1024MB                  (0xA0)
#define MIU_PROTECT_DDR_2048MB                  (0xB0)

#if defined(MIU1_LEN) && (MIU1_LEN != 0)
#define ENABLE_MIU1 1
#else
#define ENABLE_MIU1 0
#endif

typedef struct _EXT_Param
{
    struct
    {
        MS_U16 addr;
        MS_U16 value;
    } chipcfg[HASH1_PARAM_LEN];
} EXT_Param;

const EXT_Param _hash1 _SECTION_HASH_1_ =
{
    {

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    #include "script_MainPll_ARMPLL_UPLL_flash.h"
    #include "script_clk_gen.h"
    #include "script_lowpwr_initial.h"
#endif

#if (ENABLE_MSTAR_MCM_ENABLE)
    {CHIPCFG_CMD_BASE,0x1f2E},
    {0x1300<<1, 0xf0f0},
    {0x1302<<1, 0xf000},
    {0x1304<<1, 0x00f0},
    {0x1200<<1, 0xf0f0},
    {0x1202<<1, 0x00f0},
    {0x1204<<1, 0xf000},
    {0x1206<<1, 0xf0f0},
    {0x1208<<1, 0x00f0},
    {0x120a<<1, 0xf0f0},
    {0x120c<<1, 0xf000},
    {0x1210<<1, 0xf270},
    {0x1212<<1, 0xf200},
    {0x1214<<1, 0xf0f0},
    {0x1216<<1, 0x0052},
    {0x1500<<1, 0xf0f0},
    {0x1502<<1, 0x20f0},
    {0x1504<<1, 0x00f0},
    {0x1600<<1, 0x00f0},
    {0x1602<<1, 0xf8f8},
    {0x1400<<1, 0xf000},
    {0x1402<<1, 0x0000},
    {0x1404<<1, 0x0000},
    {0x1406<<1, 0x1002},
    {0x1408<<1, 0x2010},
    {0x140a<<1, 0x00f0},
    {0x140c<<1, 0x0202},
    {0x140e<<1, 0x0002},
    {0x1100<<1, 0xf000},
    {0x1102<<1, 0x00f0},
#endif

#if defined(CONFIG_HASH1_CMDS_MULTI_BLKS) && (CONFIG_HASH1_CMDS_MULTI_BLKS==1)
    #if (defined(CONFIG_MSTAR_DDR3_EXT)&&(CONFIG_MSTAR_DDR3_EXT==1)) || (defined(CONFIG_MSTAR_DDR3_MCP)&&(CONFIG_MSTAR_DDR3_MCP==1))
    {CHIPCFG_CMD_DDR_SEL,HASH1_DDR3_MAGIC},
    #elif (defined(CONFIG_MSTAR_DDR4_EXT)&&(CONFIG_MSTAR_DDR4_EXT==1)) || (defined(CONFIG_MSTAR_DDR4_MCP)&&(CONFIG_MSTAR_DDR4_MCP==1))
    {CHIPCFG_CMD_DDR_SEL,HASH1_DDR4_MAGIC},
    #endif
#endif

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
#if defined(CONFIG_MIU_SCRIPT_FILE)
    #include CONFIG_MIU_SCRIPT_FILE
#else
    #error "CONFIG_MIU_SCRIPT_FILE not defined"
#endif
#else
#include "script_MIU_init_FPGA.h"
#endif

#if 0 //test multi blks
    {CHIPCFG_CMD_BASE, 0x1f20},

    {CHIPCFG_CMD_TAG_START,0},
    _RV32_2(0x3390,0x5566),
    _RV32_2(0x3394,0x5566),
    {CHIPCFG_CMD_TAG_END,0},

    {CHIPCFG_CMD_TAG_START,1},
    _RV32_2(0x3390,0x6677),
    _RV32_2(0x3394,0x6677),
    {CHIPCFG_CMD_TAG_END,1},

    {CHIPCFG_CMD_TAG_START,3},
    _RV32_2(0x3390,0x8899),
    _RV32_2(0x3394,0x8899),
    {CHIPCFG_CMD_TAG_END,3},

    {CHIPCFG_CMD_TAG_START,2},
    _RV32_2(0x3390,0x7788),
    _RV32_2(0x3394,0x7788),
    {CHIPCFG_CMD_TAG_END,2},
#endif

#if defined(CONFIG_DRAM_MOBF)
    {CHIPCFG_CMD_BASE, 0x1f2c},
    _RV32_2(0x08ec,0xc001),
    {CHIPCFG_CMD_BASE, 0x1f20},
#if defined(CONFIG_DRAM_MOBF_8BIT)
    _RV32_2(0x12d8, 0x0000),
    _RV32_2(0x06d8, 0x0000),
    {CHIPCFG_CMD_BASE, 0x1f2C},
    _RV32_2(0x155c, 0x1000),
    _RV32_2(0x225c, 0x1000),

#elif defined(CONFIG_DRAM_MOBF_64BIT)
    _RV32_2(0x12d8, 0x0400),
    _RV32_2(0x06d8, 0x0400),
    {CHIPCFG_CMD_BASE, 0x1f2C},

#if defined(CONFIG_BWP_MIU)
    // reg_bypass_b_write = 0
    _RV32_2(0x155c, 0x0000),
    _RV32_2(0x225c, 0x0000),
#endif
    // Enable CPU byte write parser
    {CHIPCFG_CMD_BASE, 0x1f20},
    _RV32_2(0x18fc, 0x0001),
    // Enable GPU byte write parser
    WREG_B((REG_ADDR_BASE + (0x1108f6 <<1)) , 0x00),
    WREG_B((REG_ADDR_BASE + (0x1108f6 <<1)) , 0x02),
#endif
#endif

    //set reg_dram_size
#if defined(CONFIG_MSTAR_DDR_512MB_512MB)
    WREG_B((REG_ADDR_BASE + ((0x1012D2 <<1) + 1)) , MIU_PROTECT_DDR_512MB),
    WREG_B((REG_ADDR_BASE + ((0x1006D2 <<1) + 1)) , MIU_PROTECT_DDR_512MB),
#elif defined(CONFIG_MSTAR_DDR_256MB_256MB)
    WREG_B((REG_ADDR_BASE + ((0x1012D2 <<1) + 1)) , MIU_PROTECT_DDR_256MB),
    WREG_B((REG_ADDR_BASE + ((0x1006D2 <<1) + 1)) , MIU_PROTECT_DDR_256MB),
#elif defined(CONFIG_MSTAR_DDR_256MB)
    WREG_B((REG_ADDR_BASE + ((0x1012D2 <<1) + 1)) , MIU_PROTECT_DDR_256MB),
#else
    #error "not supported DDR configuration for reg_dram_size"
#endif

#if CONFIG_MSTAR_STR_ENABLE
        ///////////////////
        //Enter refresh mode
        ///////////////////
        {CHIPCFG_STR_IFDC,0x0},
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x12e0, 0x0000),
        _RV32_2(0x1246, 0xFFFE),
        _RV32_2(0x1266, 0xFFFF),
        _RV32_2(0x1286, 0xFFFF),
        _RV32_2(0x12a6, 0xFFFF),
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x1506, 0xFFFF),
        _RV32_2(0x1526, 0xFFFF),
        {CHIPCFG_CMD_BASE,0x1f28},
        _RV32_2(0x1006, 0xFFFF),
        _RV32_2(0x1026, 0xFFFF),
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x15e6, 0x00FA),
        {CHIPCFG_CMD_WAIT, 200},       // delay 200ms
#if (ENABLE_MIU1)
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x0646, 0xFFFE),
        _RV32_2(0x0666, 0xFFFF),
        _RV32_2(0x0686, 0xFFFF),
        _RV32_2(0x06a6, 0xFFFF),
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x2206, 0xFFFF),
        _RV32_2(0x2226, 0xFFFF),
        {CHIPCFG_CMD_BASE,0x1f28},
        _RV32_2(0x1106, 0xFFFF),
        _RV32_2(0x1126, 0xFFFF),
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x22e6, 0x00FA),
        {CHIPCFG_CMD_WAIT, 200},       // delay 200ms
#endif
        // Pre-charge all
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x1218, 0x0400), //reg_mrx = 0400 (precharege all
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        _RV32_2(0x1200, 0x2030), //[05]   : reg_auto_ref_off  = 1
                                               //[04]   : reg_odt           = 0
        {((0x1200<<1)+1),0x041e},//[12:9] : reg_single_cmd    = 2 (precharege)
        {((0x1200<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {((0x1200<<1)+1),0x0101},//[8]    : reg_single_cmd_en = 1
        {((0x1200<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {((0x1200<<1)+1),0x021e},//[12:9] : reg_single_cmd    = 1 (refresh)
        {((0x1200<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {((0x1200<<1)+1),0x0101},//[8]    : reg_single_cmd_en = 1
        {((0x1200<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x1246, 0xffff),
        // Enter self-refresh
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x1200<<1)+1),0x2020},//[13]   : reg_self_refresh  = 1

#if (ENABLE_MIU1)
        // Pre-charge all
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x0618, 0x0400), //reg_mrx = 0400 (precharege all
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        _RV32_2(0x0600, 0x2030), //[05]   : reg_auto_ref_off  = 1
                                               //[04]   : reg_odt           = 0
        {((0x0600<<1)+1),0x041e},//[12:9] : reg_single_cmd    = 2 (precharege)
        {((0x0600<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {((0x0600<<1)+1),0x0101},//[8]    : reg_single_cmd_en = 1
        {((0x0600<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {((0x0600<<1)+1),0x021e},//[12:9] : reg_single_cmd    = 1 (refresh)
        {((0x0600<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {((0x0600<<1)+1),0x0101},//[8]    : reg_single_cmd_en = 1
        {((0x0600<<1)+1),0x0001},//[8]    : reg_single_cmd_en = 0
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x0646, 0xffff),
        // Enter self-refresh
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x0600<<1)+1),0x2020},//[13]   : reg_self_refresh  = 1
#endif

        ///////////////////
        //Stop Power
        ///////////////////
        //Disable OENZ
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x1202<<1)+1),0x00f0},
        //ATOP_PD on
        {CHIPCFG_CMD_BASE,0x1f22},
        _RV32_2(0x0d54, 0xc000),
        //rx enable
#if (defined(CONFIG_MSTAR_DDR3_EXT)&&(CONFIG_MSTAR_DDR3_EXT==1)) || (defined(CONFIG_MSTAR_DDR3_MCP)&&(CONFIG_MSTAR_DDR3_MCP==1))
        _RV32_2(0x0d08, 0x003f),
#elif (defined(CONFIG_MSTAR_DDR4_EXT)&&(CONFIG_MSTAR_DDR4_EXT==1)) || (defined(CONFIG_MSTAR_DDR4_MCP)&&(CONFIG_MSTAR_DDR4_MCP==1))
        _RV32_2(0x0d08, 0x007f),
#endif
        //DQSM RST
        _RV32_2(0x0d1e, 0x0005),
        _RV32_2(0x0d1e, 0x000f),
        _RV32_2(0x0d1e, 0x0005),

#if (ENABLE_MIU1)
        //Disable OENZ
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x0602<<1)+1),0x00f0},
        //ATOP_PD on
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x1654, 0xc000),
        //rx enable
#if (defined(CONFIG_MSTAR_DDR3_EXT)&&(CONFIG_MSTAR_DDR3_EXT==1)) || (defined(CONFIG_MSTAR_DDR3_MCP)&&(CONFIG_MSTAR_DDR3_MCP==1))
        _RV32_2(0x1608, 0x003f),
#elif (defined(CONFIG_MSTAR_DDR4_EXT)&&(CONFIG_MSTAR_DDR4_EXT==1)) || (defined(CONFIG_MSTAR_DDR4_MCP)&&(CONFIG_MSTAR_DDR4_MCP==1))
        _RV32_2(0x1608, 0x007f),
#endif
        //DQSM RST
        _RV32_2(0x161e, 0x0005),
        _RV32_2(0x161e, 0x000f),
        _RV32_2(0x161e, 0x0005),
#endif

        ///////////////////
        //Exit SelfRefresh
        ///////////////////
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x1200<<1)+1),0x0020}, //[13]   : reg_self_refresh  = 0
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x1246, 0xfffe),
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x15e6, 0x00fa),

        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x1200<<1)+1),0x021e}, //[12:9] : reg_single_cmd    = 1 (refresh)
        {((0x1200<<1)+1),0x0001}, //[8]    : reg_single_cmd_en = 0
        {((0x1200<<1)+1),0x0101}, //[8]    : reg_single_cmd_en = 1
        {((0x1200<<1)+1),0x0001}, //[8]    : reg_single_cmd_en = 0
        _RV32_2(0x1200, 0x0030),  //[05]   : reg_auto_ref_off  = 1
                                                //[04]   : reg_odt           = 0
#if (ENABLE_MIU1)
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x0600<<1)+1),0x0020}, //[13]   : reg_self_refresh  = 0
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x0646, 0xfffe),
        {CHIPCFG_CMD_BASE,0x1f2c},
        _RV32_2(0x22e6, 0x00fa),

        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        {((0x0600<<1)+1),0x021e}, //[12:9] : reg_single_cmd    = 1 (refresh)
        {((0x0600<<1)+1),0x0001}, //[8]    : reg_single_cmd_en = 0
        {((0x0600<<1)+1),0x0101}, //[8]    : reg_single_cmd_en = 1
        {((0x0600<<1)+1),0x0001}, //[8]    : reg_single_cmd_en = 0
        _RV32_2(0x0600, 0x0030),  //[05]   : reg_auto_ref_off  = 1
                                                //[04]   : reg_odt          = 0
#endif
        {CHIPCFG_CMD_WAIT, 5},       // delay 5ms

        ///////////////////
        //Enable ODT
        ///////////////////
        {CHIPCFG_CMD_BASE_8BITS, 0x1F20},
        _RV32_2(0x1200, 0x1010), //[04]   : reg_odt           = 1
#if (ENABLE_MIU1)
        _RV32_2(0x0600, 0x1010), //[04]   : reg_odt           = 1
#endif
        {CHIPCFG_CMD_WAIT, 5},       // delay 5ms

        {CHIPCFG_STR_IFDC_END,0x0},

        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x1246, 0x7ffe),
        //_RV32_2(0x1266, 0xffff), // duplicate
        //_RV32_2(0x1286, 0xffff), // duplicate
        //_RV32_2(0x12a6, 0xffff), // duplicate
        //{CHIPCFG_CMD_BASE,0x1f2c}, // duplicate
        //_RV32_2(0x1506, 0xffff), // duplicate
        //_RV32_2(0x1526, 0xffff), // duplicate
        //{CHIPCFG_CMD_BASE,0x1f28}, // duplicate
        //_RV32_2(0x1006, 0xffff), // duplicate
        //_RV32_2(0x1026, 0xffff), // duplicate
        //{CHIPCFG_CMD_BASE,0x1f2c}, // duplicate
        //_RV32_2(0x15e6, 0x00fa), // duplicate


#if (ENABLE_MIU1)
        {CHIPCFG_CMD_BASE,0x1f20},
        _RV32_2(0x0646, 0x7ffe),
        //_RV32_2(0x0666, 0xffff), // duplicate
        //_RV32_2(0x0686, 0xffff), // duplicate
        //_RV32_2(0x06a6, 0xffff), // duplicate
        //{CHIPCFG_CMD_BASE,0x1f2c}, // duplicate
        //_RV32_2(0x2206, 0xffff), // duplicate
        //_RV32_2(0x2226, 0xffff), // duplicate
        //{CHIPCFG_CMD_BASE,0x1f28}, // duplicate
        //_RV32_2(0x1106, 0xffff), // duplicate
        //_RV32_2(0x1126, 0xffff), // duplicate
        //{CHIPCFG_CMD_BASE,0x1f2c}, // duplicate
        //_RV32_2(0x22e6, 0x00fa), // duplicate
#endif
        {CHIPCFG_CMD_WAIT, 200},       // delay 200ms

#endif //CONFIG_MSTAR_STR_ENABLE
        {CHIPCFG_CMD_END,  0xFFFF}
    }
};

