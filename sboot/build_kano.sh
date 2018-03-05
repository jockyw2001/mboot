python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231a_d01a_s.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231a_d01a_s.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01a.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01a.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01a_s.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01a_s.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d02a.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01b.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01b.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01b_s.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d01b_s.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d04a.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231b_d04a.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231c_d01a_s.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231c_d01a_s.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231d_d02a_s.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231d_d02a_s.nand.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/.config.kano.ddi.231e_d01a.emmc.32bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231a_d01a_s.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231a_d01a_s.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a_s.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a_s.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d02a.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b_s.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b_s.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d04a.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d04a.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231c_d01a_s.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231c_d01a_s.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231d_d02a_s.emmc.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231d_d02a_s.nand.64bit.kernel arm
python FixHash2StartAddr.py configs/kano/DDI_64bit_kernel/.config.kano.ddi.231e_d01a.emmc.64bit.kernel arm

make clean
make distclean

cp configs/kano/.config.kano.ddi.231a_d01a_s.emmc.32bit.kernel .config
echo "build kano.ddi.231a_d01a_s.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231A_D01A_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231a_d01a_s.nand.32bit.kernel .config
echo "build kano.ddi.231a_d01a_s.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231A_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01a.emmc.32bit.kernel .config
echo "build kano.ddi.231b_d01a.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01A_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01a.nand.32bit.kernel .config
echo "build kano.ddi.231b_d01a.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01A_1866_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01a_s.emmc.32bit.kernel .config
echo "build kano.ddi.231b_d01a_s.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01A_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01a_s.nand.32bit.kernel .config
echo "build kano.ddi.231b_d01a_s.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d02a.emmc.32bit.kernel .config
echo "build kano.ddi.231b_d02a.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D02A_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01b.emmc.32bit.kernel .config
echo "build kano.ddi.231b_d01b.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01B_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01b.nand.32bit.kernel .config
echo "build kano.ddi.231b_d01b.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01B_1866_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01b_s.emmc.32bit.kernel .config
echo "build kano.ddi.231b_d01b_s.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01B_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d01b_s.nand.32bit.kernel .config
echo "build kano.ddi.231b_d01b_s.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D01B_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d04a.emmc.32bit.kernel .config
echo "build kano.ddi.231b_d04a.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D04A_DDR4_2400_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231b_d04a.nand.32bit.kernel .config
echo "build kano.ddi.231b_d04a.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231B_D04A_DDR4_2400_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231c_d01a_s.emmc.32bit.kernel .config
echo "build kano.ddi.231c_d01a_s.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231C_D01A_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231c_d01a_s.nand.32bit.kernel .config
echo "build kano.ddi.231c_d01a_s.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231C_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231d_d02a_s.emmc.32bit.kernel .config
echo "build kano.ddi.231d_d02a_s.emmc.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231D_D02A_S_DDR4_2400_EMMC.bin

make clean
make distclean

cp configs/kano/.config.kano.ddi.231d_d02a_s.nand.32bit.kernel .config
echo "build kano.ddi.231d_d02a_s.nand.32bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231D_D02A_S_DDR4_2400_NAND.bin

make clean
make distclean

cp -vf configs/kano/.config.kano.ddi.231e_d01a.emmc.32bit.kernel .config
echo "build kano.ddi.231e_d01a.emmc.32bit.kernel .."
make oldconfig
make clean;make
cp -vf bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MST231E_D01A_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231a_d01a_s.emmc.64bit.kernel .config
echo "build kano.ddi.231a_d01a_s.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231A_D01A_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231a_d01a_s.nand.64bit.kernel .config
echo "build kano.ddi.231a_d01a_s.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231A_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a.emmc.64bit.kernel .config
echo "build kano.ddi.231b_d01a.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01A_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a.nand.64bit.kernel .config
echo "build kano.ddi.231b_d01a.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01A_1866_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a_s.emmc.64bit.kernel .config
echo "build kano.ddi.231b_d01a_s.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01A_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01a_s.nand.64bit.kernel .config
echo "build kano.ddi.231b_d01a_s.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d02a.emmc.64bit.kernel .config
echo "build kano.ddi.231b_d02a.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D02A_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b.emmc.64bit.kernel .config
echo "build kano.ddi.231b_d01b.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01B_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b.nand.64bit.kernel .config
echo "build kano.ddi.231b_d01b.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01B_1866_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b_s.emmc.64bit.kernel .config
echo "build kano.ddi.231b_d01b_s.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01B_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d01b_s.nand.64bit.kernel .config
echo "build kano.ddi.231b_d01b_s.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D01B_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d04a.emmc.64bit.kernel .config
echo "build kano.ddi.231b_d04a.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D04A_DDR4_2400_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231b_d04a.nand.64bit.kernel .config
echo "build kano.ddi.231b_d04a.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231B_D04A_DDR4_2400_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231c_d01a_s.emmc.64bit.kernel .config
echo "build kano.ddi.231c_d01a_s.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231C_D01A_S_1866_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231c_d01a_s.nand.64bit.kernel .config
echo "build kano.ddi.231c_d01a_s.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231C_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231d_d02a_s.emmc.64bit.kernel .config
echo "build kano.ddi.231d_d02a_s.emmc.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231D_D02A_S_DDR4_2400_EMMC.bin

make clean
make distclean

cp configs/kano/DDI_64bit_kernel/.config.kano.ddi.231d_d02a_s.nand.64bit.kernel .config
echo "build kano.ddi.231d_d02a_s.nand.64bit.kernel .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231D_D02A_S_DDR4_2400_NAND.bin

make clean
make distclean

cp -vf configs/kano/DDI_64bit_kernel/.config.kano.ddi.231e_d01a.emmc.64bit.kernel .config
echo "build kano.ddi.231e_d01a.emmc.64bit.kernel .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kano/DDI_Package/ddi_kano_linux/bsp/linux/MBoot_64bit_kernel/MST231E_D01A_1866_EMMC.bin
