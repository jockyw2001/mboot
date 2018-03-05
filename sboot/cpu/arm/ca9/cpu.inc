################################################################################
#
# Copyright (c) 2008-2010 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################


@-------------------------------------------------
@Macro: drv_clk_L2_clk_ratio
@0x05= 1:2
@0x07= 1:3
@0x09= 1:4
@-------------------------------------------------
.macro drv_clk_L2_clk_ratio
    WREG      0x1F203ADC, 0x0000
    @WREG_B    0x1F203ADC, 0x04	      @mcu:L2 = 2:1
    @WREG_B    0x1F203ADC, 0x05
    WREG_B    0x1F203ADC, 0x02	      @mcu:L2 = 3:2
    WREG_B    0x1F203ADC, 0x03
    WREG      0x16000000, 0x0020
    WREG      0x15000F80, 0x0001
    WFI
    WREG      0x1F201644, 0x8000    @switch to CPU CLK
.endm

@-------------------------------------------------
@Macro: drv_arm_exldr_exstr_enable
@-------------------------------------------------
.macro drv_arm_exldr_exstr_enable
     WREG    0x1F203114, 0x0030
.endm