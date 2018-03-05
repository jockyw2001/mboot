mkdir -p tmp_k5tn/MBoot_BFN/BD_MST266A_CPU1008_DDR3_1866_256M_MSD7TXX5X_AXX_Nanya_BFN/
mkdir tmp_k5tn/MBoot_BFN/BD_MST266A_CPU1008_DDR3_1866_256M_MSD7TXX5X_MIX_Nanya_Samsung_BFN/
mkdir tmp_k5tn/MBoot_BFN/BD_MST266A_CPU1008_DDR3_1866_512M_MSD7TXX7X_BFN/
mkdir tmp_k5tn/MBoot_BFN/BD_MST266B_CPU1008_DDR3_1866_512M_MSA3XXX7_BFN/
mkdir tmp_k5tn/MBoot_BFN/BD_MST266C_DVBS2_CPU1008_DDR3_1866_512M_MSA3HXX7_BFN/
mkdir tmp_k5tn/MBoot_BFN/BD_MST266D_TSIO_CPU1008_DDR3_1866_512M_MSD7TXX7X_BFN/

make clean
make distclean

python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR2.1333.64M.MSD7TXX1X.NOR2NAND
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR2.1333.64M.MSD7TXX1X.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR2.1333.64M.MSA3XXX1.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266C.DVBS2.CPU1008.DDR2.1333.64M.MSA3HXX1.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR2NAND
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266D.TSIO.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR2NAND
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266E.SD.CPU500.DDR2.1333.64M.MSD7X321C.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.128M.MSA3XXX3.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.AXX.Nanya.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.MIX.Nanya.Samsung.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.256M.MSA3XXX5.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.NOR
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.MIX.Nanya.Samsung.BFN
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.AXX.Nanya.BFN
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266C.DVBS2.CPU1008.DDR3.1866.512M.MSA3HXX7.BFN
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266D.TSIO.CPU1008.DDR3.1866.512M.MSD7TXX7X.BFN
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.BFN
python FixHash2StartAddr.py ./configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.BFN

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR2.1333.64M.MSD7TXX1X.NOR2NAND .config
echo "build .config.BD.MST266A.CPU1008.DDR2.1333.64M.MSD7TXX1X.NOR2NAND .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266A_CPU1008_DDR2_1333_64M_MSD7TXX1X_NOR2NAND.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR2.1333.64M.MSD7TXX1X.NOR .config
echo "build .config.BD.MST266A.CPU1008.DDR2.1333.64M.MSD7TXX1X.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266A_CPU1008_DDR2_1333_64M_MSD7TXX1X_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR2.1333.64M.MSA3XXX1.NOR .config
echo "build .config.BD.MST266B.CPU1008.DDR2.1333.64M.MSA3XXX1.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266B_CPU1008_DDR2_1333_64M_MSA3XXX1_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266C.DVBS2.CPU1008.DDR2.1333.64M.MSA3HXX1.NOR .config
echo "build .config.BD.MST266C.DVBS2.CPU1008.DDR2.1333.64M.MSA3HXX1.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266C_DVBS2_CPU1008_DDR2_1333_64M_MSA3HXX1_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.128M.MSA3XXX3.NOR .config
echo "build .config.BD.MST266B.CPU1008.DDR3.1866.128M.MSA3XXX3.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266B_CPU1008_DDR3_1866_128M_MSA3XXX3_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.AXX.Nanya.NOR .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.AXX.Nanya.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266A_CPU1008_DDR3_1866_256M_MSD7TXX5X_AXX_Nanya_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.MIX.Nanya.Samsung.NOR .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.MIX.Nanya.Samsung.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266A_CPU1008_DDR3_1866_256M_MSD7TXX5X_MIX_Nanya_Samsung_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.256M.MSA3XXX5.NOR .config
echo "build .config.BD.MST266B.CPU1008.DDR3.1866.256M.MSA3XXX5.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266B_CPU1008_DDR3_1866_256M_MSA3XXX5_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266A_CPU1008_DDR3_1866_512M_MSD7TXX7X_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.NOR .config
echo "build .config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266B_CPU1008_DDR3_1866_512M_MSA3XXX7_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR2NAND .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR2NAND .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266A_CPU1008_DDR3_1866_512M_MSD7TXX7X_NOR2NAND.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266D.TSIO.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR2NAND .config
echo "build .config.BD.MST266D.TSIO.CPU1008.DDR3.1866.512M.MSD7TXX7X.NOR2NAND .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266D_TSIO_CPU1008_DDR3_1866_512M_MSD7TXX7X_NOR2NAND.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266E.SD.CPU500.DDR2.1333.64M.MSD7X321C.NOR
echo "build .config.BD.MST266E.SD.CPU500.DDR2.1333.64M.MSD7X321C.NOR .."
make oldconfig
make clean;make
cp bin/flash_with_uboot.bin tmp_k5tn/BD_MST266E_SD_CPU500_DDR2_1333_64M_MSD7X321C_NOR.bin

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.MIX.Nanya.Samsung.BFN .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.MIX.Nanya.Samsung.BFN .."
make oldconfig
make clean;make
cp bin/{bfn_app.bin,bfn_loader.bin,bfn_miu.bin,flash.bin} tmp_k5tn/MBoot_BFN/BD_MST266A_CPU1008_DDR3_1866_256M_MSD7TXX5X_MIX_Nanya_Samsung_BFN/

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.AXX.Nanya.BFN .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.256M.MSD7TXX5X.AXX.Nanya.BFN .."
make oldconfig
make clean;make
cp bin/{bfn_app.bin,bfn_loader.bin,bfn_miu.bin,flash.bin} tmp_k5tn/MBoot_BFN/BD_MST266A_CPU1008_DDR3_1866_256M_MSD7TXX5X_AXX_Nanya_BFN/

cp configs/k5tn/ddi_k5tn/.config.BD.MST266C.DVBS2.CPU1008.DDR3.1866.512M.MSA3HXX7.BFN .config
echo "build .config.BD.MST266C.DVBS2.CPU1008.DDR3.1866.512M.MSA3HXX7.BFN .."
make oldconfig
make clean;make
cp bin/{bfn_app.bin,bfn_loader.bin,bfn_miu.bin,flash.bin} tmp_k5tn/MBoot_BFN/BD_MST266C_DVBS2_CPU1008_DDR3_1866_512M_MSA3HXX7_BFN/

cp configs/k5tn/ddi_k5tn/.config.BD.MST266D.TSIO.CPU1008.DDR3.1866.512M.MSD7TXX7X.BFN .config
echo "build .config.BD.MST266D.TSIO.CPU1008.DDR3.1866.512M.MSD7TXX7X.BFN .."
make oldconfig
make clean;make
cp bin/{bfn_app.bin,bfn_loader.bin,bfn_miu.bin,flash.bin} tmp_k5tn/MBoot_BFN/BD_MST266D_TSIO_CPU1008_DDR3_1866_512M_MSD7TXX7X_BFN/

cp configs/k5tn/ddi_k5tn/.config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.BFN .config
echo "build .config.BD.MST266B.CPU1008.DDR3.1866.512M.MSA3XXX7.BFN .."
make oldconfig
make clean;make
cp bin/{bfn_app.bin,bfn_loader.bin,bfn_miu.bin,flash.bin} tmp_k5tn/MBoot_BFN/BD_MST266B_CPU1008_DDR3_1866_512M_MSA3XXX7_BFN/

cp configs/k5tn/ddi_k5tn/.config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.BFN .config
echo "build .config.BD.MST266A.CPU1008.DDR3.1866.512M.MSD7TXX7X.BFN .."
make oldconfig
make clean;make
cp bin/{bfn_app.bin,bfn_loader.bin,bfn_miu.bin,flash.bin} tmp_k5tn/MBoot_BFN/BD_MST266A_CPU1008_DDR3_1866_512M_MSD7TXX7X_BFN/
