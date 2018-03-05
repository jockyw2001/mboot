


OUTPUT(elf32-littlemips)
ENTRY(BOOTROM_Entry)


MEMORY
{
    boot : ORIGIN = 0xB2000000, LENGTH = 14K
    boot2 : ORIGIN = 0xB2000000 + 9K, LENGTH = 5K
    boot3 : ORIGIN = 0xB2000000 + 9K, LENGTH = 5K
    ram : ORIGIN = 0x80100000, LENGTH = 128K
}

SECTIONS
{
 _start_address_text_section = .;

     .text1 :
    {
        _ld_bootrom_start = ABSOLUTE(.);
        obj/bootrom_rom.o (.text*)
        obj/bootrom_rom.o (.rodata*)
        obj/drvRIU.o (.text*)
        obj/boot.o (.text*)
        obj/uart16550.o (.text*)
        obj/bootrom_trim.o (.text*)
        obj/bootrom_trim.o (.rodata*)





        obj/drvNAND_prg.o (.text*)
        obj/drvNAND_hal_v5.o (.text*)
        obj/drvNAND_platform.o (.text*)

        obj/apiBDMA.o (.text*)
        obj/MSDecompress.o (.text*)
        obj/MSDecompress.o (.data*)
        obj/7alloc.o (.text*)
        obj/lzmadec.o (.text*)
        obj/zlib.o (.text*)
        obj/cache.o (.text*)
        obj/zlib.o (.scommon*)

        . = LENGTH(boot) - 5k;
        _ld_program_reload_addr = ABSOLUTE(.);
        obj/bootrom_pll.o (.text*)
        obj/bootrom_clock.o (.text*)
        obj/bootrom_cache.o (.text*)
        obj/bootrom_peripheral.o (.text*)
        . = LENGTH(boot) - 2k;

        . = LENGTH(boot);

        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;
    } > boot

    .text2 : AT (LOADADDR(.text1) + LENGTH(boot))
    {
        _ld_miu_flow_flash_start = LENGTH(boot) - _ld_program_reload_addr;
        _ld_miu_flow_flash_size = LENGTH(boot2) - _ld_program_reload_addr;
        _ld_miu_flow_start = ABSOLUTE(.);

        obj/bootrom_miu.o (.text*)
        obj/bootrom_data.o (.rodata*)

        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;

    } > boot2

    .text3 : AT (LOADADDR(.text2) + LENGTH(boot2))
    {
        _ld_miu_str_flow_flash_start = LENGTH(boot2) - _ld_program_reload_addr + _ld_miu_flow_flash_start;
        _ld_miu_str_flow_flash_size = LENGTH(boot3) - _ld_program_reload_addr;
  _ld_miu_str_flow_start = ABSOLUTE(.);

        obj/bootrom_miu_str.o (.text*)
        obj/bootrom_data.o (.rodata*)





        _ld_miu_str_flow_end = ABSOLUTE(.);
        _ld_miu_str_flow_size = _ld_miu_str_flow_end - _ld_miu_str_flow_start;

    } > boot3

    .text4 : AT (LOADADDR(.text1) + 0x8000)
    {
        _ld_bootram_start = .;

        obj/bootram_rom.o (.text*)
        obj/drvPadConf.o (.rodata*)

        . = LENGTH(ram) - 1k;

        . = LENGTH(ram);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;
    } > ram

   . = ALIGN(16);
   _end_address_text_section = .;
 _size_text_section = _end_address_text_section - _start_address_text_section;



    . = 0x80100000;

    _start_address_data_section = .;

    .data(_start_address_data_section) : AT(_end_address_text_section)
    {
        *(.data*)
    } > ram

 . = ALIGN(4);
 _end_address_data_section = .;
 _size_data_section = _end_address_data_section - _start_address_data_section;



 _start_address_bss_section = .;

    .bss :
    {
        *(.bss)
    } > ram

 . = ALIGN(4);
 _end_address_bss_section = .;
 _size_bss_section = _end_address_bss_section - _start_address_bss_section;

    _stack_pointer = 0x80100000 + 128K - 4;

    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {

    } > ram

}
