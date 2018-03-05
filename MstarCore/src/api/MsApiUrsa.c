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

#include <command.h>
#include <common.h>
#include <MsDebug.h>
#include <MsApiUrsa.h>
#include <MsApiPanel.h>
#include <panel/MsDrvPanel.h>
#include <bootlogo/MsPoolDB.h>

#if defined(CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_6m38.h>
#endif

#if defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
#include <ursa/ursa_9.h>
#endif

#if defined(CONFIG_URSA_10) 
#include <ursa/ursa_10.h>
#endif

#include <ursa/ursa_vb1.h>

#if defined (CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa6(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if(0 != IsPanelReady())
    {
        UBOOT_ERROR("Panel is not ready\n");
        return -1;
    }
    MDrv_Ursa_6M38_SWI2C_Init();
    MDrv_Ursa_6M38_Mute(TRUE);
    MDrv_Ursa_6M38_4K2K60_init();
    udelay(30000);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    udelay(40000);
    MDrv_Ursa_6M38_4K2K_DisableAutoMute();
    MDrv_Ursa_6M38_Mute(FALSE);
    return 0;
}
#endif

#if defined (CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa9(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    PanelType panel_data;
    MDrv_Ursa_9_SWI2C_Init();
    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(2, 2);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(4, 4);
    }
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1_per_init(4, 8);
    }

    memset(&panel_data, 0, sizeof(panel_data));
    if(Read_PanelParaFromflash(&panel_data)!=0)
    {
        UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
        return -1;
    }

    if (LINK_VBY1_10BIT_2LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(1920, panel_data.m_wPanelWidth);
    }
    else if (LINK_VBY1_10BIT_4LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
    }
    else if (LINK_VBY1_10BIT_8LANE == getLinkExtType())
    {
        MDrv_Ursa_9_Set_Lane_VB1(3840, panel_data.m_wPanelWidth);
    }
    mdelay(30);
    MApi_PNL_SetVideoHWTraining(TRUE);  // Enable HW training.
    mdelay(40);
    return 0;
}
#endif

int do_send_I2C_cmd_unlock_transfer_board(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char* opt_unlock = NULL;
    opt_unlock = getenv("unlock_cmi");
    if(opt_unlock != NULL)
    {
       if(strcmp(opt_unlock,"1")==0)
        {
            UBOOT_TRACE("IN\n");
            MDrv_Cmi_Transfer_Unlock();
            UBOOT_TRACE("OK\n");
        }
    }
    return 0;
}


#if defined(CONFIG_URSA_10)
int do_send_I2C_cmd_to_ursa10(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MDrv_Ursa_10_Lvds_init();
    return 0;
}
#endif

#if defined(CONFIG_URSA6_VB1) ||  defined(CONFIG_URSA_10) || defined(CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
int do_send_I2C_cmd_to_ursa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#if defined (CONFIG_URSA6_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa6(cmdtp,flag,argc,argv);
#endif
#if defined (CONFIG_URSA9_VB1) || defined(CONFIG_URSA_UNION)
    do_send_I2C_cmd_to_ursa9(cmdtp,flag,argc,argv);
#endif
#if defined(CONFIG_URSA_10)
    do_send_I2C_cmd_to_ursa10(cmdtp,flag,argc,argv);
#endif
    return 0;
}
#endif
