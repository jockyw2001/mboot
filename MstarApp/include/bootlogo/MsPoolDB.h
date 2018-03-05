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
#ifndef _SPI_DATABASE_H__
#define _SPI_DATABASE_H__
#include <ShareType.h>
#include <MsApiKeypad.h>
#include <apiPNL.h>

#include <MsApiPanel.h>
#include <gop/MsDrvGop.h>
#include <MsSystem.h>

#define BUFFER_SIZE                 128
#define spi_sector_size             0x10000
#define env_sector_size             0x20000

#define Kpd_ChID_Offset             20

#define PWM_DUTY   "u32PWMDuty"
#define PWM_PERIOD "u32PWMPeriod"
#define PWM_DIVPWM "u16DivPWM"
#define PWM_POLPWM "bPolPWM"
#define PWM_MAXPWM "u16MaxPWMvalue"
#define PWM_MINPWM "u16MinPWMvalue"
#define PWM_CH "pnl_pwmch"

#if(CONFIG_ENABLE_V_I_PWM == 1)
// wuyian for pwm0 pwm2
#define I_PWM_ENABLE "I_PWM_ENABLE"
#define I_PWM_DUTY   "I_u32DutyPWM"
#define I_PWM_PERIOD "I_u32PeriodPWM"
#define I_PWM_DIVPWM "I_u16DivPWM"
#define I_PWM_POLPWM "I_bPolPWM"
#define I_PWM_MAXPWM "I_u16MaxPWMvalue"
#define I_PWM_MINPWM "I_u16MinPWMvalue"
#define I_PWM_CH "I_u8PWM_CH"

#define V_PWM_ENABLE "V_PWM_ENABLE"
#define V_PWM_DUTY   "V_u32DutyPWM"
#define V_PWM_PERIOD "V_u32PeriodPWM"
#define V_PWM_DIVPWM "V_u16DivPWM"
#define V_PWM_POLPWM "V_bPolPWM"
#define V_PWM_MAXPWM "V_u16MaxPWMvalue"
#define V_PWM_MINPWM "V_u16MinPWMvalue"
#define V_PWM_CH "V_u8PWM_CH"
#endif

#define KEYPAD_SEL "KEYPAD_SEL"

typedef enum
{
    E_DB_LOGO=0,
    E_DB_MUSIC,
    E_DB_BOARD_PARA,
    E_DB_PANEL_PARA,
    E_DB_URSA_6M30,
    E_DB_URSA,
    E_DB_TCON,
    E_DB_PNL_VB1,
    E_DB_PNL_GOPMUX,
    E_DB_PWM,
    E_DB_MISC,
    E_DB_MSPI,
    E_DB_DMA,
    E_DB_HDMITX,
    E_DB_IR_BOOTCHECK,
    E_DB_IR_WAKEUP,
    E_DB_MAX,
}DB_INDEX;

typedef struct
{
    U32 Offset;
    U32 Size;
    U32 u32CrcCheck;
}ST_DBDATA;

typedef struct
{
    ST_DBDATA dbdata[E_DB_MAX];
    U32 u32CrcCheck;
}ST_DBTABLE;

typedef struct
{
    U8  enInColorFmt;
    U8  enOutColorFmt;
    U8  enInColorQuantRange;
    U8  enOutColorQuantRange;
    U8  enOutputMode;
    U8  enColorDepth;
    U8  enLoadDefaultFromMboot;
}st_hdmitx_setting;

int write_dbtable(U32 u32LogoOffset);
int read_dbtable(U32 u32LogoOffset);

int Read_LogoToDisplayAddr(U32 u32LogoDramAddr);
int Read_PanelParaFromflash(PanelType * panel_data);
int Read_BoardParaFromflash(st_board_para * board_data);
int Read_KeypadParaFromflash(st_keypad_para *keypad_data);
int Read_MusicToPlayAddr(MS_U32 u32MusicDramAddr);
int Read_Ursa_6m30_Para(ursa_6m30_cmd_table * cmd_table);
int Read_Gop_Mux_ParaFromFlash(st_gopmux_para * gopmux_para);

#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M30 == 1)
int iniparser_6m30_cmd(ursa_6m30_cmd_table * cmd_table);
int Load_Ursa6m30Para_ToFlash(void);
int Ursa_6M30_Setting(ursa_6m30_cmd_table * cmd_table);
#elif (ENABLE_URSA_8 == 1)|| (ENABLE_URSA_6M40 == 1)
int Read_Ursa_Para(ursa_cmd_table * cmd_table);
int Load_UrsaPara_ToFlash(void);
#if (ENABLE_URSA_8 == 1)
int Ursa_8_Setting(ursa_cmd_table * cmd_table);
#else
int Ursa_6M40_Syetting(ursa_cmd_table * cmd_table);
#endif

int Parser_Ursa_Ini(ursa_cmd_table * cmd_table);
#endif
#endif

int Read_TConFromFlash(U32 u32TConAddr);

//int Read_PnlVb1FromFlash(U32 u32PnlVb1Addr);
int Read_PnlVb1FromFlash(pnl_VB1ChannelOrder * channelOrder_data);
int Read_MiscSetting_ToFlash(st_sys_misc_setting * misc_data);
int Read_PWMSetting_ToFlash(st_pwm_setting * pwm_data);
int Read_HDMITXFromFlash(st_hdmitx_setting * u32HDMITXAddr);

#if (CONFIG_LOCAL_DIMMING)
//MSPI DRV For Local Dimming  API Structure Declaration
#include <drvMSPI.h>

typedef enum
{
    E_LD_EDGE_TB_TYPE = 0, // top and bottom
    E_LD_EDGE_LR_TYPE = 1, // left and right
    E_LD_DIRECT_TYPE  = 2, //default
    E_LD_LOCAL_TYPE   = 3  // new mode supporting 32x32 LED
} EN_LD_LED_TYPE;

//ST_DRV_LD_DMA_INFO describe DMA
typedef struct
{
    MS_U8 bEdge2DEn;
    MS_U8 u8LDMAchanel;
    MS_U8 u8LDMATrimode;
    MS_U8 u8LDMACheckSumMode;
    MS_U16 u16DMADelay[4];
    MS_U8 u8cmdlength;                //the number of spi head
    MS_U16 u16MspiHead[8];            //led mcu  start Indicator1 command
    MS_U16 u16LedNum;
    MS_U8 u8ClkHz;                    //60hz or 120HZ
    //for LD config
    //MS_U8 u8PackOffset;
    //MS_U8 u8PackLength;
    MS_U8 u8YoffEnd;
    MS_U8 u8LEDWidth;                //led Width
    MS_U8 u8LEDHeight;               //led Height
    MS_U8 u8LDFWidth;                // Statistic width
    MS_U8 u8LDFHeight;               // Statistic height
    MS_U8 u8LSFWidth;                // LSF width
    MS_U8 u8LSFHeight;               // LSF height
    EN_LD_LED_TYPE  eLEDType;        // E_LD_DIRECT_TYPE EnuLD_LED_TYPE
    MS_U32 u32DMABaseOffset;
}ST_DRV_LD_DMA_INFO;


int Read_MSPIPara_FromFash(ST_DRV_MSPI_INFO * mspi_info);
int Read_DMAPara_FromFash(ST_DRV_LD_DMA_INFO * dma_info);
#endif
#endif //__SPI_DATABASE_H__
