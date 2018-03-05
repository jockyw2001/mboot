#!/bin/bash

if [ ! $1 ]; then
    echo "please specify directory"
    exit 1
fi

if [ ! $2 ]; then
    CONFIGS=`ls -a ${1}.config*`
else
    echo ${#2}
    if [ ${#2} -lt 2 ]; then
        echo "wildcard needs at least 2 characters"
        exit 1
    fi
    CONFIGS=`ls -a ${1}.config*${2}* 2> /dev/null`
#    echo ${CONFIGS}
fi

echo "build all configs under $1 ..."
if [ ${#CONFIGS} -eq 0 ]; then
    echo "No configs found"
    exit 1
else
    echo "Number of configs found containing '${2}': `echo ${CONFIGS} | wc -w`"
fi

for config in ${CONFIGS}; do
    BOOT_BIN=`echo ${config} | sed 's/.*\.config\.//g' | sed 's/\./_/g'`
    mkdir -p bin/${BOOT_BIN}
    echo -e "start \033[4;34;40m${config}\033[0m"
    cp $config .config
    make oldconfig
    make clean
    make
    if [ $? != 0 ]; then
        echo -e " ${config} \033[1;31;40mbuild failed!\033[0m"
        exit 1
    else
        echo -e " ${config} \033[1;32;40mbuild OK!\033[0m"
    fi
    cp bin/bfn_* bin/${BOOT_BIN}/
    cp bin/flash_with_uboot.bin bin/${BOOT_BIN}
done 
