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
#define _RV32_2(a, b)    { (a) << 1, b }



#if defined(MIU1_LEN) && (MIU1_LEN != 0)
#define ENABLE_MIU1 1
#else
#define ENABLE_MIU1 0
#endif

//swch 4
{CHIPCFG_CMD_BASE, 0x1f20},
_RV32_2(0x0b3a, 0x0808),   //turn on miu clock

//MIU0 Software reset
_RV32_2(0x121e, 0x0000),   //hold miu_0
_RV32_2(0x121e, 0x0c01),   //miu_sw_rst
_RV32_2(0x121e, 0x0c00),
_RV32_2(0x1202, 0x0000),   //disable all ddr bus out

//MIU0 Request mask
_RV32_2(0x1246, 0x7FFE),   //mask group0
_RV32_2(0x1266, 0xFFFF),   //mask group1
_RV32_2(0x1286, 0xFFFF),   //mask group2
_RV32_2(0x12A6, 0xFFFF),   //mask group3

#if (ENABLE_MIU1)
//MIU1 Software reset
_RV32_2(0x061e, 0x0000),   //hold miu_1
_RV32_2(0x061e, 0x0c01),   //miu_sw_rst
_RV32_2(0x061e, 0x0c00),
_RV32_2(0x0602, 0x0000),   //disable all ddr bus out

//MIU1 Request mask
_RV32_2(0x0646, 0x7FFE),   //mask group0
_RV32_2(0x0666, 0xFFFF),   //mask group1
_RV32_2(0x0686, 0xFFFF),   //mask group2
_RV32_2(0x06A6, 0xFFFF),   //mask group3
#endif

#if CONFIG_MSTAR_STR_ENABLE    
    {CHIPCFG_CMD_AVD_RESET, 0x00},
#endif

// MIU0 : DDR3 - 1600
{CHIPCFG_CMD_BASE, 0x1f22},
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if (ENABLE_MEM1600MHz)
_RV32_2(0x0d36, 0x8000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
_RV32_2(0x0d34, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
_RV32_2(0x0d30, 0x2b52),   //ddfset
_RV32_2(0x0d32, 0x0035),   //ddrpll_reset/ddrpll_porst/ddrpl_pd
_RV32_2(0x0d2a, 0x0528),   //SSC enable (1%)
_RV32_2(0x0d28, 0xc00d),   //SSC enable (1%)
#else
#error "Unsupport DDR freq!!!"
#endif
#endif

//Avoid DDRPLL unknown(MCLK05X & MCLK025X) by resset ddrat[28]
//_RV32_2(0x0d24, 0x1000),

// MIU1: DDR3-1600
#if (ENABLE_MIU1)
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if (ENABLE_MEM1600MHz)
_RV32_2(0x0db6, 0x8000),   //reg_DDRPLL_LOOP_DIV_FIRST  : [15:14]   divide by 4
_RV32_2(0x0db4, 0x0300),   //reg_DDRPLL_LOOP_DIV_SECOND : [15:14]   divide by 3
_RV32_2(0x0db0, 0x2b52),   //ddfset
_RV32_2(0x0db2, 0x0035),   //ddrpll_reset/ddrpll_porst/ddrpl_pd

_RV32_2(0x0daa, 0x0528),   //SSC enable (1%)
_RV32_2(0x0da8, 0xc00d),   //SSC enable (1%)

#else
#error "Unsupport DDR freq!!!"
#endif
#endif
#endif

{CHIPCFG_CMD_WAIT, 1},       // delay 1ms


// MIU0:67 line-> MIU0 DLL calibration

{CHIPCFG_CMD_BASE, 0x1f20},

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
_RV32_2( 0x1202, 0xf3a3  ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
_RV32_2( 0x1204, 0x004b  ),  //reg_8x_2cmd_mode
#if CONFIG_MSTAR_STR_ENABLE    
    _RV32_2(0x1206, 0x1c30,
#else
_RV32_2( 0x1206, 0x0c30  ),
#endif
_RV32_2( 0x1208, 0x1cbb  ),
_RV32_2( 0x120a, 0x2766  ),
_RV32_2( 0x120c, 0x96c8  ),  //reg_tWL
_RV32_2( 0x120e, 0x4114  ), // default 2T Mode is on please check 110d54 bit[15:14] should be 10
_RV32_2( 0x1210, 0x1c70  ),  //MR0
_RV32_2( 0x1212, 0x4040  ),  //MR1
_RV32_2( 0x1214, 0x8018  ),  //MR2
_RV32_2( 0x1216, 0xC000  ),  //MR3
#else
#error "Unsupport DDR freq!!!"
#endif

_RV32_2( 0x1226, 0x0000  ),

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
_RV32_2( 0x1228, 0x40B0  ), // enable dynamic ODT (20130515)
#endif

//Yuan
_RV32_2( 0x12d8, 0x0200  ),   //bypass dramabf
#if CONFIG_MSTAR_STR_ENABLE    
    _RV32_2(0x12d8, 0x0200),
#endif

//_RV32_2( 0x122c, 0x0000  ),
_RV32_2( 0x12d4, 0x0000  ),   // 20130108 turn off read_last_done_z_bypass
_RV32_2( 0x12b2, 0xffff  ),   // 20130108 change last_done_z position
_RV32_2( 0x1292, 0xffff  ),   // 20130108 change last_done_z position
_RV32_2( 0x1272, 0xffff  ),   // 20130108 change last_done_z position
_RV32_2( 0x1252, 0xffff  ),   // 20130108 change last_done_z position

{CHIPCFG_CMD_BASE, 0x1f22},

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if CONFIG_MSTAR_STR_ENABLE   
    _RV32_2(0x0d00, 0x0a08),  //20130109 PAD MUX and IC side odt enable
#else
_RV32_2( 0x0d00, 0x0a00  ),  //20130109 PAD MUX and IC side odt enable
#endif
_RV32_2( 0x0d04, 0x0080  ),  //reg_dqs_pre_state
_RV32_2( 0x0d10, 0x0200  ),
_RV32_2( 0x0d38, 0x0044  ),
_RV32_2( 0x0d48, 0x0044  ),
_RV32_2( 0x0d3a, 0x5052  ),
_RV32_2( 0x0d4a, 0x8082  ),
_RV32_2( 0x0d02, 0x0000  ),
#else
#error "Unsupport DDR freq!!!"
#endif

_RV32_2( 0x0d3c, 0x1133  ),
_RV32_2( 0x0d4c, 0x0033  ),

#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
_RV32_2( 0x0d5a, 0x3333  ),
_RV32_2( 0x0d5c, 0x3333  ),
_RV32_2( 0x0d5e, 0x3333  ),
#else
#error "Unsupport DDR freq!!!"
#endif

_RV32_2( 0x0d3e, 0x1011  ),
_RV32_2( 0x0d50, 0x1111  ),
_RV32_2( 0x0d6c, 0x6677  ),
_RV32_2( 0x0d6e, 0x7777  ),   //reg_dll0_ph              2      : [3:0] , 20140702 CAE neil.hsu need to test
_RV32_2( 0x0d6a, 0x0020  ),
_RV32_2( 0x0d68, 0x0020  ),
_RV32_2( 0x0d74, 0x0044  ),
#if CONFIG_MSTAR_STR_ENABLE   
_RV32_2(0x0d72, 0x8803), //eason
#else
_RV32_2( 0x0d72, 0x0003  ),
#endif
//DLL calibration
_RV32_2( 0x0d60, 0x000c  ),
_RV32_2( 0x0d60, 0x0008  ),
_RV32_2( 0x0d62, 0x007f  ),
_RV32_2( 0x0d64, 0xf200  ),
_RV32_2( 0x0d60, 0x2370  ),

{CHIPCFG_CMD_WAIT, 5},       // delay 5ms
{CHIPCFG_CMD_BASE, 0x1f20},
_RV32_2( 0x121e, 0x0c01  ), //miu software reset
#if CONFIG_MSTAR_STR_ENABLE 
_RV32_2(0x121e, 0x0c00), 
#endif
{CHIPCFG_CMD_WAIT, 1},       // wait 1 us
#if CONFIG_MSTAR_STR_ENABLE
#else
_RV32_2( 0x121e, 0x0c08  ),
#endif
{CHIPCFG_CMD_BASE, 0x1f22},

_RV32_2( 0x0d02, 0xaaaa  ), // @reg_cko_state        cc  : [7:0]
_RV32_2( 0x0d18, 0x0000  ),

{CHIPCFG_CMD_BASE, 0x1f20},

#if CONFIG_MSTAR_STR_ENABLE
#else
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
_RV32_2( 0x1202, 0x03a3  ),
#endif
#endif

{CHIPCFG_CMD_BASE, 0x1f22},
#if (MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d54, 0x8070),  //20130329  reg_cmd_mode_sel
#else
_RV32_2( 0x0d54, 0x8000  ),  //20130329  reg_cmd_mode_sel
#endif
#endif

//MIU1
{CHIPCFG_CMD_BASE, 0x1f20},
#if ( ENABLE_MIU1 == 1 )

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
_RV32_2( 0x0602, 0xf3a3  ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
_RV32_2( 0x0604, 0x004b  ),
#if CONFIG_MSTAR_STR_ENABLE
_RV32_2(0x0606, 0x1c30),
#else
_RV32_2( 0x0606, 0x0c30  ),
#endif
_RV32_2( 0x0608, 0x1cbb  ),
_RV32_2( 0x060a, 0x2766  ),
_RV32_2( 0x060c, 0x96c8  ),
_RV32_2( 0x060e, 0x4114 ),  // default 2T Mode is on please check 110d54 bit[15:14] should be 10
_RV32_2( 0x0610, 0x1c70  ),  //MR0
_RV32_2( 0x0612, 0x4040  ),  //MR1
_RV32_2( 0x0614, 0x8018  ),  //MR2
_RV32_2( 0x0616, 0xC000  ),  //MR3
#else
#error "Unsupport DDR freq!!!"
#endif

#if(MIU_CONFIG == DDR3_MIU0_16_MIU1_16)
_RV32_2( 0x0628, 0x40b0  ), // enable dynamic ODT (20130515)
#endif

_RV32_2( 0x062c, 0x0000  ),
//_RV32_2( 0x06d8, 0x0200  ), //bypass dramobf

#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x06d8, 0x0200),
#endif
_RV32_2( 0x06d4, 0x0000  ), // 20130108 turn off read_last_done_z_bypass
_RV32_2( 0x06b2, 0xffff  ), // 20130108 change last_done_z position
_RV32_2( 0x0692, 0xffff  ),   // 20130108 change last_done_z position
_RV32_2( 0x0672, 0xffff  ),   // 20130108 change last_done_z position
_RV32_2( 0x0652, 0xffff  ),  // 20130108 change last_done_z position

{CHIPCFG_CMD_BASE, 0x1f22},
#if CONFIG_MSTAR_STR_ENABLE
    _RV32_2(0x0d80, 0x0a08),
#else
_RV32_2( 0x0d80, 0x0a00  ),
#endif
_RV32_2( 0x0d84, 0x0080  ),
_RV32_2( 0x0d90, 0x0200  ),

_RV32_2( 0x0db8, 0x0044  ),
_RV32_2( 0x0dc8, 0x0044  ),
_RV32_2( 0x0dba, 0x5052  ),
_RV32_2( 0x0dca, 0x8082  ),
_RV32_2( 0x0d82, 0x0000  ),
_RV32_2( 0x0dbc, 0x1133  ),
_RV32_2( 0x0dcc, 0x0033  ),
_RV32_2( 0x0dbe, 0x1011  ),
_RV32_2( 0x0dd0, 0x1111  ),

_RV32_2( 0x0dda, 0x3333  ),
_RV32_2( 0x0ddc, 0x3333  ),
_RV32_2( 0x0dde, 0x3333  ),


_RV32_2( 0x0dec, 0x6677  ), //reg_dll1_ph
_RV32_2( 0x0dee, 0x7777  ), //reg_dll1_ph
_RV32_2( 0x0dea, 0x0020  ),
_RV32_2( 0x0de8, 0x0020  ),
_RV32_2( 0x0df4, 0x4444  ),
#if CONFIG_MSTAR_STR_ENABLE
_RV32_2(0x0df2, 0x8803), //eason
#else
_RV32_2( 0x0df2, 0x0003  ),
#endif
//DLL calibration
_RV32_2( 0x0de0, 0x000c  ),
_RV32_2( 0x0de0, 0x0008  ),
_RV32_2( 0x0de2, 0x007f  ),
_RV32_2( 0x0de4, 0xf200  ),
_RV32_2( 0x0de0, 0x2370  ),

{CHIPCFG_CMD_WAIT, 5},       // delay 5ms

{CHIPCFG_CMD_BASE, 0x1f20},
_RV32_2( 0x061e, 0x0c01  ), //0x0c01

{CHIPCFG_CMD_WAIT, 1},       // delay 1ms

_RV32_2( 0x061e, 0x0c08  ), //0x0c01

{CHIPCFG_CMD_BASE, 0x1f22},
_RV32_2( 0x0d82, 0xaaaa  ), //reg_cko_state        cc  : [7:0]
_RV32_2( 0x0d98, 0x0000  ), //
#if CONFIG_MSTAR_STR_ENABLE
#else
{CHIPCFG_CMD_BASE, 0x1f20},
_RV32_2( 0x0602, 0x03a3  ),  //ck/addr/data/cke/data swap/Nx/col/bank/bus width/dram type => 0x2a2=ddr2+16bits+8banks+10col+4x
#endif
{CHIPCFG_CMD_BASE, 0x1f22},
#if CONFIG_MSTAR_STR_ENABLE
_RV32_2(0x0dd4, 0x8070), //20130329  reg_cmd_mode_sel
#else
_RV32_2( 0x0dd4, 0x8000  ), //20130329  reg_cmd_mode_sel
#endif
#endif // end enable MIU!

#if CONFIG_MSTAR_STR_ENABLE
#else
{CHIPCFG_CMD_WAIT, 2},       // delay 2ms
{CHIPCFG_CMD_BASE, 0x1f22},

//MIU0
_RV32_2( 0x0d08, 0x003f  ), //reg_rx_en
#if (ENABLE_MIU1)
_RV32_2( 0x0d88, 0x003f  ), //reg_rx_en
#endif

//Yuan
//MIU0
_RV32_2( 0x1200, 0x0000  ),  //reg_init_miu
_RV32_2( 0x1200, 0x0008  ),  //power on to desert rstz > 200us
_RV32_2( 0x1200, 0x000c  ),
//MIU1
_RV32_2( 0x0600, 0x0000  ),  //reg_init_miu
_RV32_2( 0x0600, 0x0008  ),  //power on to desert rstz > 200us
_RV32_2( 0x0600, 0x000c  ),
{CHIPCFG_CMD_WAIT, 1},       // delay 1ms
//MIU0
_RV32_2( 0x1200, 0x000e  ),  //rsts -> cke on > 500us (internal state initialization)
//MIU1
_RV32_2( 0x0600, 0x000e  ),  //rsts -> cke on > 500us (internal state initialization)
{CHIPCFG_CMD_WAIT, 2},       // delay 2ms
//MIU0
_RV32_2( 0x1200, 0x001f  ),  //cke to cmd > tXPR
//MIU1
_RV32_2( 0x0600, 0x001f  ),  //cke to cmd > tXPR

//End Yuan


//Add Kaiser/KaiserS SW Patch here
//MIU0
{CHIPCFG_CMD_BASE, 0x1f22},
_RV32_2( 0x0d0e, 0x00e7  ),

{CHIPCFG_CMD_BASE, 0x1f20},
_RV32_2( 0x12f0, 0x0000  ), // SW Patch for BIST, 20130108 select MIU0
_RV32_2( 0x12e4, 0x0001  ), // 20130108 test one data
_RV32_2( 0x12e0, 0x0200  ), // 20130108 write only
_RV32_2( 0x12e0, 0x0201  ), // 20130108 write onlyd
_RV32_2( 0x12e0, 0x0201  ), // 20130108 dummy write only
_RV32_2( 0x12e0, 0x0000  ), // 20130108 return to original state
{CHIPCFG_CMD_BASE, 0x1f22},
_RV32_2( 0x0d0e, 0x00a7  ),

//MIU1
{CHIPCFG_CMD_BASE, 0x1f22},
_RV32_2( 0x0d8e, 0x00e7  ),

{CHIPCFG_CMD_BASE, 0x1f20},
_RV32_2( 0x12f0, 0x8000  ), // SW Patch for BIST, 20130108 select MIU1
_RV32_2( 0x12e4, 0x0001  ), // 20130108 test one data
_RV32_2( 0x12e0, 0x0200  ), // 20130108 write only
_RV32_2( 0x12e0, 0x0201  ), // 20130108 write only
_RV32_2( 0x12e0, 0x0201  ), // 20130108 dummy write only
_RV32_2( 0x12e0, 0x0000  ), // 20130108 return to original state
_RV32_2( 0x12f0, 0x0000  ), // 20130108 select MIU0
{CHIPCFG_CMD_BASE, 0x1f22},
_RV32_2( 0x0d8e, 0x00a7  ),

//Yuan
//MIU_Init_Priority
//MIU_Set_Group_Priority
{CHIPCFG_CMD_BASE, 0x1f20},
//MIU0 Request unmask
_RV32_2(0x1246, 0x0000),   //unmask group0
_RV32_2(0x1266, 0x0000),   //unmask group1
_RV32_2(0x1286, 0x0000),   //unmask group2
_RV32_2(0x12A6, 0x0000),   //unmask group3

_RV32_2(0x121e, 0x0c08),   //SW initial done and turn on arbitor
_RV32_2(0x061e, 0x0c08),   //SW initial done and turn on arbitor
#endif

