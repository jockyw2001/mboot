#include "MsTypes.h"
#include "MsOS.h"
#include "apiJPEG.h"
#include "sbootTypes.h"
#include "sbootJPD.h"

extern MS_U8 _ld_LOGO_load_start[];

extern void LDR_PUTDW(MS_U32 val);
extern void LDR_PUTS(const char *s);
extern MS_BOOL NAND_Read_LogoData(MS_U8 *u8_buf, MS_U32 u32NandOffset);

#define JPD_INTER_BUF_ADR CONFIG_JPD_INTER_ADDR
#define JPD_INTER_BUF_LEN CONFIG_JPD_INTER_SIZE
#define JPD_READ_BUF_ADR CONFIG_JPD_READ_ADDR
#define JPD_READ_BUF_LEN CONFIG_JPD_READ_SIZE
#define JPD_OUT_ADR CONFIG_JPD_OUT_ADDR
#define JPD_OUT_LEN CONFIG_JPD_OUT_SIZE

#if defined CONFIG_SBOOT_BOOT_LOGO_STORED_IN_SPI
#define VA_INPUT_DATA_ADDR CONFIG_LOGO_START_ADDRESS_IN_DRAM + CONFIG_LOGO_HEADER_SIZE
#elif defined CONFIG_SBOOT_BOOT_LOGO_STORED_IN_NAND
#define VA_INPUT_DATA_ADDR CONFIG_LOGO_START_ADDRESS_IN_DRAM + CONFIG_LOGO_HEADER_SIZE
#define LOGO_DATA_IN_NAND_FLASH CONFIG_LOGO_START_ADDRESS_IN_NAND
#elif defined CONFIG_SBOOT_BOOT_LOGO_STORED_IN_HASH2
#define VA_INPUT_DATA_ADDR NULL
    volatile MS_U8 _gu8BaseLineJPEG[] = {
        #include "data/colorbar_400x400_jpg.dat"
    };
#endif

#define JPD_BASE_WIDTH_MAX 1600
#define JPD_BASE_HEIGHT_MAX 1200

static EN_JPD_DECODE_STATE _geJpdDecodeState = E_JPD_IDLE;
static MS_U32 _gu32ReadBuffVirAddr = 0;
static MS_U8 *_gpInputDataAddr = (MS_U8 *)(VA_INPUT_DATA_ADDR);

static MS_U32 _gu32InputDataLength = 0;
static MS_U32 _gu32ReadPosition = 0;
static MS_S32 _gs32InitByteRead = 0;
static MS_BOOL _gu8EOF_flag = FALSE;

static MS_U16 _gu16JpdWidth = 0;
static MS_U16 _gu16JpdHeight = 0;
static MS_U16 _gu16JpdPitch = 0;

static MS_BOOL _Sboot_JPD_Init()
{
    MS_BOOL bRet = FALSE;

    _geJpdDecodeState = E_JPD_INIT;
    _gs32InitByteRead = 0;
    _gu8EOF_flag = FALSE;

    _gu32ReadBuffVirAddr = MS_PA2KSEG1(JPD_READ_BUF_ADR);

// Get Logo data & size
#if defined (CONFIG_SBOOT_BOOT_LOGO_STORED_IN_HASH2)
    _gpInputDataAddr = (MS_U8 *)_gu8BaseLineJPEG;
    _gu32InputDataLength = sizeof(_gu8BaseLineJPEG);
#elif defined (CONFIG_SBOOT_BOOT_LOGO_STORED_IN_SPI)
    MS_U32 u32LogoAddr = CONFIG_LOGO_START_ADDRESS_IN_SPI;
    MS_U32 u32LogoHeaderSize = CONFIG_LOGO_HEADER_SIZE;
    MS_U32 i = 0;

    _gu32InputDataLength = (*((unsigned int *)(u32LogoAddr)));
    u32LogoAddr += u32LogoHeaderSize;

    if(_gu32InputDataLength > CONFIG_LOGO_SIZE)
    {
        LDR_PUTS("Load LOGO in SPI FAIL\n");
        return bRet;
    }

    for(i = 0; i < ((_gu32InputDataLength)); i ++)
    {
        *((unsigned char *)(_gpInputDataAddr + i)) = *((unsigned char *)((u32LogoAddr) + i));
    }
#elif defined (CONFIG_SBOOT_BOOT_LOGO_STORED_IN_NAND)
    bRet = NAND_Read_LogoData((MS_U8*)CONFIG_LOGO_START_ADDRESS_IN_DRAM, LOGO_DATA_IN_NAND_FLASH);
    if( bRet == FALSE )
    {
        LDR_PUTS("Load LOGO in NAND FAIL\n");
        return bRet;
    }
    _gu32InputDataLength = (*((unsigned int *)(CONFIG_LOGO_START_ADDRESS_IN_DRAM)));
#endif

    LDR_PUTS("Logo Addr in DRAM = 0x");
    LDR_PUTDW((MS_U32)_gpInputDataAddr);
    LDR_PUTS("\n");
    LDR_PUTS("Logo size = 0x");
    LDR_PUTDW((MS_U32)_gu32InputDataLength);
    LDR_PUTS("\n");

    _gu32ReadPosition = 0;
    MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_NONE);

    bRet = TRUE;
    return bRet;

}

static MS_S32 _Sboot_JPD_FileRead(MS_U8 *pbuf, MS_U32 u32MaxBytesToRead, MS_BOOL *pEOF_flag)
{
    MS_U8* u8RequestDataAddr = 0;
    MS_S32 s32BytesRead = 0;
    MS_U32 i = 0;

    u8RequestDataAddr = pbuf;
    if (_gu32ReadPosition <= _gu32InputDataLength)
    {
        s32BytesRead = u32MaxBytesToRead;
        if ((_gu32ReadPosition + u32MaxBytesToRead) >= _gu32InputDataLength)
        {
            s32BytesRead = _gu32InputDataLength - _gu32ReadPosition;
            *pEOF_flag = TRUE;
        }

        for(i = 0; i < s32BytesRead; i++)
        {
            u8RequestDataAddr[i] = _gpInputDataAddr[_gu32ReadPosition + i];
        }
        //MsOS_FlushMemory();
    }
    else
    {
        *pEOF_flag = TRUE;
    }
    _gu32ReadPosition += s32BytesRead;
    return s32BytesRead;
}

static MS_S32 _Sboot_JPD_FillHdrFunc(MS_PHYADDR BufAddr, MS_U32 BufLength)
{
    MS_S32 s32BytesRead = 0;
    MS_U32 buf_left = 0;
    MS_U32 u32BufAddr = (MS_U32) BufAddr;
    MS_U32 bufVirAddr = MS_PA2KSEG1(u32BufAddr);

    LDR_PUTS("  u32BufAddr = ");
    LDR_PUTDW(u32BufAddr);
    LDR_PUTS("  bufVirAddr = ");
    LDR_PUTDW(bufVirAddr);
    LDR_PUTS("\n");

    do
    {
        s32BytesRead = _Sboot_JPD_FileRead((MS_U8 *)(bufVirAddr + buf_left), BufLength - buf_left, &_gu8EOF_flag);

        if (s32BytesRead < 0)
        {
            return s32BytesRead;
        }

        buf_left += s32BytesRead;
    } while(( buf_left < BufLength ) && ( !_gu8EOF_flag ));

    MApi_JPEG_UpdateReadInfo(buf_left, _gu8EOF_flag);

    return buf_left;
}

static MS_BOOL _Sboot_JPD_PreLoadBuffer(JPEG_BuffLoadType u8MRBuffType)
{
    MS_S32 bytes_read = 0;
    MS_U32 buf_left = 0;
    MS_U32 u32RequestDataAddr, u32RequestDataSize;
    //LDR_PUTS("IN\n");

    if(_gu8EOF_flag)
    {
        MApi_JPEG_ProcessEOF(u8MRBuffType);
        return TRUE;
    }

    u32RequestDataAddr = _gu32ReadBuffVirAddr;
    u32RequestDataSize = JPD_READ_BUF_LEN/2;

    if(u8MRBuffType == E_JPEG_BUFFER_HIGH)
    {
        u32RequestDataAddr += u32RequestDataSize;
    }

    do
    {
        bytes_read = _Sboot_JPD_FileRead((MS_U8 *)(u32RequestDataAddr + buf_left), u32RequestDataSize - buf_left, &_gu8EOF_flag);

        if (bytes_read == -1)
        {
            MApi_JPEG_SetErrCode( E_JPEG_STREAM_READ );
            return FALSE;
        }

        buf_left += bytes_read;
    } while(( buf_left < u32RequestDataSize ) && ( !_gu8EOF_flag ));

    MApi_JPEG_UpdateReadInfo(buf_left, _gu8EOF_flag);
    if(buf_left > 0)
    {
        MApi_JPEG_SetMRBufferValid(u8MRBuffType);
    }
    //LDR_PUTS("OK\n");
    return TRUE;
}


static EN_JPEG_DECODE_STATUS _Sboot_JPD_WaitDone(void)
{
    EN_JPEG_DECODE_STATUS eDecodeStatus = E_DECODING;
    //LDR_PUTS("IN\n");
    switch(MApi_JPEG_WaitDone())
    {
        case E_JPEG_DEC_DONE:
            //LDR_PUTS("ApiWaitDone_state: E_JPEG_DEC_DONE\n");
            eDecodeStatus = E_DECODE_DONE;
            break;
        case E_JPEG_DEC_FAILED:
        case E_JPEG_DEC_RST_ERROR:
        case E_JPEG_DEC_BITSTREAM_ERROR:
            //LDR_PUTS("ApiWaitDone_state: E_JPEG_DEC_FAILED or E_JPEG_DEC_RST_ERROR or E_JPEG_DEC_BITSTREAM_ERROR\n");
            //LDR_PUTS("Baseline decode error\n");
            eDecodeStatus = E_DECODE_ERR;
            break;
        case E_JPEG_DEC_MRBL_DONE:
            //LDR_PUTS("ApiWaitDone_state: E_JPEG_DEC_MRBL_DONE\n");
            if(!_Sboot_JPD_PreLoadBuffer(E_JPEG_BUFFER_LOW))
            {
                if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
                    eDecodeStatus = E_STREAM_READ_ERR;
                else
                    eDecodeStatus = E_DECODE_ERR;
            }
            else
            {
                eDecodeStatus = E_DECODING;
            }
            break;
        case E_JPEG_DEC_MRBH_DONE:
            //LDR_PUTS("ApiWaitDone_state: E_JPEG_DEC_MRBH_DONE\n");
            if(!_Sboot_JPD_PreLoadBuffer(E_JPEG_BUFFER_HIGH))
            {
                if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
                    eDecodeStatus = E_STREAM_READ_ERR;
                else
                    eDecodeStatus = E_DECODE_ERR;
            }
            else
            {
                eDecodeStatus = E_DECODING;
            }
            break;
        case E_JPEG_DEC_DECODING:
        default:
            //LDR_PUTS("ApiWaitDone_state: E_JPEG_DEC_DECODING\n");
            eDecodeStatus = E_DECODING;
            break;
    }
    //LDR_PUTS("OK\n");
    return eDecodeStatus;
}

static void _Sboot_JPD_SetStatus(void)
{
    if( _geJpdDecodeState == E_JPD_DECODE_DONE )
    {
        _gu16JpdHeight = MApi_JPEG_GetAlignedHeight();
        _gu16JpdWidth = MApi_JPEG_GetAlignedWidth();
        _gu16JpdPitch = MApi_JPEG_GetAlignedPitch();
    }
}

static void _Sboot_JPD_StartDecode(EN_JPD_DECODE_TYPE eDecodeType)
{
    if(_geJpdDecodeState != E_JPD_INIT)
    {
        LDR_PUTS("_Sboot_JPD_StartDecode failed\n");
        return;
    }

    // init JPEG DECODE BUFFER
    JPEG_InitParam InitParam;

    MApi_JPEG_SetMaxDecodeResolution(JPD_BASE_WIDTH_MAX, JPD_BASE_HEIGHT_MAX);

    InitParam.u32MRCBufAddr = JPD_READ_BUF_ADR;
    InitParam.u32MRCBufSize = JPD_READ_BUF_LEN;
    InitParam.u32MWCBufAddr = JPD_OUT_ADR;
    InitParam.u32MWCBufSize = JPD_OUT_LEN;
    InitParam.u32InternalBufAddr = JPD_INTER_BUF_ADR;
    InitParam.u32InternalBufSize = JPD_INTER_BUF_LEN;

    InitParam.u32DecByteRead =  _gs32InitByteRead;
    InitParam.bEOF = _gu8EOF_flag; // TRUE, the number of read_act, End of File ?

    if(E_JPD_MAIN_DECODE == eDecodeType)
    {
        InitParam.u8DecodeType = E_JPEG_TYPE_MAIN;
    }
    else
    {
        InitParam.u8DecodeType = E_JPEG_TYPE_THUMBNAIL;
    }

    InitParam.bInitMem = TRUE;
    InitParam.pFillHdrFunc = (JPEG_FillHdrFunc)_Sboot_JPD_FillHdrFunc;

    MApi_JPEG_Init(&InitParam);

    if(MApi_JPEG_GetErrorCode() == E_JPEG_NO_ERROR)
    {
        _geJpdDecodeState = E_JPD_DECODE_HEADER;
    }
    else
    {
        _geJpdDecodeState = E_JPD_DECODE_ERR;
    }


    if(_geJpdDecodeState == E_JPD_DECODE_HEADER)
    {
        //LDR_PUTS("DECODE HEADER=======================================\n");
        if(E_JPEG_FAILED == MApi_JPEG_DecodeHdr())
        {
            if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
            {
                //LDR_PUTS("DecodeHdr read failed\n");
                _geJpdDecodeState = E_JPD_IDLE;
            }
            else
            {
                //LDR_PUTS("DecodeHdr failed\n");
                _geJpdDecodeState = E_JPD_DECODE_ERR;
            }
        }
        else
        {
            _geJpdDecodeState = E_JPD_DECODE_BEGIN;
        }
    }

    //LDR_PUTS("DECODE HEADER DONE\n");

    if(_geJpdDecodeState == E_JPD_IDLE)
    {
        //LDR_PUTS("E_JPD_IDLE\n");
        MApi_JPEG_Rst();
        MApi_JPEG_Exit();
        _geJpdDecodeState = E_JPD_INIT;
        return;
    }

    //LDR_PUTS("DECODE BEGIN========================================\n");

    //begin decode
    while(_geJpdDecodeState == E_JPD_DECODE_BEGIN)
    {
        switch(MApi_JPEG_Decode())
        {
            case E_JPEG_DONE:
                // This state is only for progressive JPEG file
                //LDR_PUTS("Decode_state: E_JPEG_DONE\n");
                _geJpdDecodeState = E_JPD_DECODE_BEGIN;
                break;
            case E_JPEG_OKAY:
                //LDR_PUTS("Decode_state: E_JPEG_OKAY\n");
                _geJpdDecodeState = E_JPD_WAITDONE;
                break;
            case E_JPEG_FAILED:
            default:
                //LDR_PUTS("Decode_state: E_JPEG_FAILED\n");
                //LDR_PUTS("MsDrv_StartDecode: DecodeBegin failed\n");
                _geJpdDecodeState = E_JPD_DECODE_ERR;
                break;
        }
    }
    //LDR_PUTS("DECODE BEGIN DONE");

    //wait done
    //LDR_PUTS("WAITDONE============================================\n");
    while(_geJpdDecodeState == E_JPD_WAITDONE)
    {
        switch(_Sboot_JPD_WaitDone())
        {
            case E_DECODE_DONE:
                //LDR_PUTS("APPWaitDone_state: E_DECODE_DONE\n");
                _geJpdDecodeState = E_JPD_DECODE_DONE;
                break;

            case E_DECODING:
                //LDR_PUTS("APPWaitDone_state: E_DECODING\n");
                _geJpdDecodeState = E_JPD_WAITDONE;
                break;

            case E_STREAM_READ_ERR:
            case E_DECODE_ERR:
            default:
                //LDR_PUTS("APPWaitDone_state: E_STREAM_READ_ERR or E_DECODE_ERR\n");
                //LDR_PUTS("WaitDone failed\n");
                _geJpdDecodeState = E_JPD_DECODE_ERR;
                MApi_JPEG_Rst();
                break;
        }
    }

    _Sboot_JPD_SetStatus();
    MApi_JPEG_Exit();
    _geJpdDecodeState = E_JPD_INIT;


}



MS_BOOL Sboot_JPD_Decode(MS_U32 *u32JpdOutAddr, RectInfo *stRectInfo)
{
    if(_Sboot_JPD_Init() == FALSE)
    {
        return FALSE;
    }

    if(JPD_READ_BUF_ADR == 0xFFFF || JPD_OUT_ADR == 0xFFFF ||JPD_INTER_BUF_ADR == 0xFFFF)
    {
        LDR_PUTS("MMAP of JPD error, Skip JPD Decode !!! \n");
        return FALSE;
    }

    //Preload data for init JPEG decoder data.
    _gs32InitByteRead = _Sboot_JPD_FileRead((MS_U8 *)_gu32ReadBuffVirAddr, JPD_READ_BUF_LEN, &_gu8EOF_flag);

    if (_gs32InitByteRead < 0)
    {
        LDR_PUTS("Initial read file error!!\n");
        return FALSE;
    }

    _Sboot_JPD_StartDecode(E_JPD_MAIN_DECODE);
    *u32JpdOutAddr = JPD_OUT_ADR;
    stRectInfo->u16Width = _gu16JpdWidth;
    stRectInfo->u16Height = _gu16JpdHeight;
    stRectInfo->u16Pitch = _gu16JpdPitch;

    return TRUE;
}
