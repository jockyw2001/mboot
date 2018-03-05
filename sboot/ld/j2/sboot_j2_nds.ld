OUTPUT(elf32-littlemips)
OUTPUT_ARCH(mips)

ENTRY(BOOT_Entry)


MEMORY
{
    /* internal data ram */
    spram (!x) : ORIGIN = 0x88000000, LENGTH = 8K /* spram 4K */

    /* reset address */
    boot (rx) : ORIGIN = 0xBFC00000, LENGTH = 2K  /* 0xBFC00000 */

    bootrom (rx) : ORIGIN = 0xb4000800, LENGTH = 6K
    flash (rx) : ORIGIN = 0xb4002000, LENGTH = 8K

    hash2 (rx) : ORIGIN = 0x80000000, LENGTH = 64K

    /*stack2      :   ORIGIN = 0x80FFFC00,        LENGTH = 1K*/
    stack2      :   ORIGIN = 0x80A80000,        LENGTH = 1K
    app   (rx)  :   ORIGIN = 0x81000000,        LENGTH = 8M

    /* simulation */
    dummy       :   ORIGIN = 0xE0000000,        LENGTH = 0x100000
    garbage     :   ORIGIN = 0xF0000000,        LENGTH = 0x100000
}

_ld_load_start = 0xb4000000;

SECTIONS
{
    .boot : AT ( 0xb4000000 )
    {
        obj/boot.o (.text)

        . = 2K-4;
        LONG(0xDEADBEEF)
    } > boot

    .bootrom :
    {
        obj/bootrom.o (.text)
        obj/bootrom_c.o (.text)
        obj/bootrom.o (.data)

        . = 6K-4;
        LONG(0xDEADBEEF)
    } > bootrom

    .flash :
    {
        /* HASH0 Data: 0x94002000 -> 0x9400210F */
        _ld_HASH0_run_start = .;
        obj/param.o (.hash0)
        FILL(0x00000000);
        _ld_HASH0_run_end = .;  /* ALIGN(0x10) */

        /* Signature Area: */
        . = 0x0D00;
        /* HASH0 Signature: 0x94002D00 -> 0x94002DFF */
        _ld_SIG0_load_start = .;
        obj/param.o (.sig0)
        /* HASH1 Signature: 0x94002E00 -> 0x94002EFF */
        _ld_SIG1_load_start = .;
        obj/param.o (.sig1)
        /* HASH2 Signature: 0x94002F00 -> 0x94002FFF */
        _ld_SIG2_load_start = .;
        obj/param.o (.sig2)

        /* HASH1 Data: 0x94003000 -> 0x94003FFF */
        . = 0x1000;
        _ld_HASH1_run_start = .;
        obj/param.o (.hash1)
        _ld_HASH1_run_hash2_size = .;
        LONG( _ld_LDR_run_end - _ld_LDR_run_header );       /* hash_size(->run_base)*/
        _ld_HASH1_run_end = .;

    } > flash

    /* HASH2 Data: 0x94004000 -> 0x940040FF */
    _ld_LDR_load_start = 0xb4004000;
    _ld_LDR_load_end = _ld_LDR_load_start + 1024;
    .hash2 : AT (_ld_LDR_load_start)
    {
        _ld_LDR_run_header = .;

        LONG( . );                                          /* run_base(16x)        */
        LONG( _ld_LDR_run_start - _ld_LDR_run_header);      /* ldr_entry offset     */
        LONG( _ld_LDR_stack_end );                          /* ldr_stack_end        */

        . = 0x100 - 4;
        LONG(0xDEADBEEF)
        _ld_LDR_run_header_end = .;

        _ld_LDR_run_start = .;
        obj/loader.o (.ldr.entry)
        obj/loader.o (.text)
        obj/loader.o (.data)

        . = ALIGN(0x400);
        . = ALIGN(0x2000); /* align to 8kb */
        _ld_LDR_run_end = .;
    } > hash2

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


    .bin.app : AT ( LOADADDR(.boot)+24K )
    {
        LONG( . );                                                      /* run_base(16x)        */
        LONG( SIZEOF(.bin.app) );
        LONG( 0x00000000 );
        LONG( 0x00000000 );
        LONG( APP_ENTRY  );                                             /* app_entry offset     */
        LONG( 0x00000000 );
        . = 0x100;

        . = APP_BIN;                                                    /* ecos bin base        */
        _ld_APP_run_start = .;
        obj/app.bin.o (*)
        . = ALIGN(16);
    } > app
    _ld_APP_load_start = LOADADDR(.bin.app);
    _ld_APP_load_end = LOADADDR(.bin.app)+SIZEOF(.bin.app);

    .ldr_stack :
    {
        /*. = . + 0x400 - 4;*/
        _ld_LDR_stack_end = .;
    } > stack2

    .data :
    {
        . = 0x0000;
    } > spram

    .bss :
    {
    } > spram

    .stack :
    {
        _ld_STACK_start = ADDR(.data)+3K;
        FILL(0xDEADBEEF)

        *(.stack)

        . = 4K-SIZEOF(.data)-4;
        _ld_STACK_end = .;
    } > spram

    /* Check garbage data */
    .garbage :
    {
/*      * (*) */
    } > garbage

}

