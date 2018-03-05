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
#include <config.h>
#include <configs/uboot_board_config.h>
#include <MsTypes.h>
#include <malloc.h>

#include <MsCommon.h>
#include <MsVersion.h>
#include <apiPNL.h>
#include <drvPWM.h>
#include <drvPM.h>
#include <drvMBX.h>
#include <panel/MsDrvPanel.h>
#include <mhal_panel.h>
#include <panel/Gamma12BIT_256.inc>
#include <MsApiPM.h>
#include <MsSystem.h>
#include <MsSysUtility.h>
#include <MsUboot.h>
#include <MsMmap.h>
#if(CONFIG_HDMITX_MSTAR_ROCKET ==1)
#include <apiHDMITx.h>
#endif

#if(ENABLE_URSA_6M30 == 1)
#include <ursa/ursa_6m30.h>
#endif
#if(ENABLE_URSA_8 == 1)
#include <ursa/ursa_8.h>
#endif
#if(ENABLE_URSA6_VB1 == 1 || ENABLE_NOVA_KS2 == 1)
#include <ursa/ursa_6m38.h>
#endif
#if(ENABLE_URSA7_VB1 == 1)
#include <ursa/ursa_7.h>
#endif
#if(ENABLE_URSA9_VB1 == 1)
#include <ursa/ursa_9.h>
#endif
#if (CONFIG_URSA_UNION)
#include <ursa/ursa_common.h>
#endif
#if (CONFIG_XC_FRC_VB1)
#include <apiXC.h>
#endif
#include <mstarstr.h>
#include <bootlogo/MsPoolDB.h>
#include <drvGPIO.h>
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

#include <MsDebug.h>
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
unsigned long lPanleTimer =0;
unsigned long lPanelOnTiming=0;
unsigned int  TCON_VERSION=0;
//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static unsigned char PanelLinkExtType=10;





#define LVDS_OUTPUT_USER          4
#if(ENABLE_URSA_6M30 == 1)
#define PANEL_4K2K_ENABLE         0
#endif
#if (CONFIG_PANEL_INIT)
void SetDivison(MS_U16 u16GPIOINDEX,MS_U8 u8GPIOVALUE)
{
	UBOOT_DEBUG("%s:gpio %d set %d \n",__FUNCTION__,u16GPIOINDEX,u8GPIOVALUE);

	u16GPIOINDEX=u16GPIOINDEX+1;
	if(u16GPIOINDEX!=0)
	{
		if(u8GPIOVALUE==0)
		{
			mdrv_gpio_set_low(u16GPIOINDEX);
		}
		else
		{
			mdrv_gpio_set_high(u16GPIOINDEX);
		}
	}
}
void SetPWM(U16 Period,U16 Duty,U16 PwmDiv,PWM_ChNum PWMPort)
{
    PWM_Result result = E_PWM_FAIL;

    U16 bPolPWM = 0;

    result = MDrv_PWM_Init(E_PWM_DBGLV_NONE);
    if(result == E_PWM_FAIL)
    {
        UBOOT_DEBUG("Sorry , init failed!~\n");
        return ;
    }

    st_pwm_setting pwm_setting;
    Read_PWMSetting_ToFlash(&pwm_setting);
    bPolPWM = pwm_setting.m_bPolPWM;
	UBOOT_DEBUG("\nbPolPWM_T = %d\n",bPolPWM);

    //MDrv_PWM_UnitDiv(0); //duplicate  MDrv_PWM_Div
    MDrv_PWM_Oen(PWMPort, 0);    /* Set 0 for output enable */
    MDrv_PWM_Period(PWMPort, Period);
    MDrv_PWM_DutyCycle(PWMPort, Duty);
    MDrv_PWM_Div(PWMPort, PwmDiv);
	if(bPolPWM==1)
    MDrv_PWM_Polarity(PWMPort, TRUE);
	else
    MDrv_PWM_Polarity(PWMPort, FALSE);
    MDrv_PWM_Vdben(PWMPort, FALSE);
    MDrv_PWM_Dben(PWMPort,FALSE);
}

#if(CONFIG_ENABLE_V_I_PWM == 1)
// wuyian for pwm0 pwm2
extern int bSafeModeEnable;
void checkSafeModeEnable(void)
{
    U8 eep_safemode = 0xff;
    char str[BUFFER_SIZE];
    char *p_str=NULL;
    p_str = getenv("bSafeModeEnable");
    if(strcmp(p_str, "false") == 0)
    {
        if(strcmp(getenv("safemodeset"), "off") != 0)
        {
            setenv("safemodeset","off");
            saveenv();
            eep_safemode = 0;
            memset(str, 0 , BUFFER_SIZE);
            snprintf(str, BUFFER_SIZE, "eeprom write 0x%x 0x001E 1", (MS_PHYADDR)&eep_safemode);
            run_command(str, 0);
        }
        printf("---wya---:force set safemodeset off \n");
    }
}

MS_BOOL checkNeedSetSafeMode(void)
{
    if(strcmp(getenv("safemodeset"), "on") == 0)
    {
        printf("---wya---:safemodeset on \n");
        return TRUE;
    }
    return FALSE;
}

void SetV_I_PWM(U16 Period,U16 Duty,U16 PwmDiv,PWM_ChNum PWMPort,U16 PolPWM)
{
    PWM_Result result = E_PWM_FAIL;

    result = MDrv_PWM_Init(E_PWM_DBGLV_NONE);
    if(result == E_PWM_FAIL)
    {
        UBOOT_DEBUG("Sorry , init failed!~\n");
        return ;
    }

    //MDrv_PWM_UnitDiv(0); //duplicate  MDrv_PWM_Div
    MDrv_PWM_Oen(PWMPort, 0);    /* Set 0 for output enable */
    MDrv_PWM_Period(PWMPort, Period);
    MDrv_PWM_DutyCycle(PWMPort, Duty);
    MDrv_PWM_Div(PWMPort, PwmDiv);
    if(PolPWM==1)
        MDrv_PWM_Polarity(PWMPort, TRUE);
    else
        MDrv_PWM_Polarity(PWMPort, FALSE);
    MDrv_PWM_Vdben(PWMPort, FALSE);
    MDrv_PWM_Dben(PWMPort,FALSE);
}


int V_I_PWM_init(const char *str)
{
    U8 bPWMEnable = 0;
    U16 u32PWMDuty = 0;
    U16 u32PWMPeriod = 0;
    U16 u32PWMDIV = 0;
    U16 bPolPWM = 0;
    U16 u16maxPWM =0;
    U16 u16minPWM =0;
    PWM_ChNum PWMPort = E_PWM_CH0;
    char *p_str = NULL;

    char *Str_PWM_ENABLE = NULL;
    char *Str_PWM_DUTY   = NULL;
    char *Str_PWM_PERIOD = NULL;
    char *Str_PWM_DIVPWM = NULL;
    char *Str_PWM_POLPWM = NULL;
    char *Str_PWM_MAXPWM = NULL;
    char *Str_PWM_MINPWM = NULL;
    char *Str_PWM_CH = NULL;

    if(strcmp(str, "I-PWM") == 0)
    {
        Str_PWM_ENABLE = I_PWM_ENABLE;
        Str_PWM_DUTY = I_PWM_DUTY;
        Str_PWM_PERIOD = I_PWM_PERIOD;
        Str_PWM_DIVPWM = I_PWM_DIVPWM;
        Str_PWM_POLPWM = I_PWM_POLPWM;
        Str_PWM_MAXPWM = I_PWM_MAXPWM;
        Str_PWM_MINPWM = I_PWM_MINPWM;
        Str_PWM_CH = I_PWM_CH;
    }
    else if(strcmp(str, "V-PWM") == 0)
    {
        Str_PWM_ENABLE = V_PWM_ENABLE;
        Str_PWM_DUTY = V_PWM_DUTY;
        Str_PWM_PERIOD = V_PWM_PERIOD;
        Str_PWM_DIVPWM = V_PWM_DIVPWM;
        Str_PWM_POLPWM = V_PWM_POLPWM;
        Str_PWM_MAXPWM = V_PWM_MAXPWM;
        Str_PWM_MINPWM = V_PWM_MINPWM;
        Str_PWM_CH = V_PWM_CH;
    }
    else
    {
        return 0;
    }


    p_str = getenv(Str_PWM_ENABLE);
    if(p_str != NULL)
    {
        bPWMEnable = simple_strtol(p_str, NULL, 16);
    }

    printf("init %s = %d\n",Str_PWM_ENABLE,bPWMEnable);

    checkSafeModeEnable();

    if(bPWMEnable == 1)
    {
        p_str = getenv(Str_PWM_DIVPWM);
        if(p_str != NULL)
        {
            u32PWMDIV = simple_strtol(p_str, NULL, 16);
        }

        p_str = getenv(Str_PWM_PERIOD);
        if(p_str != NULL)
        {
            u32PWMPeriod = simple_strtol(p_str, NULL, 16);
        }

        p_str = getenv(Str_PWM_DUTY);
        if(p_str != NULL)
        {
            u32PWMDuty = simple_strtol(p_str, NULL, 16);
        }

        p_str = getenv(Str_PWM_POLPWM);
        if(p_str != NULL)
        {
            bPolPWM = simple_strtol(p_str, NULL, 16);
        }

        p_str = getenv(Str_PWM_MAXPWM);
        if(p_str != NULL)
        {
            u16maxPWM = simple_strtol(p_str, NULL, 16);
        }

        p_str = getenv(Str_PWM_MINPWM);
        if(p_str != NULL)
        {
            u16minPWM = simple_strtol(p_str, NULL, 16);
        }

        p_str = getenv(Str_PWM_CH);
        if( p_str != NULL )
        {
            PWMPort = (PWM_ChNum)simple_strtol(p_str, NULL, 16);
            if(PWMPort >= 0 && PWMPort <= 9)
            {
                if(checkNeedSetSafeMode())
                {
                    printf("---wya--- INIT is safemode\n");
                    u32PWMDuty = 0xBF;
                }

                printf("init %s = %d\n",Str_PWM_CH,PWMPort);
                printf("init %s = %d\n",Str_PWM_PERIOD,u32PWMPeriod);
                printf("init %s = %d\n",Str_PWM_DUTY,u32PWMDuty);
                printf("init %s = %d\n",Str_PWM_DIVPWM,u32PWMDIV);
                printf("init %s = %d\n",Str_PWM_POLPWM,bPolPWM);
                printf("init %s = %d\n",Str_PWM_MAXPWM,u16maxPWM);
                printf("init %s = %d\n",Str_PWM_MINPWM,u16minPWM);
                #if(CONFIG_MSTAR_BD_MST082B_10AJQ_MONET_CUS36 == 1)
                if(strcmp(str, "V-PWM") == 0)
                {
                    MDrv_PM_PWM_Dben(1);
                    MDrv_PM_PWM_Period(u32PWMPeriod); // U16 u16PeriodPWM)
                    MDrv_PM_PWM_DutyCycle(u32PWMDuty); // U16 u16DutyPWM)
                    MDrv_PM_PWM_Div(u32PWMDIV);
                    MDrv_PM_PWM_Polarity(bPolPWM);
                    MDrv_PM_PWM_Enable();
                }
                else
                {
                    SetV_I_PWM(u32PWMPeriod,u32PWMDuty, u32PWMDIV,PWMPort,bPolPWM);
                }
                #else
                SetV_I_PWM(u32PWMPeriod,u32PWMDuty, u32PWMDIV,PWMPort,bPolPWM);
                #endif
            }
        }
    }

    return 0;

}
#endif //CONFIG_ENABLE_V_I_PWM


int PWM_init(void)
{
    U16 u32PWMDuty = 0;
    U16 u32PWMPeriod = 0;
    U16 u32PWMDIV = 0;
	U16 u16maxPWM =0;
	U16 u16minPWM =0;
	PWM_ChNum PWMPort = 0;

    st_pwm_setting pwm_setting;
    Read_PWMSetting_ToFlash(&pwm_setting);

    u32PWMDIV = pwm_setting.m_u16DivPWM;
    u32PWMPeriod = pwm_setting.m_u32PWMPeriod;
    u16maxPWM = pwm_setting.m_u16maxPWM;
    u16minPWM = pwm_setting.m_u16minPWM;

    PWMPort = (PWM_ChNum)pwm_setting.m_u16PWMPort;
    if(PWMPort < 0 || PWMPort >9)
    {
        UBOOT_DEBUG("PWM CH out of range ,So use default CH2\n");
        PWMPort = (PWM_ChNum)E_PWM_CH2;
    }

    u32PWMDuty = (u16maxPWM+u16minPWM)/2;

    UBOOT_DEBUG("u32PWMPeriod_T = %d\n",u32PWMPeriod);
    UBOOT_DEBUG("u32PWMDuty_T = %d\n",u32PWMDuty);
    UBOOT_DEBUG("u32PWMDIV_T = %d\n",u32PWMDIV);
    UBOOT_DEBUG("u16maxPWM_T = %d\n",u16maxPWM);
    UBOOT_DEBUG("u16minPWM_T = %d\n",u16minPWM);

    SetPWM(u32PWMPeriod,u32PWMDuty, u32PWMDIV,PWMPort);
    return 0;
}

#include <MsOS.h>
#include <drvXC_IOPort.h>
#include <apiXC.h>
#include <panel/pnl_tcon_tbl.h>
#include <panel/pnl_tcon_tbl.c>


E_TCON_TAB_TYPE eTCONPNL_TypeSel = TCON_TABTYPE_GENERAL;
static void _MApi_TCon_Tab_DumpPSRegTab(MS_U8* pu8TconTab)
{
    #define TCON_DUMP_TIMEOUT_TIME  1000
    MS_U32 u32tabIdx = 0;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;
    MS_U8 u8NeedDelay;
    MS_U8 u8DelayTime;
    MS_U32 u32StartTime = MsOS_GetSystemTime();
    UBOOT_TRACE("IN\n");


    if (pu8TconTab == NULL)
    {
        UBOOT_ERROR("pu8TconTab is NULL! at %s %u \n", __FUNCTION__, __LINE__);
        return;
    }

    while(1)
    {
        if( (MsOS_GetSystemTime() - u32StartTime) > TCON_DUMP_TIMEOUT_TIME )
        {
            UBOOT_ERROR("Dump tcon tab timeout! at %s %u \n", __FUNCTION__, __LINE__);
            return;
        }

        u32Addr     = ((pu8TconTab[u32tabIdx]<<8) + pu8TconTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask     = pu8TconTab[(u32tabIdx +2)] & 0xFF;
        u16Value    = pu8TconTab[(u32tabIdx +3)] & 0xFF;
        u8NeedDelay = pu8TconTab[(u32tabIdx +4)] & 0xFF;
        u8DelayTime = pu8TconTab[(u32tabIdx +5)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        u32Addr = (u32Addr | 0x100000);

        if( u32Addr == REG_TCON_BASE )
        {
            UBOOT_DEBUG("Wait V sync \n");
            MApi_XC_WaitOutputVSync(1, 50, MAIN_WINDOW);
        }

        if (u32Addr & 0x1)
        {
            u32Addr --;
            //temp_mask, need check!! MApi_PNL_Write2ByteMask(u32Addr, (u16Value << 8), (u16Mask << 8));//MApi_XC_Write2ByteMask
            MApi_XC_Write2ByteMask(u32Addr, (u16Value << 8), (u16Mask << 8));
            UBOOT_DEBUG("[Odd .addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, (u16Mask << 8), (u16Value << 8));
        }
        else
        {
            MApi_XC_Write2ByteMask(u32Addr, u16Value, u16Mask);
            //temp_mask, need check!! MApi_PNL_Write2ByteMask(u32Addr, u16Value, u16Mask);//MApi_XC_Write2ByteMask
            UBOOT_DEBUG("[Even.addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);
        }
        // Check need delay?
        if( u8NeedDelay && u8DelayTime )
        {
            mdelay(u8DelayTime);
        }
        u32tabIdx = u32tabIdx + 7;
    }
    UBOOT_TRACE("OK\n");
}

// TCon register type
typedef enum
{
    EN_TCON_REGISTERTYPE_6_BYTES = 0,       // 32-bit address, 4+1+1, address+mask+value
    EN_TCON_REGISTERTYPE_4_BYTES,       // 16-bit address, 2+1+1, address+mask+value
    EN_TCON_REGISTERTYPE_POWERSEQENCE_9_BYTES,      // 32-bit address, 4+1+1+1+1+1, address+mask+value+delayReady+delayTime+signalType
    EN_TCON_REGISTERTYPE_POWERSEQENCE_7_BYTES,      // 16-bit address, 2+1+1+1+1+1, address+mask+value+delayReady+delayTime+signalType
    EN_TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_15_BYTES, // 32-bit address, 4+1+1+4+1+1+1+1+1, subAddress+subMask+subValue+address+mask+value+delayReady+delayTime+signalType
    EN_TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_11_BYTES, // 16-bit address, 2+1+1+2+1+1+1+1+1, subAddress+subMask+subValue+address+mask+value+delayReady+delayTime+signalType
    EN_TCON_REGISTERTYPE_POWERGAMMA_4_BYTES, // Register type 6: P-Gamma (Power Gamma) 4-byte register format (16-bit address size and 16-bit value size)
    EN_TCON_REGISTERTYPE_3_BYTES, // Register type 7: TCON 3-byte register format (12-bit address size)
    EN_TCON_REGISTERTYPE_PANEL_1_BYTES, // Register type 8: panel
    EN_TCON_REGISTERTYPE_END,
} EN_TCON_REGISTERTYPE;

#define TCON_REGISTERTYPE_1_BYTES   1
#define TCON_REGISTERTYPE_6_BYTES   6
#define TCON_REGISTERTYPE_4_BYTES   4
#define TCON_REGISTERTYPE_POWERSEQENCE_9_BYTES   9
#define TCON_REGISTERTYPE_POWERSEQENCE_7_BYTES   7
#define TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_15_BYTES   15
#define TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_11_BYTES   11

unsigned int _registerTypeToSize(unsigned char registerType)
{
    EN_TCON_REGISTERTYPE en = (EN_TCON_REGISTERTYPE)registerType;
    switch (en)
    {
        case EN_TCON_REGISTERTYPE_6_BYTES:
            return TCON_REGISTERTYPE_6_BYTES;
        case EN_TCON_REGISTERTYPE_4_BYTES:
            return TCON_REGISTERTYPE_4_BYTES;
        case EN_TCON_REGISTERTYPE_POWERSEQENCE_9_BYTES:
            return TCON_REGISTERTYPE_POWERSEQENCE_9_BYTES;
        case EN_TCON_REGISTERTYPE_POWERSEQENCE_7_BYTES:
            return TCON_REGISTERTYPE_POWERSEQENCE_7_BYTES;
        case EN_TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_15_BYTES:
            return TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_15_BYTES;
        case EN_TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_11_BYTES:
            return TCON_REGISTERTYPE_POWERSEQENCE_WITHSUB_11_BYTES;
        case EN_TCON_REGISTERTYPE_PANEL_1_BYTES:
            return TCON_REGISTERTYPE_1_BYTES;
        default:
            UBOOT_ERROR("register type out of range: %d", en);
            return 0;
    }
}

#if (!CONFIG_UBOOT_MINI_LIB)
extern ST_DBTABLE gstDbtable;

static unsigned char* _load_TCON_from_dbtable(void)
{
    void* Dram_Addr = NULL;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("u32FileSize=0x%x\n",(unsigned int)gstDbtable.dbdata[E_DB_TCON].Size);
    Dram_Addr=malloc(gstDbtable.dbdata[E_DB_TCON].Size);
    if(Dram_Addr==NULL)
    {
        UBOOT_ERROR("malloc fail!!\n");
    }
    UBOOT_DEBUG("Dram_Addr = %p \n",Dram_Addr);
    if(Read_TConFromFlash((U32)Dram_Addr)!=0) //read logo from spi/emmc to dram
    {
        UBOOT_ERROR("Error: Read_LogoToDisplayAddr Fail\n");
        return NULL;
    }
    //check VERSION
    TCON_VERSION  = *((unsigned char*) (Dram_Addr ));
    printf("TCON VERSION %d \n",TCON_VERSION);
    UBOOT_TRACE("OK\n");
    return (unsigned char*)Dram_Addr;
}

/***** Tcon  Major header:32 ********
    1. BinaryID:2
    2. CustomerID:2
    3. Checksum:2
    4. DataLength:4
    5. Description:18
    6.  Reserved:4
  ***** Tcon  sub header: 8 *****
    1. 2D Table:1
    2. 3D Table:1
    3. reserver: 6

  ***** each sub table information : 8 *******
  1. Sub-Table Type:1
  2. Register count:2
  3. Register type:1
  4. Register list offset:4
*********************************/
static void _parseAndDump_TCON_bin(MS_U8* Dram_Addr,APIPNL_TCON_TAB_TYPE TconType)
{
    unsigned char  u8TCon2DTableNum;
    unsigned char  u8TableType=0;
    unsigned short u16RegisterCount=0;
    unsigned char  u8RegisterType=0;
    unsigned int   u32RegisterlistOffset=0;
    unsigned int   u32RegisterlistSize=0;
    unsigned char * targetDramBuf = NULL;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("===== TconType = %d =====\n",TconType);
    // Version 1 skip major header 32 byte:
    Dram_Addr += 32;

    u8TCon2DTableNum= *Dram_Addr;
    if(u8TCon2DTableNum == 0)
    {
        UBOOT_ERROR("u8TCon2DTableNum can not be 0\n");
        return;
    }
    // skip sub header 8byte:
    Dram_Addr += 8;
    if(TCON_VERSION==3)
    {
        // Version 2 skip verison infor 40 byte:
        Dram_Addr += 40;
    }

    // get specific Register list offset (each item is 8byte)
    u8TableType           = *((unsigned char*) (Dram_Addr + (TconType * 8) + 0));
    u16RegisterCount      = *((unsigned short*)(Dram_Addr + (TconType * 8) + 1));
    u8RegisterType        = *((unsigned char*) (Dram_Addr + (TconType * 8) + 3));
    u32RegisterlistOffset = *((unsigned int*)  (Dram_Addr + (TconType * 8) + 4));
    u32RegisterlistSize   = u16RegisterCount * _registerTypeToSize(u8RegisterType);
    UBOOT_DEBUG("u8TableType           = %d\n",u8TableType);
    UBOOT_DEBUG("u16RegisterCount      = %d\n",u16RegisterCount);
    UBOOT_DEBUG("u8RegisterType        = %d\n",u8RegisterType);
    UBOOT_DEBUG("u32RegisterlistOffset = 0x%x\n",u32RegisterlistOffset);
    UBOOT_DEBUG("u32RegisterlistSize   = 0x%x\n",u32RegisterlistSize);
    if(TCON_VERSION==3)
    {
        Dram_Addr+=(u32RegisterlistOffset-80);//minus major header 32 & sub header 8 & verison 40
    }
    else if (TCON_VERSION ==1)
    {
        Dram_Addr+=(u32RegisterlistOffset-40);//minus major header 32 & sub header 8
    }
    UBOOT_DEBUG("Dram_Addr+pRegisterlistOffset = %p\n",Dram_Addr);

    //copy the data section to target dram buffer
    //and extra 4byte:0xFFFFFFFF, it's for driver detects the register list end.
    targetDramBuf = malloc(u32RegisterlistSize+4);
    if(targetDramBuf==NULL)
    {
        UBOOT_ERROR("malloc fail!!\n");
        return;
    }
    memset(targetDramBuf,0xFF,u32RegisterlistSize+4);
    memcpy(targetDramBuf,Dram_Addr,u32RegisterlistSize);
    UBOOT_DUMP((unsigned int)targetDramBuf,u32RegisterlistSize+4);

    //dump table to PNL driver
    MApi_PNL_TCONMAP_DumpTable(targetDramBuf,TconType);

    free(targetDramBuf);
    UBOOT_TRACE("OK\n");
    return;
}

static void Init_TCON_Panel(void)
{
    UBOOT_TRACE("IN\n");
    unsigned char *Dram_Addr =NULL;

    MApi_PNL_TCON_Init();
    if(gstDbtable.dbdata[E_DB_TCON].Size!=0)
    {
        Dram_Addr=_load_TCON_from_dbtable();
        _parseAndDump_TCON_bin(Dram_Addr,E_APIPNL_TCON_TAB_TYPE_GENERAL);
        _parseAndDump_TCON_bin(Dram_Addr,E_APIPNL_TCON_TAB_TYPE_GPIO);
        _parseAndDump_TCON_bin(Dram_Addr,E_APIPNL_TCON_TAB_TYPE_SCALER);
        _parseAndDump_TCON_bin(Dram_Addr,E_APIPNL_TCON_TAB_TYPE_MOD);
        if(TCON_VERSION==3)
        {
            _parseAndDump_TCON_bin(Dram_Addr,7); // verison 2 new panel table
        }
        free((void*)Dram_Addr);
    }
    else
    {
        TCON_TAB_INFO* pTcontab = &(TConMAP_Main[eTCONPNL_TypeSel]);
        MApi_PNL_TCONMAP_DumpTable(pTcontab->pTConInitTab, E_APIPNL_TCON_TAB_TYPE_GENERAL);
        MApi_PNL_TCONMAP_DumpTable(pTcontab->pTConInit_GPIOTab, E_APIPNL_TCON_TAB_TYPE_GPIO);
        MApi_PNL_TCONMAP_DumpTable(pTcontab->pTConInit_MODTab, E_APIPNL_TCON_TAB_TYPE_MOD);
        _MApi_TCon_Tab_DumpPSRegTab(pTcontab->pTConPower_Sequence_OnTab);
    }
    UBOOT_TRACE("OK\n");
}
#endif

#if (CONFIG_3D_HWLVDSLRFLAG)
static void PNL_Set3D_HWLVDSLRFlag(void)
{
    MS_PNL_HW_LVDSResInfo lvdsresinfo;
    lvdsresinfo.bEnable = 1;
    lvdsresinfo.u16channel = 0x03; // channel A: BIT0, channel B: BIT1,
    lvdsresinfo.u32pair = 0x18; // pair 0: BIT0, pair 1: BIT1, pair 2: BIT2, pair 3: BIT3, pair 4: BIT4, etc ...

    MApi_PNL_HWLVDSReservedtoLRFlag(lvdsresinfo);
}
#endif

static void MsDrv_SetChannelOrder(MS_U8 chOrderType,pnl_VB1ChannelOrder * channelOrder_data)
{
    MS_U16 u16OutputOrder0_3=0;
    MS_U16 u16OutputOrder4_7=0;
    MS_U16 u16OutputOrder8_11=0;
    MS_U16 u16OutputOrder12_13=0;
    UBOOT_TRACE("IN\n");
    if(chOrderType==VB1_CH_ORDER_16V)
    {
        u16OutputOrder0_3=(channelOrder_data->u8Vb116vOrder[0]<<8)+(channelOrder_data->u8Vb116vOrder[1]);
        u16OutputOrder4_7=(channelOrder_data->u8Vb116vOrder[2]<<8)+(channelOrder_data->u8Vb116vOrder[3]);
        u16OutputOrder8_11=(channelOrder_data->u8Vb116vOrder[4]<<8)+(channelOrder_data->u8Vb116vOrder[5]);
        u16OutputOrder12_13=(channelOrder_data->u8Vb116vOrder[6]<<8)+(channelOrder_data->u8Vb116vOrder[7]);
    }
    else if(chOrderType==VB1_CH_ORDER_8V)
    {
        u16OutputOrder0_3=(channelOrder_data->u8Vb18vOrder[0]<<8)+(channelOrder_data->u8Vb18vOrder[1]);
        u16OutputOrder4_7=(channelOrder_data->u8Vb18vOrder[2]<<8)+(channelOrder_data->u8Vb18vOrder[3]);
        u16OutputOrder8_11=(channelOrder_data->u8Vb18vOrder[4]<<8)+(channelOrder_data->u8Vb18vOrder[5]);
        u16OutputOrder12_13=(channelOrder_data->u8Vb18vOrder[6]<<8)+(channelOrder_data->u8Vb18vOrder[7]);
    }
     else if(chOrderType==VB1_CH_ORDER_4V)
    {
        u16OutputOrder0_3=(channelOrder_data->u8Vb14vOrder[0]<<8)+(channelOrder_data->u8Vb14vOrder[1]);
        u16OutputOrder4_7=(channelOrder_data->u8Vb14vOrder[2]<<8)+(channelOrder_data->u8Vb14vOrder[3]);
        u16OutputOrder8_11=(channelOrder_data->u8Vb14vOrder[4]<<8)+(channelOrder_data->u8Vb14vOrder[5]);
        u16OutputOrder12_13=(channelOrder_data->u8Vb14vOrder[6]<<8)+(channelOrder_data->u8Vb14vOrder[7]);
    }
      else if(chOrderType==VB1_CH_ORDER_2V)
    {
        u16OutputOrder0_3=(channelOrder_data->u8Vb12vOrder[0]<<8)+(channelOrder_data->u8Vb12vOrder[1]);
        u16OutputOrder4_7=(channelOrder_data->u8Vb12vOrder[2]<<8)+(channelOrder_data->u8Vb12vOrder[3]);
        u16OutputOrder8_11=(channelOrder_data->u8Vb12vOrder[4]<<8)+(channelOrder_data->u8Vb12vOrder[5]);
        u16OutputOrder12_13=(channelOrder_data->u8Vb12vOrder[6]<<8)+(channelOrder_data->u8Vb12vOrder[7]);
    }
      else if(chOrderType==VB1_CH_ORDER_1V)
    {
        u16OutputOrder0_3=(channelOrder_data->u8Vb11vOrder[0]<<8)+(channelOrder_data->u8Vb11vOrder[1]);
        u16OutputOrder4_7=(channelOrder_data->u8Vb11vOrder[2]<<8)+(channelOrder_data->u8Vb11vOrder[3]);
        u16OutputOrder8_11=(channelOrder_data->u8Vb11vOrder[4]<<8)+(channelOrder_data->u8Vb11vOrder[5]);
        u16OutputOrder12_13=(channelOrder_data->u8Vb11vOrder[6]<<8)+(channelOrder_data->u8Vb11vOrder[7]);
    }
    if((u16OutputOrder0_3+u16OutputOrder4_7+u16OutputOrder8_11+u16OutputOrder12_13)!=0)
    {
        MApi_PNL_MOD_OutputChannelOrder(APIPNL_OUTPUT_CHANNEL_ORDER_USER,u16OutputOrder0_3,u16OutputOrder4_7,u16OutputOrder8_11,u16OutputOrder12_13);
    }
    else
    {
        UBOOT_INFO("MOD Output Channel Order not set!!!!\n");
    }
    UBOOT_TRACE("OK\n");
}

MS_BOOL MsDrv_PNL_Init(PanelType*  panel_data)
{
    MS_BOOL ret=TRUE;
    char* s=NULL;
    MS_U16 u16PanelDCLK=0;
    UBOOT_TRACE("IN\n");
/* For Maserati 120hz with LG panel*/
    st_board_para boardpara;

    UNUSED(s);
    UNUSED(u16PanelDCLK);
    MApi_PNL_PreInit(E_PNL_NO_OUTPUT);
    MApi_PNL_SkipTimingChange(FALSE);
#if  (CONFIG_TV_CHIP==1)
    st_sys_misc_setting misc_setting;
    Read_MiscSetting_ToFlash(&misc_setting);

    if (panel_data->m_ePanelLinkType == LINK_TTL)
    {
        UBOOT_DEBUG("Panle Link Type=LINK_TTL \n");
        MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_TTL_CFG0_7, PANEL_LINK_TTL_CFG8_15, PANEL_LINK_TTL_CFG16_21);
    }
    else if(panel_data->m_ePanelLinkType == LINK_LVDS)
    {
        MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_LVDS_CFG0_7, PANEL_LINK_LVDS_CFG8_15, PANEL_LINK_LVDS_CFG16_21);
        UBOOT_DEBUG("Panle Link Type=LINK_LVDS \n");
    }
    else if(panel_data->m_ePanelLinkType == LINK_EXT)
    {
        UBOOT_DEBUG("Panle Link Type=LINK_EXT \n");
        UBOOT_DEBUG("Link Ext Type=%u \n", getLinkExtType());
        MApi_PNL_SetLPLLTypeExt(getLinkExtType());
        u16PanelDCLK = misc_setting.m_u16PanelDCLK;
        MApi_PNL_ForceSetPanelDCLK(u16PanelDCLK,TRUE);
        //Read VB1 CH Order from Customer ini
        pnl_VB1ChannelOrder  channelOrder_data;
        Read_PnlVb1FromFlash(&channelOrder_data);
        if((LINK_VBY1_10BIT_8LANE==getLinkExtType())||(LINK_VBY1_8BIT_8LANE==getLinkExtType()))
        {
            MsDrv_SetChannelOrder(VB1_CH_ORDER_8V,&channelOrder_data);
            MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_EXT_8LANE_CFG0_7, PANEL_LINK_EXT_8LANE_CFG8_15, PANEL_LINK_EXT_8LANE_CFG16_21);
            UBOOT_DEBUG("VB1 Link Type=LINK_VBY1_10/8BIT_8LANE, u16PanelDCLK = %u\n", u16PanelDCLK);
        }
        else  if((LINK_VBY1_8BIT_16LANE==getLinkExtType())||(LINK_VBY1_10BIT_16LANE==getLinkExtType()))
        {
            MsDrv_SetChannelOrder(VB1_CH_ORDER_16V,&channelOrder_data);
            MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_EXT_16LANE_CFG0_7, PANEL_LINK_EXT_16LANE_CFG8_15, PANEL_LINK_EXT_16LANE_CFG16_21);
            UBOOT_DEBUG("VB1 Link Type=LINK_VBY1_10/8BIT_16LANE, u16PanelDCLK = %u\n", u16PanelDCLK);
        }
        else if((LINK_VBY1_10BIT_4LANE==getLinkExtType())||(LINK_VBY1_8BIT_4LANE==getLinkExtType()))
        {
            MsDrv_SetChannelOrder(VB1_CH_ORDER_4V,&channelOrder_data);
            MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_EXT_4LANE_CFG0_7, PANEL_LINK_EXT_4LANE_CFG8_15, PANEL_LINK_EXT_4LANE_CFG16_21);
            UBOOT_DEBUG("VB1 Link Type=LINK_VBY1_10/8BIT_4LANE, u16PanelDCLK = %u\n", u16PanelDCLK);
        }
        else if((LINK_VBY1_10BIT_2LANE==getLinkExtType())||(LINK_VBY1_8BIT_2LANE==getLinkExtType()))
        {
            MsDrv_SetChannelOrder(VB1_CH_ORDER_2V,&channelOrder_data);
            MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_EXT_2LANE_CFG0_7, PANEL_LINK_EXT_2LANE_CFG8_15, PANEL_LINK_EXT_2LANE_CFG16_21);
            UBOOT_DEBUG("VB1 Link Type=LINK_VBY1_10/8BIT_2LANE, u16PanelDCLK = %u\n", u16PanelDCLK);
        }
        else if((LINK_VBY1_10BIT_1LANE==getLinkExtType())||(LINK_VBY1_8BIT_1LANE==getLinkExtType()))
        {
            MsDrv_SetChannelOrder(VB1_CH_ORDER_1V,&channelOrder_data);
            MApi_PNL_MOD_OutputConfig_User(PANEL_LINK_EXT_1LANE_CFG0_7, PANEL_LINK_EXT_1LANE_CFG8_15, PANEL_LINK_EXT_1LANE_CFG16_21);
            UBOOT_DEBUG("VB1 Link Type=LINK_VBY1_10/8BIT_1LANE, u16PanelDCLK = %u\n", u16PanelDCLK);
        }
        else
        {
            UBOOT_ERROR("VB1 Link Type NA !!!!\n");
        }
    }
    else if(panel_data->m_ePanelLinkType == LINK_DAC_P)
    {
        u16PanelDCLK = misc_setting.m_u16PanelDCLK;
        MApi_PNL_ForceSetPanelDCLK(u16PanelDCLK,TRUE);
    }
    if (misc_setting.m_u8PixelShiftEnable)
    {
        UBOOT_DEBUG("Enable PixelShift\n");
        MApi_PNL_Init_MISC(E_APIPNL_MISC_PIXELSHIFT_ENABLE);
    }
#endif
    char  *pTconPanelEnv = NULL;
    if (((pTconPanelEnv = getenv("enable_tcon_panel")) != NULL) && (strcmp(pTconPanelEnv,"1") == 0))
    {
        MApi_PNL_Init_MISC(E_APIPNL_MISC_ENABLE_MANUAL_VSYNC_CTRL);
    }
#if (ENABLE_HDMITX_MSTAR_ROCKET==1)
    char * p_str = NULL;
    int u8ResolutionEnv = -1;
    MApi_PNL_MOD_OutputConfig_User(0x1550, 0x0155, 0x0000);

    MApi_PNL_Init_MISC(E_APIPNL_MISC_SKIP_T3D_CONTROL|E_APIPNL_MISC_ENABLE_MANUAL_VSYNC_CTRL);
    p_str = getenv ("resolution");
    if(NULL != p_str)
    {
        u8ResolutionEnv = (int)simple_strtol(p_str, NULL, 10);
        UBOOT_DEBUG("ROCKET Link Type=%d \n",u8ResolutionEnv);
        if(u8ResolutionEnv>=HDMITX_RES_3840x2160p_24Hz  &&  u8ResolutionEnv<HDMITX_RES_MAX)
        {
            MApi_PNL_SetLPLLTypeExt(getLinkExtType());
            MApi_PNL_PreInit(E_PNL_CLK_DATA);
            MApi_PNL_ForceSetPanelDCLK((panel_data->m_dwPanelDCLK *2),TRUE);
            UBOOT_DEBUG("ROCKET Link Type=E_HAL_LTH_OUTPUT_4K2K \n");
        }
    }
#elif (ENABLE_HDMITX_MSTAR_ROCKET2==1)
	MApi_PNL_Init_MISC(E_APIPNL_MISC_ENABLE_MANUAL_VSYNC_CTRL);
#endif
#if  (CONFIG_TV_CHIP==1)
    MApi_PNL_On(panel_data->m_wPanelOnTiming1);//Power on TCON and Delay Timming1
#endif
/* For Maserati 120hz with LG panel*/
	//load board para
#if !defined(CONFIG_STATIC_PANEL_PARA)
    if(Read_BoardParaFromflash(&boardpara)==0)
    {
	        UBOOT_DEBUG("\nRead_BoardParaFromflash:GPIO1_PAD_NAME %s\n",boardpara.m_sGPIO1_PAD_NAME);
		UBOOT_DEBUG("Read_BoardParaFromflash:GPIO2_PAD_NAME %s\n",boardpara.m_sGPIO2_PAD_NAME);
		if ((strlen(boardpara.m_sGPIO1_PAD_NAME) != 0) && (strlen(boardpara.m_sGPIO2_PAD_NAME) != 0))
		{
			printf("\n%s:SetDivison \n", __FUNCTION__);
			SetDivison(boardpara.m_u16GPIO1_INDEX,boardpara.m_u8GPIO1_VALUE);
			SetDivison(boardpara.m_u16GPIO2_INDEX,boardpara.m_u8GPIO2_VALUE);
		}
		else
		{
			printf("\n%s:Bypass SetDivison \n", __FUNCTION__);
		}
   }
   else
   {
		UBOOT_DEBUG("%s: Read_BoardParaFromflash() failed, at %d\n", __func__, __LINE__);
   }
#endif
	MApi_PNL_Init(panel_data);
        g_IPanel.Dump();
        g_IPanel.SetGammaTbl(E_APIPNL_GAMMA_12BIT, tAllGammaTab, GAMMA_MAPPING_MODE);
#if(ENABLE_URSA_6M30 == 1)
        if(PANEL_4K2K_ENABLE == 1)
        {
            MDrv_WriteByte(0x1032B5, 0xF0);  //LR Flag Send From LVDS Dummy Pixel
        }

        if(bMst6m30Installed)
        {
            ret=MApi_PNL_PreInit(E_PNL_CLK_DATA);
            MApi_PNL_SetOutput(E_APIPNL_OUTPUT_CLK_DATA);
            g_IPanel.Enable(TRUE);
            MApi_PNL_On(panel_data->m_wPanelOnTiming1);//Power on TCON and Delay Timming1
            MDrv_Ursa_6M30_LVDS_Enalbe(TRUE);//run_command("ursa_lvds_on", 0);
            PWM_init();
            MApi_PNL_On(panel_data->m_wPanelOnTiming2);//Delay Timming2
            lPanelOnTiming=panel_data->m_wPanelOnTiming2;
            lPanleTimer=MsSystemGetBootTime();
        }
        else
#endif
        {
        #if  (CONFIG_TV_CHIP==1)
            #if defined (CONFIG_URSA7_VB1) && (CONFIG_URSA7_VB1 == 1)
                MDrv_Ursa_7_Set_2_lane_VB1_per_init();
            #endif

            ret=MApi_PNL_PreInit(E_PNL_CLK_DATA);
            #if (CONFIG_URSA_UNION)
                if (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_KS2)
                {
                     MDrv_KS2_Panel_Unlock();
                }
            #endif

            #if defined(CONFIG_NOVA_KS2)
                MDrv_KS2_Panel_Unlock();
                if(panel_data->m_wPanelWidth == 3840 && panel_data->m_wPanelHeight == 2160)
                {
                    MDrv_KS2_Panel_Bootlogo(1);
                }
                else
                {
                    MDrv_KS2_Panel_Bootlogo(0);
                    MDrv_KS2_Panel_AutoMuteMode();
                }
            #endif
        #if(CONFIG_ENABLE_V_I_PWM == 1)
            // wuyian for pwm0 pwm2
            printf("->I-PWM init; ->V-PWM init begin!\n");
            V_I_PWM_init("I-PWM");
            V_I_PWM_init("V-PWM");
            printf("->I-PWM init; ->V-PWM init end!\n");
        #endif
#if !defined(CONFIG_STATIC_PANEL_PARA)
            PWM_init();
#endif

#if (CONFIG_URSA6_VB1 == 1 || CONFIG_URSA9_VB1 == 1 ) // Fix handshake issue, mantis 0795280.
            MApi_PNL_SetVideoHWTraining(FALSE); // Disable HW training.
#endif
            MApi_PNL_En(TRUE, panel_data->m_wPanelOnTiming2);//Delay Timming2
            lPanelOnTiming=panel_data->m_wPanelOnTiming2;
            lPanleTimer=MsSystemGetBootTime();
#endif

#if (!CONFIG_UBOOT_MINI_LIB)
            if (((pTconPanelEnv = getenv("enable_tcon_panel")) != NULL) && (strcmp(pTconPanelEnv,"1") == 0))
            {
                if(MApi_XC_Init(NULL, 0) == TRUE)
                {
                    //Fix back porch for TCON
                    XC_PANEL_INFO_EX stPanelInfoEx;
                    memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
                    stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
                    stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);
                    stPanelInfoEx.bVSyncBackPorchValid = TRUE;
                    stPanelInfoEx.u16VSyncBackPorch = panel_data->m_ucPanelVBackPorch; //set back porch value
                    stPanelInfoEx.u16VFreq = 500; //this step setting info is only for 50hz
                    if(MApi_XC_SetExPanelInfo(TRUE, &stPanelInfoEx))//Check the set is accepted or not
                        UBOOT_DEBUG("---%s:%d Set ExPanel Info OK\n", __FUNCTION__, __LINE__);
                    else
                        UBOOT_ERROR("---%s:%d Set ExPanel Info Fail\n", __FUNCTION__, __LINE__);

                    stPanelInfoEx.u16VFreq = 600; //set same setting for 60hz
                    if(MApi_XC_SetExPanelInfo(TRUE, &stPanelInfoEx))//Check the set is accepted or not
                        UBOOT_DEBUG("---%s:%d Set ExPanel Info OK\n", __FUNCTION__, __LINE__);
                    else
                        UBOOT_ERROR("---%s:%d Set ExPanel Info Fail\n", __FUNCTION__, __LINE__);

                    Init_TCON_Panel();
                }
                else
                {
                    UBOOT_DEBUG("xc Init failed....\n");
                }
            }
#endif
        }


    #if (CONFIG_3D_HWLVDSLRFLAG)
    PNL_Set3D_HWLVDSLRFlag();
    #endif
#if defined (MASERATI_PANEL_SW_PATCH)
    WriteRegBit(0x111EEE,BIT1,BIT1); //Lockn and HTPDn use ch12
    Write2Byte(0x111E04,0x0003); //vb1 ch0 ch1
#endif

#if defined (CONFIG_ENABLE_DEMURA)
    char  *pDemuraEnv = getenv("enable_demura");
    if ((pDemuraEnv != NULL) && (strcmp(pDemuraEnv, "1") == 0))
    {
        MApi_Demura_Init(panel_data);
    }
#endif

    UBOOT_TRACE("OK\n");

#if (CONFIG_XC_FRC_VB1==1)
 if((LINK_VBY1_8BIT_16LANE==getLinkExtType())||(LINK_VBY1_10BIT_16LANE==getLinkExtType()))
    {
        MDrv_XC_Sys_Init_XC();
    }
#endif

#if defined (CONFIG_URSA7_VB1) && (CONFIG_URSA7_VB1 == 1)
    MDrv_Ursa_7_Set_2_lane_VB1();
#endif

	return ret;

}

void MsDrv_PNL_BackLigth_On(void)
{
	UBOOT_TRACE("IN\n");
	unsigned long lpanelDelayTime=0;
	unsigned long ltotalTime=MsSystemGetBootTime();

	if(lPanelOnTiming>(ltotalTime-lPanleTimer))
	{
		lpanelDelayTime=(lPanelOnTiming-(ltotalTime-lPanleTimer));

		mdelay(lpanelDelayTime);
		printf("\n---%s:%d Set MsDrv_PNL_BackLigth_On DelayTask %lu \n", __FUNCTION__, __LINE__,lpanelDelayTime);
	}
	else
	{
		UBOOT_DEBUG("\n---%s:%d Set MsDrv_PNL_BackLigth_On No Delay \n", __FUNCTION__, __LINE__);

	}
	if(FALSE == pm_check_back_ground_active())
	{
		MApi_PNL_SetBackLight(BACKLITE_INIT_SETTING);//Power on backlight
	}
	else
	{
		MApi_PNL_SetBackLight(DISABLE);
	}

	UBOOT_TRACE("OK\n");

}

void setLinkExtType(APIPNL_LINK_EXT_TYPE linkExtType)
{
    PanelLinkExtType=linkExtType;
}
APIPNL_LINK_EXT_TYPE getLinkExtType(void)
{
    return PanelLinkExtType;
}


#if (CONFIG_XC_FRC_VB1==1)

int MDrv_XC_Sys_Init_XC(void)
{
    XC_INITDATA sXC_InitData;
    XC_INITDATA *pstXC_InitData= &sXC_InitData;
    XC_INITMISC sXC_Init_Misc;
    U32 u32Addr=0,u32Size=0;


    // reset to zero

    memset(&sXC_InitData, 0, sizeof(sXC_InitData));
    memset(&sXC_Init_Misc, 0, sizeof(XC_INITMISC));

    // Init XC
#if 1// (OBA2!=1) // remove when XC driver update
    // Check library version. Do not modify this statement please.
    pstXC_InitData->u32XC_version = XC_INITDATA_VERSION;
#endif
    pstXC_InitData->u32XTAL_Clock = MST_XTAL_CLOCK_HZ;
#if 1


    if(get_addr_from_mmap("E_MMAP_ID_XC_MAIN_FB", &u32Addr)!=0)
    {
        printf("get E_MMAP_ID_XC_MAIN_FB mmap fail, it may CAM mmap , set xc addr 0 size 0 \n");
        u32Addr=0;
        u32Size=0;
    }
    else
   {
        get_length_from_mmap("E_MMAP_ID_XC_MAIN_FB", (U32 *)&u32Size);
    }

    pstXC_InitData->u32Main_FB_Size = u32Size; //SCALER_DNR_BUF_LEN;
    pstXC_InitData->u32Main_FB_Start_Addr = u32Addr;//((SCALER_DNR_BUF_MEMORY_TYPE & MIU1) ? (SCALER_DNR_BUF_ADR | MIU_INTERVAL) : (SCALER_DNR_BUF_ADR));

    // Init DNR Address in Main & Sub channel. Keep the same. If project support FB PIP mode, set Sub DNR Address in AP layer (eg. mapp_init).
    pstXC_InitData->u32Sub_FB_Size = pstXC_InitData->u32Main_FB_Size;
    pstXC_InitData->u32Sub_FB_Start_Addr = pstXC_InitData->u32Main_FB_Start_Addr;

    // Chip related.
    pstXC_InitData->bIsShareGround = ENABLE;
#endif

#if 1 // (OBA2!=1) // remove when XC driver update
    // Board related
    pstXC_InitData->eScartIDPort_Sel = SCART_ID_SEL;//SCART_ID_SEL | SCART2_ID_SEL ;
#endif
    pstXC_InitData->bCEC_Use_Interrupt = FALSE;

    pstXC_InitData->bEnableIPAutoCoast = 0;

    pstXC_InitData->bMirror = FALSE;


    // panel info
    pstXC_InitData->stPanelInfo.u16HStart = g_IPanel.HStart();      // DE H start
    pstXC_InitData->stPanelInfo.u16VStart = g_IPanel.VStart();
    pstXC_InitData->stPanelInfo.u16Width  = g_IPanel.Width();
    pstXC_InitData->stPanelInfo.u16Height = g_IPanel.Height();
    pstXC_InitData->stPanelInfo.u16HTotal = g_IPanel.HTotal();
    pstXC_InitData->stPanelInfo.u16VTotal = g_IPanel.VTotal();

    pstXC_InitData->stPanelInfo.u16DefaultVFreq = g_IPanel.DefaultVFreq();

    pstXC_InitData->stPanelInfo.u8LPLL_Mode = g_IPanel.LPLL_Mode();
    pstXC_InitData->stPanelInfo.enPnl_Out_Timing_Mode = (E_XC_PNL_OUT_TIMING_MODE)(g_IPanel.OutTimingMode());

    pstXC_InitData->stPanelInfo.u16DefaultHTotal = g_IPanel.HTotal();
    pstXC_InitData->stPanelInfo.u16DefaultVTotal = g_IPanel.VTotal();
    pstXC_InitData->stPanelInfo.u32MinSET = g_IPanel.MinSET();
    pstXC_InitData->stPanelInfo.u32MaxSET = g_IPanel.MaxSET();
    pstXC_InitData->stPanelInfo.eLPLL_Type = (E_XC_PNL_LPLL_TYPE) g_IPanel.LPLL_Type();
    //printf("%s, %d, pstXC_InitData->stPanelInfo.eLPLL_Type=%u\n", __FUNCTION__, __LINE__, pstXC_InitData->stPanelInfo.eLPLL_Type);
    pstXC_InitData->bDLC_Histogram_From_VBlank = FALSE;

    if (  MApi_XC_GetCapability(E_XC_SUPPORT_IMMESWITCH)  )
    {
        sXC_Init_Misc.u32MISC_A |= E_XC_INIT_MISC_A_IMMESWITCH;
    }

    if ( MApi_XC_GetCapability(E_XC_SUPPORT_FRC_INSIDE) && 1 )
    {
        sXC_Init_Misc.u32MISC_A |= E_XC_INIT_MISC_A_FRC_INSIDE;
        sXC_Init_Misc.u32MISC_A |= E_XC_INIT_MISC_A_FRC_DUAL_MIU;
    }
// TODO: =====Start Temp function=====//After Interface/Struct definition are fixed, remove these!!

    if(sXC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
    {
        XC_PREINIT_INFO_t stXC_PANEL_INFO_ADV;
        memset(&stXC_PANEL_INFO_ADV, 0, sizeof(XC_PREINIT_INFO_t));

        stXC_PANEL_INFO_ADV.u8PanelHSyncWidth      = g_IPanel.HSynWidth();
        stXC_PANEL_INFO_ADV.u8PanelHSyncBackPorch  = g_IPanel.HSynBackPorch();
        stXC_PANEL_INFO_ADV.u8PanelVSyncWidth      = MApi_GetPanelSpec(MApi_PNL_GetPnlTypeSetting())->m_ucPanelVSyncWidth;
        stXC_PANEL_INFO_ADV.u8PanelVSyncBackPorch  = g_IPanel.VSynBackPorch();
        stXC_PANEL_INFO_ADV.u16VTrigX              = 0x82F;
            stXC_PANEL_INFO_ADV.u16VTrigY              = (g_IPanel.VStart()+g_IPanel.Height()+12)%(MApi_GetPanelSpec(MApi_PNL_GetPnlTypeSetting())->m_wPanelVTotal);//0x45B;

        // �ڧ�Memory�������ª�code����, �s�����u�ݭn�@��memory
        if(get_addr_from_mmap("E_MMAP_ID_XC_FRC_L", &u32Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_FRC_L mmap fail\n");
            return -1;
        }
        get_length_from_mmap("E_MMAP_ID_XC_FRC_L", (U32 *)&u32Size);

        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_MEMC_MemAddr    = u32Addr;  //?�o�O��FRC�һݪ�memory
        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_MEMC_MemSize    = u32Size;
        if(get_addr_from_mmap("E_MMAP_ID_XC_FRC_R", &u32Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_FRC_R mmap fail\n");
            return -1;
        }
        get_length_from_mmap("E_MMAP_ID_XC_FRC_R", (U32 *)&u32Size);

        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_R2_MemAddr    = u32Addr;
        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_R2_MemSize    = u32Size;

        if(get_addr_from_mmap("E_MMAP_ID_XC_FRC_PQ", &u32Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_FRC_PQ mmap fail\n");
            return -1;
        }
        get_length_from_mmap("E_MMAP_ID_XC_FRC_PQ", (U32 *)&u32Size);

        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_ME1_MemAddr    = u32Addr;
        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_ME1_MemSize    = u32Size;

        stXC_PANEL_INFO_ADV.FRCInfo.u16FB_YcountLinePitch  = 0x00;
        stXC_PANEL_INFO_ADV.FRCInfo.u16PanelWidth          = g_IPanel.Width();
        stXC_PANEL_INFO_ADV.FRCInfo.u16PanelHeigh          = g_IPanel.Height();
        stXC_PANEL_INFO_ADV.FRCInfo.u8FRC3DPanelType       = E_XC_3D_PANEL_NONE;

        stXC_PANEL_INFO_ADV.FRCInfo.bFRC                   = TRUE; // TRUE: Normal; FALSE: Bypass
        stXC_PANEL_INFO_ADV.FRCInfo.u83Dmode               = 0x00;
        stXC_PANEL_INFO_ADV.FRCInfo.u8IpMode               = 0x00;
        stXC_PANEL_INFO_ADV.FRCInfo.u8MirrorMode           = 0x00;
        stXC_PANEL_INFO_ADV.FRCInfo.u32FRC_FrameSize       = 0;
        stXC_PANEL_INFO_ADV.FRCInfo.u83D_FI_out            = 0;

#if defined(CONFIG_SUPPORT_64BIT_PA)
        U64 u64Addr=0;
        XC_FRC_ADDRESS_INFO stXC_FRC_64BITS_ADDRESS_INFO;
        memset(&stXC_FRC_64BITS_ADDRESS_INFO, 0, sizeof(XC_FRC_ADDRESS_INFO));

        if(get_64bit_addr_from_mmap("E_MMAP_ID_XC_FRC_L", &u64Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_FRC_L mmap fail\n");
            return -1;
        }
        get_length_from_mmap("E_MMAP_ID_XC_FRC_L", (U32 *)&u32Size);
        stXC_FRC_64BITS_ADDRESS_INFO.u64FRC_IPM_L_MemAddr = u64Addr;
        stXC_FRC_64BITS_ADDRESS_INFO.u64FRC_IPM_L_MemSize = u32Size;

        if(get_64bit_addr_from_mmap("E_MMAP_ID_XC_FRC_R", &u64Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_FRC_R mmap fail\n");
            return -1;
        }
        get_length_from_mmap("E_MMAP_ID_XC_FRC_R", (U32 *)&u32Size);
        stXC_FRC_64BITS_ADDRESS_INFO.u64FRC_IPM_R_MemAddr    = u64Addr;
        stXC_FRC_64BITS_ADDRESS_INFO.u64FRC_IPM_R_MemSize    = u32Size;

        if(get_64bit_addr_from_mmap("E_MMAP_ID_XC_FRC_PQ", &u64Addr)!=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_FRC_PQ mmap fail\n");
            return -1;
        }
        get_length_from_mmap("E_MMAP_ID_XC_FRC_PQ", (U32 *)&u32Size);
        stXC_FRC_64BITS_ADDRESS_INFO.u64FRC_PQ_MemAddr    = u64Addr;
        stXC_FRC_64BITS_ADDRESS_INFO.u64FRC_PQ_MemSize    = u32Size;

        MApi_XC_PreInit(E_XC_PREINIT_FRC_ADDRESS, & stXC_FRC_64BITS_ADDRESS_INFO, sizeof(XC_FRC_ADDRESS_INFO));
#endif

        MApi_XC_PreInit( E_XC_PREINIT_FRC, &stXC_PANEL_INFO_ADV, sizeof(XC_PREINIT_INFO_t) );
        pstXC_InitData->stPanelInfo.u8LPLL_Mode = 2; // Quad mode

    }
// TODO: =====End of Temp Function=====


    if(MApi_XC_Init(pstXC_InitData, sizeof(XC_INITDATA)) == FALSE)
    {
        printf("L:%d, XC_Init failed because of InitData wrong, please update header file and compile again\n", __LINE__);
    }

#if 1

    if (is_str_resume() == 1)
    {
       MApi_XC_FRC_Mute(TRUE);
    }

    if(MApi_XC_Init_MISC(&sXC_Init_Misc, sizeof(XC_INITMISC)) == FALSE)
    {
        printf("L:%d, XC Init MISC failed because of InitData wrong, please update header file and compile again\n", __LINE__);
    }

    // Init Gamma table
#endif
    return 1;

}
#endif

#endif

