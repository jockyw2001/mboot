# Add Main PLL & MIPS CLK Select here
# Power up Atop

# Initial DMD ANA MISC
WREG (REG_ADDR_BASE + (0x101e38 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x112002 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x1e04


WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x0604
WREG (REG_ADDR_BASE + (0x112866 <<1 )) , 0x2400  // Set MPLL_LOOP_DIV_FIRST and SECOND
WREG (REG_ADDR_BASE + (0x112860 <<1 )) , 0x1c01  // Set MPLL_ADC_DIV_SEL

WREG (REG_ADDR_BASE + (0x112840 <<1 )) , 0x0000  // Set MPLL_LOOP_DIV_FIRST and SECOND
WREG (REG_ADDR_BASE + (0x112878 <<1 )) , 0x0000  // Set MPLL_ADC_DIV_SEL

WREG (REG_ADDR_BASE + (0x1128a2 <<1 )) , 0x0001  // Set 864  Mode
#WREG (REG_ADDR_BASE + (0x1128a2 <<1 )) , 0x0003  // Set 432  Mode

DELAYUS_12M 1000

# Initial DMD ANA MISC Done

# MIPS ANA MISC Start

# MIPSPLL
#if (CONFIG_CPU_667MHZ == 1) // 667Mhz
WREG (REG_ADDR_BASE + (0x110ac0 <<1 )) , 0x5a94   # reg_mipspll_loop_div2[7:0] Divde N
WREG (REG_ADDR_BASE + (0x110ac2 <<1 )) , 0x007c   # reg_mipspll_loop_div2[7:0] Divde N
WREG (REG_ADDR_BASE + (0x110ac4 <<1 )) , 0x0001
WREG (REG_ADDR_BASE + (0x110a22 <<1 )) , 0x1e80
DELAYUS_12M 1000
# MIPS CLKGEN
WREG (REG_ADDR_BASE + (0x101d1e <<1 )) , 0x0020
WREG (REG_ADDR_BASE + (0x101d1e <<1 )) , 0x0021

#elif (CONFIG_CPU_1008MHZ == 1) // 1008Mhz
WREG (REG_ADDR_BASE + (0x110ac0 <<1 )) , 0xf1a9   # reg_mipspll_loop_div2[7:0] Divde N
WREG (REG_ADDR_BASE + (0x110ac2 <<1 )) , 0x0052   # reg_mipspll_loop_div2[7:0] Divde N
WREG (REG_ADDR_BASE + (0x110ac4 <<1 )) , 0x0001
WREG (REG_ADDR_BASE + (0x110a22 <<1 )) , 0x1e80
DELAYUS_12M 1000
# MIPS CLKGEN
WREG (REG_ADDR_BASE + (0x101d1e <<1 )) , 0x0020
WREG (REG_ADDR_BASE + (0x101d1e <<1 )) , 0x0021

#else
#error "no such setting~"
#endif
# Turn on all CLK

# WREG (REG_ADDR_BASE + (0x100b00 <<1 )) , 0x0004
WREG (REG_ADDR_BASE + (0x100b2e <<1 )) , 0x0300
WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0000

//--------------------------------------
//switch clk_mcu to 216MHz
//--------------------------------------

WREG (REG_ADDR_BASE + (0x100b20 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x100bfa <<1 )) , 0x0001

# CLKGEN0
WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x8000

# SMC PAD
WREG (REG_ADDR_BASE + (0x101e50 <<1 )) , 0x0100
WREG (REG_ADDR_BASE + (0x102958 <<1 )) , 0x0040 //Power down SMC pad


# After MIPS PLL is Stable

WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x8000

//--------------------------------------
//       Bypass Byte Write Parser
//--------------------------------------
WREG (REG_ADDR_BASE + (0x160f00 <<1 )) , 0x2000 //mips
WREG (REG_ADDR_BASE + (0x160f02 <<1 )) , 0x2000 //vd_r2
WREG (REG_ADDR_BASE + (0x160f04 <<1 )) , 0x2000 //sec_r2_0
WREG (REG_ADDR_BASE + (0x160f06 <<1 )) , 0x2000 //sec_r2_1
WREG (REG_ADDR_BASE + (0x160f08 <<1 )) , 0x2000 //sec_mcu51
WREG (REG_ADDR_BASE + (0x160f0a <<1 )) , 0x2000 //mcu51
WREG (REG_ADDR_BASE + (0x160f0c <<1 )) , 0x2000 //gpd
WREG (REG_ADDR_BASE + (0x160f0e <<1 )) , 0x2000 //ge
WREG (REG_ADDR_BASE + (0x160f10 <<1 )) , 0x2000 //mvd
WREG (REG_ADDR_BASE + (0x160f12 <<1 )) , 0x2000 //usb_0
WREG (REG_ADDR_BASE + (0x160f14 <<1 )) , 0x2000 //usb_1
WREG (REG_ADDR_BASE + (0x160f16 <<1 )) , 0x2000 //fcie
WREG (REG_ADDR_BASE + (0x160f18 <<1 )) , 0x2000 //piu_0
WREG (REG_ADDR_BASE + (0x160f1a <<1 )) , 0x2000 //piu_1
WREG (REG_ADDR_BASE + (0x160f1c <<1 )) , 0x2000 //tsp_0
WREG (REG_ADDR_BASE + (0x160f1e <<1 )) , 0x2000 //tsp_1
WREG (REG_ADDR_BASE + (0x160f20 <<1 )) , 0x2000 //tsp_2
WREG (REG_ADDR_BASE + (0x160f22 <<1 )) , 0x2000 //tsp_3
WREG (REG_ADDR_BASE + (0x160f24 <<1 )) , 0x2000 //tsp_4
WREG (REG_ADDR_BASE + (0x160f26 <<1 )) , 0x2000 //tsp_5
WREG (REG_ADDR_BASE + (0x160f28 <<1 )) , 0x2000 //tsp_6
WREG (REG_ADDR_BASE + (0x160f2a <<1 )) , 0x2000 //tsp_7
WREG (REG_ADDR_BASE + (0x160f2c <<1 )) , 0x2000 //au_r2
WREG (REG_ADDR_BASE + (0x160f2e <<1 )) , 0x2000 //evd
WREG (REG_ADDR_BASE + (0x160f30 <<1 )) , 0x2000 //v9_dma


