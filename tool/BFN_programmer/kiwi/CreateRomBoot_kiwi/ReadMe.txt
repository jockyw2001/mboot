Generate and flash RomBoot.bin steps

<Case 1>   
   If there has no mboot on your target board, then 
   1. Move the moot.bin and Hash0_DAT.bin to this folder
      (Hash0_DAT.bin use the defualt bin file in this folder. 
       If u want to generate it, Go to //THEALE/RBoot/ to generate)

   2. Execute
      #./buildROMCodeBin.sh
   
      The script will generate RomBoot.bin for Tool bin to burn into the NAND.

<Case 2>
   If there has mboot on your target board, then you can just update mboot by tftp

   1: put the "mboot.bin" that you want to update in this folder
        (option)You can use the mBoot from //THEALE/Uranus/Kiwi/DDI_Package/ddi_pkg_linux/bsp/linux/Kiwi_Mbootbin
   2: set the Base Directory of tftp program to this path
   3: Enter mboot
   4: type "mstar writecis_nand.txt" or "mstar writecis_spinand.txt"