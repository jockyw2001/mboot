################################################################################
#
# Copyright (c) 2008-2010 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################

#-------------------------------------------------------------------------------------------------
# CHIP registers
#-------------------------------------------------------------------------------------------------
    .if (CPU==1)
REG_ADDR_BASE           = 0xbf000000
CLK_GEN0_BASE           = 0xBF201600

R_REG_WDT_MAX_LO        = 0xbf006010
R_REG_WDT_MAX_HI        = 0xbf006014
R_REG_WDT_CLR           = 0xbf006000
    .endif

    .if (CPU==0) # aeon
    .endif

READ_DATA_HITABLE_BURST_SIZE = 0x0008
READ_INST_HITABLE_BURST_SIZE = 0x0008
WRITE_BUFFER_TIMEOUT         = 0x0000 			# 0x0000 is the default value, means disabled
WRITE_BUFFER_THRESHOLD       = 0x0080            # 0x0080 is the default value, [1:0] 10 = histable disable

ENABLE_MEM1066MHz = 0
ENABLE_MEM800MHz = 1
ENABLE_MEM667MHz = 0


#-------------------------------------------------------------------------------------------------
# Flash registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# Other registers
#-------------------------------------------------------------------------------------------------


#-------------------------------------------------------------------------------------------------
# UART registers
#-------------------------------------------------------------------------------------------------
CPU_CLK_FREQ            = 504000000                         #CPU clock
UART_CLK_FREQ           = 123000000                         #Clock source
UART_BAUDRATE           = 115200                            #Baud rate
UART_DIVISOR            = (UART_CLK_FREQ/(16*UART_BAUDRATE))#Divisor


#-------------------------------------------------------------------------------------------------
# SecureChip registers
#-------------------------------------------------------------------------------------------------

R_CA_BASE               = 0xbf227400
R_CA_SBOOT              = (R_CA_BASE + (0x17 << 2))
R_CA_JTAG_PWD0          = (R_CA_BASE + 0x24 << 2)
R_CA_JTAG_PWD1          = (R_CA_BASE + 0x25 << 2)
R_CA_JTAG_PWD2          = (R_CA_BASE + 0x26 << 2)
R_CA_JTAG_PWD3          = (R_CA_BASE + 0x27 << 2)

R_CA_POSTMASK           = (R_CA_BASE + 0x28 << 2)
R_CA_DBGMODE            = (R_CA_BASE + 0x42 << 2)

CA_SBOOT_ACTIVE         = 0xF000
CA_POSTMASK_ENABLE      = 0x0004
CA_DBGMODE_ACTIVE       = 0x8000


#-------------------------------------------------------------------------------------------------
# Macros
#-------------------------------------------------------------------------------------------------

   .macro  WREG_INIT
   _REGBASE = 0x0
   .endm

    .macro  WREG    reg, value
    .if (\reg >> 16) <> _REGBASE
        lui     $8,     (\reg >> 16)
        _REGBASE = (\reg >> 16)
    .endif
    li      $9,     (\value)
    sw      $9,     (\reg & 0xFFFF)($8)
    .endm

    .macro  WREG_B    reg, value
    .if (\reg >> 16) <> _REGBASE
        lui     $8,     (\reg >> 16)
        _REGBASE = (\reg >> 16)
    .endif
    li      $9,     (\value)
    sb      $9,     (\reg & 0xFFFF)($8)
    .endm

    .macro  RREG    var, reg
    .if (\reg >> 16) <> _REGBASE
        lui     $8,     (\reg >> 16)
        _REGBASE = (\reg >> 16)
    .endif
    lw      \var,     (\reg & 0xFFFF)($8)
    .endm

    .macro  WREG_DBG  value
    WREG_INIT
    WREG    0xBF206700, (\value) # mailbox 0
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_PowerOnChip
# @note Power On Sequence for Chip
#-------------------------------------------------------------------------------------------------

    .macro  CHIP_PowerOnChip
    WREG_INIT

#ecb_setting:                                        # same as T3 U02 setting
    WREG  0xbf203184, READ_DATA_HITABLE_BURST_SIZE  # 0x01, Read Data hitable/burst size
    WREG  0xbf203188, READ_INST_HITABLE_BURST_SIZE  # 0x02, Read Instruction hitable/burst size
    WREG  0xbf20318C, WRITE_BUFFER_TIMEOUT          # write buffer flush timeout
    WREG  0xbf203190, WRITE_BUFFER_THRESHOLD        # write buffer threshhold <-- 6 level

#BOOT_EnableAccessMIU:
    WREG   0xBF005788 , 0x0004
    WREG   0xBF00578C , 0x3c11
    WREG   0xBF005790 , 0x0010
    WREG   0xBF005794 , 0x403c
    WREG   0xBF005798 , 0x0101

#BOOT_PowerOnSeq:
#BOOTROM_MPLL_INIT:
#    #include "script_MainPll_MIPSPLL_UPLL.S"
    # turn on MainPLL, MIPSLL and UPLL
    #swch 4
    # MainPLL : DMPLL
    WREG (REG_ADDR_BASE + (0x11286A <<1 )) , 0x0004    #Charge pump current control
    WREG (REG_ADDR_BASE + (0x112860 <<1 )) , 0x7800    #MPLL div clocks enable
    WREG (REG_ADDR_BASE + (0x112866 <<1 )) , 0x0902    #Loop divider ; VCO = 24*2^2*9 = 864
    WREG (REG_ADDR_BASE + (0x112802 <<1 )) , 0x0446
    WREG (REG_ADDR_BASE + (0x112818 <<1 )) , 0x0000
    WREG_B (REG_ADDR_BASE + (0x112826 <<1 )) , 0x0040
    WREG_B (REG_ADDR_BASE + (0x112840 <<1 )) , 0x0000
    # MIPSPLL
    WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0065    #clr bit4, disable power down
    WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0045    #clr Bit5, disable reset
    WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0005    #clr bit6, disable power reset
    # 504MHz
    WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0009    #input_div2, {output_div1,loop_div1,input_div1}
    WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0015    #output_div2, loop_div2
    # 456MHZ
    #WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0009    #input_div2, {output_div1,loop_div1,input_div1}
    #WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0013    #output_div2, loop_div2
    # 312MHz
    #WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0004    #input_div2, {output_div1,loop_div1,input_div1}
    #WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x000d    #output_div2, loop_div2
    # 396MHz
    #WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0005    #input_div2, {output_div1,loop_div1,input_div1}
    #WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0021    #output_div2, loop_div2
    # TXPLL (216Mhz for 480p)
    WREG (REG_ADDR_BASE + (0x103350 <<1 )) , 0x0000      # enable clk_hdmi_tx_p
    WREG (REG_ADDR_BASE + (0x11303c <<1 )) , 0xffff      # clkgen enable
    WREG (REG_ADDR_BASE + (0x11308a <<1 )) , 0x0000      # power on synth pll
    WREG (REG_ADDR_BASE + (0x113066 <<1 )) , 0x0000      # power on tx pll
    WREG (REG_ADDR_BASE + (0x113054 <<1 )) , 0x0000      # power on tm atop
    WREG (REG_ADDR_BASE + (0x11305c <<1 )) , 0x000f      # enable tm atop
    WREG (REG_ADDR_BASE + (0x113056 <<1 )) , 0x0000      # power on tm atop on tm_reg
    WREG (REG_ADDR_BASE + (0x113058 <<1 )) , 0x0000      # power on tm atop on tm_reg
    WREG (REG_ADDR_BASE + (0x113000 <<1 )) , 0x0080      # software reset synthesizer
    WREG (REG_ADDR_BASE + (0x113000 <<1 )) , 0x0000
    WREG (REG_ADDR_BASE + (0x113100 <<1 )) , 0x0804      # [12]: BYPASS_DC_FIFO, [11]: REG_HPLL_LOCK_CHK, [10]: REG_DC_FIFO_RST, [9:8]: START_DE_PH
                              # [7:6]: REG_DC_MODE, [5]: REG_EN_CLAMP, [4]: PKT_MANUAL_MODE, [3]: REG_USER_ENABLE, [2]: REG_EN_PKT, [1]: REG_PHYMODE, [0]: REG_DVIMODE
    # VEPLL
    WREG (REG_ADDR_BASE + (0x103100 <<1 )) , 0x0000      #disable power reset
    WREG (REG_ADDR_BASE + (0x103106 <<1 )) , 0x0001      #disable reset & power down
    # UPLL (turn 480Mhz in USB)
    # USBC0 <--> UTMI0
    WREG_B (REG_ADDR_BASE + (0x100700   <<1 )) , 0x0a    # Disable MAC initial suspend, Reset UHC
    WREG_B (REG_ADDR_BASE + (0x100700   <<1 )) , 0x28    # Release UHC reset, enable UHC and OTG XIU function
    WREG_B (REG_ADDR_BASE + (0x103aa2   <<1 )) , 0xe0    # Set PLL_TEST[23:22] for enable 480MHz clock
    WREG_B (REG_ADDR_BASE + (0x103aa0   <<1 )) , 0x03    # UTMI offset 0x20 bit1=1#Set PLL_TEST[1] for PLL multiplier 20X
    WREG_B (REG_ADDR_BASE + (0x103a82   <<1 )) , 0x84    # Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG_B (REG_ADDR_BASE + (0x103a83   <<1 )-1) , 0x20    # Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG_B (REG_ADDR_BASE + (0x103a89   <<1 )-1) , 0x00    # #3=0 Disable force_pll_on
    WREG_B (REG_ADDR_BASE + (0x103a88   <<1 )) , 0x00    # #7=0 Enable band-gap current
    WREG_B (REG_ADDR_BASE + (0x103a81   <<1 )-1) , 0x6b    # Turn on reference voltage and regulator
    WREG_B (REG_ADDR_BASE + (0x103a80   <<1 )) , 0xc3    # reg_pdn: bit<15>, bit <2> ref_pdn
    DELAYUS_12M 1000                  # delay 1ms
    WREG_B (REG_ADDR_BASE + (0x103a81   <<1 )-1) , 0x69    # Turn on UPLL, reg_pdn: bit<9>
    DELAYUS_12M 2000                  # delay 2ms
    WREG_B (REG_ADDR_BASE + (0x103a80   <<1 )) , 0x01    # Turn all (including hs_current) use override mode
    WREG_B (REG_ADDR_BASE + (0x103a81   <<1 )-1) , 0x00
    #WREG_B (REG_ADDR_BASE + (0x103a83   <<1 )-1) , 0x00    ##7=0
    WREG_B (REG_ADDR_BASE + (0x103ab3   <<1 )-1) , 0x41    # set CA_START as 1
    WREG_B (REG_ADDR_BASE + (0x103ab3   <<1 )-1) , 0x40    # release CA_START
    # USBC1 <--> UTMI1
    WREG_B (REG_ADDR_BASE + (0x100780   <<1 )) , 0x0a    # Disable MAC initial suspend, Reset UHC
    WREG_B (REG_ADDR_BASE + (0x100780   <<1 )) , 0x28    # Release UHC reset, enable UHC and OTG XIU function
    WREG_B (REG_ADDR_BASE + (0x103a22   <<1 )) , 0xe0    # Set PLL_TEST[23:21] for enable 480MHz clock
    WREG_B (REG_ADDR_BASE + (0x103a20   <<1 )) , 0x03    # Set PLL_TEST[1] for PLL multiplier 20X
    WREG_B (REG_ADDR_BASE + (0x103a02   <<1 )) , 0x84    # Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG_B (REG_ADDR_BASE + (0x103a03   <<1 )-1) , 0x20
    WREG_B (REG_ADDR_BASE + (0x103a09   <<1 )-1) , 0x00    # Disable force_pll_on
    WREG_B (REG_ADDR_BASE + (0x103a08   <<1 )) , 0x00    # Enable band-gap current
    WREG_B (REG_ADDR_BASE + (0x103a01   <<1 )-1) , 0x6b    # Turn on reference voltage and regulator
    WREG_B (REG_ADDR_BASE + (0x103a00   <<1 )) , 0xc3    # reg_pdn: bit<15>, bit <2> ref_pdn
    DELAYUS_12M 1000                  # delay 1ms
    WREG_B (REG_ADDR_BASE + (0x103a01   <<1 )-1) , 0x69    # Turn on UPLL, reg_pdn: bit<9>
    DELAYUS_12M 2000                  # delay 2ms
    WREG_B (REG_ADDR_BASE + (0x103a00   <<1 )) , 0x01    # Turn all (including hs_current) use override mode
    WREG_B (REG_ADDR_BASE + (0x103a01   <<1 )-1) , 0x00
    #WREG_B (REG_ADDR_BASE + (0x103a03   <<1 )-1) , 0x00    # #7=0
    WREG_B (REG_ADDR_BASE + (0x103a33   <<1 )-1) , 0x41    # set CA_START as 1
    WREG_B (REG_ADDR_BASE + (0x103a33   <<1 )-1) , 0x40    # release CA_START
    # USBC2 <--> UTMI2
    WREG_B (REG_ADDR_BASE + (0x110200   <<1 )) , 0x0a    # Disable MAC initial suspend, Reset UHC
    WREG_B (REG_ADDR_BASE + (0x110200   <<1 )) , 0x68    # Release UHC reset, enable UHC and OTG XIU function
    WREG_B (REG_ADDR_BASE + (0x102a22   <<1 )) , 0xe0    # Set PLL_TEST[23:21] for enable 480MHz clock
    WREG_B (REG_ADDR_BASE + (0x102a20   <<1 )) , 0x03    # Set PLL_TEST[1] for PLL multiplier 20X
    WREG_B (REG_ADDR_BASE + (0x102a02   <<1 )) , 0x84    # Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG_B (REG_ADDR_BASE + (0x102a03   <<1 )-1) , 0x20
    WREG_B (REG_ADDR_BASE + (0x102a09   <<1 )-1) , 0x00    # Disable force_pll_on
    WREG_B (REG_ADDR_BASE + (0x102a08   <<1 )) , 0x00    # Enable band-gap current
    WREG_B (REG_ADDR_BASE + (0x102a01   <<1 )-1) , 0x6b    # Turn on reference voltage and regulator
    WREG_B (REG_ADDR_BASE + (0x102a00   <<1 )) , 0xc3    # reg_pdn: bit<15>, bit <2> ref_pdn
    DELAYUS_12M 1000                  # delay 1ms
    WREG_B (REG_ADDR_BASE + (0x102a01   <<1 )-1) , 0x69    # Turn on UPLL, reg_pdn: bit<9>
    DELAYUS_12M 2000                  # delay 2ms
    WREG_B (REG_ADDR_BASE + (0x102a00   <<1 )) , 0x01    # Turn all (including hs_current) use override mode
    WREG_B (REG_ADDR_BASE + (0x102a01   <<1 )-1) , 0x00
    #WREG_B (REG_ADDR_BASE + (0x102a03   <<1 )-1) , 0x00    # #7=0
    WREG_B (REG_ADDR_BASE + (0x102a33   <<1 )-1) , 0x41    # set CA_START as 1
    WREG_B (REG_ADDR_BASE + (0x102a33   <<1 )-1) , 0x40    # release CA_START

#BOOTROM_CLK_GEN0_DVBT_INIT:
#    #include "script_clk_gen.S"
    #swch 4
    #<<CLKGEN_PM>>
    WREG (REG_ADDR_BASE + (0x0008E4 <<1 )) , 0x0001		# enable fast read
    WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x1008   # clk_mcu = 172M (step 1/2)
    #WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x1000   # clk_mcu = 12M (step 1/2)
                              # clk_spi = 54M (step_1/2)
    WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x5088   # clk_mcu = 172M (step 2/2)
    #WREG (REG_ADDR_BASE + (0x000e40 <<1 )) , 0x5000   # clk_mcu = 12M (step 2/2)
                              # clk_spi = 54M (step_2/2)
    #<<CLKGEN0>>
    WREG (REG_ADDR_BASE + (0x100b02 <<1 )) , 0x0007   # SRAM bist test clk 7:xtal, 6:54M, 5:62M 4:72M 3:86M 2:108M 1:123M 0:144M
    #MCU
    WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0400   # clk_mcu = 172M (step 1/2)
    WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0401   # clk_mcu = 172M (step 2/2)
    WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x0000   # clk_ocp on
                              # clk_mips = 312M (step 1/2) (MIPSPLL_VCO)
    #WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x8000   # clk_mips = 312M (step 2/2) (MIPSPLL_VCO)
    WREG (REG_ADDR_BASE + (0x100b24 <<1 )) , 0x0004   # clk_aeon = 172M (step_1/2)
    WREG (REG_ADDR_BASE + (0x100b24 <<1 )) , 0x0084   # clk_aeon = 172M (step_2/2)
    WREG (REG_ADDR_BASE + (0x100b28 <<1 )) , 0x0000   # clk_uart1 = 172M (step_1/2)
                              # clk_uart2 = 172M (step_1/2)
    WREG (REG_ADDR_BASE + (0x100b26 <<1 )) , 0x0c06   # clk_uart0 = 123M
                              # clk_uart1 = 172M (step_2/2)
                              # clk_uart2 = 172M (step_2/2)
    WREG (REG_ADDR_BASE + (0x100b2c <<1 )) , 0x0010   # clk_spi = 54M (step_1/2)
    WREG (REG_ADDR_BASE + (0x100b2c <<1 )) , 0x0030   # clk_spi = 54M (step_2/2)
    WREG (REG_ADDR_BASE + (0x100b2e <<1 )) , 0x0100   # clk_fuart = 54M
                              # uart synthesizer enable
    #PCM & TCK
    WREG (REG_ADDR_BASE + (0x101e08 <<1 )) , 0x0020   # CLK_TCK_IN from PAD_TS1_SYNC
    WREG (REG_ADDR_BASE + (0x100b34 <<1 )) , 0x0000   # clk_pcm = 27M
                              # clk_tck = CLK_TCK_IN
    #MIU
    WREG (REG_ADDR_BASE + (0x100b3c <<1 )) , 0x0100   # clk_mpll_syn = 432M (for VEPLL  0: 216M, 1:432M)
#    .if (ENABLE_MEM1066MHz <> 0)
#        WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0010   # clk_miu = 240M
#    .else
#        #WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0000   # clk_miu = 216M
#        WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0004   # clk_miu = 172M #SW patch for VC1 random error
#    .endif
                              # clk_miu_rec = 93K (clk_xtal/128)
    WREG (REG_ADDR_BASE + (0x100bfc <<1 )) , 0x0001   # clk_ddr_syn = 432M (for MEMPLL  0: 216M, 1:432M)
    #VE
    WREG (REG_ADDR_BASE + (0x100b48 <<1 )) , 0x0800   # clk_ve = 27M
                              # clk_vedac = 108M
    WREG (REG_ADDR_BASE + (0x100b4a <<1 )) , 0x001c   # clk_ve_in = 27M (clk_odclk)
    #TSP
    WREG (REG_ADDR_BASE + (0x101e04 <<1 )) , 0x0001   # CLK_TS0_IN from PAD_TS0_CLK
    WREG (REG_ADDR_BASE + (0x100b50 <<1 )) , 0x0000   # clk_ts0 = CLK_TS0_IN
                              # clk_ts1 = CLK_TS0_IN
    WREG (REG_ADDR_BASE + (0x100b52 <<1 )) , 0x0000   # clk_ts2 = CLK_TS0_IN
    WREG (REG_ADDR_BASE + (0x100b54 <<1 )) , 0x0000   # clk_tsp = 144M
                              # clk_stc0 = 27M (STC0_SYNTH_OUT)
    #VD_MHEG5
    WREG (REG_ADDR_BASE + (0x100b60 <<1 )) , 0x0000   # ckg_vd_mheg5 = 160M
    #HVD
    WREG (REG_ADDR_BASE + (0x100b62 <<1 )) , 0x0004   # ckg_HVD = 160M
    #JPD
    WREG (REG_ADDR_BASE + (0x100b6a <<1 )) , 0x000c   # ckg_jpg = 144M (0:72m 4:108m 8:123m c:144m)
    #MVD
    WREG (REG_ADDR_BASE + (0x100b70 <<1 )) , 0x0000   # mvd_syn off
    WREG (REG_ADDR_BASE + (0x100b72 <<1 )) , 0x0400   # clk_mvd = 144M
                              # clk_mvd2 = 144M
    WREG (REG_ADDR_BASE + (0x100b74 <<1 )) , 0x0000   # clk_mvd_chroma on
                              # clk_mvd_luma_a on
    WREG (REG_ADDR_BASE + (0x100b76 <<1 )) , 0x0000   # clk_mvd_luma_b on
                              # clk_mvd_luma_c on
    WREG (REG_ADDR_BASE + (0x100b78 <<1 )) , 0x0000   # clk_mvd_rmem on
                              # clk_mvd_rmem1 on
    #GOP
    WREG (REG_ADDR_BASE + (0x100b80 <<1 )) , 0x0000   # clk_gopg0 = 27M (clk_odclk)
                              # clk_gopg1 = 27M (clk_odclk)
    WREG (REG_ADDR_BASE + (0x100b82 <<1 )) , 0x0000   # clk_gopmix = 27M (clk_odclk) #0x0018 (mixer ve_clk)
                              # clk_gopd = 27M (clk_odclk)
    WREG (REG_ADDR_BASE + (0x100b84 <<1 )) , 0x0000   # 27M (clk_odclk)  #0x0004   # clk_gopg2 = 192M (clk_idclk)
                              # clk_gopg3 = 27M (clk_odclk)
    WREG (REG_ADDR_BASE + (0x100b86 <<1 )) , 0x0000   # clk_psram0 on
    #GE
    WREG (REG_ADDR_BASE + (0x100b90 <<1 )) , 0x000c   # clk_ge = 144M
    #STB_DC
    WREG (REG_ADDR_BASE + (0x100b98 <<1 )) , 0x0018   # clk_dc0 = 192M
    #SC
    WREG (REG_ADDR_BASE + (0x100ba2 <<1 )) , 0x0400   # clk_ficlk = 192M (clk_fclk)
    WREG (REG_ADDR_BASE + (0x100ba4 <<1 )) , 0x1000   # clk_flck = 192M
    WREG (REG_ADDR_BASE + (0x100ba6 <<1 )) , 0x000c   # clk_odclk = 27M (CLK_OUT_DAC/8) (output pixel clock)
    WREG (REG_ADDR_BASE + (0x100baa <<1 )) , 0x000c   # clk_idclk = 192M (clk_dc0)
    WREG (REG_ADDR_BASE + (0x100bb0 <<1 )) , 0x0c00   # clk_hdgen = 27M (clk_odlck)
                              # clk_hdgen_fir = 27M (CLK_OUT_DAC/8)
    WREG (REG_ADDR_BASE + (0x100bb2 <<1 )) , 0x000c   # clk_dac1 = 108M (clk_vedac) (SD)
                              # clk_dac2 = 27M (clk_hdgen_fir) (HD)
    WREG (REG_ADDR_BASE + (0x100bb4 <<1 )) , 0x000c   # clk_dcs = 27M (CLK_OUT_DAC/8)
                              # clk_dcs27 = 27M
    #EMAC
    WREG (REG_ADDR_BASE + (0x100bc0 <<1 )) , 0x0000   # clk_emac_ahb = 123M
                              # clk_emac_rx = CLK_EMAC_RX_IN
    WREG (REG_ADDR_BASE + (0x100bc2 <<1 )) , 0x0000   # clk_emac_tx = CLK_EMAC_TX_IN
                              # clk_emac_rx_ref = CLK_EMAC_RX_IN
    WREG (REG_ADDR_BASE + (0x100bc4 <<1 )) , 0x0000   # clk_emac_tx_ref = CLK_EMAC_TX_IN
    #NAND
    WREG (REG_ADDR_BASE + (0x100bc8 <<1 )) , 0x0024   # clk_fice = 54M
    #MPIF
    WREG (REG_ADDR_BASE + (0x100bd0 <<1 )) , 0x0024   # clk_mpif = 160M
    #<<CLK_GEN1>>
    #DEMOD
    WREG (REG_ADDR_BASE + (0x103300 <<1 )) , 0x0511   # parallel mode: 0x0511 / serial mode 0x0400
                              # reg_ckg_dvbtmk_ts_out_mode@0x00
    WREG (REG_ADDR_BASE + (0x103308 <<1 )) , 0x0000   # reg_ckg_dvbtc_ts@0x04[11:8] : 8KHz
    WREG (REG_ADDR_BASE + (0x10330c <<1 )) , 0x0000   # reg_ckg_dvbtc_inner1x@0x06[0:0] : DMPLLDIV14/2=31MHz
                              # reg_ckg_dvbtc_inner2x@0x06[8:8] : DMPLLDIV14=62MHz
    WREG (REG_ADDR_BASE + (0x103314 <<1 )) , 0x0000   # reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK=28.8MHz
                              # reg_ckg_demod_dma@0x0a[11:8] : ADC_CLK
    WREG (REG_ADDR_BASE + (0x103316 <<1 )) , 0x0000   # reg_ckg_dvbtc_eq8x@0x0b[0:0] : DMPLLDIV6=144MHz
                              # reg_ckg_dvbtc_eq@0x0b[8:8] : DMPLLDIV6/8=18MHz
    #CA
    WREG (REG_ADDR_BASE + (0x103348 <<1 )) , 0x0000   # clk_nsk = 123M
                              # clk_sm_ca = 27M, clk_sm_ca2 = 27M
    #HDMITX
    WREG (REG_ADDR_BASE + (0x103350 <<1 )) , 0x0000   # clk_hdmitx = 27M (clk_odclk)
    #MIIC
    WREG (REG_ADDR_BASE + (0x103360 <<1 )) , 0x0000   # clk_miic = 24M
#    .if POWER_SAVING_EN
# ...
#    .endif

#.if (ENABLE_MEM1066MHz == 1)
#    WREG    (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x000C   # clk_miu = 216M
#.else
    RREG    $11, (REG_ADDR_BASE + (0x001E02 <<1 ))
    andi    $11, $11, 0x0E00
    bnez    $11, CLK_GEN0_DONE #U03 and above
    WREG    (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0004   # clk_miu = 172M #SW patch for VC1 random error
#.endif
CLK_GEN0_DONE:

    #<<SPI>>
    WREG_B 0xBF201658, 0x10     # 54MHz
    WREG_B 0xBF201658, 0x30     # clk_spi by glitch-free clock switch
    DELAYUS_12M  100

    #Init EMAC clk
    WREG  0xbf203d40, 0x0000 # emac out pad

    #with OCP and MIPS speed up
#BOOTROM_MCU_INIT:
#.if (CPU_CLOCK>=252)
    .if (CPU_CLK_FREQ >= 252000000)
        WREG_B  0xBF203a3c, 0x50 # ocp 1:1
        DELAYUS_12M 20000
        WREG_B  0xBF203a3c, 0x51 # fire
        DELAYUS_12M 20000
        WREG_B  0xBF203a3c, 0x20 # ocp 1:3
        DELAYUS_12M 20000
        WREG_B  0xBF203a3c, 0x21 # fire
        DELAYUS_12M 20000
    .endif

    WREG_B 0xBF201644, 0x00 #turn OCP clk

#.if (CPU_CLOCK>12)
    .if (CPU_CLK_FREQ > 12000000)
        # switch to clk_mipspll_vco
        WREG_B 0xBF201645, 0x00
        WREG_B 0xBF201645, 0x80  # trigger
    .endif

#SPI_CLK:
    WREG_B 0xBF0011C4 , 0xFF #0x0f:cz_high/0x0f:cz_setup/0x0f:cz_hold time set up
    WREG_B 0xBF0011C5 , 0x0F
    WREG_B 0xBF0011C8 , 0x01 #Enable fast read mode, let isp run up to 86Mhz

    WREG_B 0xBF001C81 , 0x10 #step 1/2:set SPI clkgen to 54M
    WREG_B 0xBF001C81 , 0x50 #step 2/2:release SPI_clk from XTAL to clock gen (above line)
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_SET_MIU_CLOCK
# @note MIU CLOCK TO 288MHz
#-------------------------------------------------------------------------------------------------

        .macro CHIP_SET_MIU_CLOCK
#CHIP_SET_MIU_CLOCK:
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_ResetCHIP
# @note MIU initialization
#-------------------------------------------------------------------------------------------------

    .macro CHIP_ResetCHIP
#CHIP_ResetCHIP:
    WREG_INIT
    # watchdog chip reset
    WREG    R_REG_WDT_MAX_LO, 1
    WREG    R_REG_WDT_MAX_HI, 0
    WREG    R_REG_WDT_CLR, 1
    DELAYMS 100
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_InitUART
# Macro PUTC_Char
# @note UART output function
#-------------------------------------------------------------------------------------------------

    .macro   CHIP_InitUART
#BOOT_InitUART:
    WREG_INIT
    WREG (CLK_GEN0_BASE+0x13*4), 0x0C01

### Switch PAD ISP###################################################
    #enable reg_uart_rx_enable
    RREG    $11,        0xbf001c24
    ori     $11, $11,   0x0800
    sw      $11,        0xbf001c24

    #Select to PIU_UART0
    RREG    $11,        0xbf203d4c
    ori     $11, $11,   0x0004
    sw      $11,        0xbf203d4c

### Switch PAD ISP end###################################################

    RREG    $11,         0xbf201318   # Kernel need
    ori     $11, $11,    0x0080
    sw      $11,         0xbf201318

    WREG 0xbf201300,     UART_DIVISOR & 0x00ff
    WREG 0xbf201308,     ((UART_DIVISOR >> 8) & 0x00ff)

    RREG    $11,         0xbf201318   # Kernel need
    and     $11, $11,    0xFF7F
    sw      $11,         0xbf201318

    WREG 0xbf201310      0x0000        # Kernel need
    WREG 0xbf201310      0x0007

    WREG 0xbf201318      0x0000        # Kernel need
    WREG 0xbf201318      0x0003
    WREG 0xbf201320      0x0000        # Kernel need
    WREG 0xbf201308      0x0000        # Kernel need

    .endm


    .macro  PUTC char
    WREG_INIT
    WREG    0xBF201300, (\char)
    .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_WDTDisable
# @note Disable WDT
#-------------------------------------------------------------------------------------------------
        .macro  CHIP_WDTDisable
#CHIP_WDTDisable:
        WREG_INIT
        # Disable Watchdog Timer
        WREG    R_REG_WDT_MAX_LO, 0
        WREG    R_REG_WDT_MAX_HI, 0
        .endm


#-------------------------------------------------------------------------------------------------
# Macro CHIP_DbgActive
# @note Activate DBG
#-------------------------------------------------------------------------------------------------
        .macro  CHIP_DbgActive
#CHIP_DbgActive:
        WREG_INIT
        # Enable debug mode
        li      $8, R_CA_DBGMODE
        lw      $9, 0($8)
        ori     $9, CA_DBGMODE_ACTIVE
        sw      $9, 0($8)
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_IsL2Enable
# @param  None
# @return 0 L2 is disabled
# @return 1 L2 is enabled
#-------------------------------------------------------------------------------------------------
        .macro CHIP_IsL2Enable reg
        li      \reg, 1
        .endm

#-------------------------------------------------------------------------------------------------
# Macro CHIP_EnableL2Cache
# @param  None
# @return None
# @note   Enable L2 Cache
#-------------------------------------------------------------------------------------------------
        .macro CHIP_EnableL2Cache
#CHIP_EnableL2Cache:
        # No l2 cache, so empty it
        .endm

#-------------------------------------------------------------------------------------------------
# CHIP_EnableFPU
# @param  None
# @return None
# @note   Enable FPU
#-------------------------------------------------------------------------------------------------
        .macro CHIP_EnableFPU
#CHIP_EnableFPU:
        # No l2 cache, so empty it
        .endm
