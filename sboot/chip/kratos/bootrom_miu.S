################################################################################
#
# Copyright (c) 2006-2010 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# (¡§MStar Confidential Information¡¨) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################

#if defined(CONFIG_MSTAR_KRATOS_BD_FPGA)
    #include "bootrom_miu_fpga.S"
#elif CONFIG_MEM1333MHz
    #include "bootrom_miu_ddr2.S"
#elif (CONFIG_MEM1600MHz)
    #if defined(CONFIG_MSTAR_DDR_256MB)
        #include "bootrom_miu_ddr3_256M.S"
    #elif defined(CONFIG_MSTAR_DDR_128MB)
        #include "bootrom_miu_ddr3_128M.S"
    #else
        #error "unknown DDR type"
    #endif
#else
    #error "unknown DDR type"
#endif


#-------------------------------------------------------------------------------------------------
# Miu Auto Phase
# @param
# @return None
# @note
#-------------------------------------------------------------------------------------------------
Miu0_AutoPhase:
    WREG        0xBF2025E0, 0x0000                          # 1012f0 bit_15=0 choice to MIU_0
    li          s7, (REG_ADDR_BASE + (0x103390 << 1))       # Dummy register to store miu 0 DQS phase result
    li          t8, 0                                       # DQS phase result

    WREG        (REG_ADDR_BASE + (0x110D6E << 1)), 0x7777   # set the default phase setting to 0x7777
    RREG        t1, (REG_ADDR_BASE + (0x110D6E << 1))       # store the default phase setting to r19
    move        s3, t1

    li          t3, (REG_ADDR_BASE + (0x110D6E << 1))       # reg_0x110D6E  //miu0_phase_reg
    li          t2, (REG_ADDR_BASE + (0x1012E7 << 1))       # reg_0x1012e7  //miu0_dqs_mask_reg

AutoPhaseLoop:
    PUTK        '['

    WREG        (REG_ADDR_BASE + (0x1012e4 << 1)), 0x0401   # 1012e4~1012e7 bist test size,   (16 byte/unit)
    WREG        (REG_ADDR_BASE + (0x1012e6 << 1)), 0x0000   # 1012e4~1012e7 bist test size,   (16 byte/unit)
    WREG        (REG_ADDR_BASE + (0x1012e8 << 1)), 0x00FF   # 1012e4~1012e7 bist test pattern

    lui         t5, 0                                       # t5-->tmp_var0=0;  for loop cnt
    lui         s5, 0                                       # s5-->good_phase cnt
    lui         s4, 0                                       # s4-->good_phase_sum
    li          t9, 1                                       # t9-->good_phase bit

StartChk_x3:

BistPatternfill:                    #write data into each region respect to bist mode 0x183,0x185,0x187
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, t5, 0x0f
    sll         t1, t1, 4
    andi        t0, s3, 0x0f           # set dqs0 delay phase to r8
    or          t1, t1, t0
    sw          t1, (0)(t3)

    #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)

    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x1000   # 1012e2 bist test start address, (4K byte/unit)
    WREG        0xbf2025C0,0x0000
    li          t1, 0x0083              # bist mode 0x83
    j           BistRWModeTest
    nop

Check_bist85:
    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x1100   # 1012e2 bist test start address, (4K byte/unit)
    WREG        0xbf2025C0,0x0000
    li          t1, 0x0085              # bist mode 0x85
    j           BistRWModeTest
    nop

Check_bist87:
    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x1200   # 1012e2 bist test start address, (4K byte/unit)
    WREG        0xbf2025C0,0x0000
    li          t1, 0x0087              # bist mode 0x87

BistRWModeTest:                         # Read and Write bist
    move        s6, ra
    bal         ModeChangeDelay
    nop
    move        ra, s6

    lui         t0, 0xBF20              # bist test start
    sw          t1, (0x25C0)(t0)

CheckRWBistDone:
    RREG        t1, 0xBF2025C0
    andi        t0, t1, 0x8000
    beqz        t0, CheckRWBistDone     # check done
    nop

RWBistCheckPhaseOK:
    andi        t0, t1, 0x00FF
    beq         t0, 0x83, Check_bist85  #0x83->0x85->0x87
    nop
    beq         t0, 0x85, Check_bist87
    nop

check_x3:

    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, t5, 0x000f
    sll         t1, t1, 12
    andi        t0, s3, 0x0fff          # set dqs0 delay phase to r8
    or          t1, t1, t0
    sw          t1, (0)(t3)

    #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)

    j           StartCheck
    nop

StartChk_x2:
    #li          t1, 0xaa             # 1012e7/1006e7 =0xAA mask DQS_1 error when test DQS_0
    #sb          t1, (0)(t2)

    sw          t8, (0)(s7)           # store phase 0 result
    addiu       s7, s7, 4
    li          t8, 0                 # DQS phase result
    li          t9, 1
    j           check_x2
    nop

check_x2:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, t5, 0x000f
    sll         t1, t1, 8
    andi        t0, s3, 0xf0ff        # set dqs0 delay phase to r8
    or          t1, t1, t0
    sw          t1, (0)(t3)

    #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)

    j           StartCheck
    nop
StartChk_x1:
    #li          t1, 0xaa             # 1012e7/1006e7 =0xAA mask DQS_1 error when test DQS_0
    #sb          t1, (0)(t2)

    sw          t8, (0)(s7)           # store phase 0 result
    addiu       s7, s7, 4
    li          t8, 0                 # DQS phase result
    li          t9, 1
    j           check_x2
    nop

check_x1:

    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, t5, 0x000f
    sll         t1, t1, 4
    andi        t0, s3, 0xff0f        # set dqs0 delay phase to r8
    or          t1, t1, t0
    sw          t1, (0)(t3)

    #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)

    j           StartCheck
    nop
StartChk_x0:
    #li          t1, 0xaa             # 1012e7/1006e7 =0xAA mask DQS_1 error when test DQS_0
    #sb          t1, (0)(t2)

    sw          t8, (0)(s7)           # store phase 0 result
    addiu       s7, s7, 4
    li          t8, 0                 # DQS phase result
    li          t9, 1
    j           check_x0
    nop
check_x0:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, t5, 0x000f
    andi        t0, s3, 0xfff0  # get the default phase setting
    or          t1, t1, t0
    sw          t1, (0)(t3)

    #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)

StartCheck:
    WREG        0xbf2025C0,0x0000
    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x1000   # 1012e2 bist test start address, (4K byte/unit)
    li          t1, 0x0183              # bist mode 0x183
    j           ROBitModeTest
    nop

Check_bist185:
    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x1100   # 1012e2 bist test start address, (4K byte/unit)
    WREG        0xbf2025C0,0x0000
    li          t1, 0x0185              # bist mode 0x185
    j           ROBitModeTest
    nop

Check_bist187:
    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x1200   # 1012e2 bist test start address, (4K byte/unit)
    WREG        0xbf2025C0,0x0000
    li          t1, 0x0187              # bist mode 0x187

ROBitModeTest:                      # read only bist
    move        s6, ra
    bal         ModeChangeDelay
    nop
    move        ra, s6

    lui         t0, 0xBF20              # bist test start
    sw          t1, (0x25C0)(t0)

CheckROBistDone:
    RREG        t1, 0xBF2025C0
    andi        t0, t1, 0x8000
    beqz        t0, CheckROBistDone        # check done
    nop
    andi        t0, t1, 0x6000
    bnez        t0, checkloopFail        # bist test fail if not zero
    nop

ROBistCheckPhaseOK:
    andi        t0, t1, 0x01FF
    beq         t0, 0x183, Check_bist185       #0x83->0x85->0x87
    nop
    beq         t0, 0x185, Check_bist187
    nop

    move        s6, ra
    andi        t1, t5, 0x0f        #move t5(loop cnt) to t1 for putchar
    bal         puthex
    nop
    move        ra, s6

    addiu       s5, s5, 1         # good_phase_cnt++;
    andi        s6, t5, 0x0f      # get good_phase
    add         s4, s4 ,s6        # good_phase_sum += good phase
    or          t8, t8, t9

checkloopFail:

checkloop:
    sll         t9, t9, 1
    addiu       t5, t5, 1             # add loop cnt by 1
    bne         t5, 16 , CheckLoopDone
    nop

    beqz        s5, SetToDefault_P3    # if good_phase cnt == 0
    nop
    move        s6, s4  #good_phase_sum
    div         s6, s6 , s5
    sll         s6, s6 , 12
    j           StorePhase_P3
    nop
SetToDefault_P3:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    move        t1, s3
    j           SetPhaseToDefault_p3
    nop

StorePhase_P3:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, s3 , 0x0fff
    or          t1, t1 , s6
SetPhaseToDefault_p3:
    sw          t1, 0(t3)
    move        s3, t1

   #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)


    lui         s5, 0                  # s5-->good_phase cnt
    lui         s4, 0                  # s4-->good_phase_sum

    PUTK        ']'
    #PUTK       '\r'
    #PUTK       '\n'
    PUTK        '['

    # start check DQS_phase_0
    #sw         s3, (0)(t3)
    j           StartChk_x2
    nop

CheckLoopDone:
    bltu        t5, 16, check_x3
    nop
    bltu        t5, 32, check_x2      #AutoPhaseLoop1
    nop
    beq         t5, 32, SetP2_
    nop
    bltu        t5, 48, check_x1      #AutoPhaseLoop1
    nop
    beq         t5, 48, SetP1_
    nop
    bltu        t5, 64, check_x0      #AutoPhaseLoop1
    nop
    beq         t5, 64, SetP0_
    nop

SetP2_:
    beqz        s5, SetToDefault_P2    # if good_phase cnt == 0
    nop
    move        s6, s4  #good_phase_sum
    div         s6, s6 , s5
    sll         s6, s6 , 8
    j           StorePhase_P2
    nop
SetToDefault_P2:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    move        t1, s3
    j           SetPhaseToDefault_p2
    nop

StorePhase_P2:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, s3 , 0xf0ff
    or          t1, t1 , s6
SetPhaseToDefault_p2:
    sw          t1, 0(t3)
    move        s3, t1

   #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)


    lui         s5, 0                  # s5-->good_phase cnt
    lui         s4, 0                  # s4-->good_phase_sum

    PUTK        ']'
    #PUTK       '\r'
    #PUTK       '\n'
    PUTK        '['

    # start check DQS_phase_0
    #sw         s3, (0)(t3)
    j           StartChk_x1
    nop

SetP1_:
    beqz        s5, SetToDefault_P1    # if good_phase cnt == 0
    nop
    move        s6, s4  #good_phase_sum
    div         s6, s6 , s5
    sll         s6, s6 , 4
    j           StorePhase_P1
    nop
SetToDefault_P1:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    move        t1, s3
    j           SetPhaseToDefault_p1
    nop

StorePhase_P1:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1, s3 , 0xff0f
    or          t1, t1 , s6
SetPhaseToDefault_p1:
    sw          t1, 0(t3)
    move        s3, t1

   #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)


    lui         s5, 0                  # s5-->good_phase cnt
    lui         s4, 0                  # s4-->good_phase_sum

    PUTK        ']'
    #PUTK       '\r'
    #PUTK       '\n'
    PUTK        '['

    # start check DQS_phase_0
    #sw         s3, (0)(t3)
    j           StartChk_x0
    nop

SetP0_:
    sw          t8, (0)(s7)         #store phase 0  result
    nop
    beqz        s5, SetToDefault_P0  # good_phase cnt == 0
    nop
    move        s6, s4  #good_phase_sum
    div         s6, s6 , s5
    #sll        s6, s6 , 4
    j           StorePhase_P0
    nop
SetToDefault_P0:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    move        t1, s3
    j           SetPhaseToDefault
    nop

StorePhase_P0:
    #ClockReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x60
    sb          k0, (0)(t6)

    andi        t1,  s3 , 0xfff0
    or          t1,  t1 , s6
SetPhaseToDefault:
    sw          t1,  0(t3)
    move        s3, t1

   #ClockUnReset
    li          t6, (REG_ADDR_BASE + (0x110D0E << 1))
    lb          k0, (0)(t6)
    andi        k0, k0, 0x9F
    ori         k0, k0, 0x20
    sb          k0, (0)(t6)

    PUTK        ']'
    PUTK        '-'

    move        s6, ra
    andi        t1, s3, 0xf000
    srl         t1, t1, 12
    bal         puthex
    nop
    andi        t1, s3, 0x0f00
    srl         t1, t1, 8
    bal         puthex
    nop
    andi        t1, s3, 0x00f0
    srl         t1, t1, 4
    bal         puthex
    nop
    andi        t1, s3, 0x000f
    bal         puthex
    nop
    move        ra, s6

AutoPhaseRet:
    WREG        0xbf2025C0, 0x0000
    li          t1, 0x00             # 1012e7/1006e7 =0x00 mask DQS_1 error when test DQS_0
    sb          t1, (0)(t2)

    PUTK        '\r'
    PUTK        '\n'

    jr          ra
    nop

puthex:
    bltu        t1, 10, putx0_9
    nop
putxA_F:
    addiu       t1, t1, 0x37
    j           putx
    nop
putx0_9:
    addiu       t1, t1, 0x30
putx:
    SSSW        0xBF201300, t1
    jr          ra
    nop

ModeChangeDelay:
    lui         t0, 0x0
DelayLoop:
    addiu       t0, t0, 1
    bltu        t0, 10, DelayLoop
    nop
    jr          ra
    nop


#-------------------------------------------------------------------------------------------------
# MiuBistTest
# @param
# @return None
# @note
#-------------------------------------------------------------------------------------------------
Miu0_BistTest:
    PUTK        '\n'
    PUTK        '\r'
    PUTK        'B'
    PUTK        'I'
    PUTK        'S'
    PUTK        'T'
    PUTK        '0'
    PUTK        '-'

    WREG        (REG_ADDR_BASE + (0x1012fe << 1)), 0x0087
    WREG        (REG_ADDR_BASE + (0x1012e0 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012e2 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012e4 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012e6 << 1)), 0x0040
    WREG        (REG_ADDR_BASE + (0x1012e8 << 1)), 0x5aa5
    WREG        (REG_ADDR_BASE + (0x1012d8 << 1)), 0x0000
    WREG        (REG_ADDR_BASE + (0x1012e0 << 1)), 0x0006

BistMiu0Test:
    WREG        (REG_ADDR_BASE + (0x1012e0 << 1)), 0x0007   #bitst test mode: 7

ChkBistMiu0:
    RREG        t1, (REG_ADDR_BASE + (0x1012e0 << 1))
    andi        t0, t1, 0x8000
    beqz        t0, ChkBistMiu0
    nop

    andi        t0, t1, 0x6000
    beqz        t0, BistMiu0_OK
    nop

BistMiu0_FAIL:
    PUTK        'F'
    PUTK        'A'
    PUTK        'I'
    PUTK        'L'
    j           BistMiu0Ret
    nop


BistMiu0_OK:
    PUTK        'O'
    PUTK        'K'

BistMiu0Ret:
    PUTK        '\n'
    PUTK        '\r'

    jr          ra
    nop

