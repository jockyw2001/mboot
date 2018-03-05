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
#ifndef __SD_MUNICH_UBOOT__
#define __SD_MUNICH_UBOOT__

//#include <string.h>

#define U32  unsigned int
#define U16  unsigned short
#define U8   unsigned char
#define S32  signed int


#define BIT00 0x01
#define BIT01 0x02
#define BIT02 0x04
#define BIT03 0x08
#define BIT04 0x10
#define BIT05 0x20
#define BIT06 0x40
#define BIT07 0x80

#define WBIT00 0x0001
#define WBIT01 0x0002
#define WBIT02 0x0004
#define WBIT03 0x0008
#define WBIT04 0x0010
#define WBIT05 0x0020
#define WBIT06 0x0040
#define WBIT07 0x0080
#define WBIT08 0x0100
#define WBIT09 0x0200
#define WBIT10 0x0400
#define WBIT11 0x0800
#define WBIT12 0x1000
#define WBIT13 0x2000
#define WBIT14 0x4000
#define WBIT15 0x8000

///////////////////////////////////////////////////////////////////////////////////////////////////

#define REG_OFFSET_SHIFT_BITS           2

#define REG_FCIE_U16(Reg_Addr)          (*(volatile U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)              ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_FCIE(reg_addr)              REG_FCIE_U16(reg_addr)
#define REG_FCIE_W(reg_addr, val)       REG_FCIE(reg_addr) = (val)
#define REG_FCIE_R(reg_addr, val)       val = REG_FCIE(reg_addr)
#define REG_FCIE_SETBIT(reg_addr, val)  REG_FCIE(reg_addr) |= (val)
#define REG_FCIE_CLRBIT(reg_addr, val)  REG_FCIE(reg_addr) &= ~(val)
#define REG_FCIE_W1C(reg_addr, val)     REG_FCIE_W(reg_addr, REG_FCIE(reg_addr)&(val))

#define FCIE_RIU_W16(addr,value) *((volatile U16*)(addr)) = (value)
#define FCIE_RIU_R16(addr)       *((volatile U16*)(addr))
// read modify write 16 bits register macro
#define FCIE_RIU_16_ON(addr,value) FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)|(value))
#define FCIE_RIU_16_OF(addr,value) FCIE_RIU_W16(addr, FCIE_RIU_R16(addr)&(~(value)))




////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU BASE ADDRESS
////////////////////////////////////////////////////////////////////////////////////////////////////

// kernel
//#define RIU_PM_BASE			0xFD000000
//#define RIU_BASE				0xFD200000

// uboot
//#define RIU_PM_BASE			0x1F000000
//#define RIU_BASE				0x1F200000

#define FCIE_IP					1
// munich & maldives has no SDIO IP
#define ALTERNATIVE_IP			FCIE_IP

#define SDIO_IP_VERIFY

#define RIU_BASE				0x1F000000

#define RIU_BANK_2_BASE(BANK)		(RIU_BASE+(BANK<<9))
// register num per bank --> 0x80 = 128
// register addr offset  --> 4

////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU BANK DEFINITION
////////////////////////////////////////////////////////////////////////////////////////////////////
#define RIU_BANK_TIMER1			0x0030UL
#define RIU_BANK_MIU2			0x1006UL
#define RIU_BANK_CLKGEN0		0x100BUL
#define RIU_BANK_CHIPTOP		0x101EUL
#define RIU_BANK_SDIO0			0x1113UL
#define RIU_BANK_SDIO1			0x1114UL
#define RIU_BANK_SDIO2			0x1115UL

#define RIU_BANK_EMMCPLL			0x123FUL

#define RIU_BASE_EMMCPLL			RIU_BANK_2_BASE(RIU_BANK_EMMCPLL)
#define RIU_BASE_TIMER1			RIU_BANK_2_BASE(RIU_BANK_TIMER1)
#define RIU_BASE_MIU2			RIU_BANK_2_BASE(RIU_BANK_MIU2)
#define RIU_BASE_CLKGEN0		RIU_BANK_2_BASE(RIU_BANK_CLKGEN0)
#define RIU_BASE_CHIPTOP		RIU_BANK_2_BASE(RIU_BANK_CHIPTOP)
#define RIU_BASE_SDIO0			RIU_BANK_2_BASE(RIU_BANK_SDIO0)
#define RIU_BASE_SDIO1			RIU_BANK_2_BASE(RIU_BANK_SDIO1)
#define RIU_BASE_SDIO2			RIU_BANK_2_BASE(RIU_BANK_SDIO2)

#define FCIE_REG_BASE_ADDR              (RIU_BASE_SDIO0)
#define FCIE_POWEER_SAVE_MODE_BASE		(RIU_BASE_SDIO2)
#define FCIE_CMDFIFO_BASE_ADDR          (RIU_BASE_SDIO0 + (0x20<<2)) // CIFC command FIFO
#define FCIE_CIFD_FIFO_W				(RIU_BASE_SDIO1)
#define FCIE_CIFD_FIFO_R				(RIU_BASE_SDIO1 + (0x20<<2))
#define FCIE_CRC_BUF					(RIU_BASE_SDIO1 + (0x40<<2))

#define SD_USE_FCIE5	1

#include "drvFCIE5_reg.h"

#define RIU_UNIT_SHIFT           2


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_TIMER1 0x0030
////////////////////////////////////////////////////////////////////////////////////////////////////
#define WDT_MAX_L						(RIU_BASE_TIMER1+(0x04<<RIU_UNIT_SHIFT))
#define WDT_MAX_H						(RIU_BASE_TIMER1+(0x05<<RIU_UNIT_SHIFT))
#define TIMER1_ENABLE                   (RIU_BASE_TIMER1+(0x20<<RIU_UNIT_SHIFT))
#define TIMER1_HIT                      (RIU_BASE_TIMER1+(0x21<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_LOW                  (RIU_BASE_TIMER1+(0x22<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_HIGH                 (RIU_BASE_TIMER1+(0x23<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_LOW                  (RIU_BASE_TIMER1+(0x24<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_HIGH                 (RIU_BASE_TIMER1+(0x25<<RIU_UNIT_SHIFT))


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_MIU2 0x1006
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU2_79					(RIU_BASE_MIU2+ (0x79 << RIU_UNIT_SHIFT))
#define MIU_SELECT_BY_IP		BIT04

////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CLKGEN0 0x100B
////////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_CLK_SDIO            (RIU_BASE_CLKGEN0+(0x64<<RIU_UNIT_SHIFT))

#define BIT_SDIO_CLK_GATING		BIT0
#define BIT_SDIO_CLK_INVERSE	BIT1
#define BIT_CLKGEN_SDIO_MASK	(BIT5|BIT4|BIT3|BIT2)
#define BIT_SDIO_CLK_SRC_SEL	BIT6 // 0: clk_xtal 12M, 1: clk_nfie_p1


////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_CHIPTOP 0x101E
////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIPTOP_0B				(RIU_BASE_CHIPTOP+(0x0B << RIU_UNIT_SHIFT))
#define CHIPTOP_0C				(RIU_BASE_CHIPTOP+(0x0C << RIU_UNIT_SHIFT))
#define CHIPTOP_0D				(RIU_BASE_CHIPTOP+(0x0D << RIU_UNIT_SHIFT))
#define CHIPTOP_1D				(RIU_BASE_CHIPTOP+(0x1D << RIU_UNIT_SHIFT))
#define CHIPTOP_4F				(RIU_BASE_CHIPTOP+(0x4F << RIU_UNIT_SHIFT))
#define CHIPTOP_50				(RIU_BASE_CHIPTOP+(0x50 << RIU_UNIT_SHIFT))
#define CHIPTOP_57				(RIU_BASE_CHIPTOP+(0x57 << RIU_UNIT_SHIFT))
#define CHIPTOP_58				(RIU_BASE_CHIPTOP+(0x58 << RIU_UNIT_SHIFT))
#define CHIPTOP_5A				(RIU_BASE_CHIPTOP+(0x5A << RIU_UNIT_SHIFT))
#define CHIPTOP_5B				(RIU_BASE_CHIPTOP+(0x5B << RIU_UNIT_SHIFT))
#define CHIPTOP_7B				(RIU_BASE_CHIPTOP+(0x7B << RIU_UNIT_SHIFT))
#define CHIPTOP_6E				(RIU_BASE_CHIPTOP+(0x6E << RIU_UNIT_SHIFT))
#define CHIPTOP_64 				(RIU_BASE_CHIPTOP+(0x64 << RIU_UNIT_SHIFT))

#define EMMCPLL_68 				(RIU_BASE_EMMCPLL+(0x68 << RIU_UNIT_SHIFT))
#define EMMCPLL_6D 				(RIU_BASE_EMMCPLL+(0x6D << RIU_UNIT_SHIFT))
#define EMMCPLL_70 				(RIU_BASE_EMMCPLL+(0x70 << RIU_UNIT_SHIFT))
#define EMMCPLL_71 				(RIU_BASE_EMMCPLL+(0x71 << RIU_UNIT_SHIFT))
#define EMMCPLL_73 				(RIU_BASE_EMMCPLL+(0x73 << RIU_UNIT_SHIFT))
#define EMMCPLL_74 				(RIU_BASE_EMMCPLL+(0x74 << RIU_UNIT_SHIFT))


#define HW_TIMER_DELAY_1us              1//10
#define HW_TIMER_DELAY_5us               5
#define HW_TIMER_DELAY_10us             10
#define HW_TIMER_DELAY_100us            100
#define HW_TIMER_DELAY_500us            500
#define HW_TIMER_DELAY_1ms              (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms              (5   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms             (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms            (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms            (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s               (1000 * HW_TIMER_DELAY_1ms)


int printf(const char *format, ...);




#define SDIO_MODE_GPIO_PAD_BPS			1
#define SDIO_MODE_GPIO_PAD_SDR			2
//#define SDIO_MODE_8BITS_MACRO_SDR		3
//#define SDIO_MODE_8BITS_MACRO_DDR		4
//#define SDIO_MODE_32BITS_MACRO_DDR	5
//#define SDIO_MODE_32BITS_MACRO_SDR104	6
#define SDIO_MODE_UNKNOWN				7

//#define SDIO_PAD_SDR104			SDIO_MODE_32BITS_MACRO_SDR104
//#define SDIO_PAD_SDR50			SDIO_MODE_32BITS_MACRO_SDR104
//#define SDIO_PAD_DDR50			SDIO_MODE_8BITS_MACRO_DDR
#define SDIO_PAD_SDR25			SDIO_MODE_GPIO_PAD_SDR
#define SDIO_PAD_SDR12			SDIO_MODE_GPIO_PAD_SDR


typedef struct _SKEWER {

	unsigned int u32LatchOKStart;
	unsigned int u32LatchOKEnd;
	unsigned int u32LatchBest;
	unsigned char u8_edge;

} SKEWER;

#endif /* __SD_MUNICH_UBOOT__ */


