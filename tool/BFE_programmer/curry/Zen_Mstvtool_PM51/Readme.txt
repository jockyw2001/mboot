***burn code steps***
The programming steps are:
1.	To program bfe_app.bin
		i.	  Open mstvtool -> view -> STB eMMC Program
		ii.	  ¡§Prepare Script¡¨ enter the file: 1.xxx_PM51_Prepare_PLL_CLKGEN.txt
		iii.  ¡§DRAM Script¡¨ enter the file: 2.xxx_MIU01_DDR3_16_8X_CL14_2133.txt
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
	cf76073670f5014ef6aded7c8ea925af697ca4f9 @ mstar/mboot/MstarApp
	9330ff69a84f53b78948b55b7884679b259c7bbb @ mstar/mboot/MstarCore
	6d66d2ad90e382cc881032bb121a3fd672bd09ce @ mstar/mboot/MstarCustomer
	b17763a004bb4f85dc4535ea6c612c35543e4557 @ mstar/stb_sboot
	6788936193abbb89a8116b8ade39c89d197e4108 @ mstar/STB_Mboot
	2b92ec7000f4f4a5f525ec6774f765e4377dd7d2 @ mstar/mboot/u-boot-2011.06


  config: mstar/stb_sboot/configs/curry/BFE/.config.curry.ddi.245b_d01a_s.emmc.32bit.kernel.BFE

# MStar TV System Tool v4.159
	Mstar ISP Utility    V4.9.4