
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
# 552MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0009    #input_div2, {output_div1,loop_div1,input_div1}
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0017    #output_div2, loop_div2
# 504MHz
#WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0009    #input_div2, {output_div1,loop_div1,input_div1}
#WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0015    #output_div2, loop_div2
# 456MHZ
#WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0009    #input_div2, {output_div1,loop_div1,input_div1}
#WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0013    #output_div2, loop_div2
# 312MHz
#WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x0009    #input_div2, {output_div1,loop_div1,input_div1}
#WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x000d    #output_div2, loop_div2


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

