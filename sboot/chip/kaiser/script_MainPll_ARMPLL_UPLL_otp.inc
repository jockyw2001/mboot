#[*] BOOTROM_MPLL_INIT
@-------------------------------------------------
@Macro: drv_clk_mpll_init
@-------------------------------------------------

    WREG 0x1F206700 0x0010

    //<< RFDAC >>
    WREG   0x1F247484 0x0000    @ Disable RFDAC

    //<< RIU MASK>>
    WREG   0x1F200248 0x6000	@ mask miic0/miic1 xiu ready

    WREG_B 0x1F001C25 0x00	@ dmatop ref_pd=0

    WREG 0x1F206700 0x0011

    @Module: MPLL (0x101E)
    WREG    0x1F203C70, 0x0000      @MPLL 3.3V XTAL Power on, [8] reg_dmdtop_dmd_sel

    WREG 0x1F206700 0x0012

    @Module: DMDANA MPLL (0x1128)
    WREG_B  0x1F224005, 0x00      @[13]reg_dmd_ana_misc_rst    = 0

@-------------------------------------------------
@[2:0]       reg_dmldo_vsel
@[8:4]       reg_dmldo_tst
@[12]        reg_dmldo_pd    = 0
@-------------------------------------------------
    WREG    0x1F2250F0, 0x0000      @mpll
    WREG 0x1F206700 0x0013
@-------------------------------------------------
@[2:0]        reg_mpll_ictrl
@[3]  reg_mpll_in_sel                 1.8V or 3.3V reference clock domain select (1'b0=0==>3.3 V reference clock domain)
@[4]  reg_mpll_xtal2adc_sel           select the XTAL clock bypass to MPLL_ADC_CLK
@[5]  reg_mpll_xtal2next_pll_sel      crystal clock bypass to next PLL select
@[6]  reg_mpll_vco_offset             set VCO initial offset frequency
@[7]  reg_mpll_pd                     gated reference clock and power down PLL analog_3v: 1=power down
@[8]  reg_xtal_en                     XTAL enable register; 1:enable
@[10:9]reg_xtal_sel                   XTAL driven strength select.
@[11] reg_mpll_porst                  MPLL input  power on reset, connect to reg as MPLL_RESET
@[12] reg_mpll_reset                  PLL software reset; 1:reset
@[13] reg_pd_dmpll_clk                XTAL to MPLL clock reference power down
@[14] reg_pd_3p3_1                    XTAL to CLK_24M_3P3_1 power down
@[15] reg_pd_3p3_2                    XTAL to CLK_24M_3P3_2 power down
@-------------------------------------------------
    WREG    0x1F2250D4, 0x0000      @mpll

    WREG 0x1F206700 0x0014
@-------------------------------------------------
@0    0       reg_mpll_clk_dp_pd     dummy
@1    1       reg_adc_clk_pd         ADC output clock power down
@2    2       reg_mpll_div2_pd       MPLL_DIV2 power down
@3    3       reg_mpll_div3_pd       MPLL_DIV3 power down
@4    4       reg_mpll_div4_pd       MPLL_DIV4 power down
@5    5       reg_mpll_div8_pd       MPLL_DIV8 power down
@6    6       reg_mpll_div10_pd      MPLL_DIV10 power down
@7    7       reg_mpll_div5_pd       MPLL_DIV5 power down
@8    8       reg_mpll_div7_pd       MPLL_DIV7 power down
@9    9       reg_mpll_div14_pd      MPLL_DIV14 power down
@15   10      reg_mpll_adc_div_sel   select the ADC clock divide ratio,
@div_ratio=(2^5*b5+2^4*b4+2^3*b3+2^2*b2+2^1*b1+b0);                                                             @[b5:b0]=[000000], div_ratio=64 ADC clk=XTAL_IN * (LOOP_DIV_FIRST*LOOP_DIV_SECOND)/(INPUT_DIV_FIRST*INPUT_DIV_SECOND)/div_ratio
@-------------------------------------------------
    WREG    0x1F2250C0 0x4900   @ MPLL div clocks enable


    WREG    0x1F2250CC 0x1201  @ Loop divider ; VCO = 24*2^2*9 = 864
                              @1    0       reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
                              @15   8       reg_mpll_loop_div_second      feedback divider, div by binary data number


    WREG_B  0x1F001C25 0x01    @ PWDN_REF_pm => reg_ext_xtali_pwdn_ref = 0!!!

    WREG 0x1F206700 0x0015
@=======================================================================================================
@ MIPSPLL
@=======================================================================================================
    WREG    0x1F221800 0x3700         @ Enable PLL
                                          @`RIU_W((`RIUBASE_ANA_MISC>>1)+7'h01, 2'b11, 16'h5300);       @ CPU clock = 12*0x53h = 996 MHz

    .if (CONFIG_CPU_780MHZ == 1)
    WREG    0x1F221804 0x4100        @MIPSPLL enable --> 780MHz for bring up
    .endif

    .if (CONFIG_CPU_804MHZ == 1)
    WREG    0x1F221804 0x4300        @MIPSPLL enable --> 804MHz for bring up
    .endif

    .if (CONFIG_CPU_840MHZ == 1)
    WREG    0x1F221804 0x4600        @MIPSPLL enable --> 840MHz for bring up
    .endif

    .if (CONFIG_CPU_852MHZ == 1)
    WREG    0x1F221804 0x4700        @MIPSPLL enable --> 852MHz for bring up
    .endif

    .if (CONFIG_CPU_864MHZ == 1)
    WREG    0x1F221804 0x4800        @MIPSPLL enable --> 864MHz for bring up
    .endif

    .if (CONFIG_CPU_876MHZ == 1)
    WREG    0x1F221804 0x4900        @MIPSPLL enable --> 876MHz for bring up
    .endif

    .if (CONFIG_CPU_888MHZ == 1)
    WREG    0x1F221804 0x4a00        @MIPSPLL enable --> 888MHz for bring up
    .endif

    .if (CONFIG_CPU_900MHZ == 1)
    WREG    0x1F221804 0x4b00        @MIPSPLL enable --> 900MHz for bring up
    .endif

    .if (CONFIG_CPU_984MHZ == 1)
    WREG    0x1F221804 0x5200        @MIPSPLL enable --> 984MHz for bring up
    .endif
    
    .if (CONFIG_CPU_1008MHZ == 1)
    WREG    0x1F221804 0x5400        @MIPSPLL enable --> 1008MHz for bring up
    .endif

    WREG 0x1F206700 0x0016

@=======================================================================================================
@ MIUPLL
@=======================================================================================================
    WREG    0x1F221880 0x0003	@ Enable PLL
                                @`RIU_W((`RIUBASE_ANA_MISC_MIU>>1)+7'h01, 2'b11, 16'h3200);
                                @ [15:8] loop_div_second(LDS), default 0x32 = 300 MHz
                                @ clock = 24*LDS/4

    WREG    0x1F221884 0x3200	@ Clock = 24*50/4 = 300 MHz