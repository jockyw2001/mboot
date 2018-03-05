make clean
make distclean

cp configs/HB_kriti/1333MHZ/mboot_BD_MST204A_D01A_64M_DDR1333.config .config
echo "build mboot_BD_MST204A_D01A_64M_DDR1333"
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT/HF/Normal/1333MHZ/mboot_BD_MST204A_D01A_64M.bin
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT2/HF/Normal/1333MHZ/mboot_BD_MST204A_D01A_64M.bin

make clean
make distclean

cp configs/HB_kriti/1333MHZ/mboot_BD_MST204A_D01A_S_64M_DDR1333.config .config
echo "build mboot_BD_MST204A_D01A_S_64M_DDR1333"
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT/HF/Normal/1333MHZ/mboot_BD_MST204A_D01A_S_64M.bin
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT2/HF/Normal/1333MHZ/mboot_BD_MST204A_D01A_S_64M.bin

make clean
make distclean

cp configs/HB_kriti/1600MHZ/mboot_BD_MST204A_D01A_128M_DDR1600.config .config
echo "build mboot_BD_MST204A_D01A_128M_DDR1600"
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT/HF/Normal/1600MHZ/mboot_BD_MST204A_D01A_128M.bin
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT2/HF/Normal/1600MHZ/mboot_BD_MST204A_D01A_128M.bin

make clean
make distclean

cp configs/HB_kriti/1600MHZ/mboot_BD_MST204A_D01A_S_128M_DDR1600.config .config
echo "build mboot_BD_MST204A_D01A_S_128M_DDR1600"
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT/HF/Normal/1600MHZ/mboot_BD_MST204A_D01A_S_128M.bin
cp bin/flash_with_uboot.bin ../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kriti/INTERNAL_DVBT2/HF/Normal/1600MHZ/mboot_BD_MST204A_D01A_S_128M.bin

make clean
make distclean
