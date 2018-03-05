#include "../../include/autoconf.h"
OUTPUT(elf32-littlemips)
ENTRY(BOOT_Entry)


MEMORY
{
    /* internal data ram */
    spram (!x)  :   ORIGIN = 0x88000000,                        LENGTH = 8K             /* spram 8K              */
                                                                
    /* reset address */                                         
    boot  (rx)  :   ORIGIN = 0x94000000,                        LENGTH = 0x600
                                                                
    rom   (rx)  :   ORIGIN = 0x94000600,                        LENGTH = 0x1A00
    flash (rx)  :   ORIGIN = 0x94002000,                        LENGTH = 16K
    hash2 (rx)  :   ORIGIN = CONFIG_HASH2_START_ADDRESS,        LENGTH = 64K

    stack2      :   ORIGIN = 0x80A80000,                        LENGTH = 16K
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
        obj/boot.o (.text)

        . = 0x600-4;
        LONG(0xDEADBEEF)
    } > boot

    .rom :
    {
        obj/bootrom.o (.text)
        obj/bootrom_c.o (.text)
        obj/bootrom_c.o (.rodata*)
        obj/bootrom.o (.data)

        obj/bootrom_miu.o (.text*)
        obj/bootrom_miu.o (.rodata*)

        . = 0x1F28 - 0x0600;                                              /*  0x1F28              */
        obj/otp_revision.o(.otpcfg)
    } > rom


    .flash :
    {
        /* HASH0 */
        _ld_HASH0_run_start = .;
        obj/param.o (.hash0)
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

    _ld_LDR_load_start = 0x94004000;
    .hash2 : AT (_ld_LDR_load_start)
    {
        _ld_LDR_run_base = .;
        LONG( . );                                          /* For backward compatible  */
        LONG( _ld_LDR_run_start - _ld_LDR_run_base);        /* ldr_entry offset         */
        LONG( _ld_LDR_stack_end );                          /* ldr_stack_end            */
        LONG( SIZEOF(.hash2) );                             /* For backward compatible  */
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
    } > hash2
    _ld_LDR_load_end = LOADADDR(.hash2)+SIZEOF(.hash2);

    /* Secured Firmware */
    .bin.sfw : AT ( _ld_LDR_load_end )
    {
    } > dummy                                                           /* virtual region for linker  */

    /* Unsecured Firmware */
    _ld_sfw_end = LOADADDR(.bin.sfw)+SIZEOF(.bin.sfw);
    .bin.fw : AT ( LOADADDR(.boot)+64K )
    {
        obj/pm.bin.o
        . = ALIGN(0x10);                                                /* aligned for bin.app header */
    } > dummy                                                           /* virtual region for linker  */


    .bin.app : AT ( LOADADDR(.boot)+256K )
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

    .ldr_stack : AT ( LOADADDR(.boot)+128K ) /* fake position */
    {
        obj/loader.o (.ldr.stack)
        . += 8k;
        . = ALIGN(32);
        _ld_LDR_stack_end = .;
    } > stack2

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

