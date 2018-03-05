#ifndef _MS_DRV_DeMura_H_
#define _MS_DRV_DeMura_H_


#define DBG_DEMURA                    1

#if (DBG_DEMURA == 1)
#define DEBUG_DEMURA(msg...)           UBOOT_DEBUG(msg)
#else
#define DEBUG_DEMURA(msg...)           
#endif

#define DEMURA_PACKET_SIZE(x)           (1 << (7 + x))
#define DEMURA_MMAP_ID                  "E_MMAP_ID_DEMURA"
#define DEMURA_LUT_TMP_LOAD_ADDR        CONFIG_KERNEL_START_ADDRESS
#define LAYER_DATA_ALIGNMENT            0x80

extern unsigned long MS_RIU_MAP;


#ifndef _HWREG_UTILITY_H_
#define _HWREG_UTILITY_H_

#define RIU_READ_BYTE(addr)         ( READ_BYTE( MS_RIU_MAP + (addr) ) )
#define RIU_READ_2BYTE(addr)        ( READ_WORD( MS_RIU_MAP + (addr) ) )
#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( MS_RIU_MAP + (addr), val) }
#define RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( MS_RIU_MAP + (addr), val) }

#define W2BYTE( u32Reg, u16Val)     RIU_WRITE_2BYTE( (u32Reg) << 1 , u16Val )
#define R2BYTE( u32Reg )            RIU_READ_2BYTE( (u32Reg) << 1)

#define W1BYTEMSK( u32Addr, u8Val, u8mask)     \
            ( { RIU_WRITE_BYTE( (((u32Addr) <<1) - ((u32Addr) & 1)), (RIU_READ_BYTE((((u32Addr) <<1) - ((u32Addr) & 1))) & ~(u8mask)) | ((u8Val) & (u8mask))); })

#define W2BYTEMSK( u32Reg, u16Val, u16Mask)\
            ( { RIU_WRITE_2BYTE( (u32Reg)<< 1 , (RIU_READ_2BYTE((u32Reg) << 1) & ~(u16Mask)) | ((u16Val) & (u16Mask)) ) ; } )

#endif


typedef struct
{
    // Offset: 0x00 ~ 0x03
    MS_U32 u32AllBinCRC;

    // Offset: 0x04 ~ 0x07
    MS_U32 u32HeaderCRC;

    // Offset: 0x08 ~ 0x0B
    MS_U32 u32HeaderSize;

    // Offset: 0x0C ~ 0x0F
    MS_U32 u32AllBinSize;

    // Offset: 0x10 ~ 0x1F
    MS_U8 u8DemuraID[16];

    // Offset: 0x20 ~ 0x21
    MS_U16 u16BinVersion;

    // Offset: 0x22
    MS_U8 u8LayerDataFomrat; // 1: IC Dram format
                          // 2: Compresses IC Dram format

    // Offset: 0x23
    MS_U8 bR_ch_Enable : 1;
    MS_U8 bG_ch_Enable : 1;
    MS_U8 bB_ch_Enable : 1;
    MS_U8 Reserved : 5;

    // Offset: 0x24 ~ 0x25
    MS_U16 u16HNodeCount;

    // Offset: 0x26 ~ 0x27
    MS_U16 u16VNodeCount;

    // Offset: 0x28
    MS_U8 u4StartLayer : 4;
    MS_U8 u4EndLayer   : 4;

    // Offset: 0x29
    MS_U8 bSeperateRGB : 1;
    MS_U8 dummy        : 7;

    // Offset: 0x2A ~ 0x2D
    MS_U32 u32LayerDataAddr;

    // Offset: 0x2E ~ 0x31:
    //  Layer data original (uncompressed) size in bytes.
    //  For layer data format 0 and 1, layer data original size is equal to layer data flash size.
    MS_U32 u32LayerDataOriSize;

    // Offset: 0x32 ~ 0x35:
    //  Size (in bytes) of layer data that stored on flash.
    //  If layer data format is 2 (compressed by 'gzip -9'), it keeps the size of compressed layer data.
    MS_U32 u32LayerDataFlashSize;

    // Offset: 0x36 ~ 0x39
    MS_U32 u32RegDataAddr;

    // Offset: 0x3A ~ 0x3D
    MS_U32 u32RegDataCount; // Count of register

    // Offset: 0x3E ~ 0x3F
    MS_U16 u16DLLVersion;

    // Offset: 0x40 ~ 0x43
    MS_U32 u32ProjectId;

    // Offset: 0x44 ~ 0x47 Customer data start address
    MS_U32 u32CustomerDataAddr;

    // Offset: 0x48 ~ 0x4B: Customer data size in bytes
    MS_U32 u32CustomerDataSize;

    MS_U32   nDate;           // 0x4C~0x4F
    MS_U8    nHBlockSize;     // 0x50
    MS_U8    nVBlockSize;     // 0x51
    MS_U8    nPacketSize;     // 0x52
}  __attribute__((__packed__)) DeMuraBinHeader;


typedef enum
{
    E_DEMURA_TOOL_FORMAT          = 0,  // tool format
    E_DEMURA_UNCOMPRESS_FORMAT    = 1,  // uncompress format
    E_DEMURA_COMPRESS_FORMAT      = 2,  // compress format
} DEMURA_LAYER_FORMAT;

typedef enum
{
    AUTO_DL_TRIG_MODE             = 0,  // Download One time
    AUTO_DL_ENABLE_MODE           = 1,  // Repeated download data
}AUTODOWNLOAD_TRIG_e;


MS_BOOL MDrv_DeMura_Check_AllBinCRC(MS_U32 pBinFileBuf);
MS_BOOL MDrv_DeMura_Check_HeaderCRC(DeMuraBinHeader *pBinHeader, MS_BOOL is_str_resume);
MS_BOOL MDrv_DeMura_Is_Support(DeMuraBinHeader *pBinHeader, PanelType* panel_data);
void MDrv_DeMura_Dump_BinInfo(DeMuraBinHeader *pBinHeader);
MS_BOOL MDrv_DeMura_LoadData(DeMuraBinHeader* pBinHeader, MS_U32 LutDestAddr, MS_U32 LutSrcAddr);
MS_BOOL MDrv_DeMura_RegisterTableLoad(DeMuraBinHeader* pBinHeader, MS_U32 RegDataBuf);
void MDrv_DeMura_AutoDownload_Setup(DeMuraBinHeader* pBinHeader, MS_U32 pLayerDataAddr);
void MDrv_DeMura_EnableDemura(MS_BOOL bEnable);
extern int zunzip(void *dst, int dstlen, unsigned char *src, unsigned long *lenp, int stoponerr, int offset);


#endif
