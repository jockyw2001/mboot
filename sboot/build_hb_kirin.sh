#!/bin/bash

function ergodic(){

array_fe_type=( [1]="INTERNAL_DVBC" [2]="DVBT2" [3]="DVBS2")
array_ddr=( [1]=1866 [2]=1600 )
target_file=mboot.bin

for((i=1;i<=${#array_fe_type[*]} ;i++));do
    case ${array_fe_type[i]} in
    "INTERNAL_DVBC")
        fe_type="XCXX"
        ;;
    "DVBT2")
        fe_type="XTXX"
        ;;
    "DVBS2")
        fe_type="XSXX"
        ;;
    esac

    for((j=1;j<=${#array_ddr[*]} ;j++));do
        dircation=./configs/HB_kirin/"${array_ddr[j]}""MHZ"/
        for file in ` ls -A $dircation`
        do
            if [[ ! "$file" =~ "$fe_type" ]]; then
                continue
            fi

            echo "-------------------------------------------------------"
            echo " $file "
            echo "-------------------------------------------------------"

            local path=$1"/"$file
            local name=$file
            if [[ ! "$file" =~ "_S" ]]; then
                name=${name/_X?XX*/_128M.bin}
            else
                name=${name/_X?XX*/.bin}
            fi

            cp -r $dircation"/"$file .config
            cp -r $dircation"/"$file ./scripts/defconfig
            make defconfig 
            make clean
            make
            source_bin_path=./bin
            target_bin_path=./bin/kirin/"${array_fe_type[i]}"/HF/Normal/"${array_ddr[j]}""MHZ"
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

target_HB_path=../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin/kirin/ 
if [ -d "$target_HB_path" ]; then
        cp -r bin/kirin/* $target_HB_path
        echo "copy to HB success!!!"
else
        echo "copy to HB fail, directory is not exit!!!"
fi

}
ergodic


