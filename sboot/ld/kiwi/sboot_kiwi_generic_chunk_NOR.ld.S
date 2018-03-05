#include "../../include/autoconf.h"
OUTPUT(elf32-littlemips)
ENTRY(BOOT_Entry)


MEMORY
{
    boot :  ORIGIN = 0xBFC00000,                LENGTH = 3K
    /*rom :   ORIGIN = 0x94000000+0xC00,  LENGTH = 16K*/
    rom :   ORIGIN = 0xBFC00000+0xC00,          LENGTH = 16K
    ram :   ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 128K
    sram :  ORIGIN = 0x84000000,                LENGTH = 1K
}

_ld_load_start = 0x94000000;

SECTIONS
{
    .text1 :
    {
        obj/boot.o             (.text*)
    } > boot

    .text2 : AT ( LOADADDR(.text1) + SIZEOF(.text1) )
    {
        obj/drvRIU.o           (.text*)
        obj/bootrom.o          (.text*)
        obj/bootrom_pll.o      (.text*)
        obj/bootrom_clock.o    (.text*)
        obj/bootrom_cache.o    (.text*)
        obj/bootrom_miu.o      (.text*)
        obj/bootrom_miu_str.o  (.text*)
        obj/bootrom_trim.o     (.text*)
        obj/bootrom.o          (.rodata*)
        obj/bootrom_trim.o     (.rodata*)
        obj/bootrom_data.o     (.rodata*)
        obj/bootrom_data_str.o (.rodata*)
        obj/drvPadConf.o       (.rodata*)
        . = ALIGN(16);
        _ld_bootram_ROM_start = .;
    } > rom

    .text3 : AT ( LOADADDR(.text2) + SIZEOF(.text2) )
    {
        _ld_bootram_RAM_start = .;
        obj/bootram.o          (.text*)
        //*nandflash.o        (.text*)
        //*nandflash.o        (.rodata*)
        obj/uart16550.o        (.text*)
        //*setSPI.o           (.text*)
        //*setSPI.o           (.rodata*)
        obj/apiBDMA.o          (.text*)
        obj/apiBDMA.o          (.rodata*)
        obj/MSDecompress.o     (.text*)
        obj/MSDecompress.o     (.rodata*)
        obj/MSDecompress.o     (.data*)
        obj/MSDecompress.o     (.bss*)
        obj/apiSECURE.o        (.rodata*)
        obj/apiSECURE.o        (.text*)
        obj/apiSECURE.o        (.data*)
        obj/apiSECURE.o        (.bss*)
        obj/apiRSA.o           (.rodata*)
        obj/apiRSA.o           (.text*)
        obj/apiRSA.o           (.data*)
        obj/apiRSA.o           (.bss*)
        obj/apiSHA.o           (.rodata*)
        obj/apiSHA.o           (.text*)
        obj/apiSHA.o           (.data*)
        obj/apiSHA.o           (.bss*)
        obj/7alloc.o           (.text*)
        obj/lzmadec.o          (.text*)
        obj/zlib.o             (.text*)
        obj/zlib.o             (.rodata*)
        obj/zlib.o             (.data*)
        obj/zlib.o             (.bss*)
        obj/zlib.o             (.scommon*)
        obj/cache.o               (.text*)
        . = ALIGN(16);
        _ld_bootram_RAM_end = .;
    } > ram

    /* dummy */
    .data :
    {
        *(.data)
    } > ram

    .bss :
    {
        . = ALIGN(16);
        _ld_bootbss_start = .;
        *(.bss)
        . = ALIGN(16);
        _ld_bootbss_end = .;
    } > ram
    /DISCARD/ : {


    /* ABI crap starts here */

    *(.reginfo)
    }
    .padding : AT(LOADADDR(.text1) + 0x10000 - 4)
    {
        LONG(0x0)
    } > ram

}
