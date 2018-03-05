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
#ifndef _BOOTROM_NAND_H_
#define _BOOTROM_NAND_H_

#define REG_BANK_CLKGEN						(0x580)
#define REG_BANK_FCIE0						(0x8980)

#define RIU_BASE							0xBF200000
#define SYS_BASE							RIU_BASE												// backward compatible
#define CLKGEN_BASE                                             (SYS_BASE+(REG_BANK_CLKGEN<<REG_OFFSET_SHIFT_BITS))
#define REG_BANK_CHIPTOP                                        (0xF00)
#define REG_OFFSET_SHIFT_BITS                                   2

#define CHIPTOP_REG_BASE_ADDR                                   (SYS_BASE+(REG_BANK_CHIPTOP<<REG_OFFSET_SHIFT_BITS))

#define FCIE_REG_BASE_ADDR					(SYS_BASE+(REG_BANK_FCIE0<<REG_OFFSET_SHIFT_BITS))
#define NANDIE_BASE							FCIE_REG_BASE_ADDR										// backward compatible

#define FCIE_REG_BASE_ADDR					(SYS_BASE+(REG_BANK_FCIE0<<REG_OFFSET_SHIFT_BITS))
#define NANDIE_BASE							FCIE_REG_BASE_ADDR										// backward compatible

#define REG_CKG_FCIE_ADDR					0x64

#define reg_ckg_fcie						(CLKGEN_BASE+(REG_CKG_FCIE_ADDR<<REG_OFFSET_SHIFT_BITS))
#define reg_clk_nfie						reg_ckg_fcie

#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
#define reg_nf_en							(CHIPTOP_REG_BASE_ADDR+(0x08<<REG_OFFSET_SHIFT_BITS))
#define reg_ci_a_pe            				(CHIPTOP_REG_BASE_ADDR+(0x24<<REG_OFFSET_SHIFT_BITS))
#define reg_ci_c_pe            				(CHIPTOP_REG_BASE_ADDR+(0x25<<REG_OFFSET_SHIFT_BITS))
#define reg_ci_d_pe            				(CHIPTOP_REG_BASE_ADDR+(0x26<<REG_OFFSET_SHIFT_BITS))
#define reg_gpio_pe            				(CHIPTOP_REG_BASE_ADDR+(0x27<<REG_OFFSET_SHIFT_BITS))
#else
#define reg_nf_en							(CHIPTOP_REG_BASE_ADDR+(0x06<<REG_OFFSET_SHIFT_BITS))
#endif
#define reg_allpad_in						(CHIPTOP_REG_BASE_ADDR+(0x50<<REG_OFFSET_SHIFT_BITS))
#define reg_boot_from_pf					(CHIPTOP_REG_BASE_ADDR+(0x01<<REG_OFFSET_SHIFT_BITS))

/* reg_ckg_fcie */
#define NC_NFIE_CLK_SHIFT					0x00
#define REG50_ECC_CTRL_INIT_VALUE           0

/* NC_ECC_CTRL 0x50*/
#define BIT_NC_PAGE_SIZE_MASK				(BIT3-1)
#define BIT_NC_PAGE_SIZE_512Bn     BIT_NC_PAGE_SIZE_MASK
#define BIT_NC_PAGE_SIZE_2KB       BIT0
#define BIT_NC_PAGE_SIZE_4KB       BIT1
#define BIT_NC_PAGE_SIZE_8KB       (BIT1|BIT0)
#define BIT_NC_PAGE_SIZE_16KB      BIT2
#define BIT_NC_PAGE_SIZE_32KB      (BIT2|BIT0)
#define BIT_NC_ECC_TYPE_MASK       (BIT3|BIT4|BIT5|BIT6)
#define BIT_NC_ECC_TYPE_4b512Bn    BIT_NC_ECC_TYPE_MASK
#define BIT_NC_ECC_TYPE_8b512B     BIT3
#define BIT_NC_ECC_TYPE_12b512B    BIT4
#define BIT_NC_ECC_TYPE_16b512B    (BIT4|BIT3)
#define BIT_NC_ECC_TYPE_20b512B    BIT5
#define BIT_NC_ECC_TYPE_24b512B    (BIT5|BIT3)
#define BIT_NC_ECC_TYPE_24b1KB     (BIT5|BIT4)
#define BIT_NC_ECC_TYPE_32b1KB     (BIT5|BIT4|BIT3)
#define BIT_NC_ECC_TYPE_40b1KB     BIT6
#define BIT_NC_ECC_TYPE_RS         BIT6
#define BIT_NC_ECCERR_NSTOP        BIT7

#define BIT_NC_ECC_FLGA_MASK       (BIT1|BIT0)
#define BIT_NC_ECC_FLGA_NOERR      0
#define BIT_NC_ECC_FLGA_CORRECT    BIT0
#define BIT_NC_ECC_FLGA_FAIL       BIT1
#define BIT_NC_ECC_FLGA_CODE       BIT_NC_ECC_FLGA_MASK
#define BIT_NC_ECC_CNT_MASK        (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2)
#define BIT_NC_ECC_CNT_SHIFT       2
#define BIT_NC_ECC_SEL_LOC_MASK    (BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)
#define BIT_NC_ECC_SEL_LOC_SHIFT   8

#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
#define NC_CLK_SRC_SEL					((MS_U16)0x40<<NC_NFIE_CLK_SHIFT)
#endif
#define NC_CLK_SEL3						((MS_U16)0x20<<NC_NFIE_CLK_SHIFT)
#define NC_CLK_SEL2						((MS_U16)0x10<<NC_NFIE_CLK_SHIFT)
#define NC_CLK_SEL1						((MS_U16)0x08<<NC_NFIE_CLK_SHIFT)
#define NC_CLK_SEL0						((MS_U16)0x04<<NC_NFIE_CLK_SHIFT)
#define R_INVERT_CLOCK						((MS_U16)0x02<<NC_NFIE_CLK_SHIFT)
#define R_DISABLE_CLOCK					((MS_U16)0x01<<NC_NFIE_CLK_SHIFT)

#define NC_CLK_MASK						((MS_U16)(NC_CLK_SEL3+NC_CLK_SEL2+NC_CLK_SEL1+NC_CLK_SEL0))

#define NC_MIE_EVENT						(FCIE_REG_BASE_ADDR+(0x00<<REG_OFFSET_SHIFT_BITS))

#define NC_MMA_PRI_REG						(FCIE_REG_BASE_ADDR+(0x02<<REG_OFFSET_SHIFT_BITS))

#define NC_PATH_CTL						(FCIE_REG_BASE_ADDR+(0x0A<<REG_OFFSET_SHIFT_BITS))
#define NC_MIE_PATH_CTL					NC_PATH_CTL

#define NC_JOB_BL_CNT						(FCIE_REG_BASE_ADDR+(0x0B<<REG_OFFSET_SHIFT_BITS))

#define NC_SDIO_CTL						(FCIE_REG_BASE_ADDR+(0x1B<<REG_OFFSET_SHIFT_BITS))

#define NC_R2N_CTRL_GET						(FCIE_REG_BASE_ADDR+(0x1F<<REG_OFFSET_SHIFT_BITS))
#define RIU2NAND_STAT						NC_R2N_CTRL_GET

#define NC_R2N_CTRL_SET						(FCIE_REG_BASE_ADDR+(0x20<<REG_OFFSET_SHIFT_BITS))
#define RIU2NAND_CTL						NC_R2N_CTRL_SET

#define NC_R2N_DATA_RD						(FCIE_REG_BASE_ADDR+(0x21<<REG_OFFSET_SHIFT_BITS))
#define RIU2NAND_R							NC_R2N_DATA_RD

#define NC_CIF_FIFO_CTL						(FCIE_REG_BASE_ADDR+(0x25<<REG_OFFSET_SHIFT_BITS))

#define NC_TEST_MODE						(FCIE_REG_BASE_ADDR+(0x30<<REG_OFFSET_SHIFT_BITS))

#define NC_SIGNAL							(FCIE_REG_BASE_ADDR+(0x40<<REG_OFFSET_SHIFT_BITS))
#define NC_CFG0								NC_SIGNAL

#define NC_WIDTH							(FCIE_REG_BASE_ADDR+(0x41<<REG_OFFSET_SHIFT_BITS))
#define NC_CFG1								NC_WIDTH

#define NC_AUXREG_ADR						(FCIE_REG_BASE_ADDR+(0x43<<REG_OFFSET_SHIFT_BITS))
#define NC_AUXR_ADR							NC_AUXREG_ADR

#define NC_AUXREG_DAT						(FCIE_REG_BASE_ADDR+(0x44<<REG_OFFSET_SHIFT_BITS))
#define NC_AUXR_DAT							NC_AUXREG_DAT

#define NC_CTRL								(FCIE_REG_BASE_ADDR+(0x45<<REG_OFFSET_SHIFT_BITS))

#define NC_PART_MODE						(FCIE_REG_BASE_ADDR+(0x47<<REG_OFFSET_SHIFT_BITS))
#define NC_PARTIAL_MODE						NC_PART_MODE

#define NC_SPARE							(FCIE_REG_BASE_ADDR+(0x48<<REG_OFFSET_SHIFT_BITS))

#define NC_SPARE_SIZE						(FCIE_REG_BASE_ADDR+(0x49<<REG_OFFSET_SHIFT_BITS))

#define NC_ECC_CTRL							(FCIE_REG_BASE_ADDR+(0x50<<REG_OFFSET_SHIFT_BITS))

#define NC_ECC_STAT0						(FCIE_REG_BASE_ADDR+(0x51<<REG_OFFSET_SHIFT_BITS))
#define NC_ECC_UNCORRECT_COUNT				NC_ECC_STAT0

#define NC_ECC_STAT2						(FCIE_REG_BASE_ADDR+(0x53<<REG_OFFSET_SHIFT_BITS))

#define NC_ECC_LOC  						(FCIE_REG_BASE_ADDR+(0x54<<REG_OFFSET_SHIFT_BITS))

#define CIFIFO_CMD_ADDR						(FCIE_REG_BASE_ADDR+(0x80<<REG_OFFSET_SHIFT_BITS))


#ifndef BIT0
#define BIT0								0x0001
#endif
#ifndef BIT1
#define BIT1								0x0002
#endif
#ifndef BIT2
#define BIT2								0x0004
#endif
#ifndef BIT3
#define BIT3								0x0008
#endif
#ifndef BIT4
#define BIT4								0x0010
#endif
#ifndef BIT5
#define BIT5								0x0020
#endif
#ifndef BIT6
#define BIT6								0x0040
#endif
#ifndef BIT7
#define BIT7								0x0080
#endif
#ifndef BIT8
#define BIT8								0x0100
#endif
#ifndef BIT9
#define BIT9								0x0200
#endif
#ifndef BIT10
#define BIT10								0x0400
#endif
#ifndef BIT11
#define BIT11								0x0800
#endif
#ifndef BIT12
#define BIT12								0x1000
#endif
#ifndef BIT13
#define BIT13								0x2000
#endif

#ifndef BIT15
#define BIT15								0x8000
#endif

#define BIT_NC_ECC_FAIL						BIT0

#define BIT_MMA_EN							BIT0
#define R_MMA_ENABLE						BIT_MMA_EN

#define BIT_PARTIAL_MODE_EN					BIT0

#define BIT_NC_CIFD_ACCESS					BIT1
#define R_NC_CIFC_ACCESS					BIT_NC_CIFD_ACCESS

#define BIT_RIU_RDY_MMA						BIT0
#define R_DMA_RD4_NFC						BIT_RIU_RDY_MMA

#define BIT_NC_JOB_START					BIT0
#define R_NC_JOB_START						BIT_NC_JOB_START

#define BIT_NC_CHK_RB_EDGEn				BIT7
#define BIT_NC_CHK_RB_HIGH				BIT6
#define R_NC_CHK_RB_HIGH					BIT_NC_CHK_RB_HIGH
#define BIT_NC_WP_AUTO						BIT5
#define R_NC_WP_AUTO						BIT_NC_WP_AUTO
#define BIT_NC_WP_H						BIT4
#define R_NC_WP_EN_H						BIT_NC_WP_H
#define BIT_NC_CE_AUTO						BIT3
#define R_NC_CE_AUTO						BIT_NC_CE_AUTO
#define BIT_NC_CE_H						BIT2
#define R_NC_CE_EN_H						BIT_NC_CE_H
#define BIT_START_SECTOR_IDX_MASK  (BIT12|BIT11|BIT10|BIT9|BIT8|BIT7)
#define BIT_R2N_DI_EN						BIT2
#define R_RIU2NAND_R_EN						BIT_R2N_DI_EN
#define BIT_R2N_DI_END						BIT3
#define R_RIU2NAND_R_END					BIT_R2N_DI_END
#define BIT_MMA_DATA_END					BIT0
#define R_NC_MMA_DATA_END					BIT_MMA_DATA_END
#define R_NC_MMA_DATA_ENDE				BIT_MMA_DATA_END

#define BIT_NC_JOB_END						BIT9
#define R_NC_JOB_END						BIT_NC_JOB_END
#define R_NC_JOB_ENDE						BIT_NC_JOB_END

#define BIT_FCIE_SOFT_RST					BIT12
#define R_NFIE_SOFT_RESETZ					BIT_FCIE_SOFT_RST

#define BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE	BIT12

#define BIT_NC_ONE_COL_ADDR					BIT13
#define R_NC_ONECOLADR						BIT_NC_ONE_COL_ADDR

#define BIT_R2N_DI_START					BIT1
#define R_RIU2NAND_R_STR					BIT_R2N_DI_START

#define BIT_NC_EN							BIT5
#define	R_NC_EN								BIT_NC_EN

#define BIT_NC_R2N_RDY						BIT10
#define R_NC_RIU2NAND_END					BIT_NC_R2N_RDY
#define R_NC_RIU2NAND_INT					BIT_NC_R2N_RDY

#define BIT_NC_R2N_ECC                      BIT12

#define AUXADR_ADRSET						0x0B
#define AUXADR_INSTQUE						0x20

#define BIT_R2N_MODE_EN						BIT0
#define R_RIU2NAND_EN						BIT_R2N_MODE_EN
#define BIT_START_SECTOR_IDX_SHIFT          7
#define OP_ADR_CYCLE_00						0x00
#define OP_ADR_CYCLE_01						0x10
#define OP_ADR_CYCLE_10						0x20
#define OP_ADR_CYCLE_11						0x30

#define OP_CODE_ADDR						0x40

#define OP_ADR_SET_0						(0<<0)
#define OP_ADR_SET0						OP_ADR_SET_0

#define OP_ADR_TYPE_FUL					(2<<2)
#define OP_ADR_FULL_ADDR					OP_ADR_TYPE_FUL

#define ADR_C2TIS0							0x4C

#define OP_ADR_C3TFS0						(OP_CODE_ADDR+OP_ADR_CYCLE_00+OP_ADR_FULL_ADDR+OP_ADR_SET0)
#define OP_ADR_C3TFS1						(OP_CODE_ADDR+OP_ADR_CYCLE_00+OP_ADR_FULL_ADDR+OP_ADR_SET1)
#define OP_ADR_C3TFS2						(OP_CODE_ADDR+OP_ADR_CYCLE_00+OP_ADR_FULL_ADDR+OP_ADR_SET2)
#define OP_ADR_C3TFS3						(OP_CODE_ADDR+OP_ADR_CYCLE_00+OP_ADR_FULL_ADDR+OP_ADR_SET3)

#define OP_ADR_C4TFS0						(OP_CODE_ADDR+OP_ADR_CYCLE_01+OP_ADR_FULL_ADDR+OP_ADR_SET0)

#define OP_ADR_C5TFS0						(OP_CODE_ADDR+OP_ADR_CYCLE_10+OP_ADR_FULL_ADDR+OP_ADR_SET0)

#define AUXADR_RPTCNT						0x18

#define ACT_RAN_DIN							0x99 // for column addr != 0
#define OP_A_RANDOM_DATA_IN					ACT_RAN_DIN

#define ACT_WAITRB							0x80
#define OP_A_WAIT_READY_BUSY				ACT_WAITRB

#define ACT_BREAK							0x88
#define OP_A_BREAK							ACT_BREAK

#define ACT_SER_DIN							0x98 // for column addr == 0
#define OP_A_SERIAL_DATA_IN					ACT_SER_DIN

#define AUXADR_RAN_CNT						0x19

#define CMD_0x00							0x00
#define OP_CMD00_READ						CMD_0x00

#define CMD_0x30							0x01
#define OP_CMD30_READ						CMD_0x30

#define CMD_0x90							0x03
#define OP_CMD90_READID						CMD_0x90

#define CMD_0xFF							0x04
#define OP_CMDFF_RESET						CMD_0xFF

#define REG(Reg_Addr)						(*(volatile MS_U16*)(Reg_Addr))

#define HAL_READ_UINT16(reg_addr, val)		(val = REG(reg_addr))
#define HAL_WRITE_UINT16(reg_addr, val)		(REG(reg_addr) = val)

#define REG_SET_BITS_UINT16(reg_addr, val)	(REG(reg_addr) |= (val))
#define REG_CLR_BITS_UINT16(reg_addr, val)	(REG(reg_addr) &= ~(val))
#define REG_W1C_BITS_UINT16(reg_addr, val)  HAL_WRITE_UINT16(reg_addr, REG(reg_addr)&(val))

#define HAL_READ_UINT8(reg_addr, val)       (val=(*(volatile MS_U8*)(reg_addr)))
#define HAL_WRITE_UINT8(reg_addr, val)		((*(volatile MS_U8*)(reg_addr)) = val)

#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
#define NC_CLK_27M							(NC_CLK_SEL2+NC_CLK_SEL1+NC_CLK_SEL0)
#else
#define NC_CLK_27M							(NC_CLK_SEL2+NC_CLK_SEL1)
#endif

// ecc define region
#define ECC_TYPE_RS                 1
#define ECC_TYPE_4BIT               2
#define ECC_TYPE_8BIT               3
#define ECC_TYPE_12BIT              4
#define ECC_TYPE_16BIT              5
#define ECC_TYPE_20BIT              6
#define ECC_TYPE_24BIT              7
#define ECC_TYPE_24BIT1KB           8
#define ECC_TYPE_32BIT1KB           9
#define ECC_TYPE_40BIT1KB           10

#define ECC_CODE_BYTECNT_RS         10
#define ECC_CODE_BYTECNT_4BIT        7
#define ECC_CODE_BYTECNT_8BIT       13

#define ECC_CODE_BYTECNT_12BIT      20
#define ECC_CODE_BYTECNT_16BIT      26
#define ECC_CODE_BYTECNT_20BIT      33
#define ECC_CODE_BYTECNT_24BIT      39

#define ECC_CODE_BYTECNT_24BIT1KB   42
#define ECC_CODE_BYTECNT_32BIT1KB   56
#define ECC_CODE_BYTECNT_40BIT1KB   70

#if (defined CONFIG_MSTAR_KRONUS)||(defined CONFIG_MSTAR_KAISERIN)||(defined CONFIG_MSTAR_KAPPA)||(defined CONFIG_MSTAR_KRITI)||(defined CONFIG_MSTAR_KERES) ||(defined CONFIG_MSTAR_KIRIN)||(defined CONFIG_MSTAR_KRIS)||(defined CONFIG_MSTAR_KAYLA) || (defined CONFIG_MSTAR_K5TN)
#define NANDINFO_ECC_TYPE ECC_TYPE_24BIT1KB
#endif

#define MIUIDTAG    (0x4D495530)      // 'MIU0' <-- wont use after version 1.4
#define CISIDTAG    (0x4154534D)      // 'MSTA' <-- will use after version 1.4
#define LDRIDTAG    (0x4C445230)      // 'LDR0'
#define BINIDTAG    (0x42494E30)      // 'BIN0'

typedef enum ENCRW_STATUS
{
    NCRW_STS_OK = 0,
    //NCRW_STS_NOMEDIA = 0x1,
    NCRW_STS_PROGRAM_FAIL = 0x2,
    NCRW_STS_ECC_ERROR = 0x4,
    NCRW_STS_WRITE_PROTECT = 0x8,
    NCRW_STS_TIMEOUT = 0x10,
    //NCRW_STS_ADDRESS_ERROR = 0x20
}NCRW_STATUS;

typedef struct _DDR_TIMING_GROUP {

	MS_U8	u8_ClkIdx;
	MS_U8	u8_DqsMode;
	MS_U8	u8_DelayCell;
	MS_U8	u8_DdrTiming;

} __attribute__((__packed__))  DDR_TIMING_GROUP_t;

typedef struct _NAND_FLASH_INFO {

	MS_U8  au8_Tag[16];
	MS_U8  u8_IDByteCnt;
	MS_U8  au8_ID[15];
	MS_U32 u32_ChkSum;
	MS_U16 u16_SpareByteCnt;
	MS_U16 u16_PageByteCnt;
	MS_U16 u16_BlkPageCnt;
	MS_U16 u16_BlkCnt;
	MS_U32 u32_Config;
	MS_U16 u16_ECCType;
	MS_U16	u16_tRC;
	MS_U8  u8_tRP;
	MS_U8  u8_tREH;
	MS_U8  u8_tREA;
	MS_U8  u8_tRR;
	MS_U16 u16_tADL;
	MS_U16 u16_tRHW;
	MS_U16 u16_tWHR;
	MS_U16 u16_InitBadBlkCnt;
	MS_U8  u8_Vendor[16];
	MS_U8  u8_PartNumber[16];
	MS_U8  u8_PairPageMapLoc;
	MS_U8  u8_PairPageMapType;
	MS_U16 u16_tCCS;
	MS_U8  u8_tCS;
	//for TV ROM Code
	MS_U8  u8_BL0PBA;
	MS_U8  u8_BL1PBA;
	MS_U8  u8_UBOOTPBA;
	MS_U16	u16_tWC;
	MS_U8  u8_tWP;
	MS_U8  u8_tWH;
	MS_U16 u16_tCWAW;
	MS_U8 u8_tCLHZ;
	MS_U8 u8_AddrCycleIdx;
	DDR_TIMING_GROUP_t tDefaultDDR;
	DDR_TIMING_GROUP_t tMaxDDR;
	DDR_TIMING_GROUP_t tMinDDR;
	MS_U8  u8_HashPBA[3][2];
	MS_U16  u16_tWW;
	MS_U8	u8_ReadRetryType;
	MS_U8  u8_BitflipThreshold;
} __attribute__((__packed__)) NAND_FLASH_INFO_t;

typedef struct _NAND_NECESS_INFO
{
    MS_U16 u16NC_CFG0;
    MS_U16 u16PageByteCnt; // number of byte per page
    MS_U16 u16PageByteCnt_BK;
    MS_U16 u16BlockPageCnt; // number of page per blk
    MS_U16 u16SectorSpareByteCnt; // number of byte per sector
    MS_U16 u16SectorByteCnt; // number of byte per sector
    MS_U16 u16SpareByteCnt;
    MS_U16 u16SpareByteCnt_BK;
    MS_U16 u16PageSectorCnt;
    MS_U16 u16ECCType; // type of ecc we provide
    MS_U16 u16ECCType_BK;
    MS_U16 u16ECCCodeByteCnt;
    MS_U16 u16ECCCorretableBit;
    MS_U16 u16Reg48_Spare;
    MS_U16 u16Reg49_SpareSize;
    MS_U16 u16Reg50_EccCtrl;
    MS_U16 pBuf[1536]; // 3k size
}__attribute__((__packed__)) NAND_NECESS_INFO;


#endif

