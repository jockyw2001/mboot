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
#include <MsTypes.h>

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <drvBDMA.h>
#include <bootlogo/MsPoolDB.h>
#include <MsVfs.h>
#include <MsUtility.h>
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsRawIO.h>
#include <drvSERFLASH.h>
#include <CusConfig.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <MsApiPanel.h>
#include <uboot_mmap.h>
#include <bootlogo/iniparser.h>
#include <bootlogo/MsBootLogo.h>



#if (CONFIG_URSA_6M30)
#include <../drivers/mstar/ursa/ursa_6m30.h>
#endif

#if (ENABLE_URSA_8 ==1 )
#include <../drivers/mstar/ursa/ursa_8.h>
#endif

#if (ENABLE_URSA_6M40==1 )
#include <../drivers/mstar/ursa/ursa_6m40.h>
#endif

#if (CONFIG_DYNAMIC_IR)
#include <MsBoot.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
char const *sDB_INDEX[]=
{
    "E_DB_LOGO",
    "E_DB_MUSIC",
    "E_DB_BOARD_PARA",
    "E_DB_PANEL_PARA",
    "E_DB_URSA_6M30",
    "E_DB_URSA",
    "E_DB_TCON",
    "E_DB_PNL_VB1",
    "E_DB_PNL_GOPMUX",
    "E_DB_MISC",
    "E_DB_MSPI",
    "E_DB_DMA",
    "E_DB_HDMITX",
    "E_DB_IR_BOOTCHECK",
    "E_DB_IR_WAKEUP",
    "E_DB_MAX"
};
ST_DBTABLE gstDbtable;

static char gPnlPath[BUFFER_SIZE];
static char gBoardPath[BUFFER_SIZE];
static char gLogoPath[BUFFER_SIZE];
static char gMusicPath[BUFFER_SIZE];
static char gGopMuxPath[BUFFER_SIZE];
static char gTconBinPath[BUFFER_SIZE];
static char gLocalDimmingPath[BUFFER_SIZE];
#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
static char gModulePath[BUFFER_SIZE];
#endif
#define MPoolVolumeOffset(addr, u32DbtableOffset) (addr-u32DbtableOffset)
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
pnl_VB1ChannelOrder stVB1Channel1order;
st_sys_misc_setting gstSysMiscSetting;
st_pwm_setting gstPWMSetting;
static U8 *pDbBufferAddr = 0;
static U32 u32Dbload_size = 0;
st_hdmitx_setting stHdmitxSetting;

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern char * strncpy(char * dest,const char *src,size_t count);
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern unsigned int GetRealOffset(unsigned int);

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
int Load_DataToDbBuffer(U32 u32DbtableOffset, DB_INDEX db_index, U32 pu32DataAddr);
int Load_LogoToFlash(U32 u32DbtableOffset);
int Load_PanelSetting_ToFlash(U32 u32DbtableOffset);
int Load_BoardSetting_ToFlash(U32 u32DbtableOffset);
int Load_MusicToFlash(U32 u32DbtableOffset);
int Load_GopMuxToFlash(U32 u32DbtableOffset);
int Load_TConToFlash(U32 u32DbtableOffset);
int Load_PnlVb1ToFlash(U32 u32DbtableOffset);
int Load_PWMSetting_ToFlash(U32 u32DbtableOffset);
int Load_MiscSetting_ToFlash(U32 u32DbtableOffset);
int Load_LDMPara_ToFlash(U32 u32DbtableOffset);
int Load_HDMITXToFlash(U32 u32DbtableOffset);
#if(CONFIG_DYNAMIC_IR)
int Load_IRConfig_ToFlash(U32 u32DbtableOffset);
#endif
static void _print_dbtable_info(void);
void count_data_crc32_save(U32 addr,int size,DB_INDEX db_index);
int count_data_crc32_cmp(U32 addr,int size, DB_INDEX db_index);
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if(CONFIG_ENABLE_V_I_PWM == 1)
int bSafeModeEnable = 0;
#endif

int parse_hdmitx_setting(char *path)
{
    U32 filesize = 0;
    char str[BUFFER_SIZE]="\0";
    char *script=NULL;
    #define HDMITX_SECTION_NAME "Hdmitx"

    UBOOT_TRACE("IN\n");
    memset(&stHdmitxSetting,0,sizeof(st_hdmitx_setting));

    // Set initial value from mboot
    stHdmitxSetting.enLoadDefaultFromMboot = 1;

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        return -1;
    }

    Profile_Init(script,filesize);
    if(Profile_GetString(HDMITX_SECTION_NAME, "enInColorFmt", "", str, sizearray(str)))
    {
        stHdmitxSetting.enInColorFmt=simple_strtoul(str,NULL,16);
        // if there are hdmi setting values in customer ini
        stHdmitxSetting.enLoadDefaultFromMboot = 0;
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enOutColorFmt", "", str, sizearray(str)))
    {
        stHdmitxSetting.enOutColorFmt=simple_strtoul(str,NULL,16);
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enInColorQuantRange", "", str, sizearray(str)))
    {
        stHdmitxSetting.enInColorQuantRange=simple_strtoul(str,NULL,16);
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enOutColorQuantRange", "", str, sizearray(str)))
    {
        stHdmitxSetting.enOutColorQuantRange=simple_strtoul(str,NULL,16);
    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enOutputMode", "", str, sizearray(str)))
    {
        stHdmitxSetting.enOutputMode=simple_strtoul(str,NULL,16);

    }
    if(Profile_GetString(HDMITX_SECTION_NAME, "enColorDepth", "", str, sizearray(str)))
    {
        stHdmitxSetting.enColorDepth=simple_strtoul(str,NULL,16);

    }
    UBOOT_DEBUG("stHdmitxSetting.enInColorFmt         = %d\n", stHdmitxSetting.enInColorFmt);
    UBOOT_DEBUG("stHdmitxSetting.enOutColorFmt        = %d\n", stHdmitxSetting.enOutColorFmt);
    UBOOT_DEBUG("stHdmitxSetting.enInColorQuantRange  = %d\n", stHdmitxSetting.enInColorQuantRange);
    UBOOT_DEBUG("stHdmitxSetting.enOutColorQuantRange = %d\n", stHdmitxSetting.enOutColorQuantRange);
    UBOOT_DEBUG("stHdmitxSetting.enOutputMode         = %d\n", stHdmitxSetting.enOutputMode);
    UBOOT_DEBUG("stHdmitxSetting.enColorDepth         = %d\n", stHdmitxSetting.enColorDepth);
    UBOOT_DEBUG("stHdmitxSetting.enLoadDefaultFromMboot = %d\n", stHdmitxSetting.enLoadDefaultFromMboot);

    UBOOT_TRACE("OK\n");
    return 0;
}

int parse_sys_ini(char* path,char* ModelPath)
{
    int ret = -1;
    char *script = NULL;
    char str[BUFFER_SIZE];
    char platforminfo[BUFFER_SIZE]="\0";
    char *s;
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        goto exit;
    }
    UBOOT_INFO("sizeof( sys ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("platform", "gPlatform", "", platforminfo, sizearray(platforminfo)))
    {
        if(0 == set_bootargs_cfg("platform",platforminfo,0))
        {
            UBOOT_INFO("set %s to bootargs OK\n",platforminfo);
        }
        else
        {
            UBOOT_ERROR("set %s to bootargs fail\n",platforminfo);
        }
    }
    else
    {
        UBOOT_INFO("Can't get platform info from %s\n", path);
    }

    Profile_Init((char *)script,filesize);
    // select mode via env customer_ini_path
    s = getenv ("customer_ini_path");
    if(s)
    {
        UBOOT_INFO("customer_ini_path env value --> %s\n",s);
        UBOOT_INFO("Half model.ini path --> %s\n",s);
        snprintf(ModelPath, strlen(s), "%s",s);
        script = loadscript(ModelPath,&filesize);
        if(script == NULL)
        {
            if(Profile_GetString("model", "gModelName", "", str, sizearray(str)))
            {
                UBOOT_INFO("Half model.ini path --> %s\n",str);
                snprintf(ModelPath, BUFFER_SIZE, "%s",str);
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
            }
            setenv("customer_ini_path",ModelPath);
        }
        UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
        ret = 0;
    }
    else
    {
        U8 isFindModel = 0;
        s = getenv("model_ID");
        if(s)
        {
            char model_id[BUFFER_SIZE];
            char id[BUFFER_SIZE];
            char g_modelName[BUFFER_SIZE];
            char tmpModelName[BUFFER_SIZE];
            int i = 1;

            memset(model_id,0,BUFFER_SIZE);
            memset(id,0,BUFFER_SIZE);
            memset(g_modelName,0,BUFFER_SIZE);
            memset(tmpModelName,0,BUFFER_SIZE);
            snprintf(model_id, BUFFER_SIZE, "model_%02d",i);
            snprintf(id, BUFFER_SIZE, "id_%02d",i);
            snprintf(g_modelName, BUFFER_SIZE, "gModelName_%02d",i);
            UBOOT_DEBUG("[model_id] = %s, [g_modelName] = %s, [id] = %s\n", model_id,g_modelName, id);

            while(Profile_GetString(model_id, g_modelName,"", str, sizearray(str)))
            {
                snprintf(tmpModelName, BUFFER_SIZE, "%s", str);
                UBOOT_DEBUG("[tmpModelNmae] = %s\n", tmpModelName);
                //parsing id
                if(Profile_GetString(model_id, id,"", str, sizearray(str)))
                {
                    UBOOT_INFO("[%s] = %s\n", id,str);
                    if(strncmp(str, getenv("model_ID"), strlen(str))==0)
                    {
                        UBOOT_INFO("Half model.ini path --> %s\n",tmpModelName);
                        snprintf(ModelPath, BUFFER_SIZE, "%s",tmpModelName);
                        UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
                        setenv("customer_ini_path",ModelPath);
                        isFindModel = 1;
                        ret = 0;
                        break;
                    }
                }
                i++;
                memset(model_id,0,BUFFER_SIZE);
                memset(id,0,BUFFER_SIZE);
                memset(g_modelName,0,BUFFER_SIZE);
                snprintf(model_id, BUFFER_SIZE, "model_%02d",i);
                snprintf(id, BUFFER_SIZE, "id_%02d",i);
                snprintf(g_modelName, BUFFER_SIZE, "gModelName_%02d",i);
                UBOOT_DEBUG("[model_id] = %s, [g_modelName] = %s, [id] = %s\n", model_id,g_modelName, id);
            }

        }
        if(isFindModel==0)
        {
            if(s)
            {
                UBOOT_ERROR("can't find [id] = %s\n", getenv("model_ID"));
            }

            if(Profile_GetString("model", "gModelName", "", str, sizearray(str)))
            {
                UBOOT_INFO("Half model.ini path --> %s\n",str);
                snprintf(ModelPath, BUFFER_SIZE, "%s",str);
                UBOOT_INFO("Full model.ini  path --> %s\n",ModelPath);
                ret = 0;
            }
            setenv("customer_ini_path",ModelPath);
        }
    }
    parse_hdmitx_setting(getenv("customer_ini_path"));
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int parse_dfb_ini(char* path, st_gopmux_para *gopmux_para)
{
    int ret = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
	long n;

    script = loadscript(path,&filesize);
    if(script == NULL)
    {
    	ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( dfb ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYERCOUNTS", 0);
    gopmux_para->m_u16SettingCounts = n;
	UBOOT_DEBUG("gopmux_para->m_u16SettingCounts:%d\n", gopmux_para->m_u16SettingCounts);
	if (gopmux_para->m_u16SettingCounts > 0)
	{
        gopmux_para->m_u8SettingType = 1;

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER0_GOPINDEX", 0);
        gopmux_para->m_u16Setting0GopIndex= n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting0GopIndex:%d\n", gopmux_para->m_u16Setting0GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER1_GOPINDEX", 0);
        gopmux_para->m_u16Setting1GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting1GopIndex:%d\n", gopmux_para->m_u16Setting1GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER2_GOPINDEX", 0);
        gopmux_para->m_u16Setting2GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting2GopIndex:%d\n", gopmux_para->m_u16Setting2GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER3_GOPINDEX", 0);
        gopmux_para->m_u16Setting3GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting3GopIndex:%d\n", gopmux_para->m_u16Setting3GopIndex);

    	n = Profile_GetInteger("DirectFBRC", "DFBRC_LAYER4_GOPINDEX", 0);
        gopmux_para->m_u16Setting4GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting4GopIndex:%d\n", gopmux_para->m_u16Setting4GopIndex);
	}
	else
	{
        gopmux_para->m_u8SettingType = 0;

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUXCOUNTS", 0);
        gopmux_para->m_u16SettingCounts= n;
    	UBOOT_DEBUG("gopmux_para->m_u16SettingCounts:%d\n", gopmux_para->m_u16SettingCounts);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX0_GOPINDEX", 0);
        gopmux_para->m_u16Setting0GopIndex= n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting0GopIndex:%d\n", gopmux_para->m_u16Setting0GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX1_GOPINDEX", 0);
        gopmux_para->m_u16Setting1GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting1GopIndex:%d\n", gopmux_para->m_u16Setting1GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX2_GOPINDEX", 0);
        gopmux_para->m_u16Setting2GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting2GopIndex:%d\n", gopmux_para->m_u16Setting2GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX3_GOPINDEX", 0);
        gopmux_para->m_u16Setting3GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting3GopIndex:%d\n", gopmux_para->m_u16Setting3GopIndex);

	    n = Profile_GetInteger("DirectFBRC", "DFBRC_MUX4_GOPINDEX", 0);
        gopmux_para->m_u16Setting4GopIndex = n;
    	UBOOT_DEBUG("gopmux_para->m_u16Setting4GopIndex:%d\n", gopmux_para->m_u16Setting4GopIndex);

    }
    const U8 IP0 = 0;
    U8 gopidx = 0;
    U8 layerid = 0;
    U8 layercount = 0;
    char layername[BUFFER_SIZE];

    layercount = Profile_GetInteger("DirectFBRC", "DFBRC_MST_GOP_COUNTS", -1);
    memset(layername,0 ,sizeof(layername));

    for (layerid = 0; layerid < layercount; layerid++)
    {
        snprintf(layername, sizeof(layername), "DFBRC_MST_GOP_DSTPLANE_%d", layerid);
        if (Profile_GetInteger("DirectFBRC", layername, -1) == IP0)
        {
            snprintf(layername, sizeof(layername), "DFBRC_MST_GOP_AVAILABLE_%d", layerid);
            gopidx = Profile_GetInteger("DirectFBRC", layername, -1);

            memset(str,0 ,sizeof(str));
            snprintf(str,sizeof(str),"%u",gopidx);
            gstSysMiscSetting.m_u8LogoGopIdx = gopidx;
            UBOOT_INFO("logo -> [%s]\n",str);

            ret = 0;
            goto exit;
        }
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

int _Vb1Str2Array(char *str,unsigned char *pu8Vb1Array)
{
    unsigned char u8Number =0;
    char puTmp[5]={0};

    UBOOT_DEBUG("spilitting string : %s\n",str);
    while ((str=strstr(str,"0x"))!=NULL)
    {
        memcpy(puTmp,str,sizeof(puTmp)-1);
        u8Number=simple_strtoul(puTmp,NULL,16);
        //UBOOT_INFO("0x%x ",(unsigned int)u8Number);
        *(pu8Vb1Array++)=u8Number;
        str+=2;
    }
    UBOOT_INFO("\n");
    return 0;
}

int Str2Array(char *str, MS_U16 *pu16Array)
{
    MS_U16 u16Number =0;
    char puTmp[7]={0};

    UBOOT_DEBUG("spilitting string : %s\n",str);
    while ((str=strstr(str,"0x"))!=NULL)
    {
        memcpy(puTmp,str,sizeof(puTmp)-1);
        u16Number=simple_strtoul(puTmp,NULL,16);
        //UBOOT_INFO("0x%x \n",(unsigned int)u16Number);
        *(pu16Array++)=u16Number;
        str+=2;
    }
    UBOOT_INFO("\n");
    return 0;
}

void _parse_model_vb1(void)
{
    #define VB1_SECTION_NAME "VB1_Connector"
    char str[BUFFER_SIZE]={0};
    UBOOT_TRACE("IN\n");

    // init stVB1Channel1order
    memset(&stVB1Channel1order,0,sizeof(stVB1Channel1order));

    // 16V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "16V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb116vOrder);
    }

    // 8V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "8V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb18vOrder);
    }

    // 4V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "4V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb14vOrder);
    }

    // 2V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "2V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb12vOrder);
    }
    // 1V_Order
    if(Profile_GetString(VB1_SECTION_NAME, "1V_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb11vOrder);
    }
    // 4O_Order
    if(Profile_GetString(VB1_SECTION_NAME, "4O_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb14OOrder);
    }

    // 2O_Order
    if(Profile_GetString(VB1_SECTION_NAME, "2O_Order","", str, sizearray(str)))
    {
        _Vb1Str2Array(str,stVB1Channel1order.u8Vb12OOrder);
    }

    UBOOT_TRACE("OK\n");
    return;
}

int parse_model_ini(char* path, char* PnlPath, char* BoardPath, char* TconBinPath)
{
    int ret = -1;//
    char *script = NULL;
    char *panel_name = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    UBOOT_TRACE("IN\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            goto exit;
        }
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);

    panel_name = "m_pPanelName";

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("panel", panel_name,"", str, sizearray(str)))
    {
        UBOOT_INFO("Half panel.ini path --> %s\n",str);
        snprintf(PnlPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full panel.ini  path --> %s\n",PnlPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }

    if(Profile_GetString("board", "m_pBoardName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half board.ini path --> %s\n",str);
        snprintf(BoardPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full board.ini  path --> %s\n",BoardPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }

    if(Profile_GetString("TCON_BIN", "TCON_FILE","", str, sizearray(str)))
    {
        char* tmpTconBinPath;
        tmpTconBinPath=str;
        if(FALSE==vfs_getsize(tmpTconBinPath))
        {
            while (*tmpTconBinPath== '/') tmpTconBinPath++;
            tmpTconBinPath=strstr(tmpTconBinPath,"/");
            UBOOT_INFO("filter /config, and find file again!!\n");

            if(FALSE==vfs_getsize(tmpTconBinPath))
            {
                tmpTconBinPath=str;
            }
        }
        snprintf(TconBinPath,BUFFER_SIZE,"%s",tmpTconBinPath);
        UBOOT_INFO("TCON path --> %s\n",TconBinPath);
        ret = 0;
    }
    _parse_model_vb1();

#if (CONFIG_PANEL_INIT == 1)
    memset(str,0,sizeof(str));
    int n = Profile_GetInteger("PANEL_SWING_LEVEL", "SWING_LEVEL", -1);

    if (n >= 0)
    {
        snprintf(str,sizeof(str),"%u",n);
        UBOOT_INFO("swing_level -> [%s]\n",str);
        gstSysMiscSetting.m_u16Panel_SwingLevel = n;
    }
    else
    {
        gstSysMiscSetting.m_u16Panel_SwingLevel = 250;
    }
#endif

#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
    memset(gModulePath,0,sizeof(gModulePath));
    if(Profile_GetString("module", "m_pModuleName","", gModulePath, sizearray(gModulePath)))
    {
        UBOOT_DEBUG("module path: %s\n", gModulePath);
    }
    else
    {
        UBOOT_ERROR("get module path error\n");
    }
#endif


    if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD","", str, sizearray(str)))
    {
        UBOOT_INFO("MIRROR_OSD is  --> %s\n",str);
        if((strcmp(str, "True") == 0)||(strcmp(str, "1") == 0))
        {
            // MIRROR_OSD_TYPE   # 0:normal type.  1:Horizontal-mirror only.  2:Vertical-mirror only.  3:HV-mirror.
            if(Profile_GetString("MISC_MIRROR_CFG", "MIRROR_OSD_TYPE","", str, sizearray(str)))
            {
                UBOOT_INFO("MIRROR_OSD_TYPE is  --> %s\n",str);
                if((strcmp(str, "1") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 3;
                else if((strcmp(str, "2") == 0))
                    gstSysMiscSetting.m_u8MirrorMode = 2;
                else
                    gstSysMiscSetting.m_u8MirrorMode = 1;
            }
            else
            {
                gstSysMiscSetting.m_u8MirrorMode = 1;
            }
        }
        else if((strcmp(str, "False") == 0)||(strcmp(str, "0") == 0))
        {
            gstSysMiscSetting.m_u8MirrorMode = 0;
        }
        else
        {

            UBOOT_INFO("MIRROR_OSD is  [True] or [False], other value can not be parsed\n");
        }
    }
    else
    {
        goto exit;
    }

#if(CONFIG_ENABLE_V_I_PWM == 1)
    memset(str,0,sizeof(str));
    int temp_safemode = Profile_GetBoolean("SafeMode", "bSafeModeEnable", 0);
    printf("--wya-- parse_model_ini temp_safemode =%d\n",temp_safemode);
    if (1 == temp_safemode)
    {
        bSafeModeEnable = 1;
        setenv("bSafeModeEnable","true");
    }
    else
    {
        bSafeModeEnable = 0;
        setenv("bSafeModeEnable","false");
    }
    saveenv();
#endif

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP == 1)
int parse_module_ini(void)
{
    int ret = 0;//
    char *script = NULL;
    U32 filesize = 0;
    U32 u32Urs_Type = 0;
    MS_U8 u8PixelShift = 0;
    char* path = gModulePath;

    UBOOT_TRACE("IN\n");

    script = loadscript(gModulePath,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            ret = -1;
            goto exit;
        }
    }
    UBOOT_INFO("sizeof( module ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    u32Urs_Type = Profile_GetInteger("M_URSA", "F_URSA_URSA_TYPE", 0);
    gstSysMiscSetting.m_u32ursa_type = u32Urs_Type;
    UBOOT_DEBUG("ursa_type = %d \n",u32Urs_Type);
    u8PixelShift = Profile_GetInteger("M_PIXEL_SHIFT", "F_PIXEL_SHIFT_ENABLE", 0);
    gstSysMiscSetting.m_u8PixelShiftEnable = u8PixelShift;
    UBOOT_DEBUG("u8PixelShift = %d \n",u8PixelShift);
exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}
#endif

int parse_boot_ini(char* path)
{
    int ret = 0;//
    int n = 0;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    char *p_vol = NULL;

    UBOOT_TRACE("IN\n");
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        ret = -1;
        goto exit;
    }
    UBOOT_INFO("sizeof( boot ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    //Music
    char* countrycode = getenv("countrycode");
    char cfgname[BUFFER_SIZE] = "\0";

    if(countrycode != NULL && Profile_GetString("MUSIC_CFG", cfgname,"", str, sizearray(str)))
    {
        snprintf(cfgname, BUFFER_SIZE, "MUSIC_NAME_%s", countrycode);
        UBOOT_DEBUG("[MusicCfg] = %s\n", cfgname);
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/%s",CUSTOMER_PATH,str);
    }
    else if(Profile_GetString("MUSIC_CFG", "MUSIC_NAME","", str, sizearray(str)))
    {
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/%s",CUSTOMER_PATH,str);
    }
    else
    {
        // use default music file
        UBOOT_DEBUG("no MUSIC_NAME in %s, use default music file\n",path);
        snprintf(gMusicPath,sizeof(gMusicPath),"%s/boot0.mp3",CUSTOMER_PATH);
    }
    UBOOT_INFO("gMusicPath --> %s\n",gMusicPath);

    //Logo
    if(Profile_GetString("LOGO_CFG", "LOGO_NAME","", str, sizearray(str)))
    {
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/%s",CUSTOMER_PATH,str);
    }
    else
    {
        // use default logo file
        UBOOT_DEBUG("no LOGO_NAME in %s, use default logo file\n",path);
        snprintf(gLogoPath,sizeof(gLogoPath),"%s/boot0.jpg",CUSTOMER_PATH);
    }
    UBOOT_INFO("gLogoPath --> %s\n",gLogoPath);

    //Music volume
    n = Profile_GetInteger("MUSIC_VOL_CFG", "MUSIC_VOL", 0);
    if((n >= 0)&&(n <= 0x7F))
    {
        snprintf(str,sizeof(str),"0x%x",n);
        UBOOT_INFO("m_u32MusicVol -> [%s]\n",str);
        gstSysMiscSetting.m_u32MusicVol = n;

        if(NULL == (p_vol=getenv("music_vol")))  //set MUSIC_VOL to env
        {
            setenv("music_vol", str);
        }
    }
    else
    {
        gstSysMiscSetting.m_u32MusicVol = 30;
        if(NULL == (p_vol=getenv("music_vol")))  //set MUSIC_VOL to env
        {
            setenv("music_vol", "0x1E");
        }
    }

exit:
    if(script)
        free(script);
    UBOOT_TRACE("OK\n");
    return ret;
}


int parse_ini(void)
{
    int ret = -1;
    char InPath [BUFFER_SIZE]="\0";
    char OutPath[BUFFER_SIZE]="\0";
    memset(gPnlPath,0,sizeof(gPnlPath));
    memset(gBoardPath,0,sizeof(gBoardPath));
    memset(gMusicPath,0,sizeof(gMusicPath));
    memset(gLogoPath,0,sizeof(gLogoPath));
    memset(gGopMuxPath,0,sizeof(gGopMuxPath));
    memset(gTconBinPath,0,sizeof(gTconBinPath));
    memset(gLocalDimmingPath,0,sizeof(gLocalDimmingPath));

    UBOOT_TRACE("IN\n");
    snprintf(gLocalDimmingPath,sizeof(gLocalDimmingPath),"%s/ldm/ldm.ini",CONFIG_PATH);

    vfs_mount(CONFIG);
    snprintf(InPath,sizeof(InPath),"%s/sys.ini",CONFIG_PATH);
    ret = parse_sys_ini(InPath,OutPath);
    if(ret)
    {
        goto exit;
    }

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    vfs_mount(CONFIG);
    ret = parse_model_ini(InPath, gPnlPath, gBoardPath, gTconBinPath);
    if(ret)
    {
        goto exit;
    }

    #if (CONFIG_URSA_UNION == 1 || CONFIG_TV_CHIP==1)
    ret = parse_module_ini();
    if(ret)
    {
        UBOOT_ERROR("parse_module_ini fail \n");
    }
    #endif

    vfs_mount(CUSTOMER);
    memset(InPath,0,sizeof(InPath));
    snprintf(InPath,sizeof(InPath),"%s/boot.ini",CUSTOMER_PATH);
    ret = parse_boot_ini(InPath);
    if(ret)
    {
        goto exit;
    }
exit:
    UBOOT_TRACE("OK\n");
    return ret;
}

int prepare_nand_dbtable(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    memset((void*)&gstDbtable, 0, sizeof(gstDbtable));
    vfs_mount(CUSTOMER);

#if(ENABLE_DISPLAY_LOGO)
    gstDbtable.dbdata[E_DB_LOGO].Size = vfs_getsize(gLogoPath);
    UBOOT_DEBUG("gLogoPath=%s\n",gLogoPath);
    UBOOT_DEBUG("gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n", gstDbtable.dbdata[E_DB_LOGO].Size);

    if(gstDbtable.dbdata[E_DB_LOGO].Size > 0)
    {
        UBOOT_DEBUG("%s size =:0x%08x \n",gLogoPath, gstDbtable.dbdata[E_DB_LOGO].Size);
    }
    else
    {
        UBOOT_ERROR(" get logo_size fail...>>>:%d\n", gstDbtable.dbdata[E_DB_LOGO].Size);
        return ret;
    }
#endif

#if ENABLE_POWER_MUSIC
    //get music_size
    gstDbtable.dbdata[E_DB_MUSIC].Size = vfs_getsize(gMusicPath);
    UBOOT_DEBUG("gMusicPath=%s\n",gMusicPath);
    UBOOT_DEBUG("gstDbtable.dbdata[E_DB_MUSIC].Size=0x%x\n",gstDbtable.dbdata[E_DB_MUSIC].Size);
    if( gstDbtable.dbdata[E_DB_MUSIC].Size > 0)
    {
        UBOOT_DEBUG("%s size =:0x%08x \n",gMusicPath,gstDbtable.dbdata[E_DB_MUSIC].Size);
    }
    else
    {
        UBOOT_ERROR(" get music_size fail...>>>:%d\n",gstDbtable.dbdata[E_DB_MUSIC].Size);
        return ret; //althought music is set off, logo can be showed if it is set on
    }
#endif


#if (CONFIG_PANEL_INIT)
    gstDbtable.dbdata[E_DB_PANEL_PARA].Size = sizeof(PanelType);
    gstDbtable.dbdata[E_DB_BOARD_PARA].Size = sizeof(st_board_para);
#endif

#if(ENABLE_ENABLE_URSA == 1)
#if(ENABLE_URSA_6M30 == 1)
    gstDbtable.dbdata[E_DB_URSA_6M30].Size = sizeof(ursa_6m30_cmd_table);
#elif (ENABLE_URSA_8 == 1)|| (ENABLE_URSA_6M40 == 1)
    gstDbtable.dbdata[E_DB_URSA].Size = sizeof(ursa_cmd_table);
#endif
#endif

    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size = sizeof(st_gopmux_para);

    char  *pTconPanelEnv = NULL;
    if (((pTconPanelEnv = getenv("enable_tcon_panel")) != NULL) && (strcmp(pTconPanelEnv,"1") == 0))
    {
        if(ENABLE_MODULE_ANDROID_BOOT==0)
        {
            vfs_mount(CONFIG);
        }
        gstDbtable.dbdata[E_DB_TCON].Size = vfs_getsize(gTconBinPath);
        UBOOT_DEBUG("gLogoPath=%s\n",gTconBinPath);
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_TCON].Size=0x%x\n",gstDbtable.dbdata[E_DB_TCON].Size);

        if(gstDbtable.dbdata[E_DB_TCON].Size > 0)
        {
            UBOOT_DEBUG("%s size =:0x%08x \n",gTconBinPath,gstDbtable.dbdata[E_DB_TCON].Size);
        }
        else
        {
            UBOOT_ERROR(" get tcon_size fail...>>>:%d\n",gstDbtable.dbdata[E_DB_TCON].Size);
            return ret;
        }
    }

    gstDbtable.dbdata[E_DB_PNL_VB1].Size = sizeof(pnl_VB1ChannelOrder);
    gstDbtable.dbdata[E_DB_PWM].Size = sizeof(st_pwm_setting);
    gstDbtable.dbdata[E_DB_MISC].Size = sizeof(st_sys_misc_setting);
#if (CONFIG_LOCAL_DIMMING)
    gstDbtable.dbdata[E_DB_MSPI].Size = sizeof(ST_DRV_MSPI_INFO);
    gstDbtable.dbdata[E_DB_DMA].Size = sizeof(ST_DRV_LD_DMA_INFO);
#endif
    gstDbtable.dbdata[E_DB_HDMITX].Size = sizeof(st_hdmitx_setting);
#if (CONFIG_DYNAMIC_IR)
    gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size = sizeof(st_IRBootCheck_t);
    gstDbtable.dbdata[E_DB_IR_WAKEUP].Size = sizeof(st_IRWakeup_t);
#endif
    //calculate gstDbtable offset
    gstDbtable.dbdata[E_DB_LOGO].Offset       = u32DbtableOffset + sizeof(gstDbtable);
    gstDbtable.dbdata[E_DB_MUSIC].Offset      = gstDbtable.dbdata[E_DB_LOGO].Offset       + gstDbtable.dbdata[E_DB_LOGO].Size;
    gstDbtable.dbdata[E_DB_BOARD_PARA].Offset = gstDbtable.dbdata[E_DB_MUSIC].Offset      + gstDbtable.dbdata[E_DB_MUSIC].Size;
    gstDbtable.dbdata[E_DB_PANEL_PARA].Offset = gstDbtable.dbdata[E_DB_BOARD_PARA].Offset + gstDbtable.dbdata[E_DB_BOARD_PARA].Size;
    gstDbtable.dbdata[E_DB_URSA_6M30].Offset  = gstDbtable.dbdata[E_DB_PANEL_PARA].Offset + gstDbtable.dbdata[E_DB_PANEL_PARA].Size;
    gstDbtable.dbdata[E_DB_URSA].Offset       = gstDbtable.dbdata[E_DB_URSA_6M30].Offset  + gstDbtable.dbdata[E_DB_URSA_6M30].Size;
    gstDbtable.dbdata[E_DB_TCON].Offset       = gstDbtable.dbdata[E_DB_URSA].Offset       + gstDbtable.dbdata[E_DB_URSA].Size;
    gstDbtable.dbdata[E_DB_PNL_VB1].Offset    = gstDbtable.dbdata[E_DB_TCON].Offset       + gstDbtable.dbdata[E_DB_TCON].Size;
    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset = gstDbtable.dbdata[E_DB_PNL_VB1].Offset    + gstDbtable.dbdata[E_DB_PNL_VB1].Size;
    gstDbtable.dbdata[E_DB_PWM].Offset        = gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size;
    gstDbtable.dbdata[E_DB_MISC].Offset       = gstDbtable.dbdata[E_DB_PWM].Offset        + gstDbtable.dbdata[E_DB_PWM].Size;
    gstDbtable.dbdata[E_DB_MSPI].Offset       = gstDbtable.dbdata[E_DB_MISC].Offset       + gstDbtable.dbdata[E_DB_MISC].Size;
    gstDbtable.dbdata[E_DB_DMA].Offset       = gstDbtable.dbdata[E_DB_MSPI].Offset       + gstDbtable.dbdata[E_DB_MSPI].Size;
    gstDbtable.dbdata[E_DB_HDMITX].Offset     = gstDbtable.dbdata[E_DB_DMA].Offset       + gstDbtable.dbdata[E_DB_DMA].Size;
    gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset       = gstDbtable.dbdata[E_DB_HDMITX].Offset       + gstDbtable.dbdata[E_DB_HDMITX].Size;
    gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset     = gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset       + gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size;

    _print_dbtable_info();

    {
        U32 used_size = 0;
        U32 mpool_size = 0;
        // Get MPool volume/partition size
        mpool_size = GetRealOffset(0);

        u32Dbload_size =  sizeof(gstDbtable)
                        + gstDbtable.dbdata[E_DB_LOGO].Size
                        + gstDbtable.dbdata[E_DB_MUSIC].Size
                        + gstDbtable.dbdata[E_DB_BOARD_PARA].Size
                        + gstDbtable.dbdata[E_DB_PANEL_PARA].Size
                        + gstDbtable.dbdata[E_DB_URSA_6M30].Size
                        + gstDbtable.dbdata[E_DB_URSA].Size
                        + gstDbtable.dbdata[E_DB_TCON].Size
                        + gstDbtable.dbdata[E_DB_PNL_VB1].Size
                        + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size
                        + gstDbtable.dbdata[E_DB_PWM].Size
                        + gstDbtable.dbdata[E_DB_MISC].Size
                        + gstDbtable.dbdata[E_DB_MSPI].Size
                        + gstDbtable.dbdata[E_DB_DMA].Size
                        + gstDbtable.dbdata[E_DB_HDMITX].Size
                        + gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size
                        + gstDbtable.dbdata[E_DB_IR_WAKEUP].Size;


        //roughly calculate mpool data used size
        used_size = u32DbtableOffset + u32Dbload_size + env_sector_size;

        UBOOT_DEBUG("u32DbtableOffset  =: 0x%x \n", u32DbtableOffset);
        UBOOT_DEBUG("u32Dbload_size    =: 0x%x \n", u32Dbload_size);
        UBOOT_DEBUG("used_size         =: 0x%x \n", used_size);
        UBOOT_DEBUG("mpool_size        =: 0x%x \n", mpool_size);

        // mpool data is over mpool volume/partition size
        if(used_size >= mpool_size)
        {
            UBOOT_ERROR("mpool data is over mpool volume/partition size\n");
            jump_to_console();
        }

    }

    pDbBufferAddr = malloc(u32Dbload_size);
    if(pDbBufferAddr == NULL)
    {
        UBOOT_ERROR("malloc dbload memory error !");
        return ret;
    }

    // copy db_table to DB Pool memory buffer
    memset((void*)pDbBufferAddr,0,u32Dbload_size);
    ret = 0;


    UBOOT_TRACE("OK\n");
    return ret;
}

int write_dbtable(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    memset(&gstSysMiscSetting,0,sizeof(st_sys_misc_setting));
    memset(&gstPWMSetting,0,sizeof(st_pwm_setting));

    parse_ini();
    if(prepare_nand_dbtable(u32DbtableOffset)==-1)
    {
        if(pDbBufferAddr)
        {
            free(pDbBufferAddr);
            pDbBufferAddr = NULL;
        }
        UBOOT_ERROR("flash database not ready...>>>\n");
        return ret;
    }
    else
    {

#if ENABLE_DISPLAY_LOGO
        Load_LogoToFlash(u32DbtableOffset);
#endif
#if ENABLE_POWER_MUSIC
        Load_MusicToFlash(u32DbtableOffset);
#endif
#if (CONFIG_PANEL_INIT)

        char *p_str = NULL;
        p_str = getenv ("panel_path");
        if(NULL != p_str)
        {
            memcpy(gPnlPath, p_str, sizeof(gPnlPath));
        }
        Load_PanelSetting_ToFlash(u32DbtableOffset);
        Load_BoardSetting_ToFlash(u32DbtableOffset);
#endif
#if ENABLE_ENABLE_URSA
#if ENABLE_URSA_6M30
        //Load_Ursa6m30Para_ToFlash();
#elif (ENABLE_URSA_8 == 1) || (ENABLE_URSA_6M40 == 1)
        Load_UrsaPara_ToFlash(u32DbtableOffset);
#endif
#endif
        Load_GopMuxToFlash(u32DbtableOffset);

        char  *pTconPanelEnv = NULL;
        if (((pTconPanelEnv = getenv("enable_tcon_panel")) != NULL) && (strcmp(pTconPanelEnv,"1") == 0))
        {
            Load_TConToFlash(u32DbtableOffset);
        }

        Load_PnlVb1ToFlash(u32DbtableOffset);
        Load_PWMSetting_ToFlash(u32DbtableOffset);
        Load_MiscSetting_ToFlash(u32DbtableOffset);
#if (CONFIG_LOCAL_DIMMING)
        Load_LDMPara_ToFlash(u32DbtableOffset);
#endif
        Load_HDMITXToFlash(u32DbtableOffset);
#if (CONFIG_DYNAMIC_IR)
        Load_IRConfig_ToFlash(u32DbtableOffset);
#endif
        //calculate crc32 of gstDbtable
        gstDbtable.u32CrcCheck = crc32(0, (unsigned char *)&gstDbtable, sizeof(ST_DBDATA)*E_DB_MAX);
        UBOOT_DEBUG("gstDbtable.u32CrcCheck=%08x\n",gstDbtable.u32CrcCheck);

        // copy db_table to DB Pool memory buffer
        memcpy((void*)pDbBufferAddr,(const void *)&gstDbtable,sizeof(gstDbtable));

        // save DB Pool data to flash
        ret = raw_write((unsigned int)(pDbBufferAddr), u32DbtableOffset, u32Dbload_size);
        if(ret == -1)
        {
            UBOOT_ERROR("write data to flash fail...>>>\n");
        }
        else
        {
            UBOOT_TRACE("OK\n");
        }
        if(pDbBufferAddr)
        {
            free(pDbBufferAddr);
            pDbBufferAddr = NULL;
        }

        setenv("db_table","1");
        saveenv();
        ret = 0;
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

void _print_dbtable_info(void)
{
    UBOOT_DEBUG("read_dbtable success...>>>\n");
    UBOOT_DEBUG("read table >>> \n");
    UBOOT_DEBUG("logo  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_LOGO].Offset,        gstDbtable.dbdata[E_DB_LOGO].Size,       (gstDbtable.dbdata[E_DB_LOGO].Offset       + gstDbtable.dbdata[E_DB_LOGO].Size));
    UBOOT_DEBUG("music offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MUSIC].Offset,       gstDbtable.dbdata[E_DB_MUSIC].Size,      (gstDbtable.dbdata[E_DB_MUSIC].Offset      + gstDbtable.dbdata[E_DB_MUSIC].Size));
    UBOOT_DEBUG("board offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_BOARD_PARA].Offset,  gstDbtable.dbdata[E_DB_BOARD_PARA].Size, (gstDbtable.dbdata[E_DB_BOARD_PARA].Offset + gstDbtable.dbdata[E_DB_BOARD_PARA].Size));
    UBOOT_DEBUG("panel offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PANEL_PARA].Offset,  gstDbtable.dbdata[E_DB_PANEL_PARA].Size, (gstDbtable.dbdata[E_DB_PANEL_PARA].Offset + gstDbtable.dbdata[E_DB_PANEL_PARA].Size));
    UBOOT_DEBUG("ursa 6m30  offset[0x%08x] size[0x%08x] end[0x%08x]\n", gstDbtable.dbdata[E_DB_URSA_6M30].Offset,   gstDbtable.dbdata[E_DB_URSA_6M30].Size,  (gstDbtable.dbdata[E_DB_URSA_6M30].Offset  + gstDbtable.dbdata[E_DB_URSA_6M30].Size));
    UBOOT_DEBUG("ursa  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_URSA].Offset,        gstDbtable.dbdata[E_DB_URSA].Size,       (gstDbtable.dbdata[E_DB_URSA].Offset       + gstDbtable.dbdata[E_DB_URSA].Size));
    UBOOT_DEBUG("tcon  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_TCON].Offset,        gstDbtable.dbdata[E_DB_TCON].Size,       (gstDbtable.dbdata[E_DB_TCON].Offset       + gstDbtable.dbdata[E_DB_TCON].Size));
    UBOOT_DEBUG("vb1   offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PNL_VB1].Offset,     gstDbtable.dbdata[E_DB_PNL_VB1].Size,    (gstDbtable.dbdata[E_DB_PNL_VB1].Offset    + gstDbtable.dbdata[E_DB_PNL_VB1].Size));
    UBOOT_DEBUG("gopmux  offset[0x%08x] size[0x%08x] end[0x%08x]\n",    gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset,  gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size, (gstDbtable.dbdata[E_DB_PNL_GOPMUX].Offset + gstDbtable.dbdata[E_DB_PNL_GOPMUX].Size));
    UBOOT_DEBUG("pwm   offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_PWM].Offset,         gstDbtable.dbdata[E_DB_PWM].Size,        (gstDbtable.dbdata[E_DB_PWM].Offset        + gstDbtable.dbdata[E_DB_PWM].Size));
    UBOOT_DEBUG("misc  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MISC].Offset,        gstDbtable.dbdata[E_DB_MISC].Size,       (gstDbtable.dbdata[E_DB_MISC].Offset       + gstDbtable.dbdata[E_DB_MISC].Size));
    UBOOT_DEBUG("mspi  offset[0x%08x] size[0x%08x] end[0x%08x]\n",      gstDbtable.dbdata[E_DB_MSPI].Offset,        gstDbtable.dbdata[E_DB_MSPI].Size,       (gstDbtable.dbdata[E_DB_MSPI].Offset       + gstDbtable.dbdata[E_DB_MSPI].Size));
    UBOOT_DEBUG("dma  offset[0x%08x] size[0x%08x] end[0x%08x]\n",       gstDbtable.dbdata[E_DB_DMA].Offset,         gstDbtable.dbdata[E_DB_DMA].Size,        (gstDbtable.dbdata[E_DB_DMA].Offset        + gstDbtable.dbdata[E_DB_DMA].Size));
    UBOOT_DEBUG("bootcheck  offset[0x%08x] size[0x%08x] end[0x%08x]\n", gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset,gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size,(gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Offset + gstDbtable.dbdata[E_DB_IR_BOOTCHECK].Size));
    UBOOT_DEBUG("wakeup  offset[0x%08x] size[0x%08x] end[0x%08x]\n",       gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset,   gstDbtable.dbdata[E_DB_IR_WAKEUP].Size,  (gstDbtable.dbdata[E_DB_IR_WAKEUP].Offset  + gstDbtable.dbdata[E_DB_IR_WAKEUP].Size));
}
int read_dbtable(U32 u32DbtableOffset)
{
    if(-1 == raw_read((U32)&gstDbtable,u32DbtableOffset,sizeof(gstDbtable)))
    {
        UBOOT_ERROR("ERROR: read_mmc_dbtable fail...>>>\n");
        return -1;
    }
    else
    {
        _print_dbtable_info();
    }
    U32 checksum = 0,savesum = 0;
    checksum = crc32(0, (unsigned char *)&gstDbtable, sizeof(ST_DBDATA)*E_DB_MAX);
    savesum = gstDbtable.u32CrcCheck;

    if(checksum != savesum)
    {
        UBOOT_ERROR("Error : savesum =%08x , checksum= %08x \n",savesum,checksum);
        UBOOT_ERROR("Reload the dbtable from AP !!!\n");
        return write_dbtable(u32DbtableOffset);
    }
    return 0;
}


void count_data_crc32_save(U32 addr,int size,DB_INDEX db_index)
{
    gstDbtable.dbdata[db_index].u32CrcCheck = crc32(0,(unsigned char *)addr,size);
    UBOOT_DEBUG("%s checksum =: 0x%08x\n",sDB_INDEX[db_index],gstDbtable.dbdata[db_index].u32CrcCheck);
}

int count_data_crc32_cmp(U32 addr,int size, DB_INDEX db_index)
{
    int ret = -1;
    U32 checksum,savesum;
    checksum = crc32(0,(unsigned char *)addr,size);

    savesum = gstDbtable.dbdata[db_index].u32CrcCheck;

    if(checksum == savesum)
    {
        UBOOT_DEBUG("## CRC check done, %s\n", sDB_INDEX[db_index]);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("checksum :0X%x ; savesum :0X%x\n",checksum,savesum);
        UBOOT_ERROR("## CRC check Error, %s\n", sDB_INDEX[db_index]);
        setenv("db_table","0");
        saveenv();
    }
    return ret;
}

int Read_FlashDataToAddr(U32 u32DstAddr, DB_INDEX db_index)
{
	int ret = -1;
    UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("gstDbtable.dbdata[%s] Offset =:0x%x ... Size=: 0x%x\n",
                sDB_INDEX[db_index],
                gstDbtable.dbdata[db_index].Offset,
                gstDbtable.dbdata[db_index].Size);

    if(-1 == raw_read(u32DstAddr, gstDbtable.dbdata[db_index].Offset, gstDbtable.dbdata[db_index].Size))
    {
        UBOOT_ERROR("ERROR: Read_FlashDataToAddr fail...>>>\n");
    }
    else
    {
        UBOOT_DEBUG("Read_FlashDataToAddr success...>>>\n");
        //crc check
        if(count_data_crc32_cmp(u32DstAddr,gstDbtable.dbdata[db_index].Size,db_index)==-1)
        {
            UBOOT_ERROR("check FlashDataToAddr fail..>>>\n");
            return ret;
        }
        else
        {
            ret = 0;
        }
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_DataToDbBuffer(U32 u32DbtableOffset, DB_INDEX db_index, U32 pu32DataAddr)
{
	UBOOT_TRACE("IN\n");
    UBOOT_DEBUG("gstDbtable.dbdata[%s] Offset =:0x%x ... Size=: 0x%x\n",
                sDB_INDEX[db_index],
                gstDbtable.dbdata[db_index].Offset,
                gstDbtable.dbdata[db_index].Size);
	// copy data to db pool memory buffer
    memcpy( (void*)(pDbBufferAddr + MPoolVolumeOffset(gstDbtable.dbdata[db_index].Offset,u32DbtableOffset)),
            (void*)pu32DataAddr,
            gstDbtable.dbdata[db_index].Size);

    count_data_crc32_save(pu32DataAddr,gstDbtable.dbdata[db_index].Size,db_index);
	UBOOT_TRACE("OK\n");
	return 0;
}

#if ENABLE_DISPLAY_LOGO
int Load_LogoToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32LogoAddr=NULL;
    unsigned int u32fileSize   = 0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CUSTOMER);
    u32fileSize=vfs_getsize(gLogoPath);
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_LOGO].Size=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_LOGO].Size);
    pu32LogoAddr=malloc(u32fileSize);
    if(pu32LogoAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32LogoAddr memory error !");
        return -1;
    }
    memset((void*)pu32LogoAddr,0,gstDbtable.dbdata[E_DB_LOGO].Size);
    if (vfs_read((void *)pu32LogoAddr,gLogoPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_LOGO, (U32)pu32LogoAddr);
    }
    else
    {
        UBOOT_ERROR("ubifsload data fail ...>>>\n");
    }

    free(pu32LogoAddr);
    pu32LogoAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}
int Read_LogoToDisplayAddr(U32 u32LogoAddr)
{
    int ret = -1;
    ret = Read_FlashDataToAddr(u32LogoAddr, E_DB_LOGO);
    return ret;
}
#endif

#if ENABLE_POWER_MUSIC
int Load_MusicToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32MusicAddr=NULL;
    U32 u32fileSize=0;
    UBOOT_TRACE("IN\n");
    vfs_mount(CUSTOMER);
    u32fileSize=vfs_getsize(gMusicPath);
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_MUSIC].Size=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_MUSIC].Size);
    pu32MusicAddr = malloc(u32fileSize);
    if(pu32MusicAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32MusicAddr memory error !");
        return -1;
    }
    memset((void*)pu32MusicAddr,0,gstDbtable.dbdata[E_DB_MUSIC].Size);
    if (vfs_read((void *)pu32MusicAddr,gMusicPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MUSIC, (U32)pu32MusicAddr);
    }
    else
    {
        UBOOT_ERROR("load mp3 file faile>>>>>>>>>>\n");
    }
    free(pu32MusicAddr);
    pu32MusicAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MusicToPlayAddr(MS_U32 u32MusicDramAddr)
{
    int ret = -1;
    U32 u32MusicAddr = (U32)(PA2NVA(u32MusicDramAddr));
    // Clear ES1 buffer = 64K
    memset((void*)(u32MusicAddr), 0, 65536);
    ret = Read_FlashDataToAddr(u32MusicAddr, E_DB_MUSIC);
    return ret;
}

#endif

#if (CONFIG_PANEL_INIT)
int parse_pnl_ini(char *path,PanelType *p_data)
{
    long n;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( panel ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    n = Profile_GetInteger("panel", "m_bPanelDither", 0);
    UBOOT_DEBUG("m_bPanelDither = %ld \n",n);
    p_data->m_bPanelDither = n;
    memset(str,0,sizeof(str));
    Profile_GetString("panel", "m_pPanelName","", str, sizearray(str));

    n = Profile_GetInteger("panel", "m_ePanelLinkType", 0);
    UBOOT_DEBUG("m_ePanelLinkType = %ld \n",n);
    p_data->m_ePanelLinkType = (APIPNL_LINK_TYPE)n;
    //Profile_GetString("panel", "m_ePanelLinkExtType","", str, sizearray(str));
    n = Profile_GetInteger("panel", "m_ePanelLinkExtType", 0);
    UBOOT_DEBUG("m_ePanelLinkExtType = %ld \n",n);
//    setenv("panel_ext_type", str);
    gstSysMiscSetting.m_u16Panel_ext_type = n;
	Profile_GetString("panel", "m_u16PANEL_LVDS_CONNECT_TYPE","", str, sizearray(str));
    UBOOT_DEBUG("m_u16PANEL_LVDS_CONNECT_TYPE = %s \n",str);
    memset(gstSysMiscSetting.m_Panel_LVDS_Connect_Type, 0, sizeof(gstSysMiscSetting.m_Panel_LVDS_Connect_Type));
    strncpy(gstSysMiscSetting.m_Panel_LVDS_Connect_Type, str, sizeof(gstSysMiscSetting.m_Panel_LVDS_Connect_Type)-1);
    //saveenv();

    n = Profile_GetInteger("panel", "m_bPanelDualPort", 0);
    UBOOT_DEBUG("m_bPanelDualPort = %ld \n",n);
    p_data->m_bPanelDualPort = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapPort", 0);
    UBOOT_DEBUG("m_bPanelSwapPort  = %ld \n",n);
    p_data->m_bPanelSwapPort = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_ML", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_ML = %ld \n",n);
    p_data->m_bPanelSwapOdd_ML  = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_ML", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_ML = %ld \n",n);
    p_data->m_bPanelSwapEven_ML = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_RB", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_RB = %ld \n",n);
    p_data->m_bPanelSwapOdd_RB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_RB", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_RB = %ld \n",n);
    p_data->m_bPanelSwapEven_RB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapLVDS_POL", 0);
    UBOOT_DEBUG("m_bPanelSwapLVDS_POL = %ld \n",n);
    p_data->m_bPanelSwapLVDS_POL = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapLVDS_CH", 0);
    UBOOT_DEBUG("m_bPanelSwapLVDS_CH = %ld \n",n);
    p_data->m_bPanelSwapLVDS_CH = n;
    n = Profile_GetInteger("panel", "m_bPanelPDP10BIT", 0);
    UBOOT_DEBUG("m_bPanelPDP10BIT = %ld \n",n);
    p_data->m_bPanelPDP10BIT = n;
    n = Profile_GetInteger("panel", "m_bPanelLVDS_TI_MODE", 0);
    UBOOT_DEBUG("m_bPanelLVDS_TI_MODE = %ld \n",n);
    p_data->m_bPanelLVDS_TI_MODE = n;
    n = Profile_GetInteger("panel", "m_ucPanelDCLKDelay", 0);
    UBOOT_DEBUG("m_ucPanelDCLKDelay = %ld \n",n);
    p_data->m_ucPanelDCLKDelay = n;
    n = Profile_GetInteger("panel", "m_bPanelInvDCLK", 0);
    UBOOT_DEBUG("m_bPanelInvDCLK = %ld \n",n);
    p_data->m_bPanelInvDCLK = n;
    n = Profile_GetInteger("panel", "m_bPanelInvDE", 0);
    UBOOT_DEBUG("m_bPanelInvDE = %ld \n",n);
    p_data->m_bPanelInvDE = n;
    n = Profile_GetInteger("panel", "m_bPanelInvHSync", 0);
    UBOOT_DEBUG("m_bPanelInvHSync = %ld \n",n);
    p_data->m_bPanelInvHSync = n;
    n = Profile_GetInteger("panel", "m_bPanelInvVSync", 0);
    UBOOT_DEBUG("m_bPanelInvVSync = %ld \n",n);
    p_data->m_bPanelInvVSync = n;
    n = Profile_GetInteger("panel", "m_ucPanelDCKLCurrent", 0);
    UBOOT_DEBUG("m_ucPanelDCKLCurrent = %ld \n",n);
    p_data->m_ucPanelDCKLCurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelDECurrent", 0);
    UBOOT_DEBUG("m_ucPanelDECurrent = %ld \n",n);
    p_data->m_ucPanelDECurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelODDDataCurrent", 0);
    UBOOT_DEBUG("m_ucPanelODDDataCurrent = %ld \n",n);
    p_data->m_ucPanelODDDataCurrent = n;
    n = Profile_GetInteger("panel", "m_ucPanelEvenDataCurrent", 0);
    UBOOT_DEBUG("m_ucPanelEvenDataCurrent = %ld \n",n);
    p_data->m_ucPanelEvenDataCurrent = n;
    n = Profile_GetInteger("panel", "m_wPanelOnTiming1", 0);
    UBOOT_DEBUG("m_wPanelOnTiming1 = %ld \n",n);
    p_data->m_wPanelOnTiming1 = n;
    n = Profile_GetInteger("panel", "m_wPanelOnTiming2", 0);
    UBOOT_DEBUG("m_wPanelOnTiming2 = %ld \n",n);
    p_data->m_wPanelOnTiming2 = n;
    n = Profile_GetInteger("panel", "m_wPanelOffTiming1", 0);
    UBOOT_DEBUG("m_wPanelOffTiming1 = %ld \n",n);
    p_data->m_wPanelOffTiming1 = n;
    n = Profile_GetInteger("panel", "m_wPanelOffTiming2", 0);
    UBOOT_DEBUG("m_wPanelOffTiming2 = %ld \n",n);
    p_data->m_wPanelOffTiming2 = n;
    n = Profile_GetInteger("panel", "m_ucPanelHSyncWidth", 0);
    UBOOT_DEBUG("m_ucPanelHSyncWidth  = %ld \n",n);
    p_data->m_ucPanelHSyncWidth = n;
    n = Profile_GetInteger("panel", "m_ucPanelHSyncBackPorch", 0);
    UBOOT_DEBUG("m_ucPanelHSyncBackPorch  = %ld \n",n);
    p_data->m_ucPanelHSyncBackPorch = n;

    n = Profile_GetInteger("panel", "m_ucPanelVSyncWidth", 0);
    UBOOT_DEBUG("m_ucPanelVSyncWidth = %ld \n",n);
    p_data->m_ucPanelVSyncWidth = n;
    n = Profile_GetInteger("panel", "m_ucPanelVBackPorch", 0);
    UBOOT_DEBUG("m_ucPanelVBackPorch = %ld \n",n);
    p_data->m_ucPanelVBackPorch = n;

    n = Profile_GetInteger("panel", "m_wPanelHStart", 0);
    UBOOT_DEBUG("m_wPanelHStart = %ld \n",n);
    p_data->m_wPanelHStart = n;
    n = Profile_GetInteger("panel", "m_wPanelVStart", 0);
    UBOOT_DEBUG("m_wPanelVStart = %ld \n",n);
    p_data->m_wPanelVStart = n;
    n = Profile_GetInteger("panel", "m_wPanelWidth", 0);
    UBOOT_DEBUG("m_wPanelWidth = %ld \n",n);
    p_data->m_wPanelWidth = n;
    n = Profile_GetInteger("panel", "m_wPanelHeight", 0);
    UBOOT_DEBUG("m_wPanelHeight = %ld \n",n);
    p_data->m_wPanelHeight = n;

    n = Profile_GetInteger("panel", "m_wPanelMaxHTotal", 0);
    UBOOT_DEBUG("m_wPanelMaxHTotal = %ld \n",n);
    p_data->m_wPanelMaxHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelHTotal", 0);
    UBOOT_DEBUG("m_wPanelHTotal = %ld \n",n);
    p_data->m_wPanelHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelMinHTotal", 0);
    UBOOT_DEBUG("m_wPanelMinHTotal = %ld \n",n);
    p_data->m_wPanelMinHTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelMaxVTotal", 0);
    UBOOT_DEBUG("m_wPanelMaxVTotal = %ld \n",n);
    p_data->m_wPanelMaxVTotal = n;
    n = Profile_GetInteger("panel", "m_wPanelVTotal", 0);
    UBOOT_DEBUG("m_wPanelVTotal = %ld \n",n);
    p_data->m_wPanelVTotal = n;

    n = Profile_GetInteger("panel", "m_wPanelMinVTotal", 0);
    UBOOT_DEBUG("m_wPanelMinVTotal = %ld \n",n);
    p_data->m_wPanelMinVTotal = n;
    n = Profile_GetInteger("panel", "m_dwPanelMaxDCLK", 0);
    UBOOT_DEBUG("m_dwPanelMaxDCLK = %ld \n",n);
    p_data->m_dwPanelMaxDCLK = n;
    n = Profile_GetInteger("panel", "m_dwPanelDCLK", 0);
    UBOOT_DEBUG("m_dwPanelDCLK = %ld \n",n);
    p_data->m_dwPanelDCLK = n;
    gstSysMiscSetting.m_u16PanelDCLK = n;

    n = Profile_GetInteger("panel", "m_dwPanelMinDCLK", 0);
    UBOOT_DEBUG("m_dwPanelMinDCLK = %ld \n",n);
    p_data->m_dwPanelMinDCLK = n;
    n = Profile_GetInteger("panel", "m_wSpreadSpectrumStep", 0);
    UBOOT_DEBUG("m_wSpreadSpectrumStep = %ld \n",n);
    p_data->m_wSpreadSpectrumStep = n;
    n = Profile_GetInteger("panel", "m_wSpreadSpectrumSpan", 0);
    UBOOT_DEBUG("m_wSpreadSpectrumSpan = %ld \n",n);
    p_data->m_wSpreadSpectrumSpan = n;
    n = Profile_GetInteger("panel", "m_ucDimmingCtl", 0);
    UBOOT_DEBUG("m_ucDimmingCtl = %ld \n",n);
    p_data->m_ucDimmingCtl = n;
    n = Profile_GetInteger("panel", "m_ucMaxPWMVal", 0);
    UBOOT_DEBUG("m_ucMaxPWMVal = %ld \n",n);
    p_data->m_ucMaxPWMVal = n;
    n = Profile_GetInteger("panel", "m_ucMinPWMVal", 0);
    UBOOT_DEBUG("m_ucMinPWMVal = %ld \n",n);
    p_data->m_ucMinPWMVal = n;
    n = Profile_GetInteger("panel", "m_bPanelDeinterMode", 0);
    UBOOT_DEBUG("m_bPanelDeinterMode = %ld \n",n);
    p_data->m_bPanelDeinterMode = n;
    n = Profile_GetInteger("panel", "m_ucPanelAspectRatio", 0);
    UBOOT_DEBUG("m_ucPanelAspectRatio = %ld \n",n);
    p_data->m_ucPanelAspectRatio = (E_PNL_ASPECT_RATIO)n;

    n = Profile_GetInteger("panel", "m_u16LVDSTxSwapValue", 0);
    UBOOT_DEBUG("m_u16LVDSTxSwapValue = %ld \n",n);
    p_data->m_u16LVDSTxSwapValue = n;
    n = Profile_GetInteger("panel", "m_ucTiBitMode", 0);
    UBOOT_DEBUG("m_ucTiBitMode = %ld \n",n);
    p_data->m_ucTiBitMode = (APIPNL_TIBITMODE)n;

    n = Profile_GetInteger("panel", "m_ucOutputFormatBitMode", 0);
    UBOOT_DEBUG("m_ucOutputFormatBitMode = %ld \n",n);
    p_data->m_ucOutputFormatBitMode = (APIPNL_OUTPUTFORMAT_BITMODE)n;

    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_RG", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_RG = %ld \n",n);
    p_data->m_bPanelSwapOdd_RG = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_RG", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_RG = %ld \n",n);
    p_data->m_bPanelSwapEven_RG = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapOdd_GB", 0);
    UBOOT_DEBUG("m_bPanelSwapOdd_GB = %ld \n",n);
    p_data->m_bPanelSwapOdd_GB = n;
    n = Profile_GetInteger("panel", "m_bPanelSwapEven_GB", 0);
    UBOOT_DEBUG("m_bPanelSwapEven_GB = %ld \n",n);
    p_data->m_bPanelSwapEven_GB = n;
    n = Profile_GetInteger("panel", "m_bPanelDoubleClk", 0);
    UBOOT_DEBUG("m_bPanelDoubleClk = %ld \n",n);
    p_data->m_bPanelDoubleClk = n;
    n = Profile_GetInteger("panel", "m_dwPanelMaxSET", 0);
    UBOOT_DEBUG("m_dwPanelMaxSET = %ld \n",n);
    p_data->m_dwPanelMaxSET = n;
    n = Profile_GetInteger("panel", "m_dwPanelMinSET", 0);
    UBOOT_DEBUG("m_dwPanelMinSET = %ld \n",n);
    p_data->m_dwPanelMinSET = n;
    n = Profile_GetInteger("panel", "m_ucOutTimingMode", 0);
    UBOOT_DEBUG("m_ucOutTimingMode = %ld \n",n);
    p_data->m_ucOutTimingMode = (APIPNL_OUT_TIMING_MODE)n;
#if 1//use pwm setting in panel.ini and save it to env
    n = Profile_GetInteger("panel", "u32PeriodPWM", 0);
    UBOOT_INFO("u32PeriodPWM = %ld \n",n);
    gstPWMSetting.m_u32PWMPeriod = n;


    n = Profile_GetInteger("panel", "u16DivPWM", 0);
    UBOOT_DEBUG("u16DivPWM = %ld \n",n);
    gstPWMSetting.m_u16DivPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("u32DutyPWM = %s\n",str);
    gstPWMSetting.m_u32PWMDuty = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("bPolPWM = %s\n",str);
    gstPWMSetting.m_bPolPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MaxPWMvalue = %s\n",str);
    gstPWMSetting.m_u16maxPWM = n;

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("u16MinPWMvalue = %s\n",str);
    gstPWMSetting.m_u16minPWM = n;
    memset(str,0,sizeof(str));

#endif

#if(CONFIG_ENABLE_V_I_PWM == 1)
    // I_PWM
    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_PWM_ENABLE", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_PWM_ENABLE = %ld \n",n);
    setenv(I_PWM_ENABLE,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u8PWM_CH", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_u8PWM_CH = %ld \n",n);
    setenv(I_PWM_CH,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u32PeriodPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_u32PeriodPWM = %ld \n",n);
    setenv(I_PWM_PERIOD,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u16DivPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_u16DivPWM = %ld \n",n);
    setenv(I_PWM_DIVPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("I_u32DutyPWM = %s\n",str);
    setenv(I_PWM_DUTY,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_bPolPWM = %s\n",str);
    setenv(I_PWM_POLPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_u16MaxPWMvalue = %s\n",str);
    setenv(I_PWM_MAXPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("I_u16MinPWMvalue = %s\n",str);
    setenv(I_PWM_MINPWM,str);
    memset(str,0,sizeof(str));

    // V_PWM
    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel",  "V_PWM_ENABLE", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_PWM_ENABLE = %ld \n",n);
    setenv(V_PWM_ENABLE,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u8PWM_CH", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_u8PWM_CH = %ld \n",n);
    setenv(V_PWM_CH,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u32PeriodPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_u32PeriodPWM = %ld \n",n);
    setenv(V_PWM_PERIOD,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u16DivPWM", 0);
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_u16DivPWM = %ld \n",n);
    setenv(V_PWM_DIVPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u32DutyPWM", 0);// Duty use Div value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_INFO("V_u32DutyPWM = %s\n",str);
    setenv(V_PWM_DUTY,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "I_bPolPWM", 0);// Duty use polpwm value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_bPolPWM = %s\n",str);
    setenv(V_PWM_POLPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u16MaxPWMvalue", 0);// Duty use u16MaxPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_u16MaxPWMvalue = %s\n",str);
    setenv(V_PWM_MAXPWM,str);

    memset(str,0,sizeof(str));
    n = Profile_GetInteger("panel", "V_u16MinPWMvalue", 0);// Duty use u16MinPWMvalue value,fix it later
    snprintf(str,sizeof(str),"0x%x",n);
    UBOOT_DEBUG("V_u16MinPWMvalue = %s\n",str);
    setenv(V_PWM_MINPWM,str);

    memset(str,0,sizeof(str));


    memset(str,0,sizeof(str));
    Profile_GetString("panel", "PNL_VB1_Control","false", str, sizearray(str));
    setenv("PNL_VB1_Control",str);

    memset(str,0,sizeof(str));
#endif

    if(script)
        free(script);
    return 0;
}

int Load_PanelSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    PanelType pnl;
    UBOOT_TRACE("IN\n");
    memset(&pnl,0,sizeof(pnl));
    vfs_mount(CONFIG);

    if(parse_pnl_ini(gPnlPath,&pnl)!=0)
    {
        UBOOT_ERROR("parse_pnl_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("panel para size =: 0x%08x \n",sizeof(pnl));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PANEL_PARA, (U32)&pnl);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PanelParaFromflash(PanelType * panel_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)panel_data, E_DB_PANEL_PARA);
    UBOOT_DEBUG("panel_data.m_wPanelVStart=: 0x%x\n",panel_data->m_wPanelVStart);
    UBOOT_DEBUG("panel_data.m_wPanelWidth=: 0x%x\n",panel_data->m_wPanelWidth);
    UBOOT_DEBUG("panel_data.m_wPanelHeight=: 0x%x\n",panel_data->m_wPanelHeight);
    return ret;
}

int parse_board_ini(char *path, st_board_para *p_data)
{
    long n;
    char *script = NULL;
    U32 filesize = 0;
    char str[BUFFER_SIZE];
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }

    memset(str,0,sizeof(str));
    UBOOT_INFO("sizeof( board ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger("PanelRelativeSetting", "m_u8BOARD_PWM_PORT", 2);
    UBOOT_DEBUG("m_u8BOARD_PWM_PORT = %ld \n",n);
    p_data->m_u8BOARD_PWM_PORT = n;
    gstPWMSetting.m_u16PWMPort = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16BOARD_LVDS_CONNECT_TYPE", 0);
    UBOOT_DEBUG("m_u16BOARD_LVDS_CONNECT_TYPE = %ld \n",n);
    p_data->m_u16BOARD_LVDS_CONNECT_TYPE = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_PDP_10BIT", 0);
    UBOOT_DEBUG("m_bPANEL_PDP_10BIT = %ld \n",n);
    p_data->m_bPANEL_PDP_10BIT = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_SWAP_LVDS_POL", 0);
    UBOOT_DEBUG("m_bPANEL_SWAP_LVDS_POL = %ld \n",n);
    p_data->m_bPANEL_SWAP_LVDS_POL = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_SWAP_LVDS_CH", 0);
    UBOOT_DEBUG("m_bPANEL_SWAP_LVDS_CH = %ld \n",n);
    p_data->m_bPANEL_SWAP_LVDS_CH = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_bPANEL_CONNECTOR_SWAP_PORT", 0);
    UBOOT_DEBUG("m_bPANEL_CONNECTOR_SWAP_PORT = %ld \n",n);
    p_data->m_bPANEL_CONNECTOR_SWAP_PORT = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16LVDS_PN_SWAP_L", 0);
    UBOOT_DEBUG("m_u16LVDS_PN_SWAP_L = %ld \n",n);
    p_data->m_u16LVDS_PN_SWAP_L = n;

    n = Profile_GetInteger("PanelRelativeSetting", "m_u16LVDS_PN_SWAP_H", 0);
    UBOOT_DEBUG("m_u16LVDS_PN_SWAP_H = %ld \n",n);
    p_data->m_u16LVDS_PN_SWAP_H = n;

    // parsing [PanelDivisonGPIO_1]
    Profile_GetString("PanelDivisonGPIO_1", "PAD_NAME_1","", str, sizearray(str));
    UBOOT_DEBUG("PanelDivisonGPIO_1 = %s \n",str);
    strncpy(p_data->m_sGPIO1_PAD_NAME, str, sizeof(p_data->m_sGPIO1_PAD_NAME));

    n = Profile_GetInteger("PanelDivisonGPIO_1", "GPIO_INDEX_1", 0);
    UBOOT_DEBUG("GPIO_INDEX = %ld \n",n);
    p_data->m_u16GPIO1_INDEX = n;

    n = Profile_GetInteger("PanelDivisonGPIO_1", "VALUE_1", 0);
    UBOOT_DEBUG("VALUE = %ld \n",n);
    p_data->m_u8GPIO1_VALUE = n;

    // parsing [PanelDivisonGPIO_2]
    Profile_GetString("PanelDivisonGPIO_2", "PAD_NAME_2","", str, sizearray(str));
    UBOOT_DEBUG("PanelDivisonGPIO_2 = %s \n",str);
    strncpy(p_data->m_sGPIO2_PAD_NAME, str, sizeof(p_data->m_sGPIO2_PAD_NAME));

    n = Profile_GetInteger("PanelDivisonGPIO_2", "GPIO_INDEX_2", 0);
    UBOOT_DEBUG("GPIO_INDEX = %ld \n",n);
    p_data->m_u16GPIO2_INDEX = n;

    n = Profile_GetInteger("PanelDivisonGPIO_2", "VALUE_2", 0);
    UBOOT_DEBUG("VALUE = %ld \n",n);
    p_data->m_u8GPIO2_VALUE = n;

    if(script)
        free(script);
    return 0;
}

int Load_BoardSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_board_para stBoard;
    UBOOT_TRACE("IN\n");
    memset(&stBoard,0,sizeof(stBoard));
    vfs_mount(CONFIG);
    if(parse_board_ini(gBoardPath, &stBoard)!=0)
    {
        UBOOT_ERROR("parse_board_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("board para size =: 0x%08x \n",sizeof(stBoard));
    UBOOT_DEBUG("st_board_para.m_u8BOARD_PWM_PORT =: 0x%x \n",stBoard.m_u8BOARD_PWM_PORT);
    UBOOT_DEBUG("st_board_para.m_u16BOARD_LVDS_CONNECT_TYPE =: 0x%x \n",stBoard.m_u16BOARD_LVDS_CONNECT_TYPE);
    UBOOT_DEBUG("st_board_para.m_u8BOARD_PWM_PORT =: 0x%x \n",stBoard.m_u8BOARD_PWM_PORT);
    UBOOT_DEBUG("st_board_para.m_bPANEL_PDP_10BIT =: 0x%x \n",stBoard.m_bPANEL_PDP_10BIT);
    UBOOT_DEBUG("st_board_para.m_bPANEL_SWAP_LVDS_POL =: 0x%x \n",stBoard.m_bPANEL_SWAP_LVDS_POL);
    UBOOT_DEBUG("st_board_para.m_bPANEL_SWAP_LVDS_CH =: 0x%x \n",stBoard.m_bPANEL_SWAP_LVDS_CH);
    UBOOT_DEBUG("st_board_para.m_bPANEL_CONNECTOR_SWAP_PORT =: 0x%x \n",stBoard.m_bPANEL_CONNECTOR_SWAP_PORT);
    UBOOT_DEBUG("st_board_para.m_u16LVDS_PN_SWAP_L =: 0x%x \n",stBoard.m_u16LVDS_PN_SWAP_L);
    UBOOT_DEBUG("st_board_para.m_u16LVDS_PN_SWAP_H =: 0x%x \n",stBoard.m_u16LVDS_PN_SWAP_H);
    UBOOT_DEBUG("st_board_para.m_sGPIO1_PAD_NAME =: %s \n",stBoard.m_sGPIO1_PAD_NAME);
    UBOOT_DEBUG("st_board_para.m_u16GPIO1_INDEX =: 0x%x \n",stBoard.m_u16GPIO1_INDEX);
    UBOOT_DEBUG("st_board_para.m_u8GPIO1_VALUE =: 0x%x \n",stBoard.m_u8GPIO1_VALUE);
    UBOOT_DEBUG("st_board_para.m_sGPIO2_PAD_NAME =: %s \n",stBoard.m_sGPIO2_PAD_NAME);
    UBOOT_DEBUG("st_board_para.m_u16GPIO2_INDEX =: 0x%x \n",stBoard.m_u16GPIO2_INDEX);
    UBOOT_DEBUG("st_board_para.m_u8GPIO2_VALUE =: 0x%x \n",stBoard.m_u8GPIO2_VALUE);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_BOARD_PARA, (U32)&stBoard);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_BoardParaFromflash(st_board_para * board_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)board_data, E_DB_BOARD_PARA);
    return ret;
}

#endif


#if (ENABLE_ENABLE_URSA == 1)
#if (ENABLE_URSA_6M30 == 1)
int iniparser_6m30_cmd(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    char * script = NULL;
    char InPath[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    vfs_mount(CONFIG);
    snprintf(InPath,sizeof(InPath),"%s/6m30_mode.ini",CONFIG_PATH);
    script = loadscript(InPath,&filesize);
    if(script != NULL)
    {
        Profile_Init(script,filesize);
        cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_LVDS_CHANNEL_SWAP_MODE", 0);
        cmd_table->URSA_6M30_IN_LVDS_TIMODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_IN_LVDS_TIMODE", 0);
        cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM = Profile_GetInteger("MISC_6M30_CFG", "6M30_IN_LVDS_TI_BITNUM", 0);
        cmd_table->URSA_6M30_OUT_LVDS_TIMODE = Profile_GetInteger("MISC_6M30_CFG", "6M30_OUT_LVDS_TIMODE", 0);
        cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM = Profile_GetInteger("MISC_6M30_CFG", "6M30_OUT_LVDS_TI_BITNUM", 0);

        cmd_table->URSA_6M30_SSC_SEL = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_SEL", 0);
        cmd_table->URSA_6M30_SSC_ENABLE = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_ENABLE", 0);
        cmd_table->URSA_6M30_SSC_FREQ = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_FREQ", 0);
        cmd_table->URSA_6M30_SSC_PERCENT = Profile_GetInteger("MISC_6M30_CFG", "6M30_SSC_PERCENT", 0);

        UBOOT_DEBUG(" --->>> 6M30_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
        UBOOT_DEBUG(" --->>> 6M30_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_IN_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> 6M30_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM);
        UBOOT_DEBUG(" --->>> 6M30_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> 6M30_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM);

        UBOOT_DEBUG(" --->>> 6M30_SSC_SEL is %x \n",cmd_table->URSA_6M30_SSC_SEL);
        UBOOT_DEBUG(" --->>> 6M30_SSC_ENABLE is %x \n",cmd_table->URSA_6M30_SSC_ENABLE);
        UBOOT_DEBUG(" --->>> 6M30_SSC_FREQ is %x \n",cmd_table->URSA_6M30_SSC_FREQ);
        UBOOT_DEBUG(" --->>> 6M30_SSC_PERCENT is %x \n",cmd_table->URSA_6M30_SSC_PERCENT);
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("ubifsload 6m30.ini fail...>>>\n");
    }
    free(script);
    return ret;
}
int Read_Ursa_6m30_Para(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)cmd_table, E_DB_URSA_6M30);

    UBOOT_DEBUG(" cmd_table.6M30_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
    UBOOT_DEBUG(" cmd_table.6M30_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_IN_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.6M30_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM);
    UBOOT_DEBUG(" cmd_table.6M30_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.6M30_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM);

    UBOOT_DEBUG(" cmd_table.6M30_SSC_SEL is %x \n",cmd_table->URSA_6M30_SSC_SEL);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_ENABLE is %x \n",cmd_table->URSA_6M30_SSC_ENABLE);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_FREQ is %x \n",cmd_table->URSA_6M30_SSC_FREQ);
    UBOOT_DEBUG(" cmd_table.6M30_SSC_PERCENT is %x \n",cmd_table->URSA_6M30_SSC_PERCENT);
    return ret;
}
int Load_Ursa6m30Para_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ursa_6m30_cmd_table cmd_table= {0};
    UBOOT_TRACE("IN\n");
    if(iniparser_6m30_cmd(&cmd_table)==0)
    {
        UBOOT_DEBUG("6m30 para size =: 0x%08x \n",sizeof(cmd_table));
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_URSA_6M30].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_URSA_6M30].Offset);
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA_6M30, (U32)&cmd_table);
    }
    else
    {
        UBOOT_ERROR("parser_6m30_cmd fail ...>>>\n");
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

int Ursa_6M30_Setting(ursa_6m30_cmd_table * cmd_table)
{
    int ret = -1;
    MDrv_Ursa_6M30_ChanelSwap_Mode(cmd_table->URSA_6M30_LVDS_CHANNEL_SWAP_MODE);
    g_UrsaCMDGenSetting.g_InitInLvdsAll.bTIMode = cmd_table->URSA_6M30_IN_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_InitInLvdsAll.BitNums = cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bTIMode = cmd_table->URSA_6M30_OUT_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_6M30_OUT_LVDS_TI_BITNUM;

    MDrv_Ursa_6M30_Data_Init();

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_6M30_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_6M30_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_6M30_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_6M30_SSC_PERCENT;

    if(MDrv_Ursa_6M30_3D_Cmd(UC_SET_SSC_FREQ_PERCENT)>0)
    {
        ret = 0;
    }

    return ret;
}

#elif (ENABLE_URSA_8 == 1) || (ENABLE_URSA_6M40 == 1)

int parse_model_ini_ForUrsa(char* path, char* UrsaInIPath)
{
    int ret = -1;//
    char *script = NULL;
    char str[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            goto exit;
        }
    }
    UBOOT_INFO("sizeof( model ini script ) --------> %u bytes\n",filesize);

    Profile_Init((char *)script,filesize);
    if(Profile_GetString("Ursa", "m_pUrsaName","", str, sizearray(str)))
    {
        UBOOT_INFO("Half Ursa.ini path --> %s\n",str);
        snprintf(UrsaInIPath,BUFFER_SIZE,"%s",str);
        UBOOT_INFO("Full Ursa.ini  path --> %s\n",UrsaInIPath);
        ret = 0;
    }
    else
    {
        goto exit;
    }

exit:
    if(script)
        free(script);
    return ret;
}

int Parser_Ursa_Ini(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    char * script = NULL;
    char InPath[BUFFER_SIZE]= {0};
    char OutPath[BUFFER_SIZE]= {0};
    char UrsaBinName[BUFFER_SIZE]= {0};
    U32 filesize = 0;
    vfs_mount(CONFIG);
    memset(InPath,0,sizeof(InPath));
    snprintf(InPath,sizeof(InPath),"%s/sys.ini",CONFIG_PATH);
    ret = parse_sys_ini(InPath,OutPath);
    if(ret)
    {
        goto Ursa_exit;
    }

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    ret = parse_model_ini_ForUrsa(InPath,OutPath);
    if(ret)
    {
        goto Ursa_exit;
    }
    UBOOT_INFO("Full Ursa.ini path --> %s\n",OutPath);

    memset(InPath,0,sizeof(InPath));
    memcpy(InPath,OutPath,sizeof(OutPath));
    script = loadscript(InPath,&filesize);
    if(script != NULL)
    {
        Profile_Init(script,filesize);
        cmd_table->URSA_EXTERNAL = Profile_GetBoolean("URSA_PARA_CFG", "URSA_EXTERNAL", 1);
        cmd_table->URSA_MEMC_ENABLE = Profile_GetInteger("URSA_PARA_CFG", "URSA_MEMC_ENABLE", 0);
        cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_LVDS_CHANNEL_SWAP_MODE", 0);
        cmd_table->URSA_LVDS_SWAP_POL = Profile_GetInteger("URSA_PARA_CFG", "URSA_LVDS_SWAP_POL", 0);
        cmd_table->URSA_IN_LVDS_TIMODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_IN_LVDS_TIMODE", 0);
        cmd_table->URSA_IN_LVDS_TI_BITNUM = Profile_GetInteger("URSA_PARA_CFG", "URSA_IN_LVDS_TI_BITNUM", 0);
        cmd_table->URSA_OUT_LVDS_TIMODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_OUT_LVDS_TIMODE", 0);
        cmd_table->URSA_OUT_LVDS_TI_BITNUM = Profile_GetInteger("URSA_PARA_CFG", "URSA_OUT_LVDS_TI_BITNUM", 0);

        cmd_table->URSA_SSC_SEL = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_SEL", 0);
        cmd_table->URSA_SSC_ENABLE = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_ENABLE", 0);
        cmd_table->URSA_SSC_FREQ = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_FREQ", 0);
        cmd_table->URSA_SSC_PERCENT = Profile_GetInteger("URSA_PARA_CFG", "URSA_SSC_PERCENT", 0);
        cmd_table->URSA_OSD_PROTECT_MODE = Profile_GetInteger("URSA_PARA_CFG", "URSA_OSD_PROTECT_MODE", 0);

        UBOOT_DEBUG(" --->>> URSA_EXTERNAL is %x \n",cmd_table->URSA_EXTERNAL);
        UBOOT_DEBUG(" --->>> URSA_MEMC_ENABLE is %x \n",cmd_table->URSA_MEMC_ENABLE);
        UBOOT_DEBUG(" --->>> URSA_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
        UBOOT_DEBUG(" --->>> URSA_LVDS_SWAP_POL is %x \n",cmd_table->URSA_LVDS_SWAP_POL);
        UBOOT_DEBUG(" --->>> URSA_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_IN_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> URSA_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_IN_LVDS_TI_BITNUM);
        UBOOT_DEBUG(" --->>> URSA_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_OUT_LVDS_TIMODE);
        UBOOT_DEBUG(" --->>> URSA_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_OUT_LVDS_TI_BITNUM);

        UBOOT_DEBUG(" --->>> URSA_SSC_SEL is %x \n",cmd_table->URSA_SSC_SEL);
        UBOOT_DEBUG(" --->>> URSA_SSC_ENABLE is %x \n",cmd_table->URSA_SSC_ENABLE);
        UBOOT_DEBUG(" --->>> URSA_SSC_FREQ is %x \n",cmd_table->URSA_SSC_FREQ);
        UBOOT_DEBUG(" --->>> URSA_SSC_PERCENT is %x \n",cmd_table->URSA_SSC_PERCENT);
        UBOOT_DEBUG(" --->>> URSA_OSD_PROTECT_MODE is %x \n",cmd_table->URSA_OSD_PROTECT_MODE);

        if(cmd_table->URSA_EXTERNAL == FALSE)
        {
            if(Profile_GetString("URSA_FILE_CFG", "m_pUrsaBinName","", gstSysMiscSetting.m_Ursa_Bin_Name, sizearray(gstSysMiscSetting.m_Ursa_Bin_Name)))
            {
                UBOOT_INFO("Full Ursa Bin Name --> %s\n",UrsaBinName);
                ret = 0;
            }
            else
            {
                goto Ursa_exit;
            }
        }
        ret = 0;
    }
    else
    {
        UBOOT_ERROR("ubifsload 6m30.ini fail...>>>\n");
    }

Ursa_exit:
    if(script)
        free(script);
    return ret;
}
int Read_Ursa_Para(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    memset(cmd_table, 0, sizeof(ursa_cmd_table));
    ret = Read_FlashDataToAddr((U32)cmd_table, E_DB_URSA);

    UBOOT_DEBUG(" cmd_table.URSA_EXTERNAL is %x \n",cmd_table->URSA_EXTERNAL);
    UBOOT_DEBUG(" cmd_table.URSA_MEMC_ENABLE is %x \n",cmd_table->URSA_MEMC_ENABLE);
    UBOOT_DEBUG(" cmd_table.URSA_LVDS_CHANNEL_SWAP_MODE is %x \n",cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
    UBOOT_DEBUG(" cmd_table.URSA_LVDS_SWAP_POL is %x \n",cmd_table->URSA_LVDS_SWAP_POL);
    UBOOT_DEBUG(" cmd_table.URSA_IN_LVDS_TIMODE is %x \n",cmd_table->URSA_IN_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.URSA_IN_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_IN_LVDS_TI_BITNUM);
    UBOOT_DEBUG(" cmd_table.URSA_OUT_LVDS_TIMODE is %x \n",cmd_table->URSA_OUT_LVDS_TIMODE);
    UBOOT_DEBUG(" cmd_table.URSA_OUT_LVDS_TI_BITNUM is %x \n",cmd_table->URSA_OUT_LVDS_TI_BITNUM);

    UBOOT_DEBUG(" cmd_table.URSA_SSC_SEL is %x \n",cmd_table->URSA_SSC_SEL);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_ENABLE is %x \n",cmd_table->URSA_SSC_ENABLE);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_FREQ is %x \n",cmd_table->URSA_SSC_FREQ);
    UBOOT_DEBUG(" cmd_table.URSA_SSC_PERCENT is %x \n",cmd_table->URSA_SSC_PERCENT);

	UBOOT_DEBUG(" cmd_table.URSA_OSD_PROTECT_MODE is %x \n",cmd_table->URSA_OSD_PROTECT_MODE);
    return ret;
}
int Load_UrsaPara_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ursa_cmd_table cmd_table= {0};
    UBOOT_TRACE("IN\n");
    if(Parser_Ursa_Ini(&cmd_table)==0)
    {
        UBOOT_DEBUG("Ursa para size =: 0x%08x \n",sizeof(cmd_table));
        UBOOT_DEBUG("gstDbtable.dbdata[E_DB_URSA].Offset =: 0x%x\n",gstDbtable.dbdata[E_DB_URSA].Offset);

        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_URSA, (U32)&cmd_table);
    }
    else
    {
        UBOOT_ERROR("parser_cmd fail ...>>>\n");
    }
    UBOOT_TRACE("OK\n");
    return ret;
}

#if (ENABLE_URSA_8 == 1)
int Ursa_8_Setting(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    MDrv_Ursa_8_ChanelSwap_Mode(cmd_table->URSA_LVDS_CHANNEL_SWAP_MODE);
    //g_UrsaCMDGenSetting.g_InitInLvdsAll.bTIMode = cmd_table->URSA_6M30_IN_LVDS_TIMODE;
    //g_UrsaCMDGenSetting.g_InitInLvdsAll.BitNums = cmd_table->URSA_6M30_IN_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bLvdsSwapPol = cmd_table->URSA_LVDS_SWAP_POL;
    g_UrsaCMDGenSetting.g_OutLvdsMode.bTIMode = cmd_table->URSA_OUT_LVDS_TIMODE;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_OUT_LVDS_TI_BITNUM;

    g_UrsaCMDGenSetting.g_OsdMode.protect_mode = cmd_table->URSA_OSD_PROTECT_MODE;
    //MDrv_Ursa_6M30_Data_Init();
    udelay(50*1000);
    MDrv_Ursa_8_3D_Cmd ( UC_SET_OUT_LVDS_MODE);

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_SSC_PERCENT;
    udelay(50*1000);
    if(MDrv_Ursa_8_3D_Cmd(UC_SET_SSC_FREQ_PERCENT)>0)
    {
        ret = 0;
    }

    return ret;
}
#elif (ENABLE_URSA_6M40 == 1)
int Ursa_6M40_Syetting(ursa_cmd_table * cmd_table)
{
    int ret = -1;
    g_UrsaCMDGenSetting.g_OutLvdsMode.PanelBitNums = cmd_table->URSA_OUT_LVDS_TI_BITNUM;
    g_UrsaCMDGenSetting.g_OsdMode.protect_mode = cmd_table->URSA_OSD_PROTECT_MODE;
	UBOOT_INFO("g_UrsaCMDGenSetting.g_OsdMode.protect_mode=%d\n",g_UrsaCMDGenSetting.g_OsdMode.protect_mode);

    g_UrsaCMDGenSetting.g_SscFreqPercent.SscSel = cmd_table->URSA_SSC_SEL;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscEN = cmd_table->URSA_SSC_ENABLE;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscFreq = cmd_table->URSA_SSC_FREQ;
    g_UrsaCMDGenSetting.g_SscFreqPercent.SscPercent = cmd_table->URSA_SSC_PERCENT;

    return ret;
}
#endif

#endif

#endif

int Load_GopMuxToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_gopmux_para GopMux;
    UBOOT_TRACE("IN\n");
    memset(&GopMux,0,sizeof(GopMux));
    vfs_mount(CONFIG);

	snprintf(gGopMuxPath,sizeof(gGopMuxPath),"%s/dfbrc.ini",CONFIG_PATH);
	ret = parse_dfb_ini(gGopMuxPath, &GopMux);
	if(ret)
	{
		 UBOOT_ERROR(" parse_dfb_ini fail...\n");
		//goto exit;
	}

    UBOOT_DEBUG("GopMux para size =: 0x%08x \n",sizeof(GopMux));

    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PNL_GOPMUX, (U32)&GopMux);
    UBOOT_TRACE("OK\n");
    return ret;


}

int Read_Gop_Mux_ParaFromFlash(st_gopmux_para *gopmux_para)
{
    int ret = -1;
    memset(gopmux_para, 0, sizeof(st_gopmux_para));
    ret = Read_FlashDataToAddr((U32)gopmux_para, E_DB_PNL_GOPMUX);

    UBOOT_DEBUG("gopmux_para.m_u16SettingCounts=: 0x%x\n",gopmux_para->m_u16SettingCounts);
    UBOOT_DEBUG("gopmux_para.m_u16Setting0GopIndex=: 0x%x\n",gopmux_para->m_u16Setting0GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting1GopIndex=: 0x%x\n",gopmux_para->m_u16Setting1GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting2GopIndex=: 0x%x\n",gopmux_para->m_u16Setting2GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting3GopIndex=: 0x%x\n",gopmux_para->m_u16Setting3GopIndex);
    UBOOT_DEBUG("gopmux_para.m_u16Setting4GopIndex=: 0x%x\n",gopmux_para->m_u16Setting4GopIndex);
    return ret;
}

int Load_TConToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    unsigned int *pu32TConAddr=NULL;
    unsigned int u32fileSize   = 0;
    UBOOT_TRACE("IN\n");

    vfs_mount(CONFIG);
    u32fileSize=vfs_getsize(gTconBinPath);
    UBOOT_DEBUG("u32fileSize=0x%x;gstDbtable.dbdata[E_DB_TCON].Offset=0x%x\n",u32fileSize,gstDbtable.dbdata[E_DB_TCON].Offset);
    pu32TConAddr=malloc(u32fileSize);
    if(pu32TConAddr == NULL)
    {
        UBOOT_ERROR("malloc pu32TConAddr memory error !");
        return -1;
    }
    memset((void*)pu32TConAddr,0,gstDbtable.dbdata[E_DB_TCON].Size);
    if (vfs_read((void *)pu32TConAddr,gTconBinPath,0,u32fileSize) == 0)
    {
        ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_TCON, (U32)pu32TConAddr);
    }
    else
    {
        UBOOT_ERROR("vfs_read data fail ...>>>\n");
    }

    free(pu32TConAddr);
    pu32TConAddr = NULL;
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_TConFromFlash(U32 u32TConAddr)
{
    int ret = 0;
    ret = Read_FlashDataToAddr(u32TConAddr, E_DB_TCON);
    return ret;
}

int Load_PnlVb1ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PNL_VB1, (U32)&stVB1Channel1order);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PnlVb1FromFlash(pnl_VB1ChannelOrder * channelOrder_data)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    memset(channelOrder_data, 0, sizeof(pnl_VB1ChannelOrder));
    ret = Read_FlashDataToAddr((U32)channelOrder_data, E_DB_PNL_VB1);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 0             = 0x%x\n", channelOrder_data->u8Vb116vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 1             = 0x%x\n", channelOrder_data->u8Vb116vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 2             = 0x%x\n", channelOrder_data->u8Vb116vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 3             = 0x%x\n", channelOrder_data->u8Vb116vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 4             = 0x%x\n", channelOrder_data->u8Vb116vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 5             = 0x%x\n", channelOrder_data->u8Vb116vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 6             = 0x%x\n", channelOrder_data->u8Vb116vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb116vOrder 7             = 0x%x\n", channelOrder_data->u8Vb116vOrder[7]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 0             = 0x%x\n", channelOrder_data->u8Vb18vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 1             = 0x%x\n", channelOrder_data->u8Vb18vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 2             = 0x%x\n", channelOrder_data->u8Vb18vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 3             = 0x%x\n", channelOrder_data->u8Vb18vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 4             = 0x%x\n", channelOrder_data->u8Vb18vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 5             = 0x%x\n", channelOrder_data->u8Vb18vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 6             = 0x%x\n", channelOrder_data->u8Vb18vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 7             = 0x%x\n", channelOrder_data->u8Vb18vOrder[7]);
    UBOOT_DEBUG("channelOrder_data.u8Vb18vOrder 0             = 0x%x\n", channelOrder_data->u8Vb14vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 1             = 0x%x\n", channelOrder_data->u8Vb14vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 2             = 0x%x\n", channelOrder_data->u8Vb14vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 3             = 0x%x\n", channelOrder_data->u8Vb14vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 4             = 0x%x\n", channelOrder_data->u8Vb14vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 5             = 0x%x\n", channelOrder_data->u8Vb14vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 6             = 0x%x\n", channelOrder_data->u8Vb14vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb14vOrder 7             = 0x%x\n", channelOrder_data->u8Vb14vOrder[7]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 0             = 0x%x\n", channelOrder_data->u8Vb11vOrder[0]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 1             = 0x%x\n", channelOrder_data->u8Vb11vOrder[1]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 2             = 0x%x\n", channelOrder_data->u8Vb11vOrder[2]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 3             = 0x%x\n", channelOrder_data->u8Vb11vOrder[3]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 4             = 0x%x\n", channelOrder_data->u8Vb11vOrder[4]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 5             = 0x%x\n", channelOrder_data->u8Vb11vOrder[5]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 6             = 0x%x\n", channelOrder_data->u8Vb11vOrder[6]);
    UBOOT_DEBUG("channelOrder_data.u8Vb11vOrder 7             = 0x%x\n", channelOrder_data->u8Vb11vOrder[7]);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Load_MiscSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MISC, (U32)&gstSysMiscSetting);
    UBOOT_DEBUG("gstSysMiscSetting.m_u32MusicVol               = 0x%x\n", (unsigned int)gstSysMiscSetting.m_u32MusicVol);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8LogoGopIdx              = 0x%x\n", gstSysMiscSetting.m_u8LogoGopIdx);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16Panel_SwingLevel       = 0x%x\n", gstSysMiscSetting.m_u16Panel_SwingLevel);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8MirrorMode              = 0x%x\n", gstSysMiscSetting.m_u8MirrorMode);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16Panel_ext_type         = 0x%x\n", gstSysMiscSetting.m_u16Panel_ext_type);
    UBOOT_DEBUG("gstSysMiscSetting.m_Panel_LVDS_Connect_Type   = %s\n"  , gstSysMiscSetting.m_Panel_LVDS_Connect_Type);
    UBOOT_DEBUG("gstSysMiscSetting.m_u16PanelDCLK              = 0x%x\n", gstSysMiscSetting.m_u16PanelDCLK);
    UBOOT_DEBUG("gstSysMiscSetting.m_u32ursa_type              = 0x%x\n", (unsigned int)gstSysMiscSetting.m_u32ursa_type);
    UBOOT_DEBUG("gstSysMiscSetting.m_Ursa_Bin_Name             = %s\n"  , gstSysMiscSetting.m_Ursa_Bin_Name);
    UBOOT_DEBUG("gstSysMiscSetting.m_u8PixelShiftEnable        = 0x%x\n", gstSysMiscSetting.m_u8PixelShiftEnable);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MiscSetting_ToFlash(st_sys_misc_setting * misc_data)
{
    int ret = -1;
    memset(misc_data, 0, sizeof(st_sys_misc_setting));
    ret = Read_FlashDataToAddr((U32)misc_data, E_DB_MISC);
    UBOOT_DEBUG("misc_data->m_u32MusicVol               =: 0x%x\n", (unsigned int)misc_data->m_u32MusicVol);
    UBOOT_DEBUG("misc_data->m_u8LogoGopIdx              =: 0x%x\n", misc_data->m_u8LogoGopIdx);
    UBOOT_DEBUG("misc_data->m_u16Panel_SwingLevel       =: 0x%x\n", misc_data->m_u16Panel_SwingLevel);
    UBOOT_DEBUG("misc_data->m_u8MirrorMode              =: 0x%x\n", misc_data->m_u8MirrorMode);
    UBOOT_DEBUG("misc_data->m_u16Panel_ext_type         =: 0x%x\n", misc_data->m_u16Panel_ext_type);
    UBOOT_DEBUG("misc_data->m_Panel_LVDS_Connect_Type   =: %s\n"  , misc_data->m_Panel_LVDS_Connect_Type);
    UBOOT_DEBUG("misc_data->m_u16PanelDCLK              =: 0x%x\n", misc_data->m_u16PanelDCLK);
    UBOOT_DEBUG("misc_data->m_u32ursa_type              =: 0x%x\n", (unsigned int)misc_data->m_u32ursa_type);
    UBOOT_DEBUG("misc_data->m_Ursa_Bin_Name             =: %s\n"  , misc_data->m_Ursa_Bin_Name);
    UBOOT_DEBUG("misc_data.m_u8PixelShiftEnable         =: 0x%x\n", misc_data->m_u8PixelShiftEnable);
    return ret;
}

int Load_PWMSetting_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_PWM, (U32)&gstPWMSetting);
    UBOOT_DEBUG("gstPWMSetting.m_u32PWMPeriod              = 0x%x\n", (unsigned int)gstPWMSetting.m_u32PWMPeriod);
    UBOOT_DEBUG("gstPWMSetting.m_u16DivPWM                 = 0x%x\n", gstPWMSetting.m_u16DivPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u32PWMDuty                = 0x%x\n", (unsigned int)gstPWMSetting.m_u32PWMDuty);
    UBOOT_DEBUG("gstPWMSetting.m_bPolPWM                   = 0x%x\n", gstPWMSetting.m_bPolPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16maxPWM                 = 0x%x\n", gstPWMSetting.m_u16maxPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16minPWM                 = 0x%x\n", gstPWMSetting.m_u16minPWM);
    UBOOT_DEBUG("gstPWMSetting.m_u16PWMPort                = 0x%x\n", gstPWMSetting.m_u16PWMPort);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_PWMSetting_ToFlash(st_pwm_setting * pwm_data)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)pwm_data, E_DB_PWM);
    UBOOT_DEBUG("pwm_data->m_u32PWMPeriod              = 0x%x\n", (unsigned int)pwm_data->m_u32PWMPeriod);
    UBOOT_DEBUG("pwm_data->m_u16DivPWM                 = 0x%x\n", pwm_data->m_u16DivPWM);
    UBOOT_DEBUG("pwm_data->m_u32PWMDuty                = 0x%x\n", (unsigned int)pwm_data->m_u32PWMDuty);
    UBOOT_DEBUG("pwm_data->m_bPolPWM                   = 0x%x\n", pwm_data->m_bPolPWM);
    UBOOT_DEBUG("pwm_data->m_u16maxPWM                 = 0x%x\n", pwm_data->m_u16maxPWM);
    UBOOT_DEBUG("pwm_data->m_u16minPWM                 = 0x%x\n", pwm_data->m_u16minPWM);
    UBOOT_DEBUG("pwm_data->m_u16PWMPort                = 0x%x\n", pwm_data->m_u16PWMPort);
    return ret;
}

int Load_HDMITXToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    UBOOT_TRACE("IN\n");
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_HDMITX, (U32)&stHdmitxSetting);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_HDMITXFromFlash(st_hdmitx_setting *u32HDMITXAddr)
{
    int ret = 0;
    UBOOT_TRACE("IN\n");
    memset(u32HDMITXAddr, 0, sizeof(st_hdmitx_setting));
    ret = Read_FlashDataToAddr((U32)u32HDMITXAddr, E_DB_HDMITX);
    UBOOT_DEBUG("u32HDMITXAddr->enInColorFmt         = %d\n", u32HDMITXAddr->enInColorFmt);
    UBOOT_DEBUG("u32HDMITXAddr->enOutColorFmt        = %d\n", u32HDMITXAddr->enOutColorFmt);
    UBOOT_DEBUG("u32HDMITXAddr->enInColorQuantRange  = %d\n", u32HDMITXAddr->enInColorQuantRange);
    UBOOT_DEBUG("u32HDMITXAddr->enOutColorQuantRange = %d\n", u32HDMITXAddr->enOutColorQuantRange);
    UBOOT_DEBUG("u32HDMITXAddr->enOutputMode         = %d\n", u32HDMITXAddr->enOutputMode);
    UBOOT_DEBUG("u32HDMITXAddr->enColorDepth         = %d\n", u32HDMITXAddr->enColorDepth);
    UBOOT_DEBUG("u32HDMITXAddr->enLoadDefaultFromMboot  = %d\n", u32HDMITXAddr->enLoadDefaultFromMboot);

    // if there is no Hdmitx section in customer ini, load default value from mboot
    if(u32HDMITXAddr->enLoadDefaultFromMboot == 1)
    {
        ret = -1;
        UBOOT_TRACE("Load HDMI setting from mboot.\n");
    }

    UBOOT_TRACE("OK\n");
    return ret;
}
#if (CONFIG_DYNAMIC_IR)
static char gIRConfigPath[BUFFER_SIZE];

#define MIR_SECTION_NAME "Mboot"
#define PIR_SECTION_NAME "PM51"

static int parse_ir_bootcheck_ini(char *path, st_IRBootCheck_t *p_data)
{
    long n;
    char str[BUFFER_SIZE];
    char section_name[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;
    char i = 0;
    U16 *p = NULL;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ir_config ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    for(i = 0;i<IR_MAX;i++)
    {
        snprintf(str,sizeof(str),"%d",i);
        if(Profile_GetString(MIR_SECTION_NAME, str,"", section_name, sizearray(section_name)))
        {
            printf("section = %s \n",section_name);
            n = Profile_GetBoolean(section_name, "Enable", 1);
            UBOOT_DEBUG("Enable = %ld \n",n);
            if(n == FALSE)
                continue;
            n = Profile_GetInteger(section_name, "Protocol", 0);
            UBOOT_DEBUG("Protocol = %lx \n",n);
            (*p_data)[i].u8Protocol = n;
            n = Profile_GetInteger(section_name, "Header", 0);
            UBOOT_DEBUG("Header = %lx \n",n);
            (*p_data)[i].u32Headcode = n;

            p = &((*p_data)[i].u16Key[0]);
            n = Profile_GetInteger(section_name, "RECOVERY_KEY", 0xFFFF);
            UBOOT_DEBUG("RECOVERY_KEY = %lx \n",n);
            p[0]= n;

            n = Profile_GetInteger(section_name, "UPGRADEUSBOTA_KEY", 0xFFFF);
            UBOOT_DEBUG("UPGRADEUSBOTA_KEY = %lx \n",n);
            p[1] = n;

            n = Profile_GetInteger(section_name, "FORCEUGRADE_KEY", 0xFFFF);
            UBOOT_DEBUG("FORCEUGRADE_KEY = %lx \n",n);
            p[2]= n;

            n = Profile_GetInteger(section_name, "UPGRADEBOOTLOADER_KEY", 0xFFFF);
            UBOOT_DEBUG("UPGRADEBOOTLOADER_KEY = %lx \n",n);
            p[3] = n;

            n = Profile_GetInteger(section_name, "RECOVERYWIPEDATA_KEY", 0xFFFF);
            UBOOT_DEBUG("RECOVERYWIPEDATA_KEY = %lx \n",n);
            p[4] = n;

            n = Profile_GetInteger(section_name, "RECOVERYWIPECACHE_KEY", 0xFFFF);
            UBOOT_DEBUG("RECOVERYWIPECACHE_KEY = %lx \n",n);
            p[5]= n;

            n = Profile_GetInteger(section_name, "SECOND_SYSTEM_KEY", 0xFFFF);
            UBOOT_DEBUG("SECOND_SYSTEM_KEY = %lx \n",n);
            p[6] = n;

            n = Profile_GetInteger(section_name, "FASTBOOT_KEY", 0xFFFF);
            UBOOT_DEBUG("FASTBOOT_KEY = %lx \n",n);
            p[7] = n;

            n = Profile_GetInteger(section_name, "BRICK_TERMINATOR_RECOVERY_KEY", 0xFFFF);
            UBOOT_DEBUG("BRICK_TERMINATOR_RECOVERY_KEY = %lx \n",n);
            p[8] = n;
        }
        else
            continue;
    }

    if(script)
        free(script);
    return 0;

}
static int parse_ir_wakeup_ini(char *path, st_IRWakeup_t *p_data)
{
    long n;
    char str[BUFFER_SIZE];
    char section_name[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;
    char i = 0;
    U16 *p = NULL;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ir_config ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);
    for(i = 0;i<IR_MAX;i++)
    {
        snprintf(str,sizeof(str),"%d",i);
        if(Profile_GetString(PIR_SECTION_NAME, str,"", section_name, sizearray(section_name)))
        {
            printf("section = %s \n",section_name);
            n = Profile_GetInteger(section_name, "Protocol", 0);
            UBOOT_DEBUG("Protocol = %lx \n",n);
            (*p_data)[i].u8Protocol = n;
            n = Profile_GetInteger(section_name, "Header", 0);
            UBOOT_DEBUG("Header = %lx \n",n);
            (*p_data)[i].u32Headcode = n;
            n = Profile_GetInteger(section_name, "Key", 0xFFFF);
            UBOOT_DEBUG("WAKEUP_KEY = %lx \n",n);
            (*p_data)[i].u16Key = n;
        }
        else
            continue;
    }

    if(script)
        free(script);
    return 0;

}

int Load_IRConfig_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    st_IRBootCheck_t bootcheck_info;
    st_IRWakeup_t wakeup_info;
    UBOOT_TRACE("IN\n");
    memset(&bootcheck_info,0,sizeof(bootcheck_info));
    memset(&wakeup_info,0,sizeof(wakeup_info));
    vfs_mount(CONFIG);
    memset(gIRConfigPath,0,sizeof(gIRConfigPath));
    snprintf(gIRConfigPath,sizeof(gIRConfigPath),"%s/ir_config.ini",CONFIG_PATH);

    if(parse_ir_bootcheck_ini(gIRConfigPath, &bootcheck_info)!=0)
    {
        UBOOT_ERROR("parse_ir_bootcheck_ini fail !!\n");
        return ret;
    }

    if(parse_ir_wakeup_ini(gIRConfigPath, &wakeup_info)!=0)
    {
        UBOOT_ERROR("parse_ir_wakeup_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("bootcheck_info size =: 0x%08x \n",sizeof(bootcheck_info));
    UBOOT_DEBUG("wakeup_info size =: 0x%08x \n",sizeof(wakeup_info));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_IR_BOOTCHECK, (U32)&bootcheck_info);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_IR_WAKEUP, (U32)&wakeup_info);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_IRBootcheck_FromFlash(st_IRBootCheck_t * bootcheck_info)
{
    int ret = -1;
    unsigned char i = 0;
    if(bootcheck_info == NULL)
        return ret;
    ret = Read_FlashDataToAddr((U32)bootcheck_info, E_DB_IR_BOOTCHECK);
    for(i =0;i<IR_MAX;i++)
    {
        unsigned char j = 0;
        U16 * p;
        UBOOT_DEBUG("=====================================\n");
        UBOOT_DEBUG("IR%d :\n",i);
        UBOOT_DEBUG("u8Protocol = %x\n",(*bootcheck_info)[i].u8Protocol);
        UBOOT_DEBUG("u32Headcode = %x\n",(*bootcheck_info)[i].u32Headcode);
        p = &((*bootcheck_info)[i].u16Key[0]);
        for(j = 0;j<IR_KEY_MAX;j++)
        {
            UBOOT_DEBUG("key%d = %x\n",j,p[j]);
        }
    }
    return ret;
}

int Read_IRWakeup_FromFlash(st_IRWakeup_t * wakeup_info)
{
    int ret = -1;
    unsigned char i = 0;
    ret = Read_FlashDataToAddr((U32)wakeup_info, E_DB_IR_WAKEUP);
    for(i =0;i<IR_MAX;i++)
    {
        UBOOT_DEBUG("=====================================\n");
        UBOOT_DEBUG("IR%d :\n",i);
        UBOOT_DEBUG("u8Protocol = %x\n",(*wakeup_info)[i].u8Protocol);
        UBOOT_DEBUG("u32Headcode = %x\n",(*wakeup_info)[i].u32Headcode);
        UBOOT_DEBUG("u16Key = %x\n",(*wakeup_info)[i].u16Key);
    }
    return ret;
}

#endif
#if (CONFIG_LOCAL_DIMMING)
#include <drvMSPI.h>
#include <drvLDMA.h>
#define LDM_SECTION_NAME "begin"
static int parse_ldm_mspi_ini(char *path, ST_DRV_MSPI_INFO *p_data)
{
    long n;
    char str[BUFFER_SIZE];
    char *script = NULL;
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ldm ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8MspiChanel", 0);
    UBOOT_DEBUG("u8MspiChanel = %ld \n",n);
    p_data->u8MspiChanel = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8MspiMode", 0);
    UBOOT_DEBUG("u8MspiMode = %ld \n",n);
    p_data->u8MspiMode = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u32MspiClk", 0);
    UBOOT_DEBUG("u32MspiClk = %ld \n",n);
    p_data->u32MspiClk = n;

    if(Profile_GetString(LDM_SECTION_NAME, "u8WBitConfig[8]","", str, sizearray(str)))
    {
        _Vb1Str2Array(str, p_data->u8WBitConfig);
    }

    if(Profile_GetString(LDM_SECTION_NAME, "u8RBitConfig[8]","", str, sizearray(str)))
    {
        _Vb1Str2Array(str, p_data->u8RBitConfig);
    }

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TrStart", 0);
    UBOOT_DEBUG("u8TrStart = %ld \n",n);
    p_data->u8TrStart = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TrEnd", 0);
    UBOOT_DEBUG("u8TrEnd = %ld \n",n);
    p_data->u8TrEnd = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TB", 0);
    UBOOT_DEBUG("u8TB = %ld \n",n);
    p_data->u8TB = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8TRW", 0);
    UBOOT_DEBUG("u8TRW = %ld \n",n);
    p_data->u8TRW = n;

    if(script)
        free(script);
    return 0;
}

static int parse_ldm_dma_ini(char *path, ST_DRV_LD_DMA_INFO *p_data)
{
    long n;
    char *script = NULL;
    char str[BUFFER_SIZE];
    U32 filesize = 0;
    script = loadscript(path,&filesize);
    if(script == NULL)
    {
        while (*path== '/') path++;
        path=strstr(path,"/");
        UBOOT_INFO("filter /config, and find file again!!\n");
        script = loadscript(path,&filesize);
        if(script == NULL)
        {
            return -1;
        }
    }
    UBOOT_INFO("sizeof( ldm ini script ) --------> %u bytes\n",filesize);
    Profile_Init((char *)script,filesize);

    n = Profile_GetInteger(LDM_SECTION_NAME, "eLEDType", 0);
    UBOOT_DEBUG("eLEDType = %ld \n",n);
    p_data->eLEDType = n;

      n = Profile_GetInteger(LDM_SECTION_NAME, "u8LEDWidth", 0);
    UBOOT_DEBUG("u8LEDWidth = %ld \n",n);
    p_data->u8LEDWidth = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LEDHeight", 0);
    UBOOT_DEBUG("u8LEDHeight = %ld \n",n);
    p_data->u8LEDHeight = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDFWidth", 0);
    UBOOT_DEBUG("u8LDFWidth = %ld \n",n);
    p_data->u8LDFWidth = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDFHeight", 0);
    UBOOT_DEBUG("u8LDFHeight = %ld \n",n);
    p_data->u8LDFHeight = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LSFWidth", 0);
    UBOOT_DEBUG("u8LSFWidth = %ld \n",n);
    p_data->u8LSFWidth = n;
        n = Profile_GetInteger(LDM_SECTION_NAME, "u8LSFHeight", 0);
    UBOOT_DEBUG("u8LSFHeight = %ld \n",n);
    p_data->u8LSFHeight = n;


    n = Profile_GetInteger(LDM_SECTION_NAME, "bEdge2DEn", 0);
    UBOOT_DEBUG("bEdge2DEn = %ld \n",n);
    p_data->bEdge2DEn = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDMAchanel", 0);
    UBOOT_DEBUG("u8LDMAchanel = %ld \n",n);
    p_data->u8LDMAchanel = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDMATrimode", 0);
    UBOOT_DEBUG("u8LDMATrimode = %ld \n",n);
    p_data->u8LDMATrimode = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8LDMACheckSumMode", 0);
    UBOOT_DEBUG("u8LDMACheckSumMode = %ld \n",n);
    p_data->u8LDMACheckSumMode = n;

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8cmdlength", 0);
    UBOOT_DEBUG("u8cmdlength = %ld \n",n);
    p_data->u8cmdlength = n;

    if(Profile_GetString(LDM_SECTION_NAME, "u16MspiHead[8]","", str, sizearray(str)))
    {
        Str2Array(str, p_data->u16MspiHead);
    }

    n = Profile_GetInteger(LDM_SECTION_NAME, "u16LedNum", 0);
    UBOOT_DEBUG("u16LedNum = %ld \n",n);
    p_data->u16LedNum = n;

    if(Profile_GetString(LDM_SECTION_NAME, "u16DMADelay[4]","", str, sizearray(str)))
    {
        Str2Array(str, p_data->u16DMADelay);
    }

    n = Profile_GetInteger(LDM_SECTION_NAME, "u8ClkHz", 0);
    UBOOT_DEBUG("u8ClkHz = %ld \n",n);
    p_data->u8ClkHz = n;

    if(script)
        free(script);
    return 0;
}

int Load_LDMPara_ToFlash(U32 u32DbtableOffset)
{
    int ret = -1;
    ST_DRV_MSPI_INFO mspi_info;
    ST_DRV_LD_DMA_INFO dma_info;
    UBOOT_TRACE("IN\n");
    memset(&mspi_info,0,sizeof(mspi_info));
    vfs_mount(CONFIG);

    if(parse_ldm_mspi_ini(gLocalDimmingPath, &mspi_info)!=0)
    {
        UBOOT_ERROR("parse_ldm_mspi_ini fail !!\n");
        return ret;
    }

    if(parse_ldm_dma_ini(gLocalDimmingPath, &dma_info)!=0)
    {
        UBOOT_ERROR("parse_ldm_dma_ini fail !!\n");
        return ret;
    }

    UBOOT_DEBUG("mspi_info size =: 0x%08x \n",sizeof(mspi_info));
    UBOOT_DEBUG("dma_info size =: 0x%08x \n",sizeof(dma_info));
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_MSPI, (U32)&mspi_info);
    ret = Load_DataToDbBuffer(u32DbtableOffset, E_DB_DMA, (U32)&dma_info);
    UBOOT_TRACE("OK\n");
    return ret;
}

int Read_MSPIPara_FromFash(ST_DRV_MSPI_INFO * mspi_info)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)mspi_info, E_DB_MSPI);

    UBOOT_DEBUG("spi_info.u8MspiChanel=: 0x%x\n",mspi_info->u8MspiChanel);
    UBOOT_DEBUG("spi_info.u8MspiMode=: 0x%x\n",mspi_info->u8MspiMode);
    UBOOT_DEBUG("spi_info.u32MspiClk=: %ld\n",mspi_info->u32MspiClk);
    UBOOT_DEBUG("spi_info.u8WBitConfig=: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",mspi_info->u8WBitConfig[0],mspi_info->u8WBitConfig[1] \
                     ,mspi_info->u8WBitConfig[2],mspi_info->u8WBitConfig[3],mspi_info->u8WBitConfig[4],mspi_info->u8WBitConfig[5]
                     ,mspi_info->u8WBitConfig[6],mspi_info->u8WBitConfig[7]);
    UBOOT_DEBUG("spi_info.u8RBitConfig=: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",mspi_info->u8RBitConfig[0],mspi_info->u8RBitConfig[1] \
                    ,mspi_info->u8RBitConfig[2],mspi_info->u8RBitConfig[3],mspi_info->u8RBitConfig[4],mspi_info->u8RBitConfig[5]
                    ,mspi_info->u8RBitConfig[6],mspi_info->u8RBitConfig[7]);
    UBOOT_DEBUG("spi_info.u8TrStart=: 0x%x\n",mspi_info->u8TrStart);
    UBOOT_DEBUG("spi_info.u8TrEnd=: 0x%x\n",mspi_info->u8TrEnd);
    UBOOT_DEBUG("spi_info.u8TB=: 0x%x\n",mspi_info->u8TB);
    UBOOT_DEBUG("spi_info.u8TRW=: 0x%x\n",mspi_info->u8TRW);

    return ret;
}

int Read_DMAPara_FromFash(ST_DRV_LD_DMA_INFO * dma_info)
{
    int ret = -1;
    ret = Read_FlashDataToAddr((U32)dma_info, E_DB_DMA);

    UBOOT_DEBUG("dma_info.eLEDType=: 0x%x\n", dma_info->eLEDType);
    UBOOT_DEBUG("dma_info.u8LEDWidth=: %d\n", dma_info->u8LEDWidth);
    UBOOT_DEBUG("dma_info.u8LEDHeight=: %d\n", dma_info->u8LEDHeight);
    UBOOT_DEBUG("dma_info.u8LDFWidth=: %d\n", dma_info->u8LDFWidth);
    UBOOT_DEBUG("dma_info.u8LDFHeight=: %d\n", dma_info->u8LDFHeight);
    UBOOT_DEBUG("dma_info.u8LSFWidth=: %d\n", dma_info->u8LSFWidth);
    UBOOT_DEBUG("dma_info.u8LSFHeight=: %d\n", dma_info->u8LSFHeight);
    UBOOT_DEBUG("dma_info.bEdge2DEn=: 0x%x\n", dma_info->bEdge2DEn);

    UBOOT_DEBUG("dma_info.u8LDMAchanel=: 0x%x\n", dma_info->u8LDMAchanel);
    UBOOT_DEBUG("dma_info.u8LDMATrimode=: 0x%x\n", dma_info->u8LDMATrimode);
    UBOOT_DEBUG("dma_info.u8LDMACheckSumMode=: 0x%x\n", dma_info->u8LDMACheckSumMode);
    UBOOT_DEBUG("dma_info.u16DMADelay=: 0x%x, 0x%x, 0x%x, 0x%x\n", dma_info->u16DMADelay[0],dma_info->u16DMADelay[1]  \
                , dma_info->u16DMADelay[2],dma_info->u16DMADelay[3]);
    UBOOT_DEBUG("dma_info.u8cmdlength=: 0x%x\n", dma_info->u8cmdlength);
    UBOOT_DEBUG("dma_info.u16MspiHead=: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dma_info->u16MspiHead[0],dma_info->u16MspiHead[1] \
      , dma_info->u16MspiHead[2],dma_info->u16MspiHead[3], dma_info->u16MspiHead[4],dma_info->u16MspiHead[5]
      , dma_info->u16MspiHead[6],dma_info->u16MspiHead[7]);
    UBOOT_DEBUG("dma_info.u16LedNum=: 0x%x\n", dma_info->u16LedNum);
    UBOOT_DEBUG("dma_info.u8ClkHz=: %d\n", dma_info->u8ClkHz);
    return ret;
}
#endif
