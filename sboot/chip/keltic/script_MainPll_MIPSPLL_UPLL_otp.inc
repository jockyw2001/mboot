# Add Main PLL & MIPS CLK Select here
# Power up Atop

# Initial DMD ANA MISC
WREG (REG_ADDR_BASE + (0x100bfa <<1 )) , 0x0001 // * 

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

DELAYMS 1

# Initial DMD ANA MISC Done

# Turn on all CLK

# WREG (REG_ADDR_BASE + (0x100b00 <<1 )) , 0x0004
WREG (REG_ADDR_BASE + (0x100b2e <<1 )) , 0x0300
WREG (REG_ADDR_BASE + (0x100b3e <<1 )) , 0x0000

# MIPS ANA MISC Start

WREG (REG_ADDR_BASE + (0x110c36 <<1 )) , 0x0000   # 
 

# Set MIPS Divider

.if(CONFIG_CPU_312MHZ==1)
# 312 MHz
WREG (REG_ADDR_BASE + (0x110c32 <<1 )) , 0x000e   # reg_mipspll_output_div1[5:4] reg_mipspll_loop_div1[3:2] , reg_mipspll_input_div1[1:0]
WREG (REG_ADDR_BASE + (0x110c34 <<1 )) , 0x001a   # reg_mipspll_loop_div2[7:0] Divde N
WREG (REG_ADDR_BASE + (0x110c2a <<1 )) , 0x0028   # enable CLK
.elseif(CONFIG_CPU_504MHZ==1)
# 504 MHz
WREG (REG_ADDR_BASE + (0x110c32 <<1 )) , 0x001a   # reg_mipspll_output_div1[5:4] reg_mipspll_loop_div1[3:2] , reg_mipspll_input_div1[1:0]
WREG (REG_ADDR_BASE + (0x110c34 <<1 )) , 0x0054   # reg_mipspll_loop_div2[7:0] Divde N
WREG (REG_ADDR_BASE + (0x110c2a <<1 )) , 0x0008   # enable CLK
.else
.error "no such MIPS config"
.endif

WREG (REG_ADDR_BASE + (0x110c30 <<1 )) , 0x0005   # disable power down
WREG (REG_ADDR_BASE + (0x110c80 <<1 )) , 0x003f
WREG (REG_ADDR_BASE + (0x110c82 <<1 )) , 0x0000

DELAYMS 1   
# After MIPS PLL is Stable

WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x0000   
WREG (REG_ADDR_BASE + (0x100b22 <<1 )) , 0x8000



