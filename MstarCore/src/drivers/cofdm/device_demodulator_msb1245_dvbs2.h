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
#ifndef _MSB124X_DVBS2_H_
#define _MSB124X_DVBS2_H_
#define ENABLE_DEMOD1310_GPIO            0
#define DEMOD124X_PIN11_set_HIGH   MSB124X_GPIO_SetLevel(11,TRUE)//18
#define DEMOD124X_PIN11_set_LOW    MSB124X_GPIO_SetLevel(11,FALSE)//13
#define DEMOD124X_PIN13_set_HIGH   MSB124X_GPIO_SetLevel(13,TRUE)//power off
#define DEMOD124X_PIN13_set_LOW    MSB124X_GPIO_SetLevel(13,FALSE)//on
MS_BOOL  MSB124X_GPIO_Disable(MS_U8 u8Pin);
MS_BOOL MSB124X_GPIO_SetLevel(MS_U8 u8Pin, MS_BOOL bLevel);
MS_BOOL MSB124X_IIC_Bypass_Mode(MS_BOOL enable);
MS_BOOL  MSB124X_I2C_Channel_Set(MS_U8 ch_num);
MS_BOOL  MSB124X_I2C_Channel_Change(MS_U8 ch_num);
MS_BOOL  MSB124X_ReadReg(MS_U16 u16Addr, MS_U8 *pu8Data);
MS_BOOL MSB124X_WriteReg(MS_U16 u16Addr, MS_U8 u8Data);
MS_BOOL MSB124X_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data);
MS_BOOL MSB124X_WriteRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size);

//MS_BOOL  MSB124X_LoadDSPCode(const MS_U8  *pu8Buf, MS_U16 u16Len);
MS_BOOL  MSB124X_SetFrequency(MS_U16 u16CenterFreq, MS_U32 u32SymbolRate_Hz);
MS_U16	MSB124X_DTV_GetSignalStrength(void);
MS_U32	MSB124X_DTV_GetSignalNoiseRatio(void);
MS_U16	MSB124X_DTV_GetSignalQuality(MS_BOOL bDemodType);
MS_BOOL	MSB124X_DTV_GetLock(void);
MS_BOOL MSB124X_DiSEqC_Init(void);
MS_BOOL  ReadMailbox(MS_U16 u16Addr, MS_U8* u8Data);
MS_BOOL _Load2Sram(MS_U8 *pu8Buf, MS_U16 u16Len);
MS_BOOL MDrv_DMD_MSB1245_LoadDSPCodeToSram(void);
/*
MS_BOOL MDrv_Demod_GetRollOff(MS_U8 *pRollOff);
MS_BOOL MDrv_Demod_GetRFOffset(MS_S16 *ps16RFOff);
MS_BOOL MDrv_Demod_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL MDrv_Demod_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd);
MS_BOOL MDrv_Demod_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum);
MS_BOOL MDrv_Demod_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,DEMOD_MS_FE_CARRIER_PARAM *pTable);
MS_BOOL MDrv_Demod_BlindScan_Cancel(void);
MS_BOOL MDrv_Demod_BlindScan_End(void);
MS_BOOL MDrv_Demod_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq);
MS_BOOL MDrv_DiSEqC_SetTone(MS_BOOL bTone1);
MS_BOOL MDrv_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);
MS_BOOL MDrv_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
*/
#endif

