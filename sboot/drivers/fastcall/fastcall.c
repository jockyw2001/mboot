#include "fastcall.h"

static int fastcall_memcmp(const void * cs,const void * ct, int count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
    return res;
}


static MS_BOOL fastcall_crypt_cal(FastCryptoCalParam* param) 
{
    __asm__ __volatile__(
        "ldr x0, =" TEESMC32_OPTEE_FASTCALL_CRYPT_CAL " \n\t"
        "mov x1,%0 \n\t"
        "smc #0 \n\t"
        :
        : "r"(param)
        : "x0","x1"
    );
}

MS_BOOL fastcall_decrypt_using_REEkeybank(MS_U32 busAddr, MS_U32 len)
{
    FastCryptoCalParam AESParam;
    MS_U8 bKey[AES_KEY_LEN];

    AESParam.input_address = busAddr;
    AESParam.output_address = busAddr;
    AESParam.swkey = (MS_U64)bKey;
    AESParam.iv = (MS_U64)bKey;
    AESParam.input_size = len;
    AESParam.output_size = len;
    AESParam.swkey_len= AES_KEY_LEN;
    AESParam.iv_len = AES_KEY_LEN;
    AESParam.key_index = REE_KEYBANK_AES_KEY_INDEX;
    AESParam.key_type = SWKEY_FROM_KTAB;
    AESParam.mode = MODE_DECRYPT;
    AESParam.secure_algo = ALG_AES_ECB_NOPAD;

    flush_cache();

    fastcall_crypt_cal(&AESParam);
}

MS_BOOL fastcall_verify_using_REEkebank(MS_U32 busAddr, MS_U32 len, MS_U32 sigAddr)
{
    FastCryptoCalParam RSAParam;
    FastCryptoCalParam SHAParam;
    MS_U8 buf1[SIGNATURE_LEN+16];
    MS_U8 buf2[SHA_LEN+16];
    MS_U8* sig_plain = buf1 + (((unsigned long)buf1%16UL)+15)/16; // align 16-byte
    MS_U8* sha_out = buf2 + (((unsigned long)buf2%16UL)+15)/16; // align 16-byte
    static const MS_U8 padding1[] = { 0x00, 0x01, 0xFF, 0xFF };
    static const MS_U8 padding2[] = { 0x00, 0x30, 0x31, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20 };
    MS_U32 i, j;

    RSAParam.input_address = sigAddr;
    RSAParam.output_address = sig_plain;
    RSAParam.swkey = sig_plain;
    RSAParam.iv = sig_plain;
    RSAParam.input_size = SIGNATURE_LEN;
    RSAParam.output_size = SIGNATURE_LEN;
    RSAParam.swkey_len= AES_KEY_LEN;
    RSAParam.iv_len = AES_KEY_LEN;
    RSAParam.key_index = REE_KEYBANK_RSA_KEY_INDEX;
    RSAParam.key_type = SWKEY_FROM_KTAB;
    RSAParam.mode = MODE_VERIFY;
    RSAParam.secure_algo = ALG_RSA2048_PUBLIC;

    SHAParam.input_address = busAddr;
    SHAParam.output_address = sha_out;
    SHAParam.swkey = sha_out;
    SHAParam.iv = sha_out;
    SHAParam.input_size = len;
    SHAParam.output_size = SHA_LEN;
    SHAParam.swkey_len= AES_KEY_LEN;
    SHAParam.iv_len = AES_KEY_LEN;
    SHAParam.mode = MODE_DIGEST;
    SHAParam.secure_algo = ALG_SHA256;

    flush_cache();

    fastcall_crypt_cal(&RSAParam);
    fastcall_crypt_cal(&SHAParam);

    // compare padding
    i = 0;
    for (j=0; j<4; j++)
    {
        if (padding1[j] != sig_plain[i++])
        {
            return FALSE;
        }
    }
    for (j=0; j<200; j++)
    {
        if (0xff != sig_plain[i++])
        {
            return FALSE;
        }
    }
    for (j=0; j<20; j++)
    {
        if (padding2[j] != sig_plain[i++])
        {
            return FALSE;
        }
    }

    // compare sha
    if (fastcall_memcmp(sig_plain+SIGNATURE_LEN-SHA_LEN, sha_out, SHA_LEN)== 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
