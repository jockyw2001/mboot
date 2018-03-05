#!/bin/sh

function build()
{
    n=$1
    conf=$2
    src=bin/flash_with_uboot.bin
    dst=${conf}_flash.bin
    PREFIX="\x1b[1;36m[MAKETEST ${n}]\x1b[0m"

    echo -e $PREFIX "Test config file '$conf'"

    echo -e $PREFIX " distclean"
    make distclean || exit
    cp $conf .config
    echo -e $PREFIX " oldconfig"
    make silentoldconfig || exit
    echo -e $PREFIX " make"
    make || exit
    echo -e $PREFIX "Test OK"
    echo -e $PREFIX "Copy $src to $dst"
    cp -f $src $dst
    echo '=================================================='
}

S=$(($1 + 0))
L=$(($2 + 0))

if [ $L -le 0 ]; then
    L=10000
fi

E=$(($S + $L))

CONF=./configs/test_uboot

n=0
for conf in `find $CONF -name '*.conf' | sort`; do
    echo $n: $conf
    n=$(($n + 1))
done

n=0
for conf in `find $CONF -name '*.conf' | sort`; do
    if [ $n -ge $S -a $n -lt $E ]; then
        build $n $conf
    fi
    n=$(($n + 1))
done
