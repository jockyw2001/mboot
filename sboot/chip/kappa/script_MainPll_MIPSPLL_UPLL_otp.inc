
# turn on MainPLL, MIPSLL
# swch 4

# MainPLL : DMPLL

WREG (REG_ADDR_BASE + (0x101e38 <<1 )) , 0x0000   #reg_dmdtop_dmd_sel@0x1c[8]=0
WREG (REG_ADDR_BASE + (0x112002 <<1 )) , 0x0070   #reg_dmd_ana_misc_rst@0x01[13] reg_dmdtop =0
WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x1e03   #reg_mpll_pd@0x35[7]=0
WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x0603
WREG (REG_ADDR_BASE + (0x112866 <<1 )) , 0x1201
WREG (REG_ADDR_BASE + (0x112860 <<1 )) , 0x1c01
WREG (REG_ADDR_BASE + (0x112840 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x112878 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x101e38 <<1 )) , 0x0300

# MIPSPLL
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0005   #clr bit4, disable power down
WREG (REG_ADDR_BASE + (0x110c2a <<1 )) , 0x000f

# DSP_PLL
WREG (REG_ADDR_BASE + (0x110c80 <<1 )) , 0x0028
WREG (REG_ADDR_BASE + (0x110c82 <<1 )) , 0x0000

# Audio DSP Frequency
WREG (REG_ADDR_BASE + (0x110C86 <<1 )) , 0x0018   #reg_mipspll_loop_div2

.if (CONFIG_CPU_600MHZ==1)
# 600 MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x001A   #reg_mipspll_loop_div1[2:3]&reg_mipspll_output_div1[4:5]
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0064   #reg_mipspll_loop_div2
.else
.error "no such MIPS CPU Frequency config"
.endif