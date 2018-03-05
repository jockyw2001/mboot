#include <MsCommon.h>
#include <command.h>
#include <MsDebug.h>
#include <apiPNL.h>
#include <linux/string.h>
#include <MsDrvDemura.h>
#include "halDemura.h"


MS_U16 Mhal_DeMura_Get_PanelWidth(DeMuraBinHeader *pBinHeader)
{
    MS_U16 panel_width;
    MS_U16 packet_notes;

    if ((pBinHeader->nPacketSize > 1) || (pBinHeader->nPacketSize < 0))
    {
        DEBUG_DEMURA("Do not support nPacketSize = %d\n", pBinHeader->nPacketSize);
        return 0;
    }

    if (pBinHeader->u4EndLayer > 4)
    {
        packet_notes = 1;
        if (pBinHeader->nPacketSize == 0)
        {
            DEBUG_DEMURA("This PacketSize(%d) can not contain Layers(%d)\n",
                         DEMURA_PACKET_SIZE(pBinHeader->nPacketSize), pBinHeader->u4EndLayer - pBinHeader->u4StartLayer);
            return 0;
        }
    }
    else if (pBinHeader->u4EndLayer > 2)
    {
        packet_notes = (2 >> (1 - pBinHeader->nPacketSize));
    }
    else
    {
        packet_notes = (4 >> (1 - pBinHeader->nPacketSize));
    }

    panel_width = packet_notes * (1 << (pBinHeader->nHBlockSize)) * (pBinHeader->u16HNodeCount - 1);
    if (pBinHeader->bSeperateRGB == 0)
    {
        // Mono mode: R/G/B channel used the same table
        panel_width = panel_width * 4;
    }

    DEBUG_DEMURA("The panel_width in LUT is %d\n", panel_width);
    return panel_width;
}


void MHal_PNL_Switch_DeMura_SubBank(MS_U16 u16Bank)
{
    W2BYTE(L_BK_DEMURA(0x00), u16Bank);
}

MS_U16 MHal_PNL_Read_DeMura_SubBank(void)
{
    return (MS_U16)R2BYTE(L_BK_DEMURA(0x00));
}


MS_U16 Mhal_DeMura_Get_PanelHeight(DeMuraBinHeader *pBinHeader)
{
    MS_U16 panel_height;
    panel_height = (1 << (pBinHeader->nVBlockSize)) * (pBinHeader->u16VNodeCount - 1);
    DEBUG_DEMURA("The panel_height in LUT is %d\n", panel_height);
    return panel_height;
}


void MHal_DeMura_SetDL_BaseAddr(MS_U32 u32Addr)
{
    MS_U32 base_addr = u32Addr / DEMURA_DMA_ADDR_UNIT;
    W2BYTE(L_BK_DEMURA_DMA(0x06), (MS_U16)(base_addr & 0xFFFF));
    W2BYTEMSK(L_BK_DEMURA_DMA(0x07), (base_addr >> 16), 0x07FF);
}


void MHal_DeMura_SetDL_SramIni_Addr(MS_U16 offset)
{
    W2BYTE(L_BK_DEMURA_DMA(0x10), offset);
}


void MHal_DeMura_SetDL_Depth(MS_U16 HNodeCount)
{
    W2BYTE(L_BK_DEMURA_DMA(0x0A), HNodeCount);
}


void MHal_DeMura_SetDL_ReqLenth(MS_U16 ReqLenth)
{
    W2BYTE(L_BK_DEMURA_DMA(0x0D), ReqLenth);
}

void MHal_DeMura_SetDL_TriggerMode(AUTODOWNLOAD_TRIG_e eTrigMode)
{
    W2BYTEMSK((L_BK_DEMURA_DMA(0x01)), (eTrigMode << 5), BIT(5));
}

void MHal_DeMura_Enable_DL(MS_BOOL bEnable)
{
    W2BYTEMSK((L_BK_DEMURA_DMA(0x01)), (bEnable << 2), BIT(2));
}

void MHal_DeMura_EnableDemura(MS_BOOL bEnable)
{
    if (bEnable == TRUE)
    {
        W2BYTEMSK((L_BK_DEMURA(0x2F)),  0x07, BIT(2)|BIT(1)|BIT(0));
    }
    else
    {
        W2BYTEMSK((L_BK_DEMURA(0x2F)),  0x00, BIT(2)|BIT(1)|BIT(0));
    }
}

