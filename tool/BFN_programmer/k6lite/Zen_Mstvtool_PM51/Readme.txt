***burn code steps***
The programming steps are:
1.	MSTV -> Help -> Product Key -> Enter Key "prognand"
2.	MSTV -> View -> NAND Program (PM51) -> STB
	And set the column to following files.
	Prepare script		   -> 1.xxx_PM51_Prepare_PLL_CLKGEN.txt
	DRAM script			     -> 2.MIU_DDRxxxxx.txt
	Tool bin File		     -> 3.PM51_xxxxx.bin
	NANDINFO Bin File	   -> 4.NANDINFO.nni
	PAIRPAGEMAP Bin File -> 5.PAIRPAGEMAP_v2.ppm
	PARTITION File		   -> 6.PARTITION_v2_default.pni
	bfn_miu File		     -> 7.bfn_miu.bin
	bfn_loader File		   -> 8.bfn_loader.bin
	bfn_app File		     -> 9.bfn_app.bin
	Trigger Script 		   -> 10.xxx_PM51_Trigger.txt
	Delay				         -> 40000
3.	Open the uart console connected with PCB, and power on the PCB.
	Do relate steps in one of these two conditions.
	i.	If it’s the first time programming or NAND has no boot image =>
		After the “BO? FAIL” log is shown in the console (may need about 10s to print “FAIL”), do following steps quickly. (Note that in some security chip, it maybe has no such log. Instead, it may use debug reg to show it is in “failed” condition, which means at this time the mstvtool can connect to chips and stop ACPU by clicking “Prepare Programming” in following steps.)
		A.	Disconnect the uart console
		B.	In the mstvtool page “NAND Program (PM51) -> STB” ,click "Prepare Programming"
	ii.	If the PCB already can boot to uboot or other applications, make sure to disable the uart =>
		A.	If booting to uboot, enter “du” to disable uart.
		B.	If booting to DDI, enter “00112233” to disable uart.
		C.	Disconnect the uart console
		D.	In the mstvtool page “NAND Program (PM51) -> STB” ,click "Prepare Programming"
4.	After finish "Prepare Programming", the mstvtool page's status will show "All BFN files are done,Please click Start Programming to resume"
5.	(optional if wanting to view programming log)
	Open & connect uart console which is ready for programming messages.
6.	Click "Start Programming"
	The programming log will be shown in uart console if it is opened and connected
	When the programming is finished, the mstvtool page's status will show "Success. Please reset power"

7. (Optional if it isn't exist LDR in OPT or SPI)
	Using ISP_Tool to program flash.bin that will bring APP from NAND flash.

***note***
# bfn_app.bin, bfn_loader.bin, bfn_miu.bin, flash.bin are build from
	62a07378081a025029683dfe0aaf4921c7b39da9 @ mstar/mboot/MstarApp
	5cf381f5d3e1c42ed671bd15b694501a955bdb10 @ mstar/mboot/MstarCore
	27b09b848f5c7caa5174a13a97eda092e50b87b4 @ mstar/mboot/MstarCustomer
	fee6a9ac1d6b10e0b67eee52ad407b88456602f0 @ mstar/stb_sboot
	a8a52db7e0cb33ad70d70516bbf3dd7032f62443 @ mstar/STB_Mboot
	db483af55d4cfa5add1449b94990844f53c6f166 @ mstar/mboot/u-boot-2011.06

  config: mstar/stb_sboot/configs/k6lite/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.BFN

# The latest miu script should be downloaded from mstar/stb_sboot/chip/{CHIP}/miu_script and replace 2.MIU_DDRxxxxx.txt

# MStar TV System Tool v4.160
  Mstar ISP Utility V4.9.5
