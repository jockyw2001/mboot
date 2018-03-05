python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Samsung arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Micron arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X856.AXX.Nanya arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.AXX.Samsung arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.AXX.Micron arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X856.AXX.Nanya arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d01a.nand.DDR3.1866.MSA5X83 arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d01a_s.nand.DDR3.1866.MSA5X83 arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d02a.nand.DDR4.2133.MSA5X83 arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d02a_s.nand.DDR4.2133.MSA5X83 arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268c_d01a_s.nand.DDR3.1866.MSR620Q.Samsung arm
python FixHash2StartAddr.py configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.emmc.DDR3.1866.MSA5X859.AXX.Samsung arm

# basic MBoot
make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Samsung .config
echo "build k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Samsung .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_1866_NAND_DDR3_MSA5X859_AXX_Samsung.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Micron .config
echo "build k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Micron .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_1866_NAND_DDR3_MSA5X859_AXX_Micron.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron .config
echo "build k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_1866_NAND_DDR3_MSA5X859_MIX_Samsung_Micron.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X856.AXX.Nanya .config
echo "build k6lite.ddi.268a_d01a.nand.DDR3.1866.MSA5X856.AXX.Nanya .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_1866_NAND_DDR3_MSA5X856_AXX_Nanya.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.AXX.Samsung .config
echo "build k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.AXX.Samsung .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_S_1866_NAND_DDR3_MSA5X859_AXX_Samsung.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.AXX.Micron .config
echo "build k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.AXX.Micron .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_S_1866_NAND_DDR3_MSA5X859_AXX_Micron.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron .config
echo "build k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_S_1866_NAND_DDR3_MSA5X859_MIX_Samsung_Micron.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X856.AXX.Nanya .config
echo "build k6lite.ddi.268a_d01a_s.nand.DDR3.1866.MSA5X856.AXX.Nanya .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_S_1866_NAND_DDR3_MSA5X856_AXX_Nanya.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d01a.nand.DDR3.1866.MSA5X83 .config
echo "build k6lite.ddi.268b_d01a.nand.DDR3.1866.MSA5X83 .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268B_D01A_1866_NAND_DDR3_MSA5X83.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d01a_s.nand.DDR3.1866.MSA5X83 .config
echo "build k6lite.ddi.268b_d01a_s.nand.DDR3.1866.MSA5X83 .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268B_D01A_S_1866_NAND_DDR3_MSA5X83.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d02a.nand.DDR4.2133.MSA5X83 .config
echo "build k6lite.ddi.268b_d02a.nand.DDR4.2133.MSA5X83 .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268B_D02A_2133_NAND_DDR4_MSA5X83.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268b_d02a_s.nand.DDR4.2133.MSA5X83 .config
echo "build k6lite.ddi.268b_d02a_s.nand.DDR4.2133.MSA5X83 .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268B_D02A_S_2133_NAND_DDR4_MSA5X83.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268c_d01a_s.nand.DDR3.1866.MSR620Q.Samsung .config
echo "build k6lite.ddi.268c_d01a_s.nand.DDR3.1866.MSR620Q.Samsung .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268C_D01A_S_1866_NAND_DDR3_MSR620Q_Samsung.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/basic/.config.k6lite.ddi.268a_d01a_s.emmc.DDR3.1866.MSA5X859.AXX.Samsung .config
echo "build .config.k6lite.ddi.268a_d01a_s.emmc.DDR3.1866.MSA5X859.AXX.Samsung .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/basic/MST268A_D01A_S_1866_EMMC_DDR3_MSA5X859.AXX.Samsung.bin


make clean
make distclean

cp configs/k6lite/ddi_linux/bootlogo/.config.k6lite.mi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Micron.bootlogo .config
echo "build k6lite.mi.268a_d01a.nand.DDR3.1866.MSA5X859.AXX.Micron.bootlogo .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/bootlogo/MST268A_D01A_1866_NAND_DDR3_MSA5X859_AXX_Micron_bootlogo.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/bootlogo/.config.k6lite.mi.268a_d01a.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron.bootlogo .config
echo "build k6lite.mi.268a_d01a.nand.DDR3.1866.MSA5X859.MIX.Samsung.Micron.bootlogo .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/bootlogo/MST268A_D01A_1866_NAND_DDR3_MSA5X859_MIX_Samsung_Micron_bootlogo.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/bootlogo/.config.k6lite.mi.268a_d01a.nand.DDR3.1866.MSA5X856.AXX.Nanya.bootlogo .config
echo "build k6lite.mi.268a_d01a.nand.DDR3.1866.MSA5X856.AXX.Nanya.bootlogo .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/bootlogo/MST268A_D01A_1866_NAND_DDR3_MSA5X856_AXX_Nanya_bootlogo.bin

make clean
make distclean

cp configs/k6lite/ddi_linux/bootlogo/.config.k6lite.mi.268b_d01a.nand.DDR3.1866.MSA5X83.bootlogo .config
echo "build k6lite.mi.268b_d01a.nand.DDR3.1866.MSA5X83.bootlogo .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6Lite/DDI_Package/ddi_k6lite_linux/bsp/linux/MBoot/bootlogo/MST268B_D01A_1866_NAND_DDR3_MSA5X83_bootlogo.bin

make clean
make distclean
