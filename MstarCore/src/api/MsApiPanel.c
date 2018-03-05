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
//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <command.h>
#include <MsTypes.h>
#include <apiPNL.h>
#include <MsDebug.h>
#include <MsBoot.h>
#include <MsEnvironment.h>
#include <bootlogo/MsPoolDB.h>
#include <panel/MsDrvPanel.h>
#include <MsApiPanel.h>
#if(ENABLE_URSA_6M30 == 1)
#include <ursa/ursa_6m30.h>
#endif

#if(ENABLE_URSA_6M40 == 1)
#include <ursa/ursa_6m40.h>
#endif

#if(ENABLE_URSA_8 == 1)
#include <ursa/ursa_8.h>
#endif

#include <MsMmap.h>
#include <MsSystem.h>
#include <mstarstr.h>
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
void cmp(PanelType *p1, PanelType *p2);

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PANEL_DEBUG 0

//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL bPanleReady=FALSE;
static GetPnlTypeSet_cb fpCusGetPnlTypeSet = NULL;

#if (ENABLE_HDMI_RESOLUTION_RESET==1)
#if (ENABLE_HDMI_TX_RESOLUTION == 0)
static char* DacPanelIndexTbl[] = {
    "DACOUT_480P_60",
    "DACOUT_576P_50",
    "DACOUT_720P_50",
    "DACOUT_720P_60",
    "DACOUT_1080P_50",
    "DACOUT_1080P_60",
    "DACOUT_480I_60",
    "DACOUT_576I_50",
    "DACOUT_1080I_50",
    "DACOUT_1080I_60",
    "DACOUT_1080P_30",
    "DACOUT_1080P_25",
    "DACOUT_1080P_24",
    "DACOUT_640X480_60",
    "DACOUT_1920X2205P_24",
    "DACOUT_1280X1470P_50",
    "DACOUT_1280X1470P_60",
    "DACOUT_4K2KP_24",
    "DACOUT_4K2KP_25",
    "DACOUT_4K2KP_30",
    "DACOUT_4K2KP_50",
    "DACOUT_4K2KP_60",
    "DACOUT_4096X2160P_24",
    "DACOUT_4096X2160P_25",
    "DACOUT_4096X2160P_30",
    "DACOUT_4096X2160P_50",
    "DACOUT_4096X2160P_60",
    "DACOUT_1024X768P_60",
    "DACOUT_1280X1024P_60",
    "DACOUT_1440X900P_60",
    "DACOUT_1600X1200P_60",
};

typedef enum{
    DACOUT_RES_480P_60              = 0,
    DACOUT_RES_576P_50              = 1,
    DACOUT_RES_720P_50              = 2,
    DACOUT_RES_720P_60              = 3,
    DACOUT_RES_1080P_50             = 4,
    DACOUT_RES_1080P_60             = 5,
    DACOUT_RES_480I_60              = 6,
    DACOUT_RES_576I_50              = 7,
    DACOUT_RES_1080I_50             = 8,
    DACOUT_RES_1080I_60             = 9,
    DACOUT_RES_1080P_30             = 10,
    DACOUT_RES_1080P_25             = 11,
    DACOUT_RES_1080P_24             = 12,
    DACOUT_RES_640x480_60           = 13,
    DACOUT_RES_1920x2205P_24        = 14,
    DACOUT_RES_1280x1470P_50        = 15,
    DACOUT_RES_1280x1470P_60        = 16,
    DACOUT_RES_3840x2160P_24        = 17,
    DACOUT_RES_3840x2160P_25        = 18,
    DACOUT_RES_3840x2160P_30        = 19,
    DACOUT_RES_3840x2160P_50        = 20,
    DACOUT_RES_3840x2160P_60        = 21,
    DACOUT_RES_4096x2160P_24        = 22,
    DACOUT_RES_4096x2160P_25        = 23,
    DACOUT_RES_4096x2160P_30        = 24,
    DACOUT_RES_4096x2160P_50        = 25,
    DACOUT_RES_4096x2160P_60        = 26,
    DACOUT_RES_1024x768P_60         = 27,
    DACOUT_RES_1280x1024P_60        = 28,
    DACOUT_RES_1440x900P_60         = 29,
    DACOUT_RES_1600x1200P_60        = 30,
}DACOUT_VIDEO_TIMING;

#else
#if (ENABLE_HDMITX_MSTAR_ROCKET2 == 1)
#include <hdmitx/mstar/rocket2/MsDrvRocket.h>
static char* HdmiTxPanelIndexTable[] = {
    "",//0
    "",//1
    "",//2
    "HDMITX_VB1_480_60P",//3/
    "HDMITX_VB1_576_50P",//4/
    "HDMITX_VB1_720_50P",//5
    "HDMITX_VB1_720_60P",//6
    "",//7
    "",//8
    "",//9
    "",//10
    "",//11
    "HDMITX_VB1_1080_50P",//12
    "HDMITX_VB1_1080_60P",//13
    "HDMITX_VB1_4K2K_30P",//14
    "",//15
    "",//16
    "",//17
    "",//18
    "",//19
    "",//20
    "HDMITX_VB1_4K2K_25P",//21
    "HDMITX_VB1_4K2K_24P",//22
    "HDMITX_VB1_4K2K_50P",//23
    "HDMITX_VB1_4K2K_60P",//24
    "",//25
    "",//26
    "",//27
    "HDMITX_VB1_4096_24P",//28
};

static char* HdmiTxTimingIndexTable[] = {
    "",//0
    "",//1
    "",//2
    "HDMITX_RES_720x480p",//3
    "HDMITX_RES_720x576p",//4
    "HDMITX_RES_1280x720p_50Hz",//5
    "HDMITX_RES_1280x720p_60Hz",//6
    "",//7
    "",//8
    "",//9
    "",//10
    "",//11
    "HDMITX_RES_1920x1080p_50Hz",//12
    "HDMITX_RES_1920x1080p_60Hz",//13
    "HDMITX_RES_4K2Kp_30Hz",//14
    "",//15
    "",//16
    "",//17
    "",//18
    "",//19
    "",//20
    "HDMITX_RES_4K2Kp_25Hz",//21
    "HDMITX_RES_4K2Kp_24Hz",//22
    "HDMITX_RES_4K2Kp_50Hz",//23
    "HDMITX_RES_4K2Kp_60Hz",//24
    "",//25
    "",//26
    "",//27
    "HDMITX_RES_4096_24P",//28
};
#else
#include <apiHDMITx.h>
static char* HdmiTxPanelIndexTable[] = {
    "",
    "HDMITX_480_60I",
    "HDMITX_576_50I",
    "HDMITX_480_60P",
    "HDMITX_576_50P",
    "HDMITX_720_50P",
    "HDMITX_720_60P",
    "HDMITX_1080_50I",
    "HDMITX_1080_60I",
    "HDMITX_1080_24P",
    "HDMITX_1080_25P",
    "HDMITX_1080_30P",
    "HDMITX_1080_50P",
    "HDMITX_1080_60P",
    "HDMITX_4K2K_30P",
    "HDMITX_1470_50P",
    "HDMITX_1470_60P",
    "HDMITX_1470_24P",
    "HDMITX_1470_30P",
    "HDMITX_2205_24P",
    "HDMITX_2205_30P",
    "HDMITX_4K2K_25P",
};

static char* HdmiTxTimingIndexTable[] = {
    "HDMITX_RES_640x480p",
    "HDMITX_RES_720x480i",
    "HDMITX_RES_720x576i",
    "HDMITX_RES_720x480p",
    "HDMITX_RES_720x576p",
    "HDMITX_RES_1280x720p_50Hz",
    "HDMITX_RES_1280x720p_60Hz",
    "HDMITX_RES_1920x1080i_50Hz",
    "HDMITX_RES_1920x1080i_60Hz",
    "HDMITX_RES_1920x1080p_24Hz",
    "HDMITX_RES_1920x1080p_25Hz",
    "HDMITX_RES_1920x1080p_30Hz",
    "HDMITX_RES_1920x1080p_50Hz",
    "HDMITX_RES_1920x1080p_60Hz",
    "HDMITX_RES_4K2Kp_30Hz",
    "HDMITX_RES_1280x1470p_50Hz",
    "HDMITX_RES_1280x1470p_60Hz",
    "HDMITX_RES_1280x1470p_24Hz",
    "HDMITX_RES_1280x1470p_30Hz",
    "HDMITX_RES_1920x2205p_24Hz",
    "HDMITX_RES_1920x2205p_30Hz",
    "HDMITX_RES_4K2Kp_25Hz",
};
#endif
#endif
#endif

#if (CONFIG_TV_CHIP==0 )
typedef struct
{
    MS_U8                   u8ResolutionEnv;
    PANEL_RESOLUTION_TYPE   enResolutionType;
}RESOLUTION_DAC_MAP;

static RESOLUTION_DAC_MAP stMapTypeIndex[] = {
    {0, DACOUT_480P},
    {1, DACOUT_576P},
    {2, DACOUT_720P_50},
    {3, DACOUT_720P_60},
    {4, DACOUT_1080P_50},
    {5, DACOUT_1080P_60},
    {6, DACOUT_480I},
    {7, DACOUT_576I},
    {8, DACOUT_1080I_50},
    {9, DACOUT_1080I_60},

    {27, DACOUT_1024x768P_60},
    {28, DACOUT_1280x1024P_60},
    {29, DACOUT_1440x900P_60},
    {30, DACOUT_1600x1200P_60}
};
#endif

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
int panel_sinit(void);
int panel_dinit(void);
static PANEL_RESOLUTION_TYPE _GetPnlTypeSetting(void);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int IsPanelReady(void)
{
    if(bPanleReady==TRUE)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

APIPNL_LINK_EXT_TYPE GetPanelLinkExtType(void)
{
    APIPNL_LINK_EXT_TYPE eType = LINK_EPI34_8P;
    st_sys_misc_setting misc_setting;
    Read_MiscSetting_ToFlash(&misc_setting);
    if((APIPNL_LINK_EXT_TYPE)misc_setting.m_u16Panel_ext_type!=0)
        eType = (APIPNL_LINK_EXT_TYPE)misc_setting.m_u16Panel_ext_type;
    return eType;
}

void RegisterCBGetPnlTypeSetting(GetPnlTypeSet_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpCusGetPnlTypeSet=cb;
    UBOOT_TRACE("OK\n");
}

#if (CONFIG_TV_CHIP==0 )
static PANEL_RESOLUTION_TYPE _GetPnlTypeSettingFromEnv(void)
{
    int resolution_index = 0;
    PANEL_RESOLUTION_TYPE PanelType = DACOUT_1080I_50;
    char * p_str;
    int i = 0;

    p_str = getenv ("resolution");
    if(p_str != NULL)
    {
        resolution_index = (int)simple_strtol(p_str, NULL, 10);

        // printf("_GetPnlTypeSettingFromEnv = %d, resolution_index %d \n", sizeof(stMapTypeIndex)/sizeof(RESOLUTION_DAC_MAP), resolution_index);
        for (i = 0; i < sizeof(stMapTypeIndex)/sizeof(RESOLUTION_DAC_MAP); i++)
        {
            if (resolution_index == stMapTypeIndex[i].u8ResolutionEnv)
            {
                printf("PanelType 11 = %d", stMapTypeIndex[i].enResolutionType);
                return stMapTypeIndex[i].enResolutionType;
            }
        }
    }

    return PanelType;
}
#endif
static PANEL_RESOLUTION_TYPE _GetPnlTypeSetting(void)
{
    PANEL_RESOLUTION_TYPE PanelType = 0;
    UBOOT_TRACE("IN\n");
    if(fpCusGetPnlTypeSet!=NULL)
    {
        PanelType = fpCusGetPnlTypeSet();
    }
    else
    {
        PanelType = MApi_PNL_GetPnlTypeSetting();
    }
    UBOOT_TRACE("OK\n");
    return PanelType;
}

#if CONFIG_STATIC_PANEL_PARA
int panel_sinit(void)
{
    PanelType* panel_data = NULL;
    PANEL_RESOLUTION_TYPE enPanelType;
    UBOOT_TRACE("IN\n");

#if (CONFIG_TV_CHIP==0 )
    RegisterCBGetPnlTypeSetting(_GetPnlTypeSettingFromEnv);
#endif

    enPanelType = _GetPnlTypeSetting();
    if(MApi_PNL_PreInit(E_PNL_NO_OUTPUT)!=TRUE)
    {
        bPanleReady=FALSE;
        return -1;
    }

    panel_data=MApi_GetPanelSpec(enPanelType);
    if(panel_data==NULL)
    {
        bPanleReady=FALSE;
        return -1;
    }

    if(panel_data->m_wPanelWidth == 3840 && panel_data->m_wPanelHeight == 2160)
    {
        UBOOT_DEBUG(">> m_ePanelLinkExtType = 51 <<<\n");
        setLinkExtType(51);
    }
    //pane init
    #ifdef BD_LVDS_CONNECT_TYPE
	MApi_BD_LVDS_Output_Type(BD_LVDS_CONNECT_TYPE);
    #endif
    if(MsDrv_PNL_Init(panel_data)==FALSE)
    {
        bPanleReady=FALSE;
        return -1;
    }
    bPanleReady=TRUE;
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#if (ENABLE_HDMI_RESOLUTION_RESET==1)
static int BootArgs_reset(void)
{
    int resolution_index = 5;
    char p_str[] = "\0";
    char *p;
    char as8PanelFilePath[50] = "\0";
    #if (ENABLE_HDMI_TX_RESOLUTION == 1)
    #if (ENABLE_HDMITX_MSTAR_ROCKET2 == 1)
    EN_MAPI_DEVICE_ROCKY_VIDEO_TIMING eHDMITimgType = E_MAPI_ROCKY_RES_1920x1080p_60Hz;
    #else
    HDMITX_VIDEO_TIMING eHDMITimgType = HDMITX_RES_1280x720p_50Hz;
    #endif
    #else
    DACOUT_VIDEO_TIMING ePANELType = DACOUT_RES_720P_50;
    #endif

    p = getenv ("resolution_reset");
    if(p != NULL)
    {
        resolution_index = (int)simple_strtol(p, NULL, 10);
    }

    if (getenv ("resolution_reset") == NULL)
    {
        setenv("resolution_reset", getenv ("resolution"));
    }
    else
    {
        if (0 == strcmp(getenv("resolution"), getenv ("resolution_reset")))
        {
            #if (ENABLE_HDMI_TX_RESOLUTION == 1)
            snprintf(as8PanelFilePath, sizeof(as8PanelFilePath),"/config/panel/%s.ini", HdmiTxPanelIndexTable[resolution_index]);
            #else
            snprintf(as8PanelFilePath, sizeof(as8PanelFilePath),"/config/panel/%s.ini", DacPanelIndexTbl[resolution_index]);
            #endif

            if(strcmp(as8PanelFilePath, getenv("panel_path")) == 0)
                return 0;
        }
        setenv("resolution", getenv ("resolution_reset"));
    }

    p = getenv ("resolution");
    if(p != NULL)
    {
        resolution_index = (int)simple_strtol(p, NULL, 10);
        #if (ENABLE_HDMI_TX_RESOLUTION == 1)
        MApi_SetEnv2BootArgs("resolution=", HdmiTxTimingIndexTable[resolution_index]);
        snprintf(as8PanelFilePath, sizeof(as8PanelFilePath),"/config/panel/%s.ini", HdmiTxPanelIndexTable[resolution_index]);
        MApi_SetEnv2BootArgs("panel_path=", as8PanelFilePath);
        setenv("panel_path", as8PanelFilePath);
        setenv("panel_name", HdmiTxPanelIndexTable[resolution_index]);
        #else
        MApi_SetEnv2BootArgs("resolution=", DacPanelIndexTbl[resolution_index]);
        snprintf(as8PanelFilePath, sizeof(as8PanelFilePath),"/config/panel/%s.ini", DacPanelIndexTbl[resolution_index]);
        setenv("panel_path", as8PanelFilePath);
        setenv("panel_name", DacPanelIndexTbl[resolution_index]);
        #endif
        UBOOT_DEBUG("bootArgs need to reset !!\n");
        UBOOT_DEBUG("panel_path = %s\n", as8PanelFilePath);
        UBOOT_DEBUG("resolution = %s\n", getenv ("resolution"));
        setenv("db_table", "0");
        run_command("dbtable_init", 0);
    }
    else
    {
        //set env normal resolution.
        #if (ENABLE_HDMI_TX_RESOLUTION == 1)
        setenv("resolution", "24");  //HDMITX_VB1_4K2K_60P
        #else
        setenv("resolution", "5");   //DACOUT_1080P_60
        #endif
    }
    saveenv();
    return 0;
}
#endif

#if (!CONFIG_STATIC_PANEL_PARA)
int panel_dinit(void)
{
    MS_U16 u16Panel_SwingLevel;
    PanelType panelpara;
    st_board_para boardpara;
    st_sys_misc_setting misc_setting;
    UBOOT_TRACE("IN\n");

    memset(&panelpara, 0, sizeof(panelpara));
    memset(&boardpara, 0, sizeof(boardpara));

    #if (ENABLE_HDMI_RESOLUTION_RESET==1)
    BootArgs_reset();
    #endif
    //load panel para
#if (defined(CONFIG_URSA6_VB1) || defined(CONFIG_NOVA_KS2) || defined(CONFIG_URSA9_VB1))
	if(is_str_resume())
	{
		U32 u32PanelConfigsAddr;
        // When suspending, SN will store the latest panel param
        // for mboot getting the right timing.
		if(get_value_from_env(E_MMAP_ID_VDEC_CPU_ADR, NO_DEFAULT_MMAP_VALUE, &u32PanelConfigsAddr)!=0)
		{
			UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_VDEC_CPU_ADR);
			bPanleReady=FALSE;
			return -1;
		}
		UBOOT_DEBUG("E_MMAP_ID_VDEC_CPU = 0x%x\n", u32PanelConfigsAddr);
		UBOOT_DEBUG("(U32)(PA2NVA(u32PanelConfigsAddr)) = 0x%x\n", (U32)(PA2NVA(u32PanelConfigsAddr)));
		memcpy(&panelpara, (U32*)(PA2NVA(u32PanelConfigsAddr)), sizeof(PanelType));
        // Error handle when SN failed to pass panel param.
        //
        // Only for Utopia2k STR bringup,
        // this code should be moved to ursa(backend) kernel driver later.
        if(!(panelpara.m_wPanelWidth == 3840 && panelpara.m_wPanelHeight == 2160)
                && !(panelpara.m_wPanelWidth == 1920 && panelpara.m_wPanelHeight == 1080))
        {
            UBOOT_DEBUG(">>> can't get the right param from share memory, try flash anyway.");
            if(Read_PanelParaFromflash(&panelpara)!=0)
            {
                bPanleReady=FALSE;
                UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
                return -1;
            }
        }
	}
	else
	{
		if(Read_PanelParaFromflash(&panelpara)!=0)
		{
			bPanleReady=FALSE;
			UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
			return -1;
		}
	}
#else
    if(Read_PanelParaFromflash(&panelpara)!=0)
    {
        bPanleReady=FALSE;
        UBOOT_ERROR("%s: Read_PanelParaFromflash() failed, at %d\n", __func__, __LINE__);
        return -1;
    }
#endif

    //load board para
    if(Read_BoardParaFromflash(&boardpara)!=0)
    {
        bPanleReady=FALSE;
        UBOOT_ERROR("%s: Read_BoardParaFromflash() failed, at %d\n", __func__, __LINE__);
        return -1;
    }

    //panel setting by each board
    panelpara.m_bPanelPDP10BIT = boardpara.m_bPANEL_PDP_10BIT;
    panelpara.m_bPanelSwapLVDS_POL = boardpara.m_bPANEL_SWAP_LVDS_POL;
    panelpara.m_bPanelSwapLVDS_CH = boardpara.m_bPANEL_SWAP_LVDS_CH;
    panelpara.m_bPanelSwapPort ^= boardpara.m_bPANEL_CONNECTOR_SWAP_PORT;
    panelpara.m_u16LVDSTxSwapValue = (boardpara.m_u16LVDS_PN_SWAP_H << 8) + boardpara.m_u16LVDS_PN_SWAP_L;


    MS_U16 u16PanelDCLK=0;

    Read_MiscSetting_ToFlash(&misc_setting);
    u16PanelDCLK = misc_setting.m_u16PanelDCLK;

    if(panelpara.m_ePanelLinkType >= LINK_EXT )
    {
        UBOOT_DEBUG(">> SW setting: m_ePanelLinkExtType = %d <<<\n",panelpara.m_ePanelLinkType);
        UBOOT_DEBUG(">> GetPanelLinkExtType = %d <<<\n",GetPanelLinkExtType());
        UBOOT_DEBUG(">> u16PanelDCLK = %u <<<\n",u16PanelDCLK);
        setLinkExtType(GetPanelLinkExtType());
    }


#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M40 == 1)
    MDrv_Ursa_6M40_Set_VB1_Init(GetPanelLinkExtType());
#endif
#if (ENABLE_URSA_8 ==1 )||(ENABLE_URSA_6M40 == 1)
       ursa_cmd_table cmd_table={0};
	   if(Read_Ursa_Para(&cmd_table)==0)
	   {
	   	#if (ENABLE_URSA_8 == 1)
		   Ursa_8_Setting(&cmd_table);
		#else
			Ursa_6M40_Syetting(&cmd_table);
		#endif
	   }
	   else
	   {
		  UBOOT_ERROR("read ursa_8 data fail ...>>>\n");
	   }

#endif

#if (ENABLE_URSA_6M30 == 1)
    MDrv_Ursa_6M30_Initialize();
    if(bMst6m30Installed)
    {
        ursa_6m30_cmd_table cmd_table={0};
        if(Read_Ursa_6m30_Para(&cmd_table)==0)
        {
            Ursa_6M30_Setting(&cmd_table);
        }
        else
        {
           UBOOT_ERROR("read ursa_6m30 data fail ...>>>\n");
        }
    }
    else
    {
           UBOOT_ERROR("ursa_6m30 installed fail ...>>>\n");
    }
#endif
#endif

#if PANEL_DEBUG
    PanelType* panel_data = NULL;
    PANEL_RESOLUTION_TYPE enPanelType;
    enPanelType = MApi_PNL_GetPnlTypeSetting();
    MApi_PNL_PreInit(E_PNL_NO_OUTPUT);
    panel_data=MApi_GetPanelSpec(enPanelType);
    cmp(panel_data,&panelpara);
#endif

    MApi_BD_LVDS_Output_Type(boardpara.m_u16BOARD_LVDS_CONNECT_TYPE);
    UBOOT_DEBUG("MApi_BD_LVDS_Output_Type =0x%x\n",boardpara.m_u16BOARD_LVDS_CONNECT_TYPE);
    if(MsDrv_PNL_Init(&panelpara)==FALSE)
    {
        bPanleReady=FALSE;
        return -1;
    }

    //set swing level
    st_sys_misc_setting misc_data;
    Read_MiscSetting_ToFlash(&misc_data);
    u16Panel_SwingLevel = misc_data.m_u16Panel_SwingLevel;
#if ( ENABLE_HDMITX_MSTAR_ROCKET==0)
    if(MApi_PNL_Control_Out_Swing(u16Panel_SwingLevel)!=TRUE)
    {
        bPanleReady=FALSE;
        return -1;
    }
#endif
    bPanleReady=TRUE;

    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

int do_panel_pre_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
#if (CONFIG_STATIC_PANEL_PARA)
        ret = panel_sinit();
#else
        ret = panel_dinit();
#endif
    }
    else
    {
#if CONFIG_MINIUBOOT
#else
       if(strncmp(argv[1], "-d", 2) == 0)
       {
            ret = panel_dinit();
       }
       else if (strncmp(argv[1], "-s", 2) == 0)
       {
            ret = panel_sinit();
       }
       else
#endif
       {
           cmd_usage(cmdtp);
       }
    }
    printf("[AT][MB][panel_pre_init][%lu]\n", MsSystemGetBootTime());
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (!CONFIG_UBOOT_MINI_LIB)
int do_panel_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    if (argc < 2)
    {
#if (CONFIG_STATIC_PANEL_PARA)
        ret = panel_sinit();
#else
        ret = panel_dinit();
#endif
    }
    else
    {
       if(strncmp(argv[1], "-d", 2) == 0)
       {
            ret = panel_dinit();
       }
       else if (strncmp(argv[1], "-s", 2) == 0)
       {
            ret = panel_sinit();
       }
       else
       {
           cmd_usage(cmdtp);
		   return ret;
       }
    }
	if(bPanleReady)
	{
   		MsDrv_PNL_BackLigth_On();
	}
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

int do_backLigth_on(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
	if(bPanleReady)
	{
   		MsDrv_PNL_BackLigth_On();
	}
	else
	{
		UBOOT_ERROR("do Panel init first!!\n");
	}
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (CONFIG_LOCAL_DIMMING)
#include <drvMSPI.h>
#include <drvLDMA.h>
#include <bootlogo/MsPoolDB.h>
#define MHAL_LD_PACKLENGTH    (32)
#define LDFALIGN              (8)          //align bits
#define LD_BIN_LENGTH         (0x80000)    //bin buffer
#define BRIGHT_LEVEL_DEFAULT  (0xff)
#define LD_MAX_BLOCK          (10*1024)    //follow kernel setting in file linaro\mstar2\drv\ldm\include\Mdrv_ldm_algorithm.h

int do_local_dimming( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ST_DRV_MSPI_INFO mspi_info;
    ST_DRV_LD_DMA_INFO dma_info;
    MS_U8 * pLEDVirBuffer;
    MS_U32 LEDVirBufferSize;
    unsigned int u32BaseAddr = 0;
    unsigned int u32Size = 0;
    MS_U8 u8Bright = BRIGHT_LEVEL_DEFAULT;
    MS_U32 u32LDFAddr_L0 = 0;
    MS_U32 u32LDFAddr_L1 = 0;
    MS_U32 u32LDFAddr_R0 = 0;
    MS_U32 u32LDFAddr_R1 = 0;
    MS_U32 u32LDBAddr_L0 = 0;
    MS_U32 u32LDBAddr_L1 = 0;
    MS_U32 u32LDBAddr_R0 = 0;
    MS_U32 u32LDBAddr_R1 = 0;
    MS_U32 LDF_pack_per_row = 0;
    MS_U32 LDF_mem_size = 0;
    MS_U8 u8LDBWidth = 0;
    MS_U8 u8LDBHeight = 0;
    MS_U32 LDB_pack_per_row = 0;
    MS_U32 LDB_mem_size = LD_MAX_BLOCK;
    MS_U32 u32LDBBaseAddr = 0;
    MS_U32 u32Edge2DBaseAddr = 0;
    MS_U32 u32DMABaseOffset = 0;

    UBOOT_TRACE("IN");
    if (argc >= 2) {
        u8Bright = simple_strtoul(argv[1], NULL, 16);
    }

    memset(&mspi_info,0,sizeof(ST_DRV_MSPI_INFO));
    memset(&dma_info,0,sizeof(ST_DRV_LD_DMA_INFO));

    if(0!=Read_MSPIPara_FromFash(&mspi_info) || 0!=Read_DMAPara_FromFash(&dma_info))
    {
        UBOOT_ERROR("Read_MSPIPara_FromFash fail\n");
    }

    //get mmap addr
    if(get_value_from_env(E_MMAP_ID_LOCAL_DIMMING_ADR, NO_DEFAULT_MMAP_VALUE, &u32BaseAddr)!=0)
    {
        UBOOT_ERROR("get %s mmap addr fail\n", E_MMAP_ID_LOCAL_DIMMING_ADR);
        return -1;
    }
    if(get_value_from_env(E_MMAP_ID_LOCAL_DIMMING_LEN, NO_DEFAULT_MMAP_VALUE, &u32Size)!=0)
    {
        UBOOT_ERROR("get %s mmap size fail\n", E_MMAP_ID_LOCAL_DIMMING_LEN);
        return -1;
    }

    //Init MSPI
    MDrv_MasterSPI_CsPadConfig(0,0xFF);//default cs pin use mspi config3
    MDrv_MSPI_Info_Config(&mspi_info);
    MDrv_MSPI_RWBytes(0,0x00);//set DMA read bytes
    MDrv_MSPI_RWBytes(1,0x01);//set DMA write bytes
    MDrv_MSPI_SlaveEnable(false);

    //LDF addr calc
    if(120 == dma_info.u8ClkHz)  //120Hz panel
    {
        LDF_pack_per_row = ((dma_info.u8LDFWidth/2 *4 - 1) / MHAL_LD_PACKLENGTH) + 1;
    }
    else
    {
        LDF_pack_per_row = ((dma_info.u8LDFWidth *4 - 1) / MHAL_LD_PACKLENGTH) + 1;
    }
    LDF_mem_size = LDF_pack_per_row * MHAL_LD_PACKLENGTH * MHAL_LD_PACKLENGTH;
    LDF_mem_size = ((LDF_mem_size + 0xFF) >> LDFALIGN) << LDFALIGN; // align at 0x100

    u32LDFAddr_L0 = u32BaseAddr/MHAL_LD_PACKLENGTH;
    u32LDFAddr_L1 = (u32BaseAddr + LDF_mem_size)/MHAL_LD_PACKLENGTH;
    if (120 == dma_info.u8ClkHz)
    {
        u32LDFAddr_R0 = (u32BaseAddr + 2*LDF_mem_size)/MHAL_LD_PACKLENGTH;
        u32LDFAddr_R1 = (u32BaseAddr + 3*LDF_mem_size)/MHAL_LD_PACKLENGTH;
    }
    else
    {
        u32LDFAddr_R0 = u32LDFAddr_L0;
        u32LDFAddr_R1 = u32LDFAddr_L1;
    }

    //LDB addr calc
    if(120 == dma_info.u8ClkHz)  //120Hz panel
    {
        u8LDBWidth = dma_info.bEdge2DEn
                      ? (dma_info.eLEDType == E_LD_EDGE_LR_TYPE ? dma_info.u8LEDHeight: dma_info.u8LEDWidth)
                      : dma_info.u8LEDWidth/2 + 1;
    }
    else
    {
        u8LDBWidth = dma_info.bEdge2DEn
                      ? (dma_info.eLEDType == E_LD_EDGE_LR_TYPE ? dma_info.u8LEDHeight: dma_info.u8LEDWidth)
                      : dma_info.u8LEDWidth;
    }

    u8LDBHeight = dma_info.bEdge2DEn
                   ? (dma_info.eLEDType == E_LD_EDGE_LR_TYPE ? dma_info.u8LEDWidth : dma_info.u8LEDHeight)
                   : dma_info.u8LEDHeight;
    LDB_pack_per_row = (dma_info.bEdge2DEn && dma_info.eLEDType == E_LD_LOCAL_TYPE) ? 2 :
                                ((u8LDBWidth - 1) / MHAL_LD_PACKLENGTH) + 1;

    if(120 == dma_info.u8ClkHz)
    {
        u32LDBBaseAddr = u32BaseAddr + 4*LDF_mem_size;
        u32Edge2DBaseAddr = u32LDBBaseAddr + 4*LDB_mem_size;
    }
    else
    {
        u32LDBBaseAddr = u32BaseAddr + 2*LDF_mem_size;
        u32Edge2DBaseAddr = u32LDBBaseAddr + 2*LDB_mem_size;
    }

    u32LDBAddr_L0 = u32LDBBaseAddr / MHAL_LD_PACKLENGTH;
    u32LDBAddr_L1 = (u32LDBBaseAddr + LDB_mem_size)/ MHAL_LD_PACKLENGTH;
    if (120 == dma_info.u8ClkHz)
    {
        u32LDBAddr_R0 = (u32LDBBaseAddr + 2*LDB_mem_size) / MHAL_LD_PACKLENGTH;
        u32LDBAddr_R1 = (u32LDBBaseAddr + 3*LDB_mem_size) / MHAL_LD_PACKLENGTH;
    }
    else
    {
        u32LDBAddr_R0 = u32LDBAddr_L0;
        u32LDBAddr_R1 = u32LDBAddr_L1;
    }
    u32DMABaseOffset = (u32LDFAddr_L0*0x20) + LD_BIN_LENGTH - (u32LDBAddr_L0*0x20);

    pLEDVirBuffer = (MS_U8 *)MS_PA2KSEG1(u32BaseAddr + LD_BIN_LENGTH);
    LEDVirBufferSize = LD_BIN_LENGTH;
    memset(pLEDVirBuffer, u8Bright, LEDVirBufferSize);    //set Bright value to LED buffer

    UBOOT_DEBUG("u32LDFAddr_L0 = 0x%lx\n", u32LDFAddr_L0);
    UBOOT_DEBUG("u32LDBAddr_L0 = 0x%lx\n", u32LDBAddr_L0);
    UBOOT_DEBUG("pLEDVirBuffer addr = 0x%lx\n", (MS_U32)pLEDVirBuffer);
    UBOOT_DEBUG("LEDVirBufferSize = 0x%lx\n", LEDVirBufferSize);
    UBOOT_DEBUG("u8Bright = 0x%x\n", u8Bright);

    MDrv_LDMA_Init(dma_info.u8LDMAchanel, dma_info.u8ClkHz);
    MDrv_LDMA_SetMenuloadNumber(dma_info.u8LDMAchanel, dma_info.u16LedNum);
    MDrv_LDMA_SetSPICommandFormat(dma_info.u8LDMAchanel, dma_info.u8cmdlength, dma_info.u16MspiHead);
    MDrv_LDMA_SetCheckSumMode(dma_info.u8LDMAchanel, dma_info.u8LDMACheckSumMode);
    MDrv_LDMA_SetSpiTriggerMode(dma_info.u8LDMAchanel, dma_info.u8LDMATrimode);
    MDrv_LDMA_SetTrigDelay(dma_info.u8LDMAchanel, dma_info.u16DMADelay);
    MDrv_LDMA_EnableCS(dma_info.u8LDMAchanel, true);
    MDrv_LDMA_LD_SetLDFAddr(0, u32LDFAddr_L0, u32LDFAddr_R0);
    MDrv_LDMA_LD_SetLDFAddr(1, u32LDFAddr_L1, u32LDFAddr_R1);
    MDrv_LDMA_LD_SetLDBAddr(0, u32LDBAddr_L0, u32LDBAddr_R0);
    MDrv_LDMA_LD_SetLDBAddr(1, u32LDBAddr_L1, u32LDBAddr_R1);
    MDrv_LDMA_LD_SetEdge2DAddr(u32Edge2DBaseAddr/MHAL_LD_PACKLENGTH);
    MDrv_LDMA_LD_SetLEDBufBaseOffset(u32DMABaseOffset/MHAL_LD_PACKLENGTH);
    MDrv_LDMA_LD_SetMIUPackOffset(dma_info.u8LDMAchanel, 0);
    MDrv_LDMA_LD_SetMIUPackLength(dma_info.u8LDMAchanel, dma_info.u8LEDWidth -1);
    MDrv_LDMA_LD_SetYoffEnd(dma_info.u8LDMAchanel, dma_info.u8LEDHeight -1);
    MDrv_LDMA_LD_SetBlHeightDMA(dma_info.u8LEDHeight -1);
    MDrv_LDMA_LD_SetBlWidthDMA(dma_info.u8LEDWidth -1);
    MDrv_LDMA_LD_SetDmaEnable(dma_info.u8LDMAchanel, true);
    MDrv_LDMA_LD_Enable(true);

    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#if PANEL_DEBUG
void cmp(PanelType *p1, PanelType *p2)
{
    if((p1 ==NULL) || (p2==NULL))
    {
        printf("null return\n");
        return;
    }
//    printf("compare: '%s', '%s'\n", p1->m_pPanelName, p2->m_pPanelName);
    if(p1->m_bPanelDither != p2->m_bPanelDither)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelDither, p2->m_bPanelDither, __LINE__);
    }
    if(p1->m_ePanelLinkType != p2->m_ePanelLinkType)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ePanelLinkType, p2->m_ePanelLinkType, __LINE__);
    }
    if(p1->m_bPanelDualPort != p2->m_bPanelDualPort)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelDualPort, p2->m_bPanelDualPort, __LINE__);
    }
    if(p1->m_bPanelSwapPort != p2->m_bPanelSwapPort)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapPort, p2->m_bPanelSwapPort, __LINE__);
    }
    if(p1->m_bPanelSwapOdd_ML != p2->m_bPanelSwapOdd_ML)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapOdd_ML, p2->m_bPanelSwapOdd_ML, __LINE__);
    }
    if(p1->m_bPanelSwapEven_ML != p2->m_bPanelSwapEven_ML)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapEven_ML, p2->m_bPanelSwapEven_ML, __LINE__);
    }
    if(p1->m_bPanelSwapOdd_RB != p2->m_bPanelSwapOdd_RB)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapOdd_RB, p2->m_bPanelSwapOdd_RB, __LINE__);
    }
    if(p1->m_bPanelSwapEven_RB != p2->m_bPanelSwapEven_RB)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapEven_RB, p2->m_bPanelSwapEven_RB, __LINE__);
    }
    if(p1->m_bPanelSwapLVDS_POL != p2->m_bPanelSwapLVDS_POL)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapLVDS_POL, p2->m_bPanelSwapLVDS_POL, __LINE__);
    }
    if(p1->m_bPanelSwapLVDS_CH != p2->m_bPanelSwapLVDS_CH)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapLVDS_CH, p2->m_bPanelSwapLVDS_CH, __LINE__);
    }
    if(p1->m_bPanelPDP10BIT != p2->m_bPanelPDP10BIT)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelPDP10BIT, p2->m_bPanelPDP10BIT, __LINE__);
    }
    if(p1->m_bPanelLVDS_TI_MODE != p2->m_bPanelLVDS_TI_MODE)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelLVDS_TI_MODE, p2->m_bPanelLVDS_TI_MODE, __LINE__);
    }
    if(p1->m_ucPanelDCLKDelay != p2->m_ucPanelDCLKDelay)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelDCLKDelay, p2->m_ucPanelDCLKDelay, __LINE__);
    }
    if(p1->m_bPanelInvDCLK != p2->m_bPanelInvDCLK)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelInvDCLK, p2->m_bPanelInvDCLK, __LINE__);
    }
    if(p1->m_bPanelInvDE != p2->m_bPanelInvDE)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelInvDE, p2->m_bPanelInvDE, __LINE__);
    }
    if(p1->m_bPanelInvHSync != p2->m_bPanelInvHSync)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelInvHSync, p2->m_bPanelInvHSync, __LINE__);
    }
    if(p1->m_bPanelInvVSync != p2->m_bPanelInvVSync)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelInvVSync, p2->m_bPanelInvVSync, __LINE__);
    }
    if(p1->m_ucPanelDCKLCurrent != p2->m_ucPanelDCKLCurrent)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelDCKLCurrent, p2->m_ucPanelDCKLCurrent, __LINE__);
    }
    if(p1->m_ucPanelDECurrent != p2->m_ucPanelDECurrent)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelDECurrent, p2->m_ucPanelDECurrent, __LINE__);
    }
    if(p1->m_ucPanelODDDataCurrent != p2->m_ucPanelODDDataCurrent)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelODDDataCurrent, p2->m_ucPanelODDDataCurrent, __LINE__);
    }
    if(p1->m_ucPanelEvenDataCurrent != p2->m_ucPanelEvenDataCurrent)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelEvenDataCurrent, p2->m_ucPanelEvenDataCurrent, __LINE__);
    }
    if(p1->m_wPanelOnTiming1 != p2->m_wPanelOnTiming1)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelOnTiming1, p2->m_wPanelOnTiming1, __LINE__);
    }
    if(p1->m_wPanelOnTiming2 != p2->m_wPanelOnTiming2)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelOnTiming2, p2->m_wPanelOnTiming2, __LINE__);
    }
    if(p1->m_wPanelOffTiming1 != p2->m_wPanelOffTiming1)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelOffTiming1, p2->m_wPanelOffTiming1, __LINE__);
    }
    if(p1->m_wPanelOffTiming2 != p2->m_wPanelOffTiming2)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelOffTiming2, p2->m_wPanelOffTiming2, __LINE__);
    }
    if(p1->m_ucPanelHSyncWidth != p2->m_ucPanelHSyncWidth)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelHSyncWidth, p2->m_ucPanelHSyncWidth, __LINE__);
    }
    if(p1->m_ucPanelHSyncBackPorch != p2->m_ucPanelHSyncBackPorch)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelHSyncBackPorch, p2->m_ucPanelHSyncBackPorch, __LINE__);
    }
    if(p1->m_ucPanelVSyncWidth != p2->m_ucPanelVSyncWidth)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelVSyncWidth, p2->m_ucPanelVSyncWidth, __LINE__);
    }
    if(p1->m_ucPanelVBackPorch != p2->m_ucPanelVBackPorch)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelVBackPorch, p2->m_ucPanelVBackPorch, __LINE__);
    }
    if(p1->m_wPanelHStart != p2->m_wPanelHStart)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelHStart, p2->m_wPanelHStart, __LINE__);
    }
    if(p1->m_wPanelVStart != p2->m_wPanelVStart)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelVStart, p2->m_wPanelVStart, __LINE__);
    }
    if(p1->m_wPanelWidth != p2->m_wPanelWidth)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelWidth, p2->m_wPanelWidth, __LINE__);
    }
    if(p1->m_wPanelHeight != p2->m_wPanelHeight)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelHeight, p2->m_wPanelHeight, __LINE__);
    }
    if(p1->m_wPanelMaxHTotal != p2->m_wPanelMaxHTotal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelMaxHTotal, p2->m_wPanelMaxHTotal, __LINE__);
    }
    if(p1->m_wPanelHTotal != p2->m_wPanelHTotal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelHTotal, p2->m_wPanelHTotal, __LINE__);
    }
    if(p1->m_wPanelMinHTotal != p2->m_wPanelMinHTotal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelMinHTotal, p2->m_wPanelMinHTotal, __LINE__);
    }
    if(p1->m_wPanelMaxVTotal != p2->m_wPanelMaxVTotal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelMaxVTotal, p2->m_wPanelMaxVTotal, __LINE__);
    }
    if(p1->m_wPanelVTotal != p2->m_wPanelVTotal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelVTotal, p2->m_wPanelVTotal, __LINE__);
    }
    if(p1->m_wPanelMinVTotal != p2->m_wPanelMinVTotal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wPanelMinVTotal, p2->m_wPanelMinVTotal, __LINE__);
    }
    if(p1->m_dwPanelMaxDCLK != p2->m_dwPanelMaxDCLK)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_dwPanelMaxDCLK, p2->m_dwPanelMaxDCLK, __LINE__);
    }
    if(p1->m_dwPanelDCLK != p2->m_dwPanelDCLK)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_dwPanelDCLK, p2->m_dwPanelDCLK, __LINE__);
    }
    if(p1->m_dwPanelMinDCLK != p2->m_dwPanelMinDCLK)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_dwPanelMinDCLK, p2->m_dwPanelMinDCLK, __LINE__);
    }
    if(p1->m_wSpreadSpectrumStep != p2->m_wSpreadSpectrumStep)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wSpreadSpectrumStep, p2->m_wSpreadSpectrumStep, __LINE__);
    }
    if(p1->m_wSpreadSpectrumSpan != p2->m_wSpreadSpectrumSpan)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_wSpreadSpectrumSpan, p2->m_wSpreadSpectrumSpan, __LINE__);
    }
    if(p1->m_ucDimmingCtl != p2->m_ucDimmingCtl)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucDimmingCtl, p2->m_ucDimmingCtl, __LINE__);
    }
    if(p1->m_ucMaxPWMVal != p2->m_ucMaxPWMVal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucMaxPWMVal, p2->m_ucMaxPWMVal, __LINE__);
    }
    if(p1->m_ucMinPWMVal != p2->m_ucMinPWMVal)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucMinPWMVal, p2->m_ucMinPWMVal, __LINE__);
    }
    if(p1->m_bPanelDeinterMode != p2->m_bPanelDeinterMode)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelDeinterMode, p2->m_bPanelDeinterMode, __LINE__);
    }
    if(p1->m_ucPanelAspectRatio != p2->m_ucPanelAspectRatio)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucPanelAspectRatio, p2->m_ucPanelAspectRatio, __LINE__);
    }
    if(p1->m_u16LVDSTxSwapValue != p2->m_u16LVDSTxSwapValue)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_u16LVDSTxSwapValue, p2->m_u16LVDSTxSwapValue, __LINE__);
    }
    if(p1->m_ucTiBitMode != p2->m_ucTiBitMode)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucTiBitMode, p2->m_ucTiBitMode, __LINE__);
    }
    if(p1->m_ucOutputFormatBitMode != p2->m_ucOutputFormatBitMode)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucOutputFormatBitMode, p2->m_ucOutputFormatBitMode, __LINE__);
    }
    if(p1->m_bPanelSwapOdd_RG != p2->m_bPanelSwapOdd_RG)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapOdd_RG, p2->m_bPanelSwapOdd_RG, __LINE__);
    }
    if(p1->m_bPanelSwapEven_RG != p2->m_bPanelSwapEven_RG)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapEven_RG, p2->m_bPanelSwapEven_RG, __LINE__);
    }
    if(p1->m_bPanelSwapOdd_GB != p2->m_bPanelSwapOdd_GB)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapOdd_GB, p2->m_bPanelSwapOdd_GB, __LINE__);
    }
    if(p1->m_bPanelSwapEven_GB != p2->m_bPanelSwapEven_GB)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelSwapEven_GB, p2->m_bPanelSwapEven_GB, __LINE__);
    }
    if(p1->m_bPanelDoubleClk != p2->m_bPanelDoubleClk)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelDoubleClk, p2->m_bPanelDoubleClk, __LINE__);
    }
    if(p1->m_dwPanelMaxSET != p2->m_dwPanelMaxSET)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_dwPanelMaxSET, p2->m_dwPanelMaxSET, __LINE__);
    }
    if(p1->m_dwPanelMinSET != p2->m_dwPanelMinSET)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_dwPanelMinSET, p2->m_dwPanelMinSET, __LINE__);
    }
    if(p1->m_ucOutTimingMode != p2->m_ucOutTimingMode)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_ucOutTimingMode, p2->m_ucOutTimingMode, __LINE__);
    }
    if(p1->m_bPanelNoiseDith != p2->m_bPanelNoiseDith)
    {
        printf("diff: '%u', '%u', at %u\n", p1->m_bPanelNoiseDith, p2->m_bPanelNoiseDith, __LINE__);
    }
}
#endif //PANEL_DEBUG
