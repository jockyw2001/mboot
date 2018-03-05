/*
 *#############################################################################
 *
 * Copyright (c) 2006-2011 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * Unless otherwise stipulated in writing, any and all information contained
 * herein regardless in any format shall remain the sole proprietary of
 * MStar Semiconductor Inc. and be kept in strict confidence
 * (¡§MStar Confidential Information¡¨) by the recipient.
 * Any unauthorized act including without limitation unauthorized disclosure,
 * copying, use, reproduction, sale, distribution, modification, disassembling,
 * reverse engineering and compiling of the contents of MStar Confidential
 * Information is unlawful and strictly prohibited. MStar hereby reserves the
 * rights to any and all damages, losses, costs and expenses resulting therefrom.
 *
 *#############################################################################
 */

#ifndef _HAL_SPINOR_H_
#define _HAL_SPINOR_H_


#if defined(CONFIG_MCU_ARM)
#define CONFIG_SRAM_SPINAND_OFFSET  0x1FC02800 //512B Reserved for SPINAND Flash Driver
#define CONFIG_SRAM_SPINAND_BUF     (0x1FC02800+0x100)
#define CONFIG_RIU_BASE_ADDRESS     0x1F000000
#define RIU_BASE                    0x1F200000

#elif defined(CONFIG_MCU_MIPS32)
#define CONFIG_RIU_BASE_ADDRESS     0xBF000000
#define RIU_BASE                    0xBF200000
#define CONFIG_SRAM_SPINAND_OFFSET  0xB2003800 //512B Reserved for SPINAND Flash Driver
#define CONFIG_SRAM_SPINAND_BUF     (0xB2003800+0x100)
#else
#error  "Unknown CPU Platform"
#endif
#define CONFIG_XIU_BASE_ADDRESS     0x1F600000
#define TSP_SRAM_BASE_ADDRESS       0xB2000000

#define REG_OFFSET_SHIFT_BITS       2
#define GET_REG_ADDR(x, y)          ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_BANK_PORTSTATUS         0x0280U
#define PORTSTATUS_BASE             GET_REG_ADDR(RIU_BASE, REG_BANK_PORTSTATUS)
#define BOOTSTAGE_REG               GET_REG_ADDR(PORTSTATUS_BASE, 0x7)

#define DEBUG_REG                   GET_REG_ADDR(PORTSTATUS_BASE, 0x01)
#define DEBUG_REG_SYS_L1            GET_REG_ADDR(PORTSTATUS_BASE, 0x02)
#define DEBUG_REG_SYS_L2            GET_REG_ADDR(PORTSTATUS_BASE, 0x03)

#define REG_READ_UINT16(reg_addr, val)      val = REG((reg_addr))
#define REG_WRITE_UINT16(reg_addr, val)     REG((reg_addr)) = (val)
#define REG_SET_BITS_UINT16(reg_addr, val)  REG((reg_addr)) |= (val)

#define REG_BANK_TIMER0                     0x1800U
#define TIMER0_REG_BASE_ADDR				GET_REG_ADDR(CONFIG_RIU_BASE_ADDRESS, REG_BANK_TIMER0)

#define TIMER0_ENABLE						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x10)
#define TIMER0_HIT							GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x11)
#define TIMER0_MAX_LOW						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x12)
#define TIMER0_MAX_HIGH						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x13)
#define TIMER0_CAP_LOW						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x14)
#define TIMER0_CAP_HIGH						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x15)

#ifndef TRUE
#define TRUE                        1
#endif

#ifndef FALSE
#define FALSE                       0
#endif

#ifndef U32
#define U32  unsigned long
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

#ifndef BOOL
#define BOOL unsigned char
#endif
#define REG(Reg_Addr)           (*(volatile unsigned short *)(Reg_Addr))
#define SECURE_DEBUG(reg_addr, val)      REG((reg_addr)) = (val)
#define ALIGN_8(_x_)                (((_x_) + 7) & ~7)
#define RIU     ((unsigned short volatile *) CONFIG_RIU_BASE_ADDRESS)
#define RIU8    ((unsigned char  volatile *) CONFIG_RIU_BASE_ADDRESS)
#define XIU     ((unsigned int   volatile *) CONFIG_XIU_BASE_ADDRESS)
#define XIU8    ((unsigned char  volatile *) CONFIG_XIU_BASE_ADDRESS)

#define BIT(_bit_)                  (1 << (_bit_))
#define BIT_(x)                     BIT(x) //[OBSOLETED] //TODO: remove it later
#define BITS(_bits_, _val_)         ((BIT(((1)?_bits_)+1)-BIT(((0)?_bits_))) & (_val_<<((0)?_bits_)))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))

#define FSP_BASE_ADDR        (0x001600)
#define QSPI_BASE_ADDR       (0x001700)
#define CLK_BASE_ADDR        (0x100B00)
#define PMCLK_BASE_ADDR      (0x000F00)
#define BDMA_BASE_ADDR       (0x100900)
#define MBX_BASE_ADDR        (0x103380)
#define CHIP_BASE_ADDR       (0x101E00)
#define CHK_NUM_WAITDONE     0x10000

// CLK Register

#define REG_CLK0_CKG_SPI            0x16
    #define CLK0_CKG_SPI_MASK           BMASK(4:2)
    #define CLK0_CKG_SPI_XTALI              BITS(4:2, 0)
    #define CLK0_CKG_SPI_27MHZ              BITS(4:2, 1)
    #define CLK0_CKG_SPI_36MHZ              BITS(4:2, 2)
    #define CLK0_CKG_SPI_43MHZ              BITS(4:2, 3)
    #define CLK0_CKG_SPI_54MHZ              BITS(4:2, 4)
    #define CLK0_CKG_SPI_72MHZ              BITS(4:2, 5)
    #define CLK0_CKG_SPI_86MHZ              BITS(4:2, 6)
    #define CLK0_CKG_SPI_108MHZ             BITS(4:2, 7)
    #define CLK0_CLK_SWITCH_MASK        BMASK(5:5)
    #define CLK0_CLK_SWITCH_OFF           BITS(5:5, 0)
    #define CLK0_CLK_SWITCH_ON            BITS(5:5, 1)

// PM_SLEEP CMD.
#define REG_PM_CKG_SPI              0x20 // Ref spec. before using these setting.
    #define PM_SPI_CLK_SEL_MASK         BMASK(13:10)
    #define PM_SPI_CLK_XTALI                BITS(13:10, 0)
    #define PM_SPI_CLK_27MHZ                BITS(13:10, 1)
    #define PM_SPI_CLK_36MHZ                BITS(13:10, 2)
    #define PM_SPI_CLK_43MHZ                BITS(13:10, 3)
    #define PM_SPI_CLK_54MHZ                BITS(13:10, 4)
    #define PM_SPI_CLK_72MHZ                BITS(13:10, 5)
    #define PM_SPI_CLK_86MHZ                BITS(13:10, 6)
    #define PM_SPI_CLK_108MHZ               BITS(13:10, 7)
    #define PM_SPI_CLK_24MHZ                BITS(13:10, 15)
    #define PM_SPI_CLK_SWITCH_MASK      BMASK(14:14)
    #define PM_SPI_CLK_SWITCH_OFF           BITS(14:14, 0)
    #define PM_SPI_CLK_SWITCH_ON            BITS(14:14, 1)

// FSP Register
#define FLASH_PAGE_SIZE            256
#define FLASH_OIP                  1
//QSPI
#define REG_SPI_BURST_WRITE 0x0A
#define REG_SPI_DISABLE_BURST 0x03
#define REG_SPI_ENABLE_BURST  0x01
#define REG_SPI_TO_HARDWARE  0x02

#define REG_FSP_DONE_BIT     0x6E * 2
#define DONE_FSP               1

#define REG_FSP_WDB0    0x60
#define REG_FSP_WDB0_MASK       BMASK(7:0)
#define REG_FSP_WDB0_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB1    0x60
#define REG_FSP_WDB1_MASK       BMASK(15:8)
#define REG_FSP_WDB1_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB2    0x61
#define REG_FSP_WDB2_MASK       BMASK(7:0)
#define REG_FSP_WDB2_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB3    0x61
#define REG_FSP_WDB3_MASK       BMASK(15:8)
#define REG_FSP_WDB3_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB4    0x62
#define REG_FSP_WDB4_MASK       BMASK(7:0)
#define REG_FSP_WDB4_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB5    0x62
#define REG_FSP_WDB5_MASK       BMASK(15:8)
#define REG_FSP_WDB5_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB6    0x63
#define REG_FSP_WDB6_MASK       BMASK(7:0)
#define REG_FSP_WDB6_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB7    0x63
#define REG_FSP_WDB7_MASK       BMASK(15:8)
#define REG_FSP_WDB7_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB8    0x64
#define REG_FSP_WDB8_MASK       BMASK(7:0)
#define REG_FSP_WDB8_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB9    0x64
#define REG_FSP_WDB9_MASK       BMASK(15:8)
#define REG_FSP_WDB9_DATA(d)    BITS(15:8, d)
#define REG_FSP_WBF_SIZE   0x6a
#define REG_FSP_WBF_SIZE0_MASK   BMASK(3:0)
#define REG_FSP_WBF_SIZE0(s)    BITS(3:0,s)
#define REG_FSP_WBF_SIZE1_MASK   BMASK(7:4)
#define REG_FSP_WBF_SIZE1(s)    BITS(7:4,s)
#define REG_FSP_WBF_SIZE2_MASK   BMASK(11:8)
#define REG_FSP_WBF_SIZE2(s)    BITS(11:8,s)
#define REG_FSP_RBF_SIZE   0x6b
#define REG_FSP_RBF_SIZE0_MASK   BMASK(3:0)
#define REG_FSP_RBF_SIZE0(s)    BITS(3:0,s)
#define REG_FSP_RBF_SIZE1_MASK   BMASK(7:4)
#define REG_FSP_RBF_SIZE1(s)    BITS(7:4,s)
#define REG_FSP_RBF_SIZE2_MASK   BMASK(11:8)
#define REG_FSP_RBF_SIZE2(s)    BITS(11:8,s)

#define REG_FSP_CTRL   0x6c
#define REG_FSP_ENABLE_MASK     BMASK(0:0)
#define REG_FSP_ENABLE          BITS(0:0,1)
#define REG_FSP_DISABLE         BITS(0:0,0)
#define REG_FSP_RESET_MASK      BMASK(1:1)
#define REG_FSP_RESET           BITS(1:1,0)
#define REG_FSP_NRESET          BITS(1:1,1)
#define REG_FSP_INT_MASK        BMASK(2:2)
#define REG_FSP_INT             BITS(2:2,1)
#define REG_FSP_INT_OFF         BITS(2:2,0)
#define REG_FSP_CHK_MASK        BMASK(3:3)
#define REG_FSP_CHK             BITS(3:3,1)
#define REG_FSP_CHK_OFF         BITS(3:3,0)
#define REG_FSP_STS_BUSY         BITS(10:8,0)
#define REG_FSP_STS_MASK         BMASK(10:8)
#define REG_FSP_RDSR_MASK       BMASK(12:11)
#define REG_FSP_1STCMD          BITS(12:11,0)
#define REG_FSP_2NDCMD          BITS(12:11,1)
#define REG_FSP_3THCMD          BITS(12:11,2)
#define REG_FSP_FSCHK_MASK        BMASK(13:13)
#define REG_FSP_FSCHK_ON          BITS(13:13,1)
#define REG_FSP_FSCHK_OFF         BITS(13:13,0)
#define REG_FSP_3THCMD_MASK        BMASK(14:14)
#define REG_FSP_3THCMD_ON          BITS(14:14,1)
#define REG_FSP_3THCMD_OFF         BITS(14:14,0)
#define REG_FSP_2NDCMD_MASK        BMASK(15:15)
#define REG_FSP_2NDCMD_ON          BITS(15:15,1)
#define REG_FSP_2NDCMD_OFF         BITS(15:15,0)
#define REG_FSP_TRIGGER   0x6d
#define REG_FSP_TRIGGER_MASK        BMASK(0:0)
#define REG_FSP_FIRE             BITS(0:0,1)
#define REG_FSP_DONE_FLAG   0x6e
#define REG_FSP_DONE_FLAG_MASK        BMASK(0:0)
#define REG_FSP_DONE             BITS(0:0,1)
#define REG_FSP_DONE_CLR   0x6f
#define REG_FSP_DONE_CLR_MASK        BMASK(0:0)
#define REG_FSP_CLR             BITS(0:0,1)

#define REG_FSP_WRITEDATA_SIZE     0x4
#define REG_FSP_MAX_WRITEDATA_SIZE 0xA
#define REG_FSP_READDATA_SIZE      0xA
#define REG_FSP_RDB0    0x65
#define REG_FSP_RDB1    0x65
#define REG_FSP_RDB2    0x66
#define REG_FSP_RDB3    0x66
#define REG_FSP_RDB4    0x67
#define REG_FSP_RDB5    0x67
#define REG_FSP_RDB6    0x68
#define REG_FSP_RDB7    0x68
#define REG_FSP_RDB8    0x69
#define REG_FSP_RDB9    0x69

#define SERFLASH_BLOCK_SIZE  0x10000

#define SPI_CMD_SE			(0x20)
#define SPI_CMD_32BE		(0x52)
#define SPI_CMD_64BE		(0xD8)

typedef enum
{
    FLASH_ERASE_04K       = SPI_CMD_SE,
    FLASH_ERASE_32K       = SPI_CMD_32BE,
    FLASH_ERASE_64K       = SPI_CMD_64BE
} EN_FLASH_ERASE;

U32 HAL_SPINAND_Read (U32 u32Addr, U32 u32DataSize, U8 *pu8Data);
U8  HAL_SPINAND_ReadID(U32 u32DataSize, U8 *pu8Data);
U32 HAL_SPINAND_RFC(U32 u32Addr, U8 *pu8Data);
BOOL HAL_SPINAND_Init(void);
BOOL HAL_SPINAND_PLANE_HANDLER(U32 u32Addr);
void HAL_SPINAND_SetReadMode(BOOL bRIUFlag);
BOOL HAL_SPINOR_SetCKG(U8 u8CkgSpi);
BOOL HAL_SERFLASH_BlockErase(U32 u32StartBlock, U32 u32EndBlock, BOOL bWait);
BOOL HAL_SERFLASH_SectorErase(U32 u32SectorAddress);
BOOL HAL_SERFLASH_Write(U32 u32Addr, U32 u32Size, U8 *pu8Data);
BOOL HAL_SERFLASH_Read(U32 u32Addr, U32 u32Size, U8 *pu8Data);
BOOL HAL_FSP_WriteProtect(BOOL bEnable);

#if SPINAND_DEBUG
BOOL HAL_SPINAND_BDMA_CHECK(U32 u32Addr, U32 u32DataSize, U8 *u8pData);
#endif
#endif

