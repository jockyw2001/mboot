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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    bootrom_c.c
/// @brief  C boot rom code function
/// @author MStar Semiconductor Inc.
/// @note   Rom code functions is running on read-only memory (ROM), all function should follow the rules. \n
///         (1) source code should be standalone
///         (2) all functions are running in one thread
///         (3) all variables should be read only except local varaibles in the stack or register.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boot.h"
#include "chip.h"
#include "Board.h"

#include "bootrom_nand.h"
#include "bootrom_spi.h"

extern MS_U32 _var_LDR_run_base;
extern MS_U32 _var_LDR_size;
extern MS_U32 _var_LDR_entry_point;


void DelayMS(MS_U32 msec)
{
    MS_U32 tmo = (MS_U32)msec * (CPU_CLK_FREQ / 2 / 1000);
    MS_U32 count;

    asm volatile ("mtc0 $0, $11" : :); // clear cp0 compare
    asm volatile ("mtc0 $0, $9" : :);  // clear cp0
    asm volatile ("ehb"::);

    do
    {
        asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    } while(count < tmo);
}

void _PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

void _PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            _PUTC('\r');
        _PUTC(*s++);
    }
}

void _PUTX(char val)
{
    if (val > 0x9)
        _PUTC('A' - 10 + val);
    else
        _PUTC('0' + val);
}

void _PUTW(MS_U16 val)
{
    _PUTX((val >> 12) & 0xF);
    _PUTX((val >> 8) & 0xF);
    _PUTX((val >> 4) & 0xF);
    _PUTX(val & 0xF);
}

void _PUTDW(MS_U32 val)
{
    _PUTW((val >> 16) & 0xFFFF);
    _PUTW(val & 0xFFFF);
}

void BOOTROM_C_GetHashOneInfo(const MS_U32 u32HASH1[])
{
    _var_LDR_run_base = u32HASH1[1022];
    _var_LDR_size = u32HASH1[1023];
}

void BOOTROM_C_GetHashTwoInfo(void)
{
    MS_U32 header = _var_LDR_run_base;
    MS_U32 entry_offset = *(MS_U32*)(header + 0x4);

    _var_LDR_entry_point = ((MS_U32)header + entry_offset); // entry_address
}

void BOOTROM_C_InitMIU_SPI(void)
{
    extern MS_U8 _ld_HASH1_run_start[];

#if defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI)   //Kappa has no sram, initial miu first
    BOOTROM_C_InitParams(_ld_HASH1_run_start);
    BOOTROM_C_GetHashOneInfo((MS_U32 *)_ld_HASH1_run_start);
#else
    BOOTROM_C_GetHashOneInfo((MS_U32 *)_ld_HASH1_run_start);
    BOOTROM_C_InitParams(_ld_HASH1_run_start);
#endif

    WREG_DBG(0xF009, 1);
}


#if ((defined(CONFIG_LD_BFN_VERSION) && (CONFIG_LD_BFN_VERSION == 4)) && (defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA))) || \
    ((defined(CONFIG_LD_BFN_VERSION) && (CONFIG_LD_BFN_VERSION == 5)) && (defined(CONFIG_MSTAR_K5TN)))

void BOOTROM_C_InitMIU_NAND(void)
{

}
#elif  (!defined(CONFIG_MSTAR_KELTIC)) && ((CONFIG_LD_BFN_VERSION == 2) || (CONFIG_LD_BFN_VERSION == 0))

static NAND_NECESS_INFO _nf_info;

void NC_ConfigContext(void)
{
    if (_nf_info.u16ECCType < ECC_TYPE_24BIT1KB)
        _nf_info.u16SectorByteCnt = 0x200;
    else
        _nf_info.u16SectorByteCnt = 0x400;

    _nf_info.u16PageSectorCnt = _nf_info.u16PageByteCnt/_nf_info.u16SectorByteCnt;
    _nf_info.u16SectorSpareByteCnt = _nf_info.u16SpareByteCnt/_nf_info.u16PageSectorCnt;
    _nf_info.u16SpareByteCnt = _nf_info.u16SectorSpareByteCnt*_nf_info.u16PageSectorCnt;
}

void NC_ReInit()
{
    _nf_info.u16Reg48_Spare = _nf_info.u16SectorSpareByteCnt;
    _nf_info.u16Reg49_SpareSize = _nf_info.u16SpareByteCnt;
    _nf_info.u16Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;

    switch(_nf_info.u16PageByteCnt)
    {
        case 0x0800:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;
            break;
        case 0x1000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;
            break;
        case 0x2000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_8KB;
            break;
        case 0x4000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_16KB;
            break;
        case 0x8000:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_32KB;
            break;
    }

    switch (_nf_info.u16ECCType)
    {
        case ECC_TYPE_RS:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_RS;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_RS;
            _nf_info.u16ECCCorretableBit = 4;
            break;
        case ECC_TYPE_4BIT:
            _nf_info.u16Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_4BIT;
            _nf_info.u16ECCCorretableBit = 4;
            break;
        case ECC_TYPE_8BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT;
            _nf_info.u16ECCCorretableBit = 8;
            break;
        case ECC_TYPE_12BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_12BIT;
            _nf_info.u16ECCCorretableBit = 12;
            break;
        case ECC_TYPE_16BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_16b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_16BIT;
            _nf_info.u16ECCCorretableBit = 16;
            break;
        case ECC_TYPE_20BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_20b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_20BIT;
            _nf_info.u16ECCCorretableBit = 20;
            break;
        case ECC_TYPE_24BIT:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b512B;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT;
            _nf_info.u16ECCCorretableBit = 24;
            break;
        case ECC_TYPE_24BIT1KB:
            _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
            _nf_info.u16ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT1KB;
            _nf_info.u16ECCCorretableBit = 24;
            break;
        default:
            break;
    }
    _nf_info.u16Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
}

void NC_Config()
{
    /*sector spare size*/
    HAL_WRITE_UINT16(NC_SPARE, _nf_info.u16Reg48_Spare);
    /* page spare size*/
    HAL_WRITE_UINT16(NC_SPARE_SIZE, _nf_info.u16Reg49_SpareSize);
    /* page size and ECC type*/
    HAL_WRITE_UINT16(NC_ECC_CTRL, _nf_info.u16Reg50_EccCtrl);
}

void _NAND_SoftReset(void)
{
    MS_U16 u16Tmp;

    HAL_WRITE_UINT16(NC_SIGNAL,~BIT_NC_CE_H);
    HAL_WRITE_UINT16(NC_CFG1,0x00);
    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,0);
    HAL_WRITE_UINT16(NC_CTRL,0);
    HAL_WRITE_UINT16(NC_MIE_EVENT,R_NC_JOB_END+R_NC_MMA_DATA_END);

    REG_CLR_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);

    DelayMS(1);

    REG_SET_BITS_UINT16(NC_TEST_MODE,BIT_FCIE_SOFT_RST);

    DelayMS(2);

    HAL_WRITE_UINT16(NC_SIGNAL, _nf_info.u16NC_CFG0);

    HAL_WRITE_UINT16(NC_CFG1,0x00);
    HAL_WRITE_UINT16(NC_CTRL,0);
    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,R_NC_EN);

    NC_ConfigContext();
    NC_ReInit();
    NC_Config();

    HAL_WRITE_UINT16(NC_SDIO_CTL, 0x8210);

#ifdef CONFIG_MSTAR_KRONUS
    HAL_READ_UINT16(CIFIFO_CMD_ADDR, u16Tmp);
#endif

}

MS_U32 _NAND_wNCJOBEND(MS_U8* u8Buf)
{
    MS_U16 u16Tmp,u16Tmp2,u16Cnt,u16_i,u16_j,u16_k,u16_ByteLoc;
    MS_U32 u32Cnt=0;

    do
    {
        HAL_READ_UINT16(NC_MIE_EVENT, u16Tmp);
        if (++u32Cnt == 0x3FFFF)
        {
            _NAND_SoftReset();
            return 0;
        }
    }while ((!(u16Tmp & BIT_NC_R2N_ECC) )&&((u16Tmp & (R_NC_JOB_END|BIT_MMA_DATA_END)) != (R_NC_JOB_END|BIT_MMA_DATA_END)));

    HAL_READ_UINT16(NC_ECC_STAT2, u16Tmp2);
    if((u16Tmp2&BIT_NC_ECC_FLGA_MASK) == BIT_NC_ECC_FLGA_FAIL)
    {
        //_PUTC('X');
        //_NAND_SoftReset();
        return 0;
    }

    do
    {
        HAL_READ_UINT16(NC_MIE_EVENT, u16Tmp);
        if (++u32Cnt == 0x3FFFFF)
        {
            _NAND_SoftReset();
            return 0;
        }
    }while ((u16Tmp & (R_NC_JOB_END|BIT_MMA_DATA_END)) != (R_NC_JOB_END|BIT_MMA_DATA_END));

    if (u16Tmp & BIT_NC_R2N_ECC)
    {
        //_PUTC('E');
        REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC);

        //correct ECC
        u16Cnt = ((u16Tmp2&BIT_NC_ECC_CNT_MASK)>>BIT_NC_ECC_CNT_SHIFT);
        //_PUTDW(u16Cnt);
        for (u16_i=0;u16_i<u16Cnt;u16_i++)
        {
            REG_CLR_BITS_UINT16(NC_ECC_STAT2, BIT_NC_ECC_SEL_LOC_MASK);
            REG_SET_BITS_UINT16(NC_ECC_STAT2, u16_i<<BIT_NC_ECC_SEL_LOC_SHIFT);
LABEL_ECC_LOC_READ:
            HAL_READ_UINT16(NC_ECC_LOC, u16_j);
            HAL_READ_UINT16(NC_ECC_LOC, u16_k);
            if(u16_j != u16_k)
                goto LABEL_ECC_LOC_READ;
            u16_ByteLoc = u16_j>>3;
            if (u16_ByteLoc < _nf_info.u16SectorByteCnt)
                u8Buf[u16_ByteLoc] ^= 1<<(u16_j%8);
        }
    }
    return 1;
}

MS_U32 _NAND_ReadSector(MS_U32 Row, MS_U32 SecInPage, MS_U16* u16Buf)
{
    MS_S32 i, j;
    MS_U16 u16Tmp;
    MS_U32 retry;

    // RIU and NAND transmission
    HAL_WRITE_UINT16(RIU2NAND_CTL, R_RIU2NAND_EN);
    //HAL_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_R2N_ECC|BIT_NC_R2N_RDY|BIT_NC_JOB_END|BIT_MMA_DATA_END);

    //if (_nf_info.u16SectorByteCnt == 0x400)
    {
        HAL_WRITE_UINT16(NC_PART_MODE, BIT_PARTIAL_MODE_EN);
        REG_CLR_BITS_UINT16(NC_PART_MODE, BIT_START_SECTOR_IDX_MASK);
        REG_SET_BITS_UINT16(NC_PART_MODE, SecInPage<<BIT_START_SECTOR_IDX_SHIFT);
    }

    // depends on user set ---> because SRAM might not enough for bigger page
    HAL_WRITE_UINT16(NC_JOB_BL_CNT, 1);

    HAL_WRITE_UINT16(NC_MMA_PRI_REG,0);

    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,R_MMA_ENABLE+R_NC_EN);

    // set row address
    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_ADRSET);
    HAL_WRITE_UINT16(NC_AUXR_DAT, (SecInPage*(_nf_info.u16SectorByteCnt)));
    HAL_WRITE_UINT16(NC_AUXR_DAT, Row);
    HAL_WRITE_UINT16(NC_AUXR_DAT, (Row>>16));

    // insert to queue
    HAL_WRITE_UINT16(NC_AUXR_ADR, AUXADR_INSTQUE);

    // if small block set one column mode first
    // write command 0x00
    HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)OP_ADR_C5TFS0<<8) + OP_CMD00_READ); // 5
    //HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)OP_ADR_C4TFS0<<8) + OP_CMD00_READ); // 4
    //HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)OP_ADR_C3TFS0<<8) + OP_CMD00_READ); // 3
    // write command 0x30
    HAL_WRITE_UINT16(NC_AUXR_DAT, ((MS_U16)OP_A_WAIT_READY_BUSY<<8) + OP_CMD30_READ);

    // write address
    HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)(OP_A_BREAK+0)<<8)+OP_A_SERIAL_DATA_IN); // repeat from instruct queue 0

    //HAL_WRITE_UINT16(NC_AUXR_DAT, ((MS_U16)OP_A_SERIAL_DATA_IN<<8) + OP_A_WAIT_READY_BUSY);
    //HAL_WRITE_UINT16(NC_AUXR_DAT,((MS_U16)(OP_A_BREAK)<<8)+OP_A_WAIT_READY_BUSY); // repeat from instruct queue 0

    // set CS to low
    //_NAND_Set_CS(0);

    // start to read data
    HAL_WRITE_UINT16(NC_CTRL, R_NC_CIFC_ACCESS+R_NC_JOB_START);

    for (i=0; i<(_nf_info.u16SectorByteCnt/512); i++)
    {
        // enable RIU to NAND
        HAL_WRITE_UINT16(RIU2NAND_CTL, R_RIU2NAND_EN);
        retry = 0;
        do
        {
            retry++;
            if(retry > 0xFFFFF)
            {
                //_PUTC('F');
                goto lb_fail;
            }
            HAL_READ_UINT16(RIU2NAND_STAT, u16Tmp);
        } while ( !(u16Tmp & BIT_RIU_RDY_MMA) );

        if (i==0)
            HAL_WRITE_UINT16(RIU2NAND_CTL, R_RIU2NAND_EN | R_RIU2NAND_R_STR);
        //_PUTC('H');

        //_PUTDW(_nf_info.u16SectorByteCnt>>9);
        for (j=0; j<(256); j++)
        {
            // dangerous! no break mechanism? but if it fails, seems it is not the major problem
            do
            {
                HAL_READ_UINT16(NC_MIE_EVENT, u16Tmp);
            } while (!(u16Tmp & BIT_NC_R2N_RDY) );

            HAL_READ_UINT16(NC_R2N_DATA_RD, u16Buf[i*(256) +j]);

            //HAL_WRITE_UINT16(NC_MIE_EVENT, R_NC_RIU2NAND_END);

            HAL_WRITE_UINT16(NC_R2N_CTRL_SET, R_RIU2NAND_EN | R_RIU2NAND_R_EN);
        }

    }
lb_fail:
    HAL_WRITE_UINT16(RIU2NAND_CTL, BIT_R2N_MODE_EN|BIT_R2N_DI_END);
    retry = _NAND_wNCJOBEND((MS_U8*)u16Buf);

    //_NAND_Set_CS(1);

    HAL_WRITE_UINT16(NC_PART_MODE, 0);

    HAL_WRITE_UINT16(RIU2NAND_CTL, 0);

    return retry;
}

MS_U32 _NAND_ReadPage(MS_U32 Row, MS_U16* u16Buf, MS_U32 unit) // unit is the number of sector in the page
{
    MS_U16 i;
    MS_U32 retry;

    for (i=0;i<unit;i++)
    {
        retry = _NAND_ReadSector(Row,i,u16Buf+i*(_nf_info.u16SectorByteCnt>>1));
        //_PUTDW(Row);
        if (!retry)
        {
            //_PUTC('W');
            return 0;
        }
        //_PUTDW(*((MS_U32*)(u16Buf+i*(_nf_info.u16SectorByteCnt>>1))));
    }
    return 1;
}


void BOOTROM_C_InitMIU_NAND(void)
{
    MS_U32 BINSize;
    // _PUTC('A');
    // _PUTC('A');

    //asm volatile (" DBGREG  0x0014");
    //asm volatile ("PUTC 'O");

    _nf_info.u16NC_CFG0 = R_NC_CHK_RB_HIGH;
    //************************************
    // switch pad
    //_NAND_SwitchPAD();

    // switch pad
#ifdef CONFIG_MSTAR_KAISERIN
    BINSize = HAL_READ_UINT16(reg_nf_en, BINSize)&(0xFFF0);
#ifdef _MSD5C51_H_ // LQFP chip
    BINSize |= (BIT1);
#elif defined(_MSD5C52_H_) // BGA chip
    BINSize |= (BIT0|BIT1);
#else
    #error "no such chip currently applied to boot from nand"
#endif
    HAL_WRITE_UINT16(reg_nf_en, BINSize);
#elif defined(CONFIG_MSTAR_KRONUS) || defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI)
    HAL_READ_UINT16(reg_nf_en, BINSize);
    BINSize |= (BIT2|BIT0);
    HAL_WRITE_UINT16(reg_nf_en, BINSize);
#elif defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN) 
    HAL_READ_UINT16(reg_nf_en, BINSize);
    BINSize &= ~(BIT5|BIT4);
#if defined(_BOARD_NAND_MODE_1_)
    BINSize |= (BIT4);
    REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
    REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
    REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
#elif defined(_BOARD_NAND_MODE_2_)
    BINSize |= (BIT5);
    REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
    REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
    REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
#else // defined(_BOARD_NAND_MODE_3_)
    BINSize |= (BIT5|BIT4);
    REG_CLR_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
    REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
    REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
#endif
    HAL_WRITE_UINT16(reg_nf_en, BINSize);
    REG_SET_BITS_UINT16(reg_gpio_pe, BIT11|BIT3);
#else
    #error "no such project currently applied to boot from nand"
#endif

    // disable all pad in
    HAL_READ_UINT16(reg_allpad_in, BINSize);
    BINSize &= (~BIT15);
    HAL_WRITE_UINT16(reg_allpad_in, BINSize);

    //************************************

    HAL_WRITE_UINT16(NC_AUXR_ADR,AUXADR_RPTCNT);
    HAL_WRITE_UINT16(NC_AUXR_DAT,0x00);

    //************************************
    // set clock

    //_NAND_SetClock();
    HAL_READ_UINT16(reg_clk_nfie, BINSize);
    BINSize &= ~(NC_CLK_MASK+R_INVERT_CLOCK+R_DISABLE_CLOCK);

    BINSize |= (NC_CLK_27M&NC_CLK_MASK);
#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) 
    BINSize |= NC_CLK_SRC_SEL;
#endif
    HAL_WRITE_UINT16(reg_clk_nfie, BINSize);
    //************************************

    _nf_info.u16PageByteCnt = 0x800;
    _nf_info.u16SpareByteCnt = 0x100;
    _nf_info.u16ECCType = NANDINFO_ECC_TYPE;
    _NAND_SoftReset();

    // enable nand controller
    HAL_WRITE_UINT16(NC_MIE_PATH_CTL,R_NC_EN);

    //_NAND_FLASHRESET(&_nf_info);

    //_nf_info.u16NC_CFG0 &= ~(R_NC_CE_AUTO+R_NC_WP_AUTO);
    //_nf_info.u16NC_CFG0 &= ~(R_NC_CE_EN_H);
    _nf_info.u16NC_CFG0 |= R_NC_WP_EN_H;

    HAL_WRITE_UINT16(NC_CFG0, _nf_info.u16NC_CFG0);
    HAL_WRITE_UINT16(NC_CIF_FIFO_CTL,0);

    REG_SET_BITS_UINT16((FCIE_REG_BASE_ADDR+(0x2F<<REG_OFFSET_SHIFT_BITS)), BIT0);

    MS_U32 row_pos = 0x0;

    //read first page
    //_PUTC('A');
    REG_SET_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);
    while (1)
    {
        //_PUTC('Q');
        BINSize = _NAND_ReadPage(row_pos,_nf_info.pBuf,1);

        //_PUTDW(*((MS_U32*)(_nf_info.pBuf)));
        //_PUTDW(row_pos);
        if ((BINSize) && (*((MS_U32*)(_nf_info.pBuf)) == CISIDTAG))
        {
            //_PUTC('G');
            _nf_info.u16PageByteCnt_BK = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_PageByteCnt;
            _nf_info.u16BlockPageCnt = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_BlkPageCnt;
            _nf_info.u16SpareByteCnt_BK = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_SpareByteCnt;
            _nf_info.u16ECCType_BK = ((NAND_FLASH_INFO_t*)(_nf_info.pBuf))->u16_ECCType;


            BINSize = _NAND_ReadPage(row_pos+10,_nf_info.pBuf,1);
            if (BINSize)
            {
                BINSize = _NAND_ReadPage(row_pos+11,_nf_info.pBuf+0x200,1);
                if (BINSize)
                {
                    BINSize = _NAND_ReadPage(row_pos+12,_nf_info.pBuf+0x400,1);
                    if (BINSize)
                    {
                        break;
                    }
                }
            }
        }

        row_pos += 32;
    }
     REG_CLR_BITS_UINT16(NC_SPARE, BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE);
    //_nf_info.u16BlockPageCnt = *((MS_U32*)(_nf_info.pSprBuf+2));
    //_PUTDW(_nf_info.u16BlockPageCnt);
    //_PUTDW(_nf_info.u16PageByteCnt);
    //_PUTC('B');
    //BOOTROM_C_InitMIU_O();
    //flush_cache();

    BOOTROM_C_InitParams(_nf_info.pBuf);
     _nf_info.u16SpareByteCnt = _nf_info.u16SpareByteCnt_BK;
     _nf_info.u16PageByteCnt = _nf_info.u16PageByteCnt_BK;
     _nf_info.u16ECCType = _nf_info.u16ECCType_BK;
    _NAND_SoftReset();

    //row_pos += _nf_info.u16BlockPageCnt;
    row_pos = 10*_nf_info.u16BlockPageCnt;
    MS_U16* MIUPos = (MS_U16*)0x80800000; //temp set at this memory position
    //_PUTC('C');
    // load loader
    while (1)
    {
        //_PUTDW(row_pos);
        _NAND_ReadPage(row_pos,MIUPos, _nf_info.u16PageByteCnt/_nf_info.u16SectorByteCnt); // read data from first block
        //_PUTDW(*((MS_U32*)(MIUPos)));
        if (*((MS_U32*)(MIUPos)) == LDRIDTAG)
        {
            //_PUTC('G');
            BINSize = *((MS_U32*)(MIUPos+0xa));
            MIUPos = (MS_U16*)(*((MS_U32*)(MIUPos+0x2)));//MIU pos is the fake position and the offset

            break;
        }
        row_pos += _nf_info.u16BlockPageCnt;
    }
    //_PUTDW(row_pos);
    //_PUTC('D');
    //_nf_info.u16PageByteCnt = BINSize;
    MS_U32 i=0;
    //_PUTDW(MIUPos);
    //_PUTDW(BINSize);
    _NAND_SoftReset();

    while(BINSize)
    {
        //_PUTDW(row_pos);
        _NAND_ReadPage(row_pos+i,MIUPos, _nf_info.u16PageByteCnt/_nf_info.u16SectorByteCnt);
        if (i==0)
        {
            if (*((MS_U32*)(MIUPos)) != LDRIDTAG)
            {
                row_pos += _nf_info.u16BlockPageCnt;
            }
            else
            {
                i++;
            }
            continue;
        }

        //_PUTC('E');
        i++;
        MIUPos += (_nf_info.u16PageByteCnt>>1);
        BINSize -= ((BINSize < _nf_info.u16PageByteCnt)?BINSize:_nf_info.u16PageByteCnt);
        if (i==_nf_info.u16BlockPageCnt)
        {
            row_pos += _nf_info.u16BlockPageCnt;
            i=0;
        }
        //_PUTDW(BINSize);
    }

    //_PUTC('G');

}
#endif

//#endif
static MS_U32 _HAL_SERFLASH_WaitReadDataRdy(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR);

    do
    {
        if ((ISP_READ(REG_ISP_SPI_RD_DATARDY) & ISP_SPI_RD_DATARDY_MASK) == ISP_SPI_RD_DATARDY )
        {
            bRet = 1;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    return bRet;
}

static MS_U32 _HAL_SERFLASH_WaitWriteDone(void)
{
    MS_U32 bRet = 0;
    MS_U32 u32Timer;

    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*5);

    do
    {
        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if (_HAL_SERFLASH_WaitReadDataRdy() == 0 )
        {
            break;
        }

        if ((ISP_READ(REG_ISP_SPI_RDATA) & SF_SR_WIP_MASK) == 0 ) // WIP = 0 write done
        {
            bRet = 1;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    return bRet;
}

MS_U32 HAL_SERFLASH_ReadID()
{
#ifdef CONFIG_LD_BOOT_FROM_OTP
    // HW doesn't support ReadID on MX/ST flash; use trigger mode instead.
    MS_U32 bRet = 0;
    MS_U32 u32I;
    MS_U32 u32Size;
    MS_U8 u8ptr ;

    ISP_WRITE(REG_ISP_PASSWORD, 0xAAAA);//_HAL_ISP_Enable();

    if (!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_ReadID_return;
    }
    bRet = 1;

HAL_SERFLASH_ReadID_return:
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);//_HAL_ISP_Disable();

    //_HAL_SPI_Rest(); // to prevent BDMA fail
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);

    return bRet;
#else
    // if we dont plan to use OTP, no necessary for SPI boot
    return 1;
#endif
}

#ifdef CONFIG_MSTAR_VID_ENABLE    //Initial Voltage Identification
#if defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN)
#define VID_ROSC_CRITERIA 0x4E00 // OSC fast than 9.75Mhz

void BOOTROM_C_InitVID(MS_U32 u32Args)
{
    MS_U32 u32otp = *(volatile MS_U32*)0xBF3238E0;
    MS_U32 u32ROSC = ( u32otp >> 16 ) & 0xFFFF;
    if(u32ROSC < VID_ROSC_CRITERIA)
    {
        // set high voltage
        CpuCorePWRHigh();
    }
    else
    {
        // set low voltage
        CpuCorePWRLow();
    }
}
#endif
#endif  /*CONFIG_MSTAR_VID_ENABLE*/
