################################################################################
#
# Copyright (c) 2008-2012 MStar Semiconductor, Inc.
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



#aeon                    = 0 # for chip.inc register map
#mips                    = 1

#-------------------------------------------------------------------------------------------------
# CPU definition
#-------------------------------------------------------------------------------------------------
    .include "cpu.inc"


#-------------------------------------------------------------------------------------------------
# CHIP definition
#-------------------------------------------------------------------------------------------------

.if (ENABLE_CPU_288MHZ == 1)
CPU_CLK_FREQ            =                288000000           //[USER]
.elseif (ENABLE_CPU_312MHZ == 1)
CPU_CLK_FREQ            =                312000000           //[USER]
.elseif (ENABLE_CPU_324MHZ == 1)
CPU_CLK_FREQ            =                324000000           //[USER]
.elseif (ENABLE_CPU_336MHZ == 1)
CPU_CLK_FREQ            =                336000000           //[USER]
.elseif (ENABLE_CPU_396MHZ == 1)
CPU_CLK_FREQ            =                396000000           //[USER]
.elseif (ENABLE_CPU_432MHZ == 1)
CPU_CLK_FREQ            =                432000000           //[USER]
.elseif (ENABLE_CPU_456MHZ == 1)
CPU_CLK_FREQ            =                456000000           //[USER]
.elseif (ENABLE_CPU_504MHZ == 1)
CPU_CLK_FREQ            =                504000000           //[USER]
.elseif (ENABLE_CPU_552MHZ == 1)
CPU_CLK_FREQ            =                552000000           //[USER]
.elseif (ENABLE_CPU_600MHZ == 1)
CPU_CLK_FREQ            =                600000000           //[USER]
.elseif (ENABLE_CPU_624MHZ == 1)
CPU_CLK_FREQ            =                624000000           //[USER]
.elseif (ENABLE_CPU_648MHZ == 1)
CPU_CLK_FREQ            =                648000000           //[USER]
.elseif (ENABLE_CPU_696MHZ == 1)
CPU_CLK_FREQ            =                696000000           //[USER]
.elseif (ENABLE_CPU_720MHZ == 1)
CPU_CLK_FREQ            =                720000000           //[USER]
.elseif (ENABLE_CPU_792MHZ == 1)
CPU_CLK_FREQ            =                792000000           //[USER]
.elseif (ENABLE_CPU_840MHZ == 1)
CPU_CLK_FREQ            =                840000000           //[USER]
.elseif (ENABLE_CPU_900MHZ == 1)
CPU_CLK_FREQ            =                900000000           //[USER]
.elseif (ENABLE_CPU_984MHZ == 1)
CPU_CLK_FREQ            =                984000000           //[USER]
.elseif (ENABLE_CPU_1008MHZ == 1)
CPU_CLK_FREQ            =                1008000000           //[USER]
.else
CPU_CLK_FREQ            = 12000000
.endif


#-------------------------------------------------------------------------------------------------
# BOOT definition
#-------------------------------------------------------------------------------------------------

LDR_HDR_ENTRY           = 4
LDR_HDR_STACK           = 8


