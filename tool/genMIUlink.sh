#!/bin/bash

if [ -d  ../sboot/ ]
then
    ln -sf ../../../../sboot/chip/k5tn/miu_script ./BFN_programmer/k5tn/Zen_Mstvtool_PM51/
    ln -sf ../../../../sboot/chip/kayla/miu_script ./BFN_programmer/kayla/Zen_Mstvtool_PM51/

    ln -sf ../../../../sboot/chip/k6/miu_script ./BFN_programmer/k6/Zen_Mstvtool_PM51/
    ln -sf ../../../../sboot/chip/k6/miu_script ./BFE_programmer/k6/Zen_Mstvtool_PM51/

    ln -sf ../../../../sboot/chip/k6lite/miu_script ./BFN_programmer/k6lite/Zen_Mstvtool_PM51/
    ln -sf ../../../../sboot/chip/k6lite/miu_script ./BFE_programmer/k6lite/Zen_Mstvtool_PM51/

    echo "-- Construct symbolic link OK --"
else
    echo "-- Can't link sboot/chip/{chip}/miu_script, please git clode /mstar/stb_sboot to stb_mboot/sboot --"
fi
