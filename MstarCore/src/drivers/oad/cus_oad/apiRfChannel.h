////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiRfChannel.h
/// @brief  RF Channel Frequency High-Level API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_RF_CHANNEL_H_
#define _API_RF_CHANNEL_H_

//---------------------------------------------------------------------------
//  Include file
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// COUNTRY_TYPE_CHINA
#define FREQ_CHINA_MIN              99000                               // KHz
#define FREQ_CHINA_MAX              866000                              // KHz
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#if 1//AUSTRALIA_FREQTBL_NEW
#define FREQ_TAL_SIZE   7
typedef struct  _FreqTblType
{
       MS_U8 u8CHNo;   //channel no.
       MS_U32 u32Freq;   //frequency
       MS_S8 u8CHNoOffset;  //No. offset ----->for displaying 0:= No.  +X:No.-X and add "A"   -X:No.-X
}FreqTblType;

#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum _EN_COUNTRY_TYPE
{
    COUNTRY_TYPE_EUROPE,
    COUNTRY_TYPE_GERMANY,
    COUNTRY_TYPE_TAIWAN,
    COUNTRY_TYPE_ITALY,
    COUNTRY_TYPE_FRANCE,
    COUNTRY_TYPE_CHINA,
    COUNTRY_TYPE_AUSTRALIA,
    COUNTRY_TYPE_BRAZIL,
    COUNTRY_TYPE_CHILE,
    COUNTRY_TYPE_HONGKONG,
    COUNTRY_TYPE_ARGENTINA,
    COUNTRY_TYPE_PERU,
    COUNTRY_TYPE_UK,
    COUNTRY_TYPE_OTHER,
    COUNTRY_TYPE_AUSTRALIA_NEW,
    COUNTRY_TYPE_NUM,

} EN_COUNTRY_TYPE;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern MS_BOOL MApi_RfChannel_GetTPSetting(MS_U8 u8RF_CH, MS_U8 u8Country, MS_U16 *u16BandWidth, MS_U32 *u32Frequency);
extern MS_U8   MApi_RfChannel_Freq2ChannelNumber(MS_U8 u8Country, MS_U32 u32Frequency);
extern MS_U8   MApi_RfChannel_GetMaxRfChNum(MS_U8 u8Country);
extern MS_U8   MApi_RfChannel_GetMinRfChNum(MS_U8 u8Country);
extern MS_U8   MApi_RfChannel_GetNextRfChNum(MS_U8 u8Country, MS_U8 u8RF_CH);
extern MS_U8   MApi_RfChannel_GetPrevtRfChNum(MS_U8 u8Country, MS_U8 u8RF_CH);

#endif // _API_RF_CHANNEL_H_

