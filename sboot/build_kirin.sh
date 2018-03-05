mkdir -p tmp_kirin/

#change Hash2 start adrress of config files
python FixHash2StartAddr.py ./configs/kirin/.config.BFN3.0.kirin_221A_D01A_S_XCXX
python FixHash2StartAddr.py ./configs/kirin/.config.BFN3.0.kirin_221A_D01A_XCXX.DDI.Bootlogo
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_S_XCXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_S_XHXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_S_XSXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_S_XTXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_XCXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_XHXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_XSXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D01A_XTXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D03A_XCXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D03A_XHXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D03A_XSXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221A_D03A_XTXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_S_XCXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_S_XHXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_S_XSXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_S_XTXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_XCXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_XHXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_XSXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221B_D01A_XTXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D01A_15_XCXX_128M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D01A_15_XCXX_256M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D01A_15_XSXX_128M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D01A_15_XSXX_256M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D01A_S_15_XCXX_256M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D01A_S_15_XSXX_256M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221C_D02A_17_XCXX_512M.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221D_D01A_S_XCXX.DDI
python FixHash2StartAddr.py ./configs/kirin/.config.kirin_221D_D01A_XCXX.DDI

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_S_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_S_XCXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_S_XHXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_S_XHXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_S_XSXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_S_XSXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_S_XTXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_S_XTXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_XCXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_XHXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_XHXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_XSXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_XSXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D01A_XTXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D01A_XTXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D03A_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D03A_XCXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D03A_XHXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D03A_XHXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D03A_XSXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D03A_XSXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221A_D03A_XTXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221A_D03A_XTXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_S_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_S_XCXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_S_XHXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_S_XHXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_S_XSXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_S_XSXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_S_XTXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_S_XTXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_XCXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_XHXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_XHXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_XSXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_XSXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221B_D01A_XTXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221B_D01A_XTXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D01A_15_XCXX_128M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D01A_15_XCXX_128M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D01A_15_XCXX_256M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D01A_15_XCXX_256M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D01A_15_XSXX_128M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D01A_15_XSXX_128M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D01A_15_XSXX_256M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D01A_15_XSXX_256M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D01A_S_15_XCXX_256M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D01A_S_15_XCXX_256M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D01A_S_15_XSXX_256M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D01A_S_15_XSXX_256M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221C_D02A_17_XCXX_512M.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221C_D02A_17_XCXX_512M.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221D_D01A_S_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221D_D01A_S_XCXX.bin

make clean
make distclean
cp ./configs/kirin/.config.kirin_221D_D01A_XCXX.DDI .config
make oldconfig 
make clean;make
cp ./bin/flash_with_uboot.bin ./tmp_kirin/mboot_BD_MST221D_D01A_XCXX.bin