#include "../../include/autoconf.h"
OUTPUT(elf32-littlemips)
ENTRY(BOOTROM_Entry)


MEMORY
{
    boot :  ORIGIN = 0xB2000000,                LENGTH = 14K
    ram :   ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 128K
}

SECTIONS
{
    _start_address_text_section = .;

    .text1 :
    {
        obj/bootrom_rom.o      (.text*)
        obj/bootrom_pll.o      (.text*)
        obj/bootrom_clock.o    (.text*)
        obj/bootrom_cache.o    (.text*)
        obj/bootrom_miu.o      (.text*)
        obj/bootrom_trim.o     (.text*)
        obj/bootrom_rom.o      (.rodata*)
        obj/bootrom_trim.o     (.rodata*)
        obj/bootrom_data.o     (.rodata*)
        obj/drvRIU.o           (.text*)
        obj/drvPadConf.o       (.rodata*)
        obj/boot.o             (.text*)
        //obj/nandflash.o       (.text*)
        obj/uart16550.o        (.text*)
	#if (CONFIG_NAND_FLASH == 1)
        obj/drvNAND_prg.o      (.text*)
        obj/drvNAND_hal_v5.o   (.text*)
        obj/drvNAND_platform.o (.text*)
    #else
        obj/drvSPINAND.o       (.text*)
        obj/drvSPINAND_boot.o  (.text*)
        obj/halSPINAND.o       (.text*)
        obj/halSPINAND.o       (.rodata*)
    #endif
        //obj/setSPI.o           (.text*)
        obj/apiBDMA.o          (.text*)
        obj/MSDecompress.o     (.text*)
        obj/MSDecompress.o     (.data*)
        obj/7alloc.o           (.text*)
        obj/lzmadec.o          (.text*)
        obj/zlib.o             (.text*)
        obj/cache.o            (.text*)
        obj/zlib.o             (.scommon*)
    } > boot

    .text2 : AT (LOADADDR(.text1) + 0x8000)
    {
        obj/bootram_rom.o      (.text*)
    } > ram

      . = ALIGN(16);
      _end_address_text_section = .;
    _size_text_section = _end_address_text_section - _start_address_text_section;

/*===============================================================================================*/
/* DATA Section                                                                                     */
    . = CONFIG_BOOTRAM_LOADADDR;

    _start_address_data_section = .;

    .data(_start_address_data_section) : AT(_end_address_text_section)
    {
        *(.data*)
    } > ram

    . = ALIGN(4);
    _end_address_data_section = .;
    _size_data_section = _end_address_data_section - _start_address_data_section;

/*===============================================================================================*/
/* BSS Section                                                                                     */
    _start_address_bss_section = .;

    .bss :
    {
        *(.bss)
    } > ram

    . = ALIGN(4);
    _end_address_bss_section = .;
    _size_bss_section = _end_address_bss_section - _start_address_bss_section;

    _stack_pointer = CONFIG_BOOTRAM_LOADADDR + 128K - 4;

    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /*LONG(0x0)*/
    } > ram


}
