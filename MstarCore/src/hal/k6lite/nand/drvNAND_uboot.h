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
#ifndef __UNFD_K6LITE_UBOOT_H__
#define __UNFD_K6LITE_UBOOT_H__

#include <linux/string.h>
#include <common.h>
#include <jffs2/load_kernel.h>

#define REG_BANK_CLKGEN                     (0x580)
#define REG_BANK_CHIPTOP                    (0xF00)
#define REG_BANK_FCIE0                      (0x8980)
#define REG_BANK_FCIE2                      (0x8A00)
#define REG_BANK_TIMER0                     (0x1800)
#define REG_BANK_EMMCPLL                    (0x11F80)

#define RIU_PM_BASE                         0x1F000000
#define RIU_BASE                            0x1F200000

#define REG(Reg_Addr)                       (*(volatile U16*)(Reg_Addr))
#define REG_OFFSET_SHIFT_BITS               2
#define GET_REG_ADDR(x, y)                  (x+((y)<<REG_OFFSET_SHIFT_BITS))

#define REG_WRITE_UINT16(reg_addr, val)     REG(reg_addr) = val
#define REG_READ_UINT16(reg_addr, val)      val = REG(reg_addr)
#define HAL_WRITE_UINT16(reg_addr, val)     (REG(reg_addr) = val)
#define HAL_READ_UINT16(reg_addr, val)      val = REG(reg_addr)
#define REG_SET_BITS_UINT16(reg_addr, val)  REG(reg_addr) |= (val)
#define REG_CLR_BITS_UINT16(reg_addr, val)  REG(reg_addr) &= ~(val)
#define REG_W1C_BITS_UINT16(reg_addr, val)  REG_WRITE_UINT16(reg_addr, REG(reg_addr)&(val))

#define MPLL_CLK_REG_BASE_ADDR              GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN)
#define CHIPTOP_REG_BASE_ADDR               GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)
#define FCIE_REG_BASE_ADDR                  GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE_NC_CIFD_BASE                   GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_NC_WBUF_CIFD_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_NC_RBUF_CIFD_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2 + 0x20)
#define FCIE_REG_PWS_BASE_ADDR              (RIU_PM_BASE+(0x111500<<1))

#define TIMER0_REG_BASE_ADDR                GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER0)
#define EMMC_PLL_REG_BASE_ADDR              GET_REG_ADDR(RIU_BASE, REG_BANK_EMMCPLL)

#define REG50_ECC_CTRL_INIT_VALUE           0

#define UNFD_ST_PLAT                        0x80000000
#define IF_IP_VERIFY                        0 // [CAUTION]: to verify IP and HAL code, defaut 0

// debug
#define FCIE3_DUMP_DEBUG_BUS                1

#define NC_SEL_FCIE5                        1
#if NC_SEL_FCIE5
#include "drvNAND_reg_v5.h"
#else
#error "Error! no FCIE registers selected."
#endif

#define ENABLE_32BIT_MACRO                  1
#define ENABLE_8BIT_MACRO                   0

//FCIE5 DDR Nand
#define DDR_NAND_SUPPORT                    0
#if defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT
#define DDR_EMMC_PLL                        1
//#define FCIE4_DDR_RETRY_DQS               1
#define NAND_DELAY_CELL_PS                  300 // pico-second
#endif

#define FCIE_LFSR                           1

#define SPARE640B_CIFD512B_PATCH            0

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)
#define NC_SET_DDR_MODE()                   REG_WRITE_UINT16(NC_DDR_CTRL, pNandDrv->u16_Reg58_DDRCtrl);
#define NC_CLR_DDR_MODE()                   REG_CLR_BITS_UINT16(NC_DDR_CTRL, BIT_DDR_MASM);
#else
#define NC_SET_DDR_MODE()
#define NC_CLR_DDR_MODE()
#endif

#if (defined(DDR_NAND_SUPPORT) && DDR_NAND_SUPPORT)

#if defined(ENABLE_8BIT_MACRO) && ENABLE_8BIT_MACRO
#define DQS_MODE_0P0T                       0
#define DQS_MODE_0P5T                       1
#define DQS_MODE_1P0T                       2
#define DQS_MODE_1P5T                       3
#define DQS_MODE_2P0T                       4

#define DQS_MODE_TABLE_CNT                  5
#define DQS_MODE_SEARCH_TABLE               {DQS_MODE_1P5T, DQS_MODE_2P0T, DQS_MODE_0P5T, DQS_MODE_1P0T, DQS_MODE_0P0T}

#elif defined(ENABLE_32BIT_MACRO) && ENABLE_32BIT_MACRO
#define DQS_MODE_TABLE_CNT                  32
#define DQS_MODE_SEARCH_TABLE               {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ,21, 22, 23 ,24 ,25, 26, 27, 28, 29 ,30, 31}

#define SKEW_CLK_PHASE_CNT                  4

#endif

#define	NC_ONFI_DEFAULT_TRR                 12
#define	NC_ONFI_DEFAULT_TCS                 7
#define NC_ONFI_DEFAULT_TWW                 8
#define NC_ONFI_DEFAULT_TWHR                5
#define NC_ONFI_DEFAULT_TADL                6
#define NC_ONFI_DEFAULT_TCWAW               4
#define	NC_ONFI_DEFAULT_RX40CMD             4
#define	NC_ONFI_DEFAULT_RX40ADR             7
#define	NC_ONFI_DEFAULT_RX56                10

#define	NC_TOGGLE_DEFAULT_TRR               8
#define	NC_TOGGLE_DEFAULT_TCS               6
#define NC_TOGGLE_DEFAULT_TWW               7
#define	NC_TOGGLE_DEFAULT_TWHR              5
#define	NC_TOGGLE_DEFAULT_TADL              7
#define	NC_TOGGLE_DEFAULT_TCWAW             2
#define	NC_TOGGLE_DEFAULT_RX40CMD           4
#define	NC_TOGGLE_DEFAULT_RX40ADR           5
#define	NC_TOGGLE_DEFAULT_RX56              15
#endif

#define	NC_SDR_DEFAULT_TRR                  7
#define	NC_SDR_DEFAULT_TCS                  6
#define NC_SDR_DEFAULT_TWW                  5
#define	NC_SDR_DEFAULT_TWHR                 4
#define	NC_SDR_DEFAULT_TADL                 8
#define	NC_SDR_DEFAULT_TCWAW                2
#define	NC_SDR_DEFAULT_RX40CMD              4
#define	NC_SDR_DEFAULT_RX40ADR              5
#define	NC_SDR_DEFAULT_RX56                 5

#define	NC_INST_DELAY                       1
#define	NC_HWCMD_DELAY                      1
#define	NC_TRR_TCS                          1
#define	NC_TWHR_TCLHZ                       1
#define	NC_TCWAW_TADL                       1


#if IF_IP_VERIFY
/* select a default NAND chip for IP_VERIFY or NAND_PROGRAMMER */
//#define K9GAG08U0E                          1
//#define H27UAG8T2M                          1
//#define NAND512W3A2C                        1
//#define K9F1G08X0C                          1
//#define TH58TVG7D2GBA                       1 // <- Toggle
//#define MT29F64G08CBAAB                     1 // <- ONFI
//#define H27UAG8T2A                          1
//#define H27UF081G2A                         1
//#define K511F12ACA                          1
//#define TY8A0A111162KC40                    1
//#define K522H1GACE                          1
//#define TY890A111229KC40                    1
//#define H8ACU0CE0DAR                        1
//#define H9LA25G25HAMBR                      1
//#define TY8A0A111178KC4                     1
//#define HY27UF082G2B                        1
//#define H27UCG8V5M                          1 //4k page
//#define H27U2G8F2CTR                        1
//#define H27U4G8F2DTR                        1
#define MT29F4G08ABAEA                      1

#include "../common/drvNAND_device.h"
#endif

#define IF_SPARE_AREA_DMA                   0 // [CAUTION]

#define NAND_PAD_BYPASS_MODE                1
#define NAND_PAD_TOGGLE_MODE                2
#define NAND_PAD_ONFI_SYNC_MODE             3

#define MACRO_TYPE_8BIT                     1
#define MACRO_TYPE_32BIT                    2

#define IF_FCIE_SHARE_PINS                  0 // 1: need to nand_pads_switch at HAL's functions.
#define IF_FCIE_SHARE_CLK                   0 // 1: need to nand_clock_setting at HAL's functions.
#define IF_FCIE_SHARE_IP                    0

#define RANDOM_WR_THR_CIFD                  0
#define ENABLE_NAND_INTERRUPT_MODE          0

#define NAND_DRIVER_ROM_VERSION             0 // to save code size
#define AUTO_FORMAT_FTL                     0

#define ENABLE_CUS_READ_ENHANCEMENT         0

//FCIE5 Definition

#define SPARE_DMA_ADDR_AUTO_INC             0 //spare would increase its address when dma

//=====================================================
#define ENABLE_READ_DISTURBANCE_SUPPORT     0
#define DEBUG_REG_ECC_STATUS                NC_CF_SSO_CTL // choose dummy reg from FCIE or CHIPTOP

// 2 bits ECC status
#define ECC_STATUS_NORMAL                   0
#define ECC_STATUS_BITFLIP                  BIT0
#define ECC_STATUS_ERROR                    BIT1

//---------------------------------------------------------------------
//         RESERVED          |   BL   | UBOOT | HASH2 | HASH1 | HASH0 |
//---------------------------------------------------------------------
#define BIT_ECC_STATUS_HASH0_SHIFT          0
#define BIT_ECC_STATUS_HASH1_SHIFT          2
#define BIT_ECC_STATUS_HASH2_SHIFT          4
#define BIT_ECC_STATUS_UBOOT_SHIFT          6
#define BIT_ECC_STATUS_BL_SHIFT             8

#define HASH0_SIZE                          (10 * 1024)
#define HASH1_SIZE                          (10 * 1024)
#define HASH2_SIZE                          (10 * 1024) // TBD
#define UBOOT_SIZE                          (512 * 1024) // TBD

extern U32 nand_ReadDisturbance(void);
//=====================================================

#if defined (CONFIG_MSTAR_UNFD_FTL)
#define __VER_UNFD_FTL__          1
#define BLD_LD_OS_RD               0
#define ASSERT_FTL_ACCESS_BLK0     0
#else
#define __VER_UNFD_FTL__                    0
#endif

//=====================================================
// Nand Driver configs
//=====================================================
#define NAND_ENV_FPGA                       1
#define NAND_ENV_ASIC                       2
#ifdef __FPGA_MODE__  // currently G1 don't care this macro, even in ROM code
#define NAND_DRIVER_ENV                     NAND_ENV_FPGA
#else
#define NAND_DRIVER_ENV                     NAND_ENV_ASIC
#endif

#define UNFD_CACHE_LINE                     0x80
//=====================================================
// tool-chain attributes
//=====================================================
#define UNFD_PACK0
#define UNFD_PACK1                          __attribute__((__packed__))
#define UNFD_ALIGN0
#define UNFD_ALIGN1                         __attribute__((aligned(UNFD_CACHE_LINE)))

//=====================================================
// debug option
//=====================================================
#define NAND_TEST_IN_DESIGN                 0      /* [CAUTION] */

#ifndef NAND_DEBUG_MSG
#define NAND_DEBUG_MSG                      1
#endif

/* Define trace levels. */
#define UNFD_DEBUG_LEVEL_ERROR              (1)    /* Error condition debug messages. */
#define UNFD_DEBUG_LEVEL_WARNING            (2)    /* Warning condition debug messages. */
#define UNFD_DEBUG_LEVEL_HIGH               (3)    /* Debug messages (high debugging). */
#define UNFD_DEBUG_LEVEL_MEDIUM             (4)    /* Debug messages. */
#define UNFD_DEBUG_LEVEL_LOW                (5)    /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef UNFD_DEBUG_LEVEL
#define UNFD_DEBUG_LEVEL                    UNFD_DEBUG_LEVEL_WARNING
#endif

extern int Disable_Err_log;
#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
#define nand_print_tmt                      //msPrintfFunc
#define nand_printf                         printf
#define nand_debug(dbg_lv, tag, str, ...)       \
    do {                        \
        if (dbg_lv > UNFD_DEBUG_LEVEL || Disable_Err_log == 1)        \
            break;                  \
        else {                    \
            if (dbg_lv == UNFD_DEBUG_LEVEL_ERROR) \
                nand_printf("NAND Err:");           \
            if (tag)                  \
            {                                       \
                nand_printf("[%s]\t",__func__);        \
                /*nand_print_tmt(__func__);*/       \
            }                                       \
                          \
            nand_printf(str, ##__VA_ARGS__);      \
            /*nand_print_tmt(str, ##__VA_ARGS__);*/     \
        }                     \
    } while(0)
#else /* NAND_DEBUG_MSG */
#define nand_printf(...)
#define nand_debug(enable, tag, str, ...)   {}
#endif /* NAND_DEBUG_MSG */

static __inline void nand_assert(int condition)
{
    if (!condition) {
    }
}

#define nand_die() \
    do { \
        nand_printf(__func__); \
        nand_printf("\nUNFD Assert(%d)", __LINE__); \
        nand_assert(0); \
    } while(0);

#define nand_stop() \
    while(1)  nand_reset_WatchDog();

//=====================================================
// HW Timer for Delay
//=====================================================
#define TIMER0_ENABLE                       GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x20)
#define TIMER0_HIT                          GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x21)
#define TIMER0_MAX_LOW                      GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x22)
#define TIMER0_MAX_HIGH                     GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x23)
#define TIMER0_CAP_LOW                      GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x24)
#define TIMER0_CAP_HIGH                     GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x25)

#define HW_TIMER_DELAY_1us                  1
#define HW_TIMER_DELAY_10us                 10
#define HW_TIMER_DELAY_100us                100
#define HW_TIMER_DELAY_1ms                  (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms                  (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms                 (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms                (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms                (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s                   (1000 * HW_TIMER_DELAY_1ms)

#define NAND_SPEED_TEST                     0

extern void delay_us( unsigned us );
extern U32  nand_hw_timer_delay(U32 u32usTick);

//=====================================================
// Pads Switch
//=====================================================
#define REG_NAND_MODE_MASK                  (BIT3) 
#define NAND_MODE1                          (BIT3) 

#define reg_allpad_in                       GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define reg_emmc_config                     GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x08)
#define reg_nf_en                           GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x08)
#define reg_sd_config                       GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x08)
#define reg_nand_mode                       GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x08)
#define reg_nc_ddr_onfi                     GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x58)
#define reg_nc_ddr_toggle                 	GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x58)
#define reg_nc_pad_from_ddr                 GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x58)

#define reg_nand_en                   		GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x60)
#define reg_ddr_io_mode                     GET_REG_ADDR(EMMC_PLL_REG_BASE_ADDR, 0x6d)

#define reg_nand_pe_0_15					GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x2e)
#define reg_nand_ps_0_15					GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x2f)
#define reg_nand_pe_16_17					GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x31)
#define reg_nand_ps_16_17					GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x31)


//@FIXME
#define REG_NAND_DQS_UL                     GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x2f)
#define BIT_DQS_PULL_H                      BIT8
#define NC_DQS_PULL_H()                     REG_SET_BITS_UINT16(REG_NAND_DQS_UL, BIT_DQS_PULL_H)
#define NC_DQS_PULL_L()                     REG_CLR_BITS_UINT16(REG_NAND_DQS_UL, BIT_DQS_PULL_H)

extern U32 nand_pads_switch(U32 u32EnableFCIE);
extern U32 	nand_check_DDR_pad(void);

//=====================================================
// set FCIE clock
//=====================================================


#define ENABLE_DELAY_CELL                   0

#if defined(ENABLE_DELAY_CELL) && ENABLE_DELAY_CELL
#undef DQS_MODE_SEARCH_TABLE
#define DQS_MODE_SEARCH_TABLE               {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
#endif

#define DECIDE_CLOCK_BY_NAND                1

#define NFIE_CLK_MASK                       (BIT13|BIT12|BIT11|BIT10)
#define NFIE_CLK_12M                        (1<<10)
#define NFIE_CLK_15_5M                      (2<<10)
#define NFIE_CLK_18M                        (3<<10)
#define NFIE_CLK_21_5M                      (4<<10)
#define NFIE_CLK_27M                        (5<<10)
#define NFIE_CLK_36M                        (6<<10)
#define NFIE_CLK_40M                        (7<<10)
#define NFIE_CLK_48M                        (8<<10)
#define NFIE_CLK_54M                        (9<<10)
#define NFIE_CLK_60M                        (10<<10)
//#define NFIE_CLK_88_5M                      (11<<10)
//#define NFIE_CLK_75K                        (12<<10)
#define NFIE_CLK_80M                        (13<<10)



#define NFIE_CLK_TABLE_CNT	11
#define NFIE_CLK_TABLE	{	NFIE_CLK_12M, NFIE_CLK_15_5M, NFIE_CLK_18M, \
							NFIE_CLK_21_5M, NFIE_CLK_27M, NFIE_CLK_36M, NFIE_CLK_40M, \
							NFIE_CLK_48M, NFIE_CLK_54M, NFIE_CLK_60M, NFIE_CLK_80M}

#define NFIE_CLK_TABLE_STR	{	"12M", "15.5M", "18M",\
							"21.5M", "27M", "36M", "40M", \
							"48M", "54M", "60M", \
							"80M"}

#define NFIE_12M_VALUE      12000000
#define NFIE_15_5M_VALUE    15500000
#define NFIE_18M_VALUE      18000000
#define NFIE_21_5M_VALUE    21500000
#define NFIE_27M_VALUE      27000000
#define NFIE_36M_VALUE      36000000
#define NFIE_40M_VALUE      40000000
#define NFIE_48M_VALUE      48000000
#define NFIE_54M_VALUE    	54000000
#define NFIE_60M_VALUE      60000000
//#define NFIE_88_5M_VALUE    88500000
//#define NFIE_75K_VALUE      75000
#define NFIE_80M_VALUE      80000000




#define NFIE_CLK_VALUE_TABLE	{	NFIE_12M_VALUE, NFIE_15_5M_VALUE, NFIE_18M_VALUE, NFIE_21_5M_VALUE, \
							NFIE_27M_VALUE, NFIE_36M_VALUE, NFIE_40M_VALUE, NFIE_48M_VALUE, \
							NFIE_54M_VALUE, NFIE_60M_VALUE, NFIE_80M_VALUE}


/*Define 1 cycle Time for each clock note: define value must be the (real value -1)*/
#define NFIE_1T_12M         83
#define NFIE_1T_15_5M       64
#define NFIE_1T_18M         55
#define NFIE_1T_21_5M       46
#define NFIE_1T_27M         37
#define NFIE_1T_36M         27
#define NFIE_1T_40M         25
#define NFIE_1T_48M         20
#define NFIE_1T_54M         18
#define NFIE_1T_60M         16
//#define NFIE_1T_88_5M       11
#define NFIE_1T_80M         12




#define NFIE_1T_TABLE		{	NFIE_1T_12M, NFIE_1T_15_5M, NFIE_1T_18M, \
							NFIE_1T_21_5M, NFIE_1T_27M, NFIE_1T_36M, NFIE_1T_40M, \
							NFIE_1T_48M, NFIE_1T_54M, NFIE_1T_60M, \
							NFIE_1T_80M}


//4x CLK
#define NFIE_4CLK_12M                       (0<<10)
#define NFIE_4CLK_48M                       (1<<10)
#define NFIE_4CLK_62M                       (2<<10)
#define NFIE_4CLK_72M                       (3<<10)
#define NFIE_4CLK_86M                       (4<<10)
#define NFIE_4CLK_108M                      (5<<10)
#define NFIE_4CLK_144M                      (6<<10)
#define NFIE_4CLK_160M                      (7<<10)
#define NFIE_4CLK_192M                      (8<<10)
#define NFIE_4CLK_216M                      (9<<10)
#define NFIE_4CLK_240M                      (10<<10)
//#define NFIE_4CLK_354M                      (11<<10)
#define NFIE_4CLK_320M                      (13<<10)

#define NFIE_4CLK_TABLE_CNT                 12


#define NFIE_4CLK_TABLE                     { NFIE_4CLK_12M, NFIE_4CLK_48M, NFIE_4CLK_62M, \
                                              NFIE_4CLK_72M, NFIE_4CLK_86M, NFIE_4CLK_108M, NFIE_4CLK_144M, \
                                              NFIE_4CLK_160M, NFIE_4CLK_192M, NFIE_4CLK_216M, NFIE_4CLK_240M, \
                                              NFIE_4CLK_320M}

#define NFIE_4CLK_TABLE_STR                 { "12M", "48M", "62M", "72M",\
                                              "86M", "108", "144M", "160M", \
                                              "192M", "216M", "240M", "320M" }


#define NFIE_4CLK_12M_VALUE      12000000
#define NFIE_4CLK_48M_VALUE      48000000
#define NFIE_4CLK_62M_VALUE      62000000
#define NFIE_4CLK_72M_VALUE      72000000
#define NFIE_4CLK_86M_VALUE      86000000
#define NFIE_4CLK_108M_VALUE     108000000
#define NFIE_4CLK_144M_VALUE     144000000
#define NFIE_4CLK_160M_VALUE     160000000
#define NFIE_4CLK_192M_VALUE     192000000
#define NFIE_4CLK_216M_VALUE     216000000
#define NFIE_4CLK_240M_VALUE     240000000
//#define NFIE_4CLK_354M_VALUE     354000000
#define NFIE_4CLK_320M_VALUE     320000000


#define NFIE_4CLK_VALUE_TABLE               { NFIE_4CLK_12M_VALUE, NFIE_4CLK_48M_VALUE, NFIE_4CLK_62M_VALUE, NFIE_4CLK_72M_VALUE, \
                                              NFIE_4CLK_86M_VALUE, NFIE_4CLK_108M_VALUE, NFIE_4CLK_144M_VALUE, NFIE_4CLK_160M_VALUE, \
                                              NFIE_4CLK_192M_VALUE, NFIE_4CLK_216M_VALUE, NFIE_4CLK_240M_VALUE, NFIE_4CLK_320M_VALUE}

#define NFIE_4CLK_1T_12M        83
#define NFIE_4CLK_1T_48M        20
#define NFIE_4CLK_1T_62M        16
#define NFIE_4CLK_1T_72M        13
#define NFIE_4CLK_1T_86M        11
#define NFIE_4CLK_1T_108M       9
#define NFIE_4CLK_1T_144M       6
#define NFIE_4CLK_1T_160M       6
#define NFIE_4CLK_1T_192M       5
#define NFIE_4CLK_1T_216M       4
#define NFIE_4CLK_1T_240M       4
//#define NFIE_4CLK_1T_354M       2
#define NFIE_4CLK_1T_320M       3


#define NFIE_4CLK_1T_TABLE                  { NFIE_4CLK_1T_12M, NFIE_4CLK_1T_48M, NFIE_4CLK_1T_62M, \
                                              NFIE_4CLK_1T_72M, NFIE_4CLK_1T_86M, NFIE_4CLK_1T_108M, NFIE_4CLK_1T_144M, \
                                              NFIE_4CLK_1T_160M, NFIE_4CLK_1T_192M, NFIE_4CLK_1T_216M, NFIE_4CLK_1T_240M, \
                                              NFIE_4CLK_1T_320M}

#define DUTY_CYCLE_PATCH                    0 // 1: to enlarge low width for tREA's worst case of 25ns
#if DUTY_CYCLE_PATCH
#define FCIE3_SW_DEFAULT_CLK                NFIE_1T_80M
#define FCIE_REG41_VAL                      ((2<<9)|(2<<3)) // RE,WR pulse, Low:High=3:1
#define REG57_ECO_FIX_INIT_VALUE            0
#else
#define FCIE3_SW_DEFAULT_CLK                NFIE_CLK_54M
#define FCIE_REG41_VAL                      0               // RE,WR pulse, Low:High=1:1
#define REG57_ECO_FIX_INIT_VALUE            BIT_NC_LATCH_DATA_1_0_T // delay 1.0T
#endif
#define FCIE3_SW_SLOWEST_CLK                NFIE_CLK_12M

#define NAND_SEQ_ACC_TIME_TOL               16 //in unit of ns

#define reg_ckg_fcie                        GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x64)
#define BIT_CLK_SELECT                      BIT14

#define reg_clk_nfie_div4_en        		GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x64)
#define BIT_CLK_DIV4_ENABLE         		BIT7


//=====================================================
// set ECC clock
//=====================================================
#define REG_BANK_CLKGEN2            0x500
#define CLKGEN2_BASE                GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN2)
#define reg_ckg_ecc                 GET_REG_ADDR(CLKGEN2_BASE, 0x01)

#define ECC_CLK_GATING              (BIT0)
#define ECC_CLK_INVERSE             (BIT1)
#define ECC_CLK_MASK                (BIT4|BIT3|BIT2)
#define ECC_CLK_12M                 (0<<2)
#define ECC_CLK_54M                 (1<<2)
#define ECC_CLK_108M                (2<<2)
#define ECC_CLK_160M                (3<<2)
#define ECC_CLK_216M                (4<<2)
#define ECC_SW_DEFAULT_CLK          ECC_CLK_108M

extern U32  nand_clock_setting(U32 u32ClkParam);

extern void nand_DumpPadClk(void);

//=====================================================
// transfer DMA Address
//=====================================================
/*
 * Important:
 * The following buffers should be large enough for a whole NAND block
 */
// FIXME, this is only for verifing IP
#define DMA_W_ADDR                          0x21000000
#define DMA_R_ADDR                          0x21400000
#define DMA_W_SPARE_ADDR                    0x21800000
#define DMA_R_SPARE_ADDR                    0x21900000
#define DMA_BAD_BLK_BUF                     0x21A00000

#define DMA_W_SRAM_ADDR                     0xFDC00000
#define DMA_R_SRAM_ADDR                     0xFDC00800
#define DMA_W_SRAM_SPARE_ADDR               0xFDC01000
#define DMA_R_SRAM_SPARE_ADDR               0xFDC01800


#define MIU_CHECK_LAST_DONE                 1

//=====================================================
// misc
//=====================================================

#define NC_REG_MIU_LAST_DONE                NC_MIE_EVENT

extern void flush_cache(U32 start_addr, U32 size);
extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void);

extern int memcmp(const void *,const void *, unsigned int);
extern void *malloc(unsigned int);
extern void free(void*);

int printf(const char *format, ...);

#define NANDINFO_ECC_TYPE                   ECC_TYPE_40BIT1KB

#endif //__UNFD_K6LITE_UBOOT_H__

