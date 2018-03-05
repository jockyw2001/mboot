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


typedef unsigned short MS_U16;

#define BK_EFUSE(x) *(volatile MS_U16 *)(0xBF000000+(0x002000<<1)+((x)<<2))
#define BK_PM_SLP(x) *(volatile MS_U16 *)(0xBF000000+(0x000E00<<1)+((x)<<2))
#define BK_MIU_ATOP(x) *(volatile MS_U16 *)(0xBF000000+(0x110D00<<1)+((x)<<2))

/* avoid for compile warning */
void BootRomPreTrimSequence(void);
void BootRomPostTrimSequence(void);

void BootRomPreTrimSequence(void)
{
    MS_U16 u16EfVal;
    MS_U16 u16RegVal;

    /* PM_SAR_ATOP */
    //set reg_1x128_addr[3] to 1'b0
    BK_EFUSE(0x01) &= 0xFFF7;

    u16EfVal = BK_EFUSE(0x1D);
    //PM_vol_bgap
    u16RegVal = BK_PM_SLP(0x19);
    u16RegVal &= 0x00FF;
    u16RegVal |= ((u16EfVal>>2) & 0x3F00);
    BK_PM_SLP(0x19) = u16RegVal;

    //CLK_4M
    u16RegVal = BK_PM_SLP(0x30);
    u16RegVal &= 0xE1FF;
    u16RegVal |= ((u16EfVal<<4) & 0x1E00);
    BK_PM_SLP(0x30) = u16RegVal;

    #ifdef CONFIG_MSTAR_VID_ENABLE    //VID Initial Voltage Identification
        #if defined(CONFIG_MSTAR_KIWI)

        #define BK_PAD_TOP(x) *(volatile MS_U16 *)(0xBF000000+(0x102500<<1)+((x)<<2))

        MS_U16 u16ROSC = BK_EFUSE(0x35);
        MS_U16 u16VidThreshold = BK_EFUSE(0x32);

        #define BK_CHIP_TOP(x) *(volatile MS_U16 *)(0xBF000000+(0x101e00<<1)+((x)<<2))
        BK_CHIP_TOP(0x9) &= 0xFF8C;   //Set as to GPIO(the default vaule is not GPIO)

        //Set VID GPIO OEN to Output
        BK_PAD_TOP(0x10) &= 0xFFDF;
        if(u16VidThreshold==0x0000)
        {
            //VID GPIO Set High
            BK_PAD_TOP(0x10) |= 0x0010;
        }
        else if(u16ROSC > u16VidThreshold)
        {
            //VID GPIO Set Low
            BK_PAD_TOP(0x10) &= 0xFFEF;
        }
        else
        {
            //VID GPIO Set High
            BK_PAD_TOP(0x10) |= 0x0010;
        }

        #endif
    #endif  /*CONFIG_MSTAR_VID_ENABLE*/
}

void BootRomPostTrimSequence(void)
{
    MS_U16 u16EfVal;
    MS_U16 u16RegVal;

    /* DDR_ATOP - MIU_zq */
    /* check Activateefuse flag */
    u16EfVal = BK_EFUSE(0x19);
    if(u16EfVal & 0x0040)
    {
        u16RegVal = BK_MIU_ATOP(0x3D);
        u16RegVal &= 0x00FF;
        u16RegVal |= ((u16EfVal<<9) & 0x7E00);

        u16EfVal = BK_EFUSE(0x18);
        u16RegVal |= ((u16EfVal & 0x8000)?(0x0100):(0x0000));
        
        BK_MIU_ATOP(0x3D) = u16RegVal;
    }
}

