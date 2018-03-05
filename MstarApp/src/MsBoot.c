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
#include <exports.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsBoot.h>
#include <MsUpgrade.h>
#include <uboot_mmap.h>
#include <MsDebug.h>
#if defined (CONFIG_URSA_6M40) && defined(CONFIG_SET_4K2K_MODE)
#include <CusSystem.h>
#endif

#include <MsRawIO.h>
#include <MsSystem.h>
#include <MsMmap.h>
#include <MsApiSpi.h>
#include <drvWDT.h>
#include <stdio.h>
#if defined (CONFIG_SECURITY_BOOT)
#include <secure/MsSecureBoot.h>
#include <crypto_auth.h>
#endif

#include <MsSystem.h>
#include <MsEnvironment.h>

#include <msAPI_Power.h>
#include <MsUtility.h>
#include <CusConfig.h>
#include <MsSysUtility.h>
#if defined (CONFIG_SECURITY_BOOT)
#include <MsSetupTee.h>
#endif
#if (CONFIG_URSA_UNION)
#include <ursa/ursa_common.h>
#endif
#include "drvBDMA.h" //for bdma copy fw from flash to dram
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
#if(ENABLE_MMC==1)
#define FLASH_RAW_DATA_TARGET E_RAW_DATA_IN_MMC
#elif (ENABLE_NAND_FLASH==1)
#define FLASH_RAW_DATA_TARGET E_RAW_DATA_IN_NAND
#else
#error
#endif

#endif

#define ENV_UPGRADE_COMPLETE        "MstarUpgrade_complete"
#define ENV_UPGRADE_MODE            "upgrade_mode"
#define ENV_FORCE_UPGRADE           "force_upgrade"

#define ENV_BOOTto2SubSystem               "SubSystem"
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

#if defined (CONFIG_SEC_SYSTEM)
int ENABLE_SECOND_SYSTEM;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static EN_BOOT_MODE stBootMode = EN_BOOT_MODE_UNKNOWN;

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern void Chip_Flush_Memory(void);
extern void del_bootargs_CMA(MS_BOOL bDontSaveEnv);
//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static void _RecoveryDrvInit(void);
static void setBootToRecovery(void);
#if (ENABLE_MODULE_NETUPDATE == 1)
static void _do_NetUpgrade_mode(void);
#endif

//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------

int boot_mode_recovery(void)
{
    int ret = -1;
    char *s = NULL;
    char cmd[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    setBootToRecovery();
    _RecoveryDrvInit();

    if(getenv("E_MMAP_ID_RECOVERY_BUFFER_ADR")!=NULL)
    {
        del_bootargs_CMA(1);
    }

    s = getenv("recoverycmd");
    if(s == NULL)
    {
        snprintf(cmd, sizeof(cmd)-1, BOOT_COMMAND);
    }
    else
    {
        snprintf(cmd, sizeof(cmd)-1, s);
    }

#if defined (CONFIG_URSA_6M40) && defined(CONFIG_SET_4K2K_MODE)
    Set_4K2K_OP0();
#endif
    ret = run_command(cmd, 0);
    if(ret!=0)
    {
        UBOOT_ERROR("recovery command fail!!\n");
    }
    else
    {
        UBOOT_TRACE("OK\n");
    }
    return ret;
}

#define ENABLE_MODULE_ENV_BOOT 1
#if (ENABLE_MODULE_ENV_BOOT == 1)
EN_BOOT_MODE get_boot_mode_from_env(void)
{
    char *buffer = NULL;
    char ret = EN_BOOT_MODE_NORMAL;
    char *cUpgradeComplete = getenv(ENV_UPGRADE_COMPLETE);
    char *cUpgradeMode = getenv(ENV_UPGRADE_MODE);
    ulong ulComplete = 0;
    UBOOT_TRACE("IN\n");

#if defined(CONFIG_AN_FASTBOOT_ENABLE)
    // for fastboot reboot-bootloader command
    char *sFastboot = NULL;
    sFastboot = getenv("reboot-bootloader");
    if ((sFastboot) && (sFastboot[0] == '1')) {
       run_command("setenv reboot-bootloader 0", 0);
       run_command("saveenv", 0);
       ret = EN_BOOT_MODE_FASTBOOT;
       UBOOT_TRACE("OK\n");
       return ret;
    }
#endif

    buffer = (char*) malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }

    if(cUpgradeMode == NULL)
    {
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "setenv %s null;saveenv", ENV_UPGRADE_MODE);
        if(run_command(buffer, 0)!=0)
        {
          UBOOT_ERROR("cmd[%s] fail!!\n",buffer);
        }
    }

    if(cUpgradeComplete == NULL)
    {
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "setenv %s 0;saveenv", ENV_UPGRADE_COMPLETE);
        if(run_command(buffer, 0)!=0)
        {
          UBOOT_ERROR("cmd[%s] fail!!\n",buffer);
        }
    }

    if(cUpgradeComplete != NULL)
    ulComplete = simple_strtoul (cUpgradeComplete, NULL, 10);

    if(ulComplete == 0)
    {
        if(cUpgradeMode == NULL)
        {
            ret = EN_BOOT_MODE_NORMAL;
        }
        else if(strncmp(cUpgradeMode, "usb",3) == 0)
        {
            ret = EN_BOOT_MODE_USB_UPGRADE;
        }
#if CONFIG_MINIUBOOT
#else
        else if(strncmp(cUpgradeMode, "oad",3) == 0)
        {
            ret = EN_BOOT_MODE_OAD_UPGRADE;
        }
        else if(strncmp(cUpgradeMode, "net",3) == 0)
        {
            ret = EN_BOOT_MODE_NET_UPGRADE;
        }
#endif
#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
        else if(strncmp(cUpgradeMode, "env",3) == 0)
        {
            ret = EN_BOOT_MODE_ENV_UPGRADE;
        }
#endif
    }
    else
    {
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "setenv %s 0", ENV_UPGRADE_COMPLETE);
        run_command(buffer, 0);  // run_command("setenv MstarUpgrade_complete 0");
        run_command("saveenv", 0);
        UBOOT_DEBUG("last upgrade complete\n");
    }
    free(buffer);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

#if (ENABLE_MODULE_BOOT_IR == 1)
#if(ENABLE_IR)
EN_BOOT_MODE get_boot_mode_from_ir(void)
{
    U8 IRKey=0xFF;
    U8 IRFlag;
#if CONFIG_RESCUE_ENV && CONFIG_RESCUE_ENV_IR_TRIGGER
    U8 u8IRKeyPrev=0, u8KeyCheckCnt=0;
    U32 u32FirstTime = 0;
    const U32 BRICK_TERMINATOR_IR_TIMEOUT = 3000;
    const U8 BRICK_TERMINATOR_IR_CHECK_CNT = IR_BRICK_TERMINATOR_RECOVERY_KEY_REPEAT_REQUIRED;
#endif
    EN_BOOT_MODE mode = EN_BOOT_MODE_UNKNOWN;
    UBOOT_TRACE("IN\n");
    //IRKey = MDrv_ReadByte(0x3DA9); // get IR code
    extern BOOLEAN msIR_GetIRKeyCode(U8 *pkey, U8 *pu8flag);
    msIR_GetIRKeyCode(&IRKey,&IRFlag);
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
    // check for 1 second
    int iTmp = 0;
    while ((iTmp++ < 200) && (IRKey==0xFF))
    {
        msIR_GetIRKeyCode(&IRKey,&IRFlag);
        MsOS_DelayTask(5);//5ms
    }
#endif

#if CONFIG_RESCUE_ENV && CONFIG_RESCUE_ENV_IR_TRIGGER
    u8IRKeyPrev = IRKey;
    u32FirstTime = MsOS_GetSystemTime();
#endif
    printf("fore uup IRKey [0x%x]\n",IRKey);
    switch(IRKey) // NOTE: read IR Key to decide what boot mode to go
    {
#if CONFIG_RESCUE_ENV && CONFIG_RESCUE_ENV_IR_TRIGGER
        case IR_BRICK_TERMINATOR_RECOVERY_KEY1:
        case IR_BRICK_TERMINATOR_RECOVERY_KEY2:
            {
                while(1)
                {
                    u32FirstTime = MsOS_GetSystemTime();
                    msIR_GetIRKeyCode(&IRKey,&IRFlag);
                    if((0xFF!=IRKey) && (u8IRKeyPrev!=IRKey))
                    {
                        printf("fore uup IRKey [0x%x] cnt: %u, at %d\n", IRKey, u8KeyCheckCnt, __LINE__);
                        u8KeyCheckCnt+=1;
                        if(BRICK_TERMINATOR_IR_CHECK_CNT <= u8KeyCheckCnt)
                        {
                            mode = EN_BOOT_MODE_BRICK_TERMINATOR;
                            break;
                        }
                        u8IRKeyPrev = IRKey;
                    }
                    if(BRICK_TERMINATOR_IR_TIMEOUT < MsOS_Timer_DiffTimeFromNow(u32FirstTime))
                    {
                        printf("%s: TIMEOUT, at %d\n", __func__, __LINE__);
                        break;
                    }
                    udelay(5000);
                }
            }
            break;
#endif
        case IR_RECOVERY_KEY:
            mode = EN_BOOT_MODE_RECOVERY;
            break;
        case IR_UPGRADEUSBOTA_KEY:
            mode = EN_BOOT_MODE_OTA_UPGRADE;
            break;
        case IR_FORCEUGRADE_KEY:
            mode = EN_BOOT_MODE_USB_UPGRADE;
            break;
        case IR_UPGRADEBOOTLOADER_KEY:
            mode = EN_BOOT_MODE_USB_RECOVRY_UPGRADE;
            break;
        case IR_RECOVERYWIPEDATA_KEY:
            mode = EN_BOOT_MODE_RECOVRY_WIPE_DATA;
            break;
        case IR_RECOVERYWIPECACHE_KEY:
            mode = EN_BOOT_MODE_RECOVRY_WIPE_CACHE;
            break;
#if defined (CONFIG_SEC_SYSTEM)
        case IR_SECOND_SYSTEM_KEY:
            if (0 == strcmp(getenv(ENV_BOOTto2SubSystem),"1"))
            {
                mode = EN_BOOT_MODE_SECOND_SYSTEM;
                ENABLE_SECOND_SYSTEM=1;
            }
            break;
#endif
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
        case IR_FASTBOOT_KEY:
            mode = EN_BOOT_MODE_FASTBOOT;
            break;
#endif
        default:
            mode = EN_BOOT_MODE_UNKNOWN;
            break;
    }
    UBOOT_TRACE("OK\n");
    return mode;
}
#elif(ENABLE_DYNAMIC_IR)
static eIRBootCase msIR_SW_MatchBootCase(U8 u8IRProtocol,U32 u32IRHeadcode,U16 u16IRKeycode)
{
    st_IRBootCheck_t stIRBootCheckInfo;
    U32 i = 0;
    U32 j = 0;
    int ret = 0;
    memset(&stIRBootCheckInfo,0,(sizeof(IRBootCheckInfo_t)));
    ret = Read_IRBootcheck_FromFlash(&stIRBootCheckInfo);
    if (ret != 0)
        return IR_BOOTCASE_INVAILUE;
    for(i=0;i<IR_MAX;i++)
    {
        if((u8IRProtocol == stIRBootCheckInfo[i].u8Protocol) && (u32IRHeadcode == stIRBootCheckInfo[i].u32Headcode))
        {
            for(j=0;j<IR_KEY_MAX;j++)
            {
                if(stIRBootCheckInfo[i].u16Key[j] == u16IRKeycode)
                    return (eIRBootCase)(j+1);
            }
        }
    }
    return IR_BOOTCASE_INVAILUE;
}

extern BOOLEAN drv_IR_SW_GetKeyCode(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8RepeatFlag);

EN_BOOT_MODE get_boot_mode_from_ir(void)
{
    U8 u8IRProtocol;
    U32 u32IRHeadcode;
    U16 u16IRKeycode;
    U8 u8RepeatFlag;
    eIRBootCase eBootCase;
    EN_BOOT_MODE mode = EN_BOOT_MODE_UNKNOWN;
    UBOOT_INFO("[Mboot_IR] boot from dynamic ir \n");
    if(drv_IR_SW_GetKeyCode(&u8IRProtocol,&u32IRHeadcode,&u16IRKeycode,&u8RepeatFlag))
    {
        UBOOT_INFO("[Mboot_IR] u8IRProtocol  = %x ,u32IRHeadcode = %x ,u16IRKeycode  = %x ,u8RepeatFlag  = %x\n",u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
        eBootCase = msIR_SW_MatchBootCase(u8IRProtocol,u32IRHeadcode,u16IRKeycode);
        UBOOT_INFO("[Mboot_IR]============ [BootCase] case :%d\n",eBootCase);
        switch(eBootCase)
        {
            case IR_BOOTCASE_RECOVERY:
                mode = EN_BOOT_MODE_RECOVERY;
                break;
            case IR_BOOTCASE_PGRADEUSBOTA:
                mode = EN_BOOT_MODE_OTA_UPGRADE;
                break;
            case IR_BOOTCASE_FORCEUPGRADE:
                mode = EN_BOOT_MODE_USB_UPGRADE;
                break;
            case IR_BOOTCASE_UPGRADEBOOTLOADER:
                mode = EN_BOOT_MODE_USB_RECOVRY_UPGRADE;
                break;
            case IR_BOOTCASE_RECOVERYWIPEDATA:
                mode = EN_BOOT_MODE_RECOVRY_WIPE_DATA;
                break;
            case IR_BOOTCASE_RECOVERYWIPECACHE:
                mode = EN_BOOT_MODE_RECOVRY_WIPE_CACHE;
                break;
#if defined (CONFIG_SEC_SYSTEM)
            case IR_BOOTCASE_SECOND_SYSTEM:
                if (0 == strcmp(getenv(ENV_BOOTto2SubSystem),"1"))
                {
                    mode = EN_BOOT_MODE_SECOND_SYSTEM;
                    ENABLE_SECOND_SYSTEM=1;
                }
                break;
#endif
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
            case IR_BOOTCASE_FASTBOOT:
                mode = EN_BOOT_MODE_FASTBOOT;
                break;
#endif
            default:
                mode = EN_BOOT_MODE_UNKNOWN;
                break;
        }
    }
    return mode;
}
#endif
#endif

#if (ENABLE_MODULE_ANDROID_BOOT == 1 )

EN_BOOT_MODE get_boot_mode_from_mtd0(void)
{
    EN_BOOT_MODE mode = EN_BOOT_MODE_UNKNOWN;
    struct bootloader_message *p_msg = NULL;
    p_msg = (struct bootloader_message*)BOOT_MODE_TMP_BUF_ADDR;
    char cmd[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
#if (ENABLE_MODULE_MMC == 1)
    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX misc 32", (unsigned long)BOOT_MODE_TMP_BUF_ADDR);
#else
    snprintf(cmd, sizeof(cmd)-1, "nand read.e 0x%08lX misc 32", (unsigned long)BOOT_MODE_TMP_BUF_ADDR);
#endif
    if(-1 != run_command(cmd, 0))
    {

        if((strlen(p_msg->command) == strlen(BOOT_MODE_RECOVERY_STR)) && (0==strncmp(p_msg->command, BOOT_MODE_RECOVERY_STR, strlen(BOOT_MODE_RECOVERY_STR))))
        {
            mode = EN_BOOT_MODE_RECOVERY;
        }
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
        else if ((strlen(p_msg->command) == strlen(BOOT_MODE_BOOTLOADER_STR)) && (0 == strncmp(p_msg->command, BOOT_MODE_BOOTLOADER_STR, strlen(BOOT_MODE_BOOTLOADER_STR))))
        {
           memset(cmd, 0, CMD_BUF);
           memset((void *)BOOT_MODE_TMP_BUF_ADDR, 0, 32);

#if (ENABLE_MODULE_MMC == 1)
           snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%08lX misc 32", (unsigned long)BOOT_MODE_TMP_BUF_ADDR);
#else
           snprintf(cmd, sizeof(cmd)-1, "nand write.e 0x%08lX misc 32", (unsigned long)BOOT_MODE_TMP_BUF_ADDR);
#endif
           run_command(cmd, 0);

           mode = EN_BOOT_MODE_FASTBOOT;
        }
#endif
        /*else {do nothing;}*/
    }
    else
    {
        printf("%s: '%s' fails, at %d\n", __func__, cmd, __LINE__);
    }
    UBOOT_TRACE("OK\n");
    return mode;
}

#endif


#if (ENABLE_MODULE_BOOT_KEYPAD == 1)
extern BOOLEAN msKeypad_GetKey(U8 *pkey, U8 *pflag);

static EN_BOOT_MODE check_usb_upgrade(void)
{
    U8 u8KeyPad_KeyVal=0xFF;
    U8 u8KeyPad_RepFlag = 0;
    EN_BOOT_MODE mode = EN_BOOT_MODE_UNKNOWN;
    U32 pre_time = 0;
    U32 cur_time = 0;
    U32 interval = 0;
    U32 tick = 0;

    pre_time = MsSystemGetBootTime();
    UBOOT_INFO("\033[33;1mPressing the KEYPAD_FORCEUGRADE_KEY\033[0m\n");
    UBOOT_INFO("Ticking: ");
    while (interval < KEYPAD_HOLD_VALUE)
    {
        msKeypad_GetKey(&u8KeyPad_KeyVal,&u8KeyPad_RepFlag);
        if (u8KeyPad_KeyVal != KEYPAD_FORCEUGRADE_KEY)
        {
            break;
        }
        cur_time = MsSystemGetBootTime();
        interval = cur_time - pre_time;
        if (tick != interval/50)
        {
            tick = interval/50;
            printf("#");
        }
    }
    if (interval >= KEYPAD_HOLD_VALUE)
    {
        mode = EN_BOOT_MODE_USB_UPGRADE;
        UBOOT_INFO("\nTime up(%d.%ds), do USB Upgrade\n", KEYPAD_HOLD_VALUE/1000, (KEYPAD_HOLD_VALUE%1000)/100);
    }
    else
    {
        UBOOT_INFO("\nNo enough time(%d.%ds < %d.%ds), continue to booting...\n", interval/1000, (interval%1000)/100,
                        KEYPAD_HOLD_VALUE/1000, (KEYPAD_HOLD_VALUE%1000)/100);
    }
    return mode;
}


EN_BOOT_MODE get_boot_mode_from_keypad(void)
{
     U8 u8KeyPad_KeyVal=0xFF;
     U8 u8KeyPad_RepFlag = 0;
     EN_BOOT_MODE mode = EN_BOOT_MODE_UNKNOWN;
     UBOOT_TRACE("IN\n");
     msKeypad_GetKey(&u8KeyPad_KeyVal,&u8KeyPad_RepFlag);
     printf("fore uup u8KeyPad_KeyVal [0x%x]\n",u8KeyPad_KeyVal);
     switch(u8KeyPad_KeyVal) // NOTE: read IR Key to decide what boot mode to go
    {
#if defined(CONFIG_AN_FASTBOOT_ENABLE)
        case KEYPAD_FASTBOOT_KEY:
            mode = EN_BOOT_MODE_FASTBOOT;
            break;
#endif
        case KEYPAD_RECOVERY_KEY:
            mode = EN_BOOT_MODE_RECOVERY;
            break;
        case KEYPAD_FORCEUGRADE_KEY:
            mode = check_usb_upgrade();
            break;
        case KEYPAD_UART_DEBUG_KEY:
            mode = EN_BOOT_MODE_UART_DEBUG;
            break;
        #if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
        case KEYPAD_SYSTEM_RESTORE_KEY:
            mode = EN_BOOT_MODE_SYSTEM_RESTORE;
            break;
        #endif
        default:
            mode = EN_BOOT_MODE_UNKNOWN;
            break;
    }
     UBOOT_TRACE("OK\n");
     return mode;
}
#endif

// set boot to Recovery mode flag
static void setBootToRecovery(void)
{
    stBootMode = EN_BOOT_MODE_RECOVERY;
}

// check if boot to Recovery mode
// must call it after bootcheck command
int isBootToRecovery(void)
{
    if(EN_BOOT_MODE_RECOVERY == stBootMode)
    {
        UBOOT_DEBUG("boot to recovery mode\n");
        return 1;
    }
    else
    {
        UBOOT_DEBUG("boot to normal mode\n");
        return 0;
    }
}

#if ENABLE_STB_ECOS_BOOT
#if defined(CONFIG_DISABLE_PM_PARTITION)
#define STB_UBOOT_INFO_ADDR     0x10000
#else
#define STB_UBOOT_INFO_ADDR     0x20000
#endif
#define STB_INFO_SIZE           0x400//1024B//0x20000

void Big2LittleEndian(U8* addr,U8* data)
{

    U8 * tmp = NULL;
    tmp = (U8 *)(data);
    *tmp = *(addr+3);
    *(tmp+1) = *(addr+2);
    *(tmp+2) = *(addr+1);
    *(tmp+3) = *(addr);
    UBOOT_DEBUG("\n*(addr) 0x%x\n",*addr);
}

int do_read_boot_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buffer[256];
    U8 *pTemp;
    U32 u32AppMagicFlag;
    U32 u32AppFileLen;
    U32 u32AppDramAddr;
    U32 u32AppRestAddr;
    U32 u32AppSPIAddr;
    U32 u32LzmaBufAddr;
    U32 u32LzmaDecBufAddr;
    U32 u32UbootInfoAddr = 0;
    int ret;
    int ch = 0;
    U32 u32EnvSize = CONFIG_ENV_SIZE;

    if (tstc())
    {   /* we got a key press   */
        ch = getc();  /* consume input  */
        puts ("\b\b\b 0");
        if (ch == 0x1B)//when esc,upgrade flash_usb_auto.bin
        {
            if (-1 == appInitUsbDisk())
            {
               UBOOT_ERROR("FAIL : can not init usb!! \n");
               return -1;
            }

            sprintf(buffer, "ustar /flash_usb_auto.bin");
            UBOOT_DEBUG("cmd=%s\n", buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("USB auto upgrade fail!\n");
                return -1;
            }

            run_command("reset",0);
        }
    }

    memset(buffer, 0 , 256);
    sprintf(buffer,"spi_rdc 0x80B00000 0x%x 0x%x", STB_UBOOT_INFO_ADDR, STB_INFO_SIZE);
    UBOOT_INFO("%s\n", buffer);

    ret = run_command(buffer, 0);
    if (ret!=0)
    {
        UBOOT_ERROR("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        UBOOT_DEBUG("cmd %s success\n", buffer);
    }

#if (CONFIG_DISPLAY_LOGO == 1)
    pTemp = (U8 *)((U32)0x80B00000);
    pTemp += 0x20;//uboot rom end addr stored here
    u32UbootInfoAddr = *((U32 *) pTemp);

    UBOOT_DEBUG("u32UbootInfoAddr: 0x%x\n", u32UbootInfoAddr);
    pTemp += 0x4;
    u32UbootInfoAddr +=*((U32 *) pTemp);

    UBOOT_DEBUG("u32UbootInfoAddr1: 0x%x\n", u32UbootInfoAddr);
#else
    pTemp = (U8 *)((U32)0x80B00000);
    pTemp += 0xc;//uboot rom end addr stored here

    UBOOT_DEBUG("u32UbootInfoAddr: 0x%x\n", u32UbootInfoAddr);
    u32UbootInfoAddr = *((U32 *) pTemp);
    UBOOT_DEBUG("u32UbootInfoAddr1: 0x%x\n", u32UbootInfoAddr);
    u32UbootInfoAddr &= 0x000fffff;//get offest of 0xBFCxxxxx
    UBOOT_DEBUG("u32UbootInfoAddr: 0x%x\n", u32UbootInfoAddr);
#endif

    if (u32EnvSize > 0 )
    {
        u32UbootInfoAddr = u32UbootInfoAddr + u32EnvSize;
        u32UbootInfoAddr = u32UbootInfoAddr/u32EnvSize;
        u32UbootInfoAddr = u32UbootInfoAddr*u32EnvSize;
    }
    else
    {
        UBOOT_ERROR("Env setting is not correct (0x%x) !!\n", u32EnvSize);
        return FALSE;
    }

    UBOOT_DEBUG("BOOT_INFO_ADDR: 0x%x\n", u32UbootInfoAddr);

    memset(buffer, 0 , 256);
    sprintf(buffer,"spi_rdc 0x80B00000 0x%x 0x%x", u32UbootInfoAddr, STB_INFO_SIZE);
    UBOOT_INFO("%s\n", buffer);

    ret = run_command(buffer, 0);

    if (ret!=0)
    {
        UBOOT_ERROR("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        UBOOT_DEBUG("cmd %s success\n", buffer);
    }

    pTemp = (U8 *)((U32)0x80B00000);

    if (*((U32*)pTemp) == 0xefbeadde)
    {
       u32AppMagicFlag = 0xDEADBEEF;
    }
    Big2LittleEndian(pTemp,(U8*)(&u32AppMagicFlag));

    if (u32AppMagicFlag != 0xDEADBEEF)
    {
        UBOOT_ERROR("AppMagicFlag ERROR !!\n");
        return FALSE;
    }
    else
    {
        UBOOT_DEBUG("AppMagicFlag Correct !!\n");

    }
    UBOOT_DEBUG("\n*(pTemp +4) 0x%x\n",*(U32*)(pTemp +4));//printf just print 4 bytes in order
    Big2LittleEndian((U8 *)(pTemp +4),(U8 *) &u32AppFileLen);
    UBOOT_DEBUG("\nu32AppFileLen 0x%x\n", u32AppFileLen);

    // u32AppRestAddr
    Big2LittleEndian((U8 * )(pTemp +12),(U8 *) &u32AppRestAddr);
    Big2LittleEndian((U8 * )(pTemp +8),(U8 *) &u32AppDramAddr);
    UBOOT_DEBUG("\nu32AppDramAddr 0x%x\n",u32AppDramAddr);

    // u32AppSPIAddr
    u32AppSPIAddr = 0;
    Big2LittleEndian((U8 *)(pTemp +16),(U8 *) &u32AppSPIAddr);
    UBOOT_DEBUG("\nu32AppSPIAddr 0x%x\n", u32AppSPIAddr);

    // u32LzmaBufAddr
    u32LzmaBufAddr = 0;
    Big2LittleEndian((U8 *)(pTemp +20),(U8 *) &u32LzmaBufAddr);
    UBOOT_DEBUG("\nu32AppFileLen 0x%x\n", u32LzmaBufAddr);

    // u32LzmaDecBufAddr
    u32LzmaDecBufAddr = 0;
    Big2LittleEndian((U8 *)(pTemp +24),(U8 *) &u32LzmaDecBufAddr);
    UBOOT_DEBUG("\nu32AppFileLen 0x%x\n", u32LzmaDecBufAddr);

    memset(buffer, 0 , 256);

    //u32AppFileLen = 0x300000;
    sprintf(buffer, "setenv bootcmd ' usb exit;spi_rdc 0x%x 0x%x 0x%x; LzmaDec 0x%x 0x%x 0x%x 0x%x; go 0x%x;",u32LzmaBufAddr,u32AppSPIAddr,u32AppFileLen,u32LzmaBufAddr,u32AppFileLen,u32AppDramAddr,u32LzmaDecBufAddr,u32AppRestAddr);
    UBOOT_INFO("%s\n", buffer);

    ret = run_command (buffer, 0);
    if (ret != 0)
    {
        UBOOT_ERROR("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        UBOOT_DEBUG("cmd %s success\n", buffer);
    }

    memset(buffer, 0 , 256);
    sprintf(buffer, "saveenv");

    ret = run_command (buffer, 0);
    if (ret!=0)
    {
        UBOOT_ERROR("cmd %s failed\n", buffer);
        return FALSE;
    }
    else
    {
        UBOOT_DEBUG("cmd %s success\n", buffer);
    }

    return TRUE;
}
#endif

void _RecoveryDrvInit(void)
{

    #if (CONFIG_URSA12_VB1_FRC_BRINGUP)
        run_command ("frcup",0);
    #endif

    run_command("panel_pre_init",0);
#if(ENABLE_XC_Init==1)
    #if (CONFIG_URSA_UNION)
    if ((MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U9) || (MDrv_URSA_COM_Get_URSA_Type() == URSA_COMMON_U11))
    #endif
    {
        run_command("xc_init", 0);
    }
#endif //#if(ENABLE_XC_Init==1)

#if defined(CONFIG_ENABLE_4K2K_PANEL)
#if defined(CONFIG_INX_NOVA_VB1)
    run_command("inx_panel_set_init", 0);
#endif
#endif
#if(ENABLE_CVBS_OUT==1)
    run_command ("cvbs" , 0);
#endif
#if(ENABLE_HDMI_TX == 1)
    run_command("hdmi init",0);
#endif
#if (CONFIG_ENABLE_RTPM)
    run_command ("runtime_pm" , 0);
#endif
#if defined (CONFIG_URSA_6M40)
    run_command("ursa_lvds_on", 0);
#endif
#if defined (CONFIG_URSA_8)
    run_command("ursa8_lvds_on", 0);
#endif

    run_command("bootlogo 0 0 1 1 boot_recovery.jpg", 0);

#if defined(CONFIG_INX_VB1)
    run_command("panel_inx_vb1_init", 0);
#endif

}
#if (ENABLE_MODULE_NETUPDATE == 1)
void _do_NetUpgrade_mode(void)
{
#if defined (CONFIG_NETUPGRADE_IN_BOOT)
    UBOOT_DEBUG("Jump to KL_RC\n");
    set_bootargs_cfg("root","root=ubi:recovery",1);
    boot_mode_recovery();
#else
    run_command("cnstar",0);//net upgrade
#endif
}
#endif

#if defined (CONFIG_DUAL_SYSTEM)
typedef struct{
    U8 boot_flag;
    U8 mboot_verify_flag;
}st_bootflag;

unsigned char read_bootflag(void)
{
    st_bootflag stBootflag = {0, 0};
    char cmd[CMD_BUF]="\0";

    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX bootflag 2", (unsigned long)(&stBootflag));
    if(-1 == run_command(cmd, 0))
    {
        UBOOT_DEBUG("%s: '%s' failed, at %d\n", __func__, cmd, __LINE__);
    }

    UBOOT_DEBUG("boot_flag=0x%x, mboot_verify_flag=0x%x\n", stBootflag.boot_flag, stBootflag.mboot_verify_flag);

    return stBootflag.boot_flag;
}

int do_bootflag_read (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    st_bootflag stBootflag = {0, 0};

    stBootflag.boot_flag = read_bootflag();
    UBOOT_INFO("boot_flag = 0x%x\n", stBootflag.boot_flag);

    return 0;
}

int do_bootflag_set (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    st_bootflag stBootflag = {0, 0};
    char cmd[CMD_BUF]="\0";

    if (2 == argc)
    {
        stBootflag.boot_flag= (U8)simple_strtoul(argv[1], NULL, 16);
        UBOOT_INFO("do_bootflag_set - bootflag set value: 0x%x \n", stBootflag.boot_flag);
        snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%08lX bootflag 2", (unsigned long)(&stBootflag));
        if(-1 != run_command(cmd, 0))
        {
            run_command("reset", 0);
        }
        else
        {
            UBOOT_ERROR("do_bootflag_set 0x%x failed \n", stBootflag.boot_flag);
            return -1;
        }
    }
    else
    {
        UBOOT_ERROR("do_bootflag_set - Please enter a parameter \n");
    }

    return 0;
}

int do_sboot_verify_check (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    U8 bootflag = 0;
    U16 sboot_verify_flag = 0;

    bootflag = read_bootflag();
    sboot_verify_flag = Read2Byte(0x103380);  //read verify _flag of  uboot which set by sboot
    UBOOT_INFO("mbootflag = 0x%x, sboot_verify_flag = 0x%x\n", bootflag, sboot_verify_flag);

    switch (sboot_verify_flag)
    {
        case 0x5A:
            UBOOT_ERROR("MBOOT verify failed, set bootflag to 1.\n");
            run_command("bootflag_set 1", 0);
            break;
        case 0x5A00:
            UBOOT_ERROR("MBOOTBAK verify failed, set bootflag to 0.\n");
            run_command("bootflag_set 0", 0);
            break;
        case 0x5A5A:
            UBOOT_ERROR("MBOOT & MBOOTBAK both verify failed.\n");
            run_command("custar", 0);
            jump_to_console();
            break;
        default:
            break;
    }

    return 0;
}

int do_copy_mpool(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[CMD_BUF]="\0";

    if(0 == read_bootflag())
    {
        snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x20200000 MPOOL 0xA00000");
        UBOOT_INFO("do_copy_mpool - %s\n", cmd);
        run_command(cmd, 0);

        snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x20200000 MPOOL1 0xA00000");
        UBOOT_INFO("do_copy_mpool - %s\n", cmd);
        run_command(cmd, 0);
    }
    else
    {
        snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x20200000 MPOOL1 0xA00000");
        UBOOT_INFO("do_copy_mpool - %s\n", cmd);
        run_command(cmd, 0);

        snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x20200000 MPOOL 0xA00000");
        UBOOT_INFO("do_copy_mpool - %s\n", cmd);
        run_command(cmd, 0);
    }

    return 0;
}

int do_mboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    st_bootflag stBootflag = {0, 0};
    char cmd[CMD_BUF]="\0";

    snprintf(cmd, sizeof(cmd)-1, "mmc read.p 0x%08lX bootflag 2", (unsigned long)(&stBootflag));
    if(-1 == run_command(cmd, 0))
    {
        UBOOT_ERROR("%s: '%s' failed, at %d\n", __func__, cmd, __LINE__);
        return 0;
    }

    UBOOT_INFO("do_mboot_verify_check 0x%02x, 0x%02x\n", stBootflag.boot_flag, stBootflag.mboot_verify_flag);
    if(0x5A != stBootflag.mboot_verify_flag)
    {
        stBootflag.boot_flag = (stBootflag.boot_flag == 0)?1:0;
        stBootflag.mboot_verify_flag = 0x5A;

        UBOOT_INFO("Try another system, boot_flag=0x%02x,mboot_verify_flag=0x%02x \n", stBootflag.boot_flag, stBootflag.mboot_verify_flag);
        snprintf(cmd, sizeof(cmd)-1, "mmc write.p 0x%08lX bootflag 2", (unsigned long)(&stBootflag));
        if(-1 != run_command(cmd, 0))
        {
            run_command("reset", 0);
        }
    }
    else //another system image verify failed by mboot too.
    {
        UBOOT_INFO("Both system image verify failed,Run USB upgrade to rescue.\n");
        run_command("custar", 0);
    }

    return 0;
}
#endif

int do_bootcheck (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret=0;
    EN_BOOT_MODE BootMode;
    BOOLEAN BootFlag;
    BootMode = EN_BOOT_MODE_UNKNOWN;
    BootFlag=0; // decide the boot mode

    char *cUpgradeStatus = getenv("upgrade_status");

    // check upgrade status, if upgrade fail, clean relatived upgrade flag
    if( cUpgradeStatus != NULL )
    {
        if( strncmp(cUpgradeStatus, "ERROR", 5) == 0 )
        {
            setenv("upgrade_mode",NULL);
            setenv("MstarUpgrade_complete","0");
            setenv("upgrade_status", NULL);
            setenv("force_upgrade", NULL);
            saveenv();
        }
    }

    UBOOT_TRACE("IN\n");

    if(MDrv_WDT_IsReset())
    {
        printf("Boot Check: Reboot by WDT --------\n");
    }

#if (ENALBE_PNL_ON_UPGRADE_ON_AC_BOOT)
    if (msAPI_Power_QueryPowerOnMode() == EN_POWER_AC_BOOT)
    {
         MDrv_PM_ActiveStandbyMode(FALSE);
    }
#endif

#if (ENABLE_AUTO_USB_UPGRADE == 1)
    #if (ENABLE_AUTO_USB_UPGRADE_AC == 1)
        if (msAPI_Power_QueryPowerOnMode() == EN_POWER_AC_BOOT)
    #endif
        {
            char *cUpgradeComplete = getenv(ENV_UPGRADE_COMPLETE);
            if((cUpgradeComplete == NULL) || (0 == strcmp(cUpgradeComplete, "0")))
            {
                //bootmode via AC ON
                run_command("custar", 0);
            }
        }
#endif

#if (ENABLE_MODULE_BOOT_IR == 1)
     if(BootFlag == 0)
     {
        BootMode =get_boot_mode_from_ir();
        if(BootMode!=EN_BOOT_MODE_UNKNOWN)
        {
            //bootmode via IR
            BootFlag=1;
        }
     }
#endif

#if (ENABLE_MODULE_BOOT_KEYPAD == 1)
     if(BootFlag == 0)
     {
        BootMode =get_boot_mode_from_keypad();
        if(BootMode!=EN_BOOT_MODE_UNKNOWN)
        {
            //BootMode via KEYPAD
            BootFlag=1;
        }
     }
#endif

#if (ENABLE_MODULE_ANDROID_BOOT == 1)
    // NOTE: read BCB in mtd0 (misc) to decide what boot mode to go
    if(BootFlag == 0)
    {
        BootMode=get_boot_mode_from_mtd0();
        if(BootMode!=EN_BOOT_MODE_UNKNOWN)
        {
            //BootMode via MTD
            BootFlag=1;
        }
    }
#endif

#if (ENABLE_MODULE_ENV_BOOT == 1)
    if(BootFlag == 0)
    {
        BootMode=get_boot_mode_from_env();
    }

    char* force_upgrade = getenv(ENV_FORCE_UPGRADE);
    if(force_upgrade)
    {
        int force_flag = simple_strtoul(force_upgrade,NULL,16);
        if((force_flag <= 0x0F)&&(force_flag > 0x00))
        {
           //last time force upgrade not finish,so continue upgrading
            BootMode = EN_BOOT_MODE_USB_UPGRADE;
        }
    }
#endif

#if defined(CONFIG_AN_FASTBOOT_ENABLE)
    // done with boot mode check, cleanup
    run_command("setenv reboot-bootloader 0", 0);
    run_command("saveenv", 0);
#endif

#if (ENABLE_MODULE_ANDROID_BOOT == 1 ) || (ENABLE_MODULE_BOOT_IR == 1)
#if CONFIG_RESCUE_ENV && CONFIG_RESCUE_ENV_IR_TRIGGER
    int brick_terminator_recover_mode = 0;
#endif
#endif
    switch(BootMode)
    {
#if CONFIG_RESCUE_ENV && CONFIG_RESCUE_ENV_IR_TRIGGER
        case EN_BOOT_MODE_BRICK_TERMINATOR:
            brick_terminator_recover_mode = 1;
            break;
#endif
#if (ENABLE_MODULE_ANDROID_BOOT == 1 )
        case EN_BOOT_MODE_RECOVERY:
            boot_mode_recovery();
            break;
        case EN_BOOT_MODE_RECOVRY_WIPE_DATA:
            run_command("recovery_wipe_partition data",0);
            boot_mode_recovery();
            break;
        case EN_BOOT_MODE_RECOVRY_WIPE_CACHE:
            run_command("recovery_wipe_partition cache",0);
            boot_mode_recovery();
            break;
        #if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
        case EN_BOOT_MODE_SYSTEM_RESTORE:
            ret = run_command("SystemRestore",0);
            if (ret != -1)
            {
                boot_mode_recovery();
            }
            break;
        #endif
#endif
        #if (ENABLE_MODULE_USB == 1)
        case EN_BOOT_MODE_USB_UPGRADE:
            ret = run_command("custar",0);//usb upgrade
            break;
#if (ENABLE_MODULE_BOOT_IR == 1)
        case EN_BOOT_MODE_OTA_UPGRADE:
            ret = run_command("ota_zip_check",0);//ota upgrade
            if (ret != -1)
            {
                boot_mode_recovery();
            }
            break;
        case EN_BOOT_MODE_USB_RECOVRY_UPGRADE:
            ret = run_command("usb_bin_check",0);//usb upgrade
            if(ret == 0)
            {
                break;
            }
            ret = run_command("ota_zip_check",0);//ota upgrade
            if (ret != -1)
            {
                boot_mode_recovery();
            }
            break;
#endif
        #endif
        #if (ENABLE_MODULE_OAD == 1)
        case EN_BOOT_MODE_OAD_UPGRADE:
            ret = run_command("costar",0);//oad upgrade
            break;
        #endif
        #if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
        case EN_BOOT_MODE_ENV_UPGRADE:
            ret = run_command("rstar",0);
        #endif
        #if (ENABLE_MODULE_NETUPDATE == 1)
        case EN_BOOT_MODE_NET_UPGRADE:
            _do_NetUpgrade_mode();
            break;
        #endif
        case EN_BOOT_MODE_UART_DEBUG:
            ret = run_command("setenv UARTOnOff on", 0);
            ret = run_command("saveenv", 0);
            printf("Opening UART now\n");
            break;
        case EN_BOOT_MODE_NORMAL:
            break;
        #if defined(CONFIG_AN_FASTBOOT_ENABLE)
        case EN_BOOT_MODE_FASTBOOT:
            ret = run_command("usb start 0",0);
            ret = run_command("fastboot", 0);
            break;
        #endif
        default:
            //normal booting according bootcmd in main.c
            UBOOT_DEBUG("non available case\n");
            break;
    }

#if CONFIG_RESCUE_ENV && CONFIG_RESCUE_ENV_IR_TRIGGER
    if(1 == brick_terminator_recover_mode)
    {
        run_command("BrickTerminator force_recover",0);
        run_command("sync_mmap",0);
#if ((CONFIG_RESCUE_ENV_IR_TRIGGER) && (1 != CONFIG_RESCUE_ENV_AUTO_TRIGGER))
        setenv(BRICK_TERMINATOR_IR_TRIGGERED_STRING, "1");
        saveenv();
#endif
    }
#endif
    UBOOT_TRACE("OK\n");
    return ret;
}


#if defined(CONFIG_SEC_SYSTEM)
int do_Boot2secKernel (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *bootcmd;
    UBOOT_TRACE("IN\n");
    if (ENABLE_SECOND_SYSTEM==1)
    {
        bootcmd=getenv("bootcmd");
        UBOOT_DEBUG("%s\n",bootcmd);
        if(bootcmd!=NULL)
        {
            UBOOT_INFO("\033[0;34m*******************************\033[0m\n");
            UBOOT_INFO("\033[0;34m* BOOT to SECOND KERNEL !! *\033[0m\n");
            UBOOT_INFO("\033[0;34m*******************************\033[0m\n");
            run_command(bootcmd,0);
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
#include <MsTrustZone.h>
mstar_boot_prameters_t boot;
int do_smc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret=0;
	mstar_tee_t *tee;
	//mmc read.p 0x23000000 dtb 0x1000
	ret = run_command("mmc read.p 0xa2100000 optee 0x00100000",0);
	if(-1 == ret)
	{
		UBOOT_ERROR("read kernel fail\n");
		return -1;
	}
	tee = 0xa2100000;
	printf("\033[0;32;31m [Ian] %s %d magic %lx\033[m\n",__func__,__LINE__,tee->magic);
	printf("\033[0;32;31m [Ian] %s %d init_load_addr_lo %lx\033[m\n",__func__,__LINE__,tee->init_load_addr_lo);
	printf("\033[0;32;31m [Ian] %s %d Start %lx\033[m\n",__func__,__LINE__,tee->init_load_addr_lo - sizeof(mstar_tee_t));

	boot.BIN0_entry_point = 0xa2100000;
	boot.BIN1_entry_point = 0x20280000;
	boot.BIN1_r3 = 1234;
	boot.BIN1_r4 = 5678;
	boot.BIN1_r2 = 9012;

	printf("\033[0;32;31m [Ian] %s %d BIN0_entry_point 0x%llx \033[m\n",__func__,__LINE__,boot.BIN0_entry_point);
	printf("\033[0;32;31m [Ian] %s %d BIN1_entry_point 0x%llx \033[m\n",__func__,__LINE__,boot.BIN1_entry_point);

	__asm__ __volatile__(
		"mov  r1,%0  \n\t"
		"smc #0  \n\t"
		:: "r"(&boot)
		: "r1"
	);
}
#endif

#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
#include <MsTrustZone.h>
typedef mstar_tee_t optee_header_t;

unsigned int armfw_real_va_addr=CONFIG_MIU1_BUSADDR + 0x00023000;
unsigned int optee_real_va_addr=CONFIG_MIU1_BUSADDR + 0x02100000;
unsigned int armfw_real_pa_addr=0;
unsigned int optee_real_pa_addr=0;

int GetImage(unsigned int imageAddr, char * partition, U32 offset, U32 size)
{
    #define NUTTX_HEADER_LEN 0x80
    #define NUTTX_CONFIG_OFFSET 2
    int ret = 0;


    UBOOT_TRACE("IN\n");

    raw_io_config_push();
    ret = raw_io_config(FLASH_RAW_DATA_TARGET, partition, "");

    if(ret != 0){
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret = raw_read((U32)imageAddr, offset, size);
    if(ret != 0){
        UBOOT_ERROR("raw_read %s secure image fail!\n", partition);
        return -1;
    }

    raw_io_config_pop();
    UBOOT_TRACE("OK\n");
    return ret;
}

#if !(defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined (CONFIG_SECURITY_BOOT))
int do_read_arm_fw (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_MSTAR_NS_UBOOT
    armfw_real_va_addr=0x5ebe3000;
    return 1;
#else
    unsigned char* arm_virt_fw_addr=NULL;
    unsigned char* arm_virt_fw_align_addr=NULL;
    unsigned int header_len=sizeof(optee_header_t);
    optee_header_t* pst_armfw_header=NULL;

    printf("boot from arm fw \n");

    pst_armfw_header=(optee_header_t*)malloc(header_len);
    if(pst_armfw_header==NULL){
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(pst_armfw_header,0,header_len);

    GetImage((unsigned int)pst_armfw_header, "armfw", 0, header_len);

    //_dump(pst_armfw_header,0x100);

    UBOOT_DEBUG("sizeof(optee_header_t ) : %d\n",sizeof(optee_header_t ));
    UBOOT_DEBUG("pst_armfw_header->magic : %x\n",pst_armfw_header->magic);
    UBOOT_DEBUG("pst_armfw_header->version : %x\n",pst_armfw_header->version);
    UBOOT_DEBUG("pst_optee_header->arch_id : %x\n",pst_armfw_header->arch_id);
    UBOOT_DEBUG("pst_armfw_header->flags : %x\n",pst_armfw_header->flags);
    UBOOT_DEBUG("pst_armfw_header->init_load_addr_hi : %x\n",pst_armfw_header->init_load_addr_hi);
    UBOOT_DEBUG("pst_armfw_header->init_load_addr_lo : %x\n",pst_armfw_header->init_load_addr_lo);
    UBOOT_DEBUG("pst_armfw_header->init_size : %d\n",pst_armfw_header->init_size);

    armfw_real_va_addr=pst_armfw_header->init_load_addr_lo;
    armfw_real_pa_addr=VA2PA(pst_armfw_header->init_load_addr_lo);
    //arm_fw_real_run_addr=pst_armfw_header->init_load_addr_lo;

    arm_virt_fw_addr=malloc(pst_armfw_header->init_size+header_len+16);
    if(arm_virt_fw_addr==NULL){
        UBOOT_ERROR("malloc fail\n");
        free(pst_armfw_header);
        return -1;
    }
    arm_virt_fw_align_addr=(unsigned char*)((((U32)(((U32)arm_virt_fw_addr)+16))>>4)<<4);

    UBOOT_DEBUG("arm_virt_fw_align_addr =%x \n",(U32)arm_virt_fw_align_addr);
    if(GetImage((unsigned int)arm_virt_fw_align_addr, "armfw", 0, pst_armfw_header->init_size+header_len) == -1){
        free(pst_armfw_header);
        free(arm_virt_fw_addr);
        return -1;
    }

    UBOOT_DEBUG("VA2PA(arm_virt_fw_align_addr) :%x \n",(U32)VA2PA((unsigned int)arm_virt_fw_align_addr));
    MDrv_BDMA_MemCopy((U32)VA2PA((U32)arm_virt_fw_align_addr)+header_len, (U32)VA2PA(armfw_real_va_addr), pst_armfw_header->init_size);
    UBOOT_DEBUG("VA2PA(arm_fw_real_run_addr) :%x \n",(U32)VA2PA(armfw_real_va_addr));
 //   _dump(armfw_real_va_addr,0x100);
 //   _dump(armfw_real_va_addr+arm_fw_size-0x100,0x100);

    free(pst_armfw_header);
    free(arm_virt_fw_addr);

    return 1;
#endif
}

#else
int do_read_arm_fw (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_MSTAR_NS_UBOOT
    armfw_real_va_addr=0x5ebe3000;
    return 1;
#else
    unsigned char* arm_virt_fw_addr=NULL;
    unsigned char* arm_virt_fw_align_addr=NULL;
    unsigned int header_len=sizeof(optee_header_t);
    optee_header_t* pst_armfw_header=NULL;

    printf("boot from arm fw via secure booting\n");

    U32 imageSize = 0;
    int ret=0;
    ret = GetImageSize("armfw",0, &imageSize);
    if (0 != ret)
    {
        UBOOT_ERROR("get armfw size fail\n");
        return -1;
    }
///-------get full image
    arm_virt_fw_addr=malloc(imageSize+16);
    if(arm_virt_fw_addr==NULL){
        UBOOT_ERROR("malloc fail\n");
        free(pst_armfw_header);
        return -1;
    }
    arm_virt_fw_align_addr=(unsigned char*)((((U32)(((U32)arm_virt_fw_addr)+16))>>4)<<4);

    UBOOT_DEBUG("arm_virt_fw_align_addr =%x \n",(U32)arm_virt_fw_align_addr);
    if(GetImage((unsigned int)arm_virt_fw_align_addr, "armfw", 0, imageSize) == -1){
        free(pst_armfw_header);
        free(arm_virt_fw_addr);
        return -1;
    }

///-------auth full image
    if(-1==RawData_Authentication("armfw",arm_virt_fw_align_addr))
    {
        UBOOT_ERROR("TEE Authentication fail\n");
        return -1;
    }

///-------get header
    pst_armfw_header=(optee_header_t*)malloc(header_len);
    if(pst_armfw_header==NULL){
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(pst_armfw_header,0,header_len);

    memset(pst_armfw_header,0,header_len);
    memcpy(pst_armfw_header,arm_virt_fw_align_addr,header_len);
    //_dump(pst_armfw_header,0x100);
    UBOOT_DEBUG("sizeof(optee_header_t ) : %d\n",sizeof(optee_header_t ));
    UBOOT_DEBUG("pst_armfw_header->magic : %x\n",pst_armfw_header->magic);
    UBOOT_DEBUG("pst_armfw_header->version : %x\n",pst_armfw_header->version);
    UBOOT_DEBUG("pst_optee_header->arch_id : %x\n",pst_armfw_header->arch_id);
    UBOOT_DEBUG("pst_armfw_header->flags : %x\n",pst_armfw_header->flags);
    UBOOT_DEBUG("pst_armfw_header->init_load_addr_hi : %x\n",pst_armfw_header->init_load_addr_hi);
    UBOOT_DEBUG("pst_armfw_header->init_load_addr_lo : %x\n",pst_armfw_header->init_load_addr_lo);
    UBOOT_DEBUG("pst_armfw_header->init_size : %d\n",pst_armfw_header->init_size);

    armfw_real_va_addr=pst_armfw_header->init_load_addr_lo;
    armfw_real_pa_addr=VA2PA(pst_armfw_header->init_load_addr_lo);
    //arm_fw_real_run_addr=pst_armfw_header->init_load_addr_lo;



    UBOOT_DEBUG("VA2PA(arm_virt_fw_align_addr) :%x \n",(U32)VA2PA((unsigned int)arm_virt_fw_align_addr));
    MDrv_BDMA_MemCopy((U32)VA2PA((U32)arm_virt_fw_align_addr)+header_len, (U32)VA2PA(armfw_real_va_addr), pst_armfw_header->init_size);
    UBOOT_DEBUG("VA2PA(arm_fw_real_run_addr) :%x \n",(U32)VA2PA(armfw_real_va_addr));
    UBOOT_DEBUG("first 100 bytes\n");
    _dump(armfw_real_va_addr,0x100);
    UBOOT_DEBUG("last 100 bytes\n");
    _dump(armfw_real_va_addr+pst_armfw_header->init_size-0x100,0x100);

    free(pst_armfw_header);
    free(arm_virt_fw_addr);

    return 1;
#endif
}

#endif

int load_tee_config(U32 u32HwAesPABufAddr, U32 u32HwAesBufLen)
{
    U8* tee_config_addr=0;
    U32 tee_config_size=2276;

    printf("u32HwAesPABufAddr :%x \n",u32HwAesPABufAddr);
    printf("u32HwAesBufLen :%x \n",u32HwAesBufLen);
    tee_config_addr=malloc(tee_config_size);
    GetImage((unsigned int)tee_config_addr, "optee", 0x220+0x7fc60, tee_config_size);

    _dump(tee_config_addr, 0x40);
    memcpy((void*)u32HwAesPABufAddr, tee_config_addr,tee_config_size);
    free(tee_config_addr);
    return 1;
}

int load_HW_AES_BUFFER_info(U32 *u32HwAesPABufAddr, U32 *u32HwAesBufLen)
{

    U32 u32HwAesVABufAddr=0;
    U32 optee_mem_len=0;


    if (get_value_from_env(E_MMAP_ID_HW_AES_BUF_ADR, NO_DEFAULT_MMAP_VALUE, u32HwAesPABufAddr) != 0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_HW_AES_BUF_ADR);
        return -1;
    }
    if (get_value_from_env(E_MMAP_ID_HW_AES_BUF_LEN, NO_DEFAULT_MMAP_VALUE, u32HwAesBufLen) != 0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_HW_AES_BUF_ADR);
        return -1;
    }

    u32HwAesVABufAddr=PA2NVA(*u32HwAesPABufAddr);
    optee_mem_len=*u32HwAesPABufAddr-optee_real_pa_addr;
    memcpy((void*)u32HwAesVABufAddr,&optee_real_pa_addr,4);

    UBOOT_DEBUG("%s %d u32HwAesVABufAddr :%x \n",__func__,__LINE__,u32HwAesVABufAddr);
    UBOOT_DEBUG("%s %d u32HwAesBufLen :%x \n",__func__,__LINE__,*u32HwAesBufLen);
    return 1;
}

typedef struct
{
    U32 u32_len;
    U32 u32_addr;
}T_TEE_CONFIG;

void load_tee_config_to_tee(U32 u32_hw_aes_pa_buf_addr, U32 u32_hw_aes_buf_len, U32 u32_tee_config_offset)
{
    U32 u32_hw_aes_nva_buf_addr=PA2NVA(u32_hw_aes_pa_buf_addr);
    U32 signature_offset=544;
    T_TEE_CONFIG* pst_tee_config=NULL;
    U32 u32_config_len=0;
    if(GetImage((unsigned int)u32_hw_aes_nva_buf_addr, "optee", u32_tee_config_offset+signature_offset, 0x100) == -1){
        UBOOT_ERROR("failed to get config image\n");
        return -1;
    }

    //_dump(u32_hw_aes_nva_buf_addr, 0x100);
    pst_tee_config=u32_hw_aes_nva_buf_addr;
    pst_tee_config++;

    u32_config_len=pst_tee_config->u32_len+pst_tee_config->u32_addr;
    if(GetImage((unsigned int)u32_hw_aes_nva_buf_addr, "optee", u32_tee_config_offset+signature_offset, u32_config_len) == -1){
        UBOOT_ERROR("failed to get config image\n");
        return -1;
    }
}

#if !(defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && defined (CONFIG_SECURITY_BOOT))
int do_read_optee (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_MSTAR_NS_UBOOT
    optee_real_va_addr=0x5ee00000;
    return 1;
#else
    //unsigned int optee_real_run_addr=0xA20FFFE4;  // test address
    //unsigned int optee_size=298224;
    unsigned char* optee_virt_addr=NULL;
    unsigned char* optee_virt_align_addr=NULL;
    optee_header_t* pst_optee_header=NULL;
    unsigned int header_len=sizeof(optee_header_t);
    U32 u32HwAesPABufAddr=0;
    U32 u32HwAesBufLen=0;
    U32 u32_tee_config_offset=0;
    printf("boot from optee \n");

    pst_optee_header=(optee_header_t*)malloc(header_len);
    if(pst_optee_header==NULL){
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(pst_optee_header,0,header_len);

    GetImage((unsigned int)pst_optee_header, "optee", 0, header_len);

    UBOOT_DEBUG("sizeof(optee_header_t ) : %d\n",sizeof(optee_header_t ));
    UBOOT_DEBUG("pst_optee_header->magic : %x\n",pst_optee_header->magic);
    UBOOT_DEBUG("pst_optee_header->version : %x\n",pst_optee_header->version);
    UBOOT_DEBUG("pst_optee_header->arch_id : %x\n",pst_optee_header->arch_id);
    UBOOT_DEBUG("pst_optee_header->flags : %x\n",pst_optee_header->flags);
    UBOOT_DEBUG("pst_optee_header->init_load_addr_hi : %x\n",pst_optee_header->init_load_addr_hi);
    UBOOT_DEBUG("pst_optee_header->init_load_addr_lo : %x\n",pst_optee_header->init_load_addr_lo);
    UBOOT_DEBUG("pst_optee_header->init_size : %d\n",pst_optee_header->init_size);

    optee_real_va_addr=pst_optee_header->init_load_addr_lo;
    optee_real_pa_addr=VA2PA(pst_optee_header->init_load_addr_lo);
    //optee_real_run_addr=pst_optee_header->init_load_addr_lo;

    optee_virt_addr=malloc(pst_optee_header->init_size+header_len+16);
    if(optee_virt_addr==NULL){
        UBOOT_ERROR("malloc fail\n");
        free(pst_optee_header);
        return -1;
    }
    optee_virt_align_addr=(unsigned char*)((((U32)(((U32)optee_virt_addr)+16))>>4)<<4);

    UBOOT_DEBUG("optee_virt_align_addr =%x \n",(U32)optee_virt_align_addr);
    if(GetImage((unsigned int)optee_virt_align_addr, "optee", 0, pst_optee_header->init_size+header_len) == -1){
        UBOOT_ERROR("failed to get secure image\n");
        free(pst_optee_header);
        free(optee_virt_addr);
        return -1;
    }
    UBOOT_DEBUG("VA2PA(optee_virt_align_addr) :%x \n",(U32)VA2PA((U32)optee_virt_align_addr));
    MDrv_BDMA_MemCopy((U32)VA2PA((unsigned int)optee_virt_align_addr)+header_len, (U32)VA2PA(optee_real_va_addr), pst_optee_header->init_size);
    UBOOT_DEBUG("VA2PA(optee_real_run_addr) :%x \n",(U32)VA2PA(optee_real_va_addr));

   // _dump(optee_real_va_addr,0x100);
   // _dump(optee_real_va_addr+pst_optee_header->init_size-0x100,0x100);

    if (load_HW_AES_BUFFER_info(&u32HwAesPABufAddr,&u32HwAesBufLen) != 1)
    {
        UBOOT_ERROR("load_HW_AES_BUFFER_info failed\n");
        return -1;
    }
    UBOOT_DEBUG("header_len+pst_optee_header->init_size:%x \n",header_len+pst_optee_header->init_size);
    if(((header_len+pst_optee_header->init_size)%16)!=0)
    u32_tee_config_offset=(header_len+pst_optee_header->init_size+16)>>4<<4;
    else
        u32_tee_config_offset=header_len+pst_optee_header->init_size;

    UBOOT_DEBUG("u32_tee_config_offset :%x \n",u32_tee_config_offset);
//    load_tee_config_to_tee(u32HwAesPABufAddr,u32HwAesBufLen, u32_tee_config_offset);

    free(pst_optee_header);
    free(optee_virt_addr);
    //_dump(optee_real_run_addr,0x100);
    return 1;
#endif
}
#else
int do_read_optee (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
#ifdef CONFIG_MSTAR_NS_UBOOT
    optee_real_va_addr=0x5ee00000;
    return 1;
#else
    //unsigned int optee_real_run_addr=0xA20FFFE4;  // test address
    //unsigned int optee_size=298224;
    unsigned char* optee_virt_addr=NULL;
    unsigned char* optee_virt_align_addr=NULL;
    optee_header_t* pst_optee_header=NULL;
    unsigned int header_len=sizeof(optee_header_t);
    U32 u32HwAesPABufAddr=0;
    U32 u32HwAesBufLen=0;
    U32 u32_tee_config_offset=0;
    printf("boot from optee via secure booting\n");

    U32 imageSize = 0;
    int ret=0;
    ret = GetImageSize("tee",0, &imageSize);
    if (0 != ret)
    {
        UBOOT_ERROR("get optee size fail\n");
        return -1;
    }
    UBOOT_DEBUG("optee size : %ld\n",imageSize);

///-------get full image
    optee_virt_addr=malloc(imageSize+16);
    if(optee_virt_addr==NULL){
        UBOOT_ERROR("malloc fail\n");
        free(pst_optee_header);
        return -1;
    }
    optee_virt_align_addr=(unsigned char*)((((U32)(((U32)optee_virt_addr)+16))>>4)<<4);
    UBOOT_DEBUG("optee_virt_align_addr =%x \n",(U32)optee_virt_align_addr);
    if(GetImage((unsigned int)optee_virt_align_addr, "optee", 0, imageSize) == -1){
        UBOOT_ERROR("failed to get secure image\n");
        free(pst_optee_header);
        free(optee_virt_addr);
        return -1;
    }

///-------auth full image
    if(-1==RawData_Authentication("tee",optee_virt_align_addr))
    {
        UBOOT_ERROR("TEE Authentication fail\n");
        return -1;
    }

///-------get header
    pst_optee_header=(optee_header_t*)malloc(header_len);
    if(pst_optee_header==NULL){
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(pst_optee_header,0,header_len);
    memcpy(pst_optee_header,optee_virt_align_addr,header_len);
    UBOOT_DEBUG("sizeof(optee_header_t ) : %d\n",sizeof(optee_header_t ));
    UBOOT_DEBUG("pst_optee_header->magic : %x\n",pst_optee_header->magic);
    UBOOT_DEBUG("pst_optee_header->version : %x\n",pst_optee_header->version);
    UBOOT_DEBUG("pst_optee_header->arch_id : %x\n",pst_optee_header->arch_id);
    UBOOT_DEBUG("pst_optee_header->flags : %x\n",pst_optee_header->flags);
    UBOOT_DEBUG("pst_optee_header->init_load_addr_hi : %x\n",pst_optee_header->init_load_addr_hi);
    UBOOT_DEBUG("pst_optee_header->init_load_addr_lo : %x\n",pst_optee_header->init_load_addr_lo);
    UBOOT_DEBUG("pst_optee_header->init_size : %d\n",pst_optee_header->init_size);

    optee_real_va_addr=pst_optee_header->init_load_addr_lo;
    optee_real_pa_addr=VA2PA(pst_optee_header->init_load_addr_lo);
    //optee_real_run_addr=pst_optee_header->init_load_addr_lo;


    UBOOT_DEBUG("VA2PA(optee_virt_align_addr) :%x \n",(U32)VA2PA((U32)optee_virt_align_addr));
    MDrv_BDMA_MemCopy((U32)VA2PA((unsigned int)optee_virt_align_addr)+header_len, (U32)VA2PA(optee_real_va_addr), pst_optee_header->init_size);
    UBOOT_DEBUG("VA2PA(optee_real_run_addr) :%x \n",(U32)VA2PA(optee_real_va_addr));

    UBOOT_DEBUG("first 100 bytes\n");
    _dump(optee_real_va_addr,0x100);
    UBOOT_DEBUG("last 100 bytes\n");
    _dump(optee_real_va_addr+pst_optee_header->init_size-0x100,0x100);


    if (load_HW_AES_BUFFER_info(&u32HwAesPABufAddr,&u32HwAesBufLen) != 1)
    {
        UBOOT_ERROR("load_HW_AES_BUFFER_info failed\n");
        return -1;
    }
    u32_tee_config_offset=(header_len+pst_optee_header->init_size+16)>>4<<4;

    //load_tee_config_to_tee(u32HwAesPABufAddr,u32HwAesBufLen, u32_tee_config_offset);


    free(pst_optee_header);
    free(optee_virt_addr);
    //_dump(optee_real_run_addr,0x100);
    return 1;

#endif
}

#endif

#endif
