#include "boot.h"


#define SIGNATURE_LEN               (256)
#define SHA_LEN               (32)
#define AES_KEY_LEN               (16)


typedef struct
{
    MS_U32 u32Num;
    MS_U32 u32Size;
}IMAGE_INFO;


typedef struct
{
  MS_U8 u8SecIdentify[8];
  IMAGE_INFO info;
  MS_U8 u8Signature[SIGNATURE_LEN];
}_SUB_SECURE_INFO;


typedef enum {
    ALG_AES_ECB_NOPAD = 0,
    ALG_AES_CBC_NOPAD = 1,
    ALG_SHA256 = 2,
    ALG_RSA2048_PUBLIC = 3,
    ALG_RSA2048_PRIVATE = 4, // Not Support for now
    ALG_RSA_NOPAD = 5,   // Not Support for now
    ALG_RSASSA_PKCS1_V1_5_SHA256 = 6, // Not Support for now
} FastCryptoCalSecureAlgo;

typedef enum {
    MODE_ENCRYPT = 0,
    MODE_DECRYPT = 1,
    MODE_SIGN = 2,
    MODE_VERIFY = 3,
    MODE_MAC = 4,
    MODE_DIGEST = 5,
    MODE_DERIVE = 6,
    MODE_ENCRYPT_BY_ENCRYPTED_AEK = 256,
    MODE_DECRYPT_BY_ENCRYPTED_AEK = 257,
    MODE_DECRYPT_BY_KEYLADDER = 258,
} FastCryptoCalOperationMode;

typedef enum {
    SWKEY = 0,
    SWKEY_FROM_KTAB = 1,
    HWKEY_WITH_KEY_LADDER = 2
} FastCryptoCalKeyType;

typedef struct {
    MS_U64 input_address;
    MS_U64 output_address;
    MS_U64 swkey;
    MS_U64 iv;
    MS_U32 input_size;
    MS_U32 output_size;
    MS_U32 swkey_len;
    MS_U32 iv_len;
    MS_U32 key_index;
    FastCryptoCalKeyType key_type;
    FastCryptoCalOperationMode mode;
    FastCryptoCalSecureAlgo secure_algo;
    MS_U8 option_field[512]; // For further use.
} FastCryptoCalParam;


#define TEESMC32_OPTEE_FASTCALL_CRYPT_CAL "0xb2005860"

#define REE_KEYBANK_AES_KEY_INDEX (0)
#define REE_KEYBANK_RSA_KEY_INDEX (1)


MS_BOOL fastcall_decrypt_using_REEkeybank(MS_U32 busAddr, MS_U32 len);
MS_BOOL fastcall_verify_using_REEkebank(MS_U32 busAddr, MS_U32 len, MS_U32 sigAddr);
