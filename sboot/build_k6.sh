python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.emmc.32bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.emmc.64bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.emmc.32bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.emmc.64bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.nand.32bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.nand.64bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.nand.32bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.nand.64bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.32bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.1866 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.emmc.32bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.emmc.64bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.nand.32bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.nand.64bit.kernel.DDR3.2133 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.nand.32bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.nand.64bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.emmc.64bit.kernel.DDR3.1866 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.emmc.32bit.kernel.DDR3.1866 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.nand.64bit.kernel.DDR3.1866 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.nand.32bit.kernel.DDR3.1866 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.emmc.32bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.emmc.64bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.nand.32bit.kernel.DDR4.2400 arm
python FixHash2StartAddr.py configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.nand.64bit.kernel.DDR4.2400 arm

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.emmc.32bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257a_d01a_s.emmc.32bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D01A_S_2133_32bit_EMMC_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.emmc.64bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257a_d01a_s.emmc.64bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D01A_S_2133_64bit_EMMC_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.emmc.32bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257a_d02a_s.emmc.32bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D02A_S_2400_32bit_EMMC_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.emmc.64bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257a_d02a_s.emmc.64bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D02A_S_2400_64bit_EMMC_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.nand.32bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257a_d01a_s.nand.32bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D01A_S_2133_32bit_NAND_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d01a_s.nand.64bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257a_d01a_s.nand.64bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D01A_S_2133_64bit_NAND_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.nand.32bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257a_d02a_s.nand.32bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D02A_S_2400_32bit_NAND_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257a_d02a_s.nand.64bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257a_d02a_s.nand.64bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257A_D02A_S_2400_64bit_NAND_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.32bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257b_d01a_s.emmc.32bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D01A_S_2133_32bit_EMMC_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.1866 .config
echo "build .config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.1866 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D01A_S_1866_64bit_EMMC_DDR3.bin

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D01A_S_2133_64bit_EMMC_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.emmc.32bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257b_d02a_s.emmc.32bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D02A_S_2400_32bit_EMMC_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.emmc.64bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257b_d02a_s.emmc.64bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D02A_S_2400_64bit_EMMC_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.nand.32bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257b_d01a_s.nand.32bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D01A_S_2133_32bit_NAND_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.nand.64bit.kernel.DDR3.2133 .config
echo "build .config.k6.ddi.257b_d01a_s.nand.64bit.kernel.DDR3.2133 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D01A_S_2133_64bit_NAND_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.nand.32bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257b_d02a_s.nand.32bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D02A_S_2400_32bit_NAND_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d02a_s.nand.64bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257b_d02a_s.nand.64bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257B_D02A_S_2400_64bit_NAND_DDR4.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.emmc.64bit.kernel.DDR3.1866 .config
echo "build .config.k6.ddi.257c_d01a_s.emmc.64bit.kernel.DDR3.1866 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D01A_S_1866_64bit_EMMC_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.emmc.32bit.kernel.DDR3.1866 .config
echo "build .config.k6.ddi.257c_d01a_s.emmc.32bit.kernel.DDR3.1866 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D01A_S_1866_32bit_EMMC_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.nand.64bit.kernel.DDR3.1866 .config
echo "build .config.k6.ddi.257c_d01a_s.nand.64bit.kernel.DDR3.1866 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D01A_S_1866_64bit_NAND_DDR3.bin

make clean
make distclean

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d01a_s.nand.32bit.kernel.DDR3.1866 .config
echo "build .config.k6.ddi.257c_d01a_s.nand.32bit.kernel.DDR3.1866 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D01A_S_1866_32bit_NAND_DDR3.bin

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.emmc.32bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257c_d02a_s.emmc.32bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D02A_S_2400_32bit_EMMC_DDR4.bin

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.emmc.64bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257c_d02a_s.emmc.64bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D02A_S_2400_64bit_EMMC_DDR4.bin

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.nand.32bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257c_d02a_s.nand.32bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D02A_S_2400_32bit_NAND_DDR4.bin

cp configs/k6/64bit_sboot/.config.k6.ddi.257c_d02a_s.nand.64bit.kernel.DDR4.2400 .config
echo "build .config.k6.ddi.257c_d02a_s.nand.64bit.kernel.DDR4.2400 .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/K6/DDI_Package/ddi_k6_linux/bsp/linux/MST257C_D02A_S_2400_64bit_NAND_DDR4.bin
