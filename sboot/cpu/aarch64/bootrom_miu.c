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

#include "boot.h"
#include "chip.h"
#include "Board.h"

#define REG_ADDR_BASE         0x1F000000
#define REG_MIU_BASE          (0x1F200000 + 0x2400)     // 0xbf202400: 0x101200
#define REG_MIU_TEST_CTRL     (REG_MIU_BASE + 0x70 * 4) // 0xbf2025c0: 0x1012e0
#define REG_MIU_TEST_BASE     (REG_MIU_BASE + 0x71 * 4) // 0xbf2025c4: 0x1012e2
#define REG_MIU_TEST_LENGTH_L (REG_MIU_BASE + 0x72 * 4) // 0xbf2025c8: 0x1012e4
#define REG_MIU_TEST_LENGTH_H (REG_MIU_BASE + 0x73 * 4) // 0xbf2025cc: 0x1012e6
#define REG_MIU_TEST_DATA     (REG_MIU_BASE + 0x74 * 4) // 0xbf2025d0: 0x1012e8
#define REG_MIU_SEL0          (REG_MIU_BASE + 0x78 * 4) // 0xbf2025e0: 0x1012f0

#define MIU_TEST_LENGTH 0x800000
#define MIU_BIST_TIMEOUT 5000 // ms

#define MIU_RIU_REG_BASE                   0x1F000000

#define MIU0_RIU_DTOP                      0x1012
#define MIU1_RIU_DTOP                      0x1006
#define MIU0_RIU_ARB                       0x1615
#define MIU1_RIU_ARB                       0x1622
#define MIU0_RIU_ATOP                      0x110d
#define MIU1_RIU_ATOP                      0x1616
#define MIU0_RIU_DTOP_E                    0x152b
#define MIU1_RIU_DTOP_E                    0x152c
#define DRM_SECURE_BANK                    0x1608
#define BYTE_WR_PAR_BANK                   0x160f

//#define MIU_ENABLE_AUTO_WRITE_PHASE
#define DDR4_MIU_ENABLE_AUTO_WRITE_PHASE
//#define DDR4_READ_DBI_EN
//#define MPR_TRAINING_EN
#define VREF_DQ_EN
//#define TRIGGER_LEVEL_EN
#if defined(CONFIG_MSTAR_KANO) && (MIU_INTERFACE == DDR4_INTERFACE_BGA_16BIT)
//#define MIU_ENABLE_AUTO_READ_PHASE
#else
#define MIU_ENABLE_AUTO_READ_PHASE
#endif

void delayus(MS_U32 us);
#define MHal_MIU_ReadReg16(u32bank, u32reg ) *( ( volatile MS_U16* ) (MS_U64)(MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1)) )
#define MHal_MIU_WritReg16(u32bank, u32reg, u16val ) *( ( volatile MS_U16* ) (MS_U64)(MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1))  ) = (u16val)

#define READ_OTP_8(x) (*(volatile MS_U8*)(0x1F320000 + (x)))
#define READ_OTP_16(x) (*(volatile MS_U16*)(0x1F320000 + (x)))

#define STR_IS_AC_FLOW_CONTROL  0x1F001CDC

#if defined(CONFIG_LD_BOOT_MIU_DQS_ON_SRAM)
#define _MIU_DQS_FUNC __attribute__ ((section (".text.miu_dqs")))
#define _MIU_TRIM_FUNC __attribute__ ((section (".text.miu_trim")))
#define _MIU_BIST_FUNC __attribute__ ((section (".text.miu_bist")))
#else
#define _MIU_DQS_FUNC
#define _MIU_TRIM_FUNC
#define _MIU_BIST_FUNC
#endif

MS_U16 _MIU_DQS_FUNC VrefDQ_train_loop(MS_U16 u16RegMiuDtop, MS_U16 u16RegMiuArb, MS_U8 Indx, MS_U8 step, MS_U8 max);
void _MIU_DQS_FUNC single_cmd(MS_U16 u16RegMiuDtop, MS_U16 mrx, MS_U8 CmdVal);

const MS_U32 VREF_VAL[11] = {0x19, 0x1b, 0x1d, 0x1f, 0x21, 0x23, 0x25, 0x27, 0x29, 0x2b, 0x2d};

enum
{
    AC_ON = 1,
    DC_ON = 2,

};

static MS_U16 u16UseMIU1 = 0;

void putn(MS_U8 n)
{
    char c = '0' + n;

   *(volatile unsigned int*)(0x1F201300) = c;
}

//-------------------------------------------------------------------------------------------------
void _MIU_DQS_FUNC single_cmd(MS_U16 u16RegMiuDtop, MS_U16 mrx, MS_U8 CmdVal)
{
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, mrx); //Pre-charge all command
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x1e00) | (CmdVal << 9));   //[12:9] : reg_single_cmd =  2 (Pre-charge)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x0100);                      //[8] : reg_single_cmd_en = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x0100);                     //[8] : reg_single_cmd_en = 0
}

static void _MIU_BistTest(void)
{
    MS_U16 result;


    WREG16(REG_MIU_TEST_CTRL, 0x0003);  // bitst test mode:3


    do
    {
        result = RREG16(REG_MIU_TEST_CTRL);
        //asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    } while((result & 0x8000) == 0);

    if((result & 0x6000) == 0)
    {
        _PUTS("OK\n");
        WREG16(REG_MIU_SEL0, 0x0000);       // reset 0x1012f0 bit_15=0
        return;
    }

    _PUTS("FAIL\n");
}

static void _MIU_Bist1(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) & 0xffff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) >> 16) & 0xfff;

    WREG_DBG(0xA004, 2);
    _PUTS("BIST1_");
    WREG16((REG_ADDR_BASE + (0x100646 <<1 )) , 0x7FFE);//   Mask MIU1_group_0

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16(REG_MIU_TEST_BASE, 0x0000);
    WREG16(REG_MIU_TEST_LENGTH_L, u16Lo);
    WREG16(REG_MIU_TEST_LENGTH_H, u16Hi);

    WREG16(REG_MIU_SEL0, 0x8000);       //bit_15=1 choice to MIU_1

    _MIU_BistTest();
    WREG_DBG(0xA005, 2);
}

static void _MIU_Bist0(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) & 0xffff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) >> 16) & 0xfff;

    WREG_DBG(0xA002, 2);
    _PUTS("\nBIST0_");
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x7FFE);//   Mask MIU0_group_0

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16(REG_MIU_TEST_BASE, 0x0000);  //start address base unit
    WREG16(REG_MIU_TEST_LENGTH_L, u16Lo);
    WREG16(REG_MIU_TEST_LENGTH_H, u16Hi);

    WREG16(REG_MIU_TEST_DATA, 0x5aa5);  //bist test pattern

    _MIU_BistTest();
    WREG_DBG(0xA003, 2);
}

static void _MIU_Bist()
{
    WREG_DBG(0xA001, 2);

    _MIU_Bist0();

    if(u16UseMIU1)
    {
        _MIU_Bist1();
    }

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16((REG_ADDR_BASE  + (0x1012fe <<1 )) , 0x09E1);  // close miu BIST, 7f[11]  : reg_miu_test_off (MIU BIST use the same MIU client with TSP3)
}

static void _MIU_Check_Status(void)
{
    // Wait MIU0 init done
    while(RREG16(REG_ADDR_BASE + (0x101200 <<1)) & 0x8000 == 0);

    if(u16UseMIU1)
    {
        // Wait MIU1 init done
        while(RREG16(REG_ADDR_BASE + (0x100600 <<1)) & 0x8000 == 0);
    }
}

static void _MIU_ClearIPMask(void)
{
#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    // Unmask MIU0
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x101266 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x101286 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x1012a6 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x161506 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x161526 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x10121e <<1 )) , 0x8c08);

#if defined(CONFIG_MSTAR_CURRY)
    WREG16((REG_ADDR_BASE + (0x1615e6 <<1 )) , 0x0000);
#endif

    if(u16UseMIU1)
    {
        // Unmask MIU1
        WREG16((REG_ADDR_BASE + (0x100646 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x100666 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x100686 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x1006a6 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x162206 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x162226 <<1 )) , 0x0000);
    }
#else
    // open all MIU0 request mask (All IP can access MIU resource)
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x0000);   //Mask MIU0_group_0
    WREG16((REG_ADDR_BASE + (0x101266 <<1 )) , 0x0000);   //Mask MIU0_group_1
    WREG16((REG_ADDR_BASE + (0x101286 <<1 )) , 0x0000);   //Mask MIU0_group_2
    WREG16((REG_ADDR_BASE + (0x1012a6 <<1 )) , 0x0000);   // Mask MIU0_group_3

    WREG16((REG_ADDR_BASE + (0x10121e <<1 )), 0x0c08);  // SW initial done and turn on arbitor
#endif
}

typedef struct _Entry
{
    union
    {
        struct
        {
            MS_U16 addr;
            MS_U16 value;
        } reg;

        struct
        {
            MS_U16 param1;
            MS_U16 param2;
        } cmd;
    };
} Entry;

static const Entry *gEntrys;

static const Entry *_Next(void)
{
    const Entry *e = gEntrys++;

    if(e->cmd.param1 == CHIPCFG_CMD_END)
        return NULL;

    return e;
}

static void _MIU_DQS_FUNC RX_DQS_flow(MS_U8 u8MiuDev)
{
    MS_U16 u16RegVal0;
    MS_U16 u16RegMiuArb;
    MS_U16 u16RegMiuAtop;
    MS_U16 u16RegMiuDtop;
    MS_U16 u16RegMiuDtopE;
    MS_U16 u16DQSMaxCenter[4];
    MS_U16 u16KCode;
    MS_U16 u16KCodeOffset[4];
    MS_U8 i = 0;

    if(u8MiuDev == 0)
    {
        u16RegMiuArb   = MIU0_RIU_ARB;
        u16RegMiuAtop  = MIU0_RIU_ATOP;
        u16RegMiuDtop  = MIU0_RIU_DTOP;
        u16RegMiuDtopE = MIU0_RIU_DTOP_E;
    }
    else if(u8MiuDev == 1)
    {
        u16RegMiuArb   = MIU1_RIU_ARB;
        u16RegMiuAtop  = MIU1_RIU_ATOP;
        u16RegMiuDtop  = MIU1_RIU_DTOP;
        u16RegMiuDtopE = MIU1_RIU_DTOP_E;
    }
    else
    {
        return;
    }

    // Read RX DQ deskew result & write to REGs
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0000);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa2, u16RegVal0);
    u16DQSMaxCenter[0] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[0] = u16DQSMaxCenter[0] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0001);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa6, u16RegVal0);
    u16DQSMaxCenter[1] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[1] = u16DQSMaxCenter[1] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0002);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xa8, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xaa, u16RegVal0);
    u16DQSMaxCenter[2] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[2] = u16DQSMaxCenter[2] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0003);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xac, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xae, u16RegVal0);

    u16DQSMaxCenter[3] = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16DQSMaxCenter[3] = u16DQSMaxCenter[3] & 0x007F;

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x60, 0x33c8);
    u16KCode = MHal_MIU_ReadReg16(u16RegMiuAtop, 0x66);
    u16KCode = ((u16KCode & 0x007F) >> 1);

    for(i= 0; i < 4; i++)
    {
        if(u16DQSMaxCenter[i] > u16KCode)
        {
            u16RegVal0 = u16DQSMaxCenter[i] - u16KCode;
            if (u16RegVal0 >= 0xf)
            {
                u16KCodeOffset[i] = 0x0f;
            }
            else
            {
                u16KCodeOffset[i] = u16RegVal0;
            }
        }
        else
        {
            u16RegVal0 = u16KCode - u16DQSMaxCenter[i];
            if (u16RegVal0 >= 0xf)
            {
                u16KCodeOffset[i] = 0x0f;
            }
            else
            {
                u16KCodeOffset[i] = u16RegVal0;
            }
            u16KCodeOffset[i] = u16KCodeOffset[i] | 0x0010;
        }
    }
    u16RegVal0 = (u16KCodeOffset[1] << 8) | u16KCodeOffset[0];
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x94, u16RegVal0);
    u16RegVal0 = (u16KCodeOffset[3] << 8) | u16KCodeOffset[2];
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x96, u16RegVal0);
}

#if !defined(CONFIG_MSTAR_K6)
void _MIU_DQS_FUNC BootRom_MiuDdrDqs(MS_U8 u8MiuDev, E_DDR_TYPE eDRAM_Type)
{
    MS_U16 u16RegVal0;
    MS_U16 u16RegMiuArb;
    MS_U16 u16RegMiuAtop;
    MS_U16 u16RegMiuDtop;
    MS_U16 u16DQSMaxCenter[4];
    MS_U16 u16KCode;
    MS_U16 u16KCodeOffset[4];
    MS_U8 i = 0;
    MS_U16 u16RegVal;
    MS_U16 u16RegVal1;
    MS_U16 u16TestAddr0 = 0;
    MS_U16 u16TestAddr1 = 0;

#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
#if !defined(CONFIG_RecoverDQSSettingFromPMReg)
    u16TestAddr0 = RREG16(REG_ADDR_BASE + (0x003fb8<<1)) + (RREG16(REG_ADDR_BASE + (0x003fba<<1)) << 8);
    u16TestAddr1 = RREG16(REG_ADDR_BASE + (0x003fc8<<1)) + (RREG16(REG_ADDR_BASE + (0x003fca<<1)) << 8);
#endif
#endif

    if(u8MiuDev == 0)
    {
        u16RegMiuArb = MIU0_RIU_ARB;
        u16RegMiuAtop = MIU0_RIU_ATOP;
        u16RegMiuDtop = MIU0_RIU_DTOP;
    }
    else if(u8MiuDev == 1)
    {
        u16RegMiuArb = MIU1_RIU_ARB;
        u16RegMiuAtop = MIU1_RIU_ATOP;
        u16RegMiuDtop = MIU1_RIU_DTOP;
    }
    else
    {
        return;
    }

    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 & ~(0x0800));
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);

    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, u16TestAddr0);
    }
    else
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, u16TestAddr1);
    }

    /* Unmask MIU_group_4 bit15 */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x06, 0x7fff);

    /* Set test length */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0086);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);

    /*Enable miu_test limit mask*/
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);

    /* DPAT engine pre-setting */
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d81);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x7c, 0x8000);

    /* STEP 1: Set RX DQS Phase = 1/2* Kcode + offset */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    if(eDRAM_Type == E_DDR_4)
    {
#ifdef VREF_DQ_EN

        MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, (MHal_MIU_ReadReg16(u16RegMiuArb, 0x64) & 0xff00) | (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff00));       //Copy From 0x161565[7:0] to 0x101219[7:0]

        // Fine tune -->step :  +/- 2 codes twice
        u16RegVal = 4;//TDB
        u16RegVal = VrefDQ_train_loop(u16RegMiuDtop, u16RegMiuArb, u16RegVal-4, 1, 10);


        //SW need calculate Vref_Val = The Vref_val that have Max. minimum margin
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000); //[05] : reg_auto_ref_off = 1
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20); //[05] : reg_auto_ref_off = 1

        //Enable & Change DQ Vref
        //Set MRx[6:0] = Choose VrefDQ value & Range
        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
        delayus(1); //wait 150ns for tVREFDQE

        //Enable & Change DQ Vref
        //Set MRx[6:0] = Choose VrefDQ value & Range
        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
        delayus(1); //wait 150ns for tVREFDQE

        //Exit DQ Vref training Mode.
        single_cmd(u16RegMiuDtop, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0x80, 0x00);                    // [7]= 0 (Disable VrefDQ training)
        delayus(1); //wait 150ns for tVREFDQE

        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);           //[05] : reg_auto_ref_off = 0
        //MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) & ~0x1000);         //[12] : reg_cke_always_on = 0
#endif

#ifdef DDR4_MIU_ENABLE_AUTO_WRITE_PHASE
        //STEP5.Enable TX DQ Deskew test (determine optimized TX DQ phase)
        //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) | 0x01);            //reg_si_mode = 1
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x04);       //dpat_rst
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x00);       //dpat_rst
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xc8, 0x00);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xca, 0x00);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xcc, 0x00);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x02);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x13);

        while((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x78) & 0x8000) == 0x0);

        //STEP6. Read TX deskew result & Write to REGs
        //TX deskew result write to REGs
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
        delayus(1);

        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ3,DQ2,DQ1,DQ0}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xd0, u16RegVal0);
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ7,DQ6,DQ5,DQ4}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xd2, u16RegVal0);
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {DQM3,DQM2,DQM1,DQM0}
        u16RegVal1 = u16RegVal0 & 0x00ff;
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xd8, u16RegVal1);
        u16RegVal1 = (u16RegVal0 & 0xff00) >> 8;
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xe8, u16RegVal1);

        MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
        delayus(1);

        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ11,DQ10,DQ9,DQ8}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xd4, u16RegVal0);
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ15,DQ14,DQ13,DQ12}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xd6, u16RegVal0);
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {REGION3,REGION2,REGION1,REGION0}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x2e, u16RegVal0);

        MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
        delayus(1);

        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ19,DQ18,DQ17,DQ16}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xe0, u16RegVal0);
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ23,DQ22,DQ21,DQ20}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);

        MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
        delayus(1);

        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ27,DQ26,DQ25,DQ24}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xe4, u16RegVal0);
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ31,DQ30,DQ29,DQ28}
        MHal_MIU_WritReg16(u16RegMiuAtop, 0xe6, u16RegVal0);
#endif
    }

#ifdef MIU_ENABLE_AUTO_READ_PHASE
    /* STEP 4 : Enable RX DQ/DQS deskew (by DPAT engine) */

    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d80);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);

    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, MHal_MIU_ReadReg16(MIU1_RIU_DTOP, 0xf8) & ~0x8000);
    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    }
    else
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    }
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0001);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0);
    }while((u16RegVal0 & 0x8000) == 0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xc0, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xc2, 0x1fff);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x2e, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x20, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xd2, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd2) | 0x0001);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d83);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0100);


    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0x00f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0xffff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0xffff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x000f);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuArb, 0x78);
    }while((u16RegVal0 & 0x8000) == 0);

    // Read RX DQ deskew result & write to REGs
    RX_DQS_flow(u8MiuDev);

#endif

    /* STEP 6 : Disable DPAT engine & Set DQS Phase = 1/2* Kcode+offset (ratio mode) */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x0000);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d80);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);

    MHal_MIU_WritReg16(u16RegMiuDtop, 0xc0, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xc2, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x2e, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x20, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xd2, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd2) & ~(0x0001));

    /* Disable SI mode */
//    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
//    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 | 0x0800);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);

    _PUTS("MIU");
    putn(u8MiuDev);
    _PUTS("_DQS-OK\n\r");
}

void _MIU_DQS_FUNC BootRom_MiuDdr3Dqs_v2(MS_U8 u8MiuDev)
{
    MS_U16 u16RegVal0;
#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    MS_U16 u16RegVal1;
#endif
    MS_U16 u16RegMiuArb;
    MS_U16 u16RegMiuAtop;
    MS_U16 u16RegMiuDtop;
    MS_U16 u16RegMiuDtopE;
    MS_U16 u16DQSMaxCenter[4];
    MS_U16 u16KCode;
    MS_U16 u16KCodeOffset[4];
    MS_U16 u16RegValDQSBase;

    MS_U8 i = 0;

    if(u8MiuDev == 0)
    {
        u16RegMiuArb   = MIU0_RIU_ARB;
        u16RegMiuAtop  = MIU0_RIU_ATOP;
        u16RegMiuDtop  = MIU0_RIU_DTOP;
        u16RegMiuDtopE = MIU0_RIU_DTOP_E;
    }
    else if(u8MiuDev == 1)
    {
        u16RegMiuArb   = MIU1_RIU_ARB;
        u16RegMiuAtop  = MIU1_RIU_ATOP;
        u16RegMiuDtop  = MIU1_RIU_DTOP;
        u16RegMiuDtopE = MIU1_RIU_DTOP_E;
    }
    else
    {
        return;
    }

    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 & ~(0x0800));
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);

    /* Write DQS base from MIU reserved register */
    u16RegValDQSBase = MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xce);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, u16RegValDQSBase);  // 1to4 (16bit)

    /* Unmask MIU_group_4 bit15 */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x06, 0x7fff);

    /* Enable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) | 0x01);

    /*Enable miu_test limit mask*/
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);

    /* DPAT engine pre-setting */
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0000);

    //sw patch for 4x mode hang bug
    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    }
    else
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    }

    //MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x0040);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x28, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x28) | 0x01); // multi bank enable [0] =1'b1
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0xe8, 0x0400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0400);  // bist length
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);  // bist length
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x20);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0001);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0);
    }while((u16RegVal0 & 0x8000) == 0);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x7c, 0x800b);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c61);

    /* STEP 1: Set RX DQS Phase = 1/2* Kcode + offset */
    //MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    //MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);


#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    /* STEP 2 : Enable TX DQ deskew scan (by DPAT engine) */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0090);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc8, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xca, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xcc, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0002);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuArb, 0x78);
    }while((u16RegVal0 & 0x8000) == 0);

    /* STEP 3 : Read TX DQ deskew result & write to REGs */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd2, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    u16RegVal1 = 0x0000 | (u16RegVal0 & 0x00ff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd8, u16RegVal1);
    u16RegVal1 = 0x0000 | (u16RegVal0 >> 8);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe8, u16RegVal1);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd6, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x2e, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe6, u16RegVal0);
#endif

#ifdef MIU_ENABLE_AUTO_READ_PHASE
    /* STEP 4 : Enable RX DQ/DQS deskew (by DPAT engine) */
    //MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d80);

    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);


    //MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, MHal_MIU_ReadReg16(MIU1_RIU_DTOP, 0xf8) & ~0x8000);
    //if (u8MiuDev == 0)
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    //}
    //else if (u8MiuDev == 1)
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    //}
    //else
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    //}
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0001);
    //
    //do{
    //    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0);
    //}while((u16RegVal0 & 0x8000) == 0);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc0, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc2, 0x1fff);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x2e, 0x0001);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x20, 0x0001);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd2, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd2) | 0x0001);

    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c61);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0x00f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0xffff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0xffff);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x000f);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    //sw patch for 4x mode hang bug
    do{
        u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);
    }while((u16RegVal0 & 0xf) != 0x0d);

    DelayMS(1);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c60);
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuArb, 0x78);
    }while((u16RegVal0 & 0x8000) == 0);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x28, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x28) & 0xfffe); // multi bank disable [0] =1'b0
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0xe8, 0x0100);

    // Read RX DQ deskew result & write to REGs
    RX_DQS_flow(u8MiuDev);
#endif

    /* STEP 6 : Disable DPAT engine & Set DQS Phase = 1/2* Kcode+offset (ratio mode) */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c60);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);

    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc0, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xc2, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x2e, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x20, 0x0000);
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd2, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd2) & ~(0x0001));



    /* Disable SI mode */
    //u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 | 0x0800);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

    _PUTS("MIU");
    putn(u8MiuDev);
    _PUTS("_DQS-OK\n\r");
}

#if defined(CONFIG_RecoverDQSSettingFromPMReg)
void _MIU_DQS_FUNC BootRom_RecoverDQSSetting(void)
{
    #if defined(CONFIG_MSTAR_KANO)
    //rtc0
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x50<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x01<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x51<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x02<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x52<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x03<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x53<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x04<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x54<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x05<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x55<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x06<<1))<<1)));

    //
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x56<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f80<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x56<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f82<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x57<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f84<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x57<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f86<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x50<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f88<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x50<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f8a<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x51<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f8c<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x51<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f8e<<1)));

    WREG8(REG_ADDR_BASE + ((0x161600+(0x52<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f90<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x52<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f92<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x53<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f94<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x53<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f96<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x54<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f98<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x54<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f9a<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x55<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f9c<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x55<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f9e<<1)));

    WREG8(REG_ADDR_BASE + ((0x161600+(0x56<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa0<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x56<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fa2<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x57<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa4<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x57<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fa6<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa8<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003faa<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4b<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fac<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4b<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fae<<1)));

    WREG8(REG_ADDR_BASE + ((0x161600+(0x4a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb0<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x4a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fb2<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x4b<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb4<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x4b<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fb6<<1)));
    #elif defined(CONFIG_MSTAR_CURRY)
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x50<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x01<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x51<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x02<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x52<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x03<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x53<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x04<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x54<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x05<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x55<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x06<<1))<<1)));

    //
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x56<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f80<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x56<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f82<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x57<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f84<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x57<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f86<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x17<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f88<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x17<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f8a<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x68<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f8c<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x68<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f8e<<1)));

    WREG8(REG_ADDR_BASE + ((0x110D00+(0x69<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f90<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x69<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f92<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f94<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f96<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6b<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f98<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6b<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f9a<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6c<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f9c<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6c<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f9e<<1)));

    WREG8(REG_ADDR_BASE + ((0x110D00+(0x70<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa0<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x70<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fa2<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x71<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa4<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x71<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fa6<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa8<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003faa<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4b<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fac<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x4b<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fae<<1)));

    WREG8(REG_ADDR_BASE + ((0x110D00+(0x72<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb0<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x72<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fb2<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x73<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb4<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x73<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fb6<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x74<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb8<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x74<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fba<<1)));

    #endif
}
#endif
//-------------------------------------------------------------------------------------------------
MS_U16 _MIU_DQS_FUNC VrefDQ_train_loop(MS_U16 u16RegMiuDtop, MS_U16 u16RegMiuArb, MS_U8 Indx, MS_U8 step, MS_U8 max)
{
    MS_U16 loop;

    MS_U16 u16temp1=0;
    MS_U16 u16temp2=0;

    MS_U16 u16data[3]={0,0,0};

    MS_U8  u8temp;
    MS_U8  u8temp1;
    MS_U8  u8temp2;

    MS_U8 loop_cnt=0;

    for (loop=Indx; loop<((max*step)+Indx); loop=loop+step)
    {
        if (loop > 10)
        {
            break;
        }
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);       //[05] : reg_auto_ref_off = 1

        //Enable & Change DQ Vref
        //Set MRx[6:0] = Choose VrefDQ value & Range
        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[loop] | 0x80, 0x00); // = 1 (Enable VrefDQ training)
        delayus(1); //wait 150ns for tVREFDQE

        single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[loop] | 0x80, 0x00); // = 1 (Enable VrefDQ training)
        delayus(1); //wait 150ns for tVREFDQE

        //Enter DQ Vref training Mode.
        MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);       //[05] : reg_auto_ref_off = 0
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x8009); //SWITCH dpat deb_out = min. margin
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0004);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);                                                 //dpat_rst

        // Enable TX DQ deskew scan (by DPAT engine)
        MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0002);
        MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

        while((MHal_MIU_ReadReg16(u16RegMiuArb, 0x78) & 0x8000) == 0x0);


        u16data[2]=u16data[1];
        u16data[1]=u16data[0];
        //u16data[0]=MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);

        u16data[0]=MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e) << 4;
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x800e); //SWITCH dpat deb_out = min. margin
        u16data[0] += MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x800f); //SWITCH dpat deb_out = min. margin
        u16data[0] += MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x7e);
        //Final Min. margin = (Total Min. Margin)* 4 + Low word Min. Margin + High word Min. Margin
        u16temp1 = u16data[0] + (u16data[1] << 1) + u16data[2];
        if (u16temp1 >= u16temp2)
        {
            //if (u16temp1 > u16temp2)
            //{
            //    u8temp2=loop-1;
            //}
            u16temp2=u16temp1;
            u8temp1=loop-1;
        }
        #ifdef DEBUG
        if(loop_cnt >= 8)  {
        MHal_MIU_WritReg16(u16RegMiuArb, (0xa0 + ((loop_cnt-8)<<1) ), (u16data[0]) );
        }
        else if(loop_cnt >= 4)  {
        MHal_MIU_WritReg16(u16RegMiuArb, (0x90 + ((loop_cnt-4)<<1) ), (u16data[0]) );
        }
        else {
        MHal_MIU_WritReg16(u16RegMiuArb, (0x80 + (loop_cnt<<1) ), (u16data[0]) );
        }
        loop_cnt++;
        #endif
    }
    //u8temp = ((u8temp1 + u8temp2 + 1) >> 1);
    return u8temp1;
}

//-------------------------------------------------------------------------------------------------
void delayus(MS_U32 us)
{
    MS_U16 u16RegVal0;

    u16RegVal0 = ((us* 12) & 0xffff);
    MHal_MIU_WritReg16(0x30, 0x24, u16RegVal0);

    u16RegVal0 = ((us* 12) >> 16);
    MHal_MIU_WritReg16(0x30, 0x26, u16RegVal0);

    u16RegVal0 = 0x0002;
    MHal_MIU_WritReg16(0x30, 0x20, u16RegVal0);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(0x30, 0x22);
    }while((u16RegVal0 & 0x0001) == 0);
}

void _MIU_DQS_FUNC BootRom_MiuDdr4Dqs(MS_U8 u8MiuDev)
{
    MS_U16 u16RegVal0;
    MS_U16 u16RegVal1;
    MS_U16 u16RegMiuArb;
    MS_U16 u16RegMiuAtop;
    MS_U16 u16RegMiuDtop;
    MS_U16 u16RegMiuDtopE;
    MS_U16 u16DQSMaxCenter[4];
    MS_U16 u16KCodeOffset[4];
    MS_U16 u16KCode;
    MS_U8  i = 0;
    MS_U8  u8Cnt;
    MS_U8  u8Cnt1;
    MS_U16 u16RegVal;
    MS_U32 u32RegVal;
    MS_U8  Start[2];

    if(u8MiuDev == 0)
    {
        u16RegMiuArb  = MIU0_RIU_ARB;
        u16RegMiuAtop = MIU0_RIU_ATOP;
        u16RegMiuDtop = MIU0_RIU_DTOP;
        u16RegMiuDtopE = MIU0_RIU_DTOP_E;
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    }
    else if(u8MiuDev == 1)
    {
        u16RegMiuArb  = MIU1_RIU_ARB;
        u16RegMiuAtop = MIU1_RIU_ATOP;
        u16RegMiuDtop = MIU1_RIU_DTOP;
        u16RegMiuDtopE = MIU1_RIU_DTOP_E;
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    }
    else
    {
        return;
    }

    u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 & ~(0x0800));
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);

    /* Unmask MIU_group_4 bit15 */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x06, 0x7fff);

    /* Enable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) | 0x01);

    /*Enable miu_test limit mask*/
    /* STEP 0: STEP0. DPAT engine pre-setting  */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);
#ifdef MPR_TRAINING_EN
    /* STEP2. MPR training   (determine initial trigger level setting) */
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x20);    //[05] : reg_auto_ref_off = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) | 0x1000);  //[12] : reg_cke_always_on = 1


    //--------------------------------------------------------
    // SW set MPR pattens. (pattern can be changed here!)
    // Set 'mpr0'     = 8'h55 (DRAM default)
    // Set 'mpr1'     = 8'h33 (DRAM default)
    // Set 'mpr2'     = 8'h0f (DRAM default)
    // Set 'mpr3'     = 8'h00 (DRAM default)
    // Set 'mpr_mode' = 2'h2
    //--------------------------------------------------------
    /* MPR Write begin */
    //Insert precharge all
    single_cmd(u16RegMiuDtop, 0x0400, 0x02);

    //Program MPR Mode to Staggered mode & Enable MPR Mode
    single_cmd(u16RegMiuDtop, 0x7004, 0x00); //{3'b011,mpr_mode,11'h004}

    //------------------------------------------------------------------------------
    // If MPR pattern need change, Insert these settings (begin)
    // SW set MPR pattens. (pattern can be changed here!)
    // Set 'mpr0'     = 8'haa
    // Set 'mpr1'     = 8'h55
    // Set 'mpr2'     = 8'hf0
    // Set 'mpr3'     = 8'h00
    // Set 'mpr_mode' = 2'h2
    //--------------------------------------------------------------------------------
    //Program MPR0 register
    single_cmd(u16RegMiuDtop, 0x00aa, 0x04);  //{3'b000,5'b00000,mpr0}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Program MPR1 register
    single_cmd(u16RegMiuDtop, 0x20cc, 0x04); //{3'b001,5'b00000,mpr1}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Program MPR2 register
    single_cmd(u16RegMiuDtop, 0x40f0, 0x04); //{3'b010,5'b00000,mpr2}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Program MPR3 register
    single_cmd(u16RegMiuDtop, 0x6000, 0x04); //{3'b011,5'b00000,mpr3}
    delayus(1); //wait tWR_MPR(~=25ns)

    //Enable BIST MPR check mode
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34, 0x8000);    //reg_mpr_test_on = 1
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0001);    //reg_test_base = MPRx start
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0021);    //reg_test_length = MPR test length

    // Loop begin
    Start[0] = 0;
    u8Cnt    = 0;
    u8Cnt1   = 0;
    for (i = 0; i < 32; i = i + 4)
    {
        u16RegVal0 = (i + (i << 8)) & 0x1f1f;

        MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, u16RegVal0);
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, u16RegVal0);

        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f);                //reg_dqsm_rst_sel = 1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005);                //reg_dqsm_rst_sel = 0
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x02);        //reg_rx_ff_rst = 1
        MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & ~0x02);       //reg_rx_ff_rst = 0

        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0180);
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0181);

        if ((MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0) & 0xE000) == 0x8000)
        {
            if (u8Cnt == 0)
            {
                u8Cnt1 = 0;
                Start[0] = i;
            }
            u8Cnt++;
            u8Cnt1++;
        }
        else
        {
            u8Cnt = 0;
        }
    }

    u16RegVal0 =  (Start[0] + (u8Cnt1 << 2)) >> 1;
    u16RegVal0 = u16RegVal0 + (u16RegVal0 << 8);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, u16RegVal0 & 0x1f1f);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, u16RegVal0 & 0x1f1f);

    //Disable MPR Mode
    single_cmd(u16RegMiuDtop, 0x6000, 0x00);            //[12:9] : reg_single_cmd =  0 (MR setting)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34 ,0x0000);    //reg_mpr_test_on = 0
    delayus(1); //wait tWR_MPR(~=25ns)

    /* STEP3. Update trigger level */
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20); //[05] : reg_auto_ref_off = 0
#endif

    /* STEP4. VrefDQ training   (determine optimized DRAM VredDQ value) */
    //DPAT engine pre-setting (DQ TX auto-phase)
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x8001);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0084);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0086);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) & ~0x01);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0010);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c61);
    //MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x7c, 0x8009);


#ifdef VREF_DQ_EN

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, (MHal_MIU_ReadReg16(u16RegMiuArb, 0x64) & 0xff00) | (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff00));       //Copy From 0x161565[7:0] to 0x101219[7:0]

    // Fine tune -->step :  +/- 2 codes twice
    u16RegVal = 4;//TDB
    u16RegVal = VrefDQ_train_loop(u16RegMiuDtop, u16RegMiuArb, u16RegVal-4, 1, 10);


    //SW need calculate Vref_Val = The Vref_val that have Max. minimum margin
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000); //[05] : reg_auto_ref_off = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20); //[05] : reg_auto_ref_off = 1

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
    delayus(1); //wait 150ns for tVREFDQE

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal]  | 0x80, 0x00);
    delayus(1); //wait 150ns for tVREFDQE

    //Exit DQ Vref training Mode.
    single_cmd(u16RegMiuDtop, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0x80, 0x00);                    // [7]= 0 (Disable VrefDQ training)
    delayus(1); //wait 150ns for tVREFDQE

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);           //[05] : reg_auto_ref_off = 0
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) & ~0x1000);         //[12] : reg_cke_always_on = 0
#endif

#ifdef DDR4_MIU_ENABLE_AUTO_WRITE_PHASE

    //STEP5.Enable TX DQ Deskew test (determine optimized TX DQ phase)
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) | 0x01);            //reg_si_mode = 1
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x04);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x00);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc8, 0x00);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xca, 0x00);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xcc, 0x00);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x02);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x13);

    while((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x78) & 0x8000) == 0x0);

    //STEP6. Read TX deskew result & Write to REGs
    //TX deskew result write to REGs
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0004);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ3,DQ2,DQ1,DQ0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ7,DQ6,DQ5,DQ4}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd2, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {DQM3,DQM2,DQM1,DQM0}
    u16RegVal1 = u16RegVal0 & 0x00ff;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd8, u16RegVal1);
    u16RegVal1 = (u16RegVal0 & 0xff00) >> 8;
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe8, u16RegVal1);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0005);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ11,DQ10,DQ9,DQ8}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ15,DQ14,DQ13,DQ12}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xd6, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc6);   //reg_rdata = 0x110dc6[15:0] = {REGION3,REGION2,REGION1,REGION0}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x2e, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0006);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ19,DQ18,DQ17,DQ16}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe0, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ23,DQ22,DQ21,DQ20}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe2, u16RegVal0);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0xc0, 0x0007);
    delayus(1);

    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc2);   //reg_rdata = 0x110dc2[15:0] = {DQ27,DQ26,DQ25,DQ24}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe4, u16RegVal0);
    u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuAtop, 0xc4);   //reg_rdata = 0x110dc4[15:0] = {DQ31,DQ30,DQ29,DQ28}
    MHal_MIU_WritReg16(u16RegMiuAtop, 0xe6, u16RegVal0);
#endif

    //STEP7. Trigger level training   (determine optimize trigger level setting)

#ifdef DDR4_READ_DBI_EN
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x34, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0x34) | 0x80);   //reg_dbi_bit_test_en=1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x34) | 0x80);
#endif

    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);      //test length for RX
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0400);        //test length for RX
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0090);      //test length for RX
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);      //length
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0080);      //test mode
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);      //test mode
    MHal_MIU_WritReg16(u16RegMiuDtop, 0xd4, MHal_MIU_ReadReg16(u16RegMiuDtop, 0xd4) | 0x01);    //reg_si_mode = 1
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x00);      //reg_test_base
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2000);     //pass condition window extend to 5 phase (reg_cont_pass_num = 2) , No jump scan (reg_rx_ph_scan_step = 0)

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);

#ifdef TRIGGER_LEVEL_EN
    Start[0]=(MS_U8) MHal_MIU_ReadReg16(u16RegMiuAtop, 0x74);
    if (Start[0] > 4)
    {
        Start[0] -= 4;
    }
    else
    {
    Start[0]=0;
    }
    Start[1]=(MS_U8) MHal_MIU_ReadReg16(u16RegMiuAtop, 0x76);
    if (Start[1] > 4)
    {
        Start[1] -= 4;
    }
    else
    {
    Start[1]=0;
    }
    u32RegVal = trigger_level_trian_loop(u16RegMiuAtop, u16RegMiuArb, Start, 1, 9);

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x74, (u32RegVal & 0xff) | ((u32RegVal & 0xff) << 8));
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x76, ((u32RegVal >> 8) & 0xff) | (((u32RegVal >> 8) & 0xff) << 8));

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x000f);                  //reg_dqsm_rst_sel = 1
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1e, 0x0005);                  //reg_dqsm_rst_sel = 0
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) | 0x02);        //reg_rx_ff_rst = 1
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x1c, MHal_MIU_ReadReg16(u16RegMiuAtop, 0x1c) & ~0x02);       //reg_rx_ff_rst = 0
#endif

#ifdef MIU_ENABLE_AUTO_READ_PHASE
    //STEP8. Enable RX DQ/DQS deskew   (determine optimized RX per-bit deskew phase)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0004);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f0);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    while((MHal_MIU_ReadReg16(MIU0_RIU_ARB, 0x78) & 0x8000) == 0x0);

    // Read RX DQ deskew result & write to REGs
    RX_DQS_flow(u8MiuDev);
#endif

    //STEP10. Set DQS Phase (K/2 + offset)
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);            //[11] reg_dline_gated_control  = 0
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);             //[0]  reg_dpat_en = 0
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0c60);             //[0]  reg_live_test_en = 0
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);            //[11] reg_dline_gated_control  = 1

    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);

    /* Disable SI mode */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

#ifdef DDR4_READ_DBI_EN
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x34, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0x34) & ~0x80);   //reg_dbi_bit_test_en=0
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x34, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x34) & ~0x80);
#endif

    _PUTS("MIU");
    putn(u8MiuDev);
    _PUTS("_DQS-OK\n\r");
}

#define BIST_LEN    0x1001

void _MIU_BIST_FUNC Miu_BistTest(MS_U8 u8MIU)
{

    MS_U16 u16RegVal0;
    MS_U16 u16TestAddr0 = 0;
    MS_U16 u16TestAddr1 = 0;

#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
#if !defined(CONFIG_RecoverDQSSettingFromPMReg)
    u16TestAddr0 = RREG16(REG_ADDR_BASE + (0x003fb8<<1)) + (RREG16(REG_ADDR_BASE + (0x003fba<<1)) << 8);
    u16TestAddr1 = RREG16(REG_ADDR_BASE + (0x003fc8<<1)) + (RREG16(REG_ADDR_BASE + (0x003fca<<1)) << 8);
#endif
#endif

    if(u8MIU == 0)
    {
        //unmask MIU0_group_4 bit15
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x06, 0x7FFF);
    }
    else
    {
        //unmask MIU1_group_4 bit15
        MHal_MIU_WritReg16(MIU1_RIU_ARB, 0x06, 0x7FFF);
    }
    //1012e0 reset bist test
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);

    if(u8MIU == 0)
    {
        //1012f8 bit_15=0 choice to MIU_0
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    }
    else
    {
        //1012f8 bit_15=1 choice to MIU_1
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    }

    //1620f8 bit_15=0 choice to MIU_0
    MHal_MIU_WritReg16(0x1620, 0xf8, 0x0000);

    //1012e2 //start address base unit:4 k bytes

    if (u8MIU == 0)
    {
#if defined(CONFIG_MSTAR_CURRY)
        u16TestAddr0 = MHal_MIU_ReadReg16(MIU0_RIU_ATOP, 0xce); // for 1to4 16bit
#endif
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, u16TestAddr0);
    }
    else
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, u16TestAddr1);
    }

    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, AUTO_PHASE_ADDR);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, BIST_LEN);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, BIST_LEN >> 16);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe8, 0x5aa5);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0002);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0003);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xe0);
    }while((u16RegVal0 & 0x8000) == 0);

    if ((u16RegVal0 & 0x6000) == 0)
    {
        if(u8MIU == 0)
        {
            _PUTS("BIST_0 OK\n\r");
        }
        else
        {
            _PUTS("BIST_1 OK\n\r");
        }
    }
    else
    {
        if(u8MIU == 0)
        {
            _PUTS("BIST_0 Fail\n\r");
        }
        else
        {
            _PUTS("BIST_1 Fail\n\r");

        }
        while(1);
    }

}

void _MIU_TRIM_FUNC ATOP_Trim_Calculate(MS_U16 *u16data, MS_U32 otp_date)
{
    MS_U16 temp = *u16data;

    // do mapping
    if (temp == 14 || temp == 15)
    {
        temp += 10;
    }
    else if (temp == 17 || temp == 16)
    {
        temp -= 10;
    }

    // calculate
    switch(otp_date)
    {
        case 0x6:
            temp += 2;
            break;
        case 0x5:
            temp += 1;
            break;
        case 0x1:
            temp -= 1;
            break;
        case 0x2:
            temp -= 2;
            break;
        default:
            break;
    }

    *u16data = temp;
}

void _MIU_TRIM_FUNC ATOP_Trim_Calculate_v2(MS_U16 *u16data, MS_U32 otp_date)
{
    MS_U16 temp = *u16data;

    // calculate
    switch (otp_date)
    {
        case 0x7:
            temp = temp > 0x1C ? 0x1F : temp+3;
            break;

        case 0x6:
            temp = temp > 0x1D ? 0x1F : temp+2;
            break;

        case 0x5:
            temp = temp > 0x1E ? 0x1F : temp+1;
            break;

        case 0x1:
            temp = temp < 0x1 ? 0x0 : temp-1;
            break;

        case 0x2:
            temp = temp < 0x2 ? 0x0 : temp-2;
            break;

        case 0x3:
            temp = temp < 0x3 ? 0x0 : temp-3;
            break;

        default:
            break;
    }

    *u16data = temp;
}

void _MIU_TRIM_FUNC MIU_ATOP_Trim_Reg_Setup(MS_U64 u64Reg0, MS_U64 u64Reg1, MS_U8 u8offset0, MS_U8 u8offset1, MS_U32 u32otpvalue)
{
    MS_U16 data = 0;
    MS_U16 data1 = 0;

    data = (*(volatile MS_U16*)(u64Reg0) & (0xf<<u8offset0)) >> u8offset0;
    if(u8offset1 < 4)
    {
        data |= (*(volatile MS_U16*)(u64Reg1) & (0x1<<u8offset1)) << (4-u8offset1);
    }
    else
    {
        data |= (*(volatile MS_U16*)(u64Reg1) & (0x1<<u8offset1)) >> (u8offset1-4);
    }

#if defined(CONFIG_MSTAR_CURRY)
    ATOP_Trim_Calculate_v2(&data, u32otpvalue);
#else
    ATOP_Trim_Calculate(&data, u32otpvalue);
#endif

    data1 = *(volatile MS_U16*)(u64Reg0);
    data1 = (data1 & ~(0xf<<u8offset0)) | ((data & 0xf) << u8offset0);
    *(volatile MS_U16*)(u64Reg0) = data1;

    data1 = *(volatile MS_U16*)(u64Reg1);
    if(u8offset1 < 4)
    {
        data1 = (data1 & ~(0x1<<u8offset1)) | ((data & 0x10) >> (4-u8offset1));
    }
    else
    {
        data1 = (data1 & ~(0x1<<u8offset1)) | ((data & 0x10) << (u8offset1-4));
    }
    *(volatile MS_U16*)(u64Reg1) = data1;
}

void _MIU_TRIM_FUNC MIU_ATOP_Trim(E_DDR_TYPE eDRAM_Type)
{
    MS_U32 otp = 0;
    MS_U64 atop0_reg_2c = 0x1F000000 + (0x110d00 << 1) + (0x2c << 2);
    MS_U64 atop0_reg_2d = 0x1F000000 + (0x110d00 << 1) + (0x2d << 2);
    MS_U64 atop0_reg_2e = 0x1F000000 + (0x110d00 << 1) + (0x2e << 2);
    MS_U64 atop0_reg_2f = 0x1F000000 + (0x110d00 << 1) + (0x2f << 2);
    MS_U64 atop0_reg_6f = 0x1F000000 + (0x110d00 << 1) + (0x6f << 2);
    MS_U64 atop1_reg_2c = 0x1F000000 + (0x161600 << 1) + (0x2c << 2);
    MS_U64 atop1_reg_2d = 0x1F000000 + (0x161600 << 1) + (0x2d << 2);
    MS_U64 atop1_reg_2e = 0x1F000000 + (0x161600 << 1) + (0x2e << 2);
    MS_U64 atop1_reg_2f = 0x1F000000 + (0x161600 << 1) + (0x2f << 2);
    MS_U64 atop1_reg_6f = 0x1F000000 + (0x161600 << 1) + (0x6f << 2);

    // DDR_ATOP
    // MIU0 drvp
    #if defined(CONFIG_MSTAR_CURRY)
        if (eDRAM_Type == E_DDR_3)
            otp = (READ_OTP_8(0x3CE9) & 0x80) >> 7;
        else
            otp = (READ_OTP_8(0x3CEA) & 0x80) >> 7;
    #else
        otp = (READ_OTP_8(0x3CE9) & 0x80) >> 7;
    #endif

    if (otp)
    {
        //drvp_cs
        #if defined(CONFIG_MSTAR_CURRY)
            if (eDRAM_Type == E_DDR_3)
                otp = (READ_OTP_8(0x3CE9) & 0x70) >> 4;
            else
                otp = (READ_OTP_8(0x3CEA) & 0x70) >> 4;
        #else
            otp = (READ_OTP_8(0x3CE9) & 0x70) >> 4;
        #endif

        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2c, atop0_reg_6f, 8, 14, otp);

        //drvp_cmd
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2f, atop0_reg_6f, 8, 12, otp);

        //drvp_dqs
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2f, atop0_reg_6f, 0, 8, otp);

        //drvp_dq
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2f, atop0_reg_6f, 4, 10, otp);

        //drvp_mclk
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2f, atop0_reg_6f, 12, 13, otp);

        //drvp_dqs_h
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2d, atop0_reg_6f, 8, 9, otp);

        //drvp_dq_h
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2d, atop0_reg_6f, 12, 11, otp);
    }

    // MIU0 drvn
    #if defined(CONFIG_MSTAR_CURRY)
        if (eDRAM_Type == E_DDR_3)
            otp = (READ_OTP_8(0x3CE9) & 0x8) >> 3;
        else
            otp = (READ_OTP_8(0x3CEA) & 0x8) >> 3;
    #else
        otp = (READ_OTP_8(0x3CE9) & 0x8) >> 3;
    #endif

    if (otp)
    {
        //drvn_cs
        #if defined(CONFIG_MSTAR_CURRY)
            if (eDRAM_Type == E_DDR_3)
                otp = (READ_OTP_8(0x3CE9) & 0x7);
            else
                otp = (READ_OTP_8(0x3CEA) & 0x7);
        #else
            otp = (READ_OTP_8(0x3CE9) & 0x7);
        #endif

        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2c, atop0_reg_6f, 0, 6, otp);

        ///drvn_cmd
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2e, atop0_reg_6f, 8, 4, otp);

        //drvn_dqs
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2e, atop0_reg_6f, 0, 0, otp);

        //drvn_dq
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2e, atop0_reg_6f, 4, 2, otp);

        //drvn_mclk
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2e, atop0_reg_6f, 12, 5, otp);

        //drvn_dqs_h
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2d, atop0_reg_6f, 0, 1, otp);

        //drvn_dq_h
        MIU_ATOP_Trim_Reg_Setup(atop0_reg_2d, atop0_reg_6f, 4, 3, otp);
    }

#if defined(CONFIG_MSTAR_KANO)
    // MIU1 drvp
    if(u16UseMIU1)
    {
        otp = (READ_OTP_8(0x3CE8) & 0x80) >> 7;
        if (otp)
        {
            //drvp_cs
            otp = (READ_OTP_8(0x3CE8) & 0x70) >> 4;
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2c, atop1_reg_6f, 8, 14, otp);

            ///drvp_cmd
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2f, atop1_reg_6f, 8, 12, otp);

            //drvp_dqs
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2f, atop1_reg_6f, 0, 8, otp);

            //drvp_dq
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2f, atop1_reg_6f, 4, 10, otp);

            //drvp_mclk
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2f, atop1_reg_6f, 12, 13, otp);

            //drvp_dqs_h
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2d, atop1_reg_6f, 8, 9, otp);

            //drvp_dq_h
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2d, atop1_reg_6f, 12, 11, otp);
        }

        // MIU1 drvn
        otp = (READ_OTP_8(0x3CE8) & 0x08) >> 3;
        if (otp)
        {
            //drvn_cs
            otp = (READ_OTP_8(0x3CE8) & 0x7);
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2c, atop1_reg_6f, 0, 6, otp);

            ///drvn_cmd
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2e, atop1_reg_6f, 8, 4, otp);

            //drvn_dqs
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2e, atop1_reg_6f, 0, 0, otp);

            //drvn_dq
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2e, atop1_reg_6f, 4, 2, otp);

            //drvn_mclk
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2e, atop1_reg_6f, 12, 5, otp);

            //drvn_dqs_h
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2d, atop1_reg_6f, 0, 1, otp);

            //drvn_dq_h
            MIU_ATOP_Trim_Reg_Setup(atop1_reg_2d, atop1_reg_6f, 4, 3, otp);
        }
    }
#endif

}
#endif

static void _InitParam_End(MS_U8 u8POWER_Flag, E_DDR_TYPE eDRAM_Type)
{
#if defined(CONFIG_MSTAR_K6)
    chip_InitParam_End(u8POWER_Flag,eDRAM_Type);
#elif defined(CONFIG_MSTAR_CURRY)
#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    MS_U16 u16Regdata = 0;

    // Get MIU1 initial status to decide use MIU1 or not
    u16UseMIU1 = (*(volatile MS_U16*)(0x1F200000 + (0x0600 << 1))) & 0x1;

    _MIU_Check_Status();

    // MIU auto DQS can't work when DRAM OBF has enabled, so it need use bypass to avoid this problem
    // Bypass MIU0 DRAM OBF
    u16Regdata = (*(volatile MS_U16*)(0x1F200000 + 0x25b0));
    u16Regdata |= 0x200;
    (*(volatile MS_U16*)(0x1F200000 + 0x25b0)) = u16Regdata;

#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
#if defined(CONFIG_RecoverDQSSettingFromPMReg)
    if (u8POWER_Flag == AC_ON)
    {
        if(eDRAM_Type == E_DDR_4)
            BootRom_MiuDdr4Dqs(0);
        else
            BootRom_MiuDdr3Dqs_v2(0);
    }else if (u8POWER_Flag == DC_ON) //Restore auto phase miu setting
    {
        BootRom_RecoverDQSSetting();
    }
#else
    if (u8POWER_Flag == AC_ON)
    {
        if(eDRAM_Type == E_DDR_4)
            BootRom_MiuDdr4Dqs(0);
        else
            BootRom_MiuDdr3Dqs_v2(0);
    }
#endif
#endif

    // Close Bypass for MIU0 DRAM OBF
    u16Regdata = (*(volatile MS_U16*)(0x1F200000 + 0x25b0));
    u16Regdata &= ~0x200;
    (*(volatile MS_U16*)(0x1F200000 + 0x25b0)) = u16Regdata;

    DelayMS(1);

    MIU_ATOP_Trim(eDRAM_Type);

    if (u8POWER_Flag == AC_ON)
    {
        Miu_BistTest(0);
        if(u16UseMIU1)
        {
            Miu_BistTest(1);
        }
    }

#else //CONFIG_MSTAR_TITANIA_BD_FPGA

    if (u8POWER_Flag == AC_ON)
    {
        _MIU_Bist();
    }

#endif
    _MIU_ClearIPMask();

#else //CONFIG_MSTAR_KANO

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    MS_U16 u16Regdata = 0;

    // Get MIU1 initial status to decide use MIU1 or not
    u16UseMIU1 = (*(volatile MS_U16*)(0x1F200000 + (0x0600 << 1))) & 0x1;

    _MIU_Check_Status();

    MIU_ATOP_Trim(eDRAM_Type);

    // MIU auto DQS can't work when DRAM OBF has enabled, so it need use bypass to avoid this problem
    // Bypass MIU0 DRAM OBF
    u16Regdata = (*(volatile MS_U16*)(0x1F200000 + 0x25b0));
    u16Regdata |= 0x200;
    (*(volatile MS_U16*)(0x1F200000 + 0x25b0)) = u16Regdata;

    // Bypass MIU1 DRAM OBF
    if(u16UseMIU1)
    {
        u16Regdata = (*(volatile MS_U16*)(0x1F200000 + 0xdb0));
        u16Regdata |= 0x200;
        (*(volatile MS_U16*)(0x1F200000 + 0xdb0)) = u16Regdata;
    }

    if (u8POWER_Flag == AC_ON)
    {
        // Setup auto phase buffer address to 0x00000000 at default
        WREG8(REG_ADDR_BASE + (0x003fb8<<1), 0x00);
        WREG8(REG_ADDR_BASE + (0x003fba<<1), 0x00);
        WREG8(REG_ADDR_BASE + (0x003fc8<<1), 0x00);
        WREG8(REG_ADDR_BASE + (0x003fca<<1), 0x00);
    }

#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
    #if defined(CONFIG_RecoverDQSSettingFromPMReg)
    if (u8POWER_Flag == AC_ON)
    {
        BootRom_MiuDdrDqs(0, eDRAM_Type);
        if(u16UseMIU1)
        {
            BootRom_MiuDdrDqs(1, eDRAM_Type);
        }
    }else if (u8POWER_Flag == DC_ON) //Restore auto phase miu setting
    {
        BootRom_RecoverDQSSetting();
    }
    #else
    BootRom_MiuDdrDqs(0, eDRAM_Type);
    if(u16UseMIU1)
    {
        BootRom_MiuDdrDqs(1, eDRAM_Type);
    }
    #endif
#endif

    // Close Bypass for MIU0 DRAM OBF
    u16Regdata = (*(volatile MS_U16*)(0x1F200000 + 0x25b0));
    u16Regdata &= ~0x200;
    (*(volatile MS_U16*)(0x1F200000 + 0x25b0)) = u16Regdata;

    // Close Bypass for MIU1 DRAM OBF
    if(u16UseMIU1)
    {
        u16Regdata = (*(volatile MS_U16*)(0x1F200000 + 0xdb0));
        u16Regdata &= ~0x200;
        (*(volatile MS_U16*)(0x1F200000 + 0xdb0)) = u16Regdata;
    }

    DelayMS(1);

    if (u8POWER_Flag == AC_ON)
    {
        Miu_BistTest(0);
        if(u16UseMIU1)
        {
            Miu_BistTest(1);
        }
    }
    else if (u8POWER_Flag == DC_ON)
    {
#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
    #if !defined(CONFIG_RecoverDQSSettingFromPMReg)
        Miu_BistTest(0);
        if(u16UseMIU1)
        {
            Miu_BistTest(1);
        }
    #endif
#endif
    }

#else //CONFIG_MSTAR_TITANIA_BD_FPGA
    if (u8POWER_Flag == AC_ON)
    {
        _MIU_Bist();
    }

#endif //CONFIG_MSTAR_TITANIA_BD_FPGA
    _MIU_ClearIPMask();
#endif //if defined(CONFIG_MSTAR_K6)
}


MS_U32 BOOTROM_C_InitParams(void *pParams)
{
    MS_U64 rbase = (0x1f20 << 16);
    MS_U32 width = 16;
    MS_U8  u8POWER_Flag = AC_ON;
    E_DDR_TYPE eDRAM_Type = E_DDR_NA;
    gEntrys = pParams;

#if defined(CONFIG_HASH1_CMDS_MULTI_BLKS) && (CONFIG_HASH1_CMDS_MULTI_BLKS==1)
    MS_U8 u8DDR_Brand_Type = 0;
    MS_U8 bOutsideStartEndTag = 1;

    #if defined(CONFIG_MSTAR_K6)
    //u8DDR_Brand_Type[1:0]= gpio7:gpio6
    u8DDR_Brand_Type = ((RREG16(REG_ADDR_BASE + (0x1025be <<1))&1)<<1) | (RREG16(REG_ADDR_BASE + (0x1025bc <<1))&1);
    #endif
#endif

#if CONFIG_MSTAR_STR_ENABLE

    if ((RREG16(STR_IS_AC_FLOW_CONTROL) & 0xFF00) == 0x0100 ||
        (RREG16(STR_IS_AC_FLOW_CONTROL) & 0xFF00) == 0x0200)
    {
        u8POWER_Flag = DC_ON;
        _PUTS("SPI STR ON\n\r");

    }
    else
    {
        u8POWER_Flag = AC_ON;
        _PUTS("SPI AC ON\n\r");
    }

#else
    u8POWER_Flag = AC_ON;
#endif

    while(1)
    {
        const Entry *eCur = _Next();
        if(eCur == NULL)
            break;

        switch (eCur->cmd.param1)
        {
            case CHIPCFG_CMD_IF:
                {
                    volatile MS_U16 *addr = (void *)(rbase + eCur->cmd.param2);
                    const Entry *eCmp = _Next();
                    if(eCmp == NULL)
                    {
                        WREG_DBG(0xF002, 1);
                        return 2;
                    }

                    MS_U16 value = eCmp->cmd.param1;
                    MS_U16 mask = eCmp->cmd.param2;

                    if((*addr & mask) != value)
                    {
                        // skip to CHIPCFG_CMD_ENDIF
                        const Entry *e;
                        do
                        {
                            e = _Next();
                            if(e == NULL)
                            {
                                WREG_DBG(0xF003, 1);
                                return 3;
                            }
                        } while(e->cmd.param1 != CHIPCFG_CMD_ENDIF);
                    }
                }
                break;

            case CHIPCFG_CMD_ENDIF:
                // ignore
                break;

            case CHIPCFG_CMD_IF_MUL:
                {
                    MS_U16 index = 0;
                    MS_U16 num = eCur->cmd.param2;
                    volatile MS_U16 *addr;
                    MS_U8  value;
                    MS_U8  mask;
                    MS_BOOL cond = TRUE;

                    //check each condition is TRUE or FALSE
                    do{
                        const Entry *eCmp = _Next();
                        if(eCmp == NULL)
                        {
                            WREG_DBG(0xF002, 1);
                            return 2;
                        }

                        addr  = (void *)(rbase + eCmp->cmd.param1);
                        value = eCmp->cmd.param2 >> 8 & 0xFF;
                        mask  = eCmp->cmd.param2 >> 0 & 0xFF;
                        if((*addr & mask) != value)
                        {
                            cond = FALSE;
                            break;
                        }
                        index++;
                    }while(index < num);

                    // if condition is FALSE, skip to ENDIF
                    if(cond == FALSE)
                    {
                        const Entry *e;
                        do
                        {
                            e = _Next();
                            if(e == NULL)
                            {
                                WREG_DBG(0xF003, 1);
                                return 3;
                            }
                        } while(e->cmd.param1 != CHIPCFG_CMD_ENDIF_MUL);
                    }
                }
                break;

            case CHIPCFG_CMD_ENDIF_MUL:
                // ignore
                break;

            case CHIPCFG_CMD_BASE:
                rbase = (eCur->cmd.param2 << 16);
                width = 16;
                break;

            case CHIPCFG_CMD_BASE_8BITS:
                rbase = (eCur->cmd.param2 << 16);
                width = 8;
                break;

            case CHIPCFG_CMD_WAIT:
                DelayMS(eCur->cmd.param2);
                break;

            case CHIPCFG_CMD_DBG:
                WREG_DBG(eCur->cmd.param2, 3);
                break;
           case CHIPCFG_STR_IFAC:
                 if (u8POWER_Flag == DC_ON)
                 {
                     const Entry *e;
                     do
                     {
                         e = _Next();
                         if(e == NULL)
                         {
                             WREG_DBG(0xF003, 1);
                             return 3;
                         }
                     } while(e->cmd.param1 != CHIPCFG_STR_IFAC_END);
                 }
                 break;

             case CHIPCFG_STR_IFAC_END:
                  // ignore
                  break;

             case CHIPCFG_STR_IFDC:
                 if (u8POWER_Flag == AC_ON)
                 {
                     const Entry *e;
                     do
                     {
                         e = _Next();
                         if(e == NULL)
                         {
                             WREG_DBG(0xF003, 1);
                             return 3;
                         }
                     } while(e->cmd.param1 != CHIPCFG_STR_IFDC_END);
                 }
                 break;

            case CHIPCFG_STR_IFDC_END:
                  // ignore
                  break;

#if defined(CONFIG_HASH1_CMDS_MULTI_BLKS) && (CONFIG_HASH1_CMDS_MULTI_BLKS==1)
            case CHIPCFG_CMD_TAG_START:
                 // Compare TAG cmd block value with the value of DDR type
                 // (if different seek and go to the end of this block)
                 bOutsideStartEndTag = 0;
                 if(eCur->cmd.param2 != u8DDR_Brand_Type)
                 {
                    bOutsideStartEndTag = 1;
                    do {
                        eCur = _Next();// increment pointer to next couple;
                        //if(eCur->cmd.param1 == CHIPCFG_CMD_END)
                       // Exit;// error!
                    } while (eCur->cmd.param1 != CHIPCFG_CMD_TAG_END);
                 }

                 //break;
            case CHIPCFG_CMD_TAG_END:
                 bOutsideStartEndTag = 1;
                 break;
            case CHIPCFG_CMD_DDR_SEL:
                 // only set ddr_flow_sel once, and can't be between TAG_START/END
                 if( (eDRAM_Type == E_DDR_NA) && bOutsideStartEndTag )
                 {
                    //u8DDRFlowSel = eCur->cmd.param2;

                    if(eCur->cmd.param2 == HASH1_DDR3_MAGIC)
                    {
                        eDRAM_Type = E_DDR_3;
                    }
                    else if(eCur->cmd.param2 == HASH1_DDR4_MAGIC)
                    {
                        eDRAM_Type = E_DDR_4;
                    }
                 }
                 break;
#endif
            default:
                if(width == 8)
                {
                    MS_U8 old = RREG8(rbase + eCur->reg.addr);
                    MS_U8 val = (eCur->reg.value >> 8) & 0xff;
                    MS_U8 mask = eCur->reg.value & 0xff;

                    WREG8(rbase + eCur->reg.addr, (old & ~mask) | (val & mask));
                }
                else
                {
                    WREG16(rbase + eCur->reg.addr, eCur->reg.value);
                }
                break;
        }
    }

    _InitParam_End(u8POWER_Flag,eDRAM_Type);
    WREG_DBG(0xF001, 1);
    return 1;
}


