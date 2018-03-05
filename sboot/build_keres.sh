mkdir tmp_keres/

python FixHash2StartAddr.py ./configs/keres/.config.BFN3.0.keres_200A_D01A_S_XCXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.BFN3.0.keres_200B_D01A_XSXX.DDI.Bootlogo
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A_S_XCXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A_S_XSXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A_S_XTXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A_XCXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A_XSXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200A_D01A_XTXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A_S_XCXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A_S_XSXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A_S_XTXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A_XCXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A_XSXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200B_D01A_XTXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200C_D01A.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200C_D01A_S_XXXX.DDI
python FixHash2StartAddr.py ./configs/keres/.config.keres_200C_D01A_XXXX.DDI

make clean
make distclean
cp configs/keres/.config.keres_200A_D01A_S_XCXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200A_D01A_S_XCXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200A_D01A_S_XSXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200A_D01A_S_XSXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200A_D01A_S_XTXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200A_D01A_S_XTXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200A_D01A_XCXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200A_D01A_XCXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200A_D01A_XSXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200A_D01A_XSXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200A_D01A_XTXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200A_D01A_XTXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200B_D01A_S_XCXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200B_D01A_S_XCXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200B_D01A_S_XSXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200B_D01A_S_XSXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200B_D01A_S_XTXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200B_D01A_S_XTXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200B_D01A_XCXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200B_D01A_XCXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200B_D01A_XSXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200B_D01A_XSXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200B_D01A_XTXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200B_D01A_XTXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200C_D01A_S_XXXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200C_D01A_S_XXXX.bin

make clean
make distclean
cp configs/keres/.config.keres_200C_D01A_XXXX.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_keres/mboot_BD_MST200C_D01A_XXXX.bin
