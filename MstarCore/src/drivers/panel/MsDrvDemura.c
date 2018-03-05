#include <MsCommon.h>
#include <command.h>
#include <MsDebug.h>
#include <apiPNL.h>
#include <linux/string.h>
#include "MsDrvDemura.h"
#include "halDemura.h"


static MS_U32 bIsLoadDeMuraBinDone = FALSE;


static MS_U32 crc32_table_4C11DB7[256]=
{
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9,
    0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
    0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
    0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
    0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
    0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,
    0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49,
    0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
    0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE,
    0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
    0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
    0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066,
    0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
    0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
    0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
    0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686,
    0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
    0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
    0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47,
    0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
    0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7,
    0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F,
    0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
    0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F,
    0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
    0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
    0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30,
    0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
    0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
    0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
    0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0,
    0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
    0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

#define CRC_WIDTH     (8 * sizeof(MS_U32))
#define POLYNOMIAL    0x4C11DB7


static MS_U32 MDrv_CRC32_Cal_Seed(const MS_U8* pu8DataBuf, MS_U32 u32DataSize, MS_U32 u32Seed)
{
    MS_U32 u32DataIdx;
    MS_U8 u8Data;
    MS_U32 remainder = u32Seed;

    // crcFast()
    // Divide the message by the polynomial, a byte at a time.
    for ( u32DataIdx = 0; u32DataIdx < u32DataSize; ++u32DataIdx)
    {
        u8Data = pu8DataBuf[u32DataIdx] ^ (remainder >> (CRC_WIDTH - 8));
        remainder = crc32_table_4C11DB7[u8Data] ^ (remainder << 8);
        //printf("\r\ni=%d-%x-%x", u32DataIdx,pu8DataBuf[u32DataIdx], remainder);
    }

    // The final remainder is the CRC.
    return (remainder);
}


static MS_U32 MDrv_CRC32_Cal_DeMura(const MS_U8* pu8DataBuf, MS_U32 u32DataSize)
{
    return MDrv_CRC32_Cal_Seed(pu8DataBuf, u32DataSize, 0xFFFFFFFF);
}


MS_BOOL MDrv_DeMura_Check_HeaderCRC(DeMuraBinHeader *pBinHeader, MS_BOOL is_str_resume)
{
    DEBUG_DEMURA("IN\n");
    MS_U32 u32CalHeaderCRC = MDrv_CRC32_Cal_DeMura((MS_U8*)pBinHeader + 8, sizeof(DeMuraBinHeader)-8);
    DEBUG_DEMURA("u32CalHeaderCRC = 0x%x\n", u32CalHeaderCRC);
    DEBUG_DEMURA("pBinHeader->u32HeaderCRC = 0x%x\n", pBinHeader->u32HeaderCRC);
    if(u32CalHeaderCRC != pBinHeader->u32HeaderCRC)
    {
        return FALSE;
    }
    if (is_str_resume == TRUE)
    {
        bIsLoadDeMuraBinDone = TRUE;
    }
    DEBUG_DEMURA("OK\n");
    return TRUE;
}


MS_BOOL MDrv_DeMura_Check_AllBinCRC(MS_U32 pBinFileBuf)
{
    MS_U32 u32CalAllBinCRC;
    DeMuraBinHeader *pBinHeader;

    DEBUG_DEMURA("IN\n");
    pBinHeader= (DeMuraBinHeader *)pBinFileBuf;
    u32CalAllBinCRC = MDrv_CRC32_Cal_DeMura((MS_U8 *)(pBinFileBuf+4), (pBinHeader->u32AllBinSize - 4));
    DEBUG_DEMURA("u32CalAllBinCRC = 0x%x\n", u32CalAllBinCRC);
    DEBUG_DEMURA("pBinHeader->u32AllBinCRC = 0x%x\n", pBinHeader->u32AllBinCRC);
    if(u32CalAllBinCRC != pBinHeader->u32AllBinCRC)
    {
        DEBUG_DEMURA("Error: Cal AllBin CRC fail\n");
        return FALSE;
    }
    DEBUG_DEMURA("OK\n");
    return TRUE;
}


MS_BOOL MDrv_DeMura_Is_Support(DeMuraBinHeader *pBinHeader, PanelType* panel_data)
{
    MS_U16 Demura_LayerCount;

    DEBUG_DEMURA("IN\n");
    if(strcmp((const char *)&(pBinHeader->u8DemuraID),"mstar demura") != 0)
    {
        DEBUG_DEMURA("Error, Demura ID compare fail.\n");
        DEBUG_DEMURA("Demura ID != 'mstar demura'\n");
        return FALSE;
    }
    if ( pBinHeader->u4StartLayer    > DEMURA_MAX_LAYER
         ||(pBinHeader->u4EndLayer   > DEMURA_MAX_LAYER)
         ||(pBinHeader->u4StartLayer > pBinHeader->u4EndLayer))
    {
        DEBUG_DEMURA("Error: Layer Counter Start(%d) and End(%d)  is invalid\n", pBinHeader->u4StartLayer
                     , pBinHeader->u4EndLayer);
        return FALSE;
    }
    Demura_LayerCount = (pBinHeader->u4EndLayer - pBinHeader->u4StartLayer + 1);
    DEBUG_DEMURA("Demura_LayerCount = 0x%x\n", Demura_LayerCount);

    if (Mhal_DeMura_Get_PanelWidth(pBinHeader) != panel_data->m_wPanelWidth)
    {
        DEBUG_DEMURA("panel_data->m_wPanelWidth = %d\n", panel_data->m_wPanelWidth);
        return FALSE;
    }
    if (Mhal_DeMura_Get_PanelHeight(pBinHeader) != panel_data->m_wPanelHeight)
    {
        DEBUG_DEMURA("panel_data->m_wPanelHeight = %d\n", panel_data->m_wPanelHeight);
        return FALSE;
    }

    DEBUG_DEMURA("OK\n");
    return TRUE;
}


void MDrv_DeMura_Dump_BinInfo(DeMuraBinHeader *pBinHeader)
{
    char *pdate;
    MS_U8 i;
    DEBUG_DEMURA("IN\n");

    DEBUG_DEMURA("AllBinCRC   = 0x%x\n",  pBinHeader->u32AllBinCRC);
    DEBUG_DEMURA("HeaderCRC   = 0x%x\n",  pBinHeader->u32HeaderCRC);
    DEBUG_DEMURA("HeaderSize  = 0x%x\n",  pBinHeader->u32HeaderSize);
    DEBUG_DEMURA("AllBinSize  = 0x%x\n",  pBinHeader->u32AllBinSize);
    #if (DBG_DEMURA == 1)
    printf("DemuraID = ");
    for(i = 0; i < sizeof(pBinHeader->u8DemuraID); i++)
        printf("%02x ", pBinHeader->u8DemuraID[i]);
    printf("\n");
    #endif
    
    DEBUG_DEMURA("BinVersion   = 0x%x\n",     pBinHeader->u16BinVersion);
    DEBUG_DEMURA("LayerDataFomrat = 0x%x\n",  pBinHeader->u8LayerDataFomrat);
    DEBUG_DEMURA("bR_ch_Enable = 0x%x\n",     pBinHeader->bR_ch_Enable);
    DEBUG_DEMURA("bG_ch_Enable = 0x%x\n",     pBinHeader->bG_ch_Enable);
    DEBUG_DEMURA("bB_ch_Enable = 0x%x\n",     pBinHeader->bB_ch_Enable);

    DEBUG_DEMURA("HNodeCount  = 0x%x\n",      pBinHeader->u16HNodeCount);
    DEBUG_DEMURA("VNodeCount  = 0x%x\n",      pBinHeader->u16VNodeCount);
    DEBUG_DEMURA("StartLayer  = 0x%x\n",      pBinHeader->u4StartLayer);
    DEBUG_DEMURA("EndLayer    = 0x%x\n",      pBinHeader->u4EndLayer);
    DEBUG_DEMURA("SeperateRGB = 0x%x\n",      pBinHeader->bSeperateRGB);

    DEBUG_DEMURA("LayerDataAddr    = 0x%x\n",    pBinHeader->u32LayerDataAddr);
    DEBUG_DEMURA("LayerDataOriSize = 0x%x\n",    pBinHeader->u32LayerDataOriSize);
    DEBUG_DEMURA("LayerDataFlashSize = 0x%x\n",  pBinHeader->u32LayerDataFlashSize);
    DEBUG_DEMURA("RegDataAddr      = 0x%x\n",    pBinHeader->u32RegDataAddr);
    DEBUG_DEMURA("RegDataCount     = 0x%x\n",    pBinHeader->u32RegDataCount);

    DEBUG_DEMURA("DLLVersion  = 0x%x\n",         pBinHeader->u16DLLVersion);
    DEBUG_DEMURA("ProjectId   = 0x%x\n",         pBinHeader->u32ProjectId);
    DEBUG_DEMURA("CustomerDataAddr = 0x%x\n",    pBinHeader->u32CustomerDataAddr);
    DEBUG_DEMURA("CustomerDataSize = 0x%x\n",    pBinHeader->u32CustomerDataSize);
    #if (DBG_DEMURA == 1)
    printf("Demura Bin Build date : ");
    pdate = (char *)&(pBinHeader->nDate);
    printf("20%02x/%02x/%02x %02x:00\n", pdate[0], pdate[1], pdate[2], pdate[3]);
    #endif

    DEBUG_DEMURA("nHBlockSize = 0x%x\n",  pBinHeader->nHBlockSize);
    DEBUG_DEMURA("nVBlockSize = 0x%x\n",  pBinHeader->nVBlockSize);
    DEBUG_DEMURA("nPacketSize = 0x%x\n",  pBinHeader->nPacketSize);

    DEBUG_DEMURA("OK\n\n");
}


MS_BOOL MDrv_DeMura_LoadData(DeMuraBinHeader* pBinHeader, MS_U32 LutDestAddr, MS_U32 LutSrcAddr)
{
    MS_U8 *src;
    MS_U32 dst, dstlen;
    unsigned long lenp = (~0UL);
    MS_U32 ret, org_len;
    MS_U32 u32CalCRC;
    DeMuraBinHeader *pNewBinHeader;

    DEBUG_DEMURA("IN\n");
    if (LutDestAddr == LutSrcAddr)
    {
        DEBUG_DEMURA("Error : LutDestAddr = LutSrcAddr, Can not move data !\n");
        return FALSE;
    }
    else if ((LAYER_DATA_ALIGNMENT < sizeof(DeMuraBinHeader))
              || ((LAYER_DATA_ALIGNMENT & (DEMURA_DMA_ADDR_UNIT -1)) != 0))
    {
        DEBUG_DEMURA("LAYER_DATA_ALIGNMENT(0x%x)  Error\n", LAYER_DATA_ALIGNMENT);
        return FALSE;
    }

    // For STR resuming
    //   Copy the header
    dst = LutDestAddr;
    memset((void *)dst, 0, LAYER_DATA_ALIGNMENT);
    memcpy((void *)dst, (void *)LutSrcAddr, sizeof(DeMuraBinHeader));
    pNewBinHeader = (DeMuraBinHeader *)LutDestAddr;
    pNewBinHeader->u32AllBinSize += (lenp - pBinHeader->u32LayerDataFlashSize);

    // Load layer data
    dst = dst + LAYER_DATA_ALIGNMENT;
    pNewBinHeader->u8LayerDataFomrat = E_DEMURA_UNCOMPRESS_FORMAT;
    pNewBinHeader->u32LayerDataAddr  = LAYER_DATA_ALIGNMENT;

    if (pBinHeader->u8LayerDataFomrat == E_DEMURA_COMPRESS_FORMAT)
    {
        if (LutDestAddr > LutSrcAddr)
        {
            if ((LutDestAddr - LutSrcAddr) < pBinHeader->u32AllBinSize)
            {
                DEBUG_DEMURA("For Compress data, dest(0x%x) - src(0x%x) < AllBinSize(0x%x)\n", 
                                LutDestAddr, LutSrcAddr, pBinHeader->u32AllBinSize);
                return FALSE;
            }
        } else if (LutDestAddr < LutSrcAddr)
        {
            if ((LutSrcAddr - LutDestAddr) < pBinHeader->u32AllBinSize)
            {
                DEBUG_DEMURA("For Compress data, src(0x%x) - dest(0x%x) < AllBinSize(0x%x)\n", 
                                LutDestAddr, LutSrcAddr, pBinHeader->u32AllBinSize);
                return FALSE;
            }
        }

        dstlen = pBinHeader->u32LayerDataFlashSize - 6;    // 4byte  original filesize, 2byte magic value('0xBE', 0x'EF')
        src  = (MS_U8 *)(LutSrcAddr + pBinHeader->u32LayerDataAddr);
        org_len = *(MS_U32 *)(src + dstlen);

        DEBUG_DEMURA("dst = 0x%x, dstlen = 0x%x, src = 0x%x\n", dst, dstlen, src);
        ret = zunzip((void *)dst, dstlen, src, &lenp, 1, 0);
        if (ret != 0)
        {
            DEBUG_DEMURA("zunzip failed!\n");
            return FALSE;
        }
        if ((org_len != lenp) || (lenp != pBinHeader->u32LayerDataOriSize))
        {
            DEBUG_DEMURA("origal_len = 0x%x, lenp = 0x%x\n", org_len, lenp);
            DEBUG_DEMURA("zunzip data abort\n");
            return FALSE;
        }
        DEBUG_DEMURA("zunzip done, lenp = 0x%x\n", lenp);
    } 
    else if (pBinHeader->u8LayerDataFomrat == E_DEMURA_UNCOMPRESS_FORMAT)
    {
        memcpy((void *)dst, (void *)(LutSrcAddr + pBinHeader->u32LayerDataAddr), pBinHeader->u32LayerDataOriSize);
    }

    //   Copy the register data
    dst = dst +  pBinHeader->u32LayerDataOriSize;
    memset((void *)dst, 0, LAYER_DATA_ALIGNMENT);    // Keep a distance between register_data and layer data
    dst = dst + LAYER_DATA_ALIGNMENT;
    memcpy((void *)dst, (void *)(LutSrcAddr + pBinHeader->u32RegDataAddr), (pBinHeader->u32RegDataCount * 5));
    pNewBinHeader->u32RegDataAddr = dst - (MS_U32)LutDestAddr;

    //   Copy the Customer data
    if (pBinHeader->u32CustomerDataAddr != 0x00)
    {
        dst = dst + pBinHeader->u32RegDataCount * 5;
        memcpy((void *)dst, (void *)(LutSrcAddr + pBinHeader->u32CustomerDataAddr)
               , pBinHeader->u32CustomerDataSize);
        pNewBinHeader->u32CustomerDataAddr  = dst - (MS_U32)LutDestAddr;
    }

    //   Re generate the CRC of header
    u32CalCRC = MDrv_CRC32_Cal_DeMura((MS_U8*)pNewBinHeader + 8, sizeof(DeMuraBinHeader)-8);
    pNewBinHeader->u32HeaderCRC = u32CalCRC;
    memcpy((void *)pBinHeader, (void *)pNewBinHeader, sizeof(DeMuraBinHeader));  // Copy back the New Header
    bIsLoadDeMuraBinDone = TRUE;

    DEBUG_DEMURA("OK\n");
    return TRUE;
}


MS_BOOL MDrv_DeMura_RegisterTableLoad(DeMuraBinHeader* pBinHeader, MS_U32 RegDataBuf)
{
    MS_U32 u32Counter;
    MS_U32 u32RegisterAddress;
    MS_U8  u8Value;
    MS_U8  u8Mask;
    MS_U8 *pu8RegDataBuf = (MS_U8 *)RegDataBuf;

    DEBUG_DEMURA("IN\n");
    if (bIsLoadDeMuraBinDone != TRUE)
    {
        DEBUG_DEMURA("DeMura Bin is not ready !\n");
        return FALSE;
    }
    if((pBinHeader->u32RegDataCount == 0) || (pBinHeader->u32RegDataCount > 256))
    {
        DEBUG_DEMURA("Error: Demura Register Counter = %d, No register value needs to be loaded.\n", pBinHeader->u32RegDataCount);
        return FALSE;
    }
    MHal_PNL_Switch_DeMura_SubBank(REG_DEMURA_SUB_BANK_NUM);

    UBOOT_DUMP((unsigned int)pu8RegDataBuf, (pBinHeader->u32RegDataCount * 5));
    for (u32Counter = 0; u32Counter < pBinHeader->u32RegDataCount; u32Counter++)
    {
        // Littele Endian
        u32RegisterAddress = (pu8RegDataBuf[0]) + DEMURA_MAIN_BANK_BASE;
        u8Value = (pu8RegDataBuf[3]);
        u8Mask = (pu8RegDataBuf[4]);
        
        W1BYTEMSK(u32RegisterAddress, u8Value, u8Mask);
        pu8RegDataBuf += 5;
    }
    DEBUG_DEMURA("OK\n");
    return TRUE;
}


void MDrv_DeMura_AutoDownload_Setup(DeMuraBinHeader* pBinHeader,  MS_U32 pLayerDataAddr)
{
    DEBUG_DEMURA("IN\n");
    if (bIsLoadDeMuraBinDone != TRUE)
    {
        DEBUG_DEMURA("DeMura Bin is not ready !\n");
        return ;
    }
    MHal_PNL_Switch_DeMura_SubBank(REG_DEMURA_DMA_BANK_NUM);

    MHal_DeMura_SetDL_BaseAddr(pLayerDataAddr);
    MHal_DeMura_SetDL_SramIni_Addr(0);

    MHal_DeMura_SetDL_Depth(pBinHeader->u16HNodeCount);
    MHal_DeMura_SetDL_ReqLenth(DEMURA_DMA_REQ_LENGTH);
    MHal_DeMura_SetDL_TriggerMode(AUTO_DL_ENABLE_MODE);
    MHal_DeMura_Enable_DL(TRUE);
    DEBUG_DEMURA("OK\n");
}


void MDrv_DeMura_EnableDemura(MS_BOOL bEnable)
{
    DEBUG_DEMURA("IN\n");
    if (bIsLoadDeMuraBinDone != TRUE)
    {
        DEBUG_DEMURA("DeMura Bin is not ready !\n");
        return ;
    }
    if (MHal_PNL_Read_DeMura_SubBank() != REG_DEMURA_SUB_BANK_NUM)
    {
        MHal_PNL_Switch_DeMura_SubBank(REG_DEMURA_SUB_BANK_NUM);
    }
    MHal_DeMura_EnableDemura(bEnable);
    DEBUG_DEMURA("OK\n");
}


