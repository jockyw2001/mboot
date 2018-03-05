#include "../../include/autoconf.h"
OUTPUT(elf32-littlemips)
ENTRY(BOOT_Entry)


MEMORY
{
    /* internal data ram */
    spram (!x)  :   ORIGIN = 0x92000000,                        LENGTH = 8K             /* spram 8K       0x88000000   0x1200_0000+8K    */

    /* reset address */
    boot  (rx)  :   ORIGIN = 0x94000000,                        LENGTH = 0x600             /* 0xBFC00000            */

    rom   (rx)  :   ORIGIN = 0x94000600,                        LENGTH = 0x2A00
    flash (rx)  :   ORIGIN = 0x94003000,                        LENGTH = 16K
    hash2 (rx)  :   ORIGIN = CONFIG_HASH2_START_ADDRESS,        LENGTH = CONFIG_HASH2_SIZE

    app   (rx)  :   ORIGIN = 0x810A0000,                        LENGTH = 8M

    JPD_INTER   :   ORIGIN = 0x80000000+CONFIG_JPD_INTER_ADDR,  LENGTH = CONFIG_JPD_INTER_SIZE
    JPD_READ    :   ORIGIN = 0x80000000+CONFIG_JPD_READ_ADDR,   LENGTH = CONFIG_JPD_READ_SIZE
    JPD_OUT     :   ORIGIN = 0x80000000+CONFIG_JPD_OUT_ADDR,    LENGTH = CONFIG_JPD_OUT_SIZE
    GOP_OUT     :   ORIGIN = 0x80000000+CONFIG_GOP_OUTPUT_ADDR, LENGTH = CONFIG_GOP_OUTPUT_SIZE
    VE_FRAME    :   ORIGIN = 0x80000000+CONFIG_VE_FRAME_ADDR,   LENGTH = CONFIG_VE_FRAME_SIZE

    /* simulation */
    dummy       :   ORIGIN = 0xE0000000,                        LENGTH = 0x100000
    garbage     :   ORIGIN = 0xF0000000,                        LENGTH = 0x100000
}

_ld_load_start = 0x94000000;

SECTIONS
{
    .boot : AT ( _ld_load_start )
    {
        KEEP(obj/boot.o (.text))

        . = 0x600-4;
        LONG(0xDEADBEEF)
    } > boot

    .rom :
    {
        KEEP(obj/bootrom.o (.text))
        KEEP(obj/bootrom_c.o (.text))
        KEEP(obj/bootrom_c.o (.rodata*))
        KEEP(obj/bootrom.o (.data))

        KEEP(obj/bootrom_miu.o (.text*))
        KEEP(obj/bootrom_miu.o (.rodata*))
    } > rom


    .flash :
    {
        /* HASH0 */
        _ld_HASH0_run_start = .;

        _ld_HASH1_run_start = .;
        KEEP(obj/param.o (.hash1))
        _ld_HASH1_run_hash2_info = .;
        LONG( _ld_LDR_run_base );
        LONG( _ld_LDR_load_end - _ld_LDR_load_start);
        _ld_HASH1_run_end = .;

    } > flash


    /DISCARD/ :
    {
        KEEP(obj/otp_revision.o(.otpcfg))
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
        KEEP(obj/loader.o (.ldr.entry))
        KEEP(obj/loader.o (.text))
        KEEP(obj/loader.o (.bss*))
        KEEP(obj/loader.o (*COMMON))

        KEEP(obj/loader.o (.data))

        KEEP(obj/loader.o (.rodata*))
        KEEP(obj/cache.o (.text))
        KEEP(obj/cache.o (.data))
        KEEP(obj/cache.o (.bss))
        KEEP(obj/lzmadec.o (.text*))
        KEEP(obj/lzmadec.o (.rodata*))
        KEEP(obj/lzmadec.o (.data*))
        KEEP(obj/lzmadec.o (.bss*))

        obj/sbootBootLogo.o (.text*)
        obj/sbootBootLogo.o (.rodata*)
        obj/sbootBootLogo.o (.data*)
        obj/sbootBootLogo.o (.bss*)
        obj/sbootXC.o (.text*)
        obj/sbootXC.o (.rodata*)
        obj/sbootXC.o (.data*)
        obj/sbootXC.o (.bss*)
        obj/sbootGEGOP.o (.text*)
        obj/sbootGEGOP.o (.rodata*)
        obj/sbootGEGOP.o (.data*)
        obj/sbootGEGOP.o (.bss*)
        obj/sbootJPD.o (.text*)
        obj/sbootJPD.o (.rodata*)
        obj/sbootJPD.o (.data*)
        obj/sbootJPD.o (.bss*)

        bootlogo/k5tn/bsp/lib/libapiJPEG.a (.text*)
        bootlogo/k5tn/bsp/lib/libapiJPEG.a (.rodata*)
        bootlogo/k5tn/bsp/lib/libapiJPEG.a (.data*)
        bootlogo/k5tn/bsp/lib/libapiJPEG.a (.bss*)

        bootlogo/k5tn/bsp/lib/libMsOS.a (.text*)
        bootlogo/k5tn/bsp/lib/libMsOS.a (.rodata*)
        bootlogo/k5tn/bsp/lib/libMsOS.a (.data*)
        bootlogo/k5tn/bsp/lib/libMsOS.a (.bss*)

        bootlogo/k5tn/bsp/lib/libdrvMIU.a (.text*)
        bootlogo/k5tn/bsp/lib/libdrvMIU.a (.rodata*)
        bootlogo/k5tn/bsp/lib/libdrvMIU.a (.data*)
        bootlogo/k5tn/bsp/lib/libdrvMIU.a (.bss*)

        bootlogo/k5tn/bsp/lib/libdrvMMIO.a (.text*)
        bootlogo/k5tn/bsp/lib/libdrvMMIO.a (.rodata*)
        bootlogo/k5tn/bsp/lib/libdrvMMIO.a (.data*)
        bootlogo/k5tn/bsp/lib/libdrvMMIO.a (.bss*)

        bootlogo/k5tn/bsp/lib/libprana_k5tn.a (.text*)
        bootlogo/k5tn/bsp/lib/libprana_k5tn.a (.rodata*)
        bootlogo/k5tn/bsp/lib/libprana_k5tn.a (.data*)
        bootlogo/k5tn/bsp/lib/libprana_k5tn.a (.bss*)

        // libc.a
        bootlogo/tools/mipslinuxgnu483/errno-loc.o (.text*)
        bootlogo/tools/mipslinuxgnu483/errno-loc.o (.rodata*)
        bootlogo/tools/mipslinuxgnu483/errno-loc.o (.data*)
        bootlogo/tools/mipslinuxgnu483/errno-loc.o (.bss*)
        bootlogo/tools/mipslinuxgnu483/errno.o (.text*)
        bootlogo/tools/mipslinuxgnu483/errno.o (.rodata*)
        bootlogo/tools/mipslinuxgnu483/errno.o (.data*)
        bootlogo/tools/mipslinuxgnu483/errno.o (.bss*)

        // libgcc.a
        bootlogo/tools/mipslinuxgnu483/_umoddi3.o (.text*)
        bootlogo/tools/mipslinuxgnu483/_umoddi3.o (.rodata*)
        bootlogo/tools/mipslinuxgnu483/_umoddi3.o (.data*)
        bootlogo/tools/mipslinuxgnu483/_umoddi3.o (.bss*)
        bootlogo/tools/mipslinuxgnu483/_udivdi3.o (.text*)
        bootlogo/tools/mipslinuxgnu483/_udivdi3.o (.rodata*)
        bootlogo/tools/mipslinuxgnu483/_udivdi3.o (.data*)
        bootlogo/tools/mipslinuxgnu483/_udivdi3.o (.bss*)

        . = ALIGN(0x100);
        _ld_LDR_run_end = .;

        /* for sbrk.c */
        . = ALIGN (0x8);
        __heap = .;
        . = . + 0x08000;
        __heap_end = .;

        KEEP(obj/loader.o (.ldr.stack))
        . += 8k;
        . = ALIGN(32);
        _ld_LDR_stack_end = .;
    } > hash2

    /* from u-boot*/
    . = ALIGN(4096);_readonly_start = .;
    .text ALIGN (0x40) : { *(.text) *(.text.*) } > hash2
    .rodata ALIGN (0x40) : { . = .; *(.rodata) *(.rodata.*) } > hash2
    . = ALIGN(4096);_readonly_end = .;

	.eh_frame_hdr ALIGN (0x40) : { *(.eh_frame_entry .eh_frame_entry.*) } > hash2

    _gp = ALIGN(16);
    __got_start = .;

    .got  : {
        *(.got)
        . = ALIGN(32);
        LONG(0x00000000);
        LONG(0x00000000);
        LONG(0x00000000);
        LONG(0x4552444C);                                               //TAG:LDRE
    } > hash2
    __got_end = .;

    _ld_LDR_load_end = LOADADDR(.got)+SIZEOF(.got);

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

    .bin.app : AT ( LOADADDR(.boot)+ CONFIG_HASH2_SIZE )
    {
        LONG( . );                                                      /* run_base(16x)        */
        LONG( APP_ENTRY  );                                             /* app_entry offset     */
        LONG( 0x00000000 );
        LONG( SIZEOF(.bin.app) );
        . = 0x100;

        . = APP_BIN;                                                    /* ecos bin base        */
        _ld_APP_run_start = .;
        KEEP(obj/app.bin.o (*))
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
