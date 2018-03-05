#include "MsTypes.h"
#include "MsOS.h"
#include "sbootTypes.h"
#include "sbootBootLogo.h"
#include "sbootJPD.h"
#include "sbootXC.h"
#include "sbootGEGOP.h"
#include "string.h"
#include "stdio.h"

extern void LDR_PUTS(const char *s);

EN_OUTPUT_TIMING_TYPE _BootLogo_GetOutputTiming()
{
#if defined(CONFIG_HDMI_OUTPUT_TIMING_720X480_60I)
    return E_OUTPUT_TIMING_720X480_60I;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_720X480_60P)
    return E_OUTPUT_TIMING_720X480_60P;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_720X576_50I)
    return E_OUTPUT_TIMING_720X576_50I;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_720X576_50P)
    return E_OUTPUT_TIMING_720X576_50P;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_1280X720_50P)
    return E_OUTPUT_TIMING_1280X720_50P;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_1280X720_60P)
    return E_OUTPUT_TIMING_1280X720_60P;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_1920X1080_50I)
    return E_OUTPUT_TIMING_1920X1080_50I;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_1920X1080_60I)
    return E_OUTPUT_TIMING_1920X1080_60I;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_1920X1080_50P)
    return E_OUTPUT_TIMING_1920X1080_50P;
#elif defined(CONFIG_HDMI_OUTPUT_TIMING_1920X1080_60P)
    return E_OUTPUT_TIMING_1920X1080_60P;
#endif
}

EN_HDMI_COLOR_FORMAT _BootLogo_GetColorFormat()
{
#if defined(CONFIG_HDMI_COLOR_FORMAT_YUV)
    return E_HDMI_COLOR_FORMAT_YUV;
#elif defined(CONFIG_HDMI_COLOR_FORMAT_RGB)
    return E_HDMI_COLOR_FORMAT_RGB;
#endif

}

MS_BOOL do_bootlogo()
{
    LDR_PUTS("\nStart bootlogo\n");

    MS_BOOL bRet = FALSE;
    MS_U32 u32Addr = 0;
    RectInfo stRectInfoSrc = {0, 0};
    EN_OUTPUT_TIMING_TYPE enOutputTiming = _BootLogo_GetOutputTiming();
    EN_HDMI_COLOR_FORMAT enHdmiColorFormat = _BootLogo_GetColorFormat();

    Sboot_XC_Init(enOutputTiming);
    Sboot_VE_Init(enOutputTiming);

    if(enHdmiColorFormat == E_HDMI_COLOR_FORMAT_RGB)
    {
        Sboot_XC_HDMI_YUV2RGB();
    }

    bRet = Sboot_JPD_Decode(&u32Addr, &stRectInfoSrc);
    if(bRet)
    {
        Sboot_GEGOP_Init(u32Addr, stRectInfoSrc, enOutputTiming);
    }

    LDR_PUTS("\nEnd bootlogo\n");
    return bRet;
}
