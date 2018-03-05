#ifndef _NS_UBOOT_VERIFY_H_
#define _NS_UBOOT_VERIFY_H_

#define SIGNATURE_LEN               (256)


typedef struct
{
    MS_U32 u32Num;
    MS_U32 u32Size;
}IMAGE_INFO;

typedef struct
{
  U8 u8SecIdentify[8];
  IMAGE_INFO info;
  U8 u8Signature[SIGNATURE_LEN];
}_SUB_SECURE_INFO;


extern MS_U8 EmbeddedMBootRsaPub[];
extern MS_U8 EmbeddedMBootAes[];


unsigned long long BA2PA(unsigned long long u64BusAddr);
MS_BOOL uboot_Verify(unsigned int u32RAM_START, unsigned int len);
MS_BOOL uboot_Decrypt(U32 addr, U32 len, U8* key);
extern void LDR_PUTARR(MS_U8 *pArr, MS_U32 len);

#endif
