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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvHDMITx.h
/// @author MStar Semiconductor Inc.
/// @brief  HDMI Tx Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HDMITX_H_
#define _DRV_HDMITX_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include "halHDMITx.h"
#ifdef CUSTOMER_NDS
#include "apiHDMITx_NDS.h"
#endif // CUSTOMER_NDS

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define HDMITX_EDID_BLK_SIZE            128U //wilson@kano
#define HDMITX_CEA_DAT_BLK_TYPE_NUM		8U //wilson@kano


#ifdef MSOS_TYPE_LINUX_KERNEL
#define HDMITX_ISR_ENABLE              1U
#else
#define HDMITX_ISR_ENABLE              1U
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//*********************//
//        DVI / HDMI   //
//*********************//

/*
       Bit1:
          - 0: DVI
          - 1: HDMI
       Bit0:
          - 0: without HDCP
          - 1: with HDCP
*/
typedef enum
{
    E_HDMITX_DVI            = 0,  // DVI without HDCP
    E_HDMITX_DVI_HDCP       = 1,  // DVI with HDCP
    E_HDMITX_HDMI           = 2,  // HDMI without HDCP
    E_HDMITX_HDMI_HDCP      = 3,  // HDMI with HDCP
} MsHDMITX_OUTPUT_MODE;

typedef enum
{
    E_HDMITX_EVENT_RUN = 0x00000001,
    E_HDMITX_EVENT_IRQ = 0x00000002,
    //wilson@kano
    E_HDMITX_EVENT_RXTIMER = 0x00000004,
    E_HDMITX_EVENT_CECRX = 0x00000008,
    #if 0
    E_HDMITX_EVENT_RITIMER = 0x00000004,
    E_HDMITX_EVENT_RXTIMER = 0x00000008,
    E_HDMITX_EVENT_CECRX = 0x00000010,
    #endif
} MDrvHDMITXEvent;

typedef enum //wilson@kano
{
    E_HdcpTX_EVENT_RUN = 0x00000001,
    E_HdcpTX_EVENT_IRQ = 0x00000002,
    E_HdcpTX_EVENT_RITIMER = 0x00000004,
} MDrvHdcpTXEvent;



typedef enum
{
    //wilson@kano        
	E_HDMITX_FSM_PENDING		= 0,
	E_HDMITX_FSM_VALIDATE_EDID	= 1,
	E_HDMITX_FSM_PRE_TRANSMIT	= 2,
	E_HDMITX_FSM_TRANSMIT		= 3,
	E_HDMITX_FSM_DONE			= 4,
#if 0
    E_HDMITX_FSM_PENDING                    = 0,
    E_HDMITX_FSM_CHECK_HPD                  = 1,
    E_HDMITX_FSM_VALIDATE_EDID              = 2,
    E_HDMITX_FSM_HDCP_AUTH_WAIT_RX          = 3,
    E_HDMITX_FSM_HDCP_AUTH_CHECK_R0         = 4,
    E_HDMITX_FSM_HDCP_AUTH_CHECK_REPEATER   = 5,
    E_HDMITX_FSM_HDCP_AUTH_DONE             = 6,
    E_HDMITX_FSM_HDCP_AUTH_FAIL             = 7,
#endif
} MDrvHDMITX_FSM_STATE;

typedef enum
{
    E_NORMAL_OUTPUT      = 0, // still display normally
    E_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
    E_BLUE_SCREEN = 2, // blue screen
} MDrvHDMITX_UNHDCPRX_CONTROL;

typedef enum
{
    E_CHECK_NOT_READY = 0,
    E_CHECK_REVOKED = 1,
    E_CHECK_NOT_REVOKED = 2,
}MDrvHDMITX_REVOCATION_STATE;

typedef enum
{
    E_RXFail_NORMAL_OUTPUT      = 0, // still display normally
    E_RXFail_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
    E_RXFail_BLUE_SCREEN = 2, // blue screen
} MDrvHDMITX_HDCPRXFail_CONTROL;

typedef enum
{
    E_HDMITX_HDCP_RESET                                = 0x01,
    E_HDMITX_HDCP_WAITING_ACTIVE_RX 	     = 0x02,
    E_HDMITX_HDCP_CHECK_REPEATER_READY = 0x03,
    E_HDMITX_HDCP_CHECK_R0 			     = 0x04,
    E_HDMITX_HDCP_AUTH_DONE 			     = 0x05,
    E_HDMITX_HDCP_AUTH_FAIL 			     = 0x06,

    // bit[7:6]=00 for checking valid rx
    E_HDMITX_HDCP_RX_IS_NOT_VALID		= 0x00, // 00 00
    E_HDMITX_HDCP_RX_IS_VALID         = 0x10, // 00 01
    E_HDMITX_HDCP_RX_KEY_FAIL			= 0x20, // 00 10
    E_HDMITX_HDCP_TX_KEY_FAIL         = 0x30, // 00 11
    E_HDMITX_HDCP_RX_KEY_REVOKED        = 0x0F, // 00 00 11 11

    // bit[7:6]=01 for repeater
    E_HDMITX_HDCP_REPEATER_TIMEOUT 	= 0x40, // 01 00
    E_HDMITX_HDCP_REPEATER_READY 		= 0x50, // 01 01
    E_HDMITX_HDCP_REPEATER_NOT_READY 	= 0x60, // 01 10
    E_HDMITX_HDCP_REPEATER_VALID 		= 0x70, // 01 11

    // bit[7:6]=10 for SHA1
    E_HDMITX_HDCP_REPEATER_SHA1_FAIL 	= 0x80, // 10 00
    E_HDMITX_HDCP_REPEATER_SHA1_PASS 	= 0x90, // 10 01

    // bit[7:6]=11 for Ri
    E_HDMITX_HDCP_SYNC_RI_FAIL 		    = 0xC0, // 11 00
    E_HDMITX_HDCP_SYNC_RI_PASS 		    = 0xD0  // 11 01
}MsHDMITX_HDCP_AUTH_STATUS;

typedef enum
{
	HDCP14Tx_MainState_A0       = 0x00,
	HDCP14Tx_MainState_A1andA2  = 0x01,
//	HDCP14Tx_MainState_A2 = 0x02,
	HDCP14Tx_MainState_A3       = 0x03,
	HDCP14Tx_MainState_A4       = 0x04,
	HDCP14Tx_MainState_A5       = 0x05,
	HDCP14Tx_MainState_A6       = 0x06,
	HDCP14Tx_MainState_A7       = 0x07,
	HDCP14Tx_MainState_A8       = 0x08,
	HDCP14Tx_MainState_A9       = 0x09,
} enHDCP14Tx_MainStates;

typedef enum
{
	HDCP14Tx_SubFSM_IDLE 				= 0x00,
	HDCP14Tx_SubFSM_ExchangeKSV			= 0x01,
	HDCP14Tx_SubFSM_VerifyBksv 			= 0x02,
	HDCP14Tx_SubFSM_Computation			= 0x03,
	HDCP14Tx_SubFSM_CheckR0				= 0x04,
	HDCP14Tx_SubFSM_AuthDone			= 0x05,
	HDCP14Tx_SubFSM_PollingRdyBit		= 0x06,
	HDCP14Tx_SubFSM_AuthWithRepeater	= 0x07,
	HDCP14Tx_SubFSM_CheckRi				= 0x08,
	HDCP14Tx_SubFSM_AuthFail			= 0x09
} enHDCP14Tx_SubStates;

typedef struct
{
    MS_BOOL                             hdcp_Rx_valid;
    //MS_BOOL                             hdmitx_hdcp_flag;       ///< hdmitx hdcp encryption on/off

    enHDCP14Tx_MainStates               hdcp14tx_MainState;
    enHDCP14Tx_SubStates                hdcp14tx_SubState;
    enHDCP14Tx_SubStates                hdcp14tx_PreSubState;

    // HDCP
    MS_BOOL                             hdmitx_HdcpUseInternalKey_flag;    ///< hdmitx HDCP key source
    MS_BOOL                             hdmitx_HdcpStartAuth_flag;    ///< hdmitx HDCP start authentication flag
    MS_BOOL                             hdmitx_HdcpAuthDone_flag;    ///< hdmitx HDCP authentication done flag
    MS_BOOL                             hdmitx_HdcpCheckRepeater_flag;    ///< hdmitx HDCP check repeater flag

    // CEC
    MsHDMITX_HDCP_AUTH_STATUS           hdmitx_HDCPAuth_Status;  ///< hdmitx HDCP authentication status
    MDrvHDMITX_UNHDCPRX_CONTROL         hdmitx_unHDCPRx_Control;  ///< hdmitx unHDCP Rx ouput way
    MDrvHDMITX_HDCPRXFail_CONTROL       hdmitx_HDCPRxFail_Control; ///< hdmitx HDCP Rx fail output way

    MS_U8                               hdcp_srmlist[5116]; // maximum length of the 1st generation
    MS_U8                               hdcp_revocationlist[5068]; // 5x max number of device
    MS_BOOL                             revocationlist_ready; // revocationlist update flag
    MS_U8                               revocation_size;
    MDrvHDMITX_REVOCATION_STATE         revocation_state; // revocation check state

    MS_U8                               HDCP_AKSV[5];
    MS_U8                               HDCP_BKSV[5];
    MS_U8                               ucBStatus[2];
	MS_BOOL                             HDCP_74_check;
} MDrvHdcpTx_PARAMETER_LIST; //wilson@kano

enum enCEA_DATABLK_TAG_CODE //wilson@kano
{
	CEATag_Reserved_0       = 0,
	CEATag_AudioBlk         = 1,
	CEATag_VideoBlk         = 2,
	CEATag_VendorSpecific   = 3,
	CEATag_SpeakerAlloc     = 4,
	CEATag_VESA_DTCBlk      = 5,
	CEATag_Reserved_1       = 6,
	CEATag_UseExtTag        = 7
};

enum enCEA_DATABLK_EXT_TAG_CODE //wilson@kano
{
	CEAExtTag_VideoCap_DB           = 0,
	CEAExtTag_VSVideo_DB            = 1,
	CEAExtTag_VESADisplayDev_DB     = 2,
	CEAExtTag_VESAVideoTimingBlk    = 3,
	CEAExtTag_ReservedHDMIVideoDB   = 4,
	CEAExtTag_ColorimetryDB         = 5,
	CEAExtTag_YCbCr420VideoDB       = 14,
	CEAExtTag_YCbCr420CapMapDB      = 15,
	CEAExtTag_ReservedCEAMiscAudio  = 16,
	CEAExtTag_VSAudioDB             = 17,
	CEAExtTag_ReservedHDMIAudioDB   = 18,
	CEAExtTag_InfoFrameDB           = 32
};

typedef struct
{
	MsHDMITX_AVI_VIC			        VideoTiming;
	MsHDMITX_EDID_3D_STRUCTURE_ALL		Video3DInfo;
} stHDMITX_RXEDID_VIDEO_INFO; //wilson@kano

typedef struct
{
	MS_U8						EdidBlk0[HDMITX_EDID_BLK_SIZE];
	MS_U8						EdidBlk1[HDMITX_EDID_BLK_SIZE];
	MS_U8						ManufacturerID[3];
	MS_U8						CEADataBlkLen[HDMITX_CEA_DAT_BLK_TYPE_NUM];
	MS_U8						PhyAddr[2];
	MS_BOOL						SupportHdmi;

	//HF-VSDB, scdc relative
	MS_U8                       HF_VSDBVerInfo;
	MS_U8                       MaxTmdsCharRate;
    MS_BOOL                     SupportIndependView;
    MS_BOOL                     SupportDualView;
    MS_BOOL                     Support3DOsdDisparity;
	MS_BOOL                     SupportLTEScramble;
	MS_BOOL						SupportSCDC;
	MS_BOOL						SupportRR; //read request
	MS_U8                       YUV420DeepColorInfo;

	MS_BOOL						AudSupportAI;
    MS_BOOL                     b3DPresent;
	MS_BOOL						Support2D_50Hz;
	MS_BOOL						Support2D_60Hz;
	MS_BOOL						Support3D_50Hz;
	MS_BOOL						Support3D_60Hz;
	MsHDMITX_VIDEO_COLORDEPTH_VAL	ColorDepthInfo;

	// data block content
	MS_U8						AudioDataBlk[32];
	MS_U8						VideoDataBlk[32];
	MS_U8						VendorDataBlk[32];
	MS_U8						SpeakerAllocDataBlk[32];
	MS_U8						VESA_DTCDataBlk[32];
	MS_U8						HdmiVICList[7]; //this field has only 3 bits
	stHDMITX_RXEDID_VIDEO_INFO	SupVidTiming[32];

    //for 420
    MS_U8                       YCbCr420VidDataBlk[32];
    MS_U8                       YCbCr420CapMapDataBlk[32];

    //for colorimetry
    MS_U8                       ExtColorimetry;

} stHDMITX_RxEDID_Info; //wilson@kano

typedef struct
{
    MS_BOOL                             hdmitx_hdcp_flag;       ///< hdmitx hdcp encryption on/off
    MS_BOOL                             hdmitx_enable_flag;     ///< hdmitx module actived
    MS_BOOL                             hdmitx_tmds_flag;       ///< hdmitx tmds on/off
    MS_BOOL                             hdmitx_video_flag;      ///< hdmitx video on/off
    MS_BOOL                             hdmitx_audio_flag;      ///< hdmitx audio on/off
    MS_BOOL                             hdmitx_csc_flag;        ///< hdmitx csc on/off
    //MS_BOOL                             hdmitx_audio_supportAI; ///< hdmitx audio support AI
    MS_BOOL                             hdmitx_RB_swap_flag;    ///< hdmitx R/B swap
    MS_BOOL                             hdmitx_force_mode;     ///< hdmitx output force mode: auto/force
    MS_BOOL                             hdmitx_force_output_color;     ///< hdmitx output force color format: auto/force
    MS_BOOL                             hdmitx_AFD_override_mode;     ///< hdmitx AFD override mode: auto/override
    MS_BOOL                             hdmitx_edid_ready;  ///< hdmitx get ready to Rx's EDID
    MS_BOOL                             hdmitx_avmute_flag;     ///< hdmitx AVMUTE status

    // CEC
    MS_BOOL                             hdmitx_CECEnable_flag;    ///< hdmitx CEC enable flag

    MDrvHDMITX_FSM_STATE                hdmitx_fsm_state;       ///< hdmitx fsm state
    MDrvHDMITX_FSM_STATE                hdmitx_fsm_prestate;       ///< hdmitx fsm pre-state
    MsHDMITX_RX_STATUS                  hdmitx_preRX_status;  ///< hdmitx previous Rx status

    MsHDMITX_OUTPUT_MODE                output_mode;            ///< output DVI / HDMI mode
    MsHDMITX_OUTPUT_MODE                force_output_mode;            ///< output DVI / HDMI mode
    MsHDMITX_EXT_COLORIMETRY            ext_colorimetry;
    MsHDMITX_VIDEO_COLORDEPTH_VAL       output_colordepth_val;    // output video color depth
    MsHDMITX_VIDEO_TIMING               output_video_timing;    ///< output video timing
    MsHDMITX_VIDEO_TIMING               output_video_prevtiming;    ///< output video previous timing
    MsHDMITX_VIDEO_COLOR_FORMAT         input_color;            ///< RGB444 / YUV444
    MsHDMITX_VIDEO_COLOR_FORMAT         output_color;           ///< RGB444 / YUV444
    MsHDMITX_VIDEO_COLOR_FORMAT         force_output_color;            ///< RGB444 / YUV444
    MsHDMITX_VIDEO_ASPECT_RATIO         output_aspect_ratio;    // Aspect ratio
    MsHDMITX_VIDEO_SCAN_INFO            output_scan_info; // overscan / underscan
    MsHDMITX_VIDEO_AFD_RATIO            output_afd_ratio; // AFD
    MS_U8		                        output_activeformat_present; // Active format information present
    MsHDMITX_AUDIO_FREQUENCY            output_audio_frequncy;  ///< audio sampling frequency
    MsHDMITX_AUDIO_CHANNEL_COUNT        output_audio_channel;   // audio channel count
    MsHDMITX_AUDIO_CODING_TYPE          output_audio_type;    // audio coding type

    MsHDMITX_ANALOG_TUNING              analog_setting;        // HDMI Tx Pre-emphasis and Double termination

	//EDID
	stHDMITX_RxEDID_Info                RxEdidInfo; //wilson@kano

    //wilson@kano: sorting previous EDID relative items
    //map to ColorDepthInfo //MsHDMITX_VIDEO_COLORDEPTH_VAL      edid_colordepth_val;    // EDID video color depth
    //map to PhyAddr[]//MS_U8                              edid_phyadr[2];    // EDID physical address
    //map to VideoDataBlk//MS_U8	                           short_video_descriptor[32]; // short video descriptor of EDID
    //map to AudioDataBlk//MS_U8                              short_audio_descriptor[32]; // short audio descriptor of EDID
    //map to CEADataBlkLen//MS_U8                              data_block_length[8]; // data block length of each data block
    //map to ManufacturerID//MS_U8	                           id_manufacturer_name[3]; // ID Manufacturer Name
    //map to EdidBlk0//MS_U8                              edid_block0[128]; //EDID's 1st 128 data
    //map to EdidBlk1//MS_U8                              edid_block1[128]; //EDID's 2nd 128 data
    //map to Support2D_50Hz//MS_BOOL                            edid_2D_50hz_support;
	//map to Support2D_60Hz//MS_BOOL                            edid_2D_60hz_support;
    //map to Support3D_50Hz//MS_BOOL                            edid_3D_50hz_support;
	//map to Support3D_60Hz//MS_BOOL                            edid_3D_60hz_support;
	//map to 3DPresent//MS_BOOL                            edid_3D_present;
	//map to SupportHdmi//MS_BOOL                            edid_HDMI_support;
	//SupVidTiming//MsHDMITX_EDID_3D_SUPPORT_TIMING    edid_3D_support_timing[32];
} MDrvHDMITX_PARAMETER_LIST; //wilson@kano

// debug mask definition
#define HDMITX_DBG              0x01U ///< Debug PQ Table
#define HDMITX_DBG_HDCP         0x02U ///< Debug S RULE
#define HDMITX_DBG_UTILTX       0x04U ///< Debug S RULE

/**
*   @brief HDMI Status
*/
typedef struct
{
    MS_BOOL bIsInitialized;
    MS_BOOL bIsRunning;
}MS_HDMI_TX_Status;

typedef struct
{
    MS_BOOL bIsInitialized;
    MS_BOOL bIsRunning;
}MS_HDCP_TX_Status;

// Debug
/**
*   @brief HDMI Info
*/
typedef struct
{
    MS_U8 Reserved;
}MS_HDMI_TX_INFO;

typedef struct
{
    // HDMI Tx Current, Pre-emphasis and Double termination
    MS_U8    tm_txcurrent; // TX current control(U4: 0x11302B[13:12], K1: 0x11302B[13:11])
    MS_U8    tm_pren2; // pre-emphasis mode control, 0x11302D[5]
    MS_U8    tm_precon; // TM_PRECON, 0x11302E[7:4]
    MS_U8    tm_pren; // pre-emphasis enable, 0x11302E[11:8]
    MS_U8    tm_tenpre; // Double termination pre-emphasis enable, 0x11302F[3:0]
    MS_U8    tm_ten; // Double termination enable, 0x11302F[7:4]
} drvHDMITX_ANALOG_TUNING;


//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef MDRV_HDMITX_C
#define INTERFACED
#else
#define INTERFACED extern
#endif

INTERFACED void MDrv_HDCPTx_SetAksv2R0Interval(MS_U32 u32Interval);
INTERFACED void MDrv_HDCPTx_SetUnHDCPRxCtrl(MDrvHDMITX_UNHDCPRX_CONTROL enUnHDCPCtrl);
INTERFACED void MDrv_HDCPTx_SetHDCPRxFailCtrl(MDrvHDMITX_UNHDCPRX_CONTROL enUnHDCPCtrl);
INTERFACED MS_BOOL MDrv_HDCPTx_GetAuthDoneFlag(void); //wilson@kano
INTERFACED void MDrv_HDCPTx_GetHdcpKey(MS_BOOL useinternalkey, MS_U8 *data);
INTERFACED MS_BOOL MDrv_HDCPTx_GetBksv(MS_U8 *pucData);
INTERFACED MS_BOOL MDrv_HDCPTx_GetAksv(MS_U8 *pucData);
INTERFACED void MDrv_HDCPTx_GetRevocationKeyList(MS_U8 *data, MS_U16 size);
INTERFACED enHDCP14Tx_SubStates MDrv_HDCPTx_GetPreState(void);
INTERFACED MS_BOOL MDrv_HDCPTx_GetRxValid(void); //wilson@kano
INTERFACED MS_BOOL MDrv_HDCPTx_CheckAuthFailFlag(void);
INTERFACED MDrvHDMITX_REVOCATION_STATE MDrv_HDCPTx_RevocationKey_Check(void);
INTERFACED MS_BOOL MDrv_HDCPTx_IsSRMSignatureValid(MS_U8 *data, MS_U32 size);
INTERFACED void MDrv_HDCPTx_StartAuth(MS_BOOL bFlag);
/////////////////////////// HDMI TX relative
INTERFACED MS_BOOL MDrv_HDMITx_EdidChecking(void);
INTERFACED MS_BOOL MDrv_HDMITx_GetHdcpEnFlag(void);
INTERFACED MsHDMITX_OUTPUT_MODE MDrv_HDMITx_GetOutputMode(void);
INTERFACED MS_BOOL MDrv_HDMITx_GetRxStatus(void);
INTERFACED MsHDMITX_VIDEO_COLORDEPTH_VAL MDrv_HDMITx_GetDeepColorMode(void);
INTERFACED MsHDMITX_VIDEO_COLORDEPTH_VAL MDrv_HDMITx_GetRxDCInfoFromEDID(void);
INTERFACED MS_BOOL MDrv_HDMITx_GetEdidRdyFlag(void);
INTERFACED void MDrv_HDMITx_GetRxVideoFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize);
INTERFACED void MDrv_HDMITx_GetDataBlockLengthFromEDID(MS_U8 *pu8Length, MS_U8 u8TagCode);
INTERFACED void MDrv_HDMITx_GetRxAudioFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize);
INTERFACED MS_BOOL MDrv_HDMITx_EdidGetHDMISupportFlag(void);
INTERFACED void MDrv_HDMITx_GetRxIDManufacturerName(MS_U8 *pu8Buffer);
INTERFACED void MDrv_HDMITx_GetEDIDData(MS_U8 *pu8Buffer, MS_BOOL BlockIdx);
INTERFACED void MDrv_HDMITx_GetRx3DStructureFromEDID(MsHDMITX_VIDEO_TIMING timing, MsHDMITX_EDID_3D_STRUCTURE_ALL *p3DStructure);
INTERFACED MS_U32 MDrv_HDMITx_GetAudioCTS(void);
INTERFACED MS_BOOL MDrv_HDMITx_GetAVMUTEStatus(void);
INTERFACED MS_BOOL  MDrv_HDMITx_GetLibVer(const MSIF_Version **ppVersion);
INTERFACED MS_BOOL  MDrv_HDMITx_GetInfo(MS_HDMI_TX_INFO *pInfo);
INTERFACED MS_BOOL  MDrv_HDMITx_GetStatus(MS_HDMI_TX_Status *pStatus);
INTERFACED void MDrv_HDMITx_GetEDIDPhyAdr(MS_U8* pucData);
INTERFACED MDrvHDMITX_FSM_STATE MDrv_HDMITx_GetFSMState(void);
INTERFACED void MDrv_HDMITx_SetTMDSOnOff(MS_BOOL state);
INTERFACED void MDrv_HDMITx_SetHDMITxMode(MsHDMITX_OUTPUT_MODE mode);
INTERFACED void MDrv_HDMITx_SetDeepColorMode(MsHDMITX_VIDEO_COLORDEPTH_VAL enDeepColorMode);
INTERFACED void MDrv_HDMITx_SetRBChannelSwap(MS_BOOL state); //wilson@kano
INTERFACED void MDrv_HDMITx_SetVideoOnOff(MS_BOOL state);
INTERFACED void MDrv_HDMITx_SetColorFormat(MsHDMITX_VIDEO_COLOR_FORMAT enInColor, MsHDMITX_VIDEO_COLOR_FORMAT enOutColor);
INTERFACED void MDrv_HDMITx_SetAVMUTE(MS_BOOL bflag);
INTERFACED void MDrv_HDMITx_Set_VS_InfoFrame(MsHDMITX_VIDEO_VS_FORMAT enVSFmt, MsHDMITX_VIDEO_4k2k_VIC enVid4K2KVic, MsHDMITX_VIDEO_3D_STRUCTURE enVid3DStruct);
INTERFACED void MDrv_HDMITx_SetVideoOutputMode(MsHDMITX_VIDEO_TIMING enVidTiming);
INTERFACED void MDrv_HDMITx_SetVideoOutputAsepctRatio(MsHDMITX_VIDEO_ASPECT_RATIO enOutputAR);
INTERFACED void MDrv_HDMITx_SetVideoOutputOverscan_AFD(MS_U8 ucA0, MS_BOOL bAFDOverwrite, MsHDMITX_VIDEO_SCAN_INFO enScanInfo, MsHDMITX_VIDEO_AFD_RATIO enAFDRatio);
INTERFACED void MDrv_HDMITx_SetAudioOnOff(MS_BOOL state);
INTERFACED void MDrv_HDMITx_SetAudioFrequency(MsHDMITX_AUDIO_FREQUENCY enAudFreq);
INTERFACED void MDrv_HDMITx_SetAudioChCnt(MsHDMITX_AUDIO_CHANNEL_COUNT enChCnt);
INTERFACED void MDrv_HDMITx_SetAudioFmt(MsHDMITX_AUDIO_CODING_TYPE enAudFmt);
INTERFACED void MDrv_HDMITx_SetAudioSourceFormat(MsHDMITX_AUDIO_SOURCE_FORMAT fmt);
INTERFACED void MDrv_HDMITx_SetHDCPOnOff(MS_BOOL state);
INTERFACED MS_BOOL  MDrv_HDMITx_SetDbgLevel(MS_U16 u16DbgSwitch);
INTERFACED void MDrv_HDMITx_SetHPDGpioPin(MS_U8 u8pin);
INTERFACED void MDrv_HDMITx_SetCECOnOff(MS_BOOL bflag);
INTERFACED MS_BOOL MDrv_HDMITx_GetCECStatus(void);
INTERFACED void MDrv_HDMITx_Power_OnOff(MS_BOOL bEnable);
INTERFACED void MDrv_HDMITx_TurnOnOff(MS_BOOL state);
INTERFACED void MDrv_HDMITx_EnablePacketGen(MS_BOOL bflag);
INTERFACED void MDrv_HDMITx_PKT_User_Define_Clear(void);
INTERFACED void MDrv_HDMITx_PKT_User_Define(MsHDMITX_PACKET_TYPE packet_type, MS_BOOL def_flag, MsHDMITX_PACKET_PROCESS def_process, MS_U8 def_fcnt);
INTERFACED MS_BOOL MDrv_HDMITx_PKT_Content_Define(MsHDMITX_PACKET_TYPE packet_type, MS_U8* data, MS_U8 length);
INTERFACED void MDrv_HDMITx_MuteAudioFIFO(MS_BOOL bflag);
INTERFACED void MDrv_HDMITx_AnalogTuning(drvHDMITX_ANALOG_TUNING *pInfo);
INTERFACED void MDrv_HDMITx_ForceHDMIOutputMode(MS_BOOL bflag, MsHDMITX_OUTPUT_MODE output_mode);
INTERFACED MS_BOOL MDrv_HDMITx_ForceHDMIOutputColorFormat(MS_BOOL bflag, MsHDMITX_VIDEO_COLOR_FORMAT output_color);
INTERFACED void MDrv_HDMITx_DisableRegWrite(MS_BOOL bFlag);
INTERFACED MS_BOOL MDrv_HDMITx_RxBypass_Mode(MsHDMITX_INPUT_FREQ freq, MS_BOOL bflag);
INTERFACED MS_BOOL MDrv_HDMITx_Disable_RxBypass(void);
INTERFACED MS_BOOL MDrv_HDMITx_IsSupportDVIMode(void);
INTERFACED MS_U32 MDrv_HDMITx_SetPowerState(EN_POWER_MODE u16PowerState);
INTERFACED void MDrv_HDMITx_DisableTMDSCtrl(MS_BOOL bFlag);
INTERFACED void MDrv_HDMITx_Exhibit(void);
INTERFACED MS_BOOL MDrv_HDMITxHDCPTx_Init(void); //wilson@kano

#ifdef __cplusplus
}
#endif

#endif // _DRV_HDMITX_H_

