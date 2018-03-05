//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#ifndef _MS_DisplayLogo_H_
#define _MS_DisplayLogo_H_
#include <ShareType.h>

typedef struct
{
    U16 u16DispX;
    U16 u16DispY;
    U8 bHorStretch;
    U8 bVerStretch;
}LogoDispConfigParams;

#if (!CONFIG_UBOOT_MINI_LIB)

typedef	unsigned short	WORD;
typedef	unsigned int	DWORD;
typedef	unsigned long	LONG;

#define	GET_BYTE(p)		(*p); p += 1;
#define	GET_WORD(p)		(*p) + (*(p + 1) << 8); p += 2;
#define	GET_DWORD(p)	(*p) + (*(p + 1) << 8) + (*(p + 2) << 16) + (*(p + 3) << 24); p += 4;
#define	GET_LONG(p)		(*p) + (*(p + 1) << 8) + (*(p + 2) << 16) + (*(p + 3) << 24); p += 4;

#define	GET_WORD_BE(p)	(*p << 8) + (*(p + 1)); p += 2;

#define	BM				('B') + (('M') << 8)
typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

#define	BITMAPFILEHEADER_SIZE	14

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#define	BITMAPINFOHEADER_SIZE	40

#define	BITMAPHEADER_SIZE	BITMAPFILEHEADER_SIZE+BITMAPINFOHEADER_SIZE

typedef struct tagRGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *PBITMAPINFO;

#define	SET_BITMAPFILEHEADER(sp, b)		\
		b.bfType	  		= GET_WORD(sp)	\
		b.bfSize	  		= GET_DWORD(sp)	\
		b.bfReserved1 		= GET_WORD(sp)	\
		b.bfReserved2 		= GET_WORD(sp)	\
		b.bfOffBits	  		= GET_DWORD(sp)	\

#define	SET_BITMAPINFOHEADER(sp, b)		\
		b.biSize	  		= GET_DWORD(sp)	\
		b.biWidth	  		= GET_LONG(sp)	\
		b.biHeight	  		= GET_LONG(sp)	\
		b.biPlanes	  		= GET_WORD(sp)	\
		b.biBitCount  		= GET_WORD(sp)	\
		b.biCompression	  	= GET_DWORD(sp)	\
		b.biSizeImage	  	= GET_DWORD(sp)	\
		b.biXPelsPerMeter 	= GET_LONG(sp)	\
		b.biYPelsPerMeter 	= GET_LONG(sp)	\
		b.biClrUsed		  	= GET_DWORD(sp)	\
		b.biClrImportant  	= GET_DWORD(sp)	\

static BITMAPFILEHEADER	bf;
static BITMAPINFOHEADER	bi;

#endif

#if(CONFIG_COMPRESSED_UBOOT==1)
#define LOGO_OFFSET_FROM_THE_END 0x70000 // SPI:448KB
#else
#define LOGO_OFFSET_FROM_THE_END 0x80000 // SPI:512KB
#endif
#define DEFAULT_BOOTLOGO_GOPIDX CONFIG_BOOTLOGO_GOPIDX
int do_dbtable_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootlogo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_audiopreinit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootmusic (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

