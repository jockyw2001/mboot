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
#include <malloc.h>
#include <MsDebug.h>
#include <drvMSPI.h>
#include <drvSYS.h>
#include <ShareType.h>
#include <MsVfs.h>
#include <CusConfig.h>
#include <drvGPIO.h>

//Dolby 
#if(ENABLE_DOLBY==1)

int do_dolby_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;    
    MSPI_CLKConfig ptCLKConfig;
    MS_U8 * dolby_data=NULL;//0x20400000;
    MS_U8 * dolby_temp=NULL;//0x20400000;
    MS_U32 dolby_size=0;//0x81F2A8;
    
    UBOOT_TRACE("IN\n");
    char PathBuf[CMD_BUF] = "\0";
    mdrv_gpio_init();
    mdrv_gpio_set_high(PAD_GPIO2);
    mdelay(10);
    mdrv_gpio_set_low(PAD_GPIO2);
    mdelay(10);
    mdrv_gpio_set_high(PAD_GPIO2);
    vfs_mount(CONFIG);
    snprintf(PathBuf, sizeof(PathBuf), "%s/fpga/dolby_fpga.sof",CONFIG_PATH);
    UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
    dolby_size = vfs_getsize(PathBuf);        
    UBOOT_DEBUG("u32dolby_size=0x%x\n",(unsigned int)dolby_size);
    dolby_data=(MS_U8 *)malloc(dolby_size);
    if(dolby_data==NULL)
    {
        UBOOT_ERROR("malloc for dolby_data fail==========\n");
        return 0;
    }
    if(dolby_size==0 || (vfs_read(dolby_data,PathBuf,0,dolby_size)!=0))
    {
        UBOOT_ERROR("Error: vfs_read Fail\n");
        free(dolby_data);
        return 0;
    }
    dolby_temp=dolby_data;
    MDrv_SYS_SetPadMux(E_TS0_PAD_SET,E_MSPI_PAD_ON);
    memset(&ptCLKConfig, 0, sizeof(ptCLKConfig));
    ptCLKConfig.BClkPolarity=0;
    ptCLKConfig.BClkPhase=0;
    ptCLKConfig.U8Clock=5;
    
    ret=MDrv_MSPI_Init_Ext(0);
    if(ret!=E_MSPI_OK)
    {
        UBOOT_ERROR("MDrv_MSPI_Init_Ext error %d\n",ret);
        free(dolby_data);
        return 0;
    }
    MDrv_MasterSPI_CsPadConfig(0,0xFF);
    MDrv_MSPI_CLKConfig(&ptCLKConfig);
    MDrv_MasterSPI_MaxClkConfig(0,10);
    MDrv_MSPI_SlaveEnable(true);
    while(dolby_size>0)
        {
            if(dolby_size>0xFFFF)
            {
                UBOOT_DEBUG("dolby addr [0x%x] size[0x%x] \n",(unsigned int)dolby_temp,(unsigned int)dolby_size);
                MDrv_MSPI_SlaveEnable(false);
                MDrv_MSPI_Write(dolby_temp,0xFFFF);
                dolby_temp=dolby_temp+(0xFFFF);
                dolby_size=dolby_size-(0xFFFF);
                    
            }
            else
            {
                UBOOT_DEBUG("---dolby addr [0x%x] size[0x%x] \n",(unsigned int)dolby_temp,(unsigned int)dolby_size);
                MDrv_MSPI_SlaveEnable(false);
                MDrv_MSPI_Write(dolby_temp,dolby_size);
                dolby_size=0;
            }
        }
    MDrv_MSPI_SlaveEnable(true);

    free(dolby_data);
    UBOOT_TRACE("OK\n");
    return ret; 
}

#endif

