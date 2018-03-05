make clean
make distclean

cp configs/kriti/.config.204A_D01A.CPU576_DDR2_1333.DDI .config
echo "build .config.204A_D01A.CPU576_DDR2_1333.DDI .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kriti/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kriti_Mbootbin/mboot_BD_MST204A_D01A_CPU576_DDR2_1333.bin

make clean
make distclean

cp configs/kriti/.config.204A_D01A.CPU624_DDR2_1333.DDI .config
echo "build .config.204A_D01A.CPU624_DDR2_1333.DDI .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kriti/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kriti_Mbootbin/mboot_BD_MST204A_D01A_CPU624_DDR2_1333.bin

make clean
make distclean

cp configs/kriti/.config.204A_D01A.CPU576_DDR3_1600.DDI .config
echo "build .config.204A_D01A.CPU576_DDR3_1600.DDI .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kriti/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kriti_Mbootbin/mboot_BD_MST204A_D01A_CPU576_DDR3_1600.bin

make clean
make distclean

cp configs/kriti/.config.204A_D01A.CPU624_DDR3_1600.DDI .config
echo "build .config.204A_D01A.CPU624_DDR3_1600.DDI .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kriti/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kriti_Mbootbin/mboot_BD_MST204A_D01A_CPU624_DDR3_1600.bin

