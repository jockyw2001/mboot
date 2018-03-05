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

#ifndef _MS_API_PANEL_
#define _MS_API_PANEL_
#include <MsTypes.h>
#include <apiPNL.h>
#include <panel/panel.h>

/// Max order Size  for CH order
#define MAX_ORDER_SIZE      8

typedef enum
{
    //this valuse is offset in chunk header.
    VB1_CH_ORDER_16V=0x00,
    VB1_CH_ORDER_8V,
    VB1_CH_ORDER_4V,
    VB1_CH_ORDER_2V,
    VB1_CH_ORDER_1V,
    VB1_CH_ORDER_4O,
    VB1_CH_ORDER_2O,
    VB1_CH_ORDER_ORTHER=0xFF
} EN_VB1_CH_ORTHER_ITEM;

/// VB1ChannelOrder
typedef struct
{
    // 16V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb116vOrder[MAX_ORDER_SIZE];
    // 8V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb18vOrder[MAX_ORDER_SIZE];
    // 4V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb14vOrder[MAX_ORDER_SIZE];
    // 2V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb12vOrder[MAX_ORDER_SIZE];
    // 1V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb11vOrder[MAX_ORDER_SIZE];
    // 4O VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb14OOrder[MAX_ORDER_SIZE];
    // 2O VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb12OOrder[MAX_ORDER_SIZE];
}pnl_VB1ChannelOrder;

typedef struct
{
    MS_U8 URSA_6M30_LVDS_CHANNEL_SWAP_MODE;
    MS_U8 URSA_6M30_IN_LVDS_TIMODE;
    MS_U8 URSA_6M30_IN_LVDS_TI_BITNUM;
    MS_U8 URSA_6M30_OUT_LVDS_TIMODE;
    MS_U8 URSA_6M30_OUT_LVDS_TI_BITNUM;
    MS_U8 URSA_6M30_SSC_SEL;
    MS_U8 URSA_6M30_SSC_ENABLE;
    MS_U16 URSA_6M30_SSC_FREQ;
    MS_U16 URSA_6M30_SSC_PERCENT;
}ursa_6m30_cmd_table;

typedef struct
{
    MS_U8 URSA_EXTERNAL;
    MS_U8 URSA_MEMC_ENABLE;
    MS_U8 URSA_LVDS_CHANNEL_SWAP_MODE;
    MS_U8 URSA_LVDS_SWAP_POL;
    MS_U8 URSA_IN_LVDS_TIMODE;
    MS_U8 URSA_IN_LVDS_TI_BITNUM;
    MS_U8 URSA_OUT_LVDS_TIMODE;
    MS_U8 URSA_OUT_LVDS_TI_BITNUM;
    MS_U8 URSA_SSC_SEL;
    MS_U8 URSA_SSC_ENABLE;
    MS_U16 URSA_SSC_FREQ;
    MS_U16 URSA_SSC_PERCENT;
	MS_U16 URSA_OSD_PROTECT_MODE;
}ursa_cmd_table;

typedef struct
{
    MS_U8   m_u8BOARD_PWM_PORT;
    MS_U16  m_u16BOARD_LVDS_CONNECT_TYPE;
    MS_U8   m_bPANEL_PDP_10BIT;
    MS_U8   m_bPANEL_SWAP_LVDS_POL;
    MS_U8   m_bPANEL_SWAP_LVDS_CH;
    MS_U8   m_bPANEL_CONNECTOR_SWAP_PORT;
    MS_U16  m_u16LVDS_PN_SWAP_L;
    MS_U16  m_u16LVDS_PN_SWAP_H;
    char    m_sGPIO1_PAD_NAME[32];
    MS_U16  m_u16GPIO1_INDEX;
    MS_U8   m_u8GPIO1_VALUE;
    char    m_sGPIO2_PAD_NAME[32];
    MS_U16  m_u16GPIO2_INDEX;
    MS_U8   m_u8GPIO2_VALUE;
}st_board_para;

int do_panel_init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_panel_pre_init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

int do_backLigth_on( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_local_dimming( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//-------------------------------------------------------------------------------------------------
/// Query panle status
/// @return                                   \b OUT: 0: panel is ready, -1: panel is not ready
//-------------------------------------------------------------------------------------------------
int IsPanelReady(void);
APIPNL_LINK_EXT_TYPE GetPanelLinkExtType(void);

typedef PANEL_RESOLUTION_TYPE (*GetPnlTypeSet_cb)(void);
void RegisterCBGetPnlTypeSetting(GetPnlTypeSet_cb cb);


#endif

