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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

#include "boot.h"

#define REG_TABLE_END 0xFFFF

#define BW_REG(reg) ((reg>>16)&0xFF), ((reg>>8)&0xFF), (reg&0xFF)
#define REG(reg) ((reg>>16)&0xFF), ((reg>>8)&0xFF), (reg&0xFF)

#define REG_BW_ADDR_SIZE    3
#define REG_BW_MASK_SIZE    1
#define REG_BW_BANK_SIZE    1

#define U8 MS_U8
#define code

#define RIU_MACRO_START     do {
#define RIU_MACRO_END       } while (0)


///// Table
#include "Bandwidth_RegTable.h"
#include "Bandwidth_RegTable.c"

typedef struct
{
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabIdx;
} TAB_Info;

#define RIU_BASE 0x1F000000

#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))

#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE(RIU_BASE + (addr), val) }
#define RIU_READ_BYTE(addr)         ( READ_BYTE( RIU_BASE + (addr) ) )

#define MDrv_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END

static void _MDrv_BW_DumpTable(TAB_Info* ptab_Info)
{
    MS_VIRT virtAddr;
    MS_U8 u8Bank;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8BankGOP_Bak;
	char *u8Addr;
	unsigned int ha32Addr;

    if (ptab_Info->pIPTable == NULL){
        return;
    }

    if (ptab_Info->u8TabIdx >= ptab_Info->u8TabNums){
        return;
    }

    while (1)
    {
        virtAddr = (ptab_Info->pIPTable[0]<<16) + (ptab_Info->pIPTable[1]<<8) + ptab_Info->pIPTable[2];
        u8Bank    = ptab_Info->pIPTable[3];
        u8Mask    = ptab_Info->pIPTable[4]; 
        u8Value = ptab_Info->pIPTable[REG_BW_ADDR_SIZE+REG_BW_BANK_SIZE+REG_BW_MASK_SIZE+ptab_Info->u8TabIdx];

        if ((virtAddr & 0xFFFF) == REG_TABLE_END) // check end of table
            break;

        MDrv_WriteByteMask(virtAddr , u8Value, u8Mask);

        ptab_Info->pIPTable+=(REG_BW_ADDR_SIZE+REG_BW_BANK_SIZE+REG_BW_MASK_SIZE+ptab_Info->u8TabNums); // next
    }
}


void MDrv_BW_LoadInitTable(void *pInstance)
{
    TAB_Info tab_Info;
    tab_Info.pIPTable = (void*)BWTABLE_COM;
    tab_Info.u8TabNums = 1;
    tab_Info.u8TabIdx = 0;

	_MDrv_BW_DumpTable(&tab_Info);
}



