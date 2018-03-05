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
/// file    cache.c
/// @brief  Loader code running on RAM
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boot.h"
#include "chip.h"

// SCTLR_EL3 bits definitions
#define CTLR_M		(1 << 0)	// MMU enable
#define CTLR_C		(1 << 2)	// Dcache enable
#define CTLR_I		(1 << 12)	// Icache enable

#define SECTION_SHIFT		(29UL)

// Memory types
#define MT_DEVICE_NGNRNE	0
#define MT_DEVICE_NGNRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		    4

//TCR flags.
#define TCR_T0SZ(x)		((64 - (x)) << 0)
#define TCR_IRGN0_NC		(0 << 8)
#define TCR_IRGN0_WBWA		(1 << 8)
#define TCR_IRGN0_WT		(2 << 8)
#define TCR_IRGN0_WBNWA		(3 << 8)
#define TCR_IRGN0_MASK		(3 << 8)
#define TCR_ORGN0_NC		(0 << 10)
#define TCR_ORGN0_WBWA		(1 << 10)
#define TCR_ORGN0_WT		(2 << 10)
#define TCR_ORGN0_WBNWA		(3 << 10)
#define TCR_ORGN0_MASK		(3 << 10)
#define TCR_SH0_NON		(0 << 12)
#define TCR_SH0_OUTER	(1 << 12)
#define TCR_SH0_INNER	(2 << 12)
#define TCR_TG0_4K		(0 << 14)
#define TCR_TG0_64K		(1 << 14)
#define TCR_TG0_16K		(2 << 14)
#define TCR_PS_BITS	(3 << 16)

//Hardware Level 2 page table definitions.
#define PMD_TYPE_SECT		(1 << 0)

#define PMD_SECT_AF		(1 << 10)

#define TCR_FLAG		(TCR_TG0_64K |		\
				TCR_SH0_NON |	\
				TCR_ORGN0_WBWA |		\
				TCR_IRGN0_WBWA |		\
				TCR_T0SZ(42))

#define MEMORY_ATTR	((0x00 << (MT_DEVICE_NGNRNE*8)) |	\
	                 (0x04 << (MT_DEVICE_NGNRE*8)) |		\
		         (0x0c << (MT_DEVICE_GRE*8)) |		\
		         (0x44 << (MT_NORMAL_NC*8)) |		\
			 ((0xffUL) << (MT_NORMAL*8)))

static inline unsigned int get_sctlr_el3(void)
{
    unsigned int val;

#if defined (CONFIG_TEE_LOADER)
    asm volatile(
        "mrs %0, sctlr_el2"
        : "=r" (val) : : "cc"
    );
#else
    asm volatile(
        "mrs %0, sctlr_el3"
        : "=r" (val) : : "cc"
    );
#endif

    return val;
}

static inline void set_sctlr_el3(MS_U32 val)
{
#if defined (CONFIG_TEE_LOADER)
    asm volatile(
        "msr sctlr_el2, %0"
        : : "r" (val) : "cc"
    );
#else
    asm volatile(
        "msr sctlr_el3, %0"
        : : "r" (val) : "cc"
    );
#endif

    asm volatile("isb");
}

void page_table_section_setup(MS_U64 *table_addr, MS_U64 index, MS_U64 memory_type)
{
    MS_U64 value = 0;
    MS_U64 section = index << SECTION_SHIFT;

    value = section | PMD_TYPE_SECT | PMD_SECT_AF;
    value |= memory_type << 2;
    table_addr[index] = value;
}

static inline void setup_ttbr_tcr_mair(MS_U64 table, MS_U64 tcr, MS_U64 attr)
{
    asm volatile("dsb sy");

#if defined (CONFIG_TEE_LOADER)
    asm volatile("msr ttbr0_el2, %0" : : "r" (table) : "memory");
    asm volatile("msr tcr_el2, %0" : : "r" (tcr) : "memory");
    asm volatile("msr mair_el2, %0" : : "r" (attr) : "memory");
#else
    asm volatile("msr ttbr0_el3, %0" : : "r" (table) : "memory");
    asm volatile("msr tcr_el3, %0" : : "r" (tcr) : "memory");
    asm volatile("msr mair_el3, %0" : : "r" (attr) : "memory");
#endif
	
    asm volatile("isb");
}

static void mmu_initial(void)
{
    MS_U64 *table_addr = (MS_U64 *)0x2F000000UL;
    MS_U32 sctlr;

    // VA : 0x00000000 ~ 0x1FFFFFFF ; PA : 0x00000000 ~ 0x1FFFFFFF ; Type : Device
    page_table_section_setup(table_addr, 0, MT_DEVICE_NGNRNE);

    // VA : 0x20000000 ~ 0x3FFFFFFF ; PA : 0x20000000 ~ 0x3FFFFFFF ; Type : Memory Cache
    page_table_section_setup(table_addr, 1, MT_NORMAL);

    // VA : 0x40000000 ~ 0x5FFFFFFF ; PA : 0x20000000 ~ 0x5FFFFFFF ; Type : Memory Cache
    page_table_section_setup(table_addr, 2, MT_NORMAL);

    // setup TTBR
    setup_ttbr_tcr_mair((MS_U64)table_addr, TCR_FLAG | TCR_PS_BITS, MEMORY_ATTR);

    // enable the mmu
    set_sctlr_el3(get_sctlr_el3() | CTLR_M);
}

void icache_enable(void)
{
    MS_U32 sctlr;

    invalidate_icache_all();

    sctlr = get_sctlr_el3() | CTLR_I;
	
    set_sctlr_el3(sctlr);
}

void icache_disable(void)
{
    MS_U32 sctlr;

    sctlr = get_sctlr_el3() & ~CTLR_I;
	
    set_sctlr_el3(sctlr);
}

static unsigned int current_el(void)
{
    unsigned int el;
    __asm__ __volatile__(
        "mrs %0, CurrentEL" : "=r" (el) : : "cc"
    );
    return el >> 2;
}

void dcache_enable(void)
{
    MS_U32 sctlr;
    unsigned int el;
    el = current_el();

    invalidate_dcache_all();
    if(el == 2)
    {
        invalidate_tlb_all_el2();
    }
    else if(el == 3)
    {
        invalidate_tlb_all();
    }
    mmu_initial();

    sctlr = get_sctlr_el3() | CTLR_C;

    set_sctlr_el3(sctlr);
}

void dcache_disable(void)
{
    MS_U32 sctlr;
    unsigned int el;
    el = current_el();

    sctlr = get_sctlr_el3();

    if (!(sctlr & CTLR_C))
        return;

    sctlr = sctlr & ~(CTLR_C|CTLR_M);

    set_sctlr_el3(sctlr);

    flush_dcache_all();
    if(el == 2)
    {
        invalidate_tlb_all_el2();
    }
    else if(el == 3)
    {
        invalidate_tlb_all();
    }
}
