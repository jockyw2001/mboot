
# turn on MainPLL, MIPSLL
# swch 4

# MainPLL : DMPLL
WREG (REG_ADDR_BASE + (0x101e38 <<1 )) , 0x0000 # wriu -w 0x101e38 0x0000   //reg_dmdtop_dmd_sel@0x1c[8]=0
WREG (REG_ADDR_BASE + (0x112002 <<1 )) , 0x0070 # wriu -w 0x112002 0x0070   //reg_dmd_ana_misc_rst@0x01[13] reg_dmdtop =0
WREG (REG_ADDR_BASE + (0x112840 <<1 )) , 0x0000 # wriu -w 0x112840 0x0000   //reg_pwdn_ref@0x20[4]=0
WREG (REG_ADDR_BASE + (0x112818 <<1 )) , 0x0000
WREG (REG_ADDR_BASE + (0x112878 <<1 )) , 0x1000 # wriu -w 0x112878 0x1000   //reg_dmldo_en@0x3c[12]=1
WREG (REG_ADDR_BASE + (0x112860 <<1 )) , 0x4801
WREG (REG_ADDR_BASE + (0x112866 <<1 )) , 0x2400 # wriu -w 0x112866 0x2400   //reg_mpll_loop_div_second@0x33[15:8]=8'h24=8'd36
WREG (REG_ADDR_BASE + (0x11286a <<1 )) , 0x0604 # wriu -w 0x11286a 0x0604   //reg_mpll_pd@0x35[7]=0

# MIPSPLL
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0065    #clr bit4, disable power down
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0045    #clr Bit5, disable reset
WREG (REG_ADDR_BASE + (0x110C30 <<1 )) , 0x0005    #clr bit6, disable power reset

# CPU frequency = (24*loop_div_first*loop_div_second)/2
.if (CONFIG_CPU_792MHZ == 1)
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00D1    #loop_div_first [2:3]=0 for default 1
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0042    #loop_div_second [7:0]=0x42 for default 66
.elseif (CONFIG_CPU_840MHZ == 1)
WREG (REG_ADDR_BASE + (0x110C32 <<1 )) , 0x00D1    #loop_div_first [2:3]=0 for default 1
WREG (REG_ADDR_BASE + (0x110C34 <<1 )) , 0x0046    #loop_div_second [7:0]=0x42 for default 66
.else
error no such MIPS config
.endif

