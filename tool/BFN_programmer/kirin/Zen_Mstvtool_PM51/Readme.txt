***Version change list***
- 20150629 : Mboot mtd partition lay to the head of LDR image
- 20150804 : Update 3.PM51_Keres.bin from //DAILEO/PM/mainline/Zen_ISP/...@1338403
             Fix programming failed whne ACPU is stopped and reading nand

***burn code steps***
The programming steps are:
1.	MSTV -> Help -> Product Key -> Enter Key "prognand"
2.	MSTV -> View -> NAND Program (8051) -> STB
	And set the column to following files.
	Prepare script			-> 1.xxx_PM51_Prepare_PLL_CLKGEN.txt
	DRAM script					-> 2.xxx_MIU_DDRxxxxx.txt
	Tool bin File				-> 3.PM51_xxxxx.bin
	NANDINFO Bin File		-> 4.NANDINFO.nni
	PAIRPAGEMAP Bin File-> 5.PAIRPAGEMAP_v2.ppm
	PARTITION File			-> 6.PARTITION_v2_default.pni
	bfn_miu File				-> 7.bfn_miu.bin
	bfn_loader File			-> 8.bfn_loader.bin
	bfn_app File				-> 9.bfn_app.bin
	Trigger Script 			-> 10.xxx_PM51_Trigger.txt
	Delay								-> 40000
3.	Open the uart console connected with PCB, and power on the PCB.
	Do relate steps in one of these two conditions.
	i.	If it’s the first time programming or NAND has no boot image =>
		After the “BO? FAIL” log is shown in the console (may need about 10s to print “FAIL”), do following steps quickly. (Note that in some security chip, it maybe has no such log. Instead, it may use debug reg to show it is in “failed” condition, which means at this time the mstvtool can connect to chips and stop ACPU by clicking “Prepare Programming” in following steps.)
		A.	Disconnect the uart console
		B.	In the mstvtool page “NAND Program (8051) -> STB” ,click "Prepare Programming"
	ii.	If the PCB already can boot to uboot or other applications, make sure to disable the uart =>
		A.	If booting to uboot, enter “du” to disable uart.
		B.	If booting to DDI, enter “00112233” to disable uart.
		C.	Disconnect the uart console
		D.	In the mstvtool page “NAND Program (8051) -> STB” ,click "Prepare Programming"
4.	After finish "Prepare Programming", the mstvtool page's status will show "All BFN files are done,Please click Start Programming to 	    resume"
5.	(optional if wanting to view programming log)
	Open & connect uart console which is ready for programming messages.
6.	Click "Start Programming"
	The programming log will be shown in uart console if it is opened and connected
	When the programming is finished, the mstvtool page's status will show "Success. Please reset power"

***note***
# PM51_Kirin.bin is build from //DAILEO/PM/mainline/Zen_ISP/...@1338403 (PM51_Keres)
# mstvtool version:V4.149
# Details can reference the doc:\\PERFORCE\DAILEO\MBoot_Branch\MBoot_STB\tool\doc\STB_System_Boot_Loader_Manual.doc
