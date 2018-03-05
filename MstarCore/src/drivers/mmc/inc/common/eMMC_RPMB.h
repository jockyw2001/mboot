#ifndef eMMC_RPMB
#define eMMC_RPMB

#include "eMMC.h"

#define eMMC_RPMB_REQ_AUTH_KEY         0x0001
#define eMMC_RPMB_REQ_WRITE_CNT_VAL    0x0002
#define eMMC_RPMB_REQ_AUTH_DATA_W      0x0003
#define eMMC_RPMB_REQ_AUTH_DATA_R      0x0004
#define eMMC_RPMB_REQ_RESULT_R         0x0005

#define eMMC_RPMB_RSP_AUTH_KEY         0x0100
#define eMMC_RPMB_RSP_WRITE_CNT_VAL    0x0200
#define eMMC_RPMB_RSP_AUTH_DATA_W      0x0300
#define eMMC_RPMB_RSP_AUTH_DATA_R      0x0400

typedef eMMC_PACK0 struct _eMMC_RPMB_DATA
{
   U16 u16_req_rsp;
   U16 u16_result;
   U16 u16_blk_cnt;
   U16 u16_addr;
   U32 u32_writecnt;
   U8  u8_nonce[16];
   U8  u8_data[256];
   U8  u8_auth_key[32];
   U8  u8_stuff[196];
}eMMC_PACK1 eMMC_RPMB_DATA;
extern int GetHmacKey(unsigned char *u8pHmacKey);
extern U32 eMMC_RPMB_program_auth_key(void);
extern U32 eMMC_RPMB_Get_Counter_for_cmd(U32 *pu32_WCnt);
extern U32 eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt);
extern U32 eMMC_RPMB_Read_Result(U16 *pu16_Result);
extern U32 eMMC_RPMB_NoDataReq(void);
extern U32 eMMC_RPMB_IfKeyWritten(void);
extern U32 eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt);
extern U32 eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr);
extern U32 eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr);
extern U32 eMMC_RPMB_Test(void);
#endif // eMMC_RPMB
