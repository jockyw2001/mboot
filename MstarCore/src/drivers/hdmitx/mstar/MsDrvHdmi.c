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

#include <common.h>
#include <command.h>
#include <MsCommon.h>
#include <MsTypes.h>
#include <apiPNL.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <panel/MsDrvPanel.h>
#include <apiHDMITx.h>
#include <apiDAC.h>
#include <drvDAC.h>
#include <drvTVEncoder.h>
#include <MsApiPM.h>
#include <MsEnvironment.h>
#include <MsVfs.h>
#include <malloc.h>
#include <drvSYS.h>
#include <drvXC_IOPort.h>
#include <apiXC.h>
#include <CusConfig.h>
#include <mdrv_dac_tbl.h>
#include <mdrv_dac_tbl.c>
#include <mstarstr.h>

#if(ENABLE_MSTAR_KANO == 1 ||ENABLE_MSTAR_K6 == 1 ||ENABLE_MSTAR_K6LITE == 1 ||ENABLE_MSTAR_CURRY == 1)
#include <apiPNL_EX.h>
#include <apiXC_EX.h>
#include <apiXC_PCMonitor_EX.h>
#endif


#define VE_FRAMEBUFFER_OFFSET       (16)
#define VE_WRITE_ADDR_ADJUSTMENT    (-8)
#define VE_READ_ADDR_ADJUSTMENT     (0)

typedef enum
{
    E_HDMITX_VIDEO_SI_NoData       = 0,
    E_HDMITX_VIDEO_SI_Overscanned  = 1,
    E_HDMITX_VIDEO_SI_Underscanned = 2,
    E_HDMITX_VIDEO_SI_Reserved     = 3,
} MsHDMITX_VIDEO_SCAN_INFO;

typedef enum
{
    E_HDMITX_VIDEO_AFD_SameAsPictureAR    = 8, // 1000
    E_HDMITX_VIDEO_AFD_4_3_Center         = 9, // 1001
    E_HDMITX_VIDEO_AFD_16_9_Center        = 10, // 1010
    E_HDMITX_VIDEO_AFD_14_9_Center        = 11, // 1011
    E_HDMITx_VIDEO_AFD_Others = 15, // 0000~ 0111, 1100 ~ 1111
} MsHDMITX_VIDEO_AFD_RATIO;

//auto-timing detect
#define DISPOUT_TIMING_MAX (31)
#define DISPOUT_3D_Max (9)
#define DISPOUT_AUDIO_TYPE_DDS_MAX (15)
#define EDID_VIC_4K2K_NUM (4)
#define EDID_VIC_4K2K420_NUM (32)
#define EDID_VIC_NUM     (107)
#define DACOUT720P50     (2)
#define DACOUT1080P50    (4)
#define DACOUT3840P24    (17)

typedef enum
{
    E_HDMITX_HDCP_DISABLE   = 0, // HDCP disable
    E_HDMITX_HDCP_FAIL      = 1, // HDCP fail
    E_HDMITX_HDCP_PASS      = 2, // HDCP pass
} MsHDMITX_HDCP_STATUS;


#define  HDMITX_EDID_NON_STATUS               ( 0 )
#define  HDMITX_EDID_VALID_BIT                (1<<0)
#define  HDMITX_EDID_4K2K_VALID_BIT           (1<<1)
#define  HDMITX_EDID_4K2K420_VALID_BIT        (1<<2)


typedef struct
{
    MS_BOOL bSupportedTiming[DISPOUT_TIMING_MAX];//get suported timing from EDID.
    MS_BOOL bSupportedAudioType[DISPOUT_AUDIO_TYPE_DDS_MAX];
    MS_BOOL bSupported3DType[DISPOUT_3D_Max];
    MS_BOOL bIsConnected;
    MS_BOOL bBestSupportedTiming[DISPOUT_TIMING_MAX];//get the best suported timing from EDID.
    MsHDMITX_HDCP_STATUS eHdcpStatus;
} MsHDMITX_DEVICEINFO;

typedef struct
{
    MS_U8                   u8ResolutionEnv;
    PANEL_RESOLUTION_TYPE   enResolutionType;
    E_DAC_PANEL_INDEX       enDACIndex;
}HDMI_RESOLUTION_DAC_MAP;

#define RESOLUTION_COUNTER DISPOUT_TIMING_MAX


#define HDMITX_INFO_SIZE    3


#define DONT_EN_CVBS_ENV    "CVBSOffWhenHDMIOn"


static HDMITX_ANALOG_TUNING astHdmiAnalogCfg[HDMITX_INFO_SIZE] =
{
    // sd
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // HD color depth 24bit, 30bit
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
    // DEEP_HD color depth 36bit
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x07},
};

/* display resolution
DACOUT_1080P_50 -> 4//resolution env via SN
DACOUT_1080P_60 -> 5
DACOUT_720P_50 -> 2
DACOUT_720P_60 -> 3
DACOUT_576P_60 -> 1
DACOUT_480P_60 -> 0
*/
static HDMI_RESOLUTION_DAC_MAP staMapTypeIndex[RESOLUTION_COUNTER] = {
    {0, DACOUT_480P,       DAC_PANEL_480P_60},
    {1, DACOUT_576P,       DAC_PANEL_576P_50},
    {2, DACOUT_720P_50,    DAC_PANEL_720P_50},
    {3, DACOUT_720P_60,    DAC_PANEL_720P_60},
    {4, DACOUT_1080P_50,   DAC_PANEL_1080P_50},

    {5, DACOUT_1080P_60,   DAC_PANEL_1080P_60},
    {6, DACOUT_480I,       DAC_PANEL_480I_60},
    {7, DACOUT_576I,       DAC_PANEL_576I_50},
    {8, DACOUT_1080I_50,   DAC_PANEL_1080I_50},
    {9, DACOUT_1080I_60,   DAC_PANEL_1080I_60},
#if (defined(CONFIG_KANO) || defined(CONFIG_CURRY)|| defined(CONFIG_K6)|| defined(CONFIG_K6LITE))
    {10, DACOUT_1080P_30,   DAC_PANEL_1080P_30},
    {11, DACOUT_1080P_25,   DAC_PANEL_1080P_25},
    {12, DACOUT_1080P_24,   DAC_PANEL_1080P_24},
    {13, DACOUT_640x480_60,   DAC_PANEL_640x480_60},
    {14, DACOUT_1920x2205P_24,   DAC_PANEL_1920x2205P_24},

    {15, DACOUT_1280x1470P_50,   DAC_PANEL_1280x1470P_50},
    {16, DACOUT_1280x1470P_60,   DAC_PANEL_1280x1470P_60},
    {17, DACOUT_3840x2160P_24,   DAC_PANEL_3840x2160P_24},
    {18, DACOUT_3840x2160P_25,   DAC_PANEL_3840x2160P_25},
    {19, DACOUT_3840x2160P_30,   DAC_PANEL_3840x2160P_30},

    {20, DACOUT_3840x2160P_50,   DAC_PANEL_3840x2160P_50},
    {21, DACOUT_3840x2160P_60,   DAC_PANEL_3840x2160P_60},
    {22, DACOUT_4096x2160P_24,   DAC_PANEL_4096x2160P_24},
    {23, DACOUT_4096x2160P_25,   DAC_PANEL_4096x2160P_25},
    {24, DACOUT_4096x2160P_30,   DAC_PANEL_4096x2160P_30},
    {25, DACOUT_4096x2160P_50,   DAC_PANEL_4096x2160P_50},

    {26, DACOUT_4096x2160P_60,   DAC_PANEL_4096x2160P_60},

    {27, DACOUT_1024x768P_60,    DAC_PANEL_1024x768P_60},
    {28, DACOUT_1280x1024P_60,   DAC_PANEL_1280x1024P_60},
    {29, DACOUT_1440x900P_60,    DAC_PANEL_1440x900P_60},
    {30, DACOUT_1600x1200P_60,   DAC_PANEL_1600x1200P_60},
#endif
};

static char* DacPropPanelIndexTbl[] = {
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
    "DACOUT_1024x768P_60",
    "DACOUT_1280x1024P_60",
    "DACOUT_1440x900P_60",
    "DACOUT_1600x1200P_60",
};

static MS_BOOL getResolutionTypeAndDACIndex(MS_U16 *pPanelType, MS_U16 *pDACIndex)
{
    int u8ResolutionEnv = -1;
    char * p_str = NULL;

    if((NULL == pPanelType) || (NULL == pDACIndex))
    {
        printf("error, paremetre is invalid!\n");
        return FALSE;
    }

    p_str = getenv ("resolution");
    if(NULL != p_str)
    {
        u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
    }
     else
    {
        //set env normal resolution.
        setenv("resolution", "5");
        saveenv();
    }
    //printf("getResolutionTypeAndDACIndex,line:%d£¬ u8ResolutionEnv:%d\n",__LINE__,u8ResolutionEnv);

    if((RESOLUTION_COUNTER <= u8ResolutionEnv) || (0 > u8ResolutionEnv))
    {
        printf("error, resolution env is out of [0--9]!\n");
        return FALSE;
    }

    *pPanelType = staMapTypeIndex[u8ResolutionEnv].enResolutionType;
    *pDACIndex  = staMapTypeIndex[u8ResolutionEnv].enDACIndex;
    //printf("getResolutionTypeAndDACIndex,line:%d£¬ enResolutionType:%d, enDACIndex:%d\n",__LINE__,*pPanelType,*pDACIndex);

    return TRUE;
}

#if ENABLE_STB_ECOS_BOOT
static MS_BOOL getColorFormat(HDMITX_VIDEO_COLOR_FORMAT *pColorFormat)
{
    int u8ColorFormatEnv = -1;
    char * p_str = NULL;

    p_str = getenv ("colorformat");
    if(NULL != p_str)
    {
        u8ColorFormatEnv = (int)simple_strtol(p_str, NULL, 10);
    }
    else
    {
        //set env normal colorformat.
        u8ColorFormatEnv = HDMITX_VIDEO_COLOR_YUV444;
        setenv("colorformat", "2"); // 2, HDMITX_VIDEO_COLOR_YUV444
        saveenv();
    }
    //printf("getColorFormat,line:%d, u8ColorFormatEnv:%d\n",__LINE__,u8ColorFormatEnv);

    if((u8ColorFormatEnv < HDMITX_VIDEO_COLOR_RGB444) || (u8ColorFormatEnv > HDMITX_VIDEO_COLOR_YUV420))
    {
        printf("error, colorformat env is out of [0--3]!\n");
        return FALSE;
    }

    *pColorFormat = u8ColorFormatEnv;

    return TRUE;
}
#endif

MS_BOOL setHDMITxAnalogTuning(PANEL_RESOLUTION_TYPE enResolutionType)
{
    MS_BOOL bRet = FALSE;
    HDMITX_VIDEO_COLORDEPTH_VAL Val = 0;
    HDMITX_ANALOG_TUNING stHDMITxTun;
    memset(&stHDMITxTun, 0x00 , sizeof(HDMITX_ANALOG_TUNING));

    bRet = MApi_HDMITx_GetRxDCInfoFromEDID(&Val);
    if(FALSE == bRet)
    {
        printf("%s: Error: MApi_HDMITx_GetRxDCInfoFromEDID EDID is not ready, at %d\n", __func__, __LINE__);
        return FALSE;
    }

    //HD:1080P50/60£¬deep HD:1080P50/60&36 bit£¬others SD
/*    if(DACOUT_1080P_50 > enResolutionType)
    {
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[0]), sizeof(HDMITX_ANALOG_TUNING));
    }
    else */if(((DACOUT_1080P_50 == enResolutionType) || (DACOUT_1080P_60 == enResolutionType)) &&
        (HDMITX_VIDEO_CD_36Bits == Val))
    {
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[2]), sizeof(HDMITX_ANALOG_TUNING));
    }
    else if((DACOUT_1080P_50 == enResolutionType) || (DACOUT_1080P_60 == enResolutionType))
    {
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[1]), sizeof(HDMITX_ANALOG_TUNING));
    }
    else
    {
       // printf("%s: warning: setHDMITxAnalogTuning params are out of scope now, at %d\n", __func__, __LINE__);
        memcpy(&stHDMITxTun, (HDMITX_ANALOG_TUNING*)&(astHdmiAnalogCfg[0]), sizeof(HDMITX_ANALOG_TUNING));
    }

    MApi_HDMITx_AnalogTuning(&stHDMITxTun);
    return TRUE;
}

static MS_VE_VIDEOSYS getVEVideoSys(PANEL_RESOLUTION_TYPE enPanelType)
{
    switch (enPanelType)
    {
        case DACOUT_480I:
        case DACOUT_480P:
        case DACOUT_720P_60:
        case DACOUT_1080I_60:
        case DACOUT_1080P_60:
        case DACOUT_1080P_30:
        case DACOUT_1080P_24:
        case DACOUT_640x480_60:
        case DACOUT_1920x2205P_24:
        case DACOUT_1280x1470P_60:
        case DACOUT_3840x2160P_24:
        case DACOUT_3840x2160P_30:
        case DACOUT_3840x2160P_60:
        case DACOUT_4096x2160P_24:
        case DACOUT_4096x2160P_30:
        case DACOUT_4096x2160P_60:

            return MS_VE_NTSC;
        case DACOUT_576I:
        case DACOUT_576P:
        case DACOUT_720P_50:
        case DACOUT_1080I_50:
        case DACOUT_1080P_50:
        case DACOUT_1080P_25:
        case DACOUT_1280x1470P_50:
        case DACOUT_3840x2160P_50:
        case DACOUT_4096x2160P_25:
        case DACOUT_4096x2160P_50:
            return MS_VE_PAL;
        default:
            UBOOT_ERROR("ERROR: Not support current panel resolution!\n");
            while(1);
            break;
    }

    return MS_VE_VIDEOSYS_NUM;
}

static MS_U16 getFrameRates(PANEL_RESOLUTION_TYPE enPanelType)
{
    switch (enPanelType)
    {
        case DACOUT_480I:
        case DACOUT_480P:
        case DACOUT_720P_60:
        case DACOUT_1080I_60:
        case DACOUT_1080P_60:
        case DACOUT_640x480_60:
        case DACOUT_1920x2205P_24:
        case DACOUT_1280x1470P_60:
        case DACOUT_3840x2160P_24:
        case DACOUT_3840x2160P_30:
        case DACOUT_3840x2160P_60:
        case DACOUT_4096x2160P_24:
        case DACOUT_4096x2160P_30:
        case DACOUT_4096x2160P_60:
            return 6000 / 2;
        case DACOUT_576I:
        case DACOUT_576P:
        case DACOUT_720P_50:
        case DACOUT_1080I_50:
        case DACOUT_1080P_50:
        case DACOUT_1080P_25:
        case DACOUT_1280x1470P_50:
        case DACOUT_3840x2160P_50:
        case DACOUT_4096x2160P_25:
        case DACOUT_4096x2160P_50:
        default:
            break;
    }

    return 5000 / 2;
}

static HDMITX_VIDEO_TIMING getHDMITxVideoTiming(PANEL_RESOLUTION_TYPE enPanelType)
{
    switch (enPanelType)
    {
        case DACOUT_480I:
            return HDMITX_RES_720x480i;
        case DACOUT_480P:
            return HDMITX_RES_720x480p;
        case DACOUT_576I:
            return HDMITX_RES_720x576i;
        case DACOUT_576P:
            return HDMITX_RES_720x576p;
           case DACOUT_720P_50:
            return HDMITX_RES_1280x720p_50Hz;
        case DACOUT_720P_60:
            return HDMITX_RES_1280x720p_60Hz;
        case DACOUT_1080I_50:
            return HDMITX_RES_1920x1080i_50Hz;
        case DACOUT_1080I_60:
            return HDMITX_RES_1920x1080i_60Hz;
        case DACOUT_1080P_50:
            return HDMITX_RES_1920x1080p_50Hz;
        case DACOUT_1080P_60:
            return HDMITX_RES_1920x1080p_60Hz;
        case DACOUT_1080P_30:
            return HDMITX_RES_1920x1080p_30Hz;

        case DACOUT_1080P_25:
            return HDMITX_RES_1920x1080p_25Hz;

        case DACOUT_1080P_24:
            return HDMITX_RES_1920x1080p_24Hz;

        case DACOUT_1280x1470P_50:
            return HDMITX_RES_1280X1470p_50Hz;

        case DACOUT_1280x1470P_60:
            return HDMITX_RES_1280X1470p_60Hz;

        case DACOUT_3840x2160P_24:
            return HDMITX_RES_3840x2160p_24Hz;

        case DACOUT_3840x2160P_25:
            return HDMITX_RES_3840x2160p_25Hz;

        case DACOUT_3840x2160P_30:
            return HDMITX_RES_3840x2160p_30Hz;

        case DACOUT_3840x2160P_50:
            return HDMITX_RES_3840x2160p_50Hz;

        case DACOUT_3840x2160P_60:
            return HDMITX_RES_3840x2160p_60Hz;

        case DACOUT_4096x2160P_24:
            return HDMITX_RES_4096x2160p_24Hz;

        case DACOUT_4096x2160P_25:
            return HDMITX_RES_4096x2160p_25Hz;

        case DACOUT_4096x2160P_30:
            return HDMITX_RES_4096x2160p_30Hz;

        case DACOUT_4096x2160P_50:
            return HDMITX_RES_4096x2160p_50Hz;

        case DACOUT_4096x2160P_60:
            return HDMITX_RES_4096x2160p_60Hz;

        case DACOUT_1024x768P_60:
            return HDMITX_RES_1024x768p_60Hz;

        case DACOUT_1280x1024P_60:
            return HDMITX_RES_1280x1024p_60Hz;

        case DACOUT_1440x900P_60:
            return HDMITX_RES_1440x900p_60Hz;

        case DACOUT_1600x1200P_60:
            return HDMITX_RES_1600x1200p_60Hz;

        default:
            break;
    }

    return HDMITX_RES_1280x720p_50Hz;
}

static MS_BOOL IsReciverSupportYPbPr(void)
{
    #define BLOCK_INDEX1    1

    MS_U8 u8BlockData[128] = {0};

    MApi_HDMITx_GetEDIDData(u8BlockData, BLOCK_INDEX1);

    if ( ( u8BlockData[0x03]& 0x30) == 0x00)
    {
        UBOOT_TRACE("Rx Not Support YCbCr\n");
        return FALSE;
    }
    else
    {
        UBOOT_TRACE("Rx Support YUV444 or YUV422 \n");
        return TRUE;
    }
}




#if (defined(CONFIG_KANO) || defined(CONFIG_CURRY)|| defined(CONFIG_K6)|| defined(CONFIG_K6LITE))

void _XC_InputSource_InputPort_Mapping(XC_EX_INPUT_SOURCE_TYPE src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count)
{
    switch (src_ids)
    {
        default:
        case E_XC_EX_INPUT_SOURCE_SCALER_OP:
            *u8port_count = 1;
            port_ids[0] = INPUT_PORT_SCALER_OP;
            break;
    }
}

PANEL_RESOLUTION_TYPE InitializeEx(E_DAC_PANEL_INDEX enDisplayRes)
{

    PANEL_RESOLUTION_TYPE enPanelTimingEx = PNL_RES_MAX_NUM;

    switch(enDisplayRes)
    {
        case DAC_PANEL_480I_60:
        case DAC_PANEL_480P_60:
        case DAC_PANEL_720P_60:
        case DAC_PANEL_1080I_60:
        case DAC_PANEL_1080P_60:
        case DAC_PANEL_1080P_30:
        case DAC_PANEL_640x480_60:
        case DAC_PANEL_1280x1470P_60:
        case DAC_PANEL_3840x2160P_30:
        case DAC_PANEL_3840x2160P_60:
        case DAC_PANEL_4096x2160P_30:
        case DAC_PANEL_4096x2160P_60:
        case DAC_PANEL_1080P_24:
        case DAC_PANEL_1920x2205P_24:
        case DAC_PANEL_3840x2160P_24:
        case DAC_PANEL_4096x2160P_24:
        case DAC_PANEL_3840x2160P420_30:
        case DAC_PANEL_4096x2160P420_30:
        case DAC_PANEL_3840x2160P420_60:
        case DAC_PANEL_4096x2160P420_60:
            printf("DACOUT_480I  ----\n");
            enPanelTimingEx = DACOUT_480I;
            break;

        case DAC_PANEL_576I_50:
        case DAC_PANEL_576P_50:
        case DAC_PANEL_720P_50:
        case DAC_PANEL_1080I_50:
        case DAC_PANEL_1080P_50:
        case DAC_PANEL_1080P_25:
        case DAC_PANEL_1280x1470P_50:
        case DAC_PANEL_3840x2160P_25:
        case DAC_PANEL_3840x2160P_50:
        case DAC_PANEL_4096x2160P_25:
        case DAC_PANEL_4096x2160P_50:
        case DAC_PANEL_3840x2160P420_50:
        case DAC_PANEL_4096x2160P420_50:
            printf("DACOUT_576I  ----\n");
            enPanelTimingEx = DACOUT_576I;
            break;

        default:
            printf("default DACOUT_576I  ----\n");
            enPanelTimingEx = DACOUT_576I;
            break;

    }
    return enPanelTimingEx;
}
#endif

MS_BOOL setHdcpKey(HDMITX_OUTPUT_MODE *enHdmiTxMode)
{
    #define HDCP_KEY_PATITION    "tvcustomer"
    #define HDCP_KEY_FILE        "/Customer/hdcpkey.bin"
    U32 HdcpKey_size = 0;
    MS_U8* HdcpKey_Buffer = NULL;

    //check hdcp key
    char * p_str = getenv("hdcp_key");
    if((NULL != p_str) && (0 != simple_strtol(p_str, NULL, 10))) //hdcp key enabled
    {
        if(vfs_mount(HDCP_KEY_PATITION)!=0)
        {
            UBOOT_ERROR("mount %s failed\n", HDCP_KEY_PATITION);
            return FALSE;
        }

        HdcpKey_size=vfs_getsize(HDCP_KEY_FILE);
        if(HdcpKey_size==0)
        {
            UBOOT_ERROR("get the file size of %s fail\n", HDCP_KEY_FILE);
            return FALSE;
        }

        HdcpKey_Buffer=(MS_U8*)malloc(HdcpKey_size);
        UBOOT_DEBUG("HdcpKey_Buffer at 0x%x\n",(unsigned int)HdcpKey_Buffer);
        if(HdcpKey_Buffer==NULL)
        {
            UBOOT_ERROR("malloc for HdcpKey_Buffer fail\n");
            return FALSE;
        }

        if(vfs_read((char *)HdcpKey_Buffer, HDCP_KEY_FILE, 0, HdcpKey_size)!=0)
        {
            free(HdcpKey_Buffer);
            UBOOT_ERROR("read %s fail\n",HDCP_KEY_FILE);
            return FALSE;
        }

        MApi_HDMITx_GetHdcpKey(FALSE, HdcpKey_Buffer);  //set hdcp key
        *enHdmiTxMode = HDMITX_HDMI_HDCP;

        vfs_umount();
        free(HdcpKey_Buffer);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL IsValidOutput(int index, MS_U8* pData, MS_U8* p4kData,  MS_U8* p4k420Data, MS_U32 status, MS_BOOL *bSupport, MS_BOOL *bBestSupport)
{
    *bSupport = FALSE;
    *bBestSupport = FALSE;
    MS_U8 u8Target_16x9 = 0, u8Target_4x3 = 0;
    MS_BOOL bRet = FALSE;
    int idx = 0;

    //VSD Case
    if(status & HDMITX_EDID_VALID_BIT)
    {
        //search video tag of 4k2k edid
        switch(staMapTypeIndex[index].enResolutionType)
        {
            case DACOUT_480P:
                u8Target_16x9 = HDMITX_VIC_720x480p_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480p_60_4_3;
                break;
            case DACOUT_576P:
                u8Target_16x9 = HDMITX_VIC_720x576p_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576p_50_4_3;
                break;
            case DACOUT_720P_50:
                u8Target_16x9 = HDMITX_VIC_1280x720p_50_16_9;
                break;
            case DACOUT_720P_60:
                u8Target_16x9 = HDMITX_VIC_1280x720p_60_16_9;
                break;
            case DACOUT_1080P_50:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_50_16_9;
                break;
            case DACOUT_1080P_60:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_60_16_9;
                break;
            case DACOUT_480I:
                u8Target_16x9 = HDMITX_VIC_720x480i_60_16_9;
                u8Target_4x3 = HDMITX_VIC_720x480i_60_4_3;
                break;
            case DACOUT_576I:
                u8Target_16x9 = HDMITX_VIC_720x576i_50_16_9;
                u8Target_4x3 = HDMITX_VIC_720x576i_50_4_3;
                break;
            case DACOUT_1080I_50:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_50_16_9;
                break;
            case DACOUT_1080I_60:
                u8Target_16x9 = HDMITX_VIC_1920x1080i_60_16_9;
                break;
            case DACOUT_1080P_30:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_30_16_9;
                break;
            case DACOUT_1080P_25:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_25_16_9;
                break;
            case DACOUT_1080P_24:
                u8Target_16x9 = HDMITX_VIC_1920x1080p_24_16_9;
                break;
            case DACOUT_3840x2160P_24:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case DACOUT_3840x2160P_25:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case DACOUT_3840x2160P_30:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case DACOUT_3840x2160P_50:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case DACOUT_3840x2160P_60:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
            case DACOUT_4096x2160P_24:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_24_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_24_256_135;
                break;
            case DACOUT_4096x2160P_25:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_25_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_25_256_135;
                break;
            case DACOUT_4096x2160P_30:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_30_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_30_256_135;
                break;
            case DACOUT_4096x2160P_50:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_50_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_50_256_135;
                break;
            case DACOUT_4096x2160P_60:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_60_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_60_256_135;
            default:
                break;
        }

        for(idx = 0; idx < EDID_VIC_NUM; idx++)
        {
            if(pData[idx] == 0)
            {
                break;
            }
            else if(((pData[idx]&0x7F) == u8Target_16x9) || ((pData[idx]&0x7F) == u8Target_4x3))
            {
                *bSupport = TRUE;
                //is the native timing flag true?
                if(pData[idx] & 0x80)
                {
                    *bBestSupport = TRUE;
                }
            }
        }
        bRet = TRUE;
    }

    //4K2K VIC VSDB Case
    u8Target_4x3 = 0;
    if(status & HDMITX_EDID_4K2K_VALID_BIT)
    {
        //search video tag of 4k2k edid
        switch(staMapTypeIndex[index].enResolutionType)
        {
            case DACOUT_3840x2160P_30:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_30Hz;
                break;
            case DACOUT_3840x2160P_25:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_25Hz;
                break;
            case DACOUT_3840x2160P_24:
                u8Target_4x3 = HDMITX_VIDEO_4k2k_24Hz;
                break;
            default:
                break;
        }

        for(idx = 0; idx < EDID_VIC_4K2K_NUM; idx++)
        {
            if(p4kData[idx] == 0)
            {
                break;
            }
            else if(p4kData[idx] == u8Target_4x3)
            {
                *bSupport = TRUE;
            }
        }
        bRet = TRUE;
    }

    //YPbPr420 Case
    u8Target_16x9 = 0;
    u8Target_4x3 = 0;
    if(status & HDMITX_EDID_4K2K420_VALID_BIT)
    {
        //search video tag of edid
        switch(staMapTypeIndex[index].enResolutionType)
        {
            case DACOUT_3840x2160P_30:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_30_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_30_64_27;
                break;
            case DACOUT_3840x2160P_25:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_25_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_25_64_27;
                break;
            case DACOUT_3840x2160P_24:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_24_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_24_64_27;
                break;
            case DACOUT_3840x2160P_50:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_50_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_50_64_27;
                break;
            case DACOUT_3840x2160P_60:
                u8Target_16x9 = HDMITX_VIC_3840x2160p_60_16_9;
                u8Target_4x3 =  HDMITX_VIC_3840x2160p_60_64_27;
                break;
            case DACOUT_4096x2160P_24:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_24_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_24_256_135;
                break;
            case DACOUT_4096x2160P_25:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_25_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_25_256_135;
                break;
            case DACOUT_4096x2160P_30:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_30_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_30_256_135;
                break;
            case DACOUT_4096x2160P_50:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_50_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_50_256_135;
                break;
            case DACOUT_4096x2160P_60:
                u8Target_16x9 = HDMITX_VIC_4096x2160p_60_256_135;
                u8Target_4x3 =  HDMITX_VIC_4096x2160p_60_256_135;
                break;
            default:
                break;
        }

        for(idx = 0; idx < EDID_VIC_4K2K420_NUM; idx++)
        {
            if(p4k420Data[idx] == 0)
            {
                break;
            }
            else if(p4k420Data[idx] == u8Target_16x9 || p4k420Data[idx] == u8Target_4x3)
            {
                *bSupport = TRUE;
            }
        }
        bRet = TRUE;
    }

    return bRet;
}

static MS_BOOL GetDeviceInfo(MsHDMITX_DEVICEINFO *pstDeviceInfo)
{

    MS_BOOL bSupport = FALSE;
    MS_BOOL bBestSupport = FALSE;
    MS_U8 i=0;
    MS_U8 u8HDMITx[EDID_VIC_NUM];
    MS_U8 u8HDMITx4K2K[EDID_VIC_4K2K_NUM];
    MS_U8 u8HDMITx4K2K420[EDID_VIC_4K2K420_NUM];
    MS_U32 edidStatus = HDMITX_EDID_NON_STATUS;
    UBOOT_TRACE("IN\n");

    if(pstDeviceInfo ==  NULL)
    {
        UBOOT_ERROR("ERROR: *pstDeviceInfo is NULL!\n");
        return FALSE;
    }

    //step 1. check hdmi whether connected
    pstDeviceInfo->bIsConnected = MApi_HDMITx_GetRxStatus();
    if(!pstDeviceInfo->bIsConnected)
    {
        UBOOT_DEBUG("Hdmi is not connected!\n");
        return FALSE;
    }

    //step 2. get edid timing and 4k2k edid timing list
    //VSD Case
    memset(u8HDMITx, 0, EDID_VIC_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetRxVideoFormatFromEDID(u8HDMITx, EDID_VIC_NUM))
    {
        edidStatus |= HDMITX_EDID_VALID_BIT;
    }
#if(ENABLE_MSTAR_CLIPPERS == 1 || ENABLE_MSTAR_KANO == 1 || ENABLE_MSTAR_CURRY == 1 || ENABLE_MSTAR_K6 == 1 || ENABLE_MSTAR_K6LITE == 1)
    //VSDB Case
    memset(u8HDMITx4K2K, 0, EDID_VIC_4K2K_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetVICListFromEDID(u8HDMITx4K2K, EDID_VIC_4K2K_NUM))
    {
        edidStatus |= HDMITX_EDID_4K2K_VALID_BIT;
    }

   //YPbPr420 Case
    MS_U32 u32RealLen = 0;
    memset(u8HDMITx4K2K420, 0, EDID_VIC_4K2K420_NUM * sizeof(MS_U8));
    if(TRUE == MApi_HDMITx_GetEdidDataBlocks(E_CEA_TAG_CODE_EXT_TAG, E_CEA_EXT_TAG_420VDB, u8HDMITx4K2K420, EDID_VIC_4K2K420_NUM, &u32RealLen))
    {
        edidStatus |= HDMITX_EDID_4K2K420_VALID_BIT;
    }
#endif

    if(HDMITX_EDID_NON_STATUS == edidStatus)
    {
        UBOOT_DEBUG("Get edid or 4K edid or 4K420 fail\n");
        return FALSE;
    }

    //step 3. get support Timing and bestTiming info for device
    memset((void *)(pstDeviceInfo->bSupportedTiming), 0, DISPOUT_TIMING_MAX);
    memset((void *)(pstDeviceInfo->bBestSupportedTiming), 0, DISPOUT_TIMING_MAX);
    for(i=0; i<DISPOUT_TIMING_MAX; i++)
    {
        if(IsValidOutput(i, u8HDMITx, u8HDMITx4K2K, u8HDMITx4K2K420, edidStatus, &bSupport, &bBestSupport) == TRUE)
        {
            pstDeviceInfo->bSupportedTiming[i] = bSupport;
            pstDeviceInfo->bBestSupportedTiming[i] = bBestSupport;
        }
    }

    //step 4. reserved
    //get support audio format info for device func
    //get hdcp status info for device func
    //get support 3D format info for device func
    UBOOT_TRACE("OK\n");
    return TRUE;
}

static MS_U8 CheckSupportTiming(MS_U8 current_timing)
{
    MsHDMITX_DEVICEINFO stDeviceInfo;
    MS_U8 index = 0;
    MS_U8 u8BestTimingCnt = 0;
    MS_U8 u8SupportTimingCnt = 0;
    MS_U8 u8BestTimingList[DISPOUT_TIMING_MAX]={0};
    MS_U8 u8SupportTimingList[DISPOUT_TIMING_MAX]={0};
    MS_U8 prop_timing = 0;
    UBOOT_TRACE("IN\n");

    if(GetDeviceInfo((MsHDMITX_DEVICEINFO *)&stDeviceInfo) == TRUE)
    {
        //no-auto_timing and current timing is support
        if((getenv("auto_timing") == NULL) && (stDeviceInfo.bSupportedTiming[current_timing] == TRUE))
        {
                UBOOT_TRACE("OK\n");
                return current_timing;
        }

        //restore best timing and highest timing
        for(index=0; index<DISPOUT_TIMING_MAX; index++)
        {
            if(stDeviceInfo.bBestSupportedTiming[index] == TRUE)
            {
                UBOOT_DEBUG("--Support Native Timing is: %s\n", DacPropPanelIndexTbl[index]);
                u8BestTimingList[u8BestTimingCnt++] = index;
            }
            else if(stDeviceInfo.bSupportedTiming[index] == TRUE)
            {
                UBOOT_DEBUG("--Support Timing is: %s\n", DacPropPanelIndexTbl[index]);
                u8SupportTimingList[u8SupportTimingCnt++] = index;
            }
        }

        UBOOT_DEBUG("HDMI best support timing is %d\n", u8BestTimingList[u8BestTimingCnt-1]);
        UBOOT_DEBUG("HDMI highest support timing is %d\n", u8SupportTimingList[u8SupportTimingCnt-1]);

        if(getenv("auto_timing") != NULL) //auto timing case
        {
            if(u8BestTimingCnt != 0)        //use best timing
            {
                prop_timing = u8BestTimingList[u8BestTimingCnt-1];
            }
            else        //use default timing
            {
                UBOOT_DEBUG("auto default timing is %d\n", DACOUT720P50);
                prop_timing = DACOUT720P50;
            }
        }
        else if(u8SupportTimingCnt != 0)  //no-auto timing use highest timing
        {
            while(u8SupportTimingCnt)
            {
                char cmd_buf[CMD_BUF] = "\0";
                U32 filesize = 0;
                if(vfs_mount(CONFIG)!= 0)
                {
                    UBOOT_ERROR("vfs_mount fail\n");
                    return -1;
                }
                snprintf(cmd_buf, sizeof(cmd_buf),"/config/panel/%s.ini", DacPropPanelIndexTbl[u8SupportTimingList[u8SupportTimingCnt-1]]);
                filesize = vfs_getsize(cmd_buf);
                if( (filesize > 0x10000) || (filesize == 0) )
                {
                    UBOOT_DEBUG("%s does not exist, move to next resolution\n", cmd_buf);
                    --u8SupportTimingCnt;
                }
                else
                {
                    break;
                }
            }

            if((u8SupportTimingList[u8SupportTimingCnt-1] <= 16) && (stDeviceInfo.bSupportedTiming[DACOUT1080P50] == TRUE))
            {
                prop_timing = DACOUT1080P50;
            }
            else if((u8SupportTimingList[u8SupportTimingCnt-1] <= 16) && (stDeviceInfo.bSupportedTiming[DACOUT720P50] == TRUE))
            {
                prop_timing = DACOUT720P50;
            }
            else
            {
                prop_timing = u8SupportTimingList[u8SupportTimingCnt-1];
            }
        }
    }
    else
    {
        if(getenv("auto_timing") != NULL) //auto timing use default timing
        {
            UBOOT_DEBUG("auto default timing is %d\n", DACOUT720P50);
            prop_timing = DACOUT720P50;
        }
        else if((current_timing >= DACOUT3840P24) && MApi_HDMITx_GetRxStatus()) //no-auto 4k timing and edid is not ready change to 1080P50
        {
            UBOOT_DEBUG("4k timing is  not support, change to 1080P50\n");
            prop_timing = DACOUT1080P50;
        }
    }

    UBOOT_TRACE("OK\n");
    return prop_timing;
}

#ifdef CONFIG_HDMITX_YUV422_10BIT
// computing method of YUV444: Htotal x Vtotal x frame rate x (color depth / 8)
// eg: 4K60    YUV444 8bit:  4400x2250x60x(8/8) = 594MHz

// computing method of YUV422(color depth always be 8): Htotal x Vtotal x frame rate x (8 / 8)
// eg: 4K30    YUV422 10bit: 4400x2250x30x(8/8) = 297MHz
// eg: 1080P60 YUV422 10bit: 2200x1125x60x(8/8) = 148.5MHz
// eg: 720P60  YUV422 10bit: 1650x750x60x(8/8)  = 74.25MHz

// computing method of YUV420(half of YUV444): Htotal x Vtotal x frame rate x (color depth / 8) / 2
// eg: 4K60    YUV420 10bit: 4400x2250x60x(10/8)/2 = 371.25MHz

static HDMITX_VIDEO_COLORDEPTH_VAL _CheckOutputColorDepthForSpecificColorFormat(HDMITX_VIDEO_COLOR_FORMAT eInColor, HDMITX_VIDEO_COLOR_FORMAT eOutColor, HDMITX_VIDEO_TIMING enCurTiming)
{
    #define  NEED_READ_VSDB_DATE_LEN   7
    #define  MAX_TMDS_CLK_HSVSDB_IDX   4
    #define  MAX_TMDS_CLK_VSDB_IDX     6
    #define  MAX_TMDS_CLK_FACTOR       5
    #define  MAX_TMDS_CLK_HDMI14       340
    #define  MAX_TMDS_CLK_4K2K30       297
    #define  MAX_TMDS_CLK_1080P        148
    #define  MAX_TMDS_CLK_720P         74

    HDMITX_VIDEO_COLORDEPTH_VAL eColorDepth = HDMITX_VIDEO_CD_24Bits; // default value
    MS_U8 u8Data[NEED_READ_VSDB_DATE_LEN] = {0};
    MS_U32 u32RealLen = 0, u32MaxTmdsClk = 0;

    MApi_HDMITx_GetEdidDataBlocks(E_CEA_TAG_CODE_HFVSDB, E_CEA_EXT_TAG_VCDB, u8Data, NEED_READ_VSDB_DATE_LEN, &u32RealLen);
    u32MaxTmdsClk = u8Data[MAX_TMDS_CLK_HSVSDB_IDX]*MAX_TMDS_CLK_FACTOR;
    UBOOT_DEBUG("HSVSDB enCurTiming = %d, u32MaxTmdsClk = %dMHz\n", enCurTiming, u32MaxTmdsClk);

    if(u32MaxTmdsClk == 0)
    {
        MApi_HDMITx_GetEdidDataBlocks(E_CEA_TAG_CODE_VSDB, E_CEA_EXT_TAG_VCDB, u8Data, NEED_READ_VSDB_DATE_LEN, &u32RealLen);
        u32MaxTmdsClk = u8Data[MAX_TMDS_CLK_VSDB_IDX]*MAX_TMDS_CLK_FACTOR;
        UBOOT_DEBUG("VSDB enCurTiming = %d, u32MaxTmdsClk = %dMHz\n", enCurTiming, u32MaxTmdsClk);
    }

    if(eInColor == HDMITX_VIDEO_COLOR_YUV444 && eOutColor == HDMITX_VIDEO_COLOR_YUV422) // other timings except for 4K50 & 4K60
    {
        if(enCurTiming >= HDMITX_RES_3840x2160p_24Hz && enCurTiming <= HDMITX_RES_3840x2160p_30Hz)
        {
            if(u32MaxTmdsClk > MAX_TMDS_CLK_4K2K30)
            {
                // when YUV422 10bit out, the color depth should be HDMITX_VIDEO_CD_NoID
                eColorDepth = HDMITX_VIDEO_CD_NoID;
            }
        }
        else if(enCurTiming >= HDMITX_RES_1920x1080i_50Hz && enCurTiming <= HDMITX_RES_1920x1080p_60Hz)
        {
            if(u32MaxTmdsClk > MAX_TMDS_CLK_1080P)
            {
                // when YUV422 10bit out, the color depth should be HDMITX_VIDEO_CD_NoID
                eColorDepth = HDMITX_VIDEO_CD_NoID;
            }
        }
        else if(enCurTiming >= HDMITX_RES_640x480p && enCurTiming <= HDMITX_RES_1280x720p_60Hz)
        {
            if(u32MaxTmdsClk > MAX_TMDS_CLK_720P)
            {
                // when YUV422 10bit out, the color depth should be HDMITX_VIDEO_CD_NoID
                eColorDepth = HDMITX_VIDEO_CD_NoID;
            }
        }
    }
    else if(eInColor == HDMITX_VIDEO_COLOR_YUV420 && eOutColor == HDMITX_VIDEO_COLOR_YUV420) // for 4K50 & 4K60
    {
        if(u32MaxTmdsClk > MAX_TMDS_CLK_HDMI14)
        {
            // for 4K50/60, when YUV420 10bit out, the color depth should be HDMITX_VIDEO_CD_30Bits
            eColorDepth = HDMITX_VIDEO_CD_30Bits;
        }
    }
    else if(eInColor == HDMITX_VIDEO_COLOR_YUV444 && eOutColor == HDMITX_VIDEO_COLOR_RGB444) // for DVI mode
    {
        eColorDepth = HDMITX_VIDEO_CD_24Bits;
    }
    else if(eInColor == HDMITX_VIDEO_COLOR_YUV444 && eOutColor == HDMITX_VIDEO_COLOR_YUV444) // default
    {
        eColorDepth = HDMITX_VIDEO_CD_24Bits;
    }

    return eColorDepth;
}
#endif

int msHdmitx_Disp_Init(void)
{
    MS_BOOL _bModeNTSC = FALSE;
    MS_BOOL bRet = FALSE;
    PANEL_RESOLUTION_TYPE enPanelType = PANEL_RES_MIN;
    E_DAC_PANEL_INDEX enDACIndex      = DAC_PANEL_480I_60;
    E_DAC_PANEL_INDEX enXc1DACIndex      = DAC_PANEL_480I_60;
    MS_VE_VIDEOSYS eVEStd = MS_VE_PAL;
    MS_VE_WINDOW_TYPE stVEWin = {0, 0, 720, 576};
    XC_INITDATA sXC_InitData;

    //auto-support timing variable
    int resolution_index = DACOUT1080P50;
    int prop_index = DACOUT1080P50;
    char *p_str;
    char cmd_buf[CMD_BUF] = "\0";

    UBOOT_TRACE("IN\n");
    printf("msHdmitx_Disp_Init\n");
    //printf("[%s][%d] Function entry, system time = %lu \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());


    bRet = getResolutionTypeAndDACIndex((MS_U16 *)&enPanelType, (MS_U16 *)&enDACIndex);
    //printf("msHdmitx_Init,line:%d£¬bRet:%d, enPanelType:%d,enDACIndex:%d \n",__LINE__,bRet,enPanelType,enDACIndex);
    if(FALSE == bRet)
    {
        UBOOT_ERROR("%s: Error: getResolutionTypeAndDACIndex FAILE! use 1080p default, at %d\n", __func__, __LINE__);
        enPanelType = DACOUT_1080P_60;
        enDACIndex  = DAC_PANEL_1080P_60;
    }
    UBOOT_DEBUG("[%s %d] (%u, %u)\n",__FUNCTION__,__LINE__,enPanelType,enDACIndex);

    //2. Initialization - DAC and HDMI initialization
    memset(&sXC_InitData, 0, sizeof(XC_INITDATA));
    MApi_XC_Init(&sXC_InitData, sizeof(XC_INITDATA));
    MDrv_SYS_Init();
    //case E_VOP_SEL_OSD_BLEND2:
    //    SC_W2BYTEMSK(REG_SC_BK2F_28_L, 0x0000, 0x0001);
    //    SC_W2BYTEMSK(REG_SC_BK0F_46_L, 0x0020, 0x00F0);
    //    SC_W2BYTEMSK(REG_SC_BK0F_57_L, 0x0800, 0x0800);
//MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
#if (ENABLE_MSTAR_KANO == 0 && ENABLE_MSTAR_CURRY ==0 && ENABLE_MSTAR_K6 ==0 && ENABLE_MSTAR_K6LITE ==0)
    #if defined(__ARM__)
        *(volatile unsigned int*)0x1F205E00 = 0x000F;
        *(volatile unsigned int*)0x1F205F18 = 0x0020;
    #else
        *(volatile unsigned int*)0xBF205E00 = 0x000F;
        *(volatile unsigned int*)0xBF205F18 = 0x0020;
    #endif
#else
    MApi_XC_SetOSD2VEMode(E_VOP_SEL_OSD_LAST);
#endif
    //2. Initialization - DAC
    //UBOOT_DEBUG(printf("verDispTx_Main - step1\n"));
    MApi_DAC_Init();
    MApi_DAC_SetClkInv(TRUE, TRUE);

    MApi_HDMITx_SetDbgLevel(0);
    MApi_HDMITx_SetHPDGpioPin(HDMITX_HPD_PM_GPIO);
    MApi_HDMITx_Init();

    char *disable_cvbs = getenv(DONT_EN_CVBS_ENV);
    if ((disable_cvbs != NULL) && (strcmp(disable_cvbs, "1") == 0) && (MApi_HDMITx_GetRxStatus() == TRUE))
    {
        MApi_DAC_Enable(FALSE, TRUE);
        MApi_DAC_Enable(FALSE, FALSE);
        UBOOT_DEBUG("Hdmi is connected!\n");
        UBOOT_DEBUG("%s = 1 , Dont enable CVBS\n", DONT_EN_CVBS_ENV);
    }
    else
    {
        MApi_DAC_Enable(TRUE, TRUE);
        MApi_DAC_Enable(TRUE, FALSE);
    }
    //shoulde be call after MApi_DAC_Enable
    MApi_DAC_SetOutputSwapSel(DAC_R_G_B, TRUE);// for K6Lite VGA out

    // add here for hdmitx ssc enable
#ifdef CONFIG_HDMITX_SSC
    HDMITX_SSCENABLE stSSCEn = {0, 1, 0};
    MApi_HDMITx_GeneralCtrl(HDMITX_CMD_SSC_ENABLE, (void*)&stSSCEn, sizeof(stSSCEn));
#endif

    MApi_HDMITx_TurnOnOff(TRUE);
    MApi_HDMITx_SetVideoOnOff(TRUE);
    MS_U32 u32StartTime = 0;
    MS_BOOL bHDMISupport = FALSE;
    HDMITX_VIDEO_COLOR_FORMAT eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
    HDMITX_VIDEO_COLOR_FORMAT eInColorFmt  = HDMITX_VIDEO_COLOR_YUV444;
    HDMITX_OUTPUT_MODE    eOutMode = HDMITX_HDMI;
    HDMITX_VIDEO_COLORDEPTH_VAL eEDIDColorDepth = HDMITX_VIDEO_CD_NoID;

    MS_U32 u32EdidCheckTime=0;
    while((MApi_HDMITx_EdidChecking() != TRUE) && ((u32EdidCheckTime) < 6))
    {
        UBOOT_ERROR("CHeck EDID fail %d times!\n", ++u32EdidCheckTime);
    }

    u32StartTime = MsOS_GetSystemTime();
    while((MApi_HDMITx_EDID_HDMISupport(&bHDMISupport) != TRUE) && (MsOS_Timer_DiffTimeFromNow(u32StartTime) < 10))
    {
        UBOOT_ERROR("EDID NOT READY! \n");
    }

    p_str = getenv ("resolution");
    if(p_str != NULL)
    {
        resolution_index = (int)simple_strtol(p_str, NULL, 10);
    }

    #if 0  //do not check support timing for NVR, cause edid don't have 1280*1024 1600*1200 1440*900 1024*768 timing.
    prop_index = CheckSupportTiming(resolution_index);
    if((prop_index != resolution_index) && (prop_index != 0))
    {
        resolution_index = prop_index;
        printf("select proper resolution=%d from EDID\n", resolution_index);
        printf("select proper panel_name: %s.ini\n", DacPropPanelIndexTbl[resolution_index]);
        snprintf(cmd_buf, sizeof(cmd_buf),"%d", resolution_index);
        setenv("resolution", cmd_buf);
        setenv("resolution_reset", cmd_buf);

        MApi_SetEnv2BootArgs("resolution=", DacPropPanelIndexTbl[resolution_index]);
        snprintf(cmd_buf, sizeof(cmd_buf),"/config/panel/%s.ini", DacPropPanelIndexTbl[resolution_index]);
        setenv("panel_path", cmd_buf);
        setenv("panel_name", DacPropPanelIndexTbl[resolution_index]);

        setenv("db_table", "0");
        saveenv();
        run_command("reset", 0);
    }
    #endif

    MApi_HDMITx_GetRxDCInfoFromEDID(&eEDIDColorDepth);
    if(eEDIDColorDepth <= HDMITX_VIDEO_CD_24Bits)
    {
        //To pass simplay lab HDMI test, Item 7-19
        eEDIDColorDepth = HDMITX_VIDEO_CD_NoID;
    }
    else
    {
        //Only Support HDMI 8 bit mode in Mboot
        eEDIDColorDepth = HDMITX_VIDEO_CD_24Bits;
    }
    if(bHDMISupport)
    {
#if ((ENABLE_MSTAR_K6 == 1) || (ENABLE_MSTAR_KANO == 1) || (ENABLE_MSTAR_CURRY == 1) ||(ENABLE_MSTAR_K6 == 1)|| (ENABLE_MSTAR_K6LITE == 1))
        if(enPanelType == DACOUT_3840x2160P_60)
        {
        #if (ENABLE_HDMITX_FORCE_SET_4K60_420 == 0)
            HDMITX_EDID_COLOR_FORMAT ColorFmt = HDMITX_EDID_Color_YCbCr_444;
            MApi_HDMITx_GetColorFormatFromEDID(HDMITX_RES_3840x2160p_60Hz, &ColorFmt);
            if(ColorFmt != HDMITX_EDID_Color_YCbCr_420)
            {
                // support 4k2k@60Hz YUV444
                eInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            }
            else
            {
                // only support 4k2k@60Hz YUV420
                eInColorFmt = HDMITX_VIDEO_COLOR_YUV420;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
                enDACIndex = DAC_PANEL_3840x2160P420_60;
            }
        #else
            eInColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            enDACIndex = DAC_PANEL_3840x2160P420_60;
            UBOOT_DEBUG("\033[41;33mForce set YUV420 in 4K60 for reducing power dissipation, mantis: 1200391\033[0m\n");
        #endif
        }
        else if(enPanelType == DACOUT_3840x2160P_50)
        {
        #if (ENABLE_HDMITX_FORCE_SET_4K60_420 == 0)
            HDMITX_EDID_COLOR_FORMAT ColorFmt = HDMITX_EDID_Color_YCbCr_444;
            MApi_HDMITx_GetColorFormatFromEDID(HDMITX_RES_3840x2160p_50Hz, &ColorFmt);
            if(ColorFmt != HDMITX_EDID_Color_YCbCr_420)
            {
                // support 4k2k@50Hz YUV444
                eInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            }
            else
            {
                // only support 4k2k@50Hz YUV420
                eInColorFmt = HDMITX_VIDEO_COLOR_YUV420;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
                enDACIndex = DAC_PANEL_3840x2160P420_50;
            }
        #else
            eInColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
            enDACIndex = DAC_PANEL_3840x2160P420_50;
            UBOOT_DEBUG("\033[41;33mForce set YUV420 in 4K50 for reducing power dissipation\033[0m\n");
        #endif
        }
        else if(enPanelType == DACOUT_3840x2160P_30)
        {
            HDMITX_EDID_COLOR_FORMAT ColorFmt = HDMITX_EDID_Color_YCbCr_444;
            MApi_HDMITx_GetColorFormatFromEDID(HDMITX_RES_3840x2160p_30Hz, &ColorFmt);
            if(ColorFmt != HDMITX_EDID_Color_YCbCr_420)
            {
                // support 4k2k@30Hz YUV444
                eInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            #ifdef CONFIG_HDMITX_YUV422_10BIT
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV422;
            #else
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            #endif
            }
            else
            {
                // only support 4k2k@30Hz YUV420
                eInColorFmt = HDMITX_VIDEO_COLOR_YUV420;
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV420;
                enDACIndex = DAC_PANEL_3840x2160P420_30;
            }
        }
        else if(IsReciverSupportYPbPr() == TRUE)
#else
        if(IsReciverSupportYPbPr() == TRUE)
#endif
        {
            eInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
        #ifdef CONFIG_HDMITX_YUV422_10BIT
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV422;
        #else
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
        #endif
#if ENABLE_STB_ECOS_BOOT
            //get env color format
            if(getColorFormat(&eOutColorFmt) == FALSE)
            {
                UBOOT_ERROR("%s: Error: getColorFormat FAILE! use YUV444 default, at %d\n", __func__, __LINE__);
                eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
            }
#endif
        }
        else
        {
            eInColorFmt = HDMITX_VIDEO_COLOR_RGB444;
            eOutColorFmt = HDMITX_VIDEO_COLOR_RGB444;
        }

        eOutMode = HDMITX_HDMI;
        printf("Rx Support HDMI mode! \n");
    }
    else
    {
        //force HDMI mode when can't get edid
        eOutMode = HDMITX_HDMI;
        eInColorFmt = HDMITX_VIDEO_COLOR_YUV444;
        #ifdef CONFIG_HDMITX_YUV422_10BIT
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV422;
        #else
            eOutColorFmt = HDMITX_VIDEO_COLOR_YUV444;
        #endif

        printf("Rx Support DVI mode only,so force to HDMI mode! \n");
    }

#ifdef CONFIG_HDMITX_YUV422_10BIT
    eEDIDColorDepth = _CheckOutputColorDepthForSpecificColorFormat(eInColorFmt, eOutColorFmt, getHDMITxVideoTiming(enPanelType));
    UBOOT_DEBUG("eEDIDColorDepth = %d\n", eEDIDColorDepth);
#endif

    MApi_HDMITx_SetHDMITxMode_CD(eOutMode, eEDIDColorDepth);
    if( setHdcpKey(&eOutMode) == TRUE)
    {
        printf("HDMITxMode = %d\n", eOutMode);
        MApi_HDMITx_SetHDMITxMode(eOutMode);
    }
    UBOOT_DEBUG("eInColorFmt = %d, eOutColorFmt = %d\n", eInColorFmt, eOutColorFmt);

    MApi_HDMITx_SetColorFormat(eInColorFmt, eOutColorFmt);
#if ENABLE_STB_ECOS_BOOT
    //save coloformat env
    snprintf(cmd_buf, sizeof(cmd_buf),"%d", eOutColorFmt);
    setenv("colorformat", cmd_buf);
#endif
#if(ENABLE_POWER_MUSIC==1)
    MApi_HDMITx_SetAudioOnOff(TRUE);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);
#endif
    //MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);

    //3.
    //UBOOT_DEBUG(printf("verDispTx_Main - step2\n"));
    MApi_DAC_SetYPbPrOutputTiming(enDACIndex);

#if (ENABLE_MSTAR_KANO == 1 || ENABLE_MSTAR_CURRY ==1 || ENABLE_MSTAR_K6 ==1 || ENABLE_MSTAR_K6LITE == 1)
    MS_U8 *DacTbl=NULL;

    // dump DAC general/scaler/HDGEN register tables
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT);
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_SC);
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDGEN);

    if(eEDIDColorDepth == HDMITX_VIDEO_CD_30Bits)
    {
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_10BIT_Divider);
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_10BIT);
    }
    else
    {
        //Default HDMI TX color depth : 8-bits mode
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_8BIT_Divider);
        MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_INIT_HDMITX_8BIT);
    }
#else  // old chip
    DAC_TAB_INFO DacTbl;
    DacTbl = DACMAP_Main[enDACIndex];
    MApi_DAC_DumpTable(DacTbl.pDacINITTab, DacTbl.eINITtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_SCTab, DacTbl.eINIT_SCtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDGENTab, DacTbl.eINIT_HDGENtype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BIT_DividerTab, DacTbl.eINIT_HDMITX_8BIT_Dividertype);
    MApi_DAC_DumpTable(DacTbl.pDacINIT_HDMITX_8BITTab, DacTbl.eINIT_HDMITX_8BITtype);
#endif

    MApi_DAC_SetVGAHsyncVsync(TRUE);// for VGA out
    MApi_DAC_Enable(TRUE, TRUE);
    MApi_DAC_Enable(TRUE, FALSE);

    MS_U8 u8TvFormat;
    char * p_tvstr = NULL;
    p_tvstr = getenv ("tv_format");
    if(NULL != p_tvstr)
    {
        u8TvFormat = (int)simple_strtol(p_tvstr, NULL, 10);
        if(u8TvFormat==0)
        {
            eVEStd = MS_VE_NTSC;
            _bModeNTSC = TRUE;
        }
        else
        {
            eVEStd = MS_VE_PAL;
            _bModeNTSC = FALSE;
        }
    }
    else
    {
		switch(enPanelType)
		{
			case DACOUT_480I:
			case DACOUT_480P:
			case DACOUT_720P_60:
			case DACOUT_1080I_60:
			case DACOUT_1080P_60:
				{
					eVEStd = MS_VE_NTSC;
					_bModeNTSC = TRUE;
				}
				break;
			default:
				{
					eVEStd = MS_VE_PAL;
					_bModeNTSC = FALSE;
				}
				break;
		}
	}

    U32 u32VEAddr = 0;
    U32 u32VESize = 0;
    if(get_value_from_env(E_MMAP_ID_VE_ADR, MMAP_E_MMAP_ID_VE_ADDR + VE_FRAMEBUFFER_OFFSET, &u32VEAddr) != 0)
    {
        UBOOT_ERROR("get %s env fail\n",E_MMAP_ID_VE_ADR);
	return -1;
    }
    if(get_value_from_env(E_MMAP_ID_VE_LEN, MMAP_E_MMAP_ID_VE_SIZE, &u32VESize) != 0)
    {
        UBOOT_ERROR("get E_MMAP_ID_VE_LEN env fail\n");
	return -1;
    }
//#if (defined(CONFIG_KANO) || defined(CONFIG_CURRY)|| defined(CONFIG_K6) || defined(CONFIG_K6LITE))
#if (defined(CONFIG_KANO) || defined(CONFIG_CURRY))

    // Add by Art, Kano SC1 initialization and set input from SC0 OP
    U32 u32XC1Addr = 0;
    U32 u32XC1Size = 0;
    if(get_value_from_env(E_MMAP_ID_XC1_MAIN_FB_ADR, NO_DEFAULT_MMAP_VALUE, &u32XC1Addr) != 0)
    {
        UBOOT_ERROR("get %s env fail\n",E_MMAP_ID_XC1_MAIN_FB_ADR);
	return -1;
    }
    if(get_value_from_env(E_MMAP_ID_XC1_MAIN_FB_LEN, NO_DEFAULT_MMAP_VALUE, &u32XC1Size) != 0)
    {
        UBOOT_ERROR("get E_MMAP_ID_XC1_MAIN_FB_ADR env fail\n");
	return -1;
    }
    XC_DEVICE_ID stXC_DeviceId = {0, 1};
    PNL_DeviceId stPNL_DeviceId = {0, 1};
    PNL_DeviceId stPNL_DeviceId_SC0 = {0, 0};
    XC_EX_INITDATA stXC_InitData;
    MS_S16 s16PathId = 0;
    PANEL_RESOLUTION_TYPE stPanelType ;
    XC_EX_MUX_PATH_INFO PathInfo;
    XC_EX_SETWIN_INFO stXC_SetWin_Info;
    stPanelType=InitializeEx(enDACIndex);
    if(stPanelType==DACOUT_480I)
    {
        enXc1DACIndex  = E_RES_720x480I_60Hz;
        eVEStd = MS_VE_NTSC;
        _bModeNTSC = TRUE;
        UBOOT_DEBUG("---------XC1 DAC = DAC_PANEL_480I_60 \n");
    }
    else
    {
        enXc1DACIndex  = E_RES_720x576I_50Hz;
        eVEStd = MS_VE_PAL;
        _bModeNTSC = FALSE;
        UBOOT_DEBUG("---------XC1 DAC = DAC_PANEL_576I_50 \n");
    }
    MApi_PNL_EX_Init(&stPNL_DeviceId, (PNL_EX_PanelType *) MApi_GetPanelSpec(stPanelType) ); // NEED MODIFY
    UBOOT_DEBUG("---------Width= 0x%x \n",g_IPanelEx.Width(&stPNL_DeviceId));
    UBOOT_DEBUG("---------Height= 0x%x \n",g_IPanelEx.Height(&stPNL_DeviceId));

    MApi_DAC_SetYPbPrOutputTiming(enXc1DACIndex);
#if (ENABLE_MSTAR_KANO == 1 || ENABLE_MSTAR_CURRY ==1 || ENABLE_MSTAR_K6==1 || ENABLE_MSTAR_K6LITE == 1)
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_SC1_INIT);
    MApi_DAC_DumpTable(DacTbl, DAC_TABTYPE_SC1_INIT_SC);
#else  // old chip
    MApi_DAC_DumpTable(DacTbl.pDacSC1_INITTab, DacTbl.eSC1_INITtype);
    MApi_DAC_DumpTable(DacTbl.pDacSC1_INIT_SCTab, DacTbl.eSC1_INIT_SCtype);
#endif
    memset(&stXC_InitData, 0, sizeof(XC_EX_INITDATA));
    stXC_InitData.u32XTAL_Clock = 12000000UL;
    stXC_InitData.u32Main_FB_Size = u32XC1Size; // NEED MODIFY
    stXC_InitData.u32Main_FB_Start_Addr = u32XC1Addr; // NEED MODIFY
    stXC_InitData.u32Sub_FB_Size = 0;
    stXC_InitData.u32Sub_FB_Start_Addr = 0;
    stXC_InitData.bIsShareGround = FALSE;
    stXC_InitData.bCEC_Use_Interrupt = FALSE;
    stXC_InitData.bEnableIPAutoCoast = XC_PCMONITOR_EX_ENABLE_IP_AUTO_COAST;
    stXC_InitData.bMirror = FALSE;
    stXC_InitData.stPanelInfo.u16HStart = g_IPanelEx.HStart(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16VStart = g_IPanelEx.VStart(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16Width  = g_IPanelEx.Width(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16Height = g_IPanelEx.Height(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16HTotal = g_IPanelEx.HTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16VTotal = g_IPanelEx.VTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16DefaultVFreq = g_IPanelEx.DefaultVFreq(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u8LPLL_Mode = g_IPanelEx.LPLL_Mode(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.enPnl_Out_Timing_Mode = E_XC_EX_PNL_CHG_VTOTAL;
    stXC_InitData.stPanelInfo.u16DefaultHTotal = g_IPanelEx.HTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u16DefaultVTotal = g_IPanelEx.VTotal(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u32MinSET = g_IPanelEx.MinSET(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.u32MaxSET = g_IPanelEx.MaxSET(&stPNL_DeviceId);
    stXC_InitData.stPanelInfo.eLPLL_Type = (XC_EX_PNL_LPLL_TYPE)g_IPanelEx.LPLL_Type(&stPNL_DeviceId);
    stXC_InitData.bDLC_Histogram_From_VBlank = FALSE;
  //  MApi_XC_EX_SkipSWReset(&stXC_DeviceId, FALSE);
    if(MApi_XC_EX_Init(&stXC_DeviceId, &stXC_InitData, sizeof(XC_EX_INITDATA)) == FALSE)
    {
        UBOOT_ERROR("XC_Init failed!\n");
    }
    MApi_XC_EX_SkipSWReset(&stXC_DeviceId, TRUE);
    MApi_XC_EX_SetFrameColor(&stXC_DeviceId, 0x00);
    MApi_XC_EX_SetDispWindowColor(&stXC_DeviceId, 0x82, E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_GenerateBlackVideo(&stXC_DeviceId, TRUE, E_XC_EX_MAIN_WINDOW);
    //MApi_XC_EX_set_FD_Mask_byWin(&stXC_DeviceId, TRUE, E_XC_EX_MAIN_WINDOW);
    //MApi_XC_EX_EnableFrameBufferLess(&stXC_DeviceId, FALSE);
    MApi_XC_EX_Mux_Init(&stXC_DeviceId, _XC_InputSource_InputPort_Mapping);
    memset(&PathInfo, 0, sizeof(XC_EX_MUX_PATH_INFO));
    PathInfo.Path_Type = E_XC_EX_PATH_TYPE_SYNCHRONOUS;
    PathInfo.src = E_XC_EX_INPUT_SOURCE_SCALER_OP;
    PathInfo.dest = E_XC_EX_OUTPUT_SCALER_MAIN_WINDOW;
    PathInfo.SyncEventHandler = NULL;
    PathInfo.DestOnOff_Event_Handler = NULL;
    PathInfo.path_thread = NULL;
    PathInfo.dest_periodic_handler = NULL;
    s16PathId = MApi_XC_EX_Mux_CreatePath(&stXC_DeviceId, &PathInfo, sizeof(XC_EX_MUX_PATH_INFO));
    if (s16PathId == -1)
    {
        UBOOT_ERROR("Create path fail!\n");
    }
    else
    {
        MApi_XC_EX_Mux_EnablePath(&stXC_DeviceId, (MS_U16)s16PathId);
    }
    MApi_XC_EX_SetInputSource(&stXC_DeviceId, E_XC_EX_INPUT_SOURCE_SCALER_OP, E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_DisableInputSource(&stXC_DeviceId, FALSE, E_XC_EX_MAIN_WINDOW);
    memset(&stXC_SetWin_Info, 0, sizeof(XC_EX_SETWIN_INFO));

#if (ENABLE_MSTAR_CURRY == 1)
    PanelType* pPNL_PanelType = MApi_GetPanelSpec(stPanelType);
    switch(enPanelType)
    {
        case DACOUT_480I:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_480P);
            break;
        case DACOUT_576I:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_576P);
            break;
        case DACOUT_1080I_50:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_1080P_50);
            break;
        case DACOUT_1080I_60:
            pPNL_PanelType = MApi_GetPanelSpec(DACOUT_1080P_60);
            break;
        default:
            break;
    }
#endif

    stXC_SetWin_Info.enInputSourceType = E_XC_EX_INPUT_SOURCE_SCALER_OP;
#if (ENABLE_MSTAR_CURRY == 1)
    stXC_SetWin_Info.stCapWin.y = pPNL_PanelType->m_ucPanelVBackPorch - 2;
#else
    stXC_SetWin_Info.stCapWin.y = g_IPanelEx.VSynBackPorch(&stPNL_DeviceId_SC0) - 2;
#endif
    stXC_SetWin_Info.stCapWin.x = (g_IPanelEx.HStart(&stPNL_DeviceId_SC0) - g_IPanelEx.HSynWidth(&stPNL_DeviceId_SC0)) - 1;
    stXC_SetWin_Info.stCapWin.width = g_IPanelEx.Width(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.stCapWin.height = g_IPanelEx.Height(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.bHDuplicate = FALSE;
    stXC_SetWin_Info.u16InputVTotal = g_IPanelEx.HTotal(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.u16DefaultHtotal = g_IPanelEx.VTotal(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.bInterlace = FALSE;
    stXC_SetWin_Info.u16InputVFreq = g_IPanelEx.DefaultVFreq(&stPNL_DeviceId_SC0);
    stXC_SetWin_Info.stCropWin.x = stXC_SetWin_Info.stCapWin.x;
    stXC_SetWin_Info.stCropWin.y = stXC_SetWin_Info.stCapWin.y;
    stXC_SetWin_Info.stCropWin.width = stXC_SetWin_Info.stCapWin.width;
    stXC_SetWin_Info.stCropWin.height= stXC_SetWin_Info.stCapWin.height;
    stXC_SetWin_Info.stDispWin.x = 0;
    stXC_SetWin_Info.stDispWin.y = 0;
    stXC_SetWin_Info.stDispWin.width = g_IPanelEx.Width(&stPNL_DeviceId);
    stXC_SetWin_Info.stDispWin.height = g_IPanelEx.Height(&stPNL_DeviceId);
    MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId, E_XC_EX_MAIN_WINDOW, TRUE);
    //MApi_XC_SetDbgLevel(0x81);
    if(MApi_XC_EX_SetWindow(&stXC_DeviceId, &(stXC_SetWin_Info), sizeof(XC_EX_SETWIN_INFO), E_XC_EX_MAIN_WINDOW) == FALSE)
    {
        UBOOT_ERROR("Set window failed!\n");
    }
    MApi_XC_EX_SkipWaitVsync(&stXC_DeviceId, E_XC_EX_MAIN_WINDOW, FALSE);
    //MApi_XC_EX_WaitOutputVSync(&stXC_DeviceId, 2, 50, E_XC_EX_MAIN_WINDOW);
    MApi_XC_EX_GenerateBlackVideo(&stXC_DeviceId, FALSE, E_XC_EX_MAIN_WINDOW);
#endif //end kano

    //5. VE initialization
    //UBOOT_DEBUG(printf("verDispTx_Main - step4\n"));
    if(eVEStd >= MS_VE_PAL_N)
    {
        //shift 8 pixels in PAL mode
        printf("shift %d pixels in PAL mode \n", VE_WRITE_ADDR_ADJUSTMENT);
        MDrv_VE_AdjustPositionBase(VE_WRITE_ADDR_ADJUSTMENT, VE_READ_ADDR_ADJUSTMENT);
    }
    else
    {
        //shift 0 pixels in NTSC mode
        printf("shift 0 pixels in NTSC mode \n");
        MDrv_VE_AdjustPositionBase(0, VE_READ_ADDR_ADJUSTMENT);
    }

    MDrv_VE_Init((MS_PHYADDR) (u32VEAddr + VE_FRAMEBUFFER_OFFSET));

    MDrv_VE_SetOSD(TRUE);
    MDrv_VE_SetOutputVideoStd(eVEStd);
    MDrv_VE_set_display_window(stVEWin);
    //1. VE configuration
    MS_Switch_VE_Src_Info SwitchInputSrc;
    SwitchInputSrc.InputSrcType = MS_VE_SRC_SCALER;
    MDrv_VE_SwitchInputSource(&SwitchInputSrc);
    MS_Switch_VE_Dest_Info SwitchOutputDest;
    SwitchOutputDest.OutputDstType = MS_VE_DEST_CVBS;
    MDrv_VE_SwitchOuputDest(&SwitchOutputDest);
    MS_VE_Set_Mode_Type SetModeType;
    SetModeType.u16H_CapSize     = g_IPanel.Width();
    SetModeType.u16V_CapSize     = g_IPanel.Height();
    SetModeType.u16H_CapStart    = g_IPanel.HStart();
    SetModeType.u16V_CapStart    = g_IPanel.VStart();
    SetModeType.u16H_SC_CapSize  = g_IPanel.Width();
    SetModeType.u16V_SC_CapSize  = g_IPanel.Height();
    SetModeType.u16H_SC_CapStart = g_IPanel.HStart();
    SetModeType.u16V_SC_CapStart = g_IPanel.VStart();
    SetModeType.u16InputVFreq = 2500*2/10;
    SetModeType.bSrcInterlace = FALSE;
    MDrv_VE_SetMode(&SetModeType);
    MS_VE_Output_Ctrl OutputCtrl;
    OutputCtrl.bEnable    = TRUE;
    OutputCtrl.OutputType = MS_VE_OUT_TVENCODER;
    MDrv_VE_SetOutputCtrl(&OutputCtrl);
    //printf("[%s][%d] VE setting done, system time = %lu \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    //6. Post HDMI setting
    MApi_HDMITx_SetVideoOutputTiming(getHDMITxVideoTiming(enPanelType));
    setHDMITxAnalogTuning(enPanelType);
    // MApi_HDMITx_Exhibit();
    if(getHDMITxVideoTiming(enPanelType) <= HDMITX_RES_720x576p)
    {
        //MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center);
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_4_3_Center, 0x1);
    }
    else
    {
        //MApi_HDMITx_SetVideoOutputOverscan_AFD(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center);
        MApi_HDMITx_SetVideoOutputOverscan_AFD_II(ENABLE, E_HDMITX_VIDEO_SI_NoData, E_HDMITX_VIDEO_AFD_16_9_Center, 0x1);
    }
    switch(getHDMITxVideoTiming(enPanelType))
    {
        case HDMITX_RES_3840x2160p_24Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_24Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_24Hz \n");
            break;
        case HDMITX_RES_3840x2160p_25Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_25Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_25Hz \n");
            break;
        case HDMITX_RES_3840x2160p_30Hz:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_CYCLIC_PACKET, 0x0);
            MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_4k_2k, HDMITx_VIDEO_3D_Not_in_Use, HDMITX_VIDEO_4k2k_30Hz);
            UBOOT_DEBUG("Set_VS_InfoFrame HDMITX_VIDEO_4k2k_30Hz \n");
            break;
        default:
            MApi_HDMITx_PKT_User_Define(HDMITX_VS_INFOFRAME, FALSE, HDMITX_STOP_PACKET, 0x0);
            break;
    }
    //back ground active need disable output (video and audio)
    if(is_str_resume())
    {
        MApi_HDMITx_SetAVMUTE(TRUE);
    }
    else
    {
        if((FALSE == pm_check_back_ground_active()))
        {
            MApi_HDMITx_SetTMDSOnOff(TRUE);
            MsOS_DelayTask(20);
            MApi_HDMITx_SetAVMUTE(FALSE);
        }
        else
        {
            MApi_HDMITx_SetTMDSOnOff(FALSE);
        }
    }
    //7. VE SW Reset here
    MDrv_VE_ReloadSetting();
    UBOOT_TRACE("ON\n");
    //printf("[%s][%d] Function exit, system time = %lu \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    return 0;
}


#if (CONFIG_BOOTVIDEO == 1)
XC_Internal_TimingType msHdmitx_GetEnvOutputTiming(MS_BOOL *pbNtsc)
{
    MS_BOOL bRet;
    MS_BOOL bNtsc = FALSE;
    MS_U16 enPanelType = DACOUT_1080I_50;
    MS_U16 enDACIndex = DAC_PANEL_1080I_50;
    XC_Internal_TimingType enXcTimingType = E_XC_1080I;
    bRet = getResolutionTypeAndDACIndex((MS_U16 *)&enPanelType, (MS_U16 *)&enDACIndex);

    if(FALSE == bRet)
    {
        printf("%s: Error: getResolutionTypeAndDACIndex FAILE! use 1080p default, at %d\n", __func__, __LINE__);
        enPanelType = DACOUT_1080I_50;
        enDACIndex  = DAC_PANEL_1080I_50;
    }

    switch(enDACIndex)
    {
        case DAC_PANEL_480I_60:
            enXcTimingType = E_XC_480I;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_480P_60:
            enXcTimingType = E_XC_480P;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_576I_50:
            enXcTimingType = E_XC_576I;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_576P_50:
            enXcTimingType = E_XC_576P;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_720P_50:
            enXcTimingType = E_XC_720P;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_720P_60:
            enXcTimingType = E_XC_720I;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_1080I_50:
            enXcTimingType = E_XC_1080I;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_1080I_60:
            enXcTimingType = E_XC_1080I;
            bNtsc = TRUE;
            break;
        case DAC_PANEL_1080P_50:
            enXcTimingType = E_XC_1080P;
            bNtsc = FALSE;
            break;
        case DAC_PANEL_1080P_60:
            enXcTimingType = E_XC_1080P;
            bNtsc = TRUE;
            break;
        default:
            // 1080 50I
            enXcTimingType = E_XC_1080I;
            bNtsc = FALSE;
            break;
    }

    if(pbNtsc)
        *pbNtsc = bNtsc;

    return enXcTimingType;
}
#endif  // #if (CONFIG_BOOTVIDEO == 1)
