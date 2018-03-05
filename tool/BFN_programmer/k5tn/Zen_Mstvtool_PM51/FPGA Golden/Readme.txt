***burn code steps***
The programming steps are:
1.	MSTV -> Help -> Product Key -> Enter Key "prognand"
2.	MSTV -> View -> NAND Program (PM51) -> STB
	And set the column to following files.
	Prepare script		   -> 1.xxx_PM51_Prepare.txt
	DRAM script			     -> 2.xxx_PLL_CLKGEN_MIU_DDRxxxxx.txt
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

7. (Optional if it isn't exist LDR in OTP or SPI)
	Using ISP_Tool to program flash.bin that will bring APP from NAND flash.

***note***
# bfn_app.bin, bfn_loader.bin, bfn_miu.bin, flash.bin are build from

	stb_mboot/MstarApp/d1d167acff1c7667f94e2eb751355f21b4bc28ea
	stb_mboot/MstarCore/f0200729e20692ad65a1fe3260aeb1999941856c
	stb_mboot/MstarCustomer/9a5dcc0f861531b03ea664b119490df4e4ce2e78
	stb_mboot/sboot/23135d084845d25c56b215746495c373582150a6
	stb_mboot/tool/d12fcb7fe2b9cd9fe5c3667451c6754aa87bff2e
	stb_mboot/u-boot-2011.06/d03a0897b59b0780cfb51a81c2ea9159a9f8a071

  config:mstar/stb_sboot/configs/k5tn/config.fpga.BFN

# MStar TV System Tool v4.155.258213
	Mstar ISP Utility V4.9.1