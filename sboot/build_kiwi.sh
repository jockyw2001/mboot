#!/bin/bash
# Bash Menu Script Example

boards=(
         "MST239A.D01A"
       )

boards_is_socket=(
         "Y" "N"
       )       
MEM=(
      "128M" "256M" "512M"
   )


for mem_size in "${MEM[@]}"
do 
    for board in "${boards[@]}"
    do
        for socket in "${boards_is_socket[@]}" 
        do        
            make clean
            make distclean
            
           
            if [ ${socket} == "Y" ]
            then 
				cp ./configs/kiwi/DDI_kiwi/.config.BD.${board}.S.CPU900.DDR3.1866.${mem_size}.NAND .config 
            else
				cp ./configs/kiwi/DDI_kiwi/.config.BD.${board}.CPU900.DDR3.1866.${mem_size}.NAND .config 
			
			fi
        
            make oldconfig 
            make clean;make
            if [ ${socket} == "Y" ]
            then 
			   cp bin/mboot.bin ../../../../PERFORCE/THEALE/Uranus/Kiwi/DDI_Package/ddi_pkg_linux/bsp/linux/Kiwi_Mbootbin/BD.${board}.S.CPU900.DDR3.1866.${mem_size}.NAND.tee.bin
			   echo "BD.${board}.S.CPU756.${mem_size}.1866.NAND.tee.bin for Linux completed!"
    
            else
			   cp bin/mboot.bin ../../../../PERFORCE/THEALE/Uranus/Kiwi/DDI_Package/ddi_pkg_linux/bsp/linux/Kiwi_Mbootbin/BD.${board}.CPU900.DDR3.1866.${mem_size}.NAND.tee.bin
			   echo "BD.${board}.CPU756.${mem_size}.1866.NAND.tee.bin for Linux completed!"
            fi
            
        done
    done
done
