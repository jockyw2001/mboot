make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.170A_D01A_S.nand.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.170A_D01A_S.nand.DDI ./scripts/defconfig
echo "build kaiser.170A_D01A_S.nand.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST170A_D01A_S_1866_NAND_Bootlogo.bin
#############################################################################################################

make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.170B_D01A_S.nand.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.170B_D01A_S.nand.DDI ./scripts/defconfig
echo "build kaiser.170B_D01A_S.nand.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST170B_D01A_S_1600_NAND_Bootlogo.bin
#############################################################################################################

make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.187A_D01A.nand.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.187A_D01A.nand.DDI ./scripts/defconfig
echo "build kaiser.187A_D01A.nand.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST187A_D01A_1866_NAND_Bootlogo.bin
#############################################################################################################

make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.187A_D01A.emmc.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.187A_D01A.emmc.DDI ./scripts/defconfig
echo "build kaiser.187A_D01A.emmc.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST187A_D01A_1866_EMMC_Bootlogo.bin
#############################################################################################################

make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.187A_D01A.emmc.tee.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.187A_D01A.emmc.tee.DDI ./scripts/defconfig
echo "build kaiser.187A_D01A.emmc.tee.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST187A_D01A_1866_EMMC_TEE_Bootlogo.bin
#############################################################################################################

make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.187C_D01A.emmc.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.187C_D01A.emmc.DDI ./scripts/defconfig
echo "build kaiser.187C_D01A.emmc.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST187C_D01A_1866_EMMC_Bootlogo.bin
#############################################################################################################

make clean
make distclean

cp configs/kaiser/bootlogo/.config.kaiser.187C_D01A_S.emmc.DDI .config
cp configs/kaiser/bootlogo/.config.kaiser.187C_D01A_S.emmc.DDI ./scripts/defconfig
echo "build kaiser.187C_D01A_S.emmc.DDI .."
make defconfig 
make clean;make
cp bin/mboot.bin ../../../../THEALE/Uranus/Kaiser/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/MST187C_D01A_S_1866_EMMC_Bootlogo.bin
#############################################################################################################
