#include "../../include/autoconf.h"
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
ENTRY(Boot_Entry)


MEMORY
{
    /* ARM dspram address */
    spram (!x)  :   ORIGIN = 0x10008000,        LENGTH = 16K   /*0x10008000~0x1000C000*/

    /* reset address */
    boot  (rx)  :   ORIGIN = 0x00000000,        LENGTH = 0x1300
    rom   (rx)  :   ORIGIN = 0x14001300,        LENGTH = 0x3D00
    flash (rx)  :   ORIGIN = 0x14005000,        LENGTH = 16K
    hash2 (rx)  :   ORIGIN = CONFIG_HASH2_START_ADDRESS,        LENGTH = 80K

    app   (rx)  :   ORIGIN = 0x21000000,        LENGTH = 8M

    /* simulation */
    dummy       :   ORIGIN = 0xE0000000,        LENGTH = 0x100000
    garbage     :   ORIGIN = 0xF0000000,        LENGTH = 0x100000    
}

_ld_load_start = 0x14000000;

SECTIONS
{
    .boot : AT ( _ld_load_start )
    {
        #if ENABLE_MSTAR_RELEASE_W_OTP==1
        FILL(0xFFFFFFFF);
        #else
        obj/boot.o (.text)
        #endif

        . = 0x1300-4;
        LONG(0xDEADBEEF)
    } > boot

    .rom :
    {
        #if ENABLE_MSTAR_RELEASE_W_OTP==1
        FILL(0xFFFFFFFF);
        #else
        obj/bootrom.o (.text)
        obj/bootrom_c.o (.text)
        obj/bootrom_c.o (.rodata*)
        obj/bootrom.o (.data)

        obj/bootrom_miu.o (.text*)
        obj/bootrom_miu.o (.rodata*)

        /*obj/bootrom_emmc.o (.text*)
        obj/bootrom_emmc.o (.rodata*) */
        #endif
        . = 0x3f28 - 0x300; 
        obj/otp_revision.o(.otpcfg)
    } > rom

    _ld_HASH0_load_start = LOADADDR(.flash);
    .flash :
    {
        /* HASH0 */
        _ld_HASH0_run_start = .;
        . = ALIGN(0x10);
        /* reseved for Irdeto SIG2 */
        . = 0x0200;
/*
        _ld_SIG2_load_start = .;
*/
        . = 0x0300;
/*
        _ld_HASH0_run_end = .;
*/
        /* Signature Area: */
        . = 0x0800;
/*
        _ld_SIG0_load_start = .;
*/
        . = 0x0A00;
/*
        _ld_SIG1_load_start = .;
*/
        . = 0x0C00;
/*
        _ld_SIG2_load_start = .;
*/

        . = 0x1000;
        _ld_HASH1_run_start = .;
        obj/param.o (.hash1)
        _ld_HASH1_run_hash2_info = .;
        LONG( _ld_LDR_run_base );
        LONG( SIZEOF(.hash2) );
        _ld_HASH1_run_end = .;

    } > flash


    /DISCARD/ :
    {
        *(.reginfo)
    }

    _ld_LDR_load_start = 0x14007000;   /*0x14004000*/
    .hash2 : AT (_ld_LDR_load_start)
    {
        _ld_LDR_run_base = .;
        LONG(0x3052444C);                                   /* TAG:LDR0                 */
        LONG( _ld_LDR_run_start - _ld_LDR_run_base);        /* ldr_entry offset         */
        LONG( _ld_LDR_stack_end );                          /* ldr_stack_end            */
        LONG( SIZEOF(.hash2) );                             /* For backward compatible  */
        . = ALIGN(0x100);

        _ld_LDR_run_start = .;
        obj/loader.o (.ldr.entry)
        obj/loader.o (.text)
        obj/loader.o (.bss*)
        obj/loader.o (*COMMON)
        obj/bootram.o (.text)
        obj/loader.o (.data)
        obj/bootram.o (.date)
        obj/loader.o (.rodata*)
        obj/cache.o (.text)
        obj/cache.o (.data)
        obj/cache.o (.bss)
        obj/lzmadec.o (.text*)
        obj/lzmadec.o (.rodata*)
        obj/lzmadec.o (.data*)
        obj/lzmadec.o (.bss*)

        obj/nand.o (.text*)
        obj/drvNAND_hal.o (.text*)
        obj/drvNAND_utl.o (.text*)
        obj/drvNAND_platform.o (.text*)
        obj/drvNAND_hal_v5.o (.text*)
        obj/eMMC_prg.o (.text*)
        obj/eMMC_hal_v5.o (.text*)
        obj/eMMC_hal_speed.o (.text*)
        obj/eMMC_platform.o (.text*)
        obj/nand.o (.rodata*)
        obj/drvNAND_hal.o (.rodata*)
        obj/drvNAND_utl.o (.rodata*)
        obj/drvNAND_platform.o (.rodata*)
        obj/drvNAND_hal_v5.o (.rodata*)
        obj/eMMC_prg.o (.rodata*)
        obj/eMMC_hal_v5.o (.rodata*)
        obj/eMMC_hal_speed.o (.rodata*)
        obj/eMMC_platform.o (.rodata*)
        obj/nand.o (.data*)

#if (CONFIG_MSTAR_NS_UBOOT == 1)
        obj/drvSEAL.o (.text)
        obj/halSEAL.o (.text)
        obj/halCIPHER.o (.text*)
        obj/halCIPHER.o (.rodata*)
        obj/halCIPHER.o (.data*)
        obj/drvCIPHER.o (.text*)
        obj/drvCIPHER.o (.rodata*)
        obj/drvCIPHER.o (.data*)
        obj/halRSA.o (.text*)
        obj/halRSA.o (.rodata*)
        obj/halRSA.o (.data*)
        obj/ns_uboot_loader.o (.text*)
        obj/ns_uboot_loader.o (.rodata*)
        obj/ns_uboot_loader.o (.data*)
        obj/ns_uboot_loader.o (.bss*)
#endif
        
        obj/drvNAND_hal.o (.data*)
        obj/drvNAND_utl.o (.data*)
        obj/drvNAND_platform.o (.data*)
        obj/drvNAND_hal_v5.o (.data*)
        obj/eMMC_prg.o (.data*)
        obj/eMMC_hal_v5.o (.data*)
        obj/eMMC_hal_speed.o (.data*)
        obj/eMMC_platform.o (.data*)
        obj/nand.o (.bss*)   
        obj/drvNAND_hal.o (.bss*)
        

        obj/drvNAND_utl.o (.bss*)
        
        
        obj/drvNAND_platform.o (.bss*)


        obj/drvNAND_hal_v5.o (.bss*)

        obj/eMMC_prg.o (.bss*)

        obj/eMMC_hal_v5.o (.bss*)

        obj/eMMC_hal_speed.o (.bss*)

        obj/eMMC_platform.o (.bss*)
        obj/eMMC_platform.o (*COMMON)

        . = ALIGN(0x800);
        obj/bootram_vector.o (.text*)
        obj/bootram_vector.o (.rodata*)

        *(.text.*)
        *(.text)
				
        . = ALIGN(0x100);
        _ld_LDR_run_end = .;

        obj/loader.o (.ldr.stack)
        obj/nand.o (.stack)
        obj/drvNAND_hal.o (.stack)
        obj/drvNAND_utl.o (.stack)
        obj/drvNAND_platform.o (.stack)
        obj/drvNAND_hal_v5.o (.stack)
        obj/eMMC_prg.o (.stack)
        obj/eMMC_hal_v5.o (.stack)
        obj/eMMC_hal_speed.o (.stack)
        obj/eMMC_platform.o (.stack)
        . += 8k;
        . = ALIGN(0x10);
        _ld_LDR_stack_end = .;
        LONG(0x00000000);                                               
        LONG(0x00000000);                                               
        LONG(0x00000000);                                               
        LONG(0x4552444C);                                               //TAG:LDRE
    } > hash2
    _ld_LDR_load_end = LOADADDR(.hash2)+SIZEOF(.hash2);

    /* Secured Firmware */
    .bin.sfw : AT ( _ld_LDR_load_end )
    {
    } > dummy                                                           /* virtual region for linker  */

    /* Unsecured Firmware */
    _ld_sfw_end = LOADADDR(.bin.sfw)+SIZEOF(.bin.sfw);
    .bin.fw : AT (_ld_sfw_end)
    {
        /*obj/pm.bin.o*/
        . = ALIGN(0x10);                                                /* aligned for bin.app header */
    } > dummy                                                           /* virtual region for linker  */


#if (CONFIG_MSTAR_NS_UBOOT == 1)
    .bin.app : AT ( LOADADDR(.boot)+257K )
#else
    .bin.app : AT ( LOADADDR(.boot)+129K )
#endif
    {
        LONG( . );                                                      /* run_base(16x)        */
        LONG( APP_ENTRY  );                                             /* app_entry offset     */
        LONG( 0x00000000 );
        LONG( SIZEOF(.bin.app) );
        . = 0x100;

        . = APP_BIN;                                                    /* ecos bin base        */
        _ld_APP_run_start = .;
        obj/app.bin.o (*)
        . = ALIGN(16);
    } > app
    _ld_APP_load_start = LOADADDR(.bin.app);
    _ld_APP_load_end = LOADADDR(.bin.app)+SIZEOF(.bin.app);

    .data :
    {
        . = 0x0000;
    } > spram

    .bss :
    {
        *(.bss)
    } > spram

    .stack :
    {
        _ld_STACK_start = ADDR(.data)+3K;
        FILL(0xDEADBEEF)

        *(.stack)

        . = 4K-SIZEOF(.data)-4;
        . = ALIGN(0x10);  
        _ld_STACK_end = .;
    } > spram

    /* Check garbage data */
    .garbage :
    {
/*      * (*) */
    } > garbage

}


