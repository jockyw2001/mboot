#!/usr/bin/python
review_target = "k6"
review_arch = "aarch64"
review_cpu = "ca53"
import os
import sys


file = open('.config')
review_need="n"
while True :
    file_string = file.readline()
    if file_string =='': break
    # print(file_string)
    str_array = file_string.split()
    #print(str_array)
    #print(len(str_array))
    if (len(str_array)==1):
        #print("yes")
        chip_str = "CONFIG_MSTAR_CHIP_NAME="
        cmp_len = len(chip_str)
        cmp_str = str_array[0][0:cmp_len]
        cmp_resuly = cmp(chip_str,cmp_str)
        if (cmp_resuly == 0):
            str_len = len(str_array[0])
            review_target= str_array[0][cmp_len+1:str_len-1]
            #print(review_target)
        chip_str = "CONFIG_SBOOT_CROSS_COMPILER_PREFIX="
        cmp_len = len(chip_str)
        cmp_str = str_array[0][0:cmp_len]
        cmp_resuly = cmp(chip_str,cmp_str)
        if (cmp_resuly == 0):
            str_len = len(str_array[0])
            review_arch= str_array[0][cmp_len+1:cmp_len+8]
            #print(review_arch)
        chip_str = "CONFIG_ARCH_"
        cmp_len = len(chip_str)
        cmp_str = str_array[0][0:cmp_len]
        cmp_resuly = cmp(chip_str,cmp_str)
        if (cmp_resuly == 0):
            str_len = len(str_array[0])
            review_cpu= str_array[0][cmp_len:str_len-2]
            review_cpu=review_cpu.lower()
        chip_str = "CONFIG_MSTAR_RELEASE_W_OTP="
        cmp_len = len(chip_str)
        cmp_str = str_array[0][0:cmp_len]
        cmp_resuly = cmp(chip_str,cmp_str)
        if (cmp_resuly == 0):
            str_len = len(str_array[0])
            print(str_array[0])
            review_need= str_array[0][str_len-1:str_len]

            #break
            #print(review_arch)
        #print("end")

file.close()

print(review_arch)
print(review_target)
print(review_cpu)
print(review_need)
 
if (review_need == 'y'):
    print("start review....")
    for filename in os.listdir("chip/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "chip/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

            
    for filename in os.listdir("board/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "board/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

            
    for filename in os.listdir("cpu/"):
        if review_arch == filename:
            print  filename
        else:
            full_path = "cpu/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

    for filename in os.listdir("drivers/dscmb2/src/hal/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "drivers/dscmb2/src/hal/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

    for filename in os.listdir("drivers/cipher/src/hal/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "drivers/cipher/src/hal/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

    for filename in os.listdir("drivers/spinor/hal/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "drivers/spinor/hal/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

    for filename in os.listdir("drivers/aesdma/src/hal/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "drivers/aesdma/src/hal/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)

    for filename in os.listdir("drivers/spinand/hal/"):
        if review_target == filename:
            print  filename
        else:
            full_path = "drivers/spinand/hal/"+filename
            command = "rm -rf %s"
            command = command % full_path
            result = os.system(command)
# do not review cpu_rom (FTA only)
    fta_folder = "cpu_rom"
    command = "rm -rf %s"
    command = command % fta_folder
    result = os.system(command)

        
# Ree file list remove
    list1 = ['bootrom_c.c', 'bootrom_miu.c','chip_misc_rom.c','boot.S','bootrom.S','bootseal.c'];

    for check_out in list1:
        #print  check_out
        full_path = "cpu/"+review_arch+"/"+review_cpu+"/"+check_out
        if os.path.isfile(full_path):
            #print  "find out"
            command = "rm -f %s"
            command = command % full_path
            result = os.system(command)

#    else:
#        print  "nothing"

    for check_out in list1:
        #print  check_out
        full_path = "cpu/"+review_arch+"/"+check_out
        if os.path.isfile(full_path):
            #print  "find out"
            command = "rm -f %s"
            command = command % full_path
            result = os.system(command)

#    else:
#        print  "nothing"
                

    for check_out in list1:
        full_path = "chip/"+review_target+"/"+check_out
        #print  full_path
        if os.path.isfile(full_path):
            #print  "find out"
            command = "rm -f %s"
            command = command % full_path
            result = os.system(command)

#    else:
#        print  "nothing"
            
#endif of file
        
