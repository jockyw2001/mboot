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

#ifndef __DRV_URSA_10_C__
#define __DRV_URSA_10_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_10.h"
#include <MsTypes.h>
#include <command.h>
#include <apiSWI2C.h>
#include <MsDebug.h>
//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_CMD_DATA_SUM              (7)
#define URSA_I2C_BUS_NUM               (1)
#define URSA_DDC2BI_MODE_ADDR            (0x40)
#define URSA_I2C_SUB_ADDR              (0)


static SWI2C_BusCfg Ursa_I2cBusCfg[URSA_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 400}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    //{PAD_TGPIO2, PAD_TGPIO3, 400},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------


static MS_BOOL MDrv_Ursa_SWI2C_WriteBytes(MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}

static MS_BOOL MDrv_Ursa_SWI2C_ReadBytes( MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}

//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------


void MDrv_Ursa_10_Lvds_init(void)
{
    MS_U8 u10_cmd_init[6][7] =
    {
        {0x80,0x10,0x43,0x00,0xD8,0x28,0x15},
        {0x40,0x11,0x08,0x98,0x04,0x6F,0xA0},
        {0x40,0x12,0x07,0x80,0x04,0x38,0x2F},
        {0x40,0x13,0x50,0x05,0x3C,0x00,0x00},
        {0x01,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x5D,0x01,0x01,0x01,0x00,0x02,0x00}
    };
    MS_U8 cmd_index;


    UBOOT_TRACE("IN\n");
    MApi_SWI2C_Init(Ursa_I2cBusCfg, URSA_I2C_BUS_NUM);
    for(cmd_index = 0; cmd_index < 6; cmd_index++)
    {
        UBOOT_DEBUG("[U10] send LVDS init cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_SWI2C_WriteBytes(u10_cmd_init[cmd_index]))
        {
            UBOOT_ERROR("[U10]Send cmd: FAILED !!!\n");
        }
    }

    UBOOT_TRACE("OK\n");
}


#endif //__DRV_URSA_10_C__
