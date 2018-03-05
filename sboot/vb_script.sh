ORIG_PATH=$PATH

make clean
make distclean
echo "--- Kano MBoot ---"
cp configs/kano/.config.kano.ddi.231a_d01a_s.emmc.32bit.kernel .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- Kano MBoot build failed ---"
    exit 1
fi
echo "--- Kano MBoot Done ---"

make clean
make distclean
echo "--- Keres MBoot ---"
cp ./configs/keres/.config.keres_200A_D01A.DDI .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- Keres MBoot build failed ---"
    exit 1
fi
echo "--- Keres MBoot Done ---"

make clean
make distclean
echo "--- Kris MBoot ---"
cp ./configs/kris/.config.kris_227A_D01A_7SX7.DDI .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- Kris MBoot build failed ---"
    exit 1
fi
echo "--- Kris MBoot Done ---"

make clean
make distclean
echo "--- Kirin MBoot ---"
cp ./configs/kirin/.config.kirin_221D_D01A.DDI  .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- Kirin MBoot build failed ---"
    exit 1
fi
echo "--- Kirin MBoot Done ---"

make clean
make distclean
echo "--- Kiwi MBoot ---"
cp ./configs/kiwi/DDI_kiwi/.config.BD.MST239A.D01A.S.CPU900.DDR3.1866.512M.NAND .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- Kiwi MBoot build failed ---"
    exit 1
fi
echo "--- Kiwi MBoot Done ---"

make clean
make distclean
echo "--- K6 MBoot ---"
cp configs/k6/64bit_sboot/.config.k6.ddi.257b_d01a_s.emmc.64bit.kernel.DDR3.2133 .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- K6 MBoot build failed ---"
    exit 1
fi
echo "--- K6 MBoot Done ---"

make clean
make distclean
echo "--- K1P MBoot(HB) ---"
cp configs/HB_kayla/1866MHZ/mboot_BD_MST247A_D01A_128M.config .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- K6 MBoot build failed ---"
    exit 1
fi
echo "--- K1P MBoot(HB) Done ---"

make clean
make distclean
echo "--- K1P MBoot(DDI) ---"
cp configs/kayla/.config.kayla_247A_D01A___512M_1866_MSD7XX37 .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- K6 MBoot build failed ---"
    exit 1
fi
echo "--- K1P MBoot(DDI) Done ---"

make clean
make distclean
echo "--- K5TN MBoot(DDI) ---"
cp configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.BFN .config
make oldconfig > /dev/null 2>%1
make clean
make
if [ $? != 0 ]; then
    echo "--- K5TN MBoot build failed ---"
    exit 1
fi
echo "--- K5TN MBoot(DDI) Done ---"

#make clean
#make distclean
#
#echo "--- Kaiser MBoot ---"
#cp configs/kaiser/.config.kaiser.170A_D01A_S.nand.DDI .config
#make oldconfig
#make clean
#make
#if [ $? != 0 ]; then
#    echo "--- Kaiser MBoot build failed ---"
#    exit 1
#fi

echo "------ All done ------"
