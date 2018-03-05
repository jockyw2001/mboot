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

#ifndef _DRV_SWIR_H_
#define _DRV_SWIR_H_

//------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//##############################
// Common IR register define
//##############################
#define RIU_MAP  0x1F000000 // PM bank

#define IR_REG_BASE                             (0x1E80*2) //(0x1EC0*2)

//##############################
// For Hardware RC Decode
//##############################
#define REG_IR_RC_CTRL0                         ((0x00*0x10000)+ (IR_REG_BASE + 0x00*2))
#define REG_IR_RC_RC6_LDPS_THR_L                ((0x00*0x10000)+ (IR_REG_BASE + 0x00*2))    //BIT[4:0]
    #define _IR_RC_RC6_LDPS_THR_L_MSK           (0x1F) //BIT[4:0]
    #define _IR_RC_AUTOCONFIG                   (BIT5)
    #define _IR_RC_FIFO_CLR                     (BIT6)
    #define _IR_RC_FIFO_WFST                    (BIT7)

#define REG_IR_RC_CTRL1                         ((0x00*0x10000)+ (IR_REG_BASE + 0x00*2) +1)
#define REG_IR_RC_WKUP_DEBUG_SEL                ((0x00*0x10000)+ (IR_REG_BASE + 0x00*2) +1) //BIT[6:4]
    #define _IR_RC_WKUP_DEBUG_SEL_MSK           (BIT6|BIT5|BIT4) //BIT[6:4]
    #define _IR_RC_ENABLE                       (BIT0)
    #define _IR_RC_RC6_EN                       (BIT1)
    #define _IR_RC_RC5X_EN                      (BIT2)
    #define _IR_RC_MODE_MSK                     (_IR_RC_RC5X_EN|_IR_RC_RC6_EN|_IR_RC_ENABLE)

    #define _IR_RC_WKUP_EN                      (BIT3)
    #define _IR_RC_DGB_SEL                      (BIT6|BIT5|BIT4)
    #define _IR_RC_IN_INV                       (BIT7)

#define REG_IR_RC_LGPS_THR_L                    ((0x00*0x10000)+ (IR_REG_BASE + 0x01*2))    //BIT[7:0]
#define REG_IR_RC_LGPS_THR_H                    ((0x00*0x10000)+ (IR_REG_BASE + 0x01*2) +1) //BIT[4:0]
    #define _IR_RC_LGPS_THR_L_MSK               (0xFF)
    #define _IR_RC_LGPS_THR_H_MSK               (0x1F)

#define REG_IR_RC_RC6_LGPS_MAR_L                ((0x00*0x10000)+ (IR_REG_BASE + 0x02*2))    //BIT[7:0]
#define REG_IR_RC_RC6_LGPS_MAR_H                ((0x00*0x10000)+ (IR_REG_BASE + 0x02*2) +1) //BIT[1:0]
    #define _IR_RC_RC6_LGPS_MAR_L_MSK           (0xFF)
    #define _IR_RC_RC6_LGPS_MAR_H_MSK           (0x03)

#define REG_IR_RC_RC6_LDPS_THR_H                ((0x00*0x10000)+ (IR_REG_BASE + 0x02*2) +1) //BIT[4:2]
    #define _IR_RC_RC6_LDPS_THR_H_MSK           (BIT4|BIT3|BIT2) //BIT[4:2]

#define REG_IR_RC_RC6_ECO_EN                    ((0x00*0x10000)+ (IR_REG_BASE + 0x03*2))    //BIT[0]
    #define _IR_RC_RC6_ECO_EN                   (BIT0)
#define REG_IR_RC_INTG_THR                      ((0x00*0x10000)+ (IR_REG_BASE + 0x03*2))    //BIT[7:1]
    #define _IR_RC_INTG_THR_MSK                 (0xFE)

#define REG_IR_RC_CLK_DIV                       ((0x00*0x10000)+ (IR_REG_BASE + 0x03*2) +1) //BIT[4:0]
    #define _IR_RC_CLK_DIV_MSK                  (0x1F)

#define REG_IR_RC_WDOG_COUNT                    ((0x00*0x10000)+ (IR_REG_BASE + 0x04*2))    //BIT[7:0]
#define REG_IR_RC_TIMEOUT_COUNT                 ((0x00*0x10000)+ (IR_REG_BASE + 0x04*2) +1) //BIT[7:0]
#define REG_IR_RC_COMP_KEY1                     ((0x00*0x10000)+ (IR_REG_BASE + 0x05*2))    //BIT[7:0]
#define REG_IR_RC_COMP_KEY2                     ((0x00*0x10000)+ (IR_REG_BASE + 0x05*2) +1) //BIT[7:0]

//Registers: Read Only
#define REG_IR_RC_KEY_ADDR                      ((0x00*0x10000)+ (IR_REG_BASE + 0x06*2))    //BIT[7:0]
    #define _IR_RC_RC5_KEY_ADDR_MSK             (0x1F) //{[7:6]:2'b00, [5]:toggle, [4:0]:address}
    #define _IR_RC_RC5_KEY_ADDR_TGL             (BIT5)
    #define _IR_RC_RC6_KEY_ADDR_MSK             (0xFF) //[7:0]:address

#define REG_IR_RC_KEY_CMD                       ((0x00*0x10000)+ (IR_REG_BASE + 0x06*2) +1) //BIT[7:0]
    #define _IR_RC_RC5_KEY_CMD_MSK              (0x3F)//{[7]:repeat, [6]:1'b0, [5:0]:command}
    #define _IR_RC_RC5_KEY_CMD_RPT              (BIT7)
    #define _IR_RC_RC5X_KEY_CMD_MSK             (0x7F)//{[7]:repeat, [6:0]:command}
    #define _IR_RC_RC5X_KEY_CMD_RPT             (BIT7)
    #define _IR_RC_RC6_KEY_CMD_MSK              (0xFF)//[7:0]:command

#define REG_IR_RC_RC6_KEY_MISC                  ((0x00*0x10000)+ (IR_REG_BASE + 0x07*2))    //BIT[4:0]
    #define _IR_RC_RC6_KEY_MISC_MOD_MSK         (BIT2|BIT1|BIT0)//{[4]:repeat, [3]:toggle, [2:0]:mode}
    #define _IR_RC_RC6_KEY_MISC_TGL             (BIT3)
    #define _IR_RC_RC6_KEY_MISC_RPT             (BIT4)

#define REG_IR_RC_FIFO_STATUS1                  ((0x00*0x10000)+ (IR_REG_BASE + 0x08*2))
    #define _IR_RC_FIFO_EMPTY                   (BIT0)
    #define _IR_RC_TIMEOUT_FLAG                 (BIT1)
    #define _IR_RC_FIFO_FULL                    (BIT2)
    #define _IR_RC_FIFO_WPTR_MSK                (BIT6|BIT5|BIT4)

#define REG_IR_RC_FIFO_STATUS2                  ((0x00*0x10000)+ (IR_REG_BASE + 0x08*2) +1)
    #define _IR_RC_FIFO_RPTR_MSK                (BIT2|BIT1|BIT0)

//Registers: Write Only
#define REG_IR_RC_FIFO_READ_PULSE               ((0x00*0x10000)+ (IR_REG_BASE + 0x09*2))
    #define _IR_RC_FIFO_RD_PULSE                (BIT0)

#define REG_IR_RC_WKUP_CLR                      ((0x00*0x10000)+ (IR_REG_BASE + 0x09*2) +1)
    #define _IR_RC_WKUP_CLR                     (BIT0)

#define REG_IR_RC_WKUP_ADDR                     ((0x00*0x10000)+ (IR_REG_BASE + 0x0A*2))
#define REG_IR_RC_WKUP_EN                       ((0x00*0x10000)+ (IR_REG_BASE + 0x0A*2) +1)
    #define _IR_RC_ADR_CMP_EN                   (BIT0)
    #define _IR_RC_ANY_KEY_EN                   (BIT1)

//##############################
// Common
//##############################
#define REG_IR_CTRL0                            ((0x00*0x10000)+ (IR_REG_BASE + 0x40*2))
    #define _IR_HWDEC_EN                        (BIT0)
    #define _IR_LDCCHK_EN                       (BIT1)
    #define _IR_CCODE_CHK_EN                    (BIT2)
    #define _IR_DCODE_PCHK_EN                   (BIT3)
    #define _IR_LG01H_CHK_EN                    (BIT4)
    #define _IR_RPCODE_EN                       (BIT5)
    #define _IR_INT_MSK                         (BIT6)
    #define _IR_INV                             (BIT7)

#define REG_IR_CTRL1                            ((0x00*0x10000)+ (IR_REG_BASE + 0x40*2) +1)
    #define _IR_TIMEOUT_CHK_EN                  (BIT0)
    #define _IR_SEP_CODE_CHK_EN                 (BIT1)
    #define _IR_KEY_MSB_FIRST_EN                (BIT2)
    #define _IR_BIT_INVERSE_EN                  (BIT3)
    #define _IR_TSTBUS_SEL_MSK                  (BIT5|BIT4)
    #define _IR_RAW_RPT_INT_MSK                 (BIT6)
    #define _IR_INT_CRC_MSK                     (BIT7)

#define REG_IR_HDC_UPB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x41*2))
#define REG_IR_HDC_LOB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x42*2))
#define REG_IR_OFC_UPB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x43*2))
#define REG_IR_OFC_LOB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x44*2))
#define REG_IR_OFC_RP_UPB                       ((0x00*0x10000)+ (IR_REG_BASE + 0x45*2))
#define REG_IR_OFC_RP_LOB                       ((0x00*0x10000)+ (IR_REG_BASE + 0x46*2))
#define REG_IR_LG01H_UPB                        ((0x00*0x10000)+ (IR_REG_BASE + 0x47*2))
#define REG_IR_LG01H_LOB                        ((0x00*0x10000)+ (IR_REG_BASE + 0x48*2))
#define REG_IR_LG0_UPB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x49*2))
#define REG_IR_LG0_LOB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x4A*2))
#define REG_IR_LG1_UPB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x4B*2))
#define REG_IR_LG1_LOB                          ((0x00*0x10000)+ (IR_REG_BASE + 0x4C*2))
#define REG_IR_SEPR_UPB                         ((0x00*0x10000)+ (IR_REG_BASE + 0x4D*2))
#define REG_IR_SEPR_LOB                         ((0x00*0x10000)+ (IR_REG_BASE + 0x4E*2))
#define REG_IR_TIMEOUT_CYC0                     ((0x00*0x10000)+ (IR_REG_BASE + 0x4F*2))
#define REG_IR_TIMEOUT_CYC1                     ((0x00*0x10000)+ (IR_REG_BASE + 0x50*2))
    #define _IR_TMOUT_CLR_SET                   (BIT6|BIT5|BIT4)
    #define _IR_TMOUT_CLR_SW                    (BIT7)
#define REG_IR_CODE_BYTE                        ((0x00*0x10000)+ (IR_REG_BASE + 0x50*2) +1)
    #define _IR_CODE_BIT_NUM                    (BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _IR_CCODE_BYTE                      (BIT7)
#define REG_IR_SEPR_BIT                         ((0x00*0x10000)+ (IR_REG_BASE + 0x51*2))//Mitsubishi foormat in Full mode
    #define _IR_SEPR_BIT                        (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _IR_RPT_FLG_CLR_RAW                 (BIT7)
#define REG_IR_FIFO_CTRL                        ((0x00*0x10000)+ (IR_REG_BASE + 0x51*2) +1)
    #define _IR_FIFO_DEPTH                      (BIT2|BIT1|BIT0)
    #define _IR_FIFO_FULL_EN                    (BIT3)
    #define _IR_SHOT_SEL                        (BIT5|BIT4)
    #define _IR_FIFO_CLR                        (BIT7)
#define REG_IR_CCODE                            ((0x00*0x10000)+ (IR_REG_BASE + 0x52*2))
#define REG_IR_GLHRM_NUM_L                      ((0x00*0x10000)+ (IR_REG_BASE + 0x53*2))
    #define _IR_GLHRM_NUM_MSK_L                 (0xFF)
#define REG_IR_GLHRM_NUM_H                      ((0x00*0x10000)+ (IR_REG_BASE + 0x53*2) +1)
    #define _IR_GLHRM_NUM_MSK_H                 (BIT2|BIT1|BIT0)
    #define _IR_GLHRM_EN                        (BIT3)
#define REG_IR_DECODE_MODE                      ((0x00*0x10000)+ (IR_REG_BASE + 0x53*2) +1)
    #define _IR_DECODE_MODE                     (BIT5|BIT4)
#define REG_IR_CKDIV_NUM                        ((0x00*0x10000)+ (IR_REG_BASE + 0x54*2))
#define REG_IR_KEY_DATA                         ((0x00*0x10000)+ (IR_REG_BASE + 0x54*2) +1)
#define REG_IR_SHOT_CNT0                        ((0x00*0x10000)+ (IR_REG_BASE + 0x55*2))
#define REG_IR_SHOT_CNT1                        ((0x00*0x10000)+ (IR_REG_BASE + 0x55*2) +1)
#define REG_IR_SHOT_CNT2                        ((0x00*0x10000)+ (IR_REG_BASE + 0x56*2))
    #define _IR_SHOT_CNT2_MSK                   (BIT2|BIT1|BIT0)
    #define _IR_SHOT_P                          (BIT4)
#define REG_IR_FIFO_EMPTY                       ((0x00*0x10000)+ (IR_REG_BASE + 0x56*2) +1)
    #define _IR_RPT_FLAG                        (BIT0)
    #define _IR_FIFO_EMPTY                      (BIT1)
    #define _IR_TIMEOUT_FLAG                    (BIT2)
    #define _IR_FIFO_FULL                       (BIT3)
    #define _IR_INT_FLAG                        (BIT4)
    #define _IR_INT_CRC_FLAG                    (BIT5)
    #define _IR_NEC_WKUP_FLAG                   (BIT6)
    #define _IR_RC_WKUP_FLAG                    (BIT7)

#define REG_IR_CRC_GOLDEN                       ((0x00*0x10000)+ (IR_REG_BASE + 0x57*2))
#define REG_IR_FIFO_READ_PULSE                  ((0x00*0x10000)+ (IR_REG_BASE + 0x58*2))
    #define _IR_FIFO_RD_PULSE                   (BIT0)
    #define _IR_CRC_FLAG_CLR                    (BIT1)
    #define _IR_FLAG_CLR                        (BIT2)
    #define _IR_NEC_WKUP_FLAG_CLR               (BIT3)
    #define _IR_WKUP_KEY_SEL                    (BIT7|BIT6|BIT5|BIT4)

#define REG_IR_PPM_SW_RST                       ((0x00*0x10000)+ (IR_REG_BASE + 0x58*2) +1)
    #define _IR_PPM_SW_RST                      (BIT0)

#define REG_IR_NEC_COMP_KEY1                    ((0x00*0x10000)+ (IR_REG_BASE + 0x59*2))
#define REG_IR_NEC_COMP_KEY2                    ((0x00*0x10000)+ (IR_REG_BASE + 0x59*2) +1)
#define REG_IR_NEC_COMP_KEY3                    ((0x00*0x10000)+ (IR_REG_BASE + 0x5B*2))
#define REG_IR_NEC_COMP_KEY4                    ((0x00*0x10000)+ (IR_REG_BASE + 0x5B*2) +1)

#define REG_IR_FORMAT_SEL                       ((0x00*0x10000)+ (IR_REG_BASE + 0x5A*2))
    #define _IR_FORMAT_SEL_MSK                  (BIT4|BIT3|BIT2|BIT1|BIT0)
#define REG_IR_NEC_WKUP_EN                      ((0x00*0x10000)+ (IR_REG_BASE + 0x5A*2))
    #define _IR_NEC_WKUP_EN                     (BIT6)
    #define _IR_ANYKEY_WKUP_EN                  (BIT7)

#define REG_IR_NEC_WKEY1_BIT_EN                 ((0x00*0x10000)+ (IR_REG_BASE + 0x5C*2))
#define REG_IR_NEC_WKEY2_BIT_EN                 ((0x00*0x10000)+ (IR_REG_BASE + 0x5C*2) +1)
#define REG_IR_NEC_WKEY3_BIT_EN                 ((0x00*0x10000)+ (IR_REG_BASE + 0x5D*2))
#define REG_IR_NEC_WKEY4_BIT_EN                 ((0x00*0x10000)+ (IR_REG_BASE + 0x5D*2) +1)
    #define _IR_NEC_WKEY_EN_MSK                 (0xFF)

#define REG_IR_CCODE2                           ((0x00*0x10000)+ (IR_REG_BASE + 0x60*2))
#define REG_IR_CTRL2                            ((0x00*0x10000)+ (IR_REG_BASE + 0x61*2))
    #define _IR_CCODE2_CHK_EN                   (BIT0)

#define IR_FIFO_MAX          16
typedef enum{
    STATE_INACTIVE = 0,
    STATE_HEADER_SPACE,
    STATE_BIT_DATA,
    STATE_BIT_PULSE,
    STATE_BIT_SPACE,
    STATE_BIT_TRAILER,
    STATE_RC6_DATA,
    STATE_TRAILER,
}IR_Status_e;

typedef enum{
    E_IR_TYPE_INVALID = 0,
    E_IR_TYPE_NEC,            /* NEC protocol */
    E_IR_TYPE_RC5,            /* RC5 protocol*/
    E_IR_TYPE_RC6,            /* RC6 protocol*/
    E_IR_TYPE_RCMM,           /* RCMM protocol*/
    E_IR_TYPE_KONKA,          /* Konka protocol*/
    E_IR_TYPE_HAIER,          /* Haier protocol*/
    E_IR_TYPE_RCA,            /*TCL RCA protocol**/
    E_IR_TYPE_PANASONIC,      /*Panasonic 7051 protocol**/
    E_IR_TYPE_TOSHIBA,        /*Toshiba protocol*/
    E_IR_TYPE_MAX,
}IR_Type_e;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#ifdef  DRV_SWIR_C
#define INTERFACE
#else
#define INTERFACE extern
#endif
INTERFACE BOOLEAN drv_IR_SW_GetKeyCode(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8RepeatFlag);
INTERFACE void drv_IR_SW_Initialize(void);


#undef INTERFACE
#endif
