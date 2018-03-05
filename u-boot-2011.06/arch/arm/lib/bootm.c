/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 2001  Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <fdt.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <MsApiMiu.h>
#include "MsEnvironment.h"

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
#include <asm/armv7.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd);

# ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd);
# endif
static void setup_commandline_tag (bd_t *bd, char *commandline);

# ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag (bd_t *bd, ulong initrd_start,
			      ulong initrd_end);
# endif
static void setup_end_tag (bd_t *bd);

static struct tag *params;
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG || CONFIG_INITRD_TAG */

static ulong get_sp(void);
#if defined(CONFIG_OF_LIBFDT)
static int bootm_linux_fdt(int machid, bootm_headers_t *images);
#endif

void arch_lmb_reserve(struct lmb *lmb)
{
	ulong sp;

	/*
	 * Booting a (Linux) kernel image
	 *
	 * Allocate space for command line and board info - the
	 * address should be as high as possible within the reach of
	 * the kernel (see CONFIG_SYS_BOOTMAPSZ settings), but in unused
	 * memory, which means far enough below the current stack
	 * pointer.
	 */
	sp = get_sp();
	debug("## Current stack ends at 0x%08lx ", sp);

	/* adjust sp by 1K to be safe */
	sp -= 1024;
	lmb_reserve(lmb, sp,
		    gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size - sp);
}

#if 0
// MStar start
#define BOOTTIME_SBOOT_STR "BOOTTIME_SBOOT"
#define BOOTTIME_UBOOT_STR "BOOTTIME_UBOOT"
#include <stdlib.h>
#include <malloc.h>
#include "MsEnvironment.h"

unsigned long G_MS_BOOTTIME_SBOOT=1; // global variable for storing the boot time used in sboot (ms)
unsigned long G_MS_BOOTTIME_UBOOT=1; // global variable for storing the boot time used in sboot (ms)

static void _boottime_set_to_env(void)
{
    //extern unsigned long G_MS_BOOTTIME_SBOOT; // global variable for storing the boot time used in sboot (ms)
    //extern unsigned long G_MS_BOOTTIME_UBOOT; // global variable for storing the boot time used in sboot (ms)
    extern int snprintf(char *str, size_t size, const char *fmt, ...);
    const char *strBootArg = "bootargs";
    char *strEnv = NULL;
    char *strPrevEnv = NULL;
    const int u32EnvSize = 32;

    {
        unsigned int PiuTick = *(volatile unsigned int *)(0x1f006090);
        PiuTick |= (*(volatile unsigned int *)(0x1f006094)) << 16;
        unsigned int PiuTime = PiuTick / 12000;
        G_MS_BOOTTIME_UBOOT = (unsigned long)PiuTime;
    }

    strEnv = malloc(u32EnvSize);
    if(strEnv  != NULL)
    {
        strPrevEnv = getenv((char*)strBootArg);
        if(strPrevEnv != NULL)
        {
            memset(strEnv , 0, u32EnvSize);
            snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_SBOOT_STR, G_MS_BOOTTIME_SBOOT);
            if(0 != set_bootargs_cfg((char*)strBootArg, strEnv, 1))
            {
                printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
            }

            //NOTE: getenv again because if has been updated
            strPrevEnv = getenv((char*)strBootArg);
            if(strPrevEnv != NULL)
            {
                memset(strEnv , 0,u32EnvSize);
                snprintf(strEnv , u32EnvSize-1, "%s=%lu", BOOTTIME_UBOOT_STR, G_MS_BOOTTIME_UBOOT);
                if(0 != set_bootargs_cfg((char*)strBootArg, strEnv, 1))
                {
                    printf("%s: Error: set_bootargs_cfg failed at %d\n", __func__, __LINE__);
                }
            }
        }

        free(strEnv );
    }
}
// MStar end
#endif
static void announce_and_cleanup(void)
{
    char* s = NULL;
    char* t = NULL;

    /* checkpoint for autotest boottime, plz dont remove it */
    unsigned int PiuTick = *(volatile unsigned int *)(0x1f006090);
    PiuTick |= (*(volatile unsigned int *)(0x1f006094)) << 16;
    unsigned int PiuTime = PiuTick / 12000;
    printf("[AT][MB][start kr][%u]\n", PiuTime);

	printf("\nStarting kernel ...\n\n");

    #ifdef STR_FOR_AGATE
	    *(volatile unsigned int*)(0x1f206700) = 0xdfdf;
	#else
	    extern void MsOS_DisableAllInterrupts(void);
        MsOS_DisableAllInterrupts();
    #endif

    s = getenv("sync_miuprot");
    t = getenv("KERNEL_PROTECT");

    if(!(strcmp(s, "2")))
    {
        MsApi_MiuProtect();
    }
    else if ((!(strcmp(s, "1"))) || (t != NULL))
    {
        MsApi_kernelProtect();
        s = getenv("kernelProtectBist");
        if (s != NULL)
        {
            MsApi_kernelProtectBist();
        }
    }

#ifdef CONFIG_USB_DEVICE
	{
		extern void udc_disconnect(void);
		udc_disconnect();
	}
#endif
	cleanup_before_linux();
}

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
static void do_nonsec_virt_switch(void)
{
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
	if (armv7_switch_nonsec() == 0)
#ifdef CONFIG_ARMV7_VIRT
	if (armv7_switch_hyp() == 0)
		debug("entered HYP mode\n");
#else
	debug("entered non-secure state\n");
#endif
#endif
}
#endif

/*
 *	AMP Nuttx flow
 *	OPTEE flow
 *	ARM FW flow
 */
#include <MsTrustZone.h>
#ifdef CONFIG_ARMv7_OPTEE
typedef void (*optee_entry)(int zero, int arch, uint params, unsigned long kernel_entry);
#endif

extern unsigned int optee_real_va_addr;
#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
extern unsigned int armfw_real_va_addr;
#endif

#if defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)
void ARM_FW_Enter_64(unsigned long cmd_ptr, ulong ramdisk_start, ulong ramdisk_len)
{
#ifndef CONFIG_MSTAR_NS_UBOOT //SMP ARMv8 optee flow with 64bit kernel
	mstar_boot_prameters_t bootp;
	unsigned long Fireware_entry = armfw_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x00023000;//0xC0000000
	printf("armfw_real_va_addr : %x \n",armfw_real_va_addr);
	printf("optee_real_va_addr : %x \n",optee_real_va_addr);
	printf("\033[0;32;31m [Ian] %s %d  ARMv8 Uboot for 64bit kernel Enter ARM FW\033[m\n",__func__,__LINE__);
	printf("\033[0;32;31m [Ian] %s %d %x %x %x\033[m\n",__func__,__LINE__,ramdisk_start,ramdisk_len,cmd_ptr);

	bootp.BIN0_entry_point = optee_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x02100000;
	bootp.BIN0_RW64 = MODE_RW_32; //not use now
	bootp.BIN0_bootargs = 0;
	bootp.BIN1_entry_point = 0x20280000;
	bootp.BIN1_RW64 = MODE_RW_64;
	bootp.BIN1_r2 = cmd_ptr;
	bootp.BIN1_r3 = ramdisk_start;
	bootp.BIN1_r4 = ramdisk_len;
	bootp.BIN2_RW64 = MODE_RW_64;
	printf("\033[0;32;31m [Ian] %s %d %llx %llx\033[m\n",__func__,__LINE__,bootp.BIN0_entry_point,bootp.BIN1_entry_point);
	printf("\033[0;32;31m [Ian] %s %d %llx %llx %llx\033[m\n",__func__,__LINE__,bootp.BIN1_r2,bootp.BIN1_r3,bootp.BIN1_r4);
	cleanup_before_linux();
	__asm__ __volatile__(
		"ldr  r1,[%0]  \n\t"
		"mov  r0,%1  \n\t"
		"smc #0  \n\t"
		:
		:"r"(&Fireware_entry),"r"(&bootp)
		:"r0","r1"
	);
#elif defined(CONFIG_MSTAR_NS_UBOOT) //SMP ARMv8 optee flow with 64bit kernel for NS-Uboot
	printf("armfw_real_va_addr : %x \n",armfw_real_va_addr);
	printf("optee_real_va_addr : %x \n",optee_real_va_addr);
	printf("\033[0;32;31m [Ian] %s %d  ARMv8 NS-Uboot for 64bit kernel\033[m\n",__func__,__LINE__);
	printf("\033[0;32;31m [Ian] %s %d %x %x %x\033[m\n",__func__,__LINE__,ramdisk_start,ramdisk_len,cmd_ptr);

	__asm__ __volatile__(
	"ldr  r0, =0x8400000F\n\t"
	"ldr  r3, [%0]  \n\t"
	"ldr  r4, [%1]  \n\t"
	"ldr  r2, [%2]  \n\t"
	"smc #0         \n\t"
	:
	: "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(&cmd_ptr)
	: "r0","r3","r4","r2"
	);
#endif
}

void ARM_FW_Enter_32(int machid,char* bi_boot_params)
{
	mstar_boot_prameters_t bootp;
	printf("armfw_real_va_addr : %x \n",armfw_real_va_addr);
	unsigned long Fireware_entry = armfw_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x00023000;//0xC0000000
	printf("optee_real_va_addr : %x \n",optee_real_va_addr);
	bootp.BIN0_entry_point = optee_real_va_addr;//CONFIG_MIU1_BUSADDR + 0x02100000;
	bootp.BIN0_RW64 = MODE_RW_32; //not use now
	bootp.BIN0_bootargs = 0;
	bootp.BIN1_entry_point = 0x20208000;
	bootp.BIN1_RW64 = MODE_RW_32;
	printf("\033[0;32;31m [Ian] %s %d %llx %llx\033[m\n",__func__,__LINE__,bootp.BIN0_entry_point,bootp.BIN1_entry_point);
	bootp.BIN1_r0 = 0;
	bootp.BIN1_r1 = machid;
	bootp.BIN1_r2 = bi_boot_params;
	bootp.BIN2_RW64 = MODE_RW_32;
	cleanup_before_linux();
	__asm__ __volatile__(
		"ldr  r1,[%0]  \n\t"
		"mov  r0,%1  \n\t"
		"smc #0  \n\t"
		:
		:"r"(&Fireware_entry),"r"(&bootp)
		:"r0","r1"
	);
}
#endif

int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
{
	bd_t	*bd = gd->bd;
	char	*s;
	int	machid = bd->bi_arch_number;
	void	(*kernel_entry)(int zero, int arch, uint params);

#ifdef CONFIG_CMDLINE_TAG
	char *commandline = getenv ("bootargs");
#endif

	if ((flag != 0) && (flag != BOOTM_STATE_OS_GO))
		return 1;

	s = getenv ("machid");
	if (s) {
		machid = simple_strtoul (s, NULL, 16);
		printf ("Using machid 0x%x from environment\n", machid);
	}

	show_boot_progress (15);

#ifdef CONFIG_OF_LIBFDT
	if (images->ft_len)
		return bootm_linux_fdt(machid, images);
#endif

	kernel_entry = (void (*)(int, int, uint))images->ep;

	debug ("## Transferring control to Linux (at address %08lx) ...\n",
	       (ulong) kernel_entry);

//MStar start
    _boottime_set_to_env();
#ifdef CONFIG_CMDLINE_TAG
    commandline = getenv ("bootargs"); // NOTE: should get agaig, becuase the env has been updated
#endif
//MStar end
#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
	setup_start_tag (bd);
#ifdef CONFIG_SERIAL_TAG
	setup_serial_tag (&params);
#endif
#ifdef CONFIG_REVISION_TAG
	setup_revision_tag (&params);
#endif
#ifdef CONFIG_SETUP_MEMORY_TAGS
	setup_memory_tags (bd);
#endif
#ifdef CONFIG_CMDLINE_TAG
	setup_commandline_tag (bd, commandline);
#endif
#ifdef CONFIG_INITRD_TAG
	if (images->rd_start && images->rd_end)
		setup_initrd_tag (bd, images->rd_start, images->rd_end);
#endif
	setup_end_tag(bd);
#endif

	announce_and_cleanup();

#if defined (CONFIG_ARMv8_64BIT_KERNEL) //Kernel 64bit
    //there is no atags pointer within 64 bit kernel, so we need to pass bootargs to kernel
    char* __cmdline = bd->bi_boot_params;
    //cmd_ptr is used to save pointer of command line
    unsigned long cmd_ptr = (unsigned long)bd->bi_boot_params;
    strncpy(__cmdline,commandline,strlen(commandline));
    printf("set 64 bit mode \n");

    extern ulong ramdisk_start,ramdisk_len;

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	ARM_FW_Enter_64(cmd_ptr,ramdisk_start,ramdisk_len);
#else //ARMv8 Normal boot flow with 64bit kernel
	__asm__ __volatile__(
#if (CONFIG_GIC_PATCH == 1)
	"ldr     r3, =0x16001080\n\t"
	"ldr     r2, [r3]\n\t"
	"ldr     r4, =(0x3 << 30)\n\t"
	"orr     r2, r2, r4\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16001000\n\t"
	"mov     r2, #3\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16002000\n\t"
	"mov     r2, #3\n\t"
	"str     r2, [r3]\n\t"
	"ldr     r3, =0x16002004\n\t"
	"mov     r2, #0xf8\n\t"
	"str     r2, [r3]\n\t"
#endif
	"ldr  r3, [%0]  \n\t"
	"ldr  r4, [%1]  \n\t"
	"ldr  r2, [%2]  \n\t"
	:
	: "r"(&ramdisk_start),"r"(&ramdisk_len),"r"(&cmd_ptr)
	: "r3","r4","r2"
	);
	__asm__ __volatile__("smc #0": );
#endif

#else //Kernel 32bit
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT) //AMP Nuttx flow
	do_nonsec_virt_switch();
	kernel_entry(0, machid, bd->bi_boot_params);
#elif defined(CONFIG_ARMv7_OPTEE) && !defined(CONFIG_MSTAR_NS_UBOOT) //SMP ARMv8 optee flow with 32bit kernel
	optee_entry entry;
	entry = (optee_entry) optee_real_va_addr; //here have to modify by tee.bin
	entry(0, machid, bd->bi_boot_params,(unsigned long)kernel_entry); //r0 r1 r2 r3=kernel_entry
	asm volatile( //because optee return to mboot will entry sm_set_nsec_ret_vals so we have to move back
		"mov r3,r2\n\t"
		"mov r2,r1\n\t"
		"mov r1,r0\n\t"
		"ldr r0,=0x0\n\t"
		"mov pc,r3\n\t":::
	);
#elif defined(CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE) && !defined(CONFIG_MSTAR_NS_UBOOT) //SMP ARMv8 optee flow with 32bit kernel
	ARM_FW_Enter_32(machid, bd->bi_boot_params);
#else // ARMv8 and ARMv7 Normal boot flow with 32bit kernel & NS_UBOOT
	kernel_entry(0, machid, bd->bi_boot_params);
	/* does not return */
#endif
#endif
	return 1;
}

#if defined(CONFIG_OF_LIBFDT)
static int fixup_memory_node(void *blob)
{
	bd_t	*bd = gd->bd;
	int bank;
	u64 start[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	for (bank = 0; bank < CONFIG_NR_DRAM_BANKS; bank++) {
		start[bank] = bd->bi_dram[bank].start;
		size[bank] = bd->bi_dram[bank].size;
	}

	return fdt_fixup_memory_banks(blob, start, size, CONFIG_NR_DRAM_BANKS);
}

static int bootm_linux_fdt(int machid, bootm_headers_t *images)
{
	ulong rd_len;
	void (*kernel_entry)(int zero, int dt_machid, void *dtblob);
	ulong of_size = images->ft_len;
	char **of_flat_tree = &images->ft_addr;
	ulong *initrd_start = &images->initrd_start;
	ulong *initrd_end = &images->initrd_end;
	struct lmb *lmb = &images->lmb;
	int ret;

	kernel_entry = (void (*)(int, int, void *))images->ep;

	boot_fdt_add_mem_rsv_regions(lmb, *of_flat_tree);

	rd_len = images->rd_end - images->rd_start;
	ret = boot_ramdisk_high(lmb, images->rd_start, rd_len,
				initrd_start, initrd_end);
	if (ret)
		return ret;

	ret = boot_relocate_fdt(lmb, of_flat_tree, &of_size);
	if (ret)
		return ret;

	debug("## Transferring control to Linux (at address %08lx) ...\n",
	       (ulong) kernel_entry);

	fdt_chosen(*of_flat_tree, 1);

	fixup_memory_node(*of_flat_tree);

	fdt_initrd(*of_flat_tree, *initrd_start, *initrd_end, 1);

	announce_and_cleanup();

	kernel_entry(0, machid, *of_flat_tree);
	/* does not return */

	return 1;
}
#endif

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd)
{
	params = (struct tag *) bd->bi_boot_params;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}


#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd)
{
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = bd->bi_dram[i].start;
		params->u.mem.size = bd->bi_dram[i].size;

		params = tag_next (params);
	}
}
#endif /* CONFIG_SETUP_MEMORY_TAGS */


static void setup_commandline_tag (bd_t *bd, char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}


#ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag (bd_t *bd, ulong initrd_start, ulong initrd_end)
{
	/* an ATAG_INITRD node tells the kernel where the compressed
	 * ramdisk can be found. ATAG_RDIMG is a better name, actually.
	 */
	params->hdr.tag = ATAG_INITRD2;
	params->hdr.size = tag_size (tag_initrd);

	params->u.initrd.start = initrd_start;
	params->u.initrd.size = initrd_end - initrd_start;

	params = tag_next (params);
}
#endif /* CONFIG_INITRD_TAG */

#ifdef CONFIG_SERIAL_TAG
void setup_serial_tag (struct tag **tmp)
{
	struct tag *params = *tmp;
	struct tag_serialnr serialnr;
	void get_board_serial(struct tag_serialnr *serialnr);

	get_board_serial(&serialnr);
	params->hdr.tag = ATAG_SERIAL;
	params->hdr.size = tag_size (tag_serialnr);
	params->u.serialnr.low = serialnr.low;
	params->u.serialnr.high= serialnr.high;
	params = tag_next (params);
	*tmp = params;
}
#endif

#ifdef CONFIG_REVISION_TAG
void setup_revision_tag(struct tag **in_params)
{
	u32 rev = 0;
	u32 get_board_rev(void);

	rev = get_board_rev();
	params->hdr.tag = ATAG_REVISION;
	params->hdr.size = tag_size (tag_revision);
	params->u.revision.rev = rev;
	params = tag_next (params);
}
#endif  /* CONFIG_REVISION_TAG */

static void setup_end_tag (bd_t *bd)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG || CONFIG_INITRD_TAG */

static ulong get_sp(void)
{
	ulong ret;

	asm("mov %0, sp" : "=r"(ret) : );
	return ret;
}
