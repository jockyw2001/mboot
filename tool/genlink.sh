#!/bin/bash
CHIP=$1

if [ "$1" == "" ]; then
    echo "Warning, please input the argument!"
	echo "ex: sh genlink.sh a3"
    exit 1
fi

test -d ../u-boot-2011.06/include/mstar
if [ $? != 0 ]
then
	mkdir ../u-boot-2011.06/include/mstar
	echo "mkdir ../u-boot-2011.06/include/mstar"
fi

test -d ../u-boot-2011.06/include/mstar/core
if [ $? != 0 ]
then
	mkdir ../u-boot-2011.06/include/mstar/core
	echo "mkdir ../u-boot-2011.06/include/mstar/core"
fi

test -L ../u-boot-2011.06/lib/mstar
if [ $? != 0 ]
then
# link MstarCore
if [ "$2" == "" ]; then
ln -fs ../../MstarCore/bsp/$CHIP/lib/ ../u-boot-2011.06/lib/mstar
else
ln -fs ../../MstarCore/bsp/$CHIP/lib_mini/ ../u-boot-2011.06/lib/mstar
fi
ls ../MstarCore/bsp/$CHIP/include/* | xargs -I FILES ln -s ../../../FILES ../u-boot-2011.06/include/mstar/core/
ls ../MstarCore/include/* | xargs -I FILES ln -s ../../../FILES ../u-boot-2011.06/include/mstar/core/
ln -fs ../../MstarCore/src/hal/$CHIP/board/ ../u-boot-2011.06/board/mstar
ln -fs ../../MstarCore/src/drivers/ ../u-boot-2011.06/drivers/mstar
ln -fs ../../../MstarCore/src/hal/$CHIP/$CHIP.h ../u-boot-2011.06/include/configs/$CHIP.h
ln -fs ../../../../MstarCore/src/hal/$CHIP/cpu ../u-boot-2011.06/arch/arm/lib/mstar

echo "-- Construct symbolic link OK --"
else
echo "-- already link --"
fi
