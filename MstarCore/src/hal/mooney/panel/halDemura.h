#ifndef _HAL_DEMURA_H_
#define _HAL_DEMURA_H_

#define DEMURA_MAX_LAYER             8

#define DEMURA_MAIN_BANK_BASE        0x102F00
#define REG_DEMURA_SUB_BANK_NUM      0x77
#define REG_DEMURA_DMA_BANK_NUM      0x67
#define DEMURA_DMA_ADDR_UNIT         32
#define DEMURA_DMA_REQ_LENGTH        4


#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

/* DEMURA */
#define L_BK_DEMURA(x)               BK_REG_L(DEMURA_MAIN_BANK_BASE, x)
#define H_BK_DEMURA(x)               BK_REG_H(DEMURA_MAIN_BANK_BASE, x)
#define L_BK_DEMURA_DMA(x)           BK_REG_L(DEMURA_MAIN_BANK_BASE, x)
#define H_BK_DEMURA_DMA(x)           BK_REG_H(DEMURA_MAIN_BANK_BASE, x)


MS_U16 Mhal_DeMura_Get_PanelWidth(DeMuraBinHeader *pBinHeader);
MS_U16 Mhal_DeMura_Get_PanelHeight(DeMuraBinHeader *pBinHeader);
void MHal_PNL_Switch_DeMura_SubBank(MS_U16 u16Bank);
MS_U16 MHal_PNL_Read_DeMura_SubBank(void);


void MHal_DeMura_SetDL_BaseAddr(MS_U32 u32Addr);
void MHal_DeMura_SetDL_SramIni_Addr(MS_U16 offset);
void MHal_DeMura_SetDL_Depth(MS_U16 HNodeCount);
void MHal_DeMura_SetDL_ReqLenth(MS_U16 ReqLenth);
void MHal_DeMura_SetDL_TriggerMode(AUTODOWNLOAD_TRIG_e eTrigMode);
void MHal_DeMura_Enable_DL(MS_BOOL bEnable);
void MHal_DeMura_EnableDemura(MS_BOOL bEnable);

#endif