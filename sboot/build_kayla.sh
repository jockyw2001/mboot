mkdir -p tmp_kayla/

python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247A_D01A___128M_1866_MSD7XX33
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247A_D01A___256M_1866_MSD7XX35
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247A_D01A___256M_1866_MSD7XX35.BFN
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247A_D01A___512M_1866_MSD7XX37
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247B_D01A____64M_1333_MSD7XX31
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247B_D01A_S__64M_1333_MSD7XX31
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___128M_1600_MSD5S56
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___128M_1600_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___128M_1866_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___256M_1600_MSD5S56
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___256M_1600_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___256M_1866_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___512M_1333_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___512M_1600_MSD5S56
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___512M_1600_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A___512M_1866_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A_S_512M_1600_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247C_D01A_S_512M_1866_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247D_D01A____64M_1333_MSD5S56
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247D_D01A____64M_1333_MSD5XXX
python FixHash2StartAddr.py ./configs/kayla/.config.kayla_247D_D01A_S__64M_1333_MSD5XXX


make clean
make distclean
cp configs/kayla/.config.kayla_247A_D01A___128M_1866_MSD7XX33 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247A_D01A___128M_1866_MSD7XX33.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247A_D01A___256M_1866_MSD7XX35 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247A_D01A___256M_1866_MSD7XX35.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247A_D01A___512M_1866_MSD7XX37 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247A_D01A___512M_1866_MSD7XX37.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247B_D01A____64M_1333_MSD7XX31 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247B_D01A____64M_1333_MSD7XX31.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247B_D01A_S__64M_1333_MSD7XX31 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247B_D01A_S__64M_1333_MSD7XX31.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___128M_1600_MSD5S56 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___128M_1600_MSD5S56.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___128M_1600_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___128M_1600_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___128M_1866_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___128M_1866_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___256M_1600_MSD5S56 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___256M_1600_MSD5S56.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___256M_1600_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___256M_1600_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___256M_1866_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___256M_1866_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___512M_1333_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___512M_1333_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___512M_1600_MSD5S56 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___512M_1600_MSD5S56.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___512M_1600_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___512M_1600_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A___512M_1866_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A___512M_1866_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A_S_512M_1600_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A_S_512M_1600_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247C_D01A_S_512M_1866_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247C_D01A_S_512M_1866_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247D_D01A____64M_1333_MSD5S56 .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247D_D01A____64M_1333_MSD5S56.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247D_D01A____64M_1333_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247D_D01A____64M_1333_MSD5XXX.bin

make clean
make distclean
cp configs/kayla/.config.kayla_247D_D01A_S__64M_1333_MSD5XXX .config
make oldconfig 
make clean;make
cp bin/flash_with_uboot.bin ./tmp_kayla/mboot_247D_D01A_S__64M_1333_MSD5XXX.bin
