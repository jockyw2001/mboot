make clean
make distclean

cp configs/kaiser/.config.kaiser.170A_D01A_S.nand.DDI .config
echo "build kaiser.170A_D01A_S.nand.DDI .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MST170A_D01A_S_1866_NAND.bin

make clean
make distclean

cp configs/kaiser/.config.kaiser.170B_D01A_S.nand.DDI .config
echo "build kaiser.170B_D01A_S.nand.DDI .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MST170B_D01A_S_1600_NAND.bin

make clean
make distclean

cp configs/kaiser/.config.kaiser.187A_D01A.nand.DDI .config
echo "build kaiser.187A_D01A.nand.DDI .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MST187A_D01A_1866_NAND.bin

make clean
make distclean

cp configs/kaiser/.config.kaiser.187A_D01A.emmc.DDI .config
echo "build kaiser.187A_D01A.emmc.DDI .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MST187A_D01A_1866_EMMC.bin

make clean
make distclean

cp configs/kaiser/.config.kaiser.187C_D01A.emmc.DDI .config
echo "build kaiser.187C_D01A.emmc.DDI .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MST187C_D01A_1866_EMMC.bin

make clean
make distclean

cp configs/kaiser/.config.kaiser.187C_D01A_S.emmc.DDI .config
echo "build kaiser.187C_D01A_S.emmc.DDI .."
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MST187C_D01A_S_1866_EMMC.bin


