#!/bin/bash

function ergodic(){

target_file=mboot.bin

dircation=./configs/keres/bootlogo/
pattern=".*\.[lL][iI][nN][uU][xX]$"
for file in ` ls -A $dircation`
do
 
    echo "-------------------------------------------------------"
    echo "         $file                            "
    echo "-------------------------------------------------------"
    
    if [[ "$file" =~ $pattern ]]; then
        echo "=====linux========"
        target_bin_path=~/PERFORCE/THEALE/Uranus/Keres/DDI_Package/ddi_pkg_linux/bsp/linux/MBoot_BootLogo/
    else
        echo "=====ecos========"
        target_bin_path=~/PERFORCE/THEALE/Uranus/Keres/DDI_Package/ddi_pkg_ecos/bsp/ecos/Keres_Mbootbin/MBoot_BootLogo/
    fi

    local name=$file
    name=${name/.DDI*/_Bootlogo.bin}
    name=${name/.config.keres_/mboot_BD_MST}

    source_bin_path=./bin

    cp -r $dircation"/"$file .config
    cp -r $dircation"/"$file ./scripts/defconfig
    make defconfig 
    make clean
    make

    if [ -d "$target_bin_path" ]; then
        echo "cp"
        cp $source_bin_path/$target_file $target_bin_path/$name
        echo "cp success!!!"
    else
        echo "create"
        mkdir -p "$target_bin_path"
        cp $source_bin_path/$target_file $target_bin_path/$name
        echo "cp success!!!"
    fi

done;
}
ergodic


