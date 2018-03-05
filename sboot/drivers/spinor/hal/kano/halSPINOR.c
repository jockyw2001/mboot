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
#include "halSPINOR.h"
#include "drvSPINORCMD.h"
//#include "cache.h"
#endif

#define READ_WORD(_reg)											(*(volatile U16*)(_reg))
#define WRITE_WORD(_reg, _val)      				{(*((volatile U16*)(_reg))) = (U16)(_val); }
#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile U16*)(_reg))) = ((*((volatile U16*)(_reg))) & ~(_mask)) | ((U16)(_val) & (_mask)); }
#define CLK_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(CONFIG_RIU_BASE_ADDRESS + (QSPI_BASE_ADDR << 1) + ((addr)<<2), (val), (mask))
#define PM_WRITE_MASK(addr, val, mask)      WRITE_WORD_MASK(CONFIG_RIU_BASE_ADDRESS + (PMCLK_BASE_ADDR << 1) + ((addr)<<2), (val), (mask))
#define FSP_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK((CONFIG_RIU_BASE_ADDRESS+(FSP_BASE_ADDR << 1)) + ((addr)<<2), (val), (mask))
#define QSPI_WRITE(addr, val)               WRITE_WORD((CONFIG_RIU_BASE_ADDRESS+(QSPI_BASE_ADDR << 1)) + ((addr)<<2), (val))
#define FSP_READ(addr)                      READ_WORD((CONFIG_RIU_BASE_ADDRESS+(FSP_BASE_ADDR << 1)) + ((addr)<<2))
#define FSP_WRITE(addr, val)                WRITE_WORD((CONFIG_RIU_BASE_ADDRESS+(FSP_BASE_ADDR << 1)) + ((addr)<<2), (val))


typedef struct
{
    U8  u8Clk;
    U16 eClkCkg;
} hal_clk_ckg_t;

//
//  Spi  Clk Table (List)
//
static hal_clk_ckg_t _hal_ckg_spi_pm[] = {
     {12 , PM_SPI_CLK_XTALI }
    ,{27 , PM_SPI_CLK_27MHZ }
    ,{36 , PM_SPI_CLK_36MHZ }
    ,{43 , PM_SPI_CLK_43MHZ }
    ,{54 , PM_SPI_CLK_54MHZ }
    ,{72 , PM_SPI_CLK_72MHZ }
    ,{86 , PM_SPI_CLK_86MHZ }
    ,{108, PM_SPI_CLK_108MHZ}
};

#define udelay(TimerCount, WaitComplete)    hw_timer_delay(TimerCount)

BOOL (*_HAL_FSP_PageWrite_Callback_Func)
(
	U32 u32Addr_tmp,
	U32 u32Size_tmp,
	U8 *pu8Data_tmp,
	U32 u32PageIdx_tmp,
	U32 u32Remain_tmp
);

void (*_HAL_FSP_Write_Callback_Func)(U32 u32Addr, U32 u32Size, U8 *pu8Data);


static U32 hw_timer_delay(U32 u32usTick)
{
    U32 u32HWTimer = 0;
	volatile U16 u16TimerLow = 0;
	volatile U16 u16TimerHigh = 0;

	// reset HW timer
	REG_WRITE_UINT16(TIMER0_MAX_LOW, 0xFFFF);
	REG_WRITE_UINT16(TIMER0_MAX_HIGH, 0xFFFF);
	REG_WRITE_UINT16(TIMER0_ENABLE, 0);

	// start HW timer
	REG_SET_BITS_UINT16(TIMER0_ENABLE, 0x0001);

	while( u32HWTimer < 12*u32usTick ) // wait for u32usTick micro seconds
	{
		REG_READ_UINT16(TIMER0_CAP_LOW, u16TimerLow);
		REG_READ_UINT16(TIMER0_CAP_HIGH, u16TimerHigh);

		u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
	}

	REG_WRITE_UINT16(TIMER0_ENABLE, 0);

    return u32usTick+1;
}

static BOOL _HAL_WriteByte(U32 u32RegAddr, U8 u8Val)
{
    ((unsigned char  volatile *) CONFIG_RIU_BASE_ADDRESS + (FSP_BASE_ADDR << 1))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}
static BOOL _HAL_Write2Byte(U32 u32RegAddr, U16 u16Val)
{
    RIU[FSP_BASE_ADDR + u32RegAddr] = u16Val;
    return TRUE;
}

static U16 _HAL_Read2Byte(U32 u32RegAddr)
{
    return RIU[FSP_BASE_ADDR + (u32RegAddr)];
}

static U8 _HAL_ReadByte(U32 u32RegAddr)
{
    return((unsigned char  volatile *) CONFIG_RIU_BASE_ADDRESS + (FSP_BASE_ADDR << 1))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

static BOOL _HAL_BDMA_Write2Byte(U32 u32RegAddr, U16 u16Val)
{

     *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1) + (u32RegAddr << 2)) = u16Val;
     return TRUE;
}

static U16 _HAL_QSPI_Write2Byte(U32 u32RegAddr, U16 u16Val)
{
    RIU[QSPI_BASE_ADDR + (u32RegAddr << 1)] = u16Val;
    return TRUE;
}

static U16 _HAL_CHIP_Read2Byte(U32 u32RegAddr)
{
    return RIU[CHIP_BASE_ADDR + (u32RegAddr << 1)];
}

static U16 _HAL_CHIP_Write2Byte(U32 u32RegAddr, U16 u16Val)
{
    RIU[CHIP_BASE_ADDR + (u32RegAddr << 1)] = u16Val;
    return TRUE;
}

static BOOL _HAL_FSP_ChkWaitDone(void)
{
//consider as it spend very long time to check if FSP done, so it may implment timeout method to improve
    U16 u16Try = 0;
    U8  bRet =FALSE;
    while (u16Try < CHK_NUM_WAITDONE)
    {
    	if ( (FSP_READ(REG_FSP_DONE_FLAG) & REG_FSP_DONE_FLAG_MASK) == REG_FSP_DONE )
    	{
            bRet = TRUE;
            break;
      }
        u16Try++;
    }
    FSP_WRITE_MASK(REG_FSP_DONE_CLR,REG_FSP_CLR,REG_FSP_DONE_CLR_MASK);
    return bRet;
}


static BOOL _HAL_SPINAND_BDMA_READ(U32 u32Addr, U32 u32DataSize, U8 *u8pData)
{
    U32 u32Addr1;
    BOOL bRet = FALSE;

    //Set source and destination path
    _HAL_BDMA_Write2Byte(0x00, 0x0000);
    _HAL_BDMA_Write2Byte(0x02, 0x4035);

    // Set source address
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)+(4<<2)) = (u32Addr & 0xFFFF);
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)+(5<<2)) = (u32Addr >> 16);
    #if defined(CONFIG_MCU_MIPS32)
    flush_cache((U32)u8pData, u32DataSize);
    u32Addr1 = ((U32)u8pData & 0x1fffffff);
    #else
    u32Addr1 = (U32)u8pData;
    #endif
    // Set destination  address
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)+(6<<2)) = (U16)(u32Addr1 & 0x0000FFFF);
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)+(7<<2)) = (U16)(u32Addr1 >> 16);
    // Set Size
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)+(8<<2)) = (U16)(u32DataSize & 0xFFFF);
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)+(9<<2)) = (U16)(u32DataSize >> 16);

    // Trigger
    *(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS+(BDMA_BASE_ADDR<<1)) = 1;

    do
    {
        //check done
        if(*(unsigned short  volatile *)(CONFIG_RIU_BASE_ADDRESS + (0x100902 << 1)) & 0x08)
        {
            //clear done
            *(unsigned char volatile *)(CONFIG_RIU_BASE_ADDRESS + (0x100902 << 1)) = 0x08;
            bRet = TRUE;
            break;
        }
    } while (1);

    return bRet;

}

static void _HAL_FSP_WriteBuf0(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB0,REG_FSP_WDB0_DATA(u8Data),REG_FSP_WDB0_MASK);
}

static void _HAL_FSP_WriteBuf1(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB1,REG_FSP_WDB1_DATA(u8Data),REG_FSP_WDB1_MASK);
}

static void _HAL_FSP_WriteBuf2(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB2,REG_FSP_WDB2_DATA(u8Data),REG_FSP_WDB2_MASK);
}

static void _HAL_FSP_WriteBuf3(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB3,REG_FSP_WDB3_DATA(u8Data),REG_FSP_WDB3_MASK);
}

static void _HAL_FSP_WriteBuf4(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB4,REG_FSP_WDB4_DATA(u8Data),REG_FSP_WDB4_MASK);
}

static void _HAL_FSP_WriteBuf5(U8 u8Data){
    FSP_WRITE_MASK(REG_FSP_WDB5,REG_FSP_WDB5_DATA(u8Data),REG_FSP_WDB5_MASK);
}

static void _HAL_FSP_WriteBuf6(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB6,REG_FSP_WDB6_DATA(u8Data),REG_FSP_WDB6_MASK);
}

static void _HAL_FSP_WriteBuf7(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB7,REG_FSP_WDB7_DATA(u8Data),REG_FSP_WDB7_MASK);
}

static void _HAL_FSP_WriteBuf8(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB8,REG_FSP_WDB8_DATA(u8Data),REG_FSP_WDB8_MASK);
}

static void _HAL_FSP_WriteBuf9(U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB9,REG_FSP_WDB9_DATA(u8Data),REG_FSP_WDB9_MASK);
}

void HAL_FSP_WriteBufs(U8 u8Idx, U8 u8Data)
{
    switch ( u8Idx )
    {
    case 0:
    	 _HAL_FSP_WriteBuf0(u8Data);
        break;
    case 1:
        _HAL_FSP_WriteBuf1(u8Data);
        break;
    case 2:
        _HAL_FSP_WriteBuf2(u8Data);
        break;
    case 3:
        _HAL_FSP_WriteBuf3(u8Data);
        break;
    case 4:
        _HAL_FSP_WriteBuf4(u8Data);
        break;
    case 5:
        _HAL_FSP_WriteBuf5(u8Data);
        break;
    case 6:
        _HAL_FSP_WriteBuf6(u8Data);
        break;
    case 7:
        _HAL_FSP_WriteBuf7(u8Data);
        break;
    case 8:
        _HAL_FSP_WriteBuf8(u8Data);
        break;
    case 9:
        _HAL_FSP_WriteBuf9(u8Data);
        break;
    default:
        break;
    }
}

static U8 _HAL_FSP_ReadBuf0(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB0) & 0x00FF) >> 0);
}

static U8 _HAL_FSP_ReadBuf1(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB1) & 0xFF00) >> 8);
}

static U8 _HAL_FSP_ReadBuf2(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB2) & 0x00FF) >> 0);
}

static U8 _HAL_FSP_ReadBuf3(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB3) & 0xFF00) >> 8);
}

static U8 _HAL_FSP_ReadBuf4(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB4) & 0x00FF) >> 0);
}

static U8 _HAL_FSP_ReadBuf5(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB5) & 0xFF00) >> 8);
}

static U8 _HAL_FSP_ReadBuf6(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB6) & 0x00FF) >> 0);
}

static U8 _HAL_FSP_ReadBuf7(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB7) & 0xFF00) >> 8);
}

static U8 _HAL_FSP_ReadBuf8(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB8) & 0x00FF) >> 0);
}

static U8 _HAL_FSP_ReadBuf9(void)
{
    return (U8)((FSP_READ(REG_FSP_RDB9) & 0xFF00) >> 8);
}

U8 HAL_FSP_ReadBufs(U8 u8Idx)
{
    U8 u8Data;
    switch ( u8Idx )
    {
    case 0:    u8Data = _HAL_FSP_ReadBuf0();
        break;
    case 1:    u8Data = _HAL_FSP_ReadBuf1();
        break;
    case 2:    u8Data = _HAL_FSP_ReadBuf2();
        break;
    case 3:    u8Data = _HAL_FSP_ReadBuf3();
        break;
    case 4:    u8Data = _HAL_FSP_ReadBuf4();
        break;
    case 5:    u8Data = _HAL_FSP_ReadBuf5();
        break;
    case 6:    u8Data = _HAL_FSP_ReadBuf6();
        break;
    case 7:    u8Data = _HAL_FSP_ReadBuf7();
        break;
    case 8:    u8Data = _HAL_FSP_ReadBuf8();
        break;
    case 9:    u8Data = _HAL_FSP_ReadBuf9();
        break;
    default:    u8Data = 0xFF;
                return -1;
    }
    return u8Data;
}

static void HAL_SERFLASH_AddressToBlock(U32 u32FlashAddr, U32 *pu32BlockIndex)
{
    *pu32BlockIndex = u32FlashAddr / SERFLASH_BLOCK_SIZE;
}

static void HAL_SERFLASH_BlockToAddress(U32 u32BlockIndex, U32 *pu32FlashAddr)
{
	*pu32FlashAddr = u32BlockIndex * SERFLASH_BLOCK_SIZE;
}

static BOOL _HAL_FSP_BlockErase(U32 u32FlashAddr, EN_FLASH_ERASE eSize)
{
    BOOL bRet = TRUE;
    HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
    HAL_FSP_WriteBufs(1,eSize);
    HAL_FSP_WriteBufs(2,(U8)((u32FlashAddr>>0x10)&0xFF));
    HAL_FSP_WriteBufs(3,(U8)((u32FlashAddr>>0x08)&0xFF));
    HAL_FSP_WriteBufs(4,(U8)((u32FlashAddr>>0x00)&0xFF));
    HAL_FSP_WriteBufs(5,SPI_CMD_RDSR);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(4),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(1),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(1),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_ON,REG_FSP_2NDCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_ON,REG_FSP_3THCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD,REG_FSP_RDSR_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
    bRet &= _HAL_FSP_ChkWaitDone();
    return bRet;
}

static BOOL HAL_FSP_Write_Enable(void)
{
    BOOL bRet = TRUE;
    U16 u16Data = 0;
    //write enable
    HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(0),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(0),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(0),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    u16Data = FSP_READ(REG_FSP_CTRL);
    u16Data &= 0xFF;
    FSP_WRITE(REG_FSP_CTRL, u16Data);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
    bRet &= _HAL_FSP_ChkWaitDone();
        return bRet;
}

/*Speed up 5 times write efficiency compared with "HAL_FSP_Write_Engine */
static BOOL HAL_FSP_PageWrite_Engine
(
	U32 u32Addr_tmp,
	U32 u32Size_tmp,
	U8 *pu8Data_tmp,
	U32 u32PageIdx_tmp,
	U32 u32Remain_tmp
)
{
		BOOL bRet = TRUE;
		U8  u8Status = 0;
		U32 u32I, u32J, u32K;
		U32 u32quotient;
		U32 u32remainder;
		for(u32K = 0; u32K < u32PageIdx_tmp; u32K++)
		{
			bRet =HAL_FSP_Write_Enable();
			if(!bRet)
				{
					return FALSE;
				}
			HAL_FSP_WriteBufs(0,SPI_CMD_PP);
			HAL_FSP_WriteBufs(1,(U8)((u32Addr_tmp>>0x10)&0xFF));
			HAL_FSP_WriteBufs(2,(U8)((u32Addr_tmp>>0x08)&0xFF));
			HAL_FSP_WriteBufs(3,(U8)((u32Addr_tmp>>0x00)&0xFF));
			FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(4),REG_FSP_WBF_SIZE0_MASK);
			FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
			bRet &= _HAL_FSP_ChkWaitDone();
			if(!bRet)
				{
					return FALSE;
				}
			QSPI_WRITE(REG_SPI_BURST_WRITE,REG_SPI_ENABLE_BURST);
			u32quotient = (u32Size_tmp / REG_FSP_MAX_WRITEDATA_SIZE);
			u32remainder = (u32Size_tmp % REG_FSP_MAX_WRITEDATA_SIZE);
			if(u32remainder)
			{
				u32quotient++;
				u32Size_tmp = u32remainder;
			}
			else
				u32Size_tmp = REG_FSP_MAX_WRITEDATA_SIZE;
			for( u32I = 0; u32I < u32quotient; u32I++ )
			{
				for( u32J = 0; u32J < u32Size_tmp; u32J++ )
				{
					HAL_FSP_WriteBufs(u32J,*(pu8Data_tmp+u32J));
				}
				pu8Data_tmp += u32J;
				u32Size_tmp = REG_FSP_MAX_WRITEDATA_SIZE;
				FSP_WRITE_MASK(REG_FSP_WBF_SIZE, REG_FSP_WBF_SIZE0(u32J), REG_FSP_WBF_SIZE0_MASK);
				FSP_WRITE_MASK(REG_FSP_TRIGGER, REG_FSP_FIRE, REG_FSP_TRIGGER_MASK);
				bRet &= _HAL_FSP_ChkWaitDone();
			}
			/*CS Pull high first after data transfer done*/
			QSPI_WRITE(REG_SPI_BURST_WRITE,REG_SPI_DISABLE_BURST);
			QSPI_WRITE(REG_SPI_BURST_WRITE,REG_SPI_TO_HARDWARE);
			do
			{
				HAL_FSP_WriteBufs(0,SPI_CMD_RDSR);
				FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
				FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(1),REG_FSP_RBF_SIZE0_MASK);
				FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
				bRet &= _HAL_FSP_ChkWaitDone();
				u8Status = HAL_FSP_ReadBufs(0);
			}while(u8Status & FLASH_OIP);
			
			if(u32K==u32PageIdx_tmp-1)
			{
				u32Size_tmp=u32Remain_tmp;
				u32Addr_tmp+=FLASH_PAGE_SIZE;
			}
			else
			{
				u32Size_tmp = FLASH_PAGE_SIZE;
				u32Addr_tmp+=FLASH_PAGE_SIZE;
			}
	}
	return bRet;
}

void HAL_FSP_Write_Engine(U32 u32Write_Addr, U8 u8Size, U8 *pu8Data)
{
		BOOL bRet = TRUE;
		U32 u32I;
		HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
		HAL_FSP_WriteBufs(1,SPI_CMD_PP);
		HAL_FSP_WriteBufs(2,(U8)((u32Write_Addr>>0x10)&0xFF));
		HAL_FSP_WriteBufs(3,(U8)((u32Write_Addr>>0x08)&0xFF));
		HAL_FSP_WriteBufs(4,(U8)((u32Write_Addr>>0x00)&0xFF));
		for( u32I = 0; u32I < u8Size; u32I++ )
		{
			HAL_FSP_WriteBufs((5+u32I),*(pu8Data+u32I));
		}
		HAL_FSP_WriteBufs((5 + u8Size),SPI_CMD_RDSR);
		FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
		FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1((4+u8Size)),REG_FSP_WBF_SIZE1_MASK);
		FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(1),REG_FSP_WBF_SIZE2_MASK);
		FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
		FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
		FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(1),REG_FSP_RBF_SIZE2_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_ON,REG_FSP_2NDCMD_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_ON,REG_FSP_3THCMD_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD,REG_FSP_RDSR_MASK);
		FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
		FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
		bRet &= _HAL_FSP_ChkWaitDone();
}

static BOOL HAL_FSP_Write(U32 u32Addr, U32 u32Size, U8 *pu8Data)
{
    U32 u32I;
    BOOL bRet = TRUE;
    U32 u32J;
    U32 u32quotient;
    U8  u8BufSize = REG_FSP_WRITEDATA_SIZE;
    U8  u8dat_align=0;
    U8  u8addr_align=0;
    U8  u8remainder=0;
    U8  u8writebytes=0;
    _HAL_FSP_Write_Callback_Func=HAL_FSP_Write_Engine;
    u8addr_align=((U8)(u32Addr))&0xF;
    u8dat_align=u8addr_align%4;
    if(u8dat_align)
    {
        u8writebytes = 4-u8dat_align;
        (_HAL_FSP_Write_Callback_Func)(u32Addr,u8writebytes,pu8Data);
	  		u32Addr += u8writebytes;
         pu8Data += u8writebytes;
    }
    u32quotient = ((u32Size-u8writebytes)  / u8BufSize);
    u8remainder = ((u32Size-u8writebytes)  % u8BufSize);
    for(u32J = 0; u32J < u32quotient; u32J++)
    {
        (_HAL_FSP_Write_Callback_Func)(u32Addr,u8BufSize,pu8Data);
				u32Addr += u8BufSize;
       pu8Data += u8BufSize;
    }

    if(u8remainder)
    {
        (_HAL_FSP_Write_Callback_Func)(u32Addr,u8remainder,pu8Data);
        u32Addr += u8remainder;
        pu8Data += u8remainder;
    }

    return bRet;
}

BOOL HAL_FSP_Read(U32 u32Addr, U32 u32Size, U8 *pu8Data)
{
    BOOL bRet = TRUE;
    U32 u32Idx, u32J;
    U32 u32quotient;
    U32 u32remainder;
    U8  u8BufSize = REG_FSP_READDATA_SIZE;
    u32quotient = (u32Size / u8BufSize);
    u32remainder = (u32Size % u8BufSize);
    if(u32remainder)
    {
        u32quotient++;
        u32Size = u32remainder;
    }
    else
        u32Size = u8BufSize;

    for(u32J = 0; u32J < u32quotient; u32J++)
    {
        HAL_FSP_WriteBufs(0, SPI_CMD_READ);
        HAL_FSP_WriteBufs(1,(U8)((u32Addr>>0x10)&0xFF));
        HAL_FSP_WriteBufs(2,(U8)((u32Addr>>0x08)&0xFF));
        HAL_FSP_WriteBufs(3,(U8)((u32Addr>>0x00)&0xFF));
	/*Lost first byte in using normal read command;Dummy Byte for Fast Read*/
	//HAL_FSP_WriteBufs(4, 0x00);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(4),REG_FSP_WBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(0),REG_FSP_WBF_SIZE1_MASK);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(0),REG_FSP_WBF_SIZE2_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(u32Size),REG_FSP_RBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(0),REG_FSP_RBF_SIZE2_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_OFF,REG_FSP_2NDCMD_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_OFF,REG_FSP_3THCMD_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_OFF,REG_FSP_FSCHK_MASK);
        FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
        bRet &= _HAL_FSP_ChkWaitDone();
        if(!bRet)
        	return FALSE;
        for( u32Idx = 0; u32Idx < u32Size; u32Idx++ )
            *(pu8Data + u32Idx) = HAL_FSP_ReadBufs(u32Idx);
        pu8Data += u32Idx;
        u32Addr += u32Idx;
        u32Size = u8BufSize;
    }
    return bRet;
}

BOOL HAL_SERFLASH_SectorErase(U32 u32SectorAddress)
{
		BOOL bRet = TRUE;
		bRet &= _HAL_FSP_BlockErase(u32SectorAddress,FLASH_ERASE_04K);
}

BOOL HAL_SERFLASH_BlockErase(U32 u32StartBlock, U32 u32EndBlock, BOOL bWait)
{
    BOOL bRet = TRUE;
    U32	u32Idx;
    U32	u32FlashAddr = 0;
    for( u32Idx = u32StartBlock; u32Idx <= u32EndBlock; u32Idx++ )
    {
        HAL_SERFLASH_BlockToAddress(u32Idx, &u32FlashAddr);
        bRet &= _HAL_FSP_BlockErase(u32FlashAddr,FLASH_ERASE_64K);
    }
    return bRet;
}


BOOL HAL_SERFLASH_Write(U32 u32Addr, U32 u32Size, U8 *pu8Data)
{
#if 1
		BOOL bRet = TRUE;
		U8  u8Status = 0;
		U32 u32I, u32J, u32K;
		U32 u32quotient;
		U32 u32remainder;
		U32 u32PageIdx;
		U32 u32PageRemainder;
		U16  u16dat_align=0;
		U16  u16addr_align=0;
		U16  u16remainder=0;
		U16  u16writebytes=0;
		_HAL_FSP_PageWrite_Callback_Func=HAL_FSP_PageWrite_Engine;
		/*Fix PageWrite Address Align issue*/
		udelay(40000,1);
		u16addr_align=u32Addr % FLASH_PAGE_SIZE;
		if (u16addr_align)
		{
			u16writebytes = FLASH_PAGE_SIZE - u16addr_align;
			if (u32Size < u16writebytes)
			{
				u16writebytes = u32Size;
			}
			(_HAL_FSP_PageWrite_Callback_Func)(u32Addr,u16writebytes,pu8Data,1,0);
			u32Addr +=  u16writebytes ;
			pu8Data   +=  u16writebytes ;
			u32Size -=  u16writebytes ;
		}
		if(u32Size)
		{
			u32PageIdx =  u32Size/FLASH_PAGE_SIZE;
			u32PageRemainder =u32Size  % FLASH_PAGE_SIZE;
			if(u32PageIdx)
			{
				if(u32PageRemainder)
				{
					u32PageIdx+=1;
				}
				u32Size = FLASH_PAGE_SIZE;
			}
		}
		else
		{
			u32PageIdx=1;
			u32Size = u32PageRemainder;
		}
		(_HAL_FSP_PageWrite_Callback_Func)(u32Addr,u32Size,pu8Data,u32PageIdx,u32PageRemainder);
		return bRet;

#else
		return HAL_FSP_Write(u32Addr, u32Size, pu8Data);

#endif
}


BOOL HAL_SERFLASH_Read(U32 u32Addr, U32 u32Size, U8 *pu8Data)
{
	return _HAL_SPINAND_BDMA_READ(u32Addr, u32Size, pu8Data);
	//return HAL_FSP_Read(u32Addr, u32Size, pu8Data);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SPINAND_SetCKG()
/// @brief \b Function \b Description: This function is used to set ckg_spi dynamically
/// @param <IN>        \b eCkgSpi    : enumerate the ckg_spi
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>    \b NONE    :
/// @param <NOTE>    \b : Please use this function carefully , and is restricted to Flash ability
////////////////////////////////////////////////////////////////////////////////
BOOL HAL_SPINOR_SetCKG(U8 u8CkgSpi)
{
    BOOL Ret = FALSE;
    U8 u8nonPmIdx = 0, u8PmIdx = 0;
    U8 u8Idx;
    U8 u8Size;

    u8Size = sizeof(_hal_ckg_spi_pm)/ sizeof(hal_clk_ckg_t);
    for(u8Idx = 0; u8Idx < u8Size; u8Idx++)
    {
        if(u8CkgSpi < _hal_ckg_spi_pm[u8Idx].u8Clk)
        {
            if(u8Idx)
                u8PmIdx = u8Idx - 1;
            else
                u8PmIdx = u8Idx;
            break;
        }
        else
            u8PmIdx = u8Idx;
    }

    if( _hal_ckg_spi_pm[u8PmIdx].eClkCkg == FALSE)
    {
         return Ret;
    }
    // NON-PM Doman
    CLK_WRITE_MASK(REG_CLK0_CKG_SPI,CLK0_CLK_SWITCH_OFF,CLK0_CLK_SWITCH_MASK);
    CLK_WRITE_MASK(REG_CLK0_CKG_SPI,CLK0_CKG_SPI_108MHZ,CLK0_CKG_SPI_MASK); // set ckg_spi
   // CLK_WRITE_MASK(REG_CLK0_CKG_SPI,CLK0_CKG_SPI_54MHZ,CLK0_CKG_SPI_MASK);
    CLK_WRITE_MASK(REG_CLK0_CKG_SPI,CLK0_CLK_SWITCH_ON,CLK0_CLK_SWITCH_MASK);       // run @ ckg_spi

    // PM Doman
    PM_WRITE_MASK(REG_PM_CKG_SPI,PM_SPI_CLK_SWITCH_OFF,PM_SPI_CLK_SWITCH_MASK); // run @ 12M
    PM_WRITE_MASK(REG_PM_CKG_SPI,_hal_ckg_spi_pm[u8PmIdx].eClkCkg,PM_SPI_CLK_SEL_MASK); // set ckg_spi
    //PM_WRITE_MASK(REG_PM_CKG_SPI,PM_SPI_CLK_54MHZ,PM_SPI_CLK_SEL_MASK);
    PM_WRITE_MASK(REG_PM_CKG_SPI,PM_SPI_CLK_SWITCH_ON,PM_SPI_CLK_SWITCH_MASK);  // run @ ckg_spi
    Ret = TRUE;
    return Ret;
}

BOOL HAL_FSP_WriteProtect(BOOL bEnable)
{
    BOOL bRet = TRUE;
    HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
    HAL_FSP_WriteBufs(1,SPI_CMD_WRSR);
    if (bEnable)
    {
        // SF_SR_SRWD: SRWD Status Register Write Protect
        HAL_FSP_WriteBufs(2,0xFF);
    }
    else
    {
        // [4:2] or [5:2] protect blocks
        HAL_FSP_WriteBufs(2,0x0);
    }
    HAL_FSP_WriteBufs(3,SPI_CMD_RDSR);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(2),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(1),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(1),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_ON,REG_FSP_2NDCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_ON,REG_FSP_3THCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD,REG_FSP_RDSR_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
    bRet &= _HAL_FSP_ChkWaitDone();
    return bRet;
}

