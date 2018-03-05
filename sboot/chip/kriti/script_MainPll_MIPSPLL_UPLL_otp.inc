
# turn on MainPLL, MIPSLL
# swch 4

# MainPLL : DMPLL
WREG_B (REG_ADDR_BASE + (0x000eca <<1 )) , 0x10   #enable LDO
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
.if(CONFIG_CPU_576MHZ==1)
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0060   // PLL Freq = 0x60*6 = 576 MHz
.elseif(CONFIG_CPU_624MHZ==1)
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0068   // PLL Freq = 0x68*6 = 624 MHz
.else
.error "no such MIPS config"
.endif
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0005
WREG (REG_ADDR_BASE + (0x110c2a <<1 )) , 0x000f

# DSP_PLL
WREG (REG_ADDR_BASE + (0x110c80 <<1 )) , 0x0028
WREG (REG_ADDR_BASE + (0x110c82 <<1 )) , 0x0000

# Audio DSP Frequency
WREG (REG_ADDR_BASE + (0x110C86 <<1 )) , 0x0038   #reg_mipspll_loop_div2
//===========================================================
// DACPLL
//===========================================================
WREG_B (REG_ADDR_BASE + (0x12112a <<1 )) , 0x00   //reg_dacpll_pd[0],
WREG_B (REG_ADDR_BASE + (0x12112d <<1 )) , 0x06   //reg_dacpll_loop_div[11:8], reg_dacpll_dac_mode_en[4]
WREG (REG_ADDR_BASE + (0x12112e <<1 )) , 0x0807   //reg_dacpll_dr_div  [4:0], reg_dacpll_pdreg[8], reg_dacpll_in_select[11]

// dac synth
WREG (REG_ADDR_BASE + (0x121162 <<1 )) , 0x9999   // [15:0] reg_synth_set_15_0
WREG (REG_ADDR_BASE + (0x121164 <<1 )) , 0x0019   // [15:0] reg_synth_set_31_16
WREG (REG_ADDR_BASE + (0x12116a <<1 )) , 0x0002   // [0] enable ssc 1: enalbe, 0:disable
                               										// [1] ssc_sync
WREG (REG_ADDR_BASE + (0x121166 <<1 )) , 0x0007   // step
WREG (REG_ADDR_BASE + (0x121168 <<1 )) , 0x0445   // span

WREG_B (REG_ADDR_BASE + (0x101a00 <<1 )) , 0x31   // [5] reg_sel_ext_hdclk, CLK_OUT_DAC is from DACPLL

