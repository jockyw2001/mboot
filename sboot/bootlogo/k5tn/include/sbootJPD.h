#ifndef __SBOOTJPD_H__
#define __SBOOTJPD_H__

typedef enum
{
    E_JPD_MAIN_DECODE       = 1,
    E_JPD_THUMBNAIL_DECODE
} EN_JPD_DECODE_TYPE;

typedef enum
{
    E_JPD_IDLE = 0x01,
    E_JPD_DECODE_ERR = 0x02,
    E_JPD_DECODE_HEADER = 0x03,
    E_JPD_DECODE_BEGIN = 0x04,
    E_JPD_DECODE_DONE = 0x05,
    E_JPD_WAITDONE = 0x06,
    E_JPD_INIT = 0x07,
} EN_JPD_DECODE_STATE;



MS_BOOL Sboot_JPD_Decode(MS_U32 *u32JpdOutAddr, RectInfo *stRectInfo);

#endif
