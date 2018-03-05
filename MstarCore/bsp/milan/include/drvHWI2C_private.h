#ifndef _DRVBDHWI2C_PRIV_H_
#define _DRVBDHWI2C_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvHWI2C.h
/// @author MStar Semiconductor Inc.
/// @brief HWI2C control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvHWI2C.h" 


typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_Init)(HWI2C_UnitCfg *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_Start)(void);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_Stop)(void);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_GetPortIndex)(HWI2C_PORT , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SelectPort)(HWI2C_PORT );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetClk)(HWI2C_CLKSEL );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetReadMode)(HWI2C_ReadMode  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteByte)(MS_U16 ,MS_U8 ,MS_U8  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteBytes)(MS_U16 ,MS_U32 ,MS_U8 *,MS_U32 ,MS_U8 * );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadByte)(MS_U16 ,MS_U8 ,MS_U8 * );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadBytes)(MS_U16 ,MS_U32 ,MS_U8 *,MS_U32 ,MS_U8 * );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SelectPort1)(HWI2C_PORT  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetClkP1)(HWI2C_CLKSEL );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetReadModeP1)(HWI2C_ReadMode  );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteByteP1)(MS_U16 , MS_U8 , MS_U8 );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_WriteBytesP1)(MS_U16 , MS_U32 , MS_U8 *, MS_U32 , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadByteP1)(MS_U16 , MS_U8 , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_ReadBytesP1)(MS_U16 , MS_U32 , MS_U8 *, MS_U32 , MS_U8 *);
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_SetDbgLevel)(HWI2C_DbgLv );
typedef MS_BOOL  (*IOCTL_MDrv_HWI2C_GetLibVer)(const MSIF_Version **);
typedef void     (*IOCTL_MDrv_HWI2C_GetStatus)(HWI2C_Status *);
typedef const HWI2C_Info*   (*IOCTL_MDrv_HWI2C_GetInfo)(void );
//typedef MS_U16   (*IOCTL_MDrv_HWI2C_SetPowerState)(EN_POWER_MODE );


typedef struct _HWI2C_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}HWI2C_RESOURCE_PRIVATE;


typedef struct _HWI2C_INSTANT_PRIVATE
{
    IOCTL_MDrv_HWI2C_Init        	 	fpHWI2CInit;
    IOCTL_MDrv_HWI2C_Start           	fpHWI2CStart;
    IOCTL_MDrv_HWI2C_Stop            	fpHWI2CStop;
    IOCTL_MDrv_HWI2C_GetPortIndex     	fpHWI2CGetPortIndex;
    IOCTL_MDrv_HWI2C_SelectPort   		fpHWI2CSelectPort;

    IOCTL_MDrv_HWI2C_SetClk        		fpHWI2CSetClk;
    IOCTL_MDrv_HWI2C_SetReadMode        fpHWI2CSetReadMode;
    IOCTL_MDrv_HWI2C_WriteByte          fpHWI2CWriteByte;
    IOCTL_MDrv_HWI2C_WriteBytes         fpHWI2CWriteBytes ;
    IOCTL_MDrv_HWI2C_ReadByte          	fpHWI2CReadByte;
    IOCTL_MDrv_HWI2C_ReadBytes   		fpHWI2CReadBytes;
    IOCTL_MDrv_HWI2C_SelectPort1        fpHWI2CSelectPort1;
	IOCTL_MDrv_HWI2C_SetClkP1        	fpHWI2CSetClkP1;
	IOCTL_MDrv_HWI2C_SetReadModeP1      fpHWI2CSetReadModeP1;
	IOCTL_MDrv_HWI2C_WriteByteP1        fpHWI2CWriteByteP1;
	IOCTL_MDrv_HWI2C_WriteBytesP1       fpHWI2CWriteBytesP1;
	IOCTL_MDrv_HWI2C_ReadByteP1        	fpHWI2CReadByteP1 ;
	IOCTL_MDrv_HWI2C_ReadBytesP1        fpHWI2CReadBytesP1;
	IOCTL_MDrv_HWI2C_SetDbgLevel        fpHWI2CSetDbgLevel;
	IOCTL_MDrv_HWI2C_GetLibVer        	fpHWI2CGetLibVer ;
	IOCTL_MDrv_HWI2C_GetStatus        	fpHWI2CGetStatus ;
	IOCTL_MDrv_HWI2C_GetInfo        	fpHWI2CGetInfo;
	//IOCTL_MDrv_HWI2C_SetPowerState      fpHWI2CSetPowerState;
	
}HWI2C_INSTANT_PRIVATE;


void HWI2CRegisterToUtopia(void);
MS_U32 HWI2COpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
//MS_U32 HWI2COpen(void** pInstantTmp, void* pAttribute);
MS_U32 HWI2CClose(void* pInstantTmp);
MS_U32 HWI2CIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);


#ifdef __cplusplus
}
#endif
#endif // _DRVHWI2C_PRIV_H_


