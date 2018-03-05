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
#ifndef _BOOTROM_SPI_H_
#define _BOOTROM_SPI_H_

#define SERFLASH_SAFETY_FACTOR      1000

#define SER_FLASH_TIME(_stamp, _msec)    (_stamp =_msec)
#define SER_FLASH_EXPIRE(_stamp)         ( !(--_stamp))

#define BIT(_bit_)                  (1 << (_bit_))
#define BITS(_bits_, _val_)         ((BIT(((1)?_bits_)+1)-BIT(((0)?_bits_))) & (_val_<<((0)?_bits_)))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))

#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile MS_U16*)(_reg))) = ((*((volatile MS_U16*)(_reg))) & ~(_mask)) | ((MS_U16)(_val) & (_mask)); }
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))

#define ISP_READ(addr)                      READ_WORD(0xbf001000 + ((addr)<<2))
#define ISP_WRITE(addr, val)                WRITE_WORD(0xbf001000 + ((addr)<<2), (val))
#define ISP_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(0xbf001000 + ((addr)<<2), (val), (mask))


#define REG_ISP_PASSWORD            0x00 // ISP / XIU read / DMA mutual exclusive
#define REG_ISP_SPI_COMMAND         0x01
    // please refer to the serial flash datasheet
    #define ISP_SPI_CMD_READ                BITS(7:0, 0x03)
    #define ISP_SPI_CMD_FASTREAD            BITS(7:0, 0x0B)
    #define ISP_SPI_CMD_RDID                BITS(7:0, 0x9F)
    #define ISP_SPI_CMD_WREN                BITS(7:0, 0x06)
    #define ISP_SPI_CMD_WRDI                BITS(7:0, 0x04)
    #define ISP_SPI_CMD_SE                  BITS(7:0, 0x20)
    #define ISP_SPI_CMD_32BE                BITS(7:0, 0x52)
    #define ISP_SPI_CMD_64BE                BITS(7:0, 0xD8)
    #define ISP_SPI_CMD_CE                  BITS(7:0, 0xC7)
    #define ISP_SPI_CMD_PP                  BITS(7:0, 0x02)
    #define ISP_SPI_CMD_RDSR                BITS(7:0, 0x05)
    #define ISP_SPI_CMD_RDSR2               BITS(7:0, 0x35) // support for new WinBond Flash
    #define ISP_SPI_CMD_WRSR                BITS(7:0, 0x01)
    #define ISP_SPI_CMD_DP                  BITS(7:0, 0xB9)
    #define ISP_SPI_CMD_RDP                 BITS(7:0, 0xAB)
    #define ISP_SPI_CMD_RES                 BITS(7:0, 0xAB)
    #define ISP_SPI_CMD_REMS                BITS(7:0, 0x90)
    #define ISP_SPI_CMD_REMS4               BITS(7:0, 0xCF) // support for new MXIC Flash
    #define ISP_SPI_CMD_PARALLEL            BITS(7:0, 0x55)
    #define ISP_SPI_CMD_EN4K                BITS(7:0, 0xA5)
    #define ISP_SPI_CMD_EX4K                BITS(7:0, 0xB5)
	/* MXIC Individual Block Protection Mode */
    #define ISP_SPI_CMD_WPSEL               BITS(7:0, 0x68)
    #define ISP_SPI_CMD_SBLK                BITS(7:0, 0x36)
    #define ISP_SPI_CMD_SBULK               BITS(7:0, 0x39)
    #define ISP_SPI_CMD_RDSCUR              BITS(7:0, 0x2B)
    #define ISP_SPI_CMD_RDBLOCK             BITS(7:0, 0x3C)
    #define ISP_SPI_CMD_GBLK                BITS(7:0, 0x7E)
    #define ISP_SPI_CMD_GBULK               BITS(7:0, 0x98)
#define REG_ISP_SPI_ADDR_L          0x02 // A[15:0]
#define REG_ISP_SPI_ADDR_H          0x03 // A[23:16]
#define REG_ISP_SPI_WDATA           0x04
    #define ISP_SPI_WDATA_DUMMY     BITS(7:0, 0xFF)
#define REG_ISP_SPI_RDATA           0x05
#define REG_ISP_SPI_CLKDIV          0x06 // clock = CPU clock / this div
	#define ISP_SPI_CLKDIV2 		BIT(0)
	#define ISP_SPI_CLKDIV4 		BIT(2)
	#define ISP_SPI_CLKDIV8 		BIT(6)
	#define ISP_SPI_CLKDIV16		BIT(7)
	#define ISP_SPI_CLKDIV32		BIT(8)
	#define ISP_SPI_CLKDIV64		BIT(9)
	#define ISP_SPI_CLKDIV128		BIT(10)
#define REG_ISP_DEV_SEL             0x07
#define REG_ISP_SPI_CECLR           0x08
    #define ISP_SPI_CECLR                   BITS(0:0, 1)
#define REG_ISP_SPI_RDREQ           0x0C
    #define ISP_SPI_RDREQ                   BITS(0:0, 1)
#define REG_ISP_SPI_ENDIAN          0x0F
#define REG_ISP_SPI_RD_DATARDY      0x15
    #define ISP_SPI_RD_DATARDY_MASK     BMASK(0:0)
    #define ISP_SPI_RD_DATARDY              BITS(0:0, 1)
#define REG_ISP_SPI_WR_DATARDY      0x16
    #define ISP_SPI_WR_DATARDY_MASK     BMASK(0:0)
    #define ISP_SPI_WR_DATARDY              BITS(0:0, 1)
#define REG_ISP_SPI_WR_CMDRDY       0x17
    #define ISP_SPI_WR_CMDRDY_MASK      BMASK(0:0)
    #define ISP_SPI_WR_CMDRDY               BITS(0:0, 1)
#define REG_ISP_TRIGGER_MODE        0x2a
#define REG_ISP_CHIP_SEL            0x36
    #define SFSH_CHIP_SEL_MASK          BMASK(6:0)
    #define SFSH_CHIP_SEL_FLASH1            BIT(0)
    #define SFSH_CHIP_SEL_FLASH2            BIT(1)
    #define SFSH_CHIP_SEL_SPI_DEV1          BIT(2)
    #define SFSH_CHIP_SEL_SPI_DEV2          BIT(3)
    #define SFSH_CHIP_SEL_SPI_DEV3          BIT(4)
    #define SFSH_CHIP_SEL_SPI_DEV4          BIT(5)
    #define SFSH_CHIP_SEL_SPI_DEV5          BIT(6)
//    #define SFSH_CHIP_SEC_MASK          BMASK(7:0)          // 0x00FF // TODO: review this define
    #define SFSH_CHIP_SEL_MODE_SEL_MASK BMASK(7:7)
    #define SFSH_CHIP_SEL_RIU               BITS(7:7, 1)    // 0x0080
    #define SFSH_CHIP_SEL_XIU               BITS(7:7, 0)    // 0x0000
#define REG_ISP_CHIP_RST            0x3F // SPI clock source  [0]:gate  [1]:inv  [4:2]:clk_sel  000:Xtal clock 001:27MHz 010:36MHz 011:43.2MHz 100:54MHz 101:72MHz 110:86MHz 111:108MHz  [5]:0:xtal 1:clk_Sel
    #define SFSH_CHIP_RESET_MASK          BMASK(2:2)
    #define SFSH_CHIP_RESET                 BITS(2:2, 0)
    #define SFSH_CHIP_NOTRESET              BITS(2:2, 1)
#define REG_ISP_SPI_MODE            0x72
    #define SFSH_CHIP_FAST_MASK          BMASK(0:0) // SPI CMD [0x0B]
    #define SFSH_CHIP_FAST_ENABLE           BITS(0:0, 1)
    #define SFSH_CHIP_FAST_DISABLE          BITS(0:0, 0)
    #define SFSH_CHIP_2XREAD_MASK        BMASK(2:2) // SPI CMD [0xBB]
    #define SFSH_CHIP_2XREAD_ENABLE         BITS(2:2, 1)
    #define SFSH_CHIP_2XREAD_DISABLE        BITS(2:2, 0)
#define REG_ISP_SPI_CHIP_SELE       0x7A
    #define SFSH_CHIP_SELE_MASK          BMASK(1:0) // only for secure booting = 0;
    #define SFSH_CHIP_SELE_EXT1             BITS(1:0, 0)
    #define SFSH_CHIP_SELE_EXT2             BITS(1:0, 1)
    #define SFSH_CHIP_SELE_EXT3             BITS(1:0, 2)
#define REG_ISP_SPI_CHIP_SELE_BUSY  0x7B
    #define SFSH_CHIP_SELE_BUSY_MASK     BMASK(0:0)
    #define SFSH_CHIP_SELE_SWITCH           BITS(0:0, 1)
    #define SFSH_CHIP_SELE_DONE             BITS(0:0, 0)

// Serial Flash Register // please refer to the serial flash datasheet
#define SF_SR_WIP_MASK                  BMASK(0:0)
#define SF_SR_WEL_MASK                  BMASK(1:1)
#define SF_SR_BP_MASK                   BMASK(5:2) // BMASK(4:2) is normal case but SERFLASH_TYPE_MX25L6405 use BMASK(5:2)
#define SF_SR_PROG_ERASE_ERR_MASK       BMASK(6:6)
#define SF_SR_SRWD_MASK                 BMASK(7:7)
    #define SF_SR_SRWD                      BITS(7:7, 1)

typedef struct
{
    MS_U32  u32XiuBaseAddr;     // REG_SFSH_XIU_BASE
    MS_U32  u32Mheg5BaseAddr;
    MS_U32  u32IspBaseAddr;     // REG_ISP_BASE
    MS_U32  u32PiuBaseAddr;     // REG_PIU_BASE
    MS_U32  u32PMBaseAddr;      // REG_PM_BASE
    MS_U32  u32CLK0BaseAddr;    // REG_PM_BASE
} hal_isp_t;

#endif
