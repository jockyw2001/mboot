
# turn on MainPLL, MIPSLL
# swch 4

# MainPLL : DMPLL

WREG (REG_ADDR_BASE + (0x101e38 <<1 )) , 0x0000   #reg_dmdtop_dmd_sel@0x1c[8]=0
WREG (REG_ADDR_BASE + (0x112002 <<1 )) , 0x0070   #reg_dmd_ana_misc_rst@0x01[13] reg_dmdtop =0
WREG (REG_ADDR_BASE + (0x112840 <<1 )) , 0x0000   #reg_pwdn_ref@0x20[4]=0
WREG (REG_ADDR_BASE + (0x112818 <<1 )) , 0x0000   #reg_pwdn_adci@0x0c[0]=0, reg_pwdn_adcq@0x0c[1]=0, reg_pwdn_clpbuf@0x0c[2]=0
WREG (REG_ADDR_BASE + (0x112878 <<1 )) , 0x1000   #reg_dmldo_en@0x3c[12]=1
WREG (REG_ADDR_BASE + (0x112860 <<1 )) , 0x9001   #reg_mpll_divX_pd@0x30[9:1]=0, reg_mpll_adc_div_sel@0x30[15:10]=6'h24=6'd36
WREG (REG_ADDR_BASE + (0x112866 <<1 )) , 0x2400   #reg_mpll_loop_div_second@0x33[15:8]=8'h24=8'd36
WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x0604   #reg_mpll_pd@0x35[7]=0

# MIPSPLL
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0065   #clr bit4, disable power down
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0045   #clr Bit5, disable reset
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0005   #clr bit6, disable power reset

.if (CONFIG_CPU_552MHZ==1)
# 552 MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00C5   #{output_div1,loop_div1,input_div1} //input_div2
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0017   #loop_div2 //output_div2
.elseif(CONFIG_CPU_600MHZ==1)
# 600 MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00C5   #{output_div1,loop_div1,input_div1} //input_div2
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0019   #loop_div2 //output_div2
.elseif(CONFIG_CPU_456MHZ==1)
# 456 MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00C5   #{output_div1,loop_div1,input_div1} //input_div2
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0013   #loop_div2 //output_div2
.elseif(CONFIG_CPU_696MHZ==1)
# 696 MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00C5   #{output_div1,loop_div1,input_div1} //input_div2
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x001D   #loop_div2 //output_div2
.elseif(CONFIG_CPU_288MHZ==1)
# 288 MHz
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00C5   #{output_div1,loop_div1,input_div1} //input_div2
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x000C   #loop_div2 //output_div2
.else
error no such MIPS config
.endif

