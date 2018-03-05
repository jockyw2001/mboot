////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2011 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef  __DRVFCIE_KAISER_UBOOT_H__
#define  __DRVFCIE_KAISER_UBOOT_H__

//#include <common.h> //printf()
//#include <sys/common/MsTypes.h>

#ifndef U32
#define U32  unsigned int
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               PLATFORM FUNCTION DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/* Enable D-CACHE */
#define MMC_CACHE  1

#define USE_SD_MODE 1
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               DATATYPE DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////
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
//
//                               RIU BANK INFOMATION
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define FCIE_OffShift           2
#define RIU_BASE                0xBF200000
#define RIU_BASE_CLKGEN0        (RIU_BASE+(0x0580U<<FCIE_OffShift))
#define RIU_BASE_CHIPTOP        (RIU_BASE+(0x0F00U<<FCIE_OffShift))

#define RIU_BASE_FCIE           (RIU_BASE+(0x8980U<<FCIE_OffShift))
#define RIU_BASE_FCIE1          (RIU_BASE+(0x89E0U<<FCIE_OffShift))



#define CIF_C_ADDR              RIU_BASE_FCIE1


//#define SDIO_IP_VERIFY

//=====================================================
// HW Timer for Delay
//=====================================================
#define RIU_PM_BASE             0xBF000000
#define RIU_BASE_TIMER0  		(RIU_PM_BASE+(0x1800<<FCIE_OffShift))
#define TIMER0_ENABLE			(RIU_BASE_TIMER0+(0x10<<FCIE_OffShift))
#define TIMER0_HIT				(RIU_BASE_TIMER0+(0x11<<FCIE_OffShift))
#define TIMER0_MAX_LOW			(RIU_BASE_TIMER0+(0x12<<FCIE_OffShift))
#define TIMER0_MAX_HIGH			(RIU_BASE_TIMER0+(0x13<<FCIE_OffShift))
#define TIMER0_CAP_LOW			(RIU_BASE_TIMER0+(0x14<<FCIE_OffShift))
#define TIMER0_CAP_HIGH			(RIU_BASE_TIMER0+(0x15<<FCIE_OffShift))
////////////////////////////////////////////////////////////////////////////////////////////////////
// RIU_BANK_TIMER1 0x0030
////////////////////////////////////////////////////////////////////////////////////////////////////
#define RIU_BASE_TIMER1  		(RIU_PM_BASE+(0x1800<<FCIE_OffShift))
#define RIU_UNIT_SHIFT           2

#define TIMER1_ENABLE                   (RIU_BASE_TIMER1+(0x20<<RIU_UNIT_SHIFT))
#define TIMER1_HIT                      (RIU_BASE_TIMER1+(0x21<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_LOW                  (RIU_BASE_TIMER1+(0x22<<RIU_UNIT_SHIFT))
#define TIMER1_MAX_HIGH                 (RIU_BASE_TIMER1+(0x23<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_LOW                  (RIU_BASE_TIMER1+(0x24<<RIU_UNIT_SHIFT))
#define TIMER1_CAP_HIGH                 (RIU_BASE_TIMER1+(0x25<<RIU_UNIT_SHIFT))

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               PLATFORM MACRO DEFINITION
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define halFCIE_PHY2MIUAddr(addr) addr

#if MMC_CACHE
#define halFCIE_Flush_Cache(base,size)  flush_cache(base, size)
#else
#define halFCIE_Flush_Cache(base,size)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               CHIPTOP
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define CHIPTOP_8		  (RIU_BASE_CHIPTOP+ (0x08<< FCIE_OffShift))
#define CHIPTOP_24   		   (RIU_BASE_CHIPTOP+ (0x24 << FCIE_OffShift))
#define CHIPTOP_25   		   (RIU_BASE_CHIPTOP+ (0x25 << FCIE_OffShift))
#define CHIPTOP_26   		   (RIU_BASE_CHIPTOP+ (0x26 << FCIE_OffShift))
#define CHIPTOP_27   		   (RIU_BASE_CHIPTOP+ (0x27 << FCIE_OffShift))
#define CHIPTOP_2A   		   (RIU_BASE_CHIPTOP+ (0x2a << FCIE_OffShift))
#define CHIPTOP_2C   		   (RIU_BASE_CHIPTOP+ (0x2c << FCIE_OffShift))

#define CHIPTOP_35		   (RIU_BASE_CHIPTOP+ (0x35 << FCIE_OffShift))
#define CHIPTOP_50   		   (RIU_BASE_CHIPTOP+ (0x50 << FCIE_OffShift))

#define ALL_PAD_IN              (RIU_BASE_CHIPTOP+(0x50<<FCIE_OffShift))

#define SD_TOP_CFG              (RIU_BASE_CHIPTOP+(0x5A<<FCIE_OffShift))
#define CFG_SD_PAD              WBIT05

#define EMMC_TOP_CFG            (RIU_BASE_CHIPTOP+(0x6E<<FCIE_OffShift))
#define CFG_EMMC_PAD            WBIT06
#define CFG_EMMC_DDR_PAD        WBIT07
#define CFG_EMMC_PAD_MASK       (WBIT07|WBIT06)

#define NAND_TOP_CFG            (RIU_BASE_CHIPTOP+(0x6F<<FCIE_OffShift))
#define PCMCIA_TOP_CFG          (RIU_BASE_CHIPTOP+(0x64<<FCIE_OffShift))

#define EMMC_BOOT_CFG          (RIU_BASE_CHIPTOP+(0x10<<FCIE_OffShift))
#define FORCE_BOOT_FROM_EMMC2   WBIT01

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               CLKGEN
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_CLK_NFIE            (RIU_BASE_CLKGEN0+(0x64<<FCIE_OffShift))
#define	REG_CLK_64				REG_CLK_NFIE
///////////////////////////////////////////////////////////////////////////////////////////////////
// Timer
///////////////////////////////////////////////////////////////////////////////////////////////////
extern void udelay (unsigned long usec);
#define FCIE_DELAY_MS(MS) udelay(MS*1000)
#define FCIE_DELAY_US(US) udelay(US)

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//                               EXTERN GLOBAL FUNCTION
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//void FCIE_HWTimer_Start(void);
//U32 FCIE_HWTimer_End(void);
extern U8 halFCIE_Platform_ClearEvent(U16 nReq);
extern void halFCIE_Platform_InitChiptop(void);
//extern E_IO_STS halFCIE_Platform_WaitMieEvent(U16 u16ReqVal, U32 u32WaitMs);
extern void halFCIE_Platform_Reset_Fcie(void);
extern U8 halFCIE_Platform_SetFcieClock(U32 u32Clock);
#endif  //__FCIE_PLAT_A5_UBOOT_H__
