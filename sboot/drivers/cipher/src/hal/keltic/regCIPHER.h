////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
//
//  File name: regCIPHER.h
//  Description: CIPHER Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CIPHER_REG_MCU_H_
#define _CIPHER_REG_MCU_H_


//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// Addr                             Address
// Buf                              Buffer
// Clr                              Clear
// CmdQ                             Command queue
// Cnt                              Count
// Ctrl                             Control
// Flt                              Filter
// Hw                               Hardware
// Int                              Interrupt
// Len                              Length
// Ovfw                             Overflow
// Pkt                              Packet
// Rec                              Record
// Recv                             Receive
// Rmn                              Remain
// Reg                              Register
// Req                              Request
// Rst                              Reset
// Scmb                             Scramble
// Sec                              Section
// Stat                             Status
// Sw                               Software
// Ts                               Transport Stream

//#include "MsTypes.h"
#include "drvCIPHER.h"
//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_CIPHERCTRL_BASE         (0x70B00 * 2) // bank 0x170B
#define REG_IRQCTRL_BASE            (0x01900 * 2) // bank 0x1019
#define REG_TSPCTRL_BASE            (0x01500 * 2) // bank 0x1015

#define  REG32_Data   volatile MS_U32

#define DMA_CTRL_REG_SIZE       5  //0x02:ctrl  0x03:reserved  0x04:reserved  0x05:except_flag  0x06:cryptoDMA_debug
#define DMA_DIRECT_OUTPUT_SIZE  4
#define HASH_RPT_REG_SIZE       10
#define DMA_RPT_REG_SIZE        2
#define BGC_ENG_SIZE            10
#define BGC_KEY_SIZE            4
#define BGC_IV_SIZE             4
#define BGC_SUPPORT_ALGO        2


#define HASH_CMD_MSG_SIZE       (16 * 4)
#define HASH_CMD_IV_SIZE        (8 * 4)

typedef struct _REG_BGC_CTRL
{
    REG32_Data   BGC_Period;
    REG32_Data   BGC_SAddr;
    REG32_Data   BGC_EAddr;
    REG32_Data   BGC_GOLDEN4;   //MSB [160:128]
    REG32_Data   BGC_GOLDEN3;   //bits [127:96]
    REG32_Data   BGC_GOLDEN2;   //bits [95:64]
    REG32_Data   BGC_GOLDEN1;   //bits [63:32]
    REG32_Data   BGC_GOLDEN0;   //bits [31:0]
}REG_BGCCtrl;

typedef struct _REG_AESDMACtrl
{
    REG32_Data   SpareCnt ;
    REG32_Data   Cmd_Queue;
    REG32_Data   Dma_Ctrl[DMA_CTRL_REG_SIZE]; //0x02:ctrl  0x03:reserved  0x04:reserved  0x05:except_flag  0x06:cryptoDMA_debug
        #define	 REG_DMA_CTRL 			       0x0
        #define	 REG_EXCEPT_FLAG 			   0x3
        #define  REG_CRYPTODMA_DEBUG           0x4
        #define  REG_EXCEPT_FLAG_CLEAR         0x80000
 		#define  REG_DMA_NDS_SW_IV             0x1000000
        #define  REG_DMA_SW_RESET              0x1
    REG32_Data   Dma_Out[DMA_DIRECT_OUTPUT_SIZE];
    REG32_Data   Hash_Reportp[HASH_RPT_REG_SIZE];
        #define  REG_CIPHER_RPT_THREAD_MSK     0xFFFF
        #define  REG_CIPHER_RPT_OK             0x80000000
        #define  REG_CIPHER_RPT_SHT              31
    REG32_Data   Dma_Reportp[DMA_RPT_REG_SIZE];

    //////////// Back Ground Checking //////////
    REG32_Data   BGC_Algo;
    REG_BGCCtrl  BGC_Ctrl[BGC_ENG_SIZE];
    REG32_Data   BGC_Key[BGC_KEY_SIZE];
    REG32_Data   BGC_IV[BGC_IV_SIZE];
    REG32_Data   BGC_Enable;
    REG32_Data   BGC_Fail;

}REG_AESDMACtrl;

typedef enum
{
    E_CIPHER_NDS_Region_fail,
    E_CIPHER_NDS_keyoff,
    E_CIPHER_sha_write_scr,
    E_CIPHER_sha_read_scr,
    E_CIPHER_sha_error_flag,
    E_CIPHER_except_read_scr,
    E_CIPHER_except_key,
    E_CIPHER_except_CAVid,
    E_CIPHER_except_des,
    E_CIPHER_except_NDS,
    E_CIPHER_except_chain,
    E_CIPHER_except_algo,
    E_CIPHER_except_key_HID,
    E_CIPHER_except_key_cbc_mac,
    E_CIPHER_disable_masterkey1,
    E_CIPHER_disable_masterkey0,

}REG_Cipher_ErrorFlag;

typedef enum
{
    E_CIPHER_DEBUG_MASTER_HID = 0x1,
    E_CIPHER_DEBUG_DMA_HID    = 0x2,
    E_CIPHER_DEBUG_HASH_HID   = 0x4,
    E_CIPHER_DEBUG_HASH_BUSY  = 0x8,
    E_CIPHER_DEBUG_DMA_BUSY   = 0x10,

}REG_CIPHER_Debug;

typedef struct _CMD_DmaFormat0
{
    MS_U32          CL:6;
        #define     CMD_LEN_MASK            0x3F

    MS_U32          DIV:1;

    MS_U32          DK:1;

    MS_U32          OVT:1;

    MS_U32          SB:3;
        #define     CMD_DMA_SB_SEL_CLR          0x0
        #define     CMD_DMA_SB_SEL_IV1          0x1
        #define     CMD_DMA_SB_SEL_IV2          0x2

    MS_U32          Residue:3;
    	#define     CMD_DMA_RESIDUE_CLR         0x0
    	#define     CMD_DMA_RESIDUE_CTS         0x1
    	#define     CMD_DMA_RESIDUE_SCTE52      0x2

  //jamietest  MS_U32          Decrypt:1;

    MS_U32          SubAlgo:4;
        #define     CMD_DMA_SUBALGO_ECB          0x0
        #define     CMD_DMA_SUBALGO_CBC          0x1
        #define     CMD_DMA_SUBALGO_CTR          0x2
        #define     CMD_DMA_SUBALGO_CBC_MAC      0x3  // [NOTE] This sub algorithm is AES only

    MS_U32          Algo:4;
        #define     CMD_DMA_ALGO_AES                0x1
        #define     CMD_DMA_ALGO_DES                0x2
        #define     CMD_DMA_ALGO_TDES               0x3
        #define     CMD_DMA_ALGO_M6                 0x4
        #define     CMD_DMA_ALGO_M6_CCBC_DIS        0x5
        #define     CMD_DMA_ALGO_M6_KE56            0x6
        #define     CMD_DMA_ALGO_M6_KE56_CCBC_DIS   0x7
        #define     CMD_DMA_RC4                     0x8

    MS_U32          KeySel:4; //jamietest KeySel:3

        // bit [3]
        #define     CMD_DMA_KSEL_CLR_SK             0x8

        // bit [2:0]
        #define     CMD_DMA_KSEL_SK0                0x0 //session key 0
        #define     CMD_DMA_KSEL_SK1                0x1
        #define     CMD_DMA_KSEL_SK2                0x2
        #define     CMD_DMA_KSEL_RESERVED           0x3
        #define     CMD_DMA_KSEL_mk0                0x4 //master key 0
        #define     CMD_DMA_KSEL_mk1                0x5
        #define     CMD_DMA_KSEL_CCCK               0x6 //CCCK ??

    MS_U32          CMD:4;
        #define     CMD_DMA_ENCRYPT                   0x0
        #define     CMD_DMA_DECRYPT                   0x1
        #define     CMD_DMA_MASK            0x2F

    MS_U32          HID:1;                      //[NOTE] Set by HW only
        #define     CMD_ACPU                    0x0
        #define     CMD_R2                      0x1

}CMD_DmaFormat0;

typedef struct _CMD_DmaFormat1
{
    MS_U32          TID:16;
    MS_U32          CaVid:5;
    MS_U32          IntM:1;
        #define     CMD_INT_MODE_NONE       0x0
        #define     CMD_INT_MODE_EN         0x1
        //#define     CMD_INT_MODE_WAIT_CLR   0x2   // Block next operation until interrupt is cleared
    MS_U32          KS:1;
        #define     CMD_KS_SEL_LOW64          0x1 //jamietest

    MS_U32          RR:1;  	// Report Mode
        #define     CMD_DMA_RPT_MODE_REG        0x0
        #define     CMD_DMA_RPT_MODE_DRAM       0x1

    MS_U32          Dest:1;     // File output Dest
        #define     CMD_DMA_OUTPUT_DRAM         0x0
        #define     CMD_DMA_OUTPUT_REG          0x1   //[NOTE] if HID = R2 , the output will be written into DQRAM

    MS_U32          SD:2;       // Source Data
	    #define     CMD_DMA_DATA_REG            0x0
        #define     CMD_DMA_DATA_DRAM           0x1
        #define     CMD_DMA_DATA_R2             0x2
        #define     CMD_DMA_DATA_HW_INPUT       0x3 //jamietest
    MS_U32          DIBS:1;     // Data Input Byte Swap
    MS_U32          DOBS:1;     // Output Data Byte swap
    MS_U32          DODS:1;     // Output Data bit Swap
    MS_U32          COS:1;      // Output Data Swap
    MS_U32          CIS:1;      // Input Data Swap

}CMD_DmaFormat1;

typedef struct _CMD_HashFormat0
{
    MS_U32          CL:6;
    MS_U32          IntM:1;
    MS_U32          RstHash:1;

    MS_U32          OVT:1;
    MS_U32          HOS:1; //reverse hash output //jamietest
    MS_U32          Rsv:3;//jamietest

    MS_U32          Inv16:1; //invere byte order per 16 byte //jamietest

    MS_U32          IWC_Sel:1;
        #define     CMD_HASH_IWC_RPT        0x0
        #define     CMD_HASH_IWC_CMDQ       0x1

    MS_U32          HMAC_KeySel:3;
        #define     CMD_HASH_HMAC_NONE          0x0
        #define     CMD_HASH_HMAC_IKEY_MK0      0x2
        #define     CMD_HASH_HMAC_IKEY_MK1      0x3
        #define     CMD_HASH_HMAC_OKEY_MK0      0x4 //or 0x6
        #define     CMD_HASH_HMAC_OKEY_MK1      0x5 //or 0x7

    MS_U32          Dest:2;
        #define     CMD_HASH_OUTPUT_RPT     0x0
        #define     CMD_HASH_OUTPUT_R2      0x2
        #define     CMD_HASH_OUTPUT_DRAM    0x3

    MS_U32          Src:2; //jamietest
        #define     CMD_HASH_SRC_DRAM       0x0
        #define     CMD_HASH_SRC_PRV_RPT    0x1  //use previous hash operation result in hasg_rpt_reg2~9
        #define     CMD_HASH_SRC_R2         0x2
        #define     CMD_HASH_SRC_REG        0x3

    MS_U32          AutoPad:1;   // Padding if length < 512 bits

    MS_U32          InitHashSel:2;
        #define     CMD_HASH_IV_FIPS        0x0
        #define     CMD_HASH_IV_CMD         0x1
        #define     CMD_HASH_IV_PRV_RPT     0x2

    MS_U32          HashSel:2;  //jamietest
        #define     CMD_HASH_SHA1           0x0
        #define     CMD_HASH_SHA256         0x1
        #define     CMD_HASH_MD5            0x2

    MS_U32          CMD:4;
        #define     CMD_HASH_START          0x8

    MS_U32          HID:1;

}Hash_DmaFormat0;

typedef struct _CMD_HashFormat1
{
    MS_U32          TID:16;
    MS_U32          CaVid:5;
    MS_U32          Rsv:11;

}Hash_DmaFormat1;

//=====================================================================
//            RSA Register Definition
//=====================================================================

#define REG_RSA_BASE         (0x31800)

#define REG_RSA_COMMON          ( REG_RSA_BASE + 0x0 )
#define REG_RSA_ONEWAY_SYSREG   ( REG_RSA_BASE + 0x1 )
#define REG_RSA_CTRL            ( REG_RSA_BASE + 0x2 )
#define REG_RSA_IND32_ADDR      ( REG_RSA_BASE + 0x3 )
#define REG_RSA_IND32_WDATA     ( REG_RSA_BASE + 0x4 )

#define REG_RSA_SEC_RANGE_START(_a_)   ( REG_RSA_BASE + ((_a_)* 2) + 0x5 ) // a = 0 ~ 5
#define REG_RSA_SEC_RANGE_END(_a_)     ( REG_RSA_BASE + ((_a_)* 2) + 0x6 )

#define REG_RSA_KEY_SEL         ( REG_RSA_BASE + 0x32 )
#define REG_RSA_WDATA           ( REG_RSA_BASE + 0x39 )
#define REG_RSA_LV_PROTECT_EN   ( REG_RSA_BASE + 0x3a )
#define REG_RSA_MAILBOX(_a_)    ( REG_RSA_BASE + 0x40 + (_a_) )

//=====================================================================
//            MISC Register Definition
//=====================================================================

#define REG_OTP_PUB0_BASE   (0x71400 * 2)   //bank 1714
#define REG_OTP_PUB1_BASE   (0x71500 * 2)   //bank 1715
#define REG_OTP_PUB2_BASE   (0x71600 * 2)   //bank 1716

#define REG_RANDOM_NUMBER  (REG_OTP_PUB2_BASE + (0x0 << 2))

//=====================================================================
//            IRQ Register Definition
//=====================================================================
#define REG_HST0_FIQ_STATUS_63_48    (REG_IRQCTRL_BASE + (0xf * 4))
#define REG_HST0_FIQ_MASK_63_48      (REG_IRQCTRL_BASE + (0x7 * 4))

#define REG_HTS0_FIQ_MASK_CRYPTODMA  (0xFEFF)
#define REG_HTS0_FIQ_CRYPTODMA       (0x0100)   //FIQ 56 is CryptoDMA

//=====================================================================
//            TSP Register Definition
//=====================================================================
#define REG_TSP0_nRST      (0x2)


#endif // #ifndef _AESDMA_REG_MCU_H_
