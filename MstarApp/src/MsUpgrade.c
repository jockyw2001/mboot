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
#include <stdio.h>
#include <MsUpgrade.h>
#include <MsUpgradeUtility.h>
#include <MsUtility.h>
#include <config.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <MsString.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <MsAppCB.h>
#include <CusConfig.h>
#include <MsSystem.h>
#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
#include "ms_msd_global.h"
#include "drvUSBDwld.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SCRIPT_BUF_FOR_DO_MSTAR 4096
#define DEFAULT_BIN_PATH             "/MstarUpgrade.bin"
#define DEFAULT_SCRIPT_FILE_NAME    "auto_update.txt"   // script file name
#define DEFAULT_DEBUG_SCRIPT_PATH             "/MstarDebug.script"

#define ARG_NUM_SCRIPT_FILE         1
#define IS_ARG_AVAILABLE_SCRIPT_FILE(x) ((x) > ARG_NUM_SCRIPT_FILE)
#define ARG_SCRIPT_FILE(x)              (x)[ARG_NUM_SCRIPT_FILE]
#define MAX_SCRIPT_SIZE 0x20000

#if defined(CONFIG_KENYA)
#include <configs/kenya.h>
#ifdef HB_CUS
#include "usb.h"
#include "fat.h"
#include "common.h"
#endif
#endif

#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
//MBOOT_LOADER_HEADER
#define LOADER_HEADER_SIZE_OFFSET       0
#define LOADER_HEADER_VERSION_OFFSET    1
#define LOADER_HEADER_CMD_OFFSET        2   // 2 bytes
#define LOADER_HEADER_DATA_SIZE_OFFSET  4   // 4 bytes
#define LOADER_HEADER_ADDR_OFFSET       8   // 4 bytes

#define USB_UPGRADE_HEADER_SIZE 0x20
#define LOAD_ADDR 0x30300000 //need to alignment 32 bytes
#define OTP_USB_MEM_ADDR 0x20280000
#define LOADER_USB_MEM_ADDR 0x30280000
#define MAX_DATA_BUFFER 0x10000 // 64KB
#define USB_LOG_BUFFER_SIZE 0x1000

typedef enum
{
    COMMAND_TYPE_SCRIPT   =0x8080,
    COMMAND_TYPE_COPYDATA =0x8181,
    COMMAND_TYPE_NOP      =0x8888,
    COMMAND_TYPE_RESET    =0x8989
}USB_Upgrade_COMMAND_TYPE;

typedef struct _USB_Upgrade_Header
{
    U32 HeaderSize;
    U32 Version;
    U32 Command;
    U32 DataSize;
    U32 Address;
}USB_Upgrade_Header;

typedef struct logbuffer
{
    char logbuffer[USB_LOG_BUFFER_SIZE];
    int start;
    int end;
    int count;
}USB_LogBuffer;

USB_LogBuffer g_usb_log;

#define USB_LOG_DEBUG(_usb_buf_, msg...)\
    do{\
        sprintf(_usb_buf_, msg);\
        UBOOT_DEBUG(_usb_buf_);\
        usb_push_log(_usb_buf_, strlen(_usb_buf_));\
    }while(0)

#define USB_LOG_INFO(_usb_buf_, msg...)\
    do{\
        sprintf(_usb_buf_, msg);\
        UBOOT_INFO(_usb_buf_);\
        usb_push_log(_usb_buf_, strlen(_usb_buf_));\
    }while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if ((ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_USB == 1) || (ENABLE_MODULE_NETUPDATE == 1) || (CONFIG_RESCUE_ENV))
static show_Error_cb            fpShow_Error = NULL;
static show_Finish_cb           fpShow_Finish = NULL;
static show_LoadData_cb         fpShow_LoadData = NULL;
static show_StartUpgrading_cb   fpShow_StartUpgrading = NULL;
static show_Upgrading_cb        fpShow_upgrading = NULL;
#endif

#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
static U8 usb_data_buf[MAX_DATA_BUFFER] __attribute__((aligned(32)));
static U8 usb_ack_buf[4] __attribute__((aligned(32)));
static U8 usb_ack[4] __attribute__((aligned(32)));
static U8 usb_log_buf[USB_LOG_BUFFER_SIZE] __attribute__((aligned(32)));

static U8 *usb_load_addr = (U8 *)(LOAD_ADDR-USB_UPGRADE_HEADER_SIZE);
static USB_VAR *g_usb_var = (USB_VAR *)LOADER_USB_MEM_ADDR;
static vendorCmdStruct g_scsiVendorCmdVar;
static USB_Upgrade_Header Header;
#endif

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
extern void MDrv_USB_CmdLoop(void *pUsbStruct);
#endif

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
#if (ENABLE_MODULE_USB == 1)
static int LoadScript2DramByUsb(char *upgradeFile, char *downloadBuf);
#endif

#if (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1 || (CONFIG_RESCUE_ENV))
#if defined (CONFIG_OAD_IN_MBOOT)
static int LoadScript2DramByOAD(void);
#endif
static int LoadScript2DramByFS(char *upgradeFile, char *downloadBuf);
static int LoadScript2DramByFSFromVolume(char *upgradeFile, char *downloadBuf, char *volume);
#endif

#if (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1)||(ENABLE_MODULE_USB == 1 || (CONFIG_RESCUE_ENV))
static int _show_Error(unsigned int var);
static int _show_Finish(unsigned int var);
static int _show_LoadData(unsigned int var);
static int _show_StartUpgrading(unsigned int var);
static int _show_Upgrading(unsigned int var1, unsigned int var2);
static unsigned int _check_upgrade_script_cmd_count(char *buffer);
#endif
//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
#if (ENABLE_MODULE_NET == 1)
int do_mstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char* buffer = NULL;
    char *script_buf = NULL;
    char *next_line = NULL;
    char *ScriptFile = NULL;
    unsigned long iFilesize = 0;
    char *tmp = NULL;
    int BeAbort = 1;
    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_TFTP);

    ScriptFile= IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_SCRIPT_FILE_NAME;
    UBOOT_DEBUG("ScriptFile=%s\n",ScriptFile);

    buffer=(char *)malloc(SCRIPT_BUF_FOR_DO_MSTAR);
    if(buffer==NULL)
    {
        UBOOT_ERROR("no memory for command string!!\n");
        return -1;
    }

    // Set mstar commmand abort or not when cmd error.
    tmp = getenv ("CmdAbort");
    if (tmp != NULL)
    {
        BeAbort = (int)simple_strtol(tmp, NULL, 10);
        UBOOT_DEBUG("BeAbort=%d\n",BeAbort);
    }

    //Enable emac
    if(-1 == run_command("estart", 0))
    {
        UBOOT_ERROR("estart Fail \n");
        free(buffer);
        return -1;
    }

    // Set DHCP
    if(getenv("ipaddr") == NULL)
    {
        if(-1 == run_command("dhcp",0))
        {
            UBOOT_ERROR("please use the legal mac address.\n");
            free(buffer);// Fix CID:147674
            return -1;
        }
    }
    else
    {
        UBOOT_DEBUG("ipaddr=%s\n",getenv("ipaddr") );
    }
    // Load script
    memset(buffer, 0 , CMD_BUF);
    snprintf(buffer, CMD_BUF, "tftp %X %s", buffer,ScriptFile);
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {
        UBOOT_ERROR("cmd: %s\n",buffer);
        free(buffer);
        return -1;
    }

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    // Run script
    iFilesize = (unsigned long)simple_strtol(getenv("filesize"), NULL, 16);
    UBOOT_DEBUG("iFilesize=0x%x\n",(unsigned int)iFilesize);
    script_buf = buffer;
    script_buf[iFilesize]='\n'; script_buf[iFilesize+1]='%';
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        next_line = ParaReplace(next_line, (argc-2), &argv[1]);
        UBOOT_INFO ("\n>> %s \n", next_line);
        if(-1 == run_command(next_line, 0))
        {
            UBOOT_ERROR("command error!\n");
            if(BeAbort == 1)
            {
                free(buffer);
                return -1;
            }
        }
    }
    free(buffer);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif


int read_dynamic_script_size(char *ScriptBuf)
{
    UBOOT_TRACE("IN\n");
    // read dynamic script size
    unsigned int tmp_script_size=0;
    sscanf(ScriptBuf, "# Script Size = 0x%x #", &tmp_script_size);

    if(tmp_script_size==0)
    {
        UBOOT_DEBUG("no support dynamic script size\n");
        return -1;
    }

    UBOOT_DEBUG("# Script Size = 0x%x #",tmp_script_size);
    setScritSizeOfUpgradeImage(tmp_script_size);

    UBOOT_TRACE("OK\n");
    return 0;
}

#if (ENABLE_MODULE_USB == 1)
static int LoadScript2DramByUsb(char *upgradeFile, char *downloadBuf)
{
    char *buffer=NULL;
    int partition = 0;
    int device = 0;
    UBOOT_TRACE("IN\n");
    if(check_file_partition(upgradeFile,&device,&partition)!=0)
    {
        UBOOT_ERROR("fail : no file in usb\n");
        return -1;
    }
    buffer=malloc(CMD_BUF);
    if(buffer==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(buffer,0,CMD_BUF);

    //Load Script
    snprintf(buffer, CMD_BUF, "fatload usb %d:%d  %X %s %X",device, partition, downloadBuf, upgradeFile, getScritSizeOfUpgradeImage());
    UBOOT_DEBUG("cmd=%s\n",buffer);
    if(-1 == run_command(buffer, 0))
    {

        UBOOT_ERROR("Error: cmd: %s\n",buffer);
        free(buffer);//Coverity CID:149588
        return -1;
    }


    /*Check script file is ciphertext or not(start)*/
#if (CONFIG_SECURE_USB_UPGRADE==1)
     if(check_script_encrypt(downloadBuf) == TRUE)
     {
        setUpgradeMode(EN_UPDATE_MODE_USB_WITH_SEG_DECRYPTED);
        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "fileSegRSA usb %d:%d %s",device, partition, upgradeFile);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(buffer);
            return -1;
        }

        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(-1 == run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            free(buffer);
            return -1;
        }

        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            memset(buffer, 0 , CMD_BUF);
            snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd: %s\n",buffer);
                free(buffer);
                return -1;
            }
        }
     }
     else
     {
         //Jump to console if #define of secure upgrade is enable, but image is not encrypted
         UBOOT_ERROR("Error: Image format is not for secure upgrade!!\n");
         jump_to_console();
         return -1;
     }
#else
     {
        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            //load script again
            memset(buffer, 0 , CMD_BUF);
            snprintf(buffer, CMD_BUF, "fatload usb %d:%d  %X %s %X",device, partition, downloadBuf, upgradeFile, getScritSizeOfUpgradeImage());
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd: %s\n",buffer);
                free(buffer);//Coverity CID:149588
                return -1;
            }
        }

        //check script crc
        if(check_script_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_script_crc Fail !!\n");
            free(buffer);
            return -1;
        }

        if(check_full_usb_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_full_usb_crc Fail !!\n");
            free(buffer);
            return -1;
        }
     }
#endif
     Print_UsbInfo(downloadBuf);
     free(buffer);
     UBOOT_TRACE("OK\n");
     return 0;
}

int do_ustar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf = NULL;
    char *_script_buf = NULL;
    char *next_line = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_USB); // set the Upgrade mode
    /* Load Script */
    _show_LoadData(0);

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);


    if(setUpgradeFileName(upgradeFile)==-1)
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(_script_buf);
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        return -1;
    }
    if(LoadScript2DramByUsb(upgradeFile,script_buf) == -1)
    {
        deleteUpgradeFileName();
        free(_script_buf);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        _show_Error(0);
        return -1;
    }
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    set_dont_overwrite_byname(upgradeFile);
    #endif
    //Run Script

    //script_buf = (char *)UPGRADE_BUFFER_ADDR;

    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
        deleteUpgradeFileName();
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(_script_buf);
        UBOOT_ERROR("There are no any commands in the upgrade script file\n");
        _show_Error(0);
        return -1;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    _show_StartUpgrading(0);

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {

        UBOOT_INFO("\n>> %s \n", next_line);
        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)//filterCmd
        {
            if(run_command(next_line, 0) < 0)
            {
                deleteUpgradeFileName();
                setUpgradeMode(EN_UPDATE_MODE_NONE);
                free(_script_buf);
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                _show_Error(0);
                return -1;

            }
        }
    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    free(_script_buf);
    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;


}

#if !((CONFIG_MINIUBOOT) || (CONFIG_SECURE_USB_UPGRADE))
int do_udstar (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *cmdBuf=NULL;
    char *script_buf=NULL;
    char *pBuf=NULL;
    char *next_line=NULL;
    char *upgradeFile=NULL;
    char *env=NULL;
    int ret=0;
    unsigned int u32FileSize=0;
    int partition = 0;
    int device = 0;
    UBOOT_TRACE("IN\n");

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_DEBUG_SCRIPT_PATH;

    if(check_file_partition(upgradeFile,&device,&partition)!=0)
    {
        UBOOT_ERROR("no file in usb\n");
        return -1;
    }

    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);

    cmdBuf=malloc(CMD_BUF);
    if(cmdBuf==NULL)
    {
        UBOOT_ERROR("malloc fail\n");
        return -1;
    }
    memset(cmdBuf, 0 , CMD_BUF);
    snprintf(cmdBuf, CMD_BUF, "fatload usb %d:%d %X %s 1", device,partition,cmdBuf, upgradeFile);
    UBOOT_DEBUG("cmd=%s\n",cmdBuf);
    ret=run_command(cmdBuf, 0);
    if(ret!=0)
    {
        free(cmdBuf);
        UBOOT_ERROR("No script file exist\n");
        return -1;
    }

    memset(cmdBuf, 0, CMD_BUF);
    snprintf(cmdBuf, CMD_BUF, "fatfilesize usb %d:%d %s", device,partition, upgradeFile);
    UBOOT_DEBUG("cmd=%s\n",cmdBuf);
    ret = run_command(cmdBuf, 0);
    if(ret!=0)
    {
        free(cmdBuf);
        UBOOT_ERROR("get script file's size fail\n");
        return -1;
    }
    // Get file size. If larger then DRAM, don't load the whole. Just load its script.
    env=getenv("filesize");
    if(env==NULL)
    {
       free(cmdBuf);
       UBOOT_ERROR("get env 'filesize' fail\n");
       return -1;
    }
    u32FileSize = simple_strtol(env, NULL, 16);
    pBuf=malloc(u32FileSize);
    if(pBuf==NULL)
    {
        free(cmdBuf);
        UBOOT_ERROR("malloc for script file fail\n");
        return -1;
    }
    script_buf=pBuf;
    memset(script_buf,0,u32FileSize);

    memset(cmdBuf, 0, CMD_BUF);
    snprintf(cmdBuf, CMD_BUF, "fatload usb %d:%d %X %s %X",device,partition,script_buf,upgradeFile,u32FileSize);
    UBOOT_DEBUG("cmd=%s\n",cmdBuf);
    ret = run_command(cmdBuf, 0);
    if(ret!=0)
    {
        free(pBuf);
        free(cmdBuf);
        return -1;
    }

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>>1 %s \n", next_line);

        if(strstr((char *)next_line,(char *)"run")!=NULL)
        {
            UBOOT_INFO ("skip cmd 'run' \n");
            continue;
        }

        ret=run_command(next_line, 0);
        if(ret!=0)
        {
            free(pBuf);
            free(cmdBuf);
            UBOOT_ERROR("run cmd '%s' fail\n",next_line);
            return -1;
        }
    }

    free(cmdBuf);
    free(pBuf);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif //#ifndef CONFIG_MINIUBOOT && CONFIG_SECURE_USB_UPGRADE

#endif //#if (ENABLE_MODULE_USB == 1)

#if (ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_NETUPDATE == 1 || (CONFIG_RESCUE_ENV))
#if defined (CONFIG_OAD_IN_MBOOT)
static int LoadScript2DramByOAD(void)
{
    char buffer[CMD_BUF]="\0";
    char* p_str = NULL;
    unsigned int  u32Freq = 0;
    unsigned int  u32Pid  = 0;
    unsigned char u8Band = 0;
    unsigned char u8DVBC = 0;
    unsigned char u8DVBS = 0;
    unsigned int  u32QAM = 0;
    unsigned int  u16SYMBOLRATE = 0;
    unsigned char u8SDTT_OAD  = 0;
    unsigned int  u32GroupId = 0;
    unsigned int  u16Plp  = 0xffff;
    UBOOT_TRACE("IN\n");

    // Frequency
    p_str = getenv("oad_upgrade_freq");
    if(p_str == NULL)
    {
        return -1;
    }
    u32Freq = simple_strtoul (p_str, NULL, 10);
    // PID
    p_str = getenv("oad_upgrade_pid");
    if(p_str == NULL)
    {
        return -1;
    }
    u32Pid = simple_strtoul (p_str, NULL, 10);
    // Group ID
    p_str = getenv("oad_group_pid");
    if(p_str != NULL)
    {
        u32GroupId = simple_strtoul (p_str, NULL, 10);
    }
    // OAD_DVBC
    p_str = getenv("oad_upgrade_dvbc");
    if(p_str != NULL)
    {
        u8DVBC = simple_strtoul (p_str, NULL, 10);
    }
    // OAD DVBS
    p_str = getenv("oad_upgrade_dvbs");
    if(p_str != NULL)
    {
        u8DVBS = simple_strtoul (p_str, NULL, 10);
    }

    if(u8DVBC == 1)
    {
        p_str = getenv("oad_upgrade_qam");
        u32QAM = simple_strtoul (p_str, NULL, 10);
        if(p_str == NULL)
        {
            return -1;
        }
        p_str = getenv("oad_upgrade_sym");
        u16SYMBOLRATE = simple_strtoul (p_str, NULL, 10);
        if(p_str == NULL)
        {
            return -1;
        }
        snprintf(buffer, CMD_BUF, "oaddvbc %d %d %d 0x%x 0x%x", u32Freq, u32QAM, u16SYMBOLRATE, u32Pid, u32GroupId);
    }
    else if(u8DVBS == 1)
    {
        p_str = getenv("oad_upgrade_sym");
        u16SYMBOLRATE = simple_strtoul (p_str, NULL, 10);
        if(p_str == NULL)
        {
            return -1;
        }
        snprintf(buffer, CMD_BUF, "oaddvbs %d %d 0x%x 0x%x", u32Freq, u16SYMBOLRATE, u32Pid, u32GroupId);
    }
    else // DVBT
    {
        p_str = getenv("oad_upgrade_band");
        if(p_str == NULL)
        {
            return -1;
        }

        u8Band = simple_strtoul (p_str, NULL, 10);
        if((u8Band < 1) || (u8Band > 3))
        {
            UBOOT_ERROR("wrong band: %d \n",u8Band);
            return FALSE;
        }
        p_str = getenv("oad_is_sdtt");
        if(p_str != NULL)
        {
            u8SDTT_OAD = simple_strtoul (p_str, NULL, 10);
        }
        p_str = getenv("oad_upgrade_plp");
        if(p_str != NULL)
        {
            u16Plp = simple_strtoul (p_str, NULL, 10);
        }
        snprintf(buffer, CMD_BUF, "oad %d %d 0x%x 0x%x %d 0x%x", u32Freq, u8Band, u32Pid, u32GroupId, u8SDTT_OAD, u16Plp); //do OAD: download from ts to mamory
        printf("Get CMD_BUF %d %x %x\n",u32Freq,u32Pid,u32GroupId);
    }
     //do OAD: download from ts to mamory
    if(run_command(buffer,0)!=0)
    {
        UBOOT_ERROR("do %s fail\n",buffer);
        return -1;
    }
    UBOOT_TRACE("OK\n");

    return 0;
}
#endif

static int LoadScript2DramByFS(char *upgradeFile, char *downloadBuf)
{
    char buffer[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    memset(buffer, 0 , CMD_BUF);
    //load script
    if(0 != LoadScript2DramByFSFromVolume(upgradeFile, downloadBuf, OAD))
    {
        UBOOT_ERROR("Error: LoadScript2DramByFSFromVolume Fail !!\n");
        return -1;
    }
#if (CONFIG_SECURE_USB_UPGRADE==1)
    if(check_script_encrypt(downloadBuf) == TRUE)
    {
        setUpgradeMode(EN_UPDATE_MODE_NET_WITH_SEG_DECRYPTED);
        snprintf(buffer, CMD_BUF, "fileSegRSA FS NULL %s",upgradeFile);
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(0 != run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            return -1;
        }

        memset(buffer, 0 , CMD_BUF);
        snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
        UBOOT_DEBUG("cmd=%s\n",buffer);
        if(0 != run_command(buffer, 0))
        {
            UBOOT_ERROR("Error: cmd: %s\n",buffer);
            return -1;
        }

        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            memset(buffer, 0 , CMD_BUF);
            snprintf(buffer, CMD_BUF, "filepartload %x %s %x %x",downloadBuf, upgradeFile, 0, getScritSizeOfUpgradeImage());
            UBOOT_DEBUG("cmd=%s\n",buffer);
            if(-1 == run_command(buffer, 0))
            {
                UBOOT_ERROR("Error: cmd: %s\n",buffer);
                free(buffer);
                return -1;
            }
        }
    }
    else
    {
        //Jump to console if #define of secure upgrade is enable, but image is not encrypted
        UBOOT_ERROR("Error: Image format is not for secure upgrade!!\n");
        jump_to_console();
        return -1;
    }
#else
    {
        // read dynamic script size
        if(read_dynamic_script_size(downloadBuf) == 0)
        {
            //load script again
            if(0 != LoadScript2DramByFSFromVolume(upgradeFile, downloadBuf, OAD))
            {
                UBOOT_ERROR("Error: LoadScript2DramByFSFromVolume Fail !!\n");
                return -1;
            }
        }

        //check script crc
        if(check_script_crc(upgradeFile,downloadBuf)==-1)
        {
            UBOOT_ERROR("Error: check_script_crc Fail !!\n");
            return -1;
        }
    }
#endif
    UBOOT_TRACE("OK\n");
    return 0;
}

static int LoadScript2DramByFSFromVolume(char *upgradeFile, char *downloadBuf, char *volume)
{
    UBOOT_TRACE("IN\n");

    if(upgradeFile==NULL || downloadBuf==NULL)
    {
        UBOOT_ERROR("Input NULL\n");
        return -1;
    }

    if(NULL == volume)
    {
        UBOOT_ERROR("volume is NULL\n");
        return -1;
    }
    if(vfs_mount(volume) == -1)
    {
        UBOOT_ERROR("Error: vfs_mount Fail !!\n");
        return -1;
    }
    setUpgradeFsMountName(volume);
    if(vfs_read((void *)downloadBuf,upgradeFile,0,getScritSizeOfUpgradeImage()) == -1)
    {
        UBOOT_ERROR("Error: vfs_read Fail !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

int do_nstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf  = NULL;
    char *_script_buf = NULL;
    char *next_line   = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_OAD); // set the Upgrade mode

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;
    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);
    if(setUpgradeFileName(upgradeFile)==-1)
    {
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        return -1;
    }

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize+512);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);
    _show_LoadData(0);
    // load script
    if(LoadScript2DramByFS(upgradeFile,script_buf) != 0)
    {
        goto OAD_FAIL;
    }

    //set dont overwrite
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    dont_overwrite_enable();
    do_force_overwrite_enable();
    #endif
    _show_StartUpgrading(0);

    //run script
    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
       UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       goto OAD_FAIL;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)
        {
#if ENABLE_DISPLAY_PICTURE_UPDATE
            check_filepartload_cmd(next_line, OAD);
#endif
            if(run_command(next_line, 0) < 0)
            {
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                goto OAD_FAIL;
            }
        }

    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }

    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;

OAD_FAIL:
   _show_Error(0);
    deleteUpgradeFileName();
    setUpgradeMode(EN_UPDATE_MODE_NONE);
    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }
    return -1;
}

#define BRICK_BACKUP_VOLUME "brickbackup"
int do_bstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf  = NULL;
    char *_script_buf = NULL;
    char *next_line   = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_OAD); // set the Upgrade mode

    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;
    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);
    if(setUpgradeFileName(upgradeFile)==-1)
    {
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        return -1;
    }

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize+512);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);
    _show_LoadData(0);

    // load script
    if(LoadScript2DramByFSFromVolume(upgradeFile,script_buf, BRICK_BACKUP_VOLUME) != 0)
    {
        goto OAD_FAIL;
    }

    //set dont overwrite
#if(ENABLE_MODULE_DONT_OVERWRITE==1)
    dont_overwrite_enable();
    do_force_overwrite_enable();
#endif
    _show_StartUpgrading(0);

    //run script
    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
       UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       goto OAD_FAIL;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)
        {
#if ENABLE_DISPLAY_PICTURE_UPDATE
            check_filepartload_cmd(next_line, BRICK_BACKUP_VOLUME);
#endif
            if(run_command(next_line, 0) < 0)
            {
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                goto OAD_FAIL;
            }
        }
    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }

    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;

OAD_FAIL:
    _show_Error(0);
    deleteUpgradeFileName();
    setUpgradeMode(EN_UPDATE_MODE_NONE);
    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }
    return -1;
}

int do_ostar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *script_buf  = NULL;
    char *_script_buf = NULL;
    char *next_line   = NULL;
    char *upgradeFile = NULL;
    unsigned int scriptFileSize=0;
    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;

    UBOOT_TRACE("IN\n");
    setUpgradeMode(EN_UPDATE_MODE_OAD); // set the Upgrade mode

#if defined (CONFIG_OAD_IN_MBOOT)
    scriptFileSize = 0;
    _script_buf = NULL;
    upgradeFile = NULL;
    script_buf=(char *)UPGRADE_BUFFER_ADDR;
    if(LoadScript2DramByOAD()!= 0)
    {
         UBOOT_ERROR("OAD Download in Mboot Fail!!\n");
         return -1;
    }
#else
    upgradeFile = IS_ARG_AVAILABLE_SCRIPT_FILE(argc) ? ARG_SCRIPT_FILE(argv) : DEFAULT_BIN_PATH;
    UBOOT_DEBUG("upgradeFile=%s\n",upgradeFile);
    if(setUpgradeFileName(upgradeFile)==-1)
    {
        UBOOT_ERROR("Error> set upgrade fail fail\n");
        _show_Error(0);
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        return -1;
    }

    scriptFileSize=MAX_SCRIPT_SIZE;
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return -1;
    }

    _script_buf=malloc(scriptFileSize+512);
    if(_script_buf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return -1;
    }
    script_buf=_script_buf;
    memset(script_buf,0,scriptFileSize);
    _show_LoadData(0);
    // load script
    if(LoadScript2DramByFS(upgradeFile,script_buf) != 0)
    {
        goto OAD_FAIL;
    }
#endif

    //set dont overwrite
    #if(ENABLE_MODULE_DONT_OVERWRITE==1)
    dont_overwrite_enable();
    do_force_overwrite_enable();
    #endif
    _show_StartUpgrading(0);

    //run script
    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
       UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       goto OAD_FAIL;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);

    // because application member is lazy, so we must set "sync_mmap 1" after burn code,
    run_command("setenv sync_mmap 1;saveenv", 0);
    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO ("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        _show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)
        {
#if ENABLE_DISPLAY_PICTURE_UPDATE
            check_filepartload_cmd(next_line, OAD);
#endif
            if(run_command(next_line, 0) < 0)
            {
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                goto OAD_FAIL;
            }
        }

    }

    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }

    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }

    _show_Finish(0);
    UBOOT_TRACE("OK\n");
    return 0;

OAD_FAIL:
   _show_Error(0);
    deleteUpgradeFileName();
    setUpgradeMode(EN_UPDATE_MODE_NONE);
    if(_script_buf!=NULL)
    {
        free(_script_buf);
    }
    return -1;
}

#endif

#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
int do_rstar(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
//=========================================================
    #define ENV_RECOVERY "env_recovery"
    #define SCRIPT_SIZE 0x1000

    char *cEnvRecoveryOffset = getenv(ENV_RECOVERY);
    char *script_buf = NULL;
    char *_script_buf = NULL;
    char *next_line = NULL;

    unsigned int progress_total_cnt = 0;
    unsigned int progress_cnt = 0;
    unsigned int u32RecoveryOffset = 0;

    UBOOT_TRACE("IN\n");

    if (cEnvRecoveryOffset == NULL)
    {
        UBOOT_ERROR("Recovery offset not found\n");
       //_show_Error(0);
       return -1;
    }

    u32RecoveryOffset = (unsigned int)strtol(cEnvRecoveryOffset, NULL, 16);

    _script_buf=malloc(SCRIPT_SIZE);

    if(_script_buf==NULL){
        return -1;
    }

    script_buf = _script_buf;
    memset(script_buf ,0 ,SCRIPT_SIZE);
    raw_read((unsigned int)script_buf, u32RecoveryOffset, SCRIPT_SIZE);

    //Run Script

    progress_total_cnt=_check_upgrade_script_cmd_count(script_buf);
    if(progress_total_cnt==0)
    {
        setUpgradeMode(EN_UPDATE_MODE_NONE);
        free(_script_buf);
        UBOOT_ERROR("There are no any commands in the upgrade script file\n");
       //_show_Error(0);
       return -1;
    }
    UBOOT_DEBUG("progress_total_cnt=%d\n",progress_total_cnt);


    while ((next_line = get_script_next_line(&script_buf)) != NULL)
    {
        UBOOT_INFO("\n>> %s \n", next_line);

        progress_cnt++;
        UBOOT_DEBUG("progress_cnt=%d,progress_total_cnt=%d\n",progress_cnt,progress_total_cnt);
        //_show_Upgrading(progress_cnt,progress_total_cnt);
        if(check_skip_cmd(next_line)== FALSE)//filterCmd
        {
            if(run_command(next_line, 0) < 0)
            {
                free(_script_buf);
                UBOOT_ERROR("Error> cmd: %s Fail !!\n", next_line);
                //_show_Error(0);
                return -1;

            }
        }
    }
#if 0
    if(deleteUpgradeFileName()==-1)
    {
        UBOOT_ERROR("delete upgrade name fail\n");
    }
#endif
    free(_script_buf);
    UBOOT_TRACE("OK\n");
    return 0;

}
#endif

#if ((ENABLE_MODULE_OAD == 1) || (ENABLE_MODULE_USB == 1) || (ENABLE_MODULE_NETUPDATE == 1))
int do_mboot_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    #define MBOOT_BUF_ADDR argv[1]
    #define MBOOT_SIZE argv[2]
    char ciscmd[CMD_BUF]="\0";
    UBOOT_TRACE("IN\n");
    unsigned int u32DramBuf = simple_strtoul(MBOOT_BUF_ADDR, NULL, 16);
    unsigned int u32MbootSize = simple_strtoul(MBOOT_SIZE, NULL, 16);

    UBOOT_DEBUG("u32DramBuf   : 0x%x\n",u32DramBuf);
    UBOOT_DEBUG("u32MbootSize : 0x%x\n",u32MbootSize);

    if(mboot_raw_io_Config()!= 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    if(get_raw_status()==E_RAW_DATA_IN_SPI)
    {
        UBOOT_INFO("Upgrading Mboot.bin into SPI....\n");
        if(raw_write(u32DramBuf,0,u32MbootSize)!= 0)
        {
            UBOOT_ERROR("raw_write fail!\n");
            return -1;
        }
    }
    else if(get_raw_status()==E_RAW_DATA_IN_NAND)
    {
        UBOOT_INFO("Upgrading RomBoot.bin(MBOOT) into NAND....\n");

      #if (ENABLE_EAGLE) || (ENABLE_EMERALD) || (ENABLE_NIKON) || (ENABLE_KENYA)
    	sprintf(ciscmd,"ncisbl 0x%X 0x%X 0x%X 0x%X 0x%X", \
    	        u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32MbootSize);
      #elif (ENABLE_NUGGET)
    	sprintf(ciscmd,"ncishash 0x%X 0x%X 0x%X 0x%X 0x3800 0x%X 0x%X 0x3200", \
                u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32DramBuf+0x33800,u32MbootSize-0x3800);
      #elif (ENABLE_MILAN) || (ENABLE_MARLON) || (ENABLE_MULAN)
    	sprintf(ciscmd,"ncishash 0x%X 0x%X 0x%X 0x%X 0x3800 0x%X 0x%X", \
                u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32DramBuf+0x33800,u32MbootSize-0x3800);
      #else //(ENABLE_EIFFEL) || (ENABLE_NIKE) || (ENABLE_MADISON) || (ENABLE_CLIPPERS) || (ENABLE_MIAMI) || (ENABLE_NADAL) || (ENABLE_EINSTEIN)
    	sprintf(ciscmd,"ncishash 0x%X 0x%X 0x%X 0x%X 0x2800 0x%X 0x%X", \
    	        u32DramBuf,u32DramBuf+0x10000,u32DramBuf+0x20000,u32DramBuf+0x30000,u32DramBuf+0x32800,u32MbootSize-0x2800);
      #endif
        if(run_command(ciscmd,0)!=0)
        {
            UBOOT_ERROR("CMD: ncisbl/ncishash  fail!\n");
            return -1;
        }
        setenv("CISRefresh",NULL);
        saveenv();
    }
    else if(get_raw_status()==E_RAW_DATA_IN_MMC)
    {
        UBOOT_INFO("Upgrading Mboot.bin into EMMC....\n");
        //todo
    }
    else
    {
        UBOOT_ERROR("wrong raw_io_setting !!\n");
        return -1;
    }

    UBOOT_TRACE("OK\n");
    return 0;
}

void RegisterCBShowError(show_Error_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_Error=cb;
    UBOOT_TRACE("OK\n");
}

void RegisterCBShowFinish(show_Finish_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_Finish=cb;
    UBOOT_TRACE("OK\n");
}

void RegisterCBShowLoadData(show_LoadData_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_LoadData=cb;
    UBOOT_TRACE("OK\n");
}

void RegisterCBShowStartUpgrading(show_StartUpgrading_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_StartUpgrading=cb;
    UBOOT_TRACE("OK\n");
}
void RegisterCBShowUpgrading(show_Upgrading_cb cb)
{
    UBOOT_TRACE("IN\n");
    fpShow_upgrading=cb;
    UBOOT_TRACE("OK\n");
}

void UnRegisterShowCb(void)
{
    UBOOT_TRACE("IN\n");
    fpShow_Error=NULL;
    fpShow_Finish=NULL;
    fpShow_LoadData=NULL;
    fpShow_StartUpgrading=NULL;
    fpShow_upgrading=NULL;
    UBOOT_TRACE("OK\n");
}


static int _show_Error(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_Error!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_Error(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'error' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int _show_Finish(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_Finish!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_Finish(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'finish' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int _show_LoadData(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_LoadData!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_LoadData(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'LoadData' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}
static int _show_StartUpgrading(unsigned int var)
{
#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_StartUpgrading!=NULL)
    {
        UBOOT_TRACE("OK\n");
        return fpShow_StartUpgrading(var);
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'StartUpgrading' phase\n");
        UBOOT_TRACE("OK\n");
        return 0;
    }
#else
    return 0;
#endif
}

static int _show_Upgrading(unsigned int var1, unsigned int var2)
{
    int ret = 0;

#if (ENABLE_MODULE_DISPLAY_OSD==1)
    UBOOT_TRACE("IN\n");
    if(fpShow_upgrading!=NULL)
    {
        char filesize[CMD_BUF]="\0";
        memset(filesize, 0 , CMD_BUF);
        snprintf(filesize, CMD_BUF, "%s", getenv("filesize"));
        ret = fpShow_upgrading(var1, var2);
        setenv("filesize", filesize);
        UBOOT_TRACE("OK\n");
    }
    else
    {
        UBOOT_DEBUG("No call back function for 'Upgrading' phase\n");
        UBOOT_TRACE("OK\n");
    }
#endif
    return ret;
}

static unsigned int _check_upgrade_script_cmd_count(char *buffer)
{
    unsigned int scriptFileSize=0;
    unsigned int amount=0;
    char *tempBuf=NULL;

    UBOOT_TRACE("IN\n");
    scriptFileSize=getScritSizeOfUpgradeImage();
    if(scriptFileSize==0)
    {
        UBOOT_ERROR("The size of script file is zero \n");
        return 0;
    }

    tempBuf=malloc(scriptFileSize);
    if(tempBuf==NULL)
    {
        UBOOT_ERROR("malloc fail \n");
        return 0;
    }
    memcpy(tempBuf,buffer,scriptFileSize);
    amount=check_script_cmd_count(tempBuf);
    free(tempBuf);

    UBOOT_TRACE("OK\n");
    return amount;
}

#endif

#ifdef CONFIG_BOOT_SEQUENCE_FORCE_USB_UPGRADE_LOADER
void usb_push_log(char *data, int len)
{
    strncat(g_usb_log.logbuffer, data, (len%USB_LOG_BUFFER_SIZE));
}

int usb_get_log(char *data, int len)
{
    int buf_len = strlen(g_usb_log.logbuffer);
    int get_len = (buf_len-len) > 0 ? len : buf_len;
    strncpy(data, g_usb_log.logbuffer, get_len);
    return get_len;
}

int USB_GET_LOG(U32 u32ReqLen)
{
    int ret = 0;
    ret=usb_get_log(usb_log_buf, u32ReqLen);
    memset(g_usb_log.logbuffer, 0, USB_LOG_BUFFER_SIZE);
    return ret;
}

void USB_Upgrade_Active_USB(void)
{
    UBOOT_TRACE("IN\n");
    USB_DRC_Interrupt(g_usb_var);
    MDrv_USB_CmdLoop((void*)(g_usb_var));
    UBOOT_TRACE("OK\n");
}

void USB_Upgrade_Write_Ack(void)
{
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    UBOOT_TRACE("IN\n");
    usb_ack[0] = 0xd0;
    usb_ack[1] = 0xd0;
    usb_ack[2] = 0xd0;
    usb_ack[3] = 0xd0;

    MDrv_USB_Write(usb_ack, 4, g_usb_var);
    while(1)
    {
        USB_Upgrade_Active_USB();
        if(g_scsiVendorCmdVar.ptrPPB2Buf[0] == 0)
           break;
    }
    USB_LOG_INFO(tempBuf, "USB ack OK\n\n");
}

void USB_Upgrade_Write_NACK(void)
{
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    UBOOT_TRACE("IN\n");
    usb_ack[0] = 0xee;
    usb_ack[1] = 0xee;
    usb_ack[2] = 0xee;
    usb_ack[3] = 0xee;

    MDrv_USB_Write(usb_ack, 4, g_usb_var);
    while(1)
    {
        USB_Upgrade_Active_USB();
        if(g_scsiVendorCmdVar.ptrPPB2Buf[0] == 0)
           break;
    }
    USB_LOG_INFO(tempBuf, "USB nack OK\n\n");
}

U32 USB_Upgrade_Receive_Packet(void)
{
    U32 PacketLen = 0;
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    
    USB_LOG_DEBUG(tempBuf, "USB Receive Packet\n");
    memset(&Header, 0, sizeof(USB_Upgrade_Header));
    while(1)
    {
        USB_Upgrade_Active_USB();
        PacketLen += MDrv_USB_Read((U8*)(usb_load_addr), MAX_DATA_BUFFER, g_usb_var);

        if(PacketLen >= 16)
            break;
    }
    USB_LOG_DEBUG(tempBuf, "first packet len    = 0x%x\n", PacketLen);
    Header.HeaderSize = usb_load_addr[LOADER_HEADER_SIZE_OFFSET];
    if(Header.HeaderSize != USB_UPGRADE_HEADER_SIZE)
    {
        USB_LOG_DEBUG(tempBuf, "Unsupport Header Type : 0x%x\n", Header.HeaderSize);
        return -1;
    }
    Header.Version = usb_load_addr[LOADER_HEADER_VERSION_OFFSET];
    Header.Command = (U32)(usb_load_addr[LOADER_HEADER_CMD_OFFSET])
               + (U32)(usb_load_addr[LOADER_HEADER_CMD_OFFSET+1] <<  8);
    Header.DataSize = (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET])
               + (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET+1] <<  8)
               + (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET+2] << 16)
               + (U32)(usb_load_addr[LOADER_HEADER_DATA_SIZE_OFFSET+3] << 24);
    Header.Address = (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET])
               + (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET+1] <<  8)
               + (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET+2] << 16)
               + (U32)(usb_load_addr[LOADER_HEADER_ADDR_OFFSET+3] << 24);

    USB_LOG_DEBUG(tempBuf, "Header.HeaderSize   = 0x%x\n", Header.HeaderSize);
    USB_LOG_DEBUG(tempBuf, "Header.Version      = 0x%x\n", Header.Version);
    USB_LOG_DEBUG(tempBuf, "Header.Command      = 0x%x\n", Header.Command);
    USB_LOG_DEBUG(tempBuf, "Header.DataSize     = 0x%x\n", Header.DataSize);
    USB_LOG_DEBUG(tempBuf, "Header.Address      = 0x%x\n", Header.Address);

    if(Header.DataSize + Header.HeaderSize > PacketLen)
    {
        while(1)
        {
            USB_DRC_Interrupt(g_usb_var);
            MDrv_USB_CmdLoop((void*)(g_usb_var));
            PacketLen += MDrv_USB_Read((MS_U8*)(usb_load_addr + PacketLen), MAX_DATA_BUFFER, g_usb_var);

            if(PacketLen)
            {
                if(PacketLen >= Header.DataSize + Header.HeaderSize)
                    break;
            }
        }
    }
    USB_LOG_DEBUG(tempBuf, "final packet len    = 0x%x\n", PacketLen);
    UBOOT_TRACE("OK\n");
    return 0;
}

void USB_Upgrade_Process(void)
{
    char buf[CMD_BUF];
    int ret = 0;
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    UBOOT_TRACE("IN\n");
    switch (Header.Command)
    {
        case COMMAND_TYPE_SCRIPT:
            // ignore the command start from "#" and "%"
            if(strncmp((const char *)&usb_load_addr[Header.HeaderSize], "#", 1)==0 || 
                strncmp((const char *)&usb_load_addr[Header.HeaderSize], "%", 1)==0)
            {
                USB_Upgrade_Active_USB();
                usb_load_addr[Header.HeaderSize + Header.DataSize] = 0x00;
                USB_LOG_INFO(tempBuf, ">> Ignore Run command : %s\n", &usb_load_addr[Header.HeaderSize]);
                break;
            }
            if(strncmp((const char *)&usb_load_addr[Header.HeaderSize], "reset", 5)==0)
            {
                USB_Upgrade_Write_Ack();
            }
            usb_load_addr[Header.HeaderSize + Header.DataSize] = 0x00;
            USB_LOG_INFO(tempBuf, ">> Run command : %s\n", &usb_load_addr[Header.HeaderSize]);
            USB_Upgrade_Active_USB();
            ret = run_command((const char *)&usb_load_addr[Header.HeaderSize], 0);
            if( ret == -1 )
            {
                USB_Upgrade_Write_NACK();
            }
            USB_Upgrade_Active_USB();
            break;
        case COMMAND_TYPE_COPYDATA:
            memset(buf, 0, CMD_BUF);
            sprintf(buf, "%x", Header.DataSize);
            setenv("filesize", buf);
            memcpy ((void *)Header.Address, &usb_load_addr[Header.HeaderSize], Header.DataSize);
            USB_LOG_INFO(tempBuf, ">> Copy Data To Dram, addr=0x%x, filesize=0x%s\n", Header.Address, getenv("filesize"));
            break;
        case COMMAND_TYPE_NOP:
            USB_LOG_INFO(tempBuf, ">> NOP : Do nothing\n");
            break;
        case COMMAND_TYPE_RESET:
            USB_LOG_INFO(tempBuf, ">> Run command : reset\n");
            USB_Upgrade_Write_Ack();
            run_command("reset", 0);
            break;
        default:
            USB_LOG_INFO(tempBuf, ">> Unknow Command Type: 0x%x\n", Header.Command);
            USB_Upgrade_Write_NACK();
            break;
    }
    USB_LOG_INFO(tempBuf, "USB Upgrade Process OK\n");
}

void USB_Upgrade_Init(void)
{
    UBOOT_TRACE("IN\n");
    memcpy((void *)LOADER_USB_MEM_ADDR, (const void *)OTP_USB_MEM_ADDR, (LOAD_ADDR-LOADER_USB_MEM_ADDR));
    otgInitMassBuffer(g_usb_var);

    g_scsiVendorCmdVar.ucptrDataBuf = (U8 *)(usb_data_buf);
    g_scsiVendorCmdVar.ptrPPB2Buf= (U8 *)(usb_ack_buf);
    g_scsiVendorCmdVar.ptrTmpHifBuf= (U8 *)(usb_log_buf);
    g_usb_var->ptrVCmdStruct = &g_scsiVendorCmdVar;

    memset(g_scsiVendorCmdVar.ptrPPB2Buf, 0x0, 0x4);
    memset(&g_usb_log, 0, sizeof(USB_LOG_BUFFER_SIZE));
    memset(g_usb_log.logbuffer, 0, USB_LOG_BUFFER_SIZE);
    g_usb_log.start = 0;
    g_usb_log.end = 0;
    g_usb_log.count = 0;
    USB_Upgrade_Write_Ack();
    UBOOT_TRACE("OK\n");
}

void USB_Upgrade_Func(void)
{
    char tempBuf[200] = {0};
    memset(tempBuf, 0, 200);
    USB_LOG_INFO(tempBuf, "\n========================================\n");
    USB_LOG_INFO(tempBuf, "===========Usb upgrade mode start======\n");
    USB_LOG_INFO(tempBuf, "========================================\n\n");
    USB_Upgrade_Init();
    while(1)
    {
        if(-1 != USB_Upgrade_Receive_Packet())
        {
            USB_Upgrade_Process();
        }
        USB_Upgrade_Write_Ack();
    }
}
#endif

#ifdef CONFIG_MSTAR_NS_UBOOT
#define TEE_KEY_BANK_OFFSET       0x2000
#define REE_KEY_BANK_OFFSET       0x3000
#define KEYLADDER_KEY_BANK_OFFSET 0x4000
#define KEE_TYPE_OFFSET           0x6000

#define KEY_BANK_ROM_SIZE         0x1000

int do_keybank_upgrade(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char *KeyBankName = argv[1];
    U32 u32Addr = simple_strtoul(argv[2], NULL, 16);
    U32 u32KeyOffset = 0;

    UBOOT_TRACE("IN\n");
    if(argc!=3)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    if(MsApiChunkHeader_GetValue(CH_CUSTOMER_KEY_BANK_ROM_OFFSET,&u32KeyOffset)== -1)
    {
        UBOOT_ERROR("Secure_Drv_Init fail\n");
        return -1;
    }
    if(0==strncmp(KeyBankName, "TEE", 2))
    {
        u32KeyOffset = u32KeyOffset + TEE_KEY_BANK_OFFSET;
    }
    else if(0==strncmp(KeyBankName, "REE", 2))
    {
        u32KeyOffset = u32KeyOffset + REE_KEY_BANK_OFFSET;
    }
    else if(0==strncmp(KeyBankName, "KL", 2))
    {
        u32KeyOffset = u32KeyOffset + KEYLADDER_KEY_BANK_OFFSET;
    }
    else if(0==strncmp(KeyBankName, "KEYTYPE", 2))
    {
        u32KeyOffset = u32KeyOffset + KEE_TYPE_OFFSET;
    }
    else
    {
        UBOOT_DEBUG("Unknow key bank name\n");
        return -1;
    }

    UBOOT_DEBUG("KeyBankName:%s\n", KeyBankName);
    UBOOT_DEBUG("u32Addr:0x%x\n", u32Addr);
    UBOOT_DEBUG("u32KeyOffset:0x%x\n", u32KeyOffset);

    if(raw_write(u32Addr, u32KeyOffset, KEY_BANK_ROM_SIZE)!= 0)
    {
        UBOOT_ERROR("Write KeyBank Fail\n");
        return -1;
    }
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif

