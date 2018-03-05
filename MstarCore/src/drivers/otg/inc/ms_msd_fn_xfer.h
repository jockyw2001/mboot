#ifndef _MS_MSD_FN_XFER_H_
#define _MS_MSD_FN_XFER_H_

#include "type.h"

typedef struct
{
    U8   opcode;
    U8   lun;
    U32  d32;
    U32  f32;
    U8   reserved[6];
} CBWCB;


typedef struct
{
    U32 signature;
    U32 tag;
    U32 dxfer_length;
    U8  dir_flag;
    U8  max_lun;
    U8  cmd_length;
    U8  unused;
    U8  cmd_bytes[16];
} MSDFN_BOT_CBW_STRU;


typedef struct
{
    U32 signature;
    U32 tag;
    U32 residue;
    U8  status;
    U8  unused1;
    U8  unused2;
    U8  unused3;
} MSDFN_BOT_CSW_STRU;


void USB_MSDFN_Decode_CBW(U8 *cbwP, MSDFN_BOT_CBW_STRU *cbw);
void USB_MSDFN_Encode_CSW(MSDFN_BOT_CSW_STRU *csw, U8 *cswP);

//#define RAMDISK

#ifdef RAMDISK
#define  RamDiskSize (128*1024)
#define  DISKSIZE                (RamDiskSize/512)
#define  MDrvGetDiskCapacity(x)   DISKSIZE

extern u8 Storage_Mem[RamDiskSize];
extern U32 volatile Storage_Mem_Addr;
#else
//bool TestUnitReadyCMD = 0;	// 20110310
#define  DISKSIZE                0//(RamDiskSize/512)
#endif
#endif
