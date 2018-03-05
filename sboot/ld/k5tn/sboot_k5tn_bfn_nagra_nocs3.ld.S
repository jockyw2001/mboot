OUTPUT(elf32-littlemips)
ENTRY(BOOT_Entry)
MEMORY
{
    spram (!x) : ORIGIN = 0x92000000, LENGTH = 8K
    boot (rx) : ORIGIN = 0x94000000, LENGTH = 0x600
    rom (rx) : ORIGIN = 0x94000600, LENGTH = 0x1A00
    flash (rx) : ORIGIN = 0x94002000, LENGTH = 16K
    hash2 (rx) : ORIGIN = 0x80800000, LENGTH = 64K
    app (rx) : ORIGIN = 0x81000000, LENGTH = 8M
    dummy : ORIGIN = 0xE0000000, LENGTH = 0x100000
    garbage : ORIGIN = 0xF0000000, LENGTH = 0x100000
}
_ld_load_start = 0x94000000;
SECTIONS
{
    .boot : AT ( _ld_load_start )
    {
        LONG(0x00)
        . = 0x110; LONG(0x11)
        . = 0x120; LONG(0x22)
        . = 0x130; LONG(0x33)
    } > boot
    .rom :
    {
        LONG(0x00)
    } > rom
    .flash :
    {
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
        obj/boot.o (.text)
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
        LONG( 0x00 );
        LONG( _ld_LDR_run_start - _ld_LDR_run_base);
        LONG( _ld_LDR_stack_end );
        LONG( 0x00 );
        . = ALIGN(0x100);
        _ld_LDR_run_start = .;
        obj/loader.o (.ldr.entry)
        obj/loader.o (.text)
        obj/loader.o (.data)
        obj/loader.o (.rodata*)
        obj/loader.o (.bss*)
        obj/cache.o (.text)
        obj/lzmadec.o (.text*)
        obj/lzmadec.o (.rodata*)
        obj/lzmadec.o (.data*)
        obj/lzmadec.o (.bss*)
        obj/nand.o (.text*)
        obj/nand.o (.rodata*)
        obj/nand.o (.data*)
        obj/nand.o (.bss*)
        obj/drvNAND_hal.o (.text*)
        obj/drvNAND_hal.o (.rodata*)
        obj/drvNAND_hal.o (.data*)
        obj/drvNAND_hal.o (.bss*)
        obj/drvNAND_utl.o (.text*)
        obj/drvNAND_utl.o (.rodata*)
        obj/drvNAND_utl.o (.data*)
        obj/drvNAND_utl.o (.bss*)
        obj/drvNAND_platform.o (.text*)
        obj/drvNAND_platform.o (.rodata*)
        obj/drvNAND_platform.o (.data*)
        obj/drvNAND_platform.o (.bss*)
        . = ALIGN(0x100);
        _ld_LDR_run_end = .;
        obj/loader.o (.ldr.stack)
        obj/nand.o (.stack)
        obj/drvNAND_hal.o (.stack)
        obj/drvNAND_utl.o (.stack)
        obj/drvNAND_platform.o (.stack)
        . += 8k;
        . = ALIGN(32);
        _ld_LDR_stack_end = .;
        LONG(0x00000000);
        LONG(0x00000000);
        LONG(0x00000000);
        LONG(0x4552444C);                                     //TAG:LDRE
    } > hash2
    _ld_LDR_load_end = LOADADDR(.hash2)+SIZEOF(.hash2);
    .bin.sfw : AT ( _ld_LDR_load_end )
    {
    } > dummy
    _ld_sfw_end = LOADADDR(.bin.sfw)+SIZEOF(.bin.sfw);
    .bin.fw : AT (_ld_sfw_end)
    {
        . = ALIGN(0x10);
    } > dummy
    .bin.app : AT ( LOADADDR(.boot)+128K )
    {
        LONG( . );
        LONG( APP_ENTRY );
        LONG( 0x00000000 );
        LONG( SIZEOF(.bin.app) );
        . = 0x100;
        . = APP_BIN;
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
    .garbage :
    {
    } > garbage
}
