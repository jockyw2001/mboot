
#ifndef __MHAL_MIU_H__
#define __MHAL_MIU_H__

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <configs/uboot_board_config.h>
#include <ShareType.h>
#include <MsTypes.h>

#define	MIUREG_BASE		            (0xBF200000)
#define	HAL_MIU_GP_NUM		        (4)         // MIU Group number
#define HAL_MIU_GP_PRIORITY_NUM     (4)         // MIU Group Priority number
#define	REG_MIU_GP_PRIORITY		    (0x6E << 1) //MIU group priority
#define PM_TOP_BASE                 0xBF000000
#define CHIP_ID                     0x001E00
#define CHIP_VER   					0x001E03


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Register Map
///
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MIU0_REG_BASE				    (0x1200)
#define MIU1_REG_BASE				    (0x0600)
#define MIU2_REG_BASE				    (0x62000)

#define MIU0_ARBB_REG_BASE			    (0x52000)
#define MIU1_ARBB_REG_BASE			    (0x52100)
#define MIU2_ARBB_REG_BASE			    (0x52200)

//MIU select group 0
#define		REG_MIUSEL_GP0				(0x78 << 1)
//MIU select group 1
#define		REG_MIUSEL_GP1				(0x79 << 1)
//MIU select group 2
#define		REG_MIUSEL_GP2				(0x7A << 1)
//MIU select group 3
#define		REG_MIUSEL_GP3				(0x7B << 1)

//MIU select group 4
#define		REG_MIUSEL_GP4				(0x7C << 1)
//MIU select group 5
#define		REG_MIUSEL_GP5				(0x7D << 1)
//MIU select group 6, Bank 0x1520
#define		REG_MIUSEL_GP6				(0x78 << 1)
//MIU select group 7, Bank 0x1520
#define		REG_MIUSEL_GP7				(0x79 << 1)


////
U8 HAL_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);

MS_U8* HAL_MIU_GetKernelProtectClients(void);

MS_U64 HAL_MIU_PA2BA(MS_U64 u64PhyAddr);

MS_U64 HAL_MIU_BA2PA(MS_U64 u64BusAddr);

#endif //__MHAL_MIU_H__
