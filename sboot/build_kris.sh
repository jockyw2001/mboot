mkdir -p tmp/

python FixHash2StartAddr.py ./configs/kris/.config.kris_227A_D01A_7SX7.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227A_D01A_7SX7.DDI.BFN
python FixHash2StartAddr.py ./configs/kris/.config.kris_227A_D01A_7SX7.DDI_bootvideo.BFN
python FixHash2StartAddr.py ./configs/kris/.config.kris_227A_D01A_7SX7.DDI_bootvideo.BFS
python FixHash2StartAddr.py ./configs/kris/.config.kris_227B_D01A_7SX5.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227B_D01A_S_7SX5.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227C_D01A_7SX3.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227C_D01A_7SX3_128M.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227C_D01A_7SX3_256M.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227D_D01A_7SX0.DDI
python FixHash2StartAddr.py ./configs/kris/.config.kris_227D_D01A_7SX0.DDI.bootvideo

make clean
make distclean
cp configs/kris/.config.kris_227A_D01A_7SX7.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227A_D01A_7SX7.bin

make clean
make distclean
cp configs/kris/.config.kris_227B_D01A_7SX5.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227B_D01A_7SX5.bin

make clean
make distclean
cp configs/kris/.config.kris_227B_D01A_S_7SX5.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227B_D01A_S_7SX5.bin

make clean
make distclean
cp configs/kris/.config.kris_227C_D01A_7SX3.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227C_D01A_7SX3.bin

make clean
make distclean
cp configs/kris/.config.kris_227C_D01A_7SX3_128M.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227C_D01A_7SX3_128M.bin

make clean
make distclean
cp configs/kris/.config.kris_227C_D01A_7SX3_256M.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227C_D01A_7SX3_256M.bin

make clean
make distclean
cp configs/kris/.config.kris_227D_D01A_7SX0.DDI .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp/mboot_BD_MST227D_D01A_7SX0.bin
