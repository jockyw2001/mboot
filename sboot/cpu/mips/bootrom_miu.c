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

#define REG_ADDR_BASE         (0xBF000000)
#define REG_MIU_BASE          (0xBF200000 + 0x2400)     // 0xbf202400: 0x101200
#define REG_MIU_TEST_CTRL     (REG_MIU_BASE + 0x70 * 4) // 0xbf2025c0: 0x1012e0
#define REG_MIU_TEST_BASE     (REG_MIU_BASE + 0x71 * 4) // 0xbf2025c4: 0x1012e2
#define REG_MIU_TEST_LENGTH_L (REG_MIU_BASE + 0x72 * 4) // 0xbf2025c8: 0x1012e4
#define REG_MIU_TEST_LENGTH_H (REG_MIU_BASE + 0x73 * 4) // 0xbf2025cc: 0x1012e6
#define REG_MIU_TEST_DATA     (REG_MIU_BASE + 0x74 * 4) // 0xbf2025d0: 0x1012e8
#define REG_MIU_SEL0          (REG_MIU_BASE + 0x78 * 4) // 0xbf2025e0: 0x1012f0
#define REG_MIU_ADCTEST_EN    (REG_MIU_BASE + 0x18 * 4) // 0xbf202460: 0x101230
#define MIU_TEST_LENGTH 0x800000
#define MIU_BIST_TIMEOUT 5000 // ms


#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
#if ( defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI)) 
#else
MS_U32 ChkBists(void)
{
    MS_U16 i;
    for(i = 0x81; i < 0x8C; i += 2)
    {
        MS_U32 BIST_REG = 0xBF2025C0;
        MS_U16 tmp_reg = 0;

        WREG16(BIST_REG, 0);
        //DelayMS(1);

        // start test bist
        WREG16(BIST_REG, i);

        do
        {
            tmp_reg = RREG16(BIST_REG);
        } while(!(tmp_reg & 0x8000));

        if(tmp_reg & 0x6000)
            return 0;           // for false
    }
    return 1;                   // for true
}
#endif
#if ( defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI))
//Kappa has been modified to change only single 4 bit each time
static void HAL_GoodPhaseScan(void)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;

    MS_U16 i, j, k;


    {
        //choose miu0
        WREG16(0xBF2025E0, 0);//0x1012F0
        // store default phase setting
        def_phase = RREG16(0xBF221ADC);//0x110D6E
    }


    //_PUTW(def_phase); //print default phase

    WREG16(0xBF2025C4, 0x0000);//0x1012E2
    WREG16(0xBF2025C8, 0x0401);//0x1012E4

    WREG16(0xBF2025CC, 0x0000);//0x1012E6
    WREG16(0xBF2025D0, 0x00FF);//0x1012E8

    for (i=0;i<16;i+=4)
    {
        //_PUTC('i');
        MS_U16 min=0, max=0;
        tmp_val = 0xf;
        def_phase = (def_phase &(~(tmp_val<<i)));
        for (j=0;j<16;j++)
        {
            tmp_val = (def_phase)|(j<<i);

            WREG16(0xBF221ADC, tmp_val);//0x110D0E

            for(k = 0x81; k < 0x8C; k += 2)
            {
                WREG16(0xBF2025C0, 0);
                //DelayMS(1);

                WREG16(0xBF2025C0, k);

                do
                {
                    tmp_val = RREG16(0xBF2025C0);
                } while(!(tmp_val & 0x8000));

                if(tmp_val & 0x6000)
                {
                    tmp_val = 0;
                    break;
                }
                tmp_val=1;
            }

            if(tmp_val == 1)
            {
                if (min==0)
                    min=j;
                max = j;

                //_PUTX(j & 0xF);     // print phase num if success
            }
        }
        //_PUTC('j');
//_PUTC('k');
//_PUTC('L');
        def_phase = (def_phase)|(((min+max)>>1)<<(i));
//_PUTC('M');

    }
    WREG16(0xBF221ADC, def_phase);//0x110D6E

//    _PUTW(def_phase); //print default phase

    WREG16(0xBF2025C0, 0x0000);//0x1012E0

    WREG8(0xBF2025CE, 0x00);//0x1012E0

    WREG16(0xBF2025E0, 0x0000);//0x1012F0
}
#elif defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
void HAL_GoodPhaseScan(MS_U32 MIU_NUM)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;
    MS_U16 good_phase_cnt = 0, good_phase_sum = 0;
    MS_U16 i;
    MS_U16 shift = 0;
    
    if(MIU_NUM == 0)            // MIU0
    {
        //choose miu0
        WREG16(0xBF2025E0, 0);
        // store default phase setting
        def_phase = RREG16(0xBF221ADC);
        
    }
    else
    {
        return;
    }
    
    //_PUTW(def_phase); //print default phase
    //_PUTC('[');

    WREG16(0xBF2025C4, 0x0000);
    WREG16(0xBF2025C8, 0x0401);
    
    for(;shift < 16; shift += 4)
    {
        good_phase_cnt = 0;
        good_phase_sum = 0;
        
        for(i = 0 ; i < 16 ; i++)
        {
            //clk rst
            tmp_val = RREG8(0xBF221A1C/*110D, 0x7*/);
            tmp_val = ((tmp_val & 0x9F) | 0x60);
            WREG8(0xBF221A1C/*110D, 0x7*/, tmp_val);            

                    
            tmp_val = RREG16(0xBF221ADC) & (~(0xf << shift));
            tmp_val |= (i << shift);
            WREG16(0xBF221ADC/*110D, 0x37*/, tmp_val);
            
            // clk unreset
            tmp_val = RREG8(0xBF221A1C/*110D, 0x7*/);
            tmp_val = ((tmp_val & 0x9F) | 0x20);
            WREG8(0xBF221A1C/*110D, 0x7*/, tmp_val);
    
            if(ChkBists() == 1)     // 1 for true
            {
                good_phase_cnt++;
                good_phase_sum += i;
                //_PUTX(i);     // print phase num if success
            }        
        }
        
        good_phase_sum = good_phase_sum / good_phase_cnt;
        tmp_val = RREG16(0xBF221ADC) & (~(0xf << shift));
        tmp_val |= (good_phase_sum << shift);
        WREG16(0xBF221ADC/*110D, 0x37*/, tmp_val);
    }
    
    //_PUTC(']');

    WREG16(0xBF2025C0/*1012, 0x70*/, 0x0000);

    WREG8(0xBF2025CE/*1012, 0x73, high*/, 0x00);

    WREG16(0xBF2025E0/*1012, 0x78*/, 0x0000);    
}
#else
#define DEFAULT_BIST_LENG_16BYTE 0x0401
void HAL_GoodPhaseScan(MS_U32 MIU_NUM)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;
    MS_U16 good_phase_cnt = 0, good_phase_sum = 0;
    MS_U16 i;

    if(MIU_NUM == 0)            // MIU0
    {
        //choose miu0
        WREG16(0xBF2025E0, 0);
        // store default phase setting
        def_phase = RREG16(0xBF221ADC);
    }
    else                        // MIU1
    {
        //choose miu1
        WREG16(0xBF2025E0, 0x8000);
        // store default phase setting
        def_phase = RREG16(0xBF221BDC);
    }

    //_PUTW(def_phase); //print default phase
    //_PUTC('[');

    if (MIU_BIST_LENGTH_UNIT >= 4) // 16 bytes unit
    {
        WREG16(0xBF2025C4, 0x0000);
        WREG16(0xBF2025C8, DEFAULT_BIST_LENG_16BYTE);
    }
    else
    {
        tmp_val = 4 - MIU_BIST_LENGTH_UNIT;
        WREG16(0xBF2025C4, 0x0000);
        WREG16(0xBF2025C8, DEFAULT_BIST_LENG_16BYTE << tmp_val);
    }


    for(i = 0; i <= 32; i++)
    {
        if((i & 0xF) == 0)
        {
            if(i <= 16)         // 0, 16
            {
                if(MIU_NUM == 0)        // MIU0
                    WREG8(0xBF2025CE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
                else            // MIU1
                    WREG8(0xBF200DCE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
            }
        }

        //clk rst
        tmp_val = RREG8(0xBF221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x60);
        WREG8(0xBF221A1C, tmp_val);

        if(i < 16)              // x1
        {
            tmp_val = (((i & 0xF) << 4) | (def_phase & 0xf));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0xBF221ADC, tmp_val);
            else                // MIU1
                WREG16(0xBF221BDC, tmp_val);
        }
        else if((i == 16) || (i == 32)) //x0
        {
            if(good_phase_cnt == 0)
            {
                good_phase_sum = def_phase;
            }
            else
            {
                good_phase_sum = good_phase_sum / good_phase_cnt;
                if(i == 16)     // 16
                {
                    good_phase_sum = good_phase_sum << 4;
                }
                else            // 32
                {
                    good_phase_sum = (good_phase_sum | (def_phase & 0xF0));
                }
            }
            def_phase = good_phase_sum;

            // clear value
            good_phase_cnt = 0;
            good_phase_sum = 0;

            tmp_val = ((i & 0xF) | (def_phase & 0xf0));

            if(MIU_NUM == 0)    // MIU0
                WREG16(0xBF221ADC, def_phase);
            else
                WREG16(0xBF221BDC, def_phase);
        }
        else                    // x0
        {
            tmp_val = ((i & 0xF) | (def_phase & 0xf0));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0xBF221ADC, tmp_val);
            else                // MIU1
                WREG16(0xBF221BDC, tmp_val);
        }

        // clk unreset
        tmp_val = RREG8(0xBF221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x20);
        WREG8(0xBF221A1C, tmp_val);

        if(ChkBists() == 1)     // 1 for true
        {
            if(i == 32)
                break;
            good_phase_cnt++;
            good_phase_sum += (i & 0xF);
           // _PUTX(i & 0xF);     // print phase num if success
        }
    }

    //_PUTC(']');

    WREG16(0xBF2025C0, 0x0000);

    if(MIU_NUM == 0)            // MIU0
        WREG8(0xBF2025CE, 0x00);
    else                        // MIU1
        WREG8(0xBF200DCE, 0x00);

    WREG16(0xBF2025E0, 0x0000);
}
#endif

static void HAL_AutoPhaseScan(void)
{
    // Mask MIU0 group
    WREG16(0xBF20248C, 0x7FFE);

    // call good phase
#if (defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI))
    HAL_GoodPhaseScan();
#else
    HAL_GoodPhaseScan(0);
#endif

    // UnMask MIU0 Group
    WREG16(0xBF20248C, 0x0000);

#if MIU_NUMBERS > 1
    // Mask MIU1 group
    WREG16(0xBF200C8C, 0x7FFE);

    // call good phase
    HAL_GoodPhaseScan(1);

    // UnMask MIU1 Group
    WREG16(0xBF200C8C, 0x0000);
#endif
    //Bist Test
    //HAL_BistTest();

    // stop bist test
    WREG16(0xBF2025C0, 0x0000);
}

#else // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE

static void _MIU_BistTest(void)
{
    MS_U16 result;
    MS_U32 tmo = (MS_U32)(MIU_BIST_TIMEOUT) * (CPU_CLK_FREQ / 2 / 1000);
    MS_U32 count;

    WREG16(REG_MIU_TEST_CTRL, 0x0003);  // bitst test mode:3

    asm volatile ("mtc0 $0, $11" : :); // clear cp0 compare
    asm volatile ("mtc0 $0, $9" : :);  // clear cp0

    do
    {
        result = RREG16(REG_MIU_TEST_CTRL);
        asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    } while((count < tmo) && (result & 0x8000) == 0);

    if((count < tmo) && ((result & 0x6000) == 0))
    {
        _PUTS("OK\n");
        WREG16(REG_MIU_SEL0, 0x0000);       // reset 0x1012f0 bit_15=0
        return;
    }

    _PUTS("FAIL\n");
    _Fail(ERROR_MIU_BIST);
}

#if MIU_NUMBERS > 1
static void _MIU_Bist1(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) & 0xffff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) >> 16) & 0xfff;

    WREG_DBG(0xA004, 2);
    _PUTS("BIST1_");

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16(REG_MIU_TEST_BASE, 0x0000);
    WREG16(REG_MIU_TEST_LENGTH_L, u16Lo);
    WREG16(REG_MIU_TEST_LENGTH_H, u16Hi);

    WREG16(REG_MIU_SEL0, 0x8000);       //bit_15=1 choice to MIU_1

    _MIU_BistTest();
    WREG_DBG(0xA005, 2);
}
#endif

static void _MIU_Bist0(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) & 0xffff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) >> 16) & 0xfff;

    WREG_DBG(0xA002, 2);
    _PUTS("\nBIST0_");

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

#if MIU_NUMBERS > 1
    _MIU_Bist1();
#endif

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
}

#endif // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE


static void _InitTrimBeforeHash1(void)
{
#if (defined(CONFIG_MSTAR_KELTIC))
    MS_U8 trim_read = 0,trim_write = 0;
    /*  Read DRAM ZQ TRIM DATA  */
    trim_read = *(volatile unsigned char *)(0xBF320000 + 0x3d79);
    trim_write = *(volatile unsigned char *)(0xBF000000 + (0x110d7c<<1));
    trim_write = trim_write | (trim_read << 4);
    *(volatile unsigned char *)(0xBF000000 + (0x110d7c<<1)) = trim_write;
#endif

#if (defined(CONFIG_MSTAR_K5TN))
     #define BK_MIU_ATOP(x) *(volatile MS_U16 *)(0xBF000000+(0x110D00<<1)+((x)<<2))
     #define READ_OTP_SHDW(x) (*(volatile MS_U16*)(0xBF000000 + (0x171600<<1) + (x<<2) ))
     MS_U16 U16OTPData = 0;
     MS_U16 U16ATOPData = 0;
     //DDR ATOP PAD
     //0X110D_58[15:8] <== BK1716_2A[11:4]
     U16OTPData = READ_OTP_SHDW(0x2A);
     
     if((U16OTPData >> 12) & 0x0001) //BK1716_2A[12] SW read  ZQ resistance trimming when when Activateefuse=1
     {
     	U16OTPData &= 0x0ff0;
     	U16OTPData = U16OTPData << 4;
     
     	U16ATOPData = BK_MIU_ATOP(0x58)&0x00ff;
     	U16ATOPData |= U16OTPData;
     	BK_MIU_ATOP(0x58) = U16ATOPData;
     }	
#endif

#if (defined(CONFIG_MSTAR_KAYLA))
    #define BIT(_bit_)                  (1 << (_bit_))

    #define MIU_Write2ByteMask( u32Reg, u16Val, u16Msk )                         \
        do{                                                                         \
        MS_U32 u32Reg8 = 0xbf000000 + ( (u32Reg) <<1);                                      \
        WREG16( u32Reg8 ,(RREG16(u32Reg8) & ~(u16Msk)) | ((u16Val) & (u16Msk)) );     \
        }while(0)
    
    MS_U16 trim_read = 0,trim_write = 0,trim_data = 0;
    MS_S16 trim_op = 0;

    trim_read = *(volatile MS_U16 *)(0xBF000000 + ((0x171600 + (0x2a<<1))<<1));

    if((trim_read & BIT(6)) == BIT(6))
    {
        switch((trim_read >> 3)&7)
        {
            case 6:
            {
                trim_op = 2;
                //_PUTS("2\n");
                break;
            }
            case 5:
            {
                trim_op = 1;
                //_PUTS("1\n");
                break;
            }
            case 1:
            {
                trim_op = -1;
                //_PUTS("-1\n");
                break;
            }
            case 2:
            {
                trim_op = -2;
                //_PUTS("-2\n");
                break;
            }
            default:
            {
                break;
            }
        }
     
        trim_data = *(volatile MS_U16 *)(0xBF000000 + ((0x110d00 + (0x2c<<1))<<1));
        trim_data = (trim_data >> 8) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data <<8;
        MIU_Write2ByteMask(0x110d00 +(0x2c<<1), trim_data ,BIT(8)|BIT(9)|BIT(10)|BIT(11));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + ((0x110d00 + (0x2f<<1))<<1));
        trim_data = trim_data & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        //trim_data = trim_data ;
        MIU_Write2ByteMask(0x110d00 +(0x2f<<1), trim_data ,BIT(0)|BIT(1)|BIT(2)|BIT(3));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + ((0x110d00 + (0x2f<<1))<<1));
        trim_data = (trim_data >> 4) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data <<4;
        MIU_Write2ByteMask(0x110d00 +(0x2f<<1), trim_data ,BIT(4)|BIT(5)|BIT(6)|BIT(7));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + ((0x110d00 + (0x2f<<1))<<1));
        trim_data = (trim_data >> 8) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data <<8;
        MIU_Write2ByteMask(0x110d00 +(0x2f<<1), trim_data ,BIT(8)|BIT(9)|BIT(10)|BIT(11));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + ((0x110d00 + (0x2f<<1))<<1));
        trim_data = (trim_data >> 12) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data << 12;
        MIU_Write2ByteMask(0x110d00 +(0x2f<<1), trim_data ,BIT(12)|BIT(13)|BIT(14)|BIT(15)); 
    }

    trim_read = *(volatile MS_U16 *)(0xBF000000 + ((0x171600 + (0x2a<<1))<<1));

    if((trim_read & BIT(2)) == BIT(2))
    {
        trim_read = ((trim_read & 0b11) << 1) + (((*(volatile MS_U16 *)(0xBF000000 + ((0x171600 + (0x29<<1))<<1)))>>15) &1 );              

        switch(trim_read)
        {
            case 6:
            {
                trim_op = 2;
                break;
            }
            case 5:
            {
                trim_op = 1;
                break;
            }
            case 1:
            {
                trim_op = -1;
                break;
            }
            case 2:
            {
                trim_op = -2;
                break;
            }
            default:
            {
                break;
            }
        }

        trim_data = *(volatile MS_U16 *)(0xBF000000 + ((0x110d00 + (0x2c<<1))<<1));
        trim_data = trim_data & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0:trim_data + trim_op;
        //trim_data = trim_data;
        MIU_Write2ByteMask(0x110d00 +(0x2c<<1), trim_data ,BIT(0)|BIT(1)|BIT(2)|BIT(3));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + (0x110d00 + ((0x2e<<1))<<1));
        trim_data = trim_data & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        //trim_data = trim_data ;
        MIU_Write2ByteMask(0x110d00 +(0x2e<<1), trim_data ,BIT(0)|BIT(1)|BIT(2)|BIT(3));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + (0x110d00 + ((0x2e<<1))<<1));
        trim_data = (trim_data >> 4) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data <<4;
        MIU_Write2ByteMask(0x110d00 +(0x2e<<1), trim_data ,BIT(4)|BIT(5)|BIT(6)|BIT(7));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + (0x110d00 + ((0x2e<<1))<<1));
        trim_data = (trim_data >> 8) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data <<8;
        MIU_Write2ByteMask(0x110d00 +(0x2e<<1), trim_data ,BIT(8)|BIT(9)|BIT(10)|BIT(11));

        trim_data = *(volatile MS_U16 *)(0xBF000000 + (0x110d00 + ((0x2e<<1))<<1));
        trim_data = (trim_data >> 12) & 0b1111;
        trim_data = ( (MS_S16)trim_data + trim_op < 0)? 0: ( ((MS_S16)trim_data + trim_op > 0Xf)? 0xf : trim_data + trim_op);
        trim_data = trim_data << 12;
        MIU_Write2ByteMask(0x110d00 +(0x2e<<1), trim_data ,BIT(12)|BIT(13)|BIT(14)|BIT(15)); 

    }
#endif
}

static void _InitTrimAfterHash1(void)
{
#if defined(CONFIG_MSTAR_KERES)||defined(CONFIG_MSTAR_KIRIN)||defined(CONFIG_MSTAR_KRIS)
    MS_U32 drv = 0, pos;
    MS_U32 trim_read = 0;
    
    /*  Read DRAM ZQ TRIM DATA  */
    trim_read = *(volatile unsigned int *)(0xBF320000 + 0x3c88);
    if(trim_read & 0x40000000)
    {
        MS_U32 drv1, drvTmp = 0;
        MS_U32 drvp_tune = (trim_read & 0x18000000) >> 27; 
        MS_U32 drvn_tune = (trim_read & 0x03000000) >> 24; 
        drv = *(volatile unsigned int *)(0xBF000000 + (0x110d58 << 1));
        // reg_drvn_cs
        drv1 = (drv & 0x0000000F) >> 0;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 0)) : (((drv1 + drvn_tune) & 0x7) << 0);
        // reg_drvn_cs1                
        drv1 = (drv & 0x000000F0) >> 4;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 4)) : (((drv1 + drvn_tune) & 0x7) << 4);
        // reg_drvp_cs                
        drv1 = (drv & 0x00000F00) >> 8;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 8)) : (((drv1 + drvp_tune) & 0x7) << 8);
        // reg_drvp_cs1
        drv1 = (drv & 0x0000F000) >> 12;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 12)) : (((drv1 + drvp_tune) & 0x7) << 12);
        *(volatile unsigned int *)(0xBF000000 + (0x110d58 << 1)) = drvTmp;
            
        drv = *(volatile unsigned int *)(0xBF000000 + (0x110d5c << 1));
        drvTmp = 0;

        // reg_drvn_dqs
        drv1 = (drv & 0x0000000F) >> 0;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 0)) : (((drv1 + drvn_tune) & 0x7) << 0);
        // reg_drvn_dq
        drv1 = (drv & 0x000000F0) >> 4;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 4)) : (((drv1 + drvn_tune) & 0x7) << 4);
        // reg_drvn_cmd
        drv1 = (drv & 0x00000F00) >> 8;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 8)) : (((drv1 + drvn_tune) & 0x7) << 8);
        // reg_drvn_clk
        drv1 = (drv & 0x0000F000) >> 12;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 12)) : (((drv1 + drvn_tune) & 0x7) << 12);
                       
        *(volatile unsigned int *)(0xBF000000 + (0x110d5c << 1)) = drvTmp;

        drv = *(volatile unsigned int *)(0xBF000000 + (0x110d5e << 1));
        drvTmp = 0;

        // reg_drvp_dqs
        drv1 = (drv & 0x0000000F) >> 0;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 0)) : (((drv1 + drvp_tune) & 0x7) << 0);
        // reg_drvp_dq
        drv1 = (drv & 0x000000F0) >> 4;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 4)) : (((drv1 + drvp_tune) & 0x7) << 4);
        // reg_drvp_cmd
        drv1 = (drv & 0x00000F00) >> 8;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 8)) : (((drv1 + drvp_tune) & 0x7) << 8);
        // reg_drvp_clk
        drv1 = (drv & 0x0000F000) >> 12;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 12)) : (((drv1 + drvp_tune) & 0x7) << 12);
        *(volatile unsigned int *)(0xBF000000 + (0x110d5e << 1)) = drvTmp;

        drv = *(volatile unsigned int *)(0xBF000000 + (0x110d7a << 1));
        drvTmp = 0;
        // reg_ddr_eco
        drv1 = (drv & 0x0000000F) >> 0;
        drvTmp |= (trim_read & 0x04000000) ? 
            ((drv1 < drvn_tune) ? 0 : ((drv1 - drvn_tune) << 0)) : (((drv1 + drvn_tune) & 0x7) << 0);
        drv1 = (drv & 0x000000F0) >> 4;
        drvTmp |= (trim_read & 0x20000000) ? 
            ((drv1 < drvp_tune) ? 0 : ((drv1 - drvp_tune) << 4)) : (((drv1 + drvp_tune) & 0x7) << 4);
        
        *(volatile unsigned int *)(0xBF000000 + (0x110d7a << 1)) &= ~0xFF;
        *(volatile unsigned int *)(0xBF000000 + (0x110d7a << 1)) |= drvTmp;
    }
#endif


}

static void _InitDramMOBF(void)
{
#if defined(CONFIG_MSTAR_KELTIC) || defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
#if defined(CONFIG_DRAM_MOBF)
#if defined(CONFIG_DRAM_MOBF_64BIT)

        WREG16(MIU_MOBF_CONTROL, 0x0005); /* Enable 64 bit scramble & 64 bits Pipeline*/
#endif
#if defined(CONFIG_MSTAR_KELTIC) 
        WREG16(MOBF_ENABLE,0xc800); /* Triggler MOBF */
        WREG16(MIU_WRITE_CACHE,0x0001);
#elif defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)
        WREG16(MOBF_ENABLE,0xc000); /* Triggler MOBF */
#endif
        //WREG16(MIU_MOBF_PROTECT,0x0008); /* Enable Write Protect -> Don't set bit3 to 1 to enable this feature*/
#else
        /* no MOBF and Disable MIU write Cache*/
        WREG16(MIU_WRITE_CACHE,0x0001);
#endif
#endif
}

enum 
{
    AC_ON = 1,
    DC_ON = 2,
    
};

#if defined(CONFIG_MSTAR_KERES)||defined(CONFIG_MSTAR_KIRIN)||defined(CONFIG_MSTAR_KRIS)||defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN)

#define STR_IS_AC_FLOW_CONTROL     0xBF001CDC

#if defined(CONFIG_MSTAR_STR_ENABLE)

static void _Reset_AVD(void)
{
    MS_U16 Temp;
    Temp = RREG16(REG_ADDR_BASE + (0x103612 << 1));                
    WREG16(REG_ADDR_BASE + (0x103612 << 1), Temp & (~0x4000));  
          
    Temp = RREG16(REG_ADDR_BASE + (0x103612 << 1));                
    WREG16(REG_ADDR_BASE + (0x103612 << 1), Temp | 0x4000);    
        
    Temp = RREG16(REG_ADDR_BASE + (0x103612 << 1));                
    WREG16(REG_ADDR_BASE + (0x103612 << 1), Temp & (~0x4000));        
}

static void _Enter_Refresh_Mode(void)
{
    MS_U16 Temp=0;
    WREG16(REG_ADDR_BASE + (0x1012e0 << 1), 0x0000);  
   
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0xFFFE);
    WREG16(REG_ADDR_BASE + (0x101266 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x101286 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x1012A6 << 1), 0xFFFF);
    
    WREG16(REG_ADDR_BASE + (0x101218 << 1), 0x0400);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x042F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x052F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
        
    Temp = RREG16(REG_ADDR_BASE + (0x101206 << 1));                
    WREG16(REG_ADDR_BASE + (0x101206 << 1), Temp | 0x0800); 
    
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x202F);   
}

static void _Exit_Refresh_Mode(void)
{
    MS_U16 Temp=0;
    
    Temp = RREG16(REG_ADDR_BASE + (0x101206 << 1));                
    WREG16(REG_ADDR_BASE + (0x101206 << 1), Temp & (~0x0800)); 
    
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x202F); 
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0xFFFF);
         
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x000F);   
    
    DelayMS(50);
    
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x001F);
    
    DelayMS(5);
      
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0x7ffe);
    WREG16(REG_ADDR_BASE + (0x101266 << 1), 0xffff);
    WREG16(REG_ADDR_BASE + (0x101286 << 1), 0xffff);
    WREG16(REG_ADDR_BASE + (0x1012A6 << 1), 0xffff);
 
    WREG16(REG_ADDR_BASE + (0x10121e << 1), 0x0c08);
    WREG16(REG_ADDR_BASE + (0x1012e0 << 1), 0x0000);
}
#endif //CONFIG_MSTAR_STR_ENABLE endif

static void _Disable_ATOP_SW(void)
{
    MS_U16 Temp;
    
    Temp = RREG16(REG_ADDR_BASE + (0x101202 << 1));
    WREG16(REG_ADDR_BASE + (0x101202 << 1), Temp & (~0xF000));  
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D54 << 1));
    WREG16(REG_ADDR_BASE + (0x110D54 << 1), Temp & (~0x0070));  
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D08 << 1));
    WREG16(REG_ADDR_BASE + (0x110D08 << 1), Temp | 0x00FF);
}


static void _MIU0_Push_Dram(void)
{
    //====== New Sequesnce for MIU0 dqsm reset start ======
    #if (ENABLE_MEM1866MHz)    
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00e9);
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00a9);
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00e9);
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00a9);
    #elif (ENABLE_MEM1600MHz)
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00e7);
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00a7);
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00e7);
    WREG16(REG_ADDR_BASE + (0x110d0e << 1), 0x00a7);
    #endif
    WREG16(REG_ADDR_BASE + (0x101218 << 1), 0x0400);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x043f);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x053f);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x003f);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x023f);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x033f);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x001f);
}

static void _MIU0_InitCycle(void)
{
    MS_U16 u16CheckInit = 0;
    // MIU0 INIT Cycle    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x0000); // hold miu_0    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x0008); // DRAM reset    
    DelayMS(1); //DELAYUS 1000  @ delay 1ms
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x000c); // DRAM reset+CS    R
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x000e); // DRAM reset+CS+CKE    
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x001f); //  cke to cmd > tXP
    u16CheckInit = RREG16(REG_ADDR_BASE + (0x101200 <<1 ));
    _PUTS("\nWait MIU0...\n");
    while((u16CheckInit >> 15) == 0)
    {
        u16CheckInit = RREG16(REG_ADDR_BASE + (0x101200 <<1 ));
    }
    _PUTS("MIU0 Init Done \n");
}

static void _MIU_ClearIPMask(void)
{ 
  //It has been moved to hash1(param.c) of each chip(mips series chip)

  // open all MIU0 request mask (All IP can access MIU resource)  
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x0000);   //Mask MIU0_group_0  
    WREG16((REG_ADDR_BASE + (0x101266 <<1 )) , 0x0000);   //Mask MIU0_group_1   
    WREG16((REG_ADDR_BASE + (0x101286 <<1 )) , 0x0000);   //Mask MIU0_group_2    
    WREG16((REG_ADDR_BASE + (0x1012a6 <<1 )) , 0x0000);   // Mask MIU0_group_3

    WREG16((REG_ADDR_BASE + (0x10121e <<1 )), 0x0c08);  // SW initial done and turn on arbitor 	



}
#endif // CONFIG_MSTAR_KERES endif

#if defined(CONFIG_MSTAR_KERES)||defined(CONFIG_MSTAR_KIRIN)||defined(CONFIG_MSTAR_KRIS)||defined(CONFIG_MSTAR_KAYLA)||defined(CONFIG_MSTAR_K5TN)
static void _InitParam_End(MS_U8 u8POWER_Flag)
{
#if 0 //following is moving to has1 (param.c end of _hash1)
   _PUTC('D');
   if (u8POWER_Flag == AC_ON)
    {
        MS_U16 Temp;
        Temp = RREG16(REG_ADDR_BASE + (0x110D00 << 1));
        WREG16(REG_ADDR_BASE + (0x110D00 << 1), Temp & (~0x0008));
        _Disable_ATOP_SW();
    }
    _PUTC('E');
    _MIU0_InitCycle();
    _PUTC('F');
    if (u8POWER_Flag == AC_ON)
    {
         _MIU0_Push_Dram();
         
    }

#if CONFIG_MSTAR_STR_ENABLE
    if (u8POWER_Flag == DC_ON)
    {  
        WREG16(REG_ADDR_BASE + (0x110d6c << 1), RREG16(REG_ADDR_BASE + (0x3db2 << 1)));
        WREG16(REG_ADDR_BASE + (0x110d6e << 1), RREG16(REG_ADDR_BASE + (0x3db6 << 1)));
        
        //--------------- STR ---------------//
        _Enter_Refresh_Mode();
        _Disable_ATOP_SW();
        _Exit_Refresh_Mode();
        //--------------- STR ---------------//
        _MIU0_Push_Dram();
    }
#endif //CONFIG_MSTAR_STR_ENABLE endif
    _MIU_ClearIPMask();

    _PUTC('G');
    if (u8POWER_Flag == AC_ON) 
    {  
        // ODT setting
        // 0x101228[7:4] = 0x110D0E[4:1]
        // 0x110D4E[2:0] = 0x110D3C[2:0]
        MS_U16 tmp_src = RREG16(REG_ADDR_BASE + (0x110d0e << 1));
        MS_U16 tmp_dst = RREG16(REG_ADDR_BASE + (0x101228 << 1));
        WREG16(REG_ADDR_BASE + (0x101228 << 1), (tmp_dst & ~0x00f0) | (((tmp_src >> 1) & 0x000f) << 4));
        
        tmp_src = RREG16(REG_ADDR_BASE + (0x110d3c << 1));
        tmp_dst = RREG16(REG_ADDR_BASE + (0x110d4e << 1));
        WREG16(REG_ADDR_BASE + (0x110d4e << 1), (tmp_dst & ~0x0007) | (tmp_src & 0x0007));
    }
#endif  //end moving to has1 (param.c end of _hash1)

    if (u8POWER_Flag == AC_ON) 
    { 
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
        HAL_AutoPhaseScan();
#else
        _MIU_Bist();
#endif
        WREG16(REG_MIU_ADCTEST_EN, 0x4000);  // TSP  reg_adc_test_en
    }
}
#else
static void _InitParam_End(void)
{
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
    HAL_AutoPhaseScan();
#else
    _MIU_Bist();
#endif

#if defined(CONFIG_MSTAR_KELTIC)
    WREG16(REG_MIU_ADCTEST_EN, 0x4000);  // TSP  reg_adc_test_en
#endif

}
#endif
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

#if (defined(CONFIG_MSTAR_KAPPA) || defined(CONFIG_MSTAR_KRITI))

MS_U32 BOOTROM_C_InitParams(void *pParams)
{
    MS_U32 rbase = (0xbf20 << 16);
    MS_U32 width = 16;
    MS_U16 *gParse = pParams;

    while(1)
    {
        if((*(gParse)) == CHIPCFG_CMD_END)
            break;

        switch ((*(gParse)))
        {
            case CHIPCFG_CMD_IF:
                {
                    volatile MS_U16 *addr = (void *)(rbase + (*(gParse+1)));
                    gParse+=2;
                    if((*(gParse)) == CHIPCFG_CMD_END)
                    {
                        return 2;
                    }

                    MS_U16 value = (*(gParse));
                    MS_U16 mask = (*(gParse+1));

                    if((*addr & mask) != value)
                    {
                        // skip to CHIPCFG_CMD_ENDIF
                        //Entry *e;
                        do
                        {
                            //e = gParse++;
                            //if(e->cmd.param1 == CHIPCFG_CMD_END)
                            if((*(gParse)) == CHIPCFG_CMD_END)
                            {
                                gParse+=2;
                                return 3;
                            }
                            gParse+=2;
                        } while((*(gParse)) != CHIPCFG_CMD_ENDIF);
                    }
                }
                break;

            case CHIPCFG_CMD_ENDIF:
                // ignore
                break;

            case CHIPCFG_CMD_BASE:
                rbase = ((*(gParse+1)) << 16);
                width = 16;
                break;

            case CHIPCFG_CMD_BASE_8BITS:
                rbase = ((*(gParse+1)) << 16);
                width = 8;
                break;

            case CHIPCFG_CMD_WAIT:
                DelayMS((*(gParse+1)));
                break;

            case CHIPCFG_CMD_DBG:
                WREG_DBG((*(gParse+1)), 3);
                break;

            default:
                if(width == 8)
                {
                    MS_U8 old = RREG8(rbase + (*(gParse)));
                    MS_U8 val = ((*(gParse+1)) >> 8) & 0xff;
                    MS_U8 mask = (*(gParse+1)) & 0xff;
                    WREG8(rbase + (*(gParse)), (old & ~mask) | (val & mask));
                }
                else
                {
                    WREG16(rbase + (*(gParse)), (*(gParse+1)));
                }
                break;
        }
        gParse+=2;
    }

    //For Kappa U02 DDR3 patch, write phase setting
#if ( MIU_INTERFACE == DDR3_INTERFACE_QFP_128)
    if (RREG16(0xBF203D9C)>>8)  //Revision ID
    {
        WREG16(0xBF221A70, 0x0045);  //0x110d38
    }
#endif
    _InitParam_End();
    WREG_DBG(0xF001, 1);
    return 1;
}

#else
static const Entry *gEntrys;

static const Entry *_Next(void)
{
    const Entry *e = gEntrys++;

    if(e->cmd.param1 == CHIPCFG_CMD_END)
        return NULL;

    return e;
}

MS_U32 BOOTROM_C_InitParams(void *pParams)
{
    MS_U32 rbase = (0xbf20 << 16);
    MS_U32 width = 16;
    MS_U8  u8POWER_Flag = AC_ON;
    gEntrys = pParams;


    _InitTrimBeforeHash1();
    
#if (defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN) )
#if (defined(CONFIG_MSTAR_STR_ENABLE))
    if ((RREG16(STR_IS_AC_FLOW_CONTROL) & 0xFF00) == 0x0100)
    {
        u8POWER_Flag = DC_ON;
        _PUTS("SPI STR ON\n\r");

    }
    else 
    {
        u8POWER_Flag = AC_ON;
        _PUTS("SPI AC ON\n\r");
    }

#endif // CONFIG_MSTAR_STR_ENABLE endif
#endif // CONFIG_MSTAR_KERES endif

    while(1)
    {
        const Entry *eCur = _Next();
        if(eCur == NULL)
            break;

        //_PUTW(rbase);
        //_PUTW(eCur->cmd.param1);
        //_PUTW(eCur->cmd.param2);


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

#if (defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN))  //following is for STR scripts moving to Hash1
            case CHIPCFG_MIU_INIT_CYCLE:
                 _MIU0_InitCycle();
                 break;
                 
            case CHIPCFG_STR_DC_BEFORE_REFRESH:
                 WREG16(REG_ADDR_BASE + (0x110d6c << 1), RREG16(REG_ADDR_BASE + (0x3db2 << 1)));        
                 WREG16(REG_ADDR_BASE + (0x110d6e << 1), RREG16(REG_ADDR_BASE + (0x3db6 << 1)));
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
                  
            case CHIPCFG_INIT_TRIM:
                 _InitTrimAfterHash1();
                 break; 
            
#endif //end "STR scripts moving to Hash1"
            default:

#if (defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN))                  
#if (defined(CONFIG_MSTAR_STR_ENABLE))
                if (eCur->reg.addr == 0x6700)
                {
                    if (eCur->reg.value == CHIPCFG_CMD_AVD_RESET)
                    {

                        if (u8POWER_Flag == DC_ON)
                        {
                            _Reset_AVD();
                        }
                    }         
                }
                else
#endif // CONFIG_MSTAR_KERES endif
#endif // CONFIG_MSTAR_KERES endif
                {
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
                }
                break;
        }
    }

#if (defined(CONFIG_MSTAR_KERES) || defined(CONFIG_MSTAR_KIRIN) || defined(CONFIG_MSTAR_KRIS) || defined(CONFIG_MSTAR_KAYLA) || defined(CONFIG_MSTAR_K5TN))
    //_InitTrimAfterHash1(); for keres STR flow moving to HASH1
    _InitParam_End(u8POWER_Flag);
#else
    _InitParam_End();
#endif
   
    _InitDramMOBF();
    WREG_DBG(0xF001, 1);
    return 1;
}
#endif

