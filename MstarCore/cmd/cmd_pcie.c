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

/*
 * PCIE test Utilities
 */
#include <common.h>
#include <command.h>
#include <exports.h>
#include "MsTypes.h"

#define readb(addr)			*((unsigned char volatile *)(addr))
#define writeb(val, addr)	(*((unsigned char volatile *)(addr)) = (unsigned char)val)
#define readw(addr) 		*((volatile u16 *)(addr))
#define writew(val, addr)	(*((volatile u16 *)(addr)) = (u16)val)
#define readl(addr) 		*((volatile u32 *)(addr))
#define writel(val, addr)	(*((volatile u32 *)(addr)) = (u32)val)

#define BIT0			0x01
#define BIT1			0x02
#define BIT2			0x04
#define BIT3			0x08
#define BIT4			0x10
#define BIT5			0x20
#define BIT6			0x40
#define BIT7			0x80

int do_pcietest(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return cmd_usage(cmdtp);

	if (strncmp(argv[1], "read32", 6) == 0)
	{
		u32 	addr, addr_input, offset;

		addr_input = simple_strtol(argv[2], NULL, 16);
		if (addr_input == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}

		offset = addr_input & 0xFFFF;
		addr = ((addr_input & 0xFFFF0000) << 1) | 0x1F000000;    //Bank address
		addr |= offset;

		printf("0x%x: %x\n", addr_input, readl((void*) addr));
	}
	else if (strncmp(argv[1], "write32", 7) == 0)
	{
		u32 	addr, offset, uVal;

		addr = simple_strtol(argv[2], NULL, 16);
		if (addr == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}
		uVal = simple_strtol(argv[3], NULL, 16);
		if (uVal == 0)
		{
			printf("Invalid value...\n");
			return 1;
		}

		offset = addr & 0xFFFF;
		addr = ((addr & 0xFFFF0000) << 1) | 0x1F000000;    //Bank address
		addr |= offset;

		writel((u32) uVal, (void*) addr);
	}
	else if (strncmp(argv[1], "read", 4) == 0)
	{
		u32 	addr;
		u8	data_len = 32;

		addr = simple_strtol(argv[2], NULL, 16);
		if (addr == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}
		if (strncmp(argv[3], "8", 1) == 0)
			data_len = 8;
		else if (strncmp(argv[3], "16", 2) == 0)
			data_len = 16;

		if (data_len == 8)
		{
			printf("0x%x: %x\n", addr, readb((void*) addr));
		}
		else if (data_len == 16)
		{
			printf("0x%x: %x\n", addr, readw((void*) addr));
		}
		else if (data_len == 32)
		{
			printf("0x%x: %x\n", addr, readl((void*) addr));
		}
	}
	else if (strncmp(argv[1], "write", 5) == 0)
	{
		u32 	addr, uVal;
		u8	data_len = 32;

		addr = simple_strtol(argv[2], NULL, 16);
		if (addr == 0)
		{
			printf("Invalid addr...\n");
			return 1;
		}

		if (strncmp(argv[3], "8", 1) == 0)
			data_len = 8;
		else if (strncmp(argv[3], "16", 2) == 0)
			data_len = 16;

		uVal = simple_strtol(argv[4], NULL, 16);
		if (uVal == 0)
		{
			printf("Invalid value...\n");
			return 1;
		}

		if (data_len == 8)
			writeb((u8) uVal, (void*) addr);
		else if (data_len == 16)
			writew((u16) uVal, (void*) addr);
		else if (data_len == 32)
			writel((u32) uVal, (void*) addr);
	}

	return 1;
}


U_BOOT_CMD(
	pcietest,	5,	1,	do_pcietest,
	"PCIE test system",
	"read32 addr - read 32bits data from addr \n"
	"write32 addr value - write 32bits value to addr \n"
);

