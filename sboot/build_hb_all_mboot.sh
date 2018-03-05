#!/bin/bash
declare -A chip_family;
chip_family=(
      [k1]=k1
      [kappa]=kappa
      [keltic]=keltic
      [kriti]=kriti
      [kris]=kris 
      [kirin]=kirin 
      [keres]=keres)

declare -A chip;
chip=(
      [7SX7]=CONFIG_MSD7SX7=y
      [7SX5]=CONFIG_MSD7SX5=y 
      [7SX3]=CONFIG_MSD7SX3=y 
      [7SX0]=CONFIG_MSD7SX0=y
      [5XX1]=CONFIG_MSD5XX1=y
      [5XX3]=CONFIG_MSD5XX3=y 
      [5XX5]=CONFIG_MSD5XX5=y 
      [5XX7]=CONFIG_MSD5XX7=y
      [5XX9]=CONFIG_MSD5XX9=y)

declare -A ddr_size;
ddr_size=(
      [32M]=CONFIG_MSTAR_DDR_32MB=y
      [64M]=CONFIG_MSTAR_DDR_64MB=y
      [128M]=CONFIG_MSTAR_DDR_128MB=y
      [256M]=CONFIG_MSTAR_DDR_256MB=y
      [512M]=CONFIG_MSTAR_DDR_512MB=y
      [32M*32M]=CONFIG_MSTAR_DDR_32MB_32MB=y
      [64M*32M]=CONFIG_MSTAR_DDR_64MB_32MB=y
      [64M*64M]=CONFIG_MSTAR_DDR_64MB_64MB=y
      [128M*64M]=CONFIG_MSTAR_DDR_128MB_64MB=y
      [128M*128M]=CONFIG_MSTAR_DDR_128MB_128MB=y
      [256M*256M]=CONFIG_MSTAR_DDR_256MB_256MB=y
      [512M*512M]=CONFIG_MSTAR_DDR_512MB_512MB=y)

declare -A mmap;
mmap=(
      [32M]=CONFIG_MSTAR_MMAP_32MB=y
      [64M]=CONFIG_MSTAR_MMAP_64MB=y
      [128M]=CONFIG_MSTAR_MMAP_128MB=y
      [256M]=CONFIG_MSTAR_MMAP_256MB=y
      [512M]=CONFIG_MSTAR_MMAP_512MB=y
      [32M*32M]=CONFIG_MSTAR_MMAP_32MB_32MB=y
      [64M*32M]=CONFIG_MSTAR_MMAP_64MB_32MB=y
      [64M*64M]=CONFIG_MSTAR_MMAP_64MB_64MB=y
      [128M*64M]=CONFIG_MSTAR_MMAP_128MB_64MB=y
      [128M*128M]=CONFIG_MSTAR_MMAP_128MB_128MB=y
      [256M*256M]=CONFIG_MSTAR_MMAP_256MB_256MB=y
      [512M*512M]=CONFIG_MSTAR_MMAP_512MB_512MB=y)

declare -A board;
board=(
#board k1
      [MST124A_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124A_D01A=y
      [MST124A_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124A_D01A=y
      [MST124A_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124A_D01A_S=y
      [MST124A_D02A_S]=CONFIG_MSTAR_KRONUS_BD_MST124A_D02A_S=y
      [MST124B_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124B_D01A_S=y
      [MST124B_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124B_D02A=y
      [MST124B_D02A_S]=CONFIG_MSTAR_KRONUS_BD_MST124B_D02A_S=y
      [MST124C_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124C_D01A_S=y
      [MST124C_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124C_D02A=y
      [MST124C_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124C_D02A=y
      [MST124D_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124D_D01A=y
      [MST124D_D01B]=CONFIG_MSTAR_KRONUS_BD_MST124D_D01A=y
      [MST124D_D01B_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124D_D01A=y
      [MST124D_D02A]=CONFIG_MSTAR_KRONUS_BD_MST124D_D02A=y
      [MST124D_D02A_S]=CONFIG_MSTAR_KRONUS_BD_MST124D_D02A_S=y
      [MST124E_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124E_D01A_S=y
      [MST124E_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124E_D01A_SZ=y
      [MST124SZ]=CONFIG_MSTAR_KRONUS_BD_MST124SZ=y
      [MST124G_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124G_D01A=y
      [MST124G_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124G_D01A_S=y
      [MST140A_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST140A_D01A=y
      [MST140A_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST140A_D01A_S=y
      [MST163A_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST163A_D01A_S=y
      [MST124F_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124F_D01A_S=y
      [MST124F_D01A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124F_SZ_Demo=y
      [MST124F_D02A_SZ]=CONFIG_MSTAR_KRONUS_BD_MST124F_D02A=y
      [MST124I_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124I_D01A_S=y
      [MST124H_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124H_D01A=y
      [MST124K_D01A_S]=CONFIG_MSTAR_KRONUS_BD_MST124K_D01A_S=y
      [MST124K_D01A]=CONFIG_MSTAR_KRONUS_BD_MST124K_D01A=y
      [SS2000_DT0A]=CONFIG_MSTAR_KRONUS_BD_SS2000_DT0A=y
#board kappa
      [MST173A_D01A_S]=CONFIG_MSTAR_KAPPA_BD_MST173A_D01A_S=y       
      [MST173A_D01A]=CONFIG_MSTAR_KAPPA_BD_MST173A_D01A=y
      [MST173A_D02A]=CONFIG_MSTAR_KAPPA_BD_MST173A_D02A=y       
      [MST173B_D01A_S]=CONFIG_MSTAR_KAPPA_BD_MST173B_D01A_S=y       
      [MST173B_D01A]=CONFIG_MSTAR_KAPPA_BD_MST173B_D01A=y       
      [MST173C_D01A_S]=CONFIG_MSTAR_KAPPA_BD_MST173C_D01A_S=y       
      [MST173C_D01A]=CONFIG_MSTAR_KAPPA_BD_MST173C_D01A=y       
#board keltic
      [MST183A_D01A]=CONFIG_MSTAR_KELTIC_BD_MST183A_D01A=y
      [MST183A_D01A_S]=CONFIG_MSTAR_KELTIC_BD_MST183A_D01A_S=y
      [MST183B_D01A]=CONFIG_MSTAR_KELTIC_BD_MST183B_D01A=y
      [MST183B_D01A_S]=CONFIG_MSTAR_KELTIC_BD_MST183B_D01A_S=y
#board_keres
      [MST200A_D01A_S]=CONFIG_MSTAR_KERES_BD_MST200A_D01A_S=y
      [MST200A_D01A]=CONFIG_MSTAR_KERES_BD_MST200A_D01A=y
      [MST200B_D01A_S]=CONFIG_MSTAR_KERES_BD_MST200B_D01A_S=y
      [MST200B_D01A]=CONFIG_MSTAR_KERES_BD_MST200B_D01A=y
      [MST200C_D01A_S]=CONFIG_MSTAR_KERES_BD_MST200C_D01A_S=y
      [MST200C_D01A]=CONFIG_MSTAR_KERES_BD_MST200C_D01A=y
#board_kirin
      [MST221A_D01A_S]=CONFIG_MSTAR_KIRIN_BD_MST221A_D01A_S=y
      [MST221A_D01A]=CONFIG_MSTAR_KIRIN_BD_MST221A_D01A=y
      [MST221B_D01A_S]=CONFIG_MSTAR_KIRIN_BD_MST221B_D01A_S=y
      [MST221B_D01A]=CONFIG_MSTAR_KIRIN_BD_MST221B_D01A=y
      [MST221C_D01A_S]=CONFIG_MSTAR_KIRIN_BD_MST221C_D01A_S=y
      [MST221C_D01A]=CONFIG_MSTAR_KIRINS_BD_MST221C_D01A=y
      [MST221C_D01A_S_15]=CONFIG_MSTAR_KIRIN_BD_MST221C_D01A_S_15=y
      [MST221C_D01A_15]=CONFIG_MSTAR_KIRIN_BD_MST221C_D01A_15=y
#board_kris
      [MST227A_D01A]=CONFIG_MSTAR_KRIS_BD_MST227A_D01A=y
      [MST227B_D01A]=CONFIG_MSTAR_KRIS_BD_MST227B_D01A=y
      [MST227B_D01A_S]=CONFIG_MSTAR_KRIS_BD_MST227B_D01A_S=y
      [MST227C_D01A]=CONFIG_MSTAR_KRIS_BD_MST227C_D01A=y
      [MST227D_D01A]=CONFIG_MSTAR_KRIS_BD_MST227D_D01A=y
#board kriti
      [MST204A_D01A_DDR2]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_DDR2=y
      [MST204A_D01A_DDR3]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_DDR3=y
      [MST204A_D01A_S_DDR2]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_S_DDR2=y
      [MST204A_D01A_S_DDR3]=CONFIG_MSTAR_KRITI_BD_MST204A_D01A_S_DDR3=y)

declare -A mem_freq;
mem_freq=( 
      [1866MHZ]=CONFIG_MEM1866MHz=y 
      [1600MHZ]=CONFIG_MEM1600MHz=y 
      [1300MHZ]=CONFIG_MEM1300MHz=y
      [1066MHZ]=CONFIG_MEM1066MHz=y 
      [1333MHZ]=CONFIG_MEM1333MHz=y 
      [1200MHZ]=CONFIG_MEM1200MHz=y
      [800MHZ]=CONFIG_MEM800MHz=y 
      [667MHZ]=CONFIG_MEM667MHz=y
      [400MHZ]=CONFIG_MEM400MHz=y  
      [333MHZ]=CONFIG_MEM333MHz=y)


declare -A dtv_sys;
dtv_sys=(
      [DVBC_SYS]=CONFIG_MSDXCXX=y
      [DVBT_SYS]=CONFIG_MSDXTXX=y
      [DVBS_SYS]=CONFIG_MSDXSXX=y
      [NONE_DTV_SYS]=CONFIG_MSDXXXX=y)

declare -A frontend_type;
frontend_type=(
      [DVBC]=CONFIG_DVB_C_SYS=y
      [INTERNAL_DVBC]=CONFIG_DVB_C_SYS=y
      [DVBT]=CONFIG_DVB_T_SYS=y
      [INTERNAL_DVBT]=CONFIG_DVB_T2_SYS=y
      [DVBT2]=CONFIG_DVB_T2_SYS=y
      [INTERNAL_DVBT2]=CONFIG_DVB_T2_SYS=y
      [ISDBT]=CONFIG_ISDB_T_SYS=y
      [INTERNAL_ISDBT]=CONFIG_ISDB_T_SYS=y
      [DVBS]=CONFIG_DVB_S_SYS=y
      [INTERNAL_DVBS]=CONFIG_DVB_S_SYS=y
      [DVBS2]=CONFIG_DVB_S2_SYS=y
      [DTMB]=CONFIG_DVB_DTMB_SYS=y)

declare -A tuner;
tuner=( 
      [RT710]=CONFIG_TUNER_RT710=y
      [TD1611ALF]=CONFIG_MSTAR_TUNER_NXP_TD1611ALF=y
      [FJ2207]=MSTAR_TUNER_NUTUNE_FJ2207=y
      [FT2125]=CONFIG_MSTAR_TUNER_NUTUNE_FT2125=y
      [AV2012]=CONFIG_TUNER_AV2012=y
      [MXL603]=CONFIG_MSTAR_TUNER_MXL_MXL603=y 
      [MXL608]=CONFIG_MSTAR_TUNER_MXL_MXL608=y
      [RD5815M]=CONFIG_MSTAR_TUNER_RDA5815M=y
      [RT820]=CONFIG_MSTAR_TUNER_RT820T=y)

declare -A demod;
demod=(
      [MSB124x]=CONFIG_MSTAR_MSB124x_DEMOD=y
      [MSB123x]=CONFIG_MSTAR_MSB123x_DEMOD=y
      [MSB122x]=CONFIG_MSTAR_MSB122x_DEMOD=y
      [MSB1245]=CONFIG_MSTAR_MSB1245_DEMOD=y
      [MSB1238]=CONFIG_MSTAR_MSB1238_DEMOD=y
      [MSB1400]=CONFIG_MSTAR_MSB1400_DEMOD=y
      [MSB131X]=CONFIG_MSTAR_MSB131X_DEMOD=y
      [MSB1236C]=CONFIG_MSTAR_MSB1236C_DEMOD=y
      [KERES_EMBEDDED_DVBC]=CONFIG_EMBEDDED_DVBC_DEMOD=y
      [KELTIC_EMBEDDED_DVBS]=CONFIG_MSTAR_KELTIC_INTERNAL_DVBS_DEMOD=y
      [KELTIC_EMBEDDED_DVBC]=CONFIG_MSTAR_KELTIC_INTERNAL_DVBC_DEMOD=y
      [KRITI_EMBEDDED_DVBT2]=CONFIG_MSTAR_KRITI_INTERNAL_DEMOD=y
      [KRITI_EMBEDDED_DVBT]=CONFIG_MSTAR_KRITI_INTERNAL_DEMOD=y)

declare -A CPU_Freq;
CPU_Freq=(
      [CPU984]=CONFIG_CPU_984MHZ=y
      [CPU1008]=CONFIG_CPU_1008MHZ=y
      [CPU900]=CONFIG_CPU_900MHZ=y
      [CPU840]=CONFIG_CPU_840MHZ=y
      [CPU792]=CONFIG_CPU_792MHZ=y
      [CPU720]=CONFIG_CPU_720MHZ=y
      [CPU696]=CONFIG_CPU_696MHZ=y
      [CPU667]=CONFIG_CPU_667MHZ=y
      [CPU648]=CONFIG_CPU_648MHZ=y
      [CPU624]=CONFIG_CPU_624MHZ=y
      [CPU600]=CONFIG_CPU_600MHZ=y
      [CPU576]=CONFIG_CPU_576MHZ=y
      [CPU552]=CONFIG_CPU_552MHZ=y
      [CPU504]=CONFIG_CPU_504MHZ=y
      [CPU456]=CONFIG_CPU_456MHZ=y
      [CPU432]=CONFIG_CPU_432MHZ=y
      [CPU396]=CONFIG_CPU_396MHZ=y
      [CPU348]=CONFIG_CPU_348MHZ=y
      [CPU336]=CONFIG_CPU_336MHZ=y
      [CPU324]=CONFIG_CPU_324MHZ=y
      [CPU312]=CONFIG_CPU_312MHZ=y
      [CPU288]=CONFIG_CPU_288MHZ=y)

declare -A config;
config=(
      [oad_on]='CONFIG_OAD=y'
      [oad_off]='# CONFIG_OAD is not set'
      [frontend_on]='CONFIG_FRONT_END=y'
      [frontend_off]='# CONFIG_FRONT_END is not set'
      [oad_in_mboot_on]='CONFIG_OAD_IN_MBOOT=y'
      [oad_in_mboot_off]='# CONFIG_OAD_IN_MBOOT is not set'
)

function change_choice(){

    for var in $1
        do
            sed -i 's/'"$var"'/# '"${var%=*}"' is not set/g' scripts/defconfig
        done
    sed -i 's/# '"${2%=*}"' .*/'"$2"'/g' scripts/defconfig

}

function change_config(){

    sed -i 's/# '"${config[$1]%=*}"' .*/'"${config[$1]}"'/g' scripts/defconfig
    sed -i 's/'"${config[$1]%=*}"'=y/'"${config[$1]}"'/g' scripts/defconfig
    local var=${config[$1]:2}
    sed -i 's/# '"${var% is*}"' .*/'"${config[$1]}"'/g' scripts/defconfig
    sed -i 's/'"${var% is*}"'=y/'"${config[$1]}"'/g' scripts/defconfig

}

target_path=../../../Hummingbird/MainTrunk/DVBT/boot/mboot/bin

function mstar_build(){

    rm .config 
    rm include/autoconf.h
    if [[ "$1" == "keres" || "$1" == "kirin" || "$1" == "kris" ]]
      then
        target_file=bin/mboot.bin
        make defconfig >/dev/null
    else
        target_file=bin/flash_with_uboot.bin
        make defconfig>/dev/null
        cd ../u-boot-1.1.6/;make clean;cd ../sboot/
    fi             
    make clean
    make
    mkdir -p $target_path/$1/$2/HF/Normal/$3
    if [[ "$1" == "keres" || "$1" == "k1" || "$1" == "kirin" || "$1" == "kris" ]] && [[ "$4" == *"_S" || "$4" == *"_S_15" ]]
      then
        cp $target_file $target_path/$1/$2/HF/Normal/$3/mboot_BD_$4.bin
    elif [[ "$1" == "kriti" ]]
      then
        cp $target_file $target_path/$1/$2/HF/Normal/$3/mboot_BD_${4%_DDR*}_$5.bin
    else
        cp $target_file $target_path/$1/$2/HF/Normal/$3/mboot_BD_$4_$5.bin
    fi

}

function main(){

    for var in $@
        do
            if [[ " $var " == $(echo " ${!chip_family[@]} "|grep -o " $var ") ]]
                then
                    cp .config.$var.hb.ecos scripts/defconfig
            elif [[ " $var " == $(echo " ${!chip[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${chip[*]}" ${chip[$var]}
            elif [[ " $var " == $(echo " ${!dtv_sys[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${dtv_sys[*]}" ${dtv_sys[$var]}
            elif [[ " $var " == $(echo " ${!board[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${board[*]}" ${board[$var]}
            elif [[ " $var " == $(echo " ${!ddr_size[@]} "|grep -o " $var ") ]]
                then
                    DDR_size=${ddr_size[$var]}
                    change_choice "${mmap[*]}" ${mmap[$var]}
                    change_choice "${ddr_size[*]}" $DDR_size
            elif [[ " $var " == $(echo " ${!mem_freq[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${mem_freq[*]}" ${mem_freq[$var]}
            elif [[ " $var " == $(echo " ${!frontend_type[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${frontend_type[*]}" ${frontend_type[$var]}
            elif [[ " $var " == $(echo " ${!tuner[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${tuner[*]}" ${tuner[$var]}
            elif [[ " $var " == $(echo " ${!demod[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${demod[*]}" ${demod[$var]}
            elif [[ " $var " == $(echo " ${!CPU_Freq[@]} "|grep -o " $var ") ]]
                then
                    change_choice "${CPU_Freq[*]}" ${CPU_Freq[$var]}
            elif [[ " $var " == $(echo " ${!config[@]} "|grep -o " $var ") ]]
                then
                    change_config "$var"
            else
                echo "$var error"
                exit
            fi
        done

    for var in $@
        do
              if [[ " $var " == $(echo " ${!chip_family[@]} "|grep -o " $var ") ]]
                  then
                      chip_family_dir=$var
              elif [[ " $var " == $(echo " ${!frontend_type[@]} "|grep -o " $var ") ]]
                  then
                      frontend_type_dir=$var
              elif [[ " $var " == $(echo " ${!mem_freq[@]} "|grep -o " $var ") ]]
                  then
                      mem_freq_dir=$var
              elif [[ " $var " == $(echo " ${!board[@]} "|grep -o " $var ") ]]
                  then
                      board_name=$var
              elif [[ " $var " == $(echo " ${!ddr_size[@]} "|grep -o " $var ") ]]
                  then
                      ddr_size_name=$var
              else
                      echo 
              fi
    done

    mstar_build $chip_family_dir $frontend_type_dir $mem_freq_dir $board_name $ddr_size_name

}

if [[ "$1" == "k1" || ! -n "$1" ]]
  then
#chip_family chip board CPU_Freq ddr_size mm_feq frontend_on/off frontend_type demod tuner oad_on/off oad_in_mboot_on/off
#k1 DTMB
main k1 MST124A_D01A CPU552 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124A_D02A CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124A_D01A_S CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST124B_D01A_SZ CPU552 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124B_D01A_SZ CPU552 128M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST124C_D01A_SZ CPU552 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124C_D01A_SZ CPU552 128M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124C_D02A CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST124D_D01A_SZ CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124D_D01B CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124D_D01B_SZ CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST124E_D01A_SZ CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST124F_D01A_SZ CPU552 64M 800MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST124F_D01A_SZ CPU552 64M 667MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST140A_D01A_SZ CPU552 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main k1 MST140A_D01A_SZ CPU552 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main k1 MST163A_D01A_S CPU552 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off

#k1 dvbc
main k1 MST124A_D01A CPU552 64M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124A_D02A CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124A_D01A_S CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST124B_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124B_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST124C_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124C_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124C_D02A CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST124D_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124D_D01B CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124D_D01B_SZ CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST124E_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST124F_D01A_SZ CPU552 64M 800MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124F_D01A_SZ CPU552 64M 667MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST124F_D02A_SZ CPU552 64M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST124H_D01A CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST140A_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off
main k1 MST140A_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBC oad_off oad_in_mboot_off

main k1 MST163A_D01A_S CPU552 64M 1066MHZ frontend_off DVBC oad_off oad_in_mboot_off

#k1 DVBS
main k1 MST124A_D01A CPU552 64M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124A_D02A CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124A_D01A_S CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST124B_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124B_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST124C_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124C_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124C_D02A CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST124D_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124D_D01B CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124D_D01B_SZ CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST124E_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST124F_D01A_SZ CPU552 64M 800MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST124F_D01A_SZ CPU552 64M 667MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST140A_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off
main k1 MST140A_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBS oad_off oad_in_mboot_off

main k1 MST163A_D01A_S CPU552 64M 1066MHZ frontend_off DVBS oad_off oad_in_mboot_off


#k1 DVBT
main k1 MST124A_D01A CPU552 64M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124A_D02A CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124A_D01A_S CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST124B_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124B_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST124C_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124C_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124C_D02A CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST124D_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124D_D01B CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124D_D01B_SZ CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST124E_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST124F_D01A_SZ CPU552 64M 800MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST124F_D01A_SZ CPU552 64M 667MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST140A_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off
main k1 MST140A_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBT oad_off oad_in_mboot_off

main k1 MST163A_D01A_S CPU552 64M 1066MHZ frontend_off DVBT oad_off oad_in_mboot_off

#k1 DVBT2
main k1 MST124A_D01A CPU552 64M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124A_D02A CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124A_D01A_S CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST124B_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124B_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST124C_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124C_D01A_SZ CPU552 128M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124C_D02A CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST124D_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124D_D01B CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124D_D01B_SZ CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST124E_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST124F_D01A_SZ CPU552 64M 800MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST124F_D01A_SZ CPU552 64M 667MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST140A_D01A_SZ CPU552 64M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off
main k1 MST140A_D01A_SZ CPU552 128M 1600MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

main k1 MST163A_D01A_S CPU552 64M 1066MHZ frontend_off DVBT2 oad_off oad_in_mboot_off

#k1 ISDBT
main k1 MST124A_D01A CPU552 64M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124A_D02A CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124A_D01A_S CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST124B_D01A_SZ CPU552 64M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124B_D01A_SZ CPU552 128M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST124C_D01A_SZ CPU552 64M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124C_D01A_SZ CPU552 128M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124C_D02A CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST124D_D01A_SZ CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124D_D01B CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124D_D01B_SZ CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST124E_D01A_SZ CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST124F_D01A_SZ CPU552 64M 800MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST124F_D01A_SZ CPU552 64M 667MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST140A_D01A_SZ CPU552 64M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off
main k1 MST140A_D01A_SZ CPU552 128M 1600MHZ frontend_off ISDBT oad_off oad_in_mboot_off

main k1 MST163A_D01A_S CPU552 64M 1066MHZ frontend_off ISDBT oad_off oad_in_mboot_off

fi

if [[ "$1" == "kappa" || ! -n "$1" ]]
  then
#chip_family chip board CPU_Freq ddr_size mm_feq frontend_on/off frontend_type demod tuner oad_on/off oad_in_mboot_on/off
#kappa DTMB
main kappa MST173A_D01A CPU600 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main kappa MST173A_D01A CPU600 128M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main kappa MST173A_D01A CPU600 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off
main kappa MST173A_D02A CPU600 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main kappa MST173B_D01A CPU600 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main kappa MST173B_D01A CPU600 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off

main kappa MST173C_D01A CPU600 64M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main kappa MST173C_D01A CPU600 128M 1066MHZ frontend_off DTMB oad_off oad_in_mboot_off
main kappa MST173C_D01A CPU600 128M 1600MHZ frontend_off DTMB oad_off oad_in_mboot_off
#kappa DVBS2
cp -r $target_path/kappa/DTMB $target_path/kappa/DVBS2
#kappa DVBT2
cp -r $target_path/kappa/DTMB $target_path/kappa/DVBT2
#kappa DVBS2
cp -r $target_path/kappa/DTMB $target_path/kappa/DVBS2
#kappa internal_dvbc
cp -r $target_path/kappa/DTMB $target_path/kappa/INTERNAL_DVBC
#kappa internal_dvbt
cp -r $target_path/kappa/DTMB $target_path/kappa/INTERNAL_DVBT
#kappa internal_isdbt
cp -r $target_path/kappa/DTMB $target_path/kappa/INTERNAL_ISDBT
#kappa isdbt
cp -r $target_path/kappa/DTMB $target_path/kappa/ISDBT

fi

if [[ "$1" == "keltic" || ! -n "$1" ]]
  then
#chip_family chip board CPU_Freq ddr_size mm_feq frontend_on/off frontend_type demod tuner oad_on/off oad_in_mboot_on/off
#keltic internal_dvbc
main keltic 5XX1 MST183A_D01A CPU504 32M 800MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keltic 5XX5 MST183A_D01A CPU504 64M 800MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keltic 5XX1 MST183A_D01A_S CPU504 32M 800MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keltic 5XX5 MST183A_D01A_S CPU504 64M 800MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keltic 5XX7 MST183B_D01A CPU504 64M 800MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A CPU504 64M 1333MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A CPU504 128M 1333MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keltic 5XX7 MST183B_D01A_S CPU504 64M 800MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A_S CPU504 64M 1333MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A_S CPU504 128M 1333MHZ frontend_on INTERNAL_DVBC KELTIC_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

#keltic internal_dvbs
main keltic 5XX1 MST183A_D01A CPU504 32M 800MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on
main keltic 5XX5 MST183A_D01A CPU504 64M 800MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on

main keltic 5XX1 MST183A_D01A_S CPU504 32M 800MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on
main keltic 5XX5 MST183A_D01A_S CPU504 64M 800MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on

main keltic 5XX7 MST183B_D01A CPU504 64M 800MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A CPU504 64M 1333MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A CPU504 128M 1333MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on

main keltic 5XX7 MST183B_D01A_S CPU504 64M 800MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A_S CPU504 64M 1333MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A_S CPU504 128M 1333MHZ frontend_on INTERNAL_DVBS KELTIC_EMBEDDED_DVBS RD5815M oad_on oad_in_mboot_on

#keltic DTMB
main keltic 5XX1 MST183A_D01A CPU504 32M 800MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on
main keltic 5XX5 MST183A_D01A CPU504 64M 800MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on

main keltic 5XX1 MST183A_D01A_S CPU504 32M 800MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on
main keltic 5XX5 MST183A_D01A_S CPU504 64M 800MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on

main keltic 5XX7 MST183B_D01A CPU504 64M 800MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A CPU504 64M 1333MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A CPU504 128M 1333MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on

main keltic 5XX7 MST183B_D01A_S CPU504 64M 800MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A_S CPU504 64M 1333MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on
main keltic 5XX9 MST183B_D01A_S CPU504 128M 1333MHZ frontend_on DTMB MSB1238 MXL608 oad_on oad_in_mboot_on

#keltic DVBS2
main keltic 5XX1 MST183A_D01A CPU504 32M 800MHZ frontend_off DVBS2 oad_off
main keltic 5XX5 MST183A_D01A CPU504 64M 800MHZ frontend_off DVBS2 oad_off

main keltic 5XX1 MST183A_D01A_S CPU504 32M 800MHZ frontend_off DVBS2 oad_off
main keltic 5XX5 MST183A_D01A_S CPU504 64M 800MHZ frontend_off DVBS2 oad_off

main keltic 5XX7 MST183B_D01A CPU504 64M 800MHZ frontend_off DVBS2 oad_off
main keltic 5XX9 MST183B_D01A CPU504 64M 1333MHZ frontend_off DVBS2 oad_off
main keltic 5XX9 MST183B_D01A CPU504 128M 1333MHZ frontend_off DVBS2 oad_off

main keltic 5XX7 MST183B_D01A_S CPU504 64M 800MHZ frontend_off DVBS2 oad_off
main keltic 5XX9 MST183B_D01A_S CPU504 64M 1333MHZ frontend_off DVBS2 oad_off
main keltic 5XX9 MST183B_D01A_S CPU504 128M 1333MHZ frontend_off DVBS2 oad_off

#keltic DVBT2
cp -r $target_path/keltic/DVBS2 $target_path/keltic/DVBT2

#keltic ISDBT
cp -r $target_path/keltic/DVBS2 $target_path/keltic/ISDBT

fi

if [[ "$1" == "keres" || ! -n "$1" ]]
  then
# chip_family chip dtv_sys board CPU_Freq ddr_size mm_feq frontend_on/off frontend_type demod tuner oad_on/off oad_in_mboot_on/off
#keres dvbt
main keres 7SX7 DVBT_SYS MST200A_D01A CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200A_D01A CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200A_D01A CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200A_D01A CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBT_SYS MST200A_D01A_S CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200A_D01A_S CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200A_D01A_S CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200A_D01A_S CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBT_SYS MST200B_D01A CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200B_D01A CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200B_D01A CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200B_D01A CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBT_SYS MST200B_D01A_S CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200B_D01A_S CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200B_D01A_S CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200B_D01A_S CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBT_SYS MST200C_D01A CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200C_D01A CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200C_D01A CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200C_D01A CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBT_SYS MST200C_D01A_S CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200C_D01A_S CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200C_D01A_S CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBT_SYS MST200C_D01A_S CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

#keres dvbc
main keres 7SX7 DVBC_SYS MST200A_D01A CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200A_D01A CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200A_D01A CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200A_D01A CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBC_SYS MST200A_D01A_S CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200A_D01A_S CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200A_D01A_S CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200A_D01A_S CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBC_SYS MST200B_D01A CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200B_D01A CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200B_D01A CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200B_D01A CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBC_SYS MST200B_D01A_S CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200B_D01A_S CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200B_D01A_S CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200B_D01A_S CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBC_SYS MST200C_D01A CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200C_D01A CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200C_D01A CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200C_D01A CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main keres 7SX7 DVBC_SYS MST200C_D01A_S CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200C_D01A_S CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200C_D01A_S CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main keres 7SX7 DVBC_SYS MST200C_D01A_S CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

#keres dvbs2
main keres 7SX7 DVBS_SYS MST200A_D01A CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200A_D01A CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200A_D01A CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200A_D01A CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main keres 7SX7 DVBS_SYS MST200A_D01A_S CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200A_D01A_S CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200A_D01A_S CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200A_D01A_S CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main keres 7SX7 DVBS_SYS MST200B_D01A CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200B_D01A CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200B_D01A CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200B_D01A CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main keres 7SX7 DVBS_SYS MST200B_D01A_S CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200B_D01A_S CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200B_D01A_S CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200B_D01A_S CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main keres 7SX7 DVBS_SYS MST200C_D01A CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200C_D01A CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200C_D01A CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200C_D01A CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main keres 7SX7 DVBS_SYS MST200C_D01A_S CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200C_D01A_S CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200C_D01A_S CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main keres 7SX7 DVBS_SYS MST200C_D01A_S CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

fi

if [[ "$1" == "kirin" || ! -n "$1" ]]
  then
#kirin dvbt
main kirin 7SX7 DVBT_SYS MST221A_D01A CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221A_D01A CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221A_D01A CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221A_D01A CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBT_SYS MST221A_D01A_S CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221A_D01A_S CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221A_D01A_S CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221A_D01A_S CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBT_SYS MST221B_D01A CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221B_D01A CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221B_D01A CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221B_D01A CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBT_SYS MST221B_D01A_S CPU667 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221B_D01A_S CPU667 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221B_D01A_S CPU1008 128M 1866MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBT_SYS MST221B_D01A_S CPU1008 128M 1600MHZ frontend_on DVBT2 MSB124x MXL608 oad_on oad_in_mboot_on

#kirin dvbc
main kirin 7SX7 DVBC_SYS MST221A_D01A CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221A_D01A CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221A_D01A CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221A_D01A CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBC_SYS MST221A_D01A_S CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221A_D01A_S CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221A_D01A_S CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221A_D01A_S CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBC_SYS MST221B_D01A CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221B_D01A CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221B_D01A CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221B_D01A CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBC_SYS MST221B_D01A_S CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221B_D01A_S CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221B_D01A_S CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221B_D01A_S CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBC_SYS MST221C_D01A_15 CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221C_D01A_15 CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221C_D01A_15 CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221C_D01A_15 CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBC_SYS MST221C_D01A_S_15 CPU667 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221C_D01A_S_15 CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221C_D01A_S_15 CPU1008 128M 1866MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221C_D01A_S_15 CPU1008 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

main kirin 7SX7 DVBC_SYS MST221D_D01A CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on
main kirin 7SX7 DVBC_SYS MST221D_D01A_S CPU667 128M 1600MHZ frontend_on INTERNAL_DVBC KERES_EMBEDDED_DVBC MXL608 oad_on oad_in_mboot_on

#kirin dvbs2
main kirin 7SX7 DVBS_SYS MST221A_D01A CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221A_D01A CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221A_D01A CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221A_D01A CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main kirin 7SX7 DVBS_SYS MST221A_D01A_S CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221A_D01A_S CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221A_D01A_S CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221A_D01A_S CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main kirin 7SX7 DVBS_SYS MST221B_D01A CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221B_D01A CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221B_D01A CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221B_D01A CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main kirin 7SX7 DVBS_SYS MST221B_D01A_S CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221B_D01A_S CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221B_D01A_S CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221B_D01A_S CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main kirin 7SX7 DVBS_SYS MST221C_D01A_15 CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221C_D01A_15 CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221C_D01A_15 CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221C_D01A_15 CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

main kirin 7SX7 DVBS_SYS MST221C_D01A_S_15 CPU667 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221C_D01A_S_15 CPU667 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221C_D01A_S_15 CPU1008 128M 1866MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on
main kirin 7SX7 DVBS_SYS MST221C_D01A_S_15 CPU1008 128M 1600MHZ frontend_on DVBS2 MSB1245 AV2012 oad_on oad_in_mboot_on

fi

if [[ "$1" == "kris" || ! -n "$1" ]]
  then
#kris
main kris 7SX7 MST227A_D01A CPU667 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX7 MST227A_D01A CPU667 128M 1600MHZ frontend_off DVBC oad_off
main kris 7SX7 MST227A_D01A CPU1008 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX7 MST227A_D01A CPU1008 128M 1600MHZ frontend_off DVBC oad_off

main kris 7SX5 MST227B_D01A CPU667 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX5 MST227B_D01A CPU667 128M 1600MHZ frontend_off DVBC oad_off
main kris 7SX5 MST227B_D01A CPU1008 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX5 MST227B_D01A CPU1008 128M 1600MHZ frontend_off DVBC oad_off

main kris 7SX5 MST227B_D01A_S CPU667 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX5 MST227B_D01A_S CPU667 128M 1600MHZ frontend_off DVBC oad_off
main kris 7SX5 MST227B_D01A_S CPU1008 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX5 MST227B_D01A_S CPU1008 128M 1600MHZ frontend_off DVBC oad_off

main kris 7SX3 MST227C_D01A CPU667 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX3 MST227C_D01A CPU667 128M 1600MHZ frontend_off DVBC oad_off
main kris 7SX3 MST227C_D01A CPU1008 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX3 MST227C_D01A CPU1008 128M 1600MHZ frontend_off DVBC oad_off

main kris 7SX3 MST227D_D01A CPU667 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX3 MST227D_D01A CPU667 128M 1600MHZ frontend_off DVBC oad_off
main kris 7SX3 MST227D_D01A CPU1008 128M 1866MHZ frontend_off DVBC oad_off
main kris 7SX3 MST227D_D01A CPU1008 128M 1600MHZ frontend_off DVBC oad_off

#kris internal_dvbs
cp -r $target_path/kris/DVBC $target_path/kris/INTERNAL_DVBS

#kris dvbt2
cp -r $target_path/kris/DVBC $target_path/kris/DVBT2

#kris dvbs2
cp -r $target_path/kris/DVBC $target_path/kris/DVBS2

fi


if [[ "$1" == "kriti" || ! -n "$1" ]]
  then
#chip_family board ddr_size mm_feq frontend_on/off frontend_type demod tuner oad_on/off oad_in_mboot_on/off
#kriti internal_dvbt2
main kriti MST204A_D01A_DDR2 64M 1333MHZ frontend_on INTERNAL_DVBT2 KRITI_EMBEDDED_DVBT2 MXL608 oad_on oad_in_mboot_on
main kriti MST204A_D01A_S_DDR2 64M 1333MHZ frontend_on INTERNAL_DVBT2 KRITI_EMBEDDED_DVBT2 MXL608 oad_on oad_in_mboot_on

main kriti MST204A_D01A_DDR3 128M 1600MHZ frontend_on INTERNAL_DVBT2 KRITI_EMBEDDED_DVBT2 MXL608 oad_on oad_in_mboot_on
main kriti MST204A_D01A_S_DDR3 128M 1600MHZ frontend_on INTERNAL_DVBT2 KRITI_EMBEDDED_DVBT2 MXL608 oad_on oad_in_mboot_on

#kriti internal_dvbt
main kriti MST204A_D01A_DDR2 64M 1333MHZ frontend_on INTERNAL_DVBT KRITI_EMBEDDED_DVBT MXL608 oad_on oad_in_mboot_on
main kriti MST204A_D01A_S_DDR2 64M 1333MHZ frontend_on INTERNAL_DVBT KRITI_EMBEDDED_DVBT MXL608 oad_on oad_in_mboot_on

main kriti MST204A_D01A_DDR3 128M 1600MHZ frontend_on INTERNAL_DVBT KRITI_EMBEDDED_DVBT MXL608 oad_on oad_in_mboot_on
main kriti MST204A_D01A_S_DDR3 128M 1600MHZ frontend_on INTERNAL_DVBT KRITI_EMBEDDED_DVBT MXL608 oad_on oad_in_mboot_on

#kriti DVBS2
main kriti MST204A_D01A_DDR2 64M 1333MHZ frontend_off DVBS2 oad_off oad_in_mboot_off

fi


