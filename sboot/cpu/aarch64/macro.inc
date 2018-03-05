//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

.macro ventry  label
    .align  7
    b           \label
.endm

//-------------------------------------------------
//Macro: WREG_B
//-------------------------------------------------
.macro WREG_B reg, value
    ldr         x26, =\reg
    ldr         w25, =\value
    strb        w25, [x26]
.endm

//-------------------------------------------------
//Macro: WREG
//-------------------------------------------------
.macro WREG reg, value
    ldr         x26, =\reg
    ldr         w25, =\value
    str         w25, [x26]
.endm

//-------------------------------------------------
//Macro: REG_C2M //write cpu register to mstar register
//-------------------------------------------------
.macro REG_C2M cpu_reg, mstar_reg
    ldr         x26, =\mstar_reg
    str         \cpu_reg, [x26]
.endm

//-------------------------------------------------
//Macro: RREG
//-------------------------------------------------
.macro RREG cpu_reg, mstar_reg
    ldr         x26, =\mstar_reg
    ldr         \cpu_reg, [x26]
.endm

//-------------------------------------------------
//Macro: DBGREG
//-------------------------------------------------
.macro DBGREG value
    WREG    0x1F206700, \value
.endm

//-------------------------------------------------
//Macro: MEMCPY32
//src(w28), dst(w29), len(w30) should be 32bit align
//-------------------------------------------------
.macro MEMCPY32
MEMCPY32_\@:
    ldr         x24, [x25], #8
    str         x24, [x26], #8
    subs        w27, w27,  #8
    b.ge        MEMCPY32_\@
.endm

//-------------------------------------------------
//Macro: MEMSET32
//start(w27), end(w28), value(w29)
//-------------------------------------------------
.macro MEMSET32
MEMSET32_\@:
    cmp         w25, w26
    str         w27, [w25], #4
    b.ne        MEMSET32_\@
.endm

//-------------------------------------------------
//Macro: MEMORY_FLUSH
//
//-------------------------------------------------
.macro MEMORY_FLUSH
.endm

//-------------------------------------------------
//Macro: DELAYUS
//-------------------------------------------------
.macro DELAYUS us
    ldr         x25, =(0x1F000000 + (0x003000 << 1))
    ldr         w24, =((\us * 12) & 0x0000FFFF)
    strh        w24, [x25, #(0x24 << 1)]
    ldr         w24, =(\us * 12) >> 16
    strh        w24, [x25, #(0x26 << 1)]

    ldr         w24, =0x00000002
    strh        w24, [x25, #(0x20 << 1)]
DELAYUS_\@:
    ldr         w24, [x25, #(0x22 << 1)]
    subs        w24, w24, #0x00000001
    b.ne         DELAYUS_\@
.endm

//-------------------------------------------------
//Macro: TIMER HEAD (for rum time mesurement)
//-------------------------------------------------
.macro TIMER_HEAD
    WREG        0x1F006080, 0x0000
    WREG        0x1F006088, 0xFFFF
    WREG        0x1F00608C, 0xFFFF
    WREG        0x1F006080, 0x0001
.endm

//-------------------------------------------------
//Macro: TIMER_TAIL (for rum time mesurement)
//-------------------------------------------------
.macro TIMER_TAIL
    RREG        w25, 0x1F006090
    REG_C2M     w25, 0x1F006088
    RREG        w27, 0x1F006094
    REG_C2M     w27, 0x1F00608C
.endm

.macro WREG_DBG value
    WREG    0x1F206700, \value
.endm
