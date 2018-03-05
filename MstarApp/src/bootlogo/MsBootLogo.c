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
#include <malloc.h>
#include <ShareType.h>
#include <bootlogo/MsBootLogo.h>
#include <bootlogo/MsPoolDB.h>
#include <MsApiJpd.h>
#include <MsApiGEGOP.h>
#include <MsVfs.h>
#include <MsSystem.h>
#include <MsApiPanel.h>
#include <MsUboot.h>
#include <MsDebug.h>
#include <MsRawIO.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <drvGPIO.h>
#include <MsBoot.h>
#include <miu/MsDrvMiu.h>
#include <MsEnvironment.h>
#include <apiGOP.h>
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_LOGO_STORE_IN_SPI_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_SPI
#define FLASH_DEFAULT_PARTITION SPI_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME SPI_DEFAULT_VOLUME
#elif defined (CONFIG_LOGO_STORE_IN_NAND_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_NAND
#define FLASH_DEFAULT_PARTITION NAND_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME NAND_DEFAULT_VOLUME
#elif defined  (CONFIG_LOGO_STORE_IN_EMMC_FLASH)
#define FLASH_DEFAULT_TARGET  E_RAW_DATA_IN_MMC
#define FLASH_DEFAULT_PARTITION MMC_DEFAULT_PARTITION
#define FLASH_DEFAULT_VOLUME MMC_DEFAULT_VOLUME
#endif

#define BOOTLOGO_BUFFER_SIZE 0x400000

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern ST_DBTABLE gstDbtable;
U8 bLogoIsBmp = 0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if ( CONFIG_MINIUBOOT && (1==CONFIG_LOGO_STORE_IN_MBOOT) )
#else
static BOOLEAN bdbtableReady=FALSE;
MS_BOOL bAudioPreInit=FALSE;
#endif

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// @brief \b Function Name \b dbtable_init
// @brief \b Description   \b prepare the logo/music/panel/ursa information & data
// @return                 \b dbtable_init_Result
////////////////////////////////////////////////////////////////////////////////

#if (!(CONFIG_UBOOT_MINI_LIB)&&!(CONFIG_LOGO_STORE_IN_MBOOT) && !(CONFIG_LOGO_STORE_NOT_SET) && !(CONFIG_MSTAR_TOOL_ROM_PROGRAM_NAND_BIN))
int do_dbtable_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret =-1;
    int dbtable = 0;
    U32 u32LogoOffset = 0;
    char * p_str = NULL;
    UBOOT_TRACE("IN\n");

    ret = raw_io_config(FLASH_DEFAULT_TARGET,FLASH_DEFAULT_PARTITION,FLASH_DEFAULT_VOLUME);

    if(ret != 0)
    {
        bdbtableReady=FALSE;
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret=MsApiChunkHeader_GetValue(CH_UBOOT_POOL_ROM_OFFSET,&u32LogoOffset);
    if(ret != 0)
    {
        bdbtableReady=FALSE;
        UBOOT_ERROR("get logo offset fail!\n");
        return -1;
    }

    UBOOT_DEBUG("u32LogoOffset=0x%x\n",(unsigned int)u32LogoOffset);
    if( (argc > 1) && (strcmp(argv[1],"1")==0))
    {
        UBOOT_DEBUG("sikp db_table\n");

        ret = write_dbtable(u32LogoOffset);
    }
    else
    {
        UBOOT_DEBUG("get db_table\n");
        p_str = getenv ("db_table");
        if(p_str != NULL)
        {

            dbtable = (int)simple_strtol(p_str, NULL, 10);
            UBOOT_DEBUG("db_table=%d\n",dbtable);
            if(dbtable == 0)
            {
                ret =  write_dbtable(u32LogoOffset);

            }
            else
            {
                ret =  read_dbtable(u32LogoOffset);
            }
        }
        else
        {
            UBOOT_DEBUG("No db_table\n");
            ret =  write_dbtable(u32LogoOffset);
        }
    }
    if(ret!=0)
    {
        bdbtableReady=FALSE;
        UBOOT_ERROR("dbtable init fail\n");
    }
    else
    {
        bdbtableReady=TRUE;
        UBOOT_TRACE("OK\n");
    }
    return ret;
}
#endif

#if (CONFIG_LOGO_STORE_IN_MBOOT)
static int LogoInMboot2Dram(MS_U32 *Dram_Addr)
{
    int ret = -1;
    MS_U32 u32LogoSize = 0;
    MS_U32 u32LogoOffset = 0;
    UBOOT_TRACE("IN\n");

    ret = mboot_raw_io_Config();
    if(ret != 0)
    {
        UBOOT_ERROR("raw_io_config setting fail!\n");
        return -1;
    }

    ret=MsApiChunkHeader_GetValue(CH_LOGO_ROM_OFFSET,&u32LogoOffset);
    if(ret!=0)
    {
        UBOOT_ERROR("get u32LogoOffset fail\n");
        return -1;
    }
    ret=MsApiChunkHeader_GetValue(CH_LOGO_ROM_SIZE,&u32LogoSize);
    if(ret!=0)
    {
        UBOOT_ERROR("get u32LogoSize fail\n");
        return -1;
    }

    *Dram_Addr=(MS_U32)malloc(u32LogoSize);
    if(Dram_Addr!=NULL)
    {
        if(raw_read(*Dram_Addr,u32LogoOffset,u32LogoSize)!=0)
        {
            UBOOT_ERROR("LogoInMboot2Dram Fail!!\n");
            return -1;
        }
    }

    UBOOT_TRACE("OK\n");
    return u32LogoSize;
}
#else
static unsigned int Load_Photo2Dram(char *SrcPath,U32 *Dram_Addr)
{
    U32 u32LogoSize=0;
    UBOOT_TRACE("IN\n");

    if(Dram_Addr==NULL)
    {
        UBOOT_ERROR("Error: Invalid Dram_Addr\n");
        return 0;
    }

    if(SrcPath==NULL)// from MPOOL:dbtable_init
    {
        UBOOT_DEBUG("No SrcPath\n");
        if(bdbtableReady==TRUE)
        {
            u32LogoSize = gstDbtable.dbdata[E_DB_LOGO].Size;
            UBOOT_DEBUG("u32LogoSize=0x%x\n",(unsigned int)u32LogoSize);
            *Dram_Addr=(U32)malloc(u32LogoSize);
            if((*Dram_Addr==0) || (Read_LogoToDisplayAddr(*Dram_Addr)!=0)) //read logo from spi/emmc to dram
            {
                UBOOT_ERROR("Error: Read_LogoToDisplayAddr Fail\n");
                return 0;
            }
        }
    }
    else // from nand/emmc file system directly
    {
        char PathBuf[CMD_BUF] = "\0";
        vfs_mount(CUSTOMER);
        snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CUSTOMER_PATH,SrcPath);
        UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
        u32LogoSize = vfs_getsize(PathBuf);
        UBOOT_DEBUG("u32LogoSize=0x%x\n",(unsigned int)u32LogoSize);
        *Dram_Addr=(U32)malloc(u32LogoSize);
        if(*Dram_Addr==0 || (vfs_read((void *)(*Dram_Addr),PathBuf,0,u32LogoSize)!=0))
        {
            UBOOT_ERROR("Error: vfs_read Fail\n");
            return 0;
        }
    }
    UBOOT_DEBUG("Dram_Addr=0x%x\n",(unsigned int)Dram_Addr);
    UBOOT_TRACE("OK\n");
    return u32LogoSize;
}
#endif

int MsApi_GetLogoAddrs(unsigned int *phyAddr)
{
    char *cmdline =NULL;
    char bootlogo_buffer[CMD_BUF] ="\0";
    char *p_str = NULL;
    unsigned int u32LogoBufSize=0;
    extern U8 g_logoMiuIdx;
    extern U8 bDelayLogo;

    cmdline = getenv ("bootargs");
    if ((cmdline==NULL) || (strstr(cmdline, "delaylogo=true") == NULL) || (isBootToRecovery()))
    {
        // normal logo

        // use the bootlogo buffer to store bootlogo
        strncpy(bootlogo_buffer,"E_MMAP_ID_BOOTLOGO_BUFFER",strlen("E_MMAP_ID_BOOTLOGO_BUFFER")+1);
        if (get_addr_from_mmap(bootlogo_buffer, phyAddr) == 0)
        {
            g_logoMiuIdx = MsApi_Miu_GetID((MS_U64)(*phyAddr));
            UBOOT_DEBUG("BOOTLOGO SET GOP MIU[%d]\n",g_logoMiuIdx);
            return 0;
        }
        else if (get_value_from_env(E_DFB_FRAMEBUFFER_ADR, MMAP_E_DFB_FRAMEBUFFER_ADDR, phyAddr)!=0)
        {
            UBOOT_ERROR("get %s mmap fail\n",E_DFB_FRAMEBUFFER_ADR);
            return -1;
        }
        g_logoMiuIdx = MsApi_Miu_GetID((MS_U64)(*phyAddr));
        UBOOT_DEBUG("BOOTLOGO SET GOP MIU[%d]\n",g_logoMiuIdx);
    }
    else
    {
        // delay bootlogo

        // use the bootlogo buffer to store bootlogo
        strncpy(bootlogo_buffer,"E_MMAP_ID_BOOTLOGO_BUFFER",strlen("E_MMAP_ID_BOOTLOGO_BUFFER")+1);
        if (get_addr_from_mmap(bootlogo_buffer, phyAddr) == 0)
        {
            get_length_from_mmap(bootlogo_buffer,&u32LogoBufSize);
            if((u32LogoBufSize&0xfffffff) >= BOOTLOGO_BUFFER_SIZE)
            {
                g_logoMiuIdx = MsApi_Miu_GetID((MS_U64)(*phyAddr));
                bDelayLogo = TRUE;
                return 0;
            }
        }

        memset (bootlogo_buffer, 0, CMD_BUF);
        // check if users define another buffer to store the bootlogo
        if ((p_str = getenv("bootlogo_buffer")) != NULL)
        {
            strncpy(bootlogo_buffer, p_str ,strlen(p_str));
        }
        // use the PVR buffer to store bootlogo, because the DFB buffer will be cleared during booting up
        else
        {
            strncpy(bootlogo_buffer,"E_MMAP_ID_PVR_DOWNLOAD",strlen("E_MMAP_ID_PVR_DOWNLOAD")+1);
        }

        if (get_addr_from_mmap(bootlogo_buffer,phyAddr) != 0)
        {
                UBOOT_ERROR("Cannot get the size of %s\n", bootlogo_buffer);
                return -1;
        }
        else
        {
            get_length_from_mmap(bootlogo_buffer,&u32LogoBufSize);
            if((u32LogoBufSize&0xfffffff) >= BOOTLOGO_BUFFER_SIZE)
            {
                g_logoMiuIdx = MsApi_Miu_GetID((MS_U64)(*phyAddr));
                bDelayLogo = TRUE;
            }
            else
            {
                UBOOT_ERROR("fail to delay bootlogo because the size of buffer is less than 4MB (1366*768 *ARGB),\n");
                UBOOT_ERROR("but your buffer(%s) size is only 0x%x.\n",bootlogo_buffer,*phyAddr);
                UBOOT_ERROR("please check your mmap.ini first\n");
                return -1;
            }
        }
    }
    return 0;
}

int showLogo_jpd(LogoDispConfigParams stConfig,U32 JPD_OUTADDR)
{
    int ret = -1;
    GFX_BufferInfo src_info, dst_info;
    GFX_DrawRect rect;
    MS_BOOL bHorStretch=stConfig.bHorStretch;
    MS_BOOL bVerStretch=stConfig.bVerStretch;
    unsigned int u32Addr=0;
    U8 u8logoGopIdx = 0;

    UBOOT_TRACE("IN\n");

    src_info.u32Addr = JPD_OUTADDR;
    UBOOT_DEBUG("src_info.u32Addr=0x%x\n",(unsigned int)src_info.u32Addr);

    src_info.u32ColorFmt = GFX_FMT_YUV422;
    UBOOT_DEBUG("src_info.u32ColorFmt=0x%x\n",(unsigned int)src_info.u32ColorFmt);

    src_info.u32Width = (U32)JPG_GetAlignmentWidth();
    UBOOT_DEBUG("src_info.u32Width=0x%x\n",(unsigned int)src_info.u32Width);

    src_info.u32Height = (U32)JPG_GetAlignmentHeight();
    UBOOT_DEBUG("src_info.u32Height=0x%x\n",(unsigned int)src_info.u32Height);

    src_info.u32Pitch = (U32)JPG_GetAlignmentPitch()<<1;
    UBOOT_DEBUG("src_info.u32Pitch=0x%x\n",(unsigned int)src_info.u32Pitch);

    ret = MsApi_GetLogoAddrs(&u32Addr);
    if (ret != 0)
    {
        UBOOT_ERROR("get bootlogo addr error\n");
        return -1;
    }
    dst_info.u32Addr = (MS_PHYADDR) u32Addr;

    UBOOT_DEBUG("dst_info.u32Addr=0x%x\n",(unsigned int)dst_info.u32Addr);

    dst_info.u32ColorFmt = GFX_FMT_YUV422;
    UBOOT_DEBUG("dst_info.u32ColorFmt=0x%x\n",(unsigned int)dst_info.u32ColorFmt);

    rect.srcblk.x = 0;
    UBOOT_DEBUG("rect.srcblk.x=0x%x\n",(unsigned int)rect.srcblk.x);

    rect.srcblk.y = 0;
    UBOOT_DEBUG("rect.srcblk.y=0x%x\n",(unsigned int)rect.srcblk.y);

    rect.srcblk.width = JPG_GetAlignmentWidth()/*u16Pitch*/;
    UBOOT_DEBUG("rect.srcblk.width=0x%x\n",(unsigned int)rect.srcblk.width);


    rect.srcblk.height = JPG_GetAlignmentHeight();
    UBOOT_DEBUG("rect.srcblk.height=0x%x\n",(unsigned int)rect.srcblk.height);

    rect.dstblk.x = 0;
    UBOOT_DEBUG("rect.dstblk.x=0x%x\n",(unsigned int)rect.dstblk.x);

    rect.dstblk.y = 0;
    UBOOT_DEBUG("rect.dstblk.y=0x%x\n",(unsigned int)rect.dstblk.y);

    #if (CONFIG_MIPS32 == 1 && CONFIG_TV_CHIP == 0)
        rect.dstblk.width  = ((g_IPanel.Width()+15)&0xFFFFF0)/*u16Pitch*/;
        rect.dstblk.height = g_IPanel.Height();

        dst_info.u32Width  = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch  = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<1);
        dst_info.u32Height = g_IPanel.Height();
    #else
    if( JPG_GetAlignmentWidth() > g_IPanel.Width())
    {
        rect.dstblk.width = ((g_IPanel.Width()+15)&0xFFFFF0)/*u16Pitch*/;
        dst_info.u32Width = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<1);
    }
    else
    {
		rect.dstblk.width = ((JPG_GetAlignmentWidth()+15)&0xFFFFF0)/*u16Pitch*/;
#if CONFIG_DISPLAY_LOGO_NOT_SCALE
        dst_info.u32Width = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<1);
#else
        dst_info.u32Width = (U32)((JPG_GetAlignmentWidth()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((JPG_GetAlignmentPitch()+15)&0xFFFFF0)<<1);
#endif
    }

    UBOOT_DEBUG("rect.dstblk.width=0x%x\n",(unsigned int)rect.dstblk.width);
    UBOOT_DEBUG("dst_info.u32Width=0x%x\n",(unsigned int)dst_info.u32Width);
    UBOOT_DEBUG("dst_info.u32Pitch=0x%x\n",(unsigned int)dst_info.u32Pitch);

    if( JPG_GetAlignmentHeight() > g_IPanel.Height())
    {
        rect.dstblk.height = g_IPanel.Height();
        dst_info.u32Height = g_IPanel.Height();
    }
    else
    {
		rect.dstblk.height = JPG_GetAlignmentHeight();
#if CONFIG_DISPLAY_LOGO_NOT_SCALE
        dst_info.u32Height = g_IPanel.Height();
#else
        dst_info.u32Height = (U32)JPG_GetAlignmentHeight();
#endif
    }

    UBOOT_DEBUG("rect.dstblk.height=0x%x\n",(unsigned int)rect.dstblk.height);
    UBOOT_DEBUG("dst_info.u32Height=0x%x\n",(unsigned int)dst_info.u32Height);
    #endif

     // initilize the GOP of Subtitle instead of UI;
    // otherwise, then bootlogo will be closed unexpectedly,
    // because UI is initilalized during booting up
    char* p_str = getenv("bootlogo_gopidx");
    if(p_str==NULL)
    {
		#if CONFIG_UBOOT_MINI_LIB
		    u8logoGopIdx = DEFAULT_BOOTLOGO_GOPIDX;
		#else
        st_sys_misc_setting misc_setting;
        if(Read_MiscSetting_ToFlash(&misc_setting) == -1)//read misc setting fail, set gop index to default value
        {
            u8logoGopIdx = DEFAULT_BOOTLOGO_GOPIDX;
        }
        else
        {
            u8logoGopIdx = misc_setting.m_u8LogoGopIdx;
        }
		#endif
    }
    else
    {
        u8logoGopIdx = simple_strtoul(p_str, NULL, 10);
    }
    if (isBootToRecovery() != 0)
    {
        // recovery mode bootlogo use gop 0
        u8logoGopIdx = 0;
    }
    UBOOT_INFO("[%s] GOP Boot Logo Index %d \n",__FUNCTION__,(int)u8logoGopIdx);
    MsApiDisplay_Logo(u8logoGopIdx,&rect,&src_info,&dst_info,bHorStretch,bVerStretch,stConfig.u16DispX,stConfig.u16DispY);
    UBOOT_TRACE("OK\n");
    return 0;
}

#if (!CONFIG_UBOOT_MINI_LIB)
int showLogo_bmp(LogoDispConfigParams stConfig,U32 BMP_InputADDR,U32 u32LogoSize)
{
    int ret = -1;
    GFX_BufferInfo src_info, dst_info;
    GFX_DrawRect rect;
    MS_BOOL bHorStretch=stConfig.bHorStretch;
    MS_BOOL bVerStretch=stConfig.bVerStretch;
    unsigned int u32Addr=0;
    U8 u8logoGopIdx = 0;

	U32 BMP_OUTADDR=0;
    U32* u32SrcAddr;
	U32* u32DestAddr;
	unsigned char* Img = BMP_InputADDR;

    UBOOT_TRACE("IN\n");

    SET_BITMAPFILEHEADER(Img, bf);
    SET_BITMAPINFOHEADER(Img, bi);

	UBOOT_DEBUG("bf.bfType=0x%x\n",bf.bfType);
	if (bf.bfType != BM)
		return 0;

	if(get_value_from_env(E_MMAP_ID_JPD_WRITE_ADR, MMAP_E_MMAP_ID_JPD_WRITE_ADDR, &BMP_OUTADDR)!=0)
	{
	    UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_JPD_WRITE_ADR);
	    return -1;
	}

	u32SrcAddr = BMP_InputADDR + BITMAPHEADER_SIZE;
	u32DestAddr = MsOS_PA2KSEG1(BMP_OUTADDR);

	memcpy((void*)u32DestAddr,(void*)u32SrcAddr,u32LogoSize - BITMAPHEADER_SIZE);

	UBOOT_DEBUG("JPD_InputADDR : 0x%x \n",BMP_InputADDR);
	UBOOT_DEBUG("JPD_OUTADDR : 0x%x \n",BMP_OUTADDR);

    src_info.u32Addr = BMP_OUTADDR;

    UBOOT_DEBUG("src_info.u32Addr=0x%x\n",(unsigned int)src_info.u32Addr);

    src_info.u32ColorFmt = GFX_FMT_ARGB8888;
    UBOOT_DEBUG("src_info.u32ColorFmt=0x%x\n",(unsigned int)src_info.u32ColorFmt);

    src_info.u32Width = (U32)bi.biWidth;
    UBOOT_DEBUG("src_info.u32Width=0x%x\n",(unsigned int)src_info.u32Width);

    src_info.u32Height = (U32)bi.biHeight;
    UBOOT_DEBUG("src_info.u32Height=0x%x\n",(unsigned int)src_info.u32Height);

    src_info.u32Pitch = (U32)bi.biWidth<<2;
    UBOOT_DEBUG("src_info.u32Pitch=0x%x\n",(unsigned int)src_info.u32Pitch);

    ret = MsApi_GetLogoAddrs(&u32Addr);
    if (ret != 0)
    {
        UBOOT_ERROR("get bootlogo addr error\n");
        return -1;
    }
    dst_info.u32Addr = (MS_PHYADDR) u32Addr;

    UBOOT_DEBUG("dst_info.u32Addr=0x%x\n",(unsigned int)dst_info.u32Addr);

    dst_info.u32ColorFmt = GFX_FMT_ARGB8888;
    UBOOT_DEBUG("dst_info.u32ColorFmt=0x%x\n",(unsigned int)dst_info.u32ColorFmt);

    rect.srcblk.x = 0;
    UBOOT_DEBUG("rect.srcblk.x=0x%x\n",(unsigned int)rect.srcblk.x);

    rect.srcblk.y = 0;
    UBOOT_DEBUG("rect.srcblk.y=0x%x\n",(unsigned int)rect.srcblk.y);

    rect.srcblk.width = (U16)bi.biWidth;/*u16Pitch*/;
    UBOOT_DEBUG("rect.srcblk.width=0x%x\n",(unsigned int)rect.srcblk.width);


    rect.srcblk.height = (U16)bi.biHeight;
    UBOOT_DEBUG("rect.srcblk.height=0x%x\n",(unsigned int)rect.srcblk.height);

    rect.dstblk.x = 0;
    UBOOT_DEBUG("rect.dstblk.x=0x%x\n",(unsigned int)rect.dstblk.x);

    rect.dstblk.y = 0;
    UBOOT_DEBUG("rect.dstblk.y=0x%x\n",(unsigned int)rect.dstblk.y);

    #if (CONFIG_MIPS32 == 1 && CONFIG_TV_CHIP == 0)
        rect.dstblk.width  = ((g_IPanel.Width()+15)&0xFFFFF0)/*u16Pitch*/;
        rect.dstblk.height = g_IPanel.Height();

        dst_info.u32Width  = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch  = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<2);
        dst_info.u32Height = g_IPanel.Height();
    #else
    if(bi.biWidth > g_IPanel.Width())
    {
        rect.dstblk.width = ((g_IPanel.Width()+15)&0xFFFFF0)/*u16Pitch*/;
        dst_info.u32Width = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<2);
    }
    else
    {
		rect.dstblk.width = ((bi.biWidth+15)&0xFFFFF0)/*u16Pitch*/;
#if CONFIG_DISPLAY_LOGO_NOT_SCALE
        dst_info.u32Width = (U32)((g_IPanel.Width()+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((g_IPanel.Width()+15)&0xFFFFF0)<<2);
#else
        dst_info.u32Width = (U32)((bi.biWidth+15)&0xFFFFF0);
        dst_info.u32Pitch = (U32)(((bi.biWidth+15)&0xFFFFF0)<<2);
#endif
    }

    UBOOT_DEBUG("rect.dstblk.width=0x%x\n",(unsigned int)rect.dstblk.width);
    UBOOT_DEBUG("dst_info.u32Width=0x%x\n",(unsigned int)dst_info.u32Width);
    UBOOT_DEBUG("dst_info.u32Pitch=0x%x\n",(unsigned int)dst_info.u32Pitch);

    if( bi.biHeight > g_IPanel.Height())
    {
        rect.dstblk.height = g_IPanel.Height();
        dst_info.u32Height = g_IPanel.Height();
    }
    else
    {
		rect.dstblk.height = bi.biHeight;
#if CONFIG_DISPLAY_LOGO_NOT_SCALE
        dst_info.u32Height = g_IPanel.Height();
#else
        dst_info.u32Height = (U32)bi.biHeight;
#endif
    }

    UBOOT_DEBUG("rect.dstblk.height=0x%x\n",(unsigned int)rect.dstblk.height);
    UBOOT_DEBUG("dst_info.u32Height=0x%x\n",(unsigned int)dst_info.u32Height);
    #endif

     // initilize the GOP of Subtitle instead of UI;
    // otherwise, then bootlogo will be closed unexpectedly,
    // because UI is initilalized during booting up
    char* p_str = getenv("bootlogo_gopidx");
    if(p_str==NULL)
    {
        st_sys_misc_setting misc_setting;
        if(Read_MiscSetting_ToFlash(&misc_setting) == -1)//read misc setting fail, set gop index to default value
        {
            u8logoGopIdx = DEFAULT_BOOTLOGO_GOPIDX;
        }
        else
        {
            u8logoGopIdx = misc_setting.m_u8LogoGopIdx;
        }
    }
    else
    {
        u8logoGopIdx = simple_strtoul(p_str, NULL, 10);
    }
    if (isBootToRecovery() != 0)
    {
        // recovery mode bootlogo use gop 0
        u8logoGopIdx = 0;
    }
    UBOOT_INFO("[%s] GOP Boot Logo Index %d \n",__FUNCTION__,(int)u8logoGopIdx);
    MsApiDisplay_Logo(u8logoGopIdx,&rect,&src_info,&dst_info,bHorStretch,bVerStretch,stConfig.u16DispX,stConfig.u16DispY);
    UBOOT_TRACE("OK\n");
    return 0;
}
#endif
int do_bootlogo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = -1;
    U32 u32LogoSize=0;
    U32 JPD_InputADDR=0;
    U32 JPD_OUTADDR=0;
    char *PhotoPath=NULL;
    LogoDispConfigParams stConfig;
    UBOOT_TRACE("IN\n");

    printf("\n[AT][MB][bootlogo begin][%lu]\n", MsSystemGetBootTime());

    if(argc > 5)
    {
        PhotoPath = argv[5];
        UBOOT_DEBUG("PhotoPath=%s\n",PhotoPath);
    }

    if(argc > 4)
    {
        stConfig.u16DispX = simple_strtoul(argv[1], NULL, 10);
        stConfig.u16DispY = simple_strtoul(argv[2], NULL, 10);
        stConfig.bHorStretch = simple_strtoul(argv[3], NULL, 10);
        stConfig.bVerStretch = simple_strtoul(argv[4], NULL, 10);
    }
    else
    {
        stConfig.u16DispX = 0;
        stConfig.u16DispY = 0;
        stConfig.bHorStretch = 1;
        stConfig.bVerStretch = 1;
    }

    UBOOT_DEBUG("u16DispX=0x%x,u16DispY=0x%x,bHorStretch=0x%x,bVerStretch=0x%x\n",stConfig.u16DispX,stConfig.u16DispY,stConfig.bHorStretch,stConfig.bVerStretch);

    if(IsPanelReady()!=0)
    {
        UBOOT_ERROR("Panle is not ready\n");
        return -1;
    }

#if (CONFIG_LOGO_STORE_IN_MBOOT)
    u32LogoSize = LogoInMboot2Dram(&JPD_InputADDR);
#else
    u32LogoSize = Load_Photo2Dram(PhotoPath,&JPD_InputADDR);
#endif

    UBOOT_DEBUG("u32LogoSize : 0x%x \n",u32LogoSize);
    if(u32LogoSize!= 0)
    {

		unsigned char* Img = JPD_InputADDR;

#if (!CONFIG_UBOOT_MINI_LIB)
		WORD LogoType = GET_WORD(Img);

		if(LogoType == BM)
		{
			bLogoIsBmp = 1;
            ret = showLogo_bmp(stConfig,JPD_InputADDR,u32LogoSize);
		}
        else
#endif
        {
			bLogoIsBmp = 0;
            JPD_OUTADDR=MsApi_JPD_Decode((MS_U32)JPD_InputADDR, u32LogoSize);
            ret = showLogo_jpd(stConfig,JPD_OUTADDR);
        }

        if(ret != 0)
        {
            UBOOT_ERROR("bootlogo error\n");
        }
    }

    if(((void*)JPD_InputADDR)!=NULL)
    {
        free((void*)JPD_InputADDR);
    }

    UBOOT_TRACE("OK\n");
    return ret;
}

#if (ENABLE_MODULE_POWER_MUSIC == 1)
#include <MsApiAudio.h>
#include <audio/MsDrvAudio.h>

int Load_Music2Dram(char *MusicPath,U32 Dram_Addr)
{
    U32 musicSize=0;
    UBOOT_TRACE("IN\n");

    if(MusicPath == NULL)
    {
        UBOOT_DEBUG("MusicPath is a null pointer\n");
        UBOOT_DEBUG("Dram_Addr=0x%x\n",Dram_Addr);
        //read music from spi/emmc to dram defalut logo
        if(Read_MusicToPlayAddr(Dram_Addr)!=0)
        {
            UBOOT_ERROR("Read_MusicToPlayAddr() failed\n");
            return -1;
        }
    }
    else // from nand/emmc directly
    {
        char PathBuf[128] = "\0";
        vfs_mount(CUSTOMER);
        snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CUSTOMER_PATH,MusicPath);
        UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
        musicSize = vfs_getsize(PathBuf);
        UBOOT_DEBUG("musicSize=0x%x,Dram_Addr=0x%x\n",(unsigned int)musicSize,(unsigned int)Dram_Addr);
        if(vfs_read((void *)(PA2NVA(Dram_Addr)),PathBuf,0,musicSize)==-1)
        {
            UBOOT_ERROR("vfs_read fail\n");
            return -1;
        }
    }
    UBOOT_TRACE("OK\n");
    return 0;
}

int do_audiopreinit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_TRACE("IN\n");
    int ret = 0;
    int music_on = 0;
    char* opt_music = NULL;

    opt_music = getenv("music");
    if(opt_music != NULL)
    {
        music_on = simple_strtol(opt_music, NULL, 10);
         if(music_on < 1 || music_on > 2)
            {
                printf("!!! [%s]  Skip Power Music  !!! \n",__FUNCTION__);
                return -1;
            }
    }
    //Adj_Volume_Off();      //
    Audio_Amplifier_OFF(); //
    ret =MsDrv_AudioPreInit();

    printf("[AT][MB][audio_preinit][%lu]\n", MsSystemGetBootTime());
    if(ret==-1)
    {
        UBOOT_ERROR("Audio PreInit Fail Skip Power Music !!! \n");
        bAudioPreInit = FALSE;
    }
    else
    {
        bAudioPreInit = TRUE;
    }

    UBOOT_TRACE("OK\n");

    return 1;
}

int do_bootmusic (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int ret = 0;
    char *MusicPath = NULL;
    U32 u32EsStartAddr = 0;
    int music_on = 0;
    char* opt_music = NULL;


    UBOOT_TRACE("IN\n");
    if(argc > 1)
    {
        MusicPath = argv[1];
    }

    opt_music = getenv("music");
    if(opt_music != NULL)
    {
        music_on = simple_strtol(opt_music, NULL, 10);
         if(music_on < 1 || music_on > 2)
            {
                printf("!!! [%s]  Skip Power Music  !!! \n",__FUNCTION__);
                return -1;
            }
    }

    if(bdbtableReady!=TRUE)
    {
        UBOOT_ERROR("dbtable is not ready\n");
        return -1;
    }

    if(bAudioPreInit!=TRUE)
    {
        UBOOT_ERROR("Audio PreInit is not ready\n");
        return -1;
    }
/// move to audio pre init --start--
#if 0
    Adj_Volume_Off();      //
    Audio_Amplifier_OFF(); //
    ret =MsDrv_AudioPreInit();

    if(ret==-1)
    {
        UBOOT_ERROR("Get Mmap for Audio Fail Skip Power Music !!! \n");
        return -1;
    }
#endif
/// move to audio pre init --end--

    u32EsStartAddr = MsDrv_GetEsStartAddr();

    ret = Load_Music2Dram(MusicPath, u32EsStartAddr);
    if(ret==-1)
    {
        UBOOT_ERROR("load music to dram fail\n");
    }
    else
    {
        //   MsOS_DelayTask(2000);
        play_powermusic_main();
        UBOOT_TRACE("OK\n");
    }
    return ret;
}
#endif


