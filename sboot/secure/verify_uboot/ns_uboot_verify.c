#if (!defined(CONFIG_TEE_FASTCALL))
#if (defined(CONFIG_2ND_LOADER_ENCRYPTION) || defined(CONFIG_2ND_LOADER_VERIFICATION))

#include "../drivers/cipher/src/drvCIPHER.h"
#include "ns_uboot_verify.h"


#define MIU0_PA_BASE               (0x00000000UL)
#define MIU1_PA_BASE               (0x80000000UL)

#define RSA_PUBLIC_KEY_N_LEN (256)
#define RSA_PUBLIC_KEY_E_LEN (4)
#define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEY_N_LEN+RSA_PUBLIC_KEY_E_LEN)


static void array_reverse(char* ary, MS_U32 len)
{
    MS_U32 i, j;
    char tmp;
    for (i=0, j=len-1; i<j; ++i, --j)
    {
        tmp = ary[i];
        ary[i] = ary[j];
        ary[j] = tmp;
    }
}

unsigned long long BA2PA(unsigned long long u64BusAddr)
{
    unsigned long long u64PhyAddr = 0x0;

#if defined(CONFIG_MSTAR_KASTOR) || defined(CONFIG_MSTAR_KANO)
    if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU1_BUSADDR) ) // MIU0
        u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
    else if ( (u64BusAddr >= CONFIG_MIU1_BUSADDR) )
        u64PhyAddr = u64BusAddr - CONFIG_MIU1_BUSADDR + MIU1_PA_BASE;
#else
    u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
#endif

    return u64PhyAddr;
}

static unsigned long long __PA2BA(unsigned long long u64PhyAddr)
{
    unsigned long long u64BusAddr = 0x0;

#if defined(CONFIG_MSTAR_KASTOR) || defined(CONFIG_MSTAR_KANO)
    if( (u64PhyAddr >= MIU0_PA_BASE) && (u64PhyAddr < MIU1_PA_BASE) ) // MIU0
        u64BusAddr = u64PhyAddr - MIU0_PA_BASE + CONFIG_MIU0_BUSADDR;
    else if ( (u64PhyAddr >= MIU1_PA_BASE) ) // MIU1
        u64BusAddr = u64PhyAddr - MIU1_PA_BASE + CONFIG_MIU1_BUSADDR;
#else
    u64BusAddr = u64PhyAddr - MIU0_PA_BASE + CONFIG_MIU0_BUSADDR;
#endif

    return u64BusAddr;
}


static void * Memset (void *s, int c, unsigned int n)
{
    unsigned char *t = s;

    while(n-- > 0)
        *t++ = c;

    return s;
}

static int Memcmp(const void * cs,const void * ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}

static void *Memcpy(void *dest, const void *src, unsigned int n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    while(n-- > 0)
        *d++ = *s++;

    return dest;
}

static void _Test_RSA_ByteSwap(MS_U32 *pu32RSAOut)
{
    MS_U8 *pu8Data;
    MS_U32 u32i;

    for( u32i=0 ; u32i<64 ; u32i++ )
    {
        pu8Data = (MS_U8 *)(&pu32RSAOut[u32i]);
        pu32RSAOut[u32i] = ((MS_U32) pu8Data[0])<<24 |
                            ((MS_U32) pu8Data[1])<<16 |
                            ((MS_U32) pu8Data[2])<<8  |
                            (MS_U32)pu8Data[3];
    }
}

static void _Test_RSA_Swap(MS_U32 *pu32RSAOut, MS_U32 u32Size)
{
    MS_U32 u32TmpArray[64]= {0}, u32i;

    Memset( u32TmpArray, 0, sizeof(u32TmpArray));
    for( u32i=0 ; u32i<u32Size && u32i<64 ; u32i++ )
    {
        u32TmpArray[(u32Size-1)-u32i]= pu32RSAOut[u32i];
    }
    Memcpy( pu32RSAOut, u32TmpArray, u32i*sizeof(MS_U32));
}


static void ns_uboot_SecureAuthen(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U32 u32SramSel, U8 u8RSAKey[260])
{
    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    MS_U8 *pu8Buf = NULL;
    MS_U8 *pu8DataIn = NULL;
    MS_U8 *pu8DataOut = NULL;
    DRV_CIPHER_HASHCFG cfg;
    DRV_CIPHER_RET ret = 0;
    MS_U32 pu32CmdId = 0;
    MS_U32 u32Exception = 0;

    DrvAESDMA_RSASig stSig;
    DrvAESDMA_RSAKey stKey;
    DrvAESDMA_RSAOut stRSAOut;
    MS_U32 u32OffsetE, u32OffsetN, u32OffsetA, u32KeyLen= 0, u32KeyELen, u32KeyNLen;
    U32 i;


    // Init
    if( DRV_CIPHER_OK != MDrv_CIPHER_Init())
    {
        return ;
    }

    //==============Calculate HASH=============
    Memset(&cfg, 0, sizeof(DRV_CIPHER_HASHCFG));

    cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA256;
    cfg.stInput.u32Addr = BA2PA(u32SHAInBuf);
    cfg.stInput.u32Size = u32Size;
    cfg.pu8Digest_Buf = (MS_U8*)(u32SHAOutBuf);
    cfg.u32Digest_Buf_Size = 32;

    flush_cache();
    if(DRV_CIPHER_OK != MDrv_CIPHER_HASH(cfg, &pu32CmdId))
    {
        return ;
    }

    while(FALSE == MDrv_CIPHER_IsHASHDone(pu32CmdId, &u32Exception))
    {
        //DELAY_TASK_US(20);
    }

    //==============Calculate RSA=============
    u32KeyLen = 256;
    u32KeyELen = 4;
    u32KeyNLen = 256;

    Memset(&stSig, 0, sizeof(DrvAESDMA_RSASig));
    Memset(&stKey, 0, sizeof(DrvAESDMA_RSAKey));
    Memset(&stRSAOut, 0, sizeof(DrvAESDMA_RSAOut));

    /// LSB Input
    u32OffsetE = (sizeof(stKey.u32KeyE)-u32KeyELen) / sizeof(MS_U32);
    Memcpy( &(stKey.u32KeyE[u32OffsetE]), &u8RSAKey[u32KeyLen], u32KeyELen);
    _Test_RSA_Swap(stKey.u32KeyE, 64);

    /// MSB Input
    u32OffsetN = (sizeof(stKey.u32KeyN)-u32KeyNLen) / sizeof(MS_U32);
    Memcpy( &stKey.u32KeyN[u32OffsetN], u8RSAKey, u32KeyNLen);

    /// MSB Input
    u32OffsetA = (sizeof(stSig.u32Sig)-256) / sizeof(MS_U32);
    Memcpy( &stSig.u32Sig[u32OffsetA], (MS_U8*)u32RSAInBuf , 256);

    flush_cache();
    ret = MDrv_RSA_Calculate(&stSig, &stKey, E_DRVAESDMA_RSA2048_PUBLIC);
    if(ret != DRVAESDMA_OK)
    {
        return ;
    }

    while(MDrv_RSA_IsFinished()!=DRVAESDMA_OK)
    {
        //MsOS_DelayTaskUs(20);
    }

    MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PUBLIC, &stRSAOut);
    _Test_RSA_Swap(stRSAOut.u32RSAOut, (u32KeyLen/4));
    _Test_RSA_ByteSwap(stRSAOut.u32RSAOut);

    Memcpy((MS_U8*)u32RSAOutBuf, (MS_U8*)stRSAOut.u32RSAOut, sizeof(stRSAOut.u32RSAOut));
    return ;

}


/*
    RSA-SHA signature verification function
*/
static MS_BOOL ns_uboot_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U32 u32SramSel, U8 u8RSAKey[260])
{
    U32 i;
    MS_U32 u32SHAOut[8];
    MS_U32 u32RSAOut[64];

    if(u32SramSel==0)
    {
        //Not supported!!
        //LDR_PUTS("Not Support\n");
        return FALSE;
    }
    for(i = 0; i<(32/4); i++)
    {
        u32SHAOut[i] = 0;
        u32RSAOut[i] = 0;
    }

    ns_uboot_SecureAuthen(u32PlaintextAddr,u32Size,u32SignatureAddr,(U32)u32SHAOut, (U32)u32RSAOut,u32SramSel,u8RSAKey);

    for(i = 0; i<(32/4); i++)
    {
        if (*(u32SHAOut + i) != *(u32RSAOut + 56 + i ))
        {
            LDR_PUTS("SHAOUT -->");
            LDR_PUTARR(u32SHAOut, 32);
            LDR_PUTS("\nRSAOUT -->");
            LDR_PUTARR(u32RSAOut + 56, 32);
            return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL uboot_Decrypt(U32 addr, U32 len, U8* key)
{
    MS_BOOL ret = FALSE;
    DRV_CIPHER_DMACFG stCfg;
    MS_U32 u32Exception = 0;
    MS_U32 u32CmdId = 0;

    memset(&stCfg,  0, sizeof(DRV_CIPHER_DMACFG));
    // Algo
    stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
    stCfg.stAlgo.eSubAlgo =  E_CIPHER_SUB_ECB;
    stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
    stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;

    //Key
    stCfg.stKey.eKeySrc = E_CIPHER_KSRC_CPU;
    stCfg.stKey.u8KeyIdx = 0;
    stCfg.stKey.u8KeyLen = 16;
    stCfg.stKey.pu8KeyData = key; //key;
    stCfg.u32CAVid = 0xF; // generic DRM

    //Encrypt
    stCfg.bDecrypt = TRUE;

    //InData
    stCfg.stInput.u32Addr = addr;
    stCfg.stInput.u32Size = len;

    //OutData
    stCfg.stOutput.u32Addr = addr;
    stCfg.stOutput.u32Size = len;
    //MsOS_FlushMemory();

    flush_cache();
    if(DRV_CIPHER_OK != MDrv_CIPHER_DMACalc(stCfg, &u32CmdId))
    {
        return FALSE;
    }

    do{
        ret = MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception);
    }while(ret == FALSE);

    if(u32Exception != 0)
    {
        LDR_PUTDW(u32Exception);
        PUTC('c');
        return FALSE;
    }
    return TRUE;
}


//verify uboot
MS_BOOL uboot_Verify(unsigned int u32RAM_START, unsigned int len)
{
    MS_BOOL auth_ok = FALSE;
    flush_cache();

    auth_ok = ns_uboot_SecureMain(u32RAM_START, len, ((_SUB_SECURE_INFO*)(u32RAM_START + len))->u8Signature, 0x1, EmbeddedMBootRsaPub);
    if (!auth_ok)
    {
        LDR_PUTS("uboot auth fail\r\n");
        LDR_PUTS("\r\nEmbeddedMBootRsaPub ->");
        LDR_PUTARR(EmbeddedMBootRsaPub, 260);
        LDR_PUTS("\r\nuboot signature ->");
        LDR_PUTARR(((_SUB_SECURE_INFO*)(u32RAM_START + len))->u8Signature, SIGNATURE_LEN);
        LDR_PUTS("\r\nuboot image ->");
        LDR_PUTARR(u32RAM_START, 512);
        return FALSE;
    }

    return TRUE;
}

#endif
#endif
