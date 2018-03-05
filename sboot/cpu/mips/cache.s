    .include "cpu.inc"
    .include "chip.inc"

    .section .text
    .set    noreorder

    .global enable_cache_asm
enable_cache_asm:
    mfc0    $8,     $16
    ehb
    and     $8,     M_ConfigK0
    # Skip if cached already enabled
    beq     $8,     K_CacheAttrCN, 4001f
    nop

    CPU_InitIC
    CPU_InitDC

    # Enable cache
    CPU_EnableCache

    sync
    ehb
    nop
    nop
    nop
    nop
4001:
    CHIP_IsL2Enable $9
    bne     $0, $9, 4002f
    nop

    CHIP_EnableL2Cache
    .balign 32
    .rept 32
    nop
    .endr
4002:

    jr $31
    nop

    .global flush_cache_asm
flush_cache_asm:
    CPU_FlushDC
    CPU_InitIC

    sync
    ehb
    nop
    nop
    nop
    nop

    jr $31
    nop

    .global remap_dspram_asm
remap_dspram_asm:
    CPU_RemapDSPRAM
    jr $31
    nop
