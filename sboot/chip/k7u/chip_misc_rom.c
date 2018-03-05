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

#if defined(MIU1_LEN) && (MIU1_LEN != 0)
#define MIU_NUMBERS 2
#else
#define MIU_NUMBERS 1
#endif

#ifdef CONFIG_ARMv8_64BIT_SBOOT
typedef MS_U64 REG_PHY_LEN;
#else
typedef MS_U32 REG_PHY_LEN;
#endif

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

#define MIU_ENABLE_AUTO_WRITE_PHASE
#define MIU_ENABLE_AUTO_READ_PHASE
#define MIU_ENABLE_VREF_DQ

#define MHal_MIU_ReadReg16(u32bank, u32reg ) *( ( volatile MS_U16* ) (REG_PHY_LEN)(MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1)) )
#define MHal_MIU_WritReg16(u32bank, u32reg, u16val ) *( ( volatile MS_U16* ) (REG_PHY_LEN)(MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1))  ) = (u16val)

#define READ_OTP_8(x) (*(volatile MS_U8*)(0x1F320000 + (x)))
#define READ_OTP_16(x) (*(volatile MS_U16*)(0x1F320000 + (x)))

static MS_U16 u16UseMIU1 = 0;

static const MS_U32 VREF_VAL[10] = {0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e};

enum 
{
    AC_ON = 1,
    DC_ON = 2,
    
};

static void putn(MS_U8 n)
{
    char c = '0' + n;

   *(volatile unsigned int*)(0x1F201300) = c;
}

static void single_cmd(MS_U16 u16RegMiuDtop, MS_U16 mrx, MS_U8 CmdVal)
{
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, mrx); //Pre-charge all command
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x1e00) | (CmdVal << 9));   //[12:9] : reg_single_cmd =  2 (Pre-charge)
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) | 0x0100);                      //[8] : reg_single_cmd_en = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x0100);                     //[8] : reg_single_cmd_en = 0
}

static void delayus(MS_U32 us)
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
static MS_U16 VrefDQ_train_loop(MS_U16 u16RegMiuDtop, MS_U16 u16RegMiuArb, MS_U8 Indx, MS_U8 step, MS_U8 max)
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

static void BootRom_RecoverDQSSetting(void)
{
    //rtc0
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x4a<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x01<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x50<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x02<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x51<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x03<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x52<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x04<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x53<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x05<<1))<<1)));
    WREG16(REG_ADDR_BASE + ((0x110D00+(0x68<<1))<<1),RREG16(REG_ADDR_BASE + ((0x001200 + (0x06<<1))<<1)));

    //
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x69<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f80<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x69<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f82<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f84<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f86<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6b<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f88<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6b<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f8a<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x6c<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f8c<<1)));
    WREG8(REG_ADDR_BASE + ((0x110D00+(0x17<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f8e<<1)));

    WREG8(REG_ADDR_BASE + ((0x161600+(0x4a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f90<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x4a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f92<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x50<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f94<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x50<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f96<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x51<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f98<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x51<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f9a<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x52<<1))<<1),RREG16(REG_ADDR_BASE + (0x003f9c<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x52<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003f9e<<1)));

    WREG8(REG_ADDR_BASE + ((0x161600+(0x53<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa0<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x53<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fa2<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x68<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa4<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x68<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fa6<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x69<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fa8<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x69<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003faa<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x6a<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fac<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x6a<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fae<<1)));

    WREG8(REG_ADDR_BASE + ((0x161600+(0x6b<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb0<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x6b<<1))<<1) + 1,RREG16(REG_ADDR_BASE + (0x003fb2<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x6c<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb4<<1)));
    WREG8(REG_ADDR_BASE + ((0x161600+(0x17<<1))<<1),RREG16(REG_ADDR_BASE + (0x003fb6<<1)));
}

static void BootRom_MiuDdr3Dqs(MS_U8 u8MiuDev)
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
    MS_U32 u32KcodeOffsetValue;

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
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);

    /* Unmask MIU_group_0 bit15 */
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x46, 0x7fff);

    /* Enable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) | 0x01);

    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x0000);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x8000);
    }
    else
    {
        return;
    }

    /*Enable miu_test limit mask*/
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x5c, 0x8000);

    /* DPAT engine pre-setting */
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0000);

    //sw patch for 4x mode hang bug
    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x74, 0x0e61);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU1_RIU_ARB, 0x74, 0x0e61);
        MHal_MIU_WritReg16(MIU1_RIU_ARB, 0x7c, 0x8000);
    }
    else
    {
        return;
    }

#ifdef MIU_ENABLE_AUTO_READ_PHASE
    /* STEP 4 : Enable RX DQ/DQS deskew (by DPAT engine) */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c1);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c1);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2000);
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

    /* STEP 5 : Read RX DQ deskew result & write to REGs */
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

    u32KcodeOffsetValue = (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x96) << 16) | MHal_MIU_ReadReg16(u16RegMiuAtop, 0x94);
    for(i = 0; i < 4; i++)
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
#endif

    /* STEP 6 : Disable DPAT engine & Set DQS Phase = 1/2* Kcode+offset (ratio mode) */

    DelayMS(1); // wait about 500ns for Dummy write only worst pattern BIST time

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0e60);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    DelayMS(1); //-----Wait 5us, (wait a dll_fire_en)-----

    //disable miu_test limit mask
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x5c, 0x0000);

    //Comment this or MIU bist will be failed
    //u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 | 0x0800);

    /* Disable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

    _PUTS("MIU");
    putn(u8MiuDev);
    _PUTS("_DDR3_DQS-OK\n\r");
}

static void BootRom_MiuDdr4Dqs(MS_U8 u8MiuDev)
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
    MS_U8 i = 0;
    MS_U32 u32KcodeOffsetValue;

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
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0080);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x8001);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);

    // 9.23 add
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe8, 0x00ff);

    /* Unmask MIU_group_0 bit15 */
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x46, 0x7fff);

    /* Enable SI mode */
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) | 0x01);
    /* Disable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & (~0x01));

    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x0000);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x8000);
    }
    else
    {
        return;
    }

    // 9.23 add
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0xcc, 0x8110);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0xce, 0x0022);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0xce, 0x0023);

    /*Enable miu_test limit mask*/
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x5c, 0x8000);

    /* DPAT engine pre-setting */
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0000);

    //sw patch for 4x mode hang bug
    if (u8MiuDev == 0)
    {
        MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x74, 0x0e61);
    }
    else if (u8MiuDev == 1)
    {
        MHal_MIU_WritReg16(MIU1_RIU_ARB, 0x74, 0x0e61);
        MHal_MIU_WritReg16(MIU1_RIU_ARB, 0x7c, 0x8000);
    }
    else
    {
        return;
    }

#ifdef MIU_ENABLE_VREF_DQ

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x18, (MHal_MIU_ReadReg16(u16RegMiuArb, 0x64) & 0xff00) | (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff00));       //Copy From 0x161565[7:0] to 0x101219[7:0]

    // Fine tune -->step :  +/- 2 codes twice
    u16RegVal0 = 4;//TDB
    u16RegVal0 = VrefDQ_train_loop(u16RegMiuDtop, u16RegMiuArb, u16RegVal0-4, 1, 9);


    //SW need calculate Vref_Val = The Vref_val that have Max. minimum margin
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000); //[05] : reg_auto_ref_off = 1
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20); //[05] : reg_auto_ref_off = 1

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal0]  | 0x80, 0x00);
    delayus(1); //wait 150ns for tVREFDQE

    //Enable & Change DQ Vref
    //Set MRx[6:0] = Choose VrefDQ value & Range
    single_cmd(u16RegMiuDtop, (MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0xff) | VREF_VAL[u16RegVal0]  | 0x80, 0x00);
    delayus(1); //wait 150ns for tVREFDQE

    //Exit DQ Vref training Mode.
    single_cmd(u16RegMiuDtop, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x18) & ~0x80, 0x00);                    // [7]= 0 (Disable VrefDQ training)
    delayus(1); //wait 150ns for tVREFDQE

    MHal_MIU_WritReg16(u16RegMiuDtop, 0x00, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x00) & ~0x20);           //[05] : reg_auto_ref_off = 0
    //MHal_MIU_WritReg16(u16RegMiuDtop, 0x06, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x06) & ~0x1000);         //[12] : reg_cke_always_on = 0
#endif

#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    /* STEP 1 : Set RX DQS Phase = 1/2* Kcode + offset (SW ratio mode) */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    /* STEP 2 : Enable TX DQ deskew scan (by DPAT engine) */
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0090);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x04);       //dpat_rst
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x00);       //dpat_rst

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
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x2400);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c1);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x00c1);

    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x2000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0x00f0);
//    MHal_MIU_WritReg16(u16RegMiuAtop, 0x40, 0xffff);
//    MHal_MIU_WritReg16(u16RegMiuAtop, 0x42, 0xffff);
//    MHal_MIU_WritReg16(u16RegMiuAtop, 0x44, 0x000f);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x76, 0x0001);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0013);

    do{
        u16RegVal0 = MHal_MIU_ReadReg16(u16RegMiuArb, 0x78);
    }while((u16RegVal0 & 0x8000) == 0);

    /* STEP 5 : Read RX DQ deskew result & write to REGs */
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

    u32KcodeOffsetValue = (MHal_MIU_ReadReg16(u16RegMiuAtop, 0x96) << 16) | MHal_MIU_ReadReg16(u16RegMiuAtop, 0x94);
    for(i = 0; i < 4; i++)
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
#endif

    /* STEP 6 : Disable DPAT engine & Set DQS Phase = 1/2* Kcode+offset (ratio mode) */

    DelayMS(1); // wait about 500ns for Dummy write only worst pattern BIST time

    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0e60);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);

    DelayMS(1); //-----Wait 5us, (wait a dll_fire_en)-----

    //disable miu_test limit mask
    MHal_MIU_WritReg16(u16RegMiuDtop, 0x5c, 0x0000);

    //Comment this or MIU bist will be failed
    //u16RegVal0 = MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xfe);
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xfe, u16RegVal0 | 0x0800);

    /* Disable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, MHal_MIU_ReadReg16(MIU0_RIU_DTOP, 0xd4) & ~0x01);

    _PUTS("MIU");
    putn(u8MiuDev);
    _PUTS("_DDR4_DQS-OK\n\r");
}

static void MIU_ATOP_Trim_Reg_Process(REG_PHY_LEN u32Reg0, REG_PHY_LEN u32Reg1, MS_U8 u8offset0, MS_U8 u8offset1, MS_U32 u32otpvalue)
{
    MS_U16 u16Data, u16Temp;
    #define DRV_MAX 0x001F
    #define DRV_MIN 0x0000

    u16Temp = (((RREG16(u32Reg0)) >> u8offset0) & 0x0001);
    u16Data = ((u16Temp << 4) | (((RREG16(u32Reg1)) >> u8offset1) & 0x000f)) & 0x001f;

    switch(u32otpvalue)
    {
        case 0x7:
            if ((DRV_MAX - u16Data) < 6) u16Data = DRV_MAX;
            else    u16Data += 6;
            break;

        case 0x6:
            if ((DRV_MAX - u16Data) < 4) u16Data = DRV_MAX;
            else    u16Data += 4;
            break;

        case 0x5:
            if ((DRV_MAX - u16Data) < 2) u16Data = DRV_MAX;
            else    u16Data += 2;
            break;

        default:
        case 0x0:
            break;

        case 0x1:
            if (u16Data < 2) u16Data = DRV_MIN;
            else    u16Data -= 2;
            break;

        case 0x2:
            if (u16Data < 4) u16Data = DRV_MIN;
            else    u16Data -= 4;
            break;

        case 0x3:
            if (u16Data < 6) u16Data = DRV_MIN;
            else    u16Data -= 6;
            break;
    }

    u16Temp = (RREG16(u32Reg1)) & (~(0x000f << u8offset1));
    u16Temp = u16Temp | ((u16Data & 0x000f) << u8offset1);
    WREG16(u32Reg1, u16Temp);

    u16Temp = (RREG16(u32Reg0)) & (~(0x0001 << u8offset0));
    u16Temp = u16Temp | (((u16Data & 0x0010) >> 4) << u8offset0);
    WREG16(u32Reg0, u16Temp);
}

static void MIU_ATOP_Trim(E_DDR_TYPE eDRAM_Type)
{
    MS_U32 otp = 0;
    REG_PHY_LEN atop0_reg_2c = 0x1F000000 + (0x110d00 << 1) + (0x2c << 2);
    REG_PHY_LEN atop0_reg_2d = 0x1F000000 + (0x110d00 << 1) + (0x2d << 2);
    REG_PHY_LEN atop0_reg_2e = 0x1F000000 + (0x110d00 << 1) + (0x2e << 2);
    REG_PHY_LEN atop0_reg_2f = 0x1F000000 + (0x110d00 << 1) + (0x2f << 2);
    REG_PHY_LEN atop0_reg_6f = 0x1F000000 + (0x110d00 << 1) + (0x6f << 2);
    REG_PHY_LEN atop1_reg_2c = 0x1F000000 + (0x161600 << 1) + (0x2c << 2);
    REG_PHY_LEN atop1_reg_2d = 0x1F000000 + (0x161600 << 1) + (0x2d << 2);
    REG_PHY_LEN atop1_reg_2e = 0x1F000000 + (0x161600 << 1) + (0x2e << 2);
    REG_PHY_LEN atop1_reg_2f = 0x1F000000 + (0x161600 << 1) + (0x2f << 2);
    REG_PHY_LEN atop1_reg_6f = 0x1F000000 + (0x161600 << 1) + (0x6f << 2);

    // MIU_drvp_0
    if (eDRAM_Type == E_DDR_3)
        otp = (READ_OTP_8(0x3CE8) & 0x08) >> 3;
    else
        otp = (READ_OTP_8(0x3CEA) & 0x08) >> 3;

    if (otp)
    {
        if (eDRAM_Type == E_DDR_3)
            otp = (READ_OTP_8(0x3CE8) & 0x07);
        else
            otp = (READ_OTP_8(0x3CEA) & 0x07);

        // cs
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2c, 14, 8, otp); //cs: {0x110d_6F[14],0x110d_2C[11:8]}

        // dqh
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2d, 11, 12, otp); //dqh: {0x110d_6F[11],0x110d_2D[15:12]}

        // dqsh
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2d, 9, 8, otp); //dqsh: {0x110d_6F[9],0x110d_2D[12:8]}

        // clk
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2f, 13, 12, otp); //clk: {0x110d_6F[13],0x110d_2F[15:12]}

        // cmd
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2f, 12, 8, otp); //cmd: {0x110d_6F[12],0x110d_2F[11:8]}

        // dq
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2f, 10, 4, otp); //dq: {0x110d_6F[10],0x110d_2F[7:4]}

        // dqs
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2f, 8, 0, otp); //dqs: {0x110d_6F[8],0x110d_2F[3:0]}
    }

    // MIU_drvn_0
    if (eDRAM_Type == E_DDR_3)
        otp = (READ_OTP_8(0x3CE8) & 0x80) >> 7;
    else
        otp = (READ_OTP_8(0x3CEA) & 0x80) >> 7;

    if (otp)
    {
        if (eDRAM_Type == E_DDR_3)
            otp = (READ_OTP_8(0x3CE8) & 0x70) >> 4;
        else
            otp = (READ_OTP_8(0x3CEA) & 0x70) >> 4;

        // cs
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2c, 6, 0, otp); //cs: {0x110d_6F[6],0x110d_2C[3:0]}

        // dqh
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2d, 3, 4, otp); //dqh: {0x110d_6F[3],0x110d_2D[7:4]}

        // dqsh
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2d, 1, 0, otp); //dqsh: {0x110d_6F[1],0x110d_2D[3:0]}

        // clk
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2e, 5, 12, otp); //clk: {0x110d_6F[5],0x110d_2E[15:12]}

        // cmd
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2e, 4, 8, otp); //cmd: {0x110d_6F[4],0x110d_2E[11:8]}

        // dq
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2e, 2, 4, otp); //dq: {0x110d_6F[2],0x110d_2E[7:4]}

        // dqs
        MIU_ATOP_Trim_Reg_Process(atop0_reg_6f, atop0_reg_2e, 0, 0, otp); //dqs: {0x110d_6F[0],0x110d_2E[3:0]}
    }

    if(u16UseMIU1)
    {
        // MIU_drvp_1
        if (eDRAM_Type == E_DDR_3)
            otp = (READ_OTP_8(0x3CE9) & 0x08) >> 3;
        else
            otp = (READ_OTP_8(0x3CEB) & 0x08) >> 3;

        if (otp)
        {
            if (eDRAM_Type == E_DDR_3)
                otp = (READ_OTP_8(0x3CE9) & 0x07);
            else
                otp = (READ_OTP_8(0x3CEB) & 0x07);

            // cs
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2c, 14, 8, otp); //cs: {0x1616_6F[14],0x1616_2C[11:8]}

            // dqh
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2d, 11, 12, otp); //dqh: {0x1616_6F[11],0x1616_2D[15:12]}

            // dqsh
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2d, 9, 8, otp); //dqsh: {0x1616_6F[9],0x1616_2D[12:8]}

            // clk
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2f, 13, 12, otp); //clk: {0x1616_6F[13],0x1616_2F[15:12]}

            // cmd
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2f, 12, 8, otp); //cmd: {0x1616_6F[12],0x1616_2F[11:8]}

            // dq
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2f, 10, 4, otp); //dq: {0x1616_6F[10],0x1616_2F[7:4]}

            // dqs
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2f, 8, 0, otp); //dqs: {0x1616_6F[8],0x1616_2F[3:0]}
        }

        // MIU_drvn_1
        if (eDRAM_Type == E_DDR_3)
            otp = (READ_OTP_8(0x3CE9) & 0x80) >> 7;
        else
            otp = (READ_OTP_8(0x3CEB) & 0x80) >> 7;

        if (otp)
        {
            if (eDRAM_Type == E_DDR_3)
                otp = (READ_OTP_8(0x3CE9) & 0x70) >> 4;
            else
                otp = (READ_OTP_8(0x3CEB) & 0x70) >> 4;

            // cs
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2c, 6, 0, otp); //cs: {0x1616_6F[6],0x1616_2C[3:0]}

            // dqh
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2d, 3, 4, otp); //dqh: {0x1616_6F[3],0x1616_2D[7:4]}

            // dqsh
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2d, 1, 0, otp); //dqsh: {0x1616_6F[1],0x1616_2D[3:0]}

            // clk
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2e, 5, 12, otp); //clk: {0x1616_6F[5],0x1616_2E[15:12]}

            // cmd
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2e, 4, 8, otp); //cmd: {0x1616_6F[4],0x1616_2E[11:8]}

            // dq
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2e, 2, 4, otp); //dq: {0x1616_6F[2],0x1616_2E[7:4]}

            // dqs
            MIU_ATOP_Trim_Reg_Process(atop1_reg_6f, atop1_reg_2e, 0, 0, otp); //dqs: {0x1616_6F[0],0x1616_2E[3:0]}
        }
    }

}

#define BIST_LEN    0x1001

static void Miu_BistTest(MS_U8 u8MIU)
{

    MS_U16 u16RegVal0;

    if(u8MIU == 0)
    {
        //unmask MIU0_group_0 bit15
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x46, 0x7FFF);
    }
    else
    {
        //unmask MIU1_group_0 bit15
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x46, 0x7FFF);
    }
    //1012e0 reset bist test
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);

    if(u8MIU == 0)
    {
        //1012f0 bit_15=0 choice to MIU_0
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x0000);
    }
    else
    {
        //1012f0 bit_15=1 choice to MIU_1
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x8000);
    }

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000); //start address base unit

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, BIST_LEN);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, BIST_LEN >> 16);

    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe8, 0x5aa5);

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
        MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf0, 0x0000); // reset 0x1012f0 bit_15=0
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
    WREG16((REG_ADDR_BASE + (0x141006 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x141026 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x1615e6 <<1 )) , 0x0000);
    WREG16((REG_ADDR_BASE + (0x10121e <<1 )) , 0x8c08);

    if(u16UseMIU1)
    {
        // Unmask MIU1
        WREG16((REG_ADDR_BASE + (0x100646 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x100666 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x100686 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x1006a6 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x162206 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x162226 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x141106 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x141126 <<1 )) , 0x0000);
        WREG16((REG_ADDR_BASE + (0x1622e6 <<1 )) , 0x0000);
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


void chip_InitParam_End(MS_U8 u8POWER_Flag,E_DDR_TYPE eDRAM_Type)
{

    //eDRAM_Type wait to be the input of dqs ddr3/ddr4

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
        MS_U16 u16Regdata = 0;
    
        // Get MIU1 initial status to decide use MIU1 or not
        u16UseMIU1 = (*(volatile MS_U16*)(0x1F200000 + (0x0600 << 1))) & 0x1;
    
        _MIU_Check_Status();

#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
    if (u8POWER_Flag == DC_ON) //Restore auto phase miu setting
    {
        BootRom_RecoverDQSSetting();
    }
    else
    {

#if defined(CONFIG_MSTAR_SW_BOND_CHECK)
        MS_U8 u8DDR_Brand_Type = 0;

        u8DDR_Brand_Type = RREG16(REG_ADDR_BASE + (0x000ec8<<1))&0x7;
        // If u8DDR_Brand_Type == 0x0, it use external DRAM, need do autophase
        // If != 0x0, it is MCP package, doesn't need to do autophase
        if (u8DDR_Brand_Type == 0x0)
        {
#endif
            switch (eDRAM_Type)
            {
                case E_DDR_3:
                    BootRom_MiuDdr3Dqs(0);
                    if(u16UseMIU1)
                    {
                        BootRom_MiuDdr3Dqs(1);
                    }
                    break;
          
                case E_DDR_4:
                    BootRom_MiuDdr4Dqs(0);
                    if(u16UseMIU1)
                    {
                        BootRom_MiuDdr4Dqs(1);
                    }
                    break;
          
                default:
                case E_DDR_2:
                    _PUTS("Wrong DDR type - DQS Fail\n\r");
                   break;
           }
#if defined(CONFIG_MSTAR_SW_BOND_CHECK)
       }
#endif


    }
#endif    
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

}

