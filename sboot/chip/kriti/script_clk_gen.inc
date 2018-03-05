
# turn on MainPLL, MIPSLL
# swch 4

WREG_B (REG_ADDR_BASE + (0x10334a << 1)), 0x10 			//clk_sm_ca, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100b02 << 1)), 0x07			//clk_bist, default 16'h000f, off 16'h000f

WREG_B (REG_ADDR_BASE + (0x100bfc << 1)), 0x01			//clk_miu_ddr_syn = 432 MHz

WREG_B (REG_ADDR_BASE + (0x100b3c << 1)+1), 0x01	        //clk_mpll_syn = 432 MHz

WREG_B (REG_ADDR_BASE + (0x100b3e << 1)), 0x00			//clk_miu, default 16'h0000, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100b26 << 1)+1), 0x00			//clk_uart0, default 16'h0100, off 16'h0100

WREG (REG_ADDR_BASE + (0x100b28 << 1)), 0x0000			//clk_uart1, default 16'h0001, off 16'h0001
																										//clk_uart2, default 16'h0100, off 16'h0100																										
WREG_B (REG_ADDR_BASE + (0x100b3e << 1)+1), 0x00			//clk_miu_rec, default 16'h0000, off 16'h0100

WREG (REG_ADDR_BASE + (0x100b50 << 1)), 0x0000			//clk_ts0, default 16'h0001, off 16'h0001
																										//clk_ts1, default 16'h0100, off 16'h0100
WREG_B (REG_ADDR_BASE + (0x100b52 << 1)), 0x00			//clk_ts2, default 16'h0001, off 16'h0001

WREG (REG_ADDR_BASE + (0x100b54 << 1)), 0x001c			//clk_tsp, default 16'h001c, off 16'h001d
																										//clk_stc0, default 16'h0000, off 16'h0100
WREG_B (REG_ADDR_BASE + (0x100b72 << 1)+1), 0x00			//clk_mvd, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100b98 << 1)), 0x00			//clk_dc0, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100b90 << 1)), 0x00			//clk_ge, default 16'h0001, off 16'h0001

WREG (REG_ADDR_BASE + (0x100b80 << 1)), 0x0000			//clk_gop0, default 16'h0001, off 16'h0001
																										//clk_gop1, default 16'h0100, off 16'h0100
WREG (REG_ADDR_BASE + (0x100b84 << 1)), 0x0000			//clk_gop2, default 16'h0001, off 16'h0001
																										//clk_gop3, default 16'h0100, off 16'h0100
WREG (REG_ADDR_BASE + (0x100b82 << 1)), 0x0000			//clk_gopmix, default 16'h0001, off 16'h0001
																									//clk_gopd, default 16'h0100, off 16'h0100																																																			
WREG_B (REG_ADDR_BASE + (0x100b86 << 1)), 0x00			//clk_psram0, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100baa << 1)), 0x20			//clk_idclk2, default 16'h0021, off 16'h0021

WREG_B (REG_ADDR_BASE + (0x100ba4 << 1)+1), 0x00			//clk_fclk, default 16'h0100, off 16'h0100

WREG_B (REG_ADDR_BASE + (0x100ba2 << 1)+1), 0x00			//clk_ficlk_f2, default 16'h0100, off 16'h0100

WREG_B (REG_ADDR_BASE + (0x100ba6 << 1)), 0x20			//clk_odclk, default 16'h0020, off 16'h0021

WREG (REG_ADDR_BASE + (0x100bb0 << 1)), 0x0000			//clk_hdgen, default 16'h0001, off 16'h0001
																										//clk_hdgen_fir, default 16'h0100, off 16'h0100
WREG (REG_ADDR_BASE + (0x100bb2 << 1)), 0x0000			//clk_dac1, default 16'h0001, off 16'h0001
																										//clk_dac2, default 16'h0100, off 16'h0100																																															
WREG_B (REG_ADDR_BASE + (0x100bb4 << 1)+1), 0x00			//clk_dcs27, default 16'h0100, off 16'h0100

WREG_B (REG_ADDR_BASE + (0x103350 << 1)), 0x00			//clk_hdmi_tx, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100b34 << 1)), 0x00			//clk_pcm, default 16'h0001, off 16'h0001																										

WREG_B (REG_ADDR_BASE + (0x100b4a << 1)), 0x0c			//clk_ve_in, default 16'h000d, off 16'h000d

WREG (REG_ADDR_BASE + (0x100b48 << 1)), 0x0000			//clk_ve, default 16'h0001, off 16'h0001	
																										//clk_vedac, default 16'h0100, off 16'h0100																								
WREG_B (REG_ADDR_BASE + (0x100b34 << 1)+1), 0x00			//clk_tck, default 16'h0000, off 16'h0100

WREG_B (REG_ADDR_BASE + (0x100b24 << 1)), 0x00			//clk_aeon(do not use), default 16'h0000, off 16'h0081

WREG_B (REG_ADDR_BASE + (0x100b6a << 1)), 0x00			//clk_jpd, default 16'h0001, off 16'h0001	

WREG_B (REG_ADDR_BASE + (0x100b62 << 1)), 0x00			//clk_hvd, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100bc8 << 1)), 0x00			//clk_nfie, default 16'h0001, off 16'h0001	

WREG_B (REG_ADDR_BASE + (0x100b60 << 1)), 0x00			//clk_vdmheg5, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x103308 << 1)), 0x00			//clk_dvbtc_ts, default 16'h0001, off 16'h0001

WREG (REG_ADDR_BASE + (0x103314 << 1)), 0x0000			//clk_dvbtc_adc, default 16'h0001, off 16'h0001	
																										//clk_dvbtc_adc2, default 16'h1100, off 16'h1100
WREG_B (REG_ADDR_BASE + (0x10331e << 1)), 0x00			//clk_dmdmcu, default 16'h001c, off 16'h001d

WREG_B (REG_ADDR_BASE + (0x100bd0 << 1)), 0x00			//clk_mpif, default 16'h0001, off 16'h0001	

WREG_B (REG_ADDR_BASE + (0x103348 << 1)), 0x00			//clk_nsk, default 16'h000c, off 16'h000d

WREG_B (REG_ADDR_BASE + (0x100b2e << 1)), 0x00			//clk_fuart, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x103360 << 1)), 0x00			//clk_miic, default 16'h0000, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100bf8 << 1)), 0x00			//clk_spi_m, default 16'h0001, off 16'h0021

WREG (REG_ADDR_BASE + (0x103360 << 1)), 0x0000			//clk_miic1, default 16'h0000, off 16'h0010
																										//clk_miic2, default 16'h0000, off 16'h0100																											
WREG_B (REG_ADDR_BASE + (0x100b2a << 1)), 0x00			//clk_uart3, default 16'h0001, off 16'h0001

WREG_B (REG_ADDR_BASE + (0x100b5e << 1)+1), 0x04			//clk_bdma, change to clk miu
