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
  87aa6c8ecf7436ea532510549f4d37d637b77085 @ mstar/mboot/MstarApp
  5cf381f5d3e1c42ed671bd15b694501a955bdb10 @ mstar/mboot/MstarCore
  b920bbe68555cf53d583a72bb33e4f0b73c48c11 @ mstar/mboot/MstarCustomer
  477d0e781eeed7b30a6f8422c75a86aa21cb00fa @ mstar/stb_sboot
  d12fcb7fe2b9cd9fe5c3667451c6754aa87bff2e @ mstar/STB_Mboot
  e9ec27e89c024ed6af11534b5e10765f2b6e3aa5 @ mstar/mboot/u-boot-2011.06
  1c17d4a3cfa9079e1f95ac971cbcf256c3796e17 @ mstar/mboot/pm-3.0/zenonia

  config: mstar/stb_sboot/configs/k6lite/.config.k6lite.ddi.268a_d01a_s.emmc.DDR3.1866.BFE

# The latest miu script should be downloaded from mstar/stb_sboot/chip/{CHIP}/miu_script and replace 2.MIU_DDRxxxxx.txt

# MStar TV System Tool v4.160
  Mstar ISP Utility    V4.9.5
