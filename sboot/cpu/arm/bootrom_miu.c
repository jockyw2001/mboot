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
#define MIU_ENABLE_AUTO_READ_PHASE

#define MHal_MIU_ReadReg16(u32bank, u32reg ) *( ( volatile MS_U16* ) (MS_U32)(MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1)) )
#define MHal_MIU_WritReg16(u32bank, u32reg, u16val ) *( ( volatile MS_U16* ) (MS_U32)(MIU_RIU_REG_BASE + (u32bank)*0x100*2 + ((u32reg) << 1))  ) = (u16val)

#define READ_OTP_8(x) (*(volatile MS_U8*)(0x1F320000 + (x)))
#define READ_OTP_16(x) (*(volatile MS_U16*)(0x1F320000 + (x)))

static MS_U16 u16UseMIU1 = 0;

#define STR_IS_AC_FLOW_CONTROL  0x1F001CDC

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


#ifdef CONFIG_MSTAR_KAISER

#if CONFIG_MSTAR_STR_ENABLE

#define STR_IS_AC_FLOW_CONTROL     0x1F001CDC

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
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x202F);   
    
#if MIU_NUMBERS > 1    
    WREG16(REG_ADDR_BASE + (0x100646 << 1), 0xFFFE);
    WREG16(REG_ADDR_BASE + (0x100666 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x100686 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x1006A6 << 1), 0xFFFF);
    
    WREG16(REG_ADDR_BASE + (0x100618 << 1), 0x0400);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x042F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x052F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    
    WREG16(REG_ADDR_BASE + (0x100646 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x202F);
#endif     
}

static void _Exit_Refresh_Mode(void)
{
    MS_U16 Temp=0;
      
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x000F);      
#if MIU_NUMBERS > 1      
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x000F);
#endif     
    
    DelayMS(50);
    
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x001F);
#if MIU_NUMBERS > 1
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x001F);
#endif
    
    DelayMS(5);
      
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x101266 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x101286 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x1012A6 << 1), 0x0000);
#if MIU_NUMBERS > 1  
    WREG16(REG_ADDR_BASE + (0x100646 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x100666 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x100686 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x1006A6 << 1), 0x0000);
#endif
    
    WREG16(REG_ADDR_BASE + (0x10121e << 1), 0x0c08);
    WREG16(REG_ADDR_BASE + (0x1012e0 << 1), 0x0000);
}


#endif

static void _Disable_ATOP_SW(void)
{
    MS_U16 Temp;

    
    Temp = RREG16(REG_ADDR_BASE + (0x101202 << 1));
    WREG16(REG_ADDR_BASE + (0x101202 << 1), Temp & (~0xF000));  
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D54 << 1));
    WREG16(REG_ADDR_BASE + (0x110D54 << 1), Temp & (~0x0070));  
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D08 << 1));
    WREG16(REG_ADDR_BASE + (0x110D08 << 1), Temp | 0x003F);
    
#if MIU_NUMBERS > 1   
    Temp = RREG16(REG_ADDR_BASE + (0x100602 << 1));
    WREG16(REG_ADDR_BASE + (0x100602 << 1), Temp & (~0xF000)); 
    
    Temp = RREG16(REG_ADDR_BASE + (0x110DD4 << 1));
    WREG16(REG_ADDR_BASE + (0x110DD4 << 1), Temp & (~0x0070));
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D88 << 1));
    WREG16(REG_ADDR_BASE + (0x110D88 << 1), Temp | 0x003F); 
#endif
}

#if 1
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
MS_U32 ChkBists(void)
{
    MS_U16 i;
    for(i = 0x81; i < 0x8C; i += 2)
    {
        MS_U32 BIST_REG = 0xBF2025C0;
        MS_U16 tmp_reg = 0;

        WREG16(BIST_REG, 0);
        DelayMS(1);

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

void HAL_GoodPhaseScan(MS_U32 MIU_NUM)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;
    MS_U16 good_phase_cnt = 0, good_phase_sum = 0;
    MS_U16 i;

    if(MIU_NUM == 0)            // MIU0
    {
        //choose miu0
        WREG16(0x1F2025E0, 0);
        // store default phase setting
        def_phase = RREG16(0x1F221ADC);
    }
    else                        // MIU1
    {
        //choose miu1
        WREG16(0x1F2025E0, 0x8000);
        // store default phase setting
        def_phase = RREG16(0x1F221BDC);
    }

    //_PUTW(def_phase); //print default phase
    _PUTC('[');

#ifdef CONFIG_MSTAR_KAISERIN
    WREG16(0x1F2025C4, 0x1FFF);
    WREG16(0x1F2025C8, 0x1000);
#elif defined(CONFIG_MSTAR_KRONUS)
    WREG16(0x1F2025C4, 0x0000);
    WREG16(0x1F2025C8, 0x8001);
#endif
    WREG16(0x1F2025CC, 0x0000);
    WREG16(0x1F2025D0, 0x00FF);

    for(i = 0; i <= 32; i++)
    {
        if((i & 0xF) == 0)
        {
            if(i <= 16)         // 0, 16
            {
                if(MIU_NUM == 0)        // MIU0
                    WREG8(0x1F2025CE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
                else            // MIU1
                    WREG8(0x1F200DCE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
            }
        }

        //clk rst
        tmp_val = RREG8(0x1F221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x60);
        WREG8(0x1F221A1C, tmp_val);

        if(i < 16)              // x1
        {
            tmp_val = (((i & 0xF) << 4) | (def_phase & 0xf));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0x1F221ADC, tmp_val);
            else                // MIU1
                WREG16(0x1F221BDC, tmp_val);
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
                WREG16(0x1F221ADC, def_phase);
            else
                WREG16(0x1F221BDC, def_phase);
        }
        else                    // x0
        {
            tmp_val = ((i & 0xF) | (def_phase & 0xf0));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0x1F221ADC, tmp_val);
            else                // MIU1
                WREG16(0x1F221BDC, tmp_val);
        }

        // clk unreset
        tmp_val = RREG8(0x1F221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x20);
        WREG8(0x1F221A1C, tmp_val);

        if(ChkBists() == 1)     // 1 for true
        {
            if(i == 32)
                break;
            good_phase_cnt++;
            good_phase_sum += (i & 0xF);
            _PUTX(i & 0xF);     // print phase num if success
        }
    }

    _PUTC(']');

    WREG16(0x1F2025C0, 0x0000);

    if(MIU_NUM == 0)            // MIU0
        WREG8(0x1F2025CE, 0x00);
    else                        // MIU1
        WREG8(0x1F200DCE, 0x00);

    WREG16(0x1F2025E0, 0x0000);
}

void HAL_AutoPhaseScan(void)
{
    // Mask MIU0 group
    WREG16(0x1F20248C, 0x7FFE);

    // call good phase
    HAL_GoodPhaseScan(0);

    // UnMask MIU0 Group
    WREG16(0x1F20248C, 0x0000);

#if MIU_NUMBERS > 1
    // Mask MIU1 group
    WREG16(0x1F200C8C, 0x7FFE);

    // call good phase
    HAL_GoodPhaseScan(1);

    // UnMask MIU1 Group
    WREG16(0x1F200C8C, 0x0000);
#endif
    //Bist Test
    //HAL_BistTest();

    // stop bist test
    WREG16(0x1F2025C0, 0x0000);
}

#else // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE

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

#if MIU_NUMBERS > 1
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
#endif

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

#if MIU_NUMBERS > 1
    _MIU_Bist1();
#endif

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16((REG_ADDR_BASE  + (0x1012fe <<1 )) , 0x09E1);  // close miu BIST, 7f[11]  : reg_miu_test_off (MIU BIST use the same MIU client with TSP3)
}

#endif // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE

#endif
static void _MIU0_InitCycle(void)
{
    MS_U16 u16CheckInit = 0;
    // MIU0 INIT Cycle    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x0000); // hold miu_0    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x0008); // DRAM reset    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x000c); // DRAM reset+CS    R
    DelayMS(1); //DELAYUS 1000  @ delay 1ms
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
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16   ((REG_ADDR_BASE + (0x101200 <<1 )), 0x001f); // cke to cmd > tXPR
}

static void _MIU1_InitCycle(void)
{
    MS_U16 u16CheckInit = 0;
    // MIU0 INIT Cycle    
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x0000); // hold miu_1 
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x0008); // DRAM reset    
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x000c); // DRAM reset+CS    R
    DelayMS(1); //DELAYUS 1000  @ delay 1ms
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x000e); // DRAM reset+CS+CKE    
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x001f); //  cke to cmd > tXP
    u16CheckInit = RREG16(REG_ADDR_BASE + (0x100600 <<1 ));
    _PUTS("\nWait MIU1...\n");
    while((u16CheckInit >> 15) == 0)
    {
        u16CheckInit = RREG16(REG_ADDR_BASE + (0x100600 <<1 ));
    }
    _PUTS("MIU1 Init Done \n");
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16   ((REG_ADDR_BASE + (0x100600 <<1 )), 0x001f); // cke to cmd > tXPRf
}

static void _MIU_InitPriority(void)
{
    MS_U16 u16Mask = 0, u16write = 0,u16RegMask = 0;
    u16Mask = RREG16(REG_ADDR_BASE + (0x101240 <<1 ));
    u16Mask = u16Mask & 0x0001;

    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x101240 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x101240 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x101240 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x101240 <<1 )), u16write); 
    }
    //miu0_rq0_skip

    u16Mask = RREG16(REG_ADDR_BASE + (0x101260 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x101260 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x101260 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x101260 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x101260 <<1 )), u16write); 
    }
    //miu0_rq1_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x101280 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x101280 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x101280 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x101280 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x101280 <<1 )), u16write); 
    }
    //miu0_rq2_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x1012A0 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x1012A0 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x1012A0 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x1012A0 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x1012A0 <<1 )), u16write); 
    }
    //miu0_rq3_skip
#if MIU_NUMBERS > 1

    u16Mask = RREG16(REG_ADDR_BASE + (0x100640 <<1 ));
    u16Mask = u16Mask & 0x0001;

    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x100640 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x100640 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x100640 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x100640 <<1 )), u16write); 
    }
    //miu1_rq0_skip

    u16Mask = RREG16(REG_ADDR_BASE + (0x100660 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x100660 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x100660 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x100660 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x100660 <<1 )), u16write); 
    }
    //miu1_rq1_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x100680 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x100680 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x100680 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x100680 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x100680 <<1 )), u16write); 
    }
    //miu1_rq2_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x1006A0 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x1006A0 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x1006A0 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x1006A0 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x1006A0 <<1 )), u16write); 
    }
    //miu1_rq3_skip
#endif
     _PUTS("MIU Priority Init Done \n");
}
static void _MIU_ClearIPMask(void)
{
    // open all MIU0 request mask (All IP can access MIU resource)  
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x0000);   //Mask MIU0_group_0  
    WREG16((REG_ADDR_BASE + (0x101266 <<1 )) , 0x0000);   //Mask MIU0_group_1   
    WREG16((REG_ADDR_BASE + (0x101286 <<1 )) , 0x0000);   //Mask MIU0_group_2    
    WREG16((REG_ADDR_BASE + (0x1012a6 <<1 )) , 0x0000);   // Mask MIU0_group_3
#if MIU_NUMBERS > 1
    // open all MIU1 request mask (All IP can access MIU resource)   
    WREG16((REG_ADDR_BASE + (0x100646 <<1 )) , 0x0000);  //Mask MIU0_group_0    
    WREG16((REG_ADDR_BASE + (0x100666 <<1 )) , 0x0000);  //Mask MIU0_group_1    
    WREG16((REG_ADDR_BASE + (0x100686 <<1 )) , 0x0000);  //Mask MIU0_group_2  
    WREG16((REG_ADDR_BASE + (0x1006a6 <<1 )) , 0x0000);  //Mask MIU0_group_3   
    // all MIU1 IP select MIU0 in bringup    
    WREG16((REG_ADDR_BASE + (0x1006f0 <<1 )), 0x0000);
    WREG16((REG_ADDR_BASE + (0x1006f2 <<1 )), 0x0000);
    WREG16((REG_ADDR_BASE + (0x1006f4 <<1 )), 0x0000);  
    WREG16((REG_ADDR_BASE + (0x1006f6 <<1 )), 0x0000);
#endif
    WREG16((REG_ADDR_BASE + (0x10121e <<1 )), 0x0c08);  // SW initial done and turn on arbitor     
    // WREG    (MS_BUS_RIU_ADDR + (0x1012fc <<1 )), 0x8400
 #if MIU_NUMBERS > 1 
    WREG16((REG_ADDR_BASE + (0x10061e <<1 )), 0x0c08);  // SW initial done and turn on arbitor     
    // WREG    (MS_BUS_RIU_ADDR + (0x1006fc <<1 )), 0x8400
#endif

}
#else

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

static void BootRom_MiuDdr3Dqs(MS_U8 u8MiuDev)
{
    MS_U16 u16RegVal0;
#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    MS_U16 u16RegVal1;
#endif
    MS_U16 u16RegMiuArb;
    MS_U16 u16RegMiuAtop;
    MS_U16 u16RegMiuDtop;
    MS_U16 u16DQSMaxCenter[4];
    MS_U16 u16KCode;
    MS_U16 u16KCodeOffset[4];
    MS_U8 i = 0;

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
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);

    /* Unmask MIU_group_4 bit15 */
    MHal_MIU_WritReg16(u16RegMiuArb, 0x06, 0x7fff);

    /* Set test length */
    //MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x1001);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe6, 0x0000);

    /* Enable SI mode */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);

    /*Enable miu_test limit mask*/
    MHal_MIU_WritReg16(u16RegMiuArb, 0x1c, 0x8000);

    /* DPAT engine pre-setting */
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0004);
    MHal_MIU_WritReg16(MIU0_RIU_ARB, 0x78, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x72, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x74, 0x0d83);
    MHal_MIU_WritReg16(u16RegMiuArb, 0x7c, 0x8000);

    /* STEP 1: Set RX DQS Phase = 1/2* Kcode + offset */
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0000);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x90, 0xf0f1);
    MHal_MIU_WritReg16(u16RegMiuAtop, 0x70, 0x0800);


#ifdef MIU_ENABLE_AUTO_WRITE_PHASE
    /* STEP 2 : Enable TX DQ deskew scan (by DPAT engine) */
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0090);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c0);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xd4, 0x11c1);

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

static void BootRom_MiuDdr3Dqs_v2(MS_U8 u8MiuDev)
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
    MHal_MIU_WritReg16(MIU1_RIU_DTOP, 0xf8, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe0, 0x0000);
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);

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
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0x28, MHal_MIU_ReadReg16(u16RegMiuDtop, 0x28) | 0x01); // multi bank enable
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe4, 0x0080);  // bist length
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
    //    MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    //}
    //else if (u8MiuDev == 1)
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x8000);
    //    MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x0000);
    //}
    //else
    //{
    //    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xf8, 0x0000);
    //    MHal_MIU_WritReg16(MIU2_RIU_DTOP, 0xf8, 0x8000);
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

    MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x0040);
    MHal_MIU_WritReg16(u16RegMiuDtopE, 0x80, 0x0010);

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

#define BIST_LEN    0x1001

static void Miu_BistTest(MS_U8 u8MIU)
{

    MS_U16 u16RegVal0;

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
    MHal_MIU_WritReg16(MIU0_RIU_DTOP, 0xe2, 0x0000);

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

static void ATOP_Trim_Calculate(MS_U16 *u16data, MS_U32 otp_date)
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

static void ATOP_Trim_Calculate_v2(MS_U16 *u16data, MS_U32 otp_date)
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

static void MIU_ATOP_Trim_Reg_Setup(MS_U32 u64Reg0, MS_U32 u64Reg1, MS_U8 u8offset0, MS_U8 u8offset1, MS_U32 u32otpvalue)
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

static void MIU_ATOP_Trim_Reg_Process(MS_U32 u32Reg0, MS_U32 u32Reg1, MS_U8 u8offset0, MS_U8 u8offset1, MS_U32 u32otpvalue)
{
    MS_U16 u16Data, u16Temp;
    #define DRV_MAX 0x001F
    #define DRV_MIN 0x0000

    u16Temp = (((RREG16(u32Reg0)) >> u8offset0) & 0x0001);
    u16Data = ((u16Temp << 4) | (((RREG16(u32Reg1)) >> u8offset1) & 0x000f)) & 0x001f;

    switch(u32otpvalue)
    {
        case 0x111:
            if ((DRV_MAX - u16Data) < 6) u16Data = DRV_MAX;
            else    u16Data += 6;
            break;

        case 0x110:
            if ((DRV_MAX - u16Data) < 4) u16Data = DRV_MAX;
            else    u16Data += 4;
            break;

        case 0x101:
            if ((DRV_MAX - u16Data) < 2) u16Data = DRV_MAX;
            else    u16Data += 2;
            break;

        default:
        case 0x000:
            break;

        case 0x001:
            if (u16Data < 2) u16Data = DRV_MIN;
            else    u16Data -= 2;
            break;

        case 0x010:
            if (u16Data < 4) u16Data = DRV_MIN;
            else    u16Data -= 4;
            break;

        case 0x011:
            if (u16Data < 6) u16Data = DRV_MIN;
            else    u16Data -= 6;
            break;
    }


    u16Temp = (RREG16(u32Reg1)) | ((u16Data & 0x000f) << u8offset1);
    WREG16(u32Reg1, u16Temp);

    u16Temp = (RREG16(u32Reg0)) | (((u16Data & 0x0010) >> 4) << u8offset0);
    WREG16(u32Reg0, u16Temp);

}

static void MIU_ATOP_Trim(void)
{
    MS_U32 otp = 0;
    MS_U32 atop0_reg_2c = 0x1F000000 + (0x110d00 << 1) + (0x2c << 2);
    MS_U32 atop0_reg_2d = 0x1F000000 + (0x110d00 << 1) + (0x2d << 2);
    MS_U32 atop0_reg_2e = 0x1F000000 + (0x110d00 << 1) + (0x2e << 2);
    MS_U32 atop0_reg_2f = 0x1F000000 + (0x110d00 << 1) + (0x2f << 2);
    MS_U32 atop0_reg_6f = 0x1F000000 + (0x110d00 << 1) + (0x6f << 2);
    MS_U32 atop1_reg_2c = 0x1F000000 + (0x161600 << 1) + (0x2c << 2);
    MS_U32 atop1_reg_2d = 0x1F000000 + (0x161600 << 1) + (0x2d << 2);
    MS_U32 atop1_reg_2e = 0x1F000000 + (0x161600 << 1) + (0x2e << 2);
    MS_U32 atop1_reg_2f = 0x1F000000 + (0x161600 << 1) + (0x2f << 2);
    MS_U32 atop1_reg_6f = 0x1F000000 + (0x161600 << 1) + (0x6f << 2);

    // DDR_ATOP
    // MIU0 drvp
    otp = (READ_OTP_8(0x3CE9) & 0x80) >> 7;
    if (otp)
    {
        //drvp_cs
        otp = (READ_OTP_8(0x3CE9) & 0x70) >> 4;
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
    otp = (READ_OTP_8(0x3CE9) & 0x8) >> 3;
    if (otp)
    {
        //drvn_cs
        otp = (READ_OTP_8(0x3CE9) & 0x7);
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




#ifdef CONFIG_MSTAR_KAISER
static void _InitParam_End(MS_U8 u8POWER_Flag,E_DDR_TYPE eDRAM_Type)
{
    if (u8POWER_Flag == AC_ON)
    {
#if CONFIG_MSTAR_STR_ENABLE
        MS_U16 Temp;
        Temp = RREG16(REG_ADDR_BASE + (0x110D00 << 1));
        WREG16(REG_ADDR_BASE + (0x110D00 << 1), Temp & (~0x0008));
#if MIU_NUMBERS > 1   
        Temp = RREG16(REG_ADDR_BASE + (0x110D80 << 1));
        WREG16(REG_ADDR_BASE + (0x110D80 << 1), Temp & (~0x0008)); 
#endif
        _Disable_ATOP_SW();
#endif
    }
    
    _MIU0_InitCycle();
#if MIU_NUMBERS > 1
    _MIU1_InitCycle();
#endif

#if CONFIG_MSTAR_STR_ENABLE
    if (u8POWER_Flag == DC_ON)
    {  
        //--------------- STR ---------------//
        _Enter_Refresh_Mode();
        _Disable_ATOP_SW();
        _Exit_Refresh_Mode();
        //--------------- STR ---------------//
    }
#endif
    
    _MIU_InitPriority();

    if (u8POWER_Flag == AC_ON) 
    {
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
        HAL_AutoPhaseScan();
#else
        _MIU_Bist();
#endif
    }
    
    _MIU_ClearIPMask();
}
#else // if not KAISER
static void _InitParam_End(MS_U8 u8POWER_Flag,E_DDR_TYPE eDRAM_Type)
{
#if defined(CONFIG_MSTAR_K6) || defined(CONFIG_MSTAR_K6LITE) || defined(CONFIG_MSTAR_K7U)
    chip_InitParam_End(u8POWER_Flag,eDRAM_Type);
#else

#if !defined(CONFIG_MSTAR_TITANIA_BD_FPGA)
    MS_U16 u16Regdata = 0;

    // Get MIU1 initial status to decide use MIU1 or not
    u16UseMIU1 = (*(volatile MS_U16*)(0x1F200000 + (0x0600 << 1))) & 0x1;

    _MIU_Check_Status();

#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_CURRY)
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
#endif

#if defined(CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE)
#if defined(CONFIG_MSTAR_CURRY)
    if (u8POWER_Flag == AC_ON)
    {
        BootRom_MiuDdr3Dqs_v2(0);
    }
#else
    BootRom_MiuDdr3Dqs(0);
#endif
    if(u16UseMIU1)
    {
        BootRom_MiuDdr3Dqs(1);
    }
#endif

#if defined(CONFIG_MSTAR_KANO) || defined(CONFIG_MSTAR_CURRY)
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
#endif

    DelayMS(1);

    MIU_ATOP_Trim();

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

#endif // defined(CONFIG_MSTAR_K6)
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
    MS_U32 rbase = (0x1f20 << 16);
    MS_U32 width = 16;
    MS_U8  u8POWER_Flag=0;
    E_DDR_TYPE eDRAM_Type = E_DDR_NA;

#if defined(CONFIG_HASH1_CMDS_MULTI_BLKS) && (CONFIG_HASH1_CMDS_MULTI_BLKS==1)
    MS_U8 u8DDR_Brand_Type = 0;
    MS_U8 bOutsideStartEndTag = 1;

    #if defined(CONFIG_MSTAR_K6)
    //u8DDR_Brand_Type[1:0]= gpio7:gpio6
    u8DDR_Brand_Type = ((RREG16(REG_ADDR_BASE + (0x1025be <<1))&1)<<1) | (RREG16(REG_ADDR_BASE + (0x1025bc <<1))&1);
    #endif    

    #if defined(CONFIG_MSTAR_K6LITE) || defined(CONFIG_MSTAR_K7U)
    //u8DDR_Brand_Type[2:0]= BOND_SW2:BOND_SW1:BOND_SW0
    u8DDR_Brand_Type = RREG16(REG_ADDR_BASE + (0x000ec8<<1))&0x7;
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

    gEntrys = pParams;
    //_PUTC('A');
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

            case CHIPCFG_CMD_AVD_RESET:
#ifdef CONFIG_MSTAR_KAISER
#if CONFIG_MSTAR_STR_ENABLE
                if (u8POWER_Flag == DC_ON)
                    _Reset_AVD();
#endif
#endif
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
   // _PUTC('B');
                
    _InitParam_End(u8POWER_Flag,eDRAM_Type);

    WREG_DBG(0xF001, 1);
    return 1;
}


