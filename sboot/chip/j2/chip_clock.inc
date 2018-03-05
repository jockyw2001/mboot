    .macro BootRomInitClock

                                        #//MCU
# FIXME: OTP not stable
#    WREG_B      0xBF201641, 0x00        #//Set CLK_MCU to 216MHz (Enabled)
#    WREG_B      0xBF201640, 0x01

#   WREG_B      0xBF201645, 0x80        #reg_ckg_mips=mipspll_vco,default=PASS
#   WREG_B      0xBF201648, 0x84        #reg_ckg_HK_R2=240MHz,default=PASS

                                        #//UART
    WREG_B      0xBF20164D, 0x0C        #//Set CLK_UART0 to 123MHz (Enabled)
    WREG_B      0xBF201650, 0x0C        #//Set CLK_UART1 to 123MHz (Enabled)
    WREG_B      0xBF201651, 0x0C        #//Set CLK_UART2 to 123MHz (Enabled)
    WREG_B      0xBF20165C, 0x0C        #//Set CLK_UART3 to 123MHz (Enabled)
    WREG_B      0xBF20165D, 0x0C        #//Set CLK_UART4 to 123MHz (Enabled)

                                        #//SPI Flash
    WREG_B      0xBF201658, 0x14        #//Set CLK_SPI to 54MHz (Enabled)
    WREG_B      0xBF201668, 0x00        #//Set CLK_PCM to 27MHz (Enabled)
    WREG_B      0xBF201669, 0x00        #//Set CLK_TCK (Enabled)

                                        #//MIU
    WREG_B      0xBF201678, 0x00        #//Set MEMPLL_CLK_BUF to mempll0_clk025x (Enabled)
    WREG_B      0xBF201679, 0x01        #//Set CLK_MPLL_SYN to 432MHz (Enabled)

.if (CPU_CLOCK == 240)
	WREG_B      0xBF20167C, 0x10        #//Set CLK_MIU to 240MHz (Enabled)
.else
	WREG_B      0xBF20167C, 0x0C        #//Set CLK_MIU to 216MHz (Enabled)
.endif

    WREG_B      0xBF20167D, 0x00        #//Set CLK_MIU_REC to XTAL div 8 (Enabled)

                                        #//VD
    WREG_B      0xBF201680, 0x00        #reg_clk_vd_sel=VD_ADC_CLK,default=PASS
    WREG_B      0xBF201681, 0x01        #reg_ckg_vd   =CLK_VD,default=GATING
    WREG_B      0xBF201684, 0x11        #reg_ckg_vdmcu=108MHz,default=GATING
    WREG_B      0xBF201688, 0x01        #reg_ckg_mcu_mail0=reg_ckg_mcu,default=GATING
    WREG_B      0xBF201689, 0x01        #reg_ckg_mcu_mail1=reg_ckg_mcu,default=GATING
    WREG_B      0xBF20168C, 0x01        #reg_ckg_vd2x=clk gating,default=GATING
    WREG_B      0xBF20168D, 0x01        #reg_ckg_vd_32fsc=VD_ADC_CLK,default=GATING

                                        #//VE
    WREG_B      0xBF201690, 0x01        #reg_ckg_ve =27MHz,default=GATING
    WREG_B      0xBF201691, 0x09        #reg_ckg_vedac=108MHz,default=GATING
    WREG_B      0xBF201694, 0x01        #reg_ckg_ve_in=by source,default=GATING
    WREG_B      0xBF201698, 0x01        #reg_ckg_daca2=by source,default=GATING
    WREG_B      0xBF201699, 0x01        #reg_ckg_dacb2=by source,default=GATING

                                        #//TSP
    WREG_B      0xBF2016A0, 0x01        #reg_ckg_ts0=by source,default=GATING
    WREG_B      0xBF2016A1, 0x01        #reg_ckg_ts1=by source,default=GATING
    WREG_B      0xBF2016A4, 0x01        #reg_ckg_ts2=by source,default=GATING
    WREG_B      0xBF2016A8, 0x01        #reg_ckg_tsp=144MHz,default=GATING
    WREG_B      0xBF2016A9, 0x01        #reg_ckg_stc0=STC0,default=GATING

                                        #//VD_MHEG5
    WREG_B      0xBF2016C0, 0x05        #reg_ckg_vd_mheg5=144MHz,default=GATING

                                        #//HVD
    WREG_B      0xBF2016C4, 0x05        #reg_ckg_hvd =160MHz,default=GATING

                                        #//JPD
    WREG_B      0xBF2016D4, 0x05        #reg_ckg_jpd=123MHz,default=GATING

                                        #//MVD
    WREG_B      0xBF2016E4, 0x01        #reg_ckg_mvd=144MHz,default=GATING
    WREG_B      0xBF2016E5, 0x05        #reg_ckg_mvd2=172MHz,default=GATING
    WREG_B      0xBF2016E8, 0x01        #reg_ckg_mvd_chroma=clk gating,default=GATING
    WREG_B      0xBF2016E9, 0x01        #reg_ckg_mvd_luma_a=clk gating,default=GATING
    WREG_B      0xBF2016EC, 0x01        #reg_ckg_mvd_luma_b=clk gating,default=GATING
    WREG_B      0xBF2016ED, 0x01        #reg_ckg_mvd_luma_c=clk gating,default=GATING
    WREG_B      0xBF2016F0, 0x01        #reg_ckg_mvd_rmem=clk gating,default=GATING
    WREG_B      0xBF2016F1, 0x01        #reg_ckg_mvd_rmem1=clk gating,default=GATING

    WREG_B      0xBF201700, 0x00        #reg_ckg_gopg0=clk_odclk_p,default=PASS
    WREG_B      0xBF201701, 0x00        #reg_ckg_gopg1=clk_odclk_p,default=PASS
    WREG_B      0xBF201704, 0x00        #reg_ckg_gopg2=clk_odclk_p,default=PASS
    WREG_B      0xBF201705, 0x04        #reg_ckg_gopd=CLK_ODCLK,default=PASS
    WREG_B      0xBF201708, 0x00        #reg_ckg_gopg3=clk_odclk_p,default=PASS
    WREG_B      0xBF20170C, 0x00        #reg_ckg_psram0=clk enable,default=PASS
    WREG_B      0xBF20170D, 0x00        #reg_ckg_psram1=clk enable,default=PASS

    WREG_B      0xBF201720, 0x00        #reg_ckg_ge    =216MHz,default=PASS

    WREG_B      0xBF201730, 0x01        #reg_ckg_dc0=synchronous,default=GATING
    WREG_B      0xBF201731, 0x01        #reg_ckg_dc1=synchronous,default=GATING
    WREG_B      0xBF201734, 0x01        #reg_ckg_sub_dc0=synchronous,default=GATING
    WREG_B      0xBF201735, 0x01        #reg_ckg_sub_dc1=synchronous,default=GATING

                                        #//Scaler
    WREG_B      0xBF20175C, 0x00        #//Set CLK_FIFO_MINI to CLK_LPLL (Enabled)
    WREG_B      0xBF201760, 0x01        #//Set CLK_LPLL_SYN to 432MHz (Enabled)
    WREG_B      0xBF201744, 0x44        #//Set CLK_FICLK_F1 to CLK_FCLK (Enabled) / CLK_FICLK_3D to CLK_FCLK (Enabled)
    WREG_B      0xBF201745, 0x44        #//Set CLK_FICLK_F2 to CLK_FCLK (Enabled) / CLK_FICLK2_F2 to CLK_FCLK (Enabled)
    WREG_B      0xBF201748, 0x01        #//Set CLK_FODCLK to clk_odclk (Disabled)
    WREG_B      0xBF201749, 0x09        #//Set CLK_VCLK to CLK_ODCLK (Disabled)
    WREG_B      0xBF20174C, 0x0C        #//Set CLK_ODCLK to LPLL output clock (Enabled)
    WREG_B      0xBF20174D, 0x0C        #//Set CLK_BT656 to LPLL output clock (Enabled)
    WREG_B      0xBF201750, 0x14        #//Set CLK_IDCLK_F0 to XTAL (Enabled)
    WREG_B      0xBF201751, 0x14        #//Set CLK_IDCLK_F1 to XTAL (Enabled)
    WREG_B      0xBF201754, 0x14        #//Set CLK_IDCLK_F2 to XTAL (Enabled)
    WREG_B      0xBF2017DC, 0x00        #//Set CLKO_1x_4DIGTAL Clock (Enabled)

                                        #//EMAC
    WREG_B      0xBF201780, 0x00        #//Set CLK_EMAC_AHB to 123MHz (Enabled)
    WREG_B      0xBF201781, 0x04        #//Set CLK_EMAC_RX to CLK_EMAC_REF / 2 (25MHz) (Enabled)
    WREG_B      0xBF201784, 0x04        #//Set CLK_EMAC_TX to CLK_EMAC_REF / 2 (25MHz) (Enabled)
    WREG_B      0xBF201785, 0x00        #//Set CLK_EMAC_TX_REF to CLK_EMAC_TX_IN (50MHz) (Enabled)
    WREG_B      0xBF201788, 0x00        #//Set CLK_EMAC_RX_REF to CLK_EMAC_RX_IN (50MHz) (Enabled)

                                        #//NAND Flash
    WREG_B      0xBF201790, 0x1C        #//Set CLK_NFIE to 32MHz (Enabled)
    WREG_B      0xBF201798, 0x09        #//Set CLK_ONIF to 27MHz (Disabled)

    WREG_B      0xBF2017A0, 0x1C        #//Set CLK_MPIF to 13.5MHz (Enabled)

                                        #//DEMOD
    WREG_B      0xBF206600, 0x11        #//Set CLK_ATSC_DVB_DIV divider factor to 0x11
    WREG_B      0xBF206601, 0x05        #//Set CLK_ATSC_DVB_DIV to clk_dmplldiv3 / Set CLK_DEMOD_TS2PAD to free-run clock

    WREG_B      0xBF206610, 0x01        #//Set CLK_ATSC_TS to clk_atsc_dvb_div (Disabled)
    WREG_B      0xBF206611, 0x05        #//Set CLK_DVBTC_TS to 62MHz (Disabled)
    WREG_B      0xBF206628, 0x01        #//Set CLK_DVBTC_ADC to clk_demod_adcout (Disabled)
    WREG_B      0xBF206634, 0x01        #//Set CLK_VIFDBB_DAC to clk_dmplldiv10 (Disabled)
    WREG_B      0xBF206635, 0x01        #//Set CLK_VIFDBB_VDAC to clk_dmplldiv10 (Disabled)
    WREG_B      0xBF20663C, 0x11        #//Set CLK_DMDMCU to 108MHz (Disabled)

                                        #//MFE
    WREG_B      0xBF206660, 0x09        #reg_ckg_mfe=172MHz,default=GATING
    WREG_B      0xBF206670, 0x05        #reg_ckg_disp_ipath=80MHz(WebCAM),default=GATING

                                        #//G3D
    WREG_B      0xBF206680, 0x01        #reg_ckg_g3d=216MHz,default=GATING
    WREG_B      0xBF206681, 0x01        #reg_ckg_g3d_front=240MHz,default=GATING
    WREG_B      0xBF206684, 0x01        #reg_ckg_g3d_plt=clk gating,default=GATING
    WREG_B      0xBF206685, 0x01        #reg_ckg_g3d_gbl=clk gating,default=GATING

                                        #//CA
    WREG_B      0xBF206690, 0x01        #//Set CLK_NSK to 123MHz (Disabled)
    WREG_B      0xBF206694, 0x11        #//Set CLK_SM_CA to 27MHz (Disabled) / Set CLK_SM_CA2 to 27MHz (Disabled)

    .endm
