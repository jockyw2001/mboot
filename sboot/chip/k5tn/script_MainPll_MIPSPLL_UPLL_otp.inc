# Add Main PLL & MIPS CLK Select here
# Power up Atop
//--------------------------------------
// Initial DMD ANA MISC
//--------------------------------------
WREG (REG_ADDR_BASE + (0x101e38 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x112002 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x1128ae <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x1128b0 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x112866 <<1 )) , 0x1201  // Set MPLL_LOOP_DIV_FIRST and SECOND
WREG (REG_ADDR_BASE + (0x112864 <<1 )) , 0x0700  // Enable MPLL_PDIV
WREG (REG_ADDR_BASE + (0x112860 <<1 )) , 0x1e00  // Set MPLL_ADC_DIV_SEL
WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x0603
WREG (REG_ADDR_BASE + (0x112818 <<1 )) , 0x0000

WREG (REG_ADDR_BASE + (0x112840 <<1 )) , 0x0000  // Set MPLL_LOOP_DIV_FIRST and SECOND

DELAYUS_12M 1000

//--------------------------------------
//       Bypass Byte Write Parser
//--------------------------------------
//WREG (REG_ADDR_BASE + (0x160f00 <<1 )) , 0x2000 //mips
//WREG (REG_ADDR_BASE + (0x160f02 <<1 )) , 0x2000 //vd_r2
//WREG (REG_ADDR_BASE + (0x160f04 <<1 )) , 0x2000 //sec_r2_0
//WREG (REG_ADDR_BASE + (0x160f06 <<1 )) , 0x2000 //sec_r2_1
//WREG (REG_ADDR_BASE + (0x160f08 <<1 )) , 0x2000 //sec_mcu51
//WREG (REG_ADDR_BASE + (0x160f0a <<1 )) , 0x2000 //mcu51
//WREG (REG_ADDR_BASE + (0x160f0c <<1 )) , 0x2000 //gpd
//WREG (REG_ADDR_BASE + (0x160f0e <<1 )) , 0x2000 //ge
//WREG (REG_ADDR_BASE + (0x160f10 <<1 )) , 0x2000 //mvd
//WREG (REG_ADDR_BASE + (0x160f12 <<1 )) , 0x2000 //usb_0
//WREG (REG_ADDR_BASE + (0x160f14 <<1 )) , 0x2000 //usb_1
//WREG (REG_ADDR_BASE + (0x160f16 <<1 )) , 0x2000 //fcie
//WREG (REG_ADDR_BASE + (0x160f18 <<1 )) , 0x2000 //piu_0
//WREG (REG_ADDR_BASE + (0x160f1a <<1 )) , 0x2000 //piu_1
//WREG (REG_ADDR_BASE + (0x160f1c <<1 )) , 0x2000 //tsp_0
//WREG (REG_ADDR_BASE + (0x160f1e <<1 )) , 0x2000 //tsp_1
//WREG (REG_ADDR_BASE + (0x160f20 <<1 )) , 0x2000 //tsp_2
//WREG (REG_ADDR_BASE + (0x160f22 <<1 )) , 0x2000 //tsp_3
//WREG (REG_ADDR_BASE + (0x160f24 <<1 )) , 0x2000 //tsp_4
//WREG (REG_ADDR_BASE + (0x160f26 <<1 )) , 0x2000 //tsp_5
//WREG (REG_ADDR_BASE + (0x160f28 <<1 )) , 0x2000 //tsp_6
//WREG (REG_ADDR_BASE + (0x160f2a <<1 )) , 0x2000 //tsp_7
//WREG (REG_ADDR_BASE + (0x160f2c <<1 )) , 0x2000 //au_r2
//WREG (REG_ADDR_BASE + (0x160f2e <<1 )) , 0x2000 //evd
//WREG (REG_ADDR_BASE + (0x160f30 <<1 )) , 0x2000 //v9_dma


