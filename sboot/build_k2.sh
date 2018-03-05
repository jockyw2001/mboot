cp .config.k2.ddi .config
make oldconfig > /dev/null
make
make clean
make distclean
