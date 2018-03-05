    .macro BootRomInitPll

    #//adc_atop
    WREG_B      0xBF204A1C, 0x3F        #//Soft-reset for DAC/ATOP/PLLB/ADCB/PLLA/ADCA
    WREG_B      0xBF204A1C, 0x00

    WREG        0xBF204A10, 0x0000      #//Power-on for related adc_atop IPs
    WREG        0xBF204A14, 0x0000
    WREG        0xBF204A18, 0x0000

    WREG        0xBF204AFC, 0x0030
    WREG_B      0xBF204B20, 0x08
    WREG_B      0xBF204B24, 0x08
    WREG_B      0xBF204B40, 0x00
    WREG_B      0xBF204B48, 0x00

    WREG        0xBF204B78, 0x03C0

    #//dvi_atop
    WREG        0xBF221218, 0x0000      #//Relase soft-reset for DVI/HDMI/HDCP blocks
    WREG        0xBF221380, 0x0000      #//Power-on for DVI/HDMI/HDCP blocks
    WREG        0xBF2213A4, 0x0000      #//Power-on for DVT port 2
    WREG        0xBF22120C, 0x0000      #//Enable clock for related dvi_atop IPs

    #//dvi_atop_p1_p2
    WREG        0xBF226418, 0x0000      #//Enable clock for related dvi_atop_p1 IPs
    WREG        0xBF226580, 0x0000      #//Power-on for related dvi_atop_p1 IPs

    WREG        0xBF226818, 0x0000      #//Enable clock for related dvi_atop_p2 IPs
    WREG        0xBF226980, 0x0000      #//Power-on for related dvi_atop_p2 IPs

    #//pm_sleep
    WREG        0xBF001D28, 0x0000      #//Power-on dvi_atop in PM_SLEEP
    WREG        0xBF001D2C, 0x0000

    #//ana_misc (MPLL)
    WREG        0xBF203C70, 0x0000      #//Power-on for PD_3P3_1, PD_3P3_0 and PD_3P3_USB1


    WREG_B      0xBF221800, 0x40        #//Test[6]: Enable Lock Flag
    WREG        0xBF221804, 0x0001      #//Enable MPLL / MPLL_CLK_ADC216M / MPLL_CLK_DP432M, MPLL_IN_Sel=1

    #//ana_misc (MIPSPLL)
    WREG_B      0xBF221840, 0x40        #//Test[6]: Enable Lock Flag
#    WREG        0xBF221848, 0x0411      #//Set divided by 2 control for 3.3Volt input clock: /2
                                        #//Set MPLL input-divider control: /2
                                        #//Set MPLL fisrt loop-divider control: /1
                                        #//Set Loop Filter Resistance Selection: 14k Ohm

    WREG_B      0xBF22184C, (CPU_CLOCK / 24)
#    WREG        0xBF22184C, ((CPU_CLOCK / 12) + 0x1100)

.if (CPU_CLOCK >= 600)
#    WREG        0xBF221850, 0x00A0      #//Set change-pump current and Ibias output current
    WREG_B      0xBF221850, 0x60
.endif

    WREG        0xBF221844, 0x0601      #//[8]Power-on MIPSPLL=0, [0]MPLL_IN_SEL=1

    #//USBPLL = 480Mhz
    #//USBC0 <--> UTMI0
    WREG_B      0xBF200E00, 0x0a        #//Disable MAC initial suspend, Reset UHC
    WREG_B      0xBF200E00, 0x28        #//Release UHC reset, enable UHC and OTG XIU function

    WREG_B      0xBF207544, 0xe0        #//Set PLL_TEST[23:21] for enable 480MHz clock
.if 0           
    WREG_B      0xBF207540, 0x03        #//Set PLL_TEST[1] for PLL multiplier 20X
    WREG_B      0xBF207542, 0x80        #//To have accurate 480MHz
.else           
    WREG        0xBF207540, 0x8003
.endif          
    WREG        0xBF207504, 0x2084      #//Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        0xBF207510, 0x0000      #//#7=0 Enable band-gap current  #3=0 Disable force_pll_on
    WREG        0xBF207500, 0x6bc3      #//reg_pdn: bit<15>, bit <2> ref_pdn # Turn on reference voltage and regulator
    DELAYUS     1000

    WREG        0xBF207500, 0x69c3      #//Turn on UPLL, reg_pdn: bit<9>
    DELAYUS     2000

    WREG        0xBF207500, 0x0001      #//Turn all (including hs_current) use override mode
    #WREG_B     0xBF207505, 0x00        #//7=0  #TBD

    #//USBC1 <--> UTMI1
    WREG_B      0xBF200F00, 0x0a        #//Disable MAC initial suspend, Reset UHC
    WREG_B      0xBF200F00, 0x28 #0x68        #//Release UHC reset, enable UHC and OTG XIU function

    WREG_B      0xBF207444, 0xe0        #//Set PLL_TEST[23:21] for enable 480MHz clock
.if 0           
    WREG_B      0xBF207440, 0x03        #//Set PLL_TEST[1] for PLL multiplier 20X
    WREG_B      0xBF207442, 0x80        #//To have accurate 480MHz
.else           
    WREG        0xBF207440, 0x8003
.endif          
    WREG        0xBF207404, 0x2084      #//Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG        0xBF207410, 0x0000      #//Enable band-gap current
    WREG        0xBF207400, 0x6bc3      #//reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
    DELAYUS     1000

    WREG        0xBF207400, 0x69c3      #//Turn on UPLL, reg_pdn: bit<9>
    DELAYUS     2000

    WREG        0xBF207400, 0x0001      #//Turn all (including hs_current) use override mode

   	#//USBC2 <--> UTMI2
    WREG_B 		0xBF227000, 0x0a     	#//Disable MAC initial suspend, Reset UHC
    WREG_B 		0xBF227000, 0x28 #0x68     	#//Release UHC reset, enable UHC and OTG XIU function

    WREG_B 		0xBF207244, 0xe0     	#//Set PLL_TEST[23:21] for enable 480MHz clock
.if 0
    WREG_B 		0xBF207240, 0x03     	#//Set PLL_TEST[1] for PLL multiplier 20X
    WREG_B 		0xBF207242, 0x80     	#//To have accurate 480MHz
.else
    WREG        0xBF207240, 0x8003
.endif
    WREG   		0xBF207204, 0x2084   	#//Enable CLK12_SEL bit <2> for select low voltage crystal clock
    WREG   		0xBF207210, 0x0000   	#//Enable band-gap current
    WREG   		0xBF207200, 0x6bc3   	#//reg_pdn: bit<15>, bit <2> ref_pdn  # Turn on reference voltage and regulator
    DELAYUS     1000

    WREG   		0xBF207200, 0x69c3   	#//Turn on UPLL, reg_pdn: bit<9>
    DELAYUS     2000

    WREG   		0xBF207200, 0x0001   	#//Turn all (including hs_current) use override mode

#.if (ENABLE_G3D_PLL == 1)
#    WREG        0xBF221114, 0x002C    # Turn ON G3D PLL
#.endif
    .endm
