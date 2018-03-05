cp configs/kappa/.cofig.173A_D01A_S.DDI .config
echo "build .cofig.173A_D01A_S.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173A_D01A_S.bin

make clean
make distclean

cp configs/kappa/.cofig.173A_D01A.DDI .config
echo "build .cofig.173A_D01A.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173A_D01A.bin

make clean
make distclean

cp configs/kappa/.cofig.173A_D01A.128.DDI .config
echo "build .cofig.173A_D01A.128.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173A_D01A_128MB.bin

make clean
make distclean

cp configs/kappa/.cofig.173A_D02A.DDI .config
echo "build .cofig.173A_D02A.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173A_D02A.bin

make clean
make distclean

cp configs/kappa/.cofig.173B_D01A_S.DDI .config
echo "build .cofig.173B_D01A_S.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173B_D01A_S.bin

make clean
make distclean

cp configs/kappa/.cofig.173B_D01A.DDI .config
echo "build .cofig.173B_D01A.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173B_D01A.bin

make clean
make distclean

cp configs/kappa/.cofig.173C_D01A_S.DDI .config
echo "build .cofig.173C_D01A_S.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173C_D01A_S.bin

make clean
make distclean

cp configs/kappa/.cofig.173C_D01A.DDI .config
echo "build .cofig.173C_D01A.DDI .."
make oldconfig > /dev/null
make clean;make
cp bin/flash_with_uboot.bin bin/mboot_BD_MST173C_D01A.bin

make clean
make distclean
