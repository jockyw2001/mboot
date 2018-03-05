
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
ENTRY(_vector)


MEMORY
{

    spram (!x) : ORIGIN = 0x10008000, LENGTH = 16K


    boot (rx) : ORIGIN = 0x00000000, LENGTH = 0x1300
    rom (rx) : ORIGIN = 0x14001300, LENGTH = 0x3D00
    flash (rx) : ORIGIN = 0x14005000, LENGTH = 16K
    hash2 (rx) : ORIGIN = 0x20080000, LENGTH = 105K

    app (rx) : ORIGIN = 0x21000000, LENGTH = 8M


    dummy : ORIGIN = 0xE0000000, LENGTH = 0x100000
    garbage : ORIGIN = 0xF0000000, LENGTH = 0x100000
}

_ld_load_start = 0x14000000;

SECTIONS
{
    .boot : AT ( _ld_load_start )
    {



        obj/boot.o (.text)


        . = 0x1300-4;
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
        obj/chip_misc_rom.o (.text*)
        obj/chip_misc_rom.o (.rodata*)




        . = 0x3f28 - 0x300;
        obj/otp_revision.o(.otpcfg)
    } > rom

    _ld_HASH0_load_start = LOADADDR(.flash);
    .flash :
    {

        _ld_HASH0_run_start = .;
        . = ALIGN(0x10);

        . = 0x0200;



        . = 0x0300;




        . = 0x0800;



        . = 0x0A00;



        . = 0x0C00;




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

    _ld_LDR_load_start = 0x14007000;
    .hash2 : AT (_ld_LDR_load_start)
    {
        _ld_LDR_run_base = .;
        LONG(0x3052444C);
        LONG( _ld_LDR_run_start - _ld_LDR_run_base);
        LONG( _ld_LDR_stack_end );
        LONG( SIZEOF(.hash2) );
        . = ALIGN(0x100);

        _ld_LDR_run_start = .;
        obj/loader.o (.ldr.entry)
        obj/loader.o (.text)
        obj/loader.o (.bss*)
        obj/loader.o (*COMMON)
        obj/loader.o (.data)
        obj/loader.o (.rodata*)
        obj/bootram.o (.text)
        obj/bootram.o (.date)
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
        LONG(0x4552444C);
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
    .bin.app : AT ( LOADADDR(.hash2)+105K )
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
        _ld_STACK_start = ADDR(.data)+3K;
        FILL(0xDEADBEEF)
        *(.stack)
        . = 4K-SIZEOF(.data)-4;
        . = ALIGN(0x10);
        _ld_STACK_end = .;
    } > spram
    .garbage :
    {
    } > garbage
}
