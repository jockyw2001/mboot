#include "../../include/autoconf.h"
OUTPUT(elf32-littlemips)
ENTRY(BOOT_Entry)



MEMORY
{
    /* internal data ram */
    spram (!x)  :   ORIGIN = 0x92000000,                        LENGTH = 8K             /* spram 8K       0x88000000   0x1200_0000+8K    */
 
    /* reset address */
    boot  (rx)  :   ORIGIN = 0x94000000,                        LENGTH = 0x600             /* 0xBFC00000            */
 
    rom   (rx)  :   ORIGIN = 0x94000600,                        LENGTH = 0x1A00
    flash (rx)  :   ORIGIN = 0x94002000,                        LENGTH = 16K
    hash2 (rx)  :   ORIGIN = CONFIG_HASH2_START_ADDRESS,        LENGTH = 64K

    app   (rx)  :   ORIGIN = 0x81000000,                        LENGTH = 8M

    /* simulation */
    dummy       :   ORIGIN = 0xE0000000,                        LENGTH = 0x100000
    garbage     :   ORIGIN = 0xF0000000,                        LENGTH = 0x100000
}

_ld_load_start = 0x94000000;

SECTIONS
{
    .boot : AT ( _ld_load_start )
    {
        LONG(0x00)  /* avoid empty section */

        /* Fake FPKs to avoid KL failure (KeyA==KeyB) */
        . = 0x110;  LONG(0x11)
        . = 0x120;  LONG(0x22)
        . = 0x130;  LONG(0x33)
    } > boot

    .rom :
    {
        LONG(0x00)  /* avoid empty section */
    } > rom

    .flash :
    {
        /* HASH0 */
        _ld_HASH0_run_start = .;
        _ld_HASH1_run_start = .;
        obj/param.o (.hash1)
        _ld_HASH1_run_hash2_info = .;
        LONG( _ld_LDR_run_base );
        LONG( SIZEOF(.hash2) );
        _ld_HASH1_run_end = .;

    } > flash


    /DISCARD/ :
    {
        obj/otp_revision.o(.otpcfg)
        *(.reginfo)

        /* discard boot.o */
        obj/boot.o (.text)
        /* discard bbotrom*.o */
        obj/bootrom.o (.text)
        obj/bootrom_c.o (.text)
        obj/bootrom_c.o (.rodata*)
        obj/bootrom.o (.data)
        obj/bootrom_miu.o (.text*)
        obj/bootrom_miu.o (.rodata*)
    }

    _ld_LDR_load_start = 0x94003000;
    .hash2 : AT (_ld_LDR_load_start)
    {
        _ld_LDR_run_base = .;
        LONG( 0x00 );                                          /* For backward compatible  */
        LONG( _ld_LDR_run_start - _ld_LDR_run_base);        /* ldr_entry offset         */
        LONG( _ld_LDR_stack_end );                          /* ldr_stack_end            */
        LONG( 0x00 );                                       /* For backward compatible  */
        . = ALIGN(0x100);

        _ld_LDR_run_start = .;
        obj/loader.o (.ldr.entry)
        obj/loader.o (.text)
        obj/loader.o (.data)
        obj/loader.o (.rodata)
        obj/cache.o (.text)
        obj/lzmadec.o (.text*)
        obj/lzmadec.o (.rodata*)
        obj/lzmadec.o (.data*)
        obj/lzmadec.o (.bss*)

        . = ALIGN(0x100);
        _ld_LDR_run_end = .;

        obj/loader.o (.ldr.stack)
        . += 8k;
        . = ALIGN(32);
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


    .bin.app : AT ( LOADADDR(.boot)+128K )
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
        FILL(0xDEADBEEF)

        *(.stack)

        . += 4k;
        . = ALIGN(32);
        _ld_STACK_end = .;
    } > spram

    /* Check garbage data */
    .garbage :
    {
/*      * (*) */
    } > garbage

}

