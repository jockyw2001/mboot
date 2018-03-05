***burn code steps***
The programming steps are:
1.	To program bfe_app.bin
		i.	  Open mstvtool -> view -> STB eMMC Program
		ii.	  ¡§Prepare Script¡¨ enter the file: 1.xxx_PM51_Prepare_PLL_CLKGEN.txt
		iii.  ¡§DRAM Script¡¨ enter the file: 2.MIU01_DDR3_16_8X_CL14_2133.txt
		iv.	  ¡§Tool Bin file¡¨ enter the file: PM51_xxx_User.bin
		v.	  ¡§Trigger Script¡¨ enter the file: 10.xxx_PM51_Trigger.txt
		vi.	  ¡§Program -> File¡¨ enter the file: bfe_app.bin
		vii.	Uncheck the ¡§Program -> Boot¡¨ and enter 0x1000 in ¡§Start blk addr¡¨
		viii.	Confirm mstvtool can read reg banks
		ix.	  Click ¡§Start Prepare¡¨
		x.	  Click ¡§Start Program¡¨
		xi.	  After programing done, the status will show ¡§Success, Please reset power¡¨.

2.	To program bfe_boot.bin
		i.	  Open mstvtool -> view -> STB eMMC Program
		ii.	  ¡§Prepare Script¡¨ enter the file: 1.xxx_PM51_Prepare.txt
		iii.  ¡§DRAM Script¡¨ enter the file: 2.xxx_PLL_CLKGEN03_MIU01_DDR3_16_8X_CL14_2133.txt
		iv.   ¡§Tool Bin file¡¨ enter the file: PM51_xxx_Boot.bin
		v.	  ¡§Trigger Script¡¨ enter the file: 10.xxx_PM51_Trigger.txt
		vi.	  ¡§Program -> File¡¨ enter the file: bfe_boot.bin
		vii.  Check the ¡§Program -> Boot¡¨
		viii. Confirm mstvtool can read reg banks
		ix.	  Click ¡§Start Prepare¡¨
		x.	  Click ¡§Start Program¡¨
		xi.	  After programing done, the status will show ¡§Success, Please reset power¡¨.

3. (Optional if it isn't exist LDR in OPT or SPI)
	  Using ISP_Tool to program flash.bin that will bring APP from NAND flash.

***note***
# bfe_app.bin, bfe_boot.bin, flash.bin are build from
	62a07378081a025029683dfe0aaf4921c7b39da9 @ mstar/mboot/MstarApp
	5cf381f5d3e1c42ed671bd15b694501a955bdb10 @ mstar/mboot/MstarCore
	27b09b848f5c7caa5174a13a97eda092e50b87b4 @ mstar/mboot/MstarCustomer
	fee6a9ac1d6b10e0b67eee52ad407b88456602f0 @ mstar/stb_sboot
	9e651f136d58bcdbc0ee92d3a0fe680b1bddb8b5 @ mstar/STB_Mboot
	db483af55d4cfa5add1449b94990844f53c6f166 @ mstar/mboot/u-boot-2011.06
	a6b9fb971f3d86c6525857a45a3e52697dedd87e @ mstar/mboot/pm-3.0/zenonia

  config: mstar/stb_sboot/configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.BFE

# The latest miu script should be downloaded from mstar/stb_sboot/chip/{CHIP}/miu_script and replace 2.MIU_DDRxxxxx.txt

# MStar TV System Tool v4.159
	Mstar ISP Utility    V4.9.4