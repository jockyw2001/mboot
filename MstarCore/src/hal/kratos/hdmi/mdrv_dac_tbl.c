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

//****************************************************
// DAC Drive Chip           : Kratos_DAC_Driver
// DAC Excel CodeGen Version: 1.07
// DAC Excel SW      Version: 1.02
// DAC Excel update date    : 2014/7/9 11:44
//****************************************************

#ifndef _DRVDAC_TBL_C_
#define _DRVDAC_TBL_C_

#include "MsCommon.h"
#include "mdrv_dac_tbl.h"

//****************************************************
// INIT  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480I_60_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  480I_60
//****************************************************
MS_U8 MST_DAC_480I_60_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_480P_60_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  480P_60
//****************************************************
MS_U8 MST_DAC_480P_60_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576I_50_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  576I_50
//****************************************************
MS_U8 MST_DAC_576I_50_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_576P_50_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  576P_50
//****************************************************
MS_U8 MST_DAC_576P_50_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_50_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  720P_50
//****************************************************
MS_U8 MST_DAC_720P_50_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_720P_60_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  720P_60
//****************************************************
MS_U8 MST_DAC_720P_60_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_50_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080I_50
//****************************************************
MS_U8 MST_DAC_1080I_50_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080I_60_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080I_60
//****************************************************
MS_U8 MST_DAC_1080I_60_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_50_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080P_50
//****************************************************
MS_U8 MST_DAC_1080P_50_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_60_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080P_60
//****************************************************
MS_U8 MST_DAC_1080P_60_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_30_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080P_30
//****************************************************
MS_U8 MST_DAC_1080P_30_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_25_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080P_25
//****************************************************
MS_U8 MST_DAC_1080P_25_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_1080P_24_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  1080P_24
//****************************************************
MS_U8 MST_DAC_1080P_24_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

//****************************************************
// INIT  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_GPIO  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_GPIO_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_GPIO_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_SC  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_SC_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_SC_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_MOD  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_MOD_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_MOD_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDGEN  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDGEN_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDGEN_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00 }
};

//****************************************************
// INIT_HDMITX_8BIT  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDMITX_8BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDMITX_8BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_8BIT_Divider  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDMITX_8BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDMITX_8BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDMITX_10BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDMITX_10BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_10BIT_Divider  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDMITX_10BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDMITX_10BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDMITX_12BIT_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDMITX_12BIT_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// INIT_HDMITX_12BIT_Divider  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_INIT_HDMITX_12BIT_Divider_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+DAC_TAB_640x480_60_INIT_HDMITX_12BIT_Divider_NUMS]=
{                 // Reg           Mask  Value
 { DRV_DAC_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// Gamma  640x480_60
//****************************************************
MS_U8 MST_DAC_640x480_60_Gamma_TBL[][4] =
{               // Reg           Value  Value
 {  DRV_DAC_REG(REG_TABLE_END) , 0x00, 0x00  }
};

DAC_TAB_INFO DACMAP_Main[DAC_PANEL_NUMS]=
{
{
*MST_DAC_480I_60_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_480I_60_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_480I_60_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_480I_60_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_480I_60_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_480I_60_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_480I_60_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_480I_60_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_480I_60_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_480I_60_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_480I_60_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_480I_60_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_480P_60_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_480P_60_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_480P_60_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_480P_60_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_480P_60_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_480P_60_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_480P_60_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_480P_60_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_480P_60_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_480P_60_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_480P_60_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_480P_60_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_576I_50_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_576I_50_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_576I_50_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_576I_50_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_576I_50_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_576I_50_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_576I_50_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_576I_50_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_576I_50_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_576I_50_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_576I_50_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_576I_50_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_576P_50_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_576P_50_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_576P_50_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_576P_50_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_576P_50_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_576P_50_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_576P_50_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_576P_50_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_576P_50_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_576P_50_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_576P_50_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_576P_50_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_720P_50_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_720P_50_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_720P_50_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_720P_50_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_720P_50_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_720P_50_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_720P_50_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_720P_50_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_720P_50_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_720P_50_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_720P_50_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_720P_50_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_720P_60_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_720P_60_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_720P_60_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_720P_60_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_720P_60_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_720P_60_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_720P_60_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_720P_60_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_720P_60_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_720P_60_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_720P_60_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_720P_60_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080I_50_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080I_50_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080I_50_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080I_50_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080I_50_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080I_50_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080I_50_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080I_50_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080I_50_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080I_50_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080I_50_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080I_50_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080I_60_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080I_60_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080I_60_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080I_60_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080I_60_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080I_60_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080I_60_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080I_60_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080I_60_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080I_60_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080I_60_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080I_60_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080P_50_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080P_50_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080P_50_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080P_50_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080P_50_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080P_50_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080P_50_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080P_50_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080P_50_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080P_50_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080P_50_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080P_50_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080P_60_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080P_60_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080P_60_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080P_60_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080P_60_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080P_60_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080P_60_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080P_60_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080P_60_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080P_60_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080P_60_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080P_60_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080P_30_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080P_30_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080P_30_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080P_30_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080P_30_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080P_30_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080P_30_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080P_30_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080P_30_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080P_30_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080P_30_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080P_30_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080P_25_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080P_25_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080P_25_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080P_25_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080P_25_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080P_25_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080P_25_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080P_25_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080P_25_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080P_25_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080P_25_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080P_25_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_1080P_24_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_1080P_24_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_1080P_24_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_1080P_24_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_1080P_24_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_1080P_24_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_1080P_24_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_1080P_24_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_1080P_24_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_1080P_24_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_1080P_24_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_1080P_24_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
{
*MST_DAC_640x480_60_INIT_TBL, DAC_TABTYPE_INIT, 
*MST_DAC_640x480_60_INIT_GPIO_TBL, DAC_TABTYPE_INIT_GPIO, 
*MST_DAC_640x480_60_INIT_SC_TBL, DAC_TABTYPE_INIT_SC, 
*MST_DAC_640x480_60_INIT_MOD_TBL, DAC_TABTYPE_INIT_MOD, 
*MST_DAC_640x480_60_INIT_HDGEN_TBL, DAC_TABTYPE_INIT_HDGEN, 
*MST_DAC_640x480_60_INIT_HDMITX_8BIT_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT, 
*MST_DAC_640x480_60_INIT_HDMITX_8BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider, 
*MST_DAC_640x480_60_INIT_HDMITX_10BIT_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT, 
*MST_DAC_640x480_60_INIT_HDMITX_10BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider, 
*MST_DAC_640x480_60_INIT_HDMITX_12BIT_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT, 
*MST_DAC_640x480_60_INIT_HDMITX_12BIT_Divider_TBL, DAC_TABTYPE_INIT_HDMITX_12BIT_Divider, 
*MST_DAC_640x480_60_Gamma_TBL, DAC_TABTYPE_Gamma, 
},
};

#endif
