cp configs/keltic/.config.keltic_183A_D01A_S_5XX1.DDI .config
echo "build keltic_183A_D01A_S_5XX1.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183A_D01A_S_MSD5XX1.bin
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183A_D01A_MSD5XX1.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183A_D01A_S_5XX3.DDI .config
echo "build keltic_183A_D01A_S_5XX3.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183A_D01A_S_MSD5XX3.bin
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183A_D01A_MSD5XX3.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183A_D01A_S_5XX5.DDI .config
echo "build keltic_183A_D01A_S_5XX5.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183A_D01A_S_MSD5XX5.bin
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183A_D01A_MSD5XX5.bin


make clean
make distclean

cp configs/keltic/.config.keltic_183B_D01A_S_5XX7.DDI .config
echo "build keltic_183A_D01A_S_5XX7.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183B_D01A_S_MSD5XX7.bin
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183B_D01A_MSD5XX7.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183B_D01A_S_5XX9.DDI .config
echo "build keltic_183A_D01A_S_5XX9.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183B_D01A_S_MSD5XX9.bin
cp bin/flash_with_uboot.bin bin/eCos_mboot_BD_MST183B_D01A_MSD5XX9.bin


make clean
make distclean

cp configs/keltic/.config.keltic_183A_D01A_S_5XX1.DDI.linux .config
echo "build keltic_183A_D01A_S_5XX1.DDI.linux .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183A_D01A_S_MSD5XX1.bin
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183A_D01A_MSD5XX1.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183A_D01A_S_5XX3.DDI.linux .config
echo "build keltic_183A_D01A_S_5XX3.DDI.linux .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183A_D01A_S_MSD5XX3.bin
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183A_D01A_MSD5XX3.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183A_D01A_S_5XX5.DDI.linux .config
echo "build keltic_183A_D01A_S_5XX5.DDI.linux .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/linuix_mboot_BD_MST183A_D01A_S_MSD5XX5.bin
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183A_D01A_MSD5XX5.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183B_D01A_S_5XX7.DDI.linux .config
echo "build keltic_183A_D01A_S_5XX7.DDI.linux .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183B_D01A_S_MSD5XX7.bin
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183B_D01A_MSD5XX7.bin

make clean
make distclean

cp configs/keltic/.config.keltic_183B_D01A_S_5XX9.DDI.linux .config
echo "build keltic_183A_D01A_S_5XX9.DDI.linux .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183B_D01A_S_MSD5XX9.bin
cp bin/flash_with_uboot.bin bin/linux_mboot_BD_MST183B_D01A_MSD5XX9.bin

make clean
make distclean
