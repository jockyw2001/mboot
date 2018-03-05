#!/bin/bash

function ergodic(){

array_fe_type=( [1]="INTERNAL_DVBT")
array_ddr=( [1]=1333 [2]=1866 )
target_file=mboot.bin

for((i=1;i<=${#array_fe_type[*]} ;i++));do
    case ${array_fe_type[i]} in
    "INTERNAL_DVBC")
        fe_type="DVBC"
        ;;
    "INTERNAL_DVBT")
        fe_type="DVBT"
        ;;
    "INTERNAL_DVBS")
        fe_type="DVBS"
        ;;
    esac

    for((j=1;j<=${#array_ddr[*]} ;j++));do
        dircation=./configs/HB_k5tn/"${array_ddr[j]}""MHZ"/"${array_fe_type[i]}"/
        for file in ` ls -A $dircation`
        do
            echo "-------------------------------------------------------"
            echo " $file "
            echo "-------------------------------------------------------"

            local path=$1"/"$file
            local name=$file

            name=${name/.config*/.bin}
            cp -r $dircation"/"$file .config
            cp -r $dircation"/"$file ./scripts/defconfig
            make defconfig
            make clean
            make
            source_bin_path=./bin
            target_bin_path=./bin/k5tn/"${array_fe_type[i]}"/HF/Normal/CPU900/"${array_ddr[j]}""MHZ"
            echo "target_bin_path = "$target_bin_path" "
            echo "target_file     = $name"

            if [ -d "$target_bin_path" ]; then
                echo "cp "
                cp $source_bin_path/$target_file $target_bin_path/$name
                echo "cp success!!!"
            else
                echo "create"
                mkdir -p "$target_bin_path"
                cp $source_bin_path/$target_file $target_bin_path/$name
                echo "cp success!!!"
            fi

        done;
    done;
done;

target_HB_path=../DAILEO/HB_Formal_Release/Keres/HB_ecos/1.x.x/HB/DTV/boot/mboot/bin/k5tn
if [ -d "$target_HB_path" ]; then
        cp -r bin/k5tn/* $target_HB_path
        echo "copy to HB success!!!"
else
        echo "copy to HB fail, directory is not exit!!!"
fi

}
ergodic
