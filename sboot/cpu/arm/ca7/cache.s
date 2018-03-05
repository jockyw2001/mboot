    #include "cpu.inc"
    #include "chip.inc"

    @.section .text
    @.set    noreorder

/*************************************************************************
 *
 * Create initial single-levle page table 
 *
 * setup vector mappoing
 *
 *************************************************************************/
    .global enable_mmu_asm
enable_mmu_asm:
    /* enable mmu                           */ 
    /* r0=tlb address                       */
    /* r1=relocated vector physical address */
    ldr r0, =0x3dff0000
    ldr r1, =0x3d100000
   
    /* Clear the 16K level 1 swapper page table */
    mov r4, r0
    mov r3, #0
    add r6, r0, #0x4000
1:  str r3, [r0], #4
    str r3, [r0], #4
    str r3, [r0], #4
    str r3, [r0], #4
    teq r0, r6
    bne 1b

    ldr r7, =0xC02   @noncache_mm_flags

    /*
    * Create identity mapping for first MB of kernel to
    * cater for the MMU enable.  This identity mapping
    * will be removed by paging_init().  We use our current program
    * counter to determine corresponding section base address.
    * VA=PA , length = 1MB
    */
    
    /*
    * Setup the pagetables vector mapped region.
    * VA=0x00000000 PA=0x40010000 , lengtg = 1MB
    */
    ldr r6, =0x00000000
    mov r6, r6, lsr #20         @ start of vector section
    mov r3, r1		            @ r1 = Vector PA base
    mov r3, r3, lsr #20         @ start of vector section
    orr r3, r7, r3, lsl #20      @ flags + ebase
    str r3, [r4, r6, lsl #2]      @ identity mapping

    /*
    * Setup the pagetables for SPI,RIU and ARM peripheral register direct mapping
    * mapped region.
    * VA=0x14000000 PA=0x14000000 , Length=0x01000000 (SPI)
    * VA=0x15000000 PA=0x15000000 , Length=0x01000000 (PL310)
    * VA=0x16000000 PA=0x16000000 , Length=0x01000000 (ARM peripheral register)
    * VA=0x1f000000  PA=0x20000000 , Length=0x01000000 (Mstar RIU)
    */
    ldr r3, =0x14000000
    mov r3, r3, lsr #20         @ start of AP section
    orr r3, r7, r3, lsl #20     @ flags + AP_base
    ldr r1, =0x14000000
    and r1, r1, #0xff000000
    mov r1, r1, lsr #18
    add r0, r4, r1
    ldr r1, =0x14000000
    and r1, r1, #0x00f00000
    mov r1, r1, lsr #18
    str r3, [r0, r1]!
    ldr r6, =(0x20000000 - 1)
    add r0, r0, #4
    add r6, r4, r6, lsr #18
2:  cmp r0, r6
    add r3, r3, #1 << 20
    strls   r3, [r0], #4
    bls 2b

    ldr r7, =0xC0E  @cache_mm_flags
       
    /*
    * Setup the pagetables for 4GB-1MB direct mapping
    * mapped region.
    * VA=CONFIG_MIU0_BUSADDR PA=CONFIG_MIU0_BUSADDR , Length=0x40000000
    */
    ldr r3, =0x20000000
    mov r3, r3, lsr #20         @ start of AP section
    orr r3, r7, r3, lsl #20     @ flags + AP_base
    ldr r1, =0x20000000
    and r1, r1, #0xff000000
    mov r1, r1, lsr #18
    add r0, r4, r1
    ldr r1, =0x20000000
    and r1, r1, #0x00f00000
    mov r1, r1, lsr #18
    str r3, [r0, r1]!
    ldr r1, =0x20000000
    add r6, r1, #(0x40000000-1)
    add r0, r0, #4
    add r6, r4, r6, lsr #18
3:  cmp r0, r6
    add r3, r3, #1 << 20
    strls   r3, [r0], #4
    bls 3b

    /* setup mmu*/
    ldr	r0, =0x3f /*#(domain_val(DOMAIN_USER, DOMAIN_MANAGER) | domain_val(DOMAIN_AP, DOMAIN_MANAGER) | domain_val(DOMAIN_BOOT, DOMAIN_MANAGER))*/
    mcr	p15, 0, r0, c3, c0, 0		@ load domain access register
    mcr	p15, 0, r4, c2, c0, 0		@ load page table pointer

    /* enable mmu*/
    mrc	p15, 0, r0, c1, c0, 0		@ read control reg
    ldr      r1, =0x00001001
    orr	r0, r0, r1
    mov	r0, r0
    mcr	p15, 0, r0, c1, c0, 0		@ write control reg
    mrc	p15, 0, r3, c0, c0, 0		@ read id reg

    mov pc, lr
    nop
    
/*************************************************************************
 *
 * enable D-cache
 *
 * 
 *
 *************************************************************************/
    .global enable_cache_asm
enable_cache_asm:
    mov r10, lr

    @ Setup SMP bit
    mrc p15, 0, r0, c1, c0, 1
    orr r0, r0, #(0x1 << 6)
    mcr p15, 0, r0, c1, c0, 1

    bl enable_mmu_asm
    mov lr, r10
    
    @ Invalidate D-cache by set/way
    @ Note: for Cortex-A9, there is no instruction for invalidating
    @ the whole D-cache. Need to invalidate line by line.
    @ Read cache size from the Cache Size Identification Register
    
    mrc p15, 1, r3, c0, c0, 0           @ Read current Cache Size Identification Register
    mov r1, #0x200
    sub r1, r1, #1
    and r3, r1, r3, LSR #13             @ r3 = (number of sets - 1)
    mov r0, #0                          @ r0 -> way counter
way_loop:
    mov r1, #0                          @ r1 -> set counter
set_loop:
    mov r2, r0, LSL #30                 @
    orr r2, r1, LSL #5                  @ r2 -> set/way cache-op format
    mcr p15, 0, r2, c7, c6, 2           @ Invalidate line described by r2
    add r1, r1, #1                      @ Increment set counter
    cmp r1, r3                          @ Check if the last set is reached...
    ble set_loop                        @ ...if not, continue the set_loop...
    add r0, r0, #1                      @ ...else, Increment way counter
    cmp r0, #4                          @ Check if the last way is reached...
    blt way_loop                        @ ...if not, continue the way_loop

    @ Enable dcaches
    @ D-cache is controlled by bit 2
    mrc p15, 0, r0, c1, c0, 0           @ read CP15 register 1
    orr r0, r0, #(0x1 <<2)              @ enable D Cache
    mcr p15, 0, r0, c1, c0, 0           @ write CP15 register 1

    @ Enable Program Flow Prediction
    @ Branch prediction is controlled by the System Control Register:
    @ Set Bit 11 to enable branch prediciton and return stack
    @ Turning on branch prediction requires a general enable
    @ CP15, c1. Control Register
    @ Bit 11 [Z] bit Program flow prediction:
    @ 0 = Program flow prediction disabled
    @ 1 = Program flow prediction enabled.
    mrc p15, 0, r0, c1, c0, 0           @ Read System Control Register
    orr r0, r0, #(0x1 <<11)
    mcr p15, 0, r0, c1, c0, 0           @ Write System Control Register

    @ Enable D-side prefetch
    @ Bit 2 [DP] Dside prefetch:
    @ 0 = Dside prefetch disabled
    @ 1 = Dside prefetch enabled.
    mrc 15, 0, r0, c1, c0, 1            @ Read Auxiliary Control Register
    orr r0, r0, #(0x1 <<2)              @ Enable Dside prefetch
    mcr p15, 0, r0, c1, c0, 1           @ Write Auxiliary Control Register  
    
    mov pc, lr
    nop
    
/*************************************************************************
 *
 * Flush cache
 *
 * 
 *
 *************************************************************************/    
    .global flush_cache_asm
flush_cache_asm:
    dmb                 @ ensure ordering with previous memory accesses
    mrc p15, 1, r0, c0, c0, 1       @ read clidr
    ands    r3, r0, #0x7000000      @ extract loc from clidr
    mov r3, r3, lsr #23         @ left align loc bit field
    beq finished            @ if loc is 0, then no need to clean
    mov r10, #0             @ start clean at cache level 0
loop_1:
    add r2, r10, r10, lsr #1        @ work out 3x current cache level
    mov r1, r0, lsr r2          @ extract cache type bits from clidr
    and r1, r1, #7          @ mask of the bits for current cache only
    cmp r1, #2              @ see what cache we have at this level
    blt skip                @ skip if no cache, or just i-cache
    mcr p15, 2, r10, c0, c0, 0      @ select current cache level in cssr
    isb                 @ isb to sych the new cssr&csidr
    mrc p15, 1, r1, c0, c0, 0       @ read the new csidr
    and r2, r1, #7          @ extract the length of the cache lines
    add r2, r2, #4          @ add 4 (line length offset)
    ldr r4, =0x3ff
    ands    r4, r4, r1, lsr #3      @ find maximum number on the way size
    clz r5, r4              @ find bit position of way size increment
    ldr r7, =0x7fff
    ands    r7, r7, r1, lsr #13     @ extract max number of the index size
loop_2:
    mov r9, r4              @ create working copy of max way size
loop_3:
    orr r11, r10, r9, lsl r5     @ factor way and cache number into r11

    orr r11, r11, r7, lsl r2     @ factor index number into r11

    mcr p15, 0, r11, c7, c14, 2     @ clean & invalidate by set/way
    subs    r9, r9, #1          @ decrement the way
    bge loop_3
    subs    r7, r7, #1          @ decrement the index
    bge loop_2
skip:
    add r10, r10, #2            @ increment cache number
    cmp r3, r10
    bgt loop_1
finished:
    mov r10, #0             @ swith back to cache level 0
    mcr p15, 2, r10, c0, c0, 0      @ select current cache level in cssr
    dsb
    isb

    mov     pc, lr
    nop
    