################################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# (¡§MStar Confidential Information¡¨) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################
#include "boot.inc"
#include "chip.inc"
#include "macro.inc"

#if defined (CONFIG_TEE_LOADER)
#//================================================================================================
#//ree_entry
#//================================================================================================
.globl      ree_entry
ree_entry:
    PUTC 'r'
    PUTC 'e'
    PUTC 'e'
    PUTC '_'
    PUTC 'e'
    PUTC 'n'
    PUTC 't'
    PUTC 'r'
    PUTC 'y'
    PUTC '\n'

    # Initial Stack Pointer
    ldr     r13, =_ld_REE_LDR_stack_end

    b  LDR_Entry
#endif
