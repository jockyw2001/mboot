#ifndef _MS_MSD_GLOBAL_H_
#define _MS_MSD_GLOBAL_H_

#include "Globalvar.h"
#include "ms_drc.h"
#include "ms_otg.h"

//#define DMA

void Init_gUSB_Var(USB_VAR *gUSBStruct, vendorCmdStruct *pCmdStruct);
void otgInitMassBuffer(USB_VAR *gUSBStruct);
void Init_vendorCmd_Var(vendorCmdStruct *pCmdStruct);
#endif
