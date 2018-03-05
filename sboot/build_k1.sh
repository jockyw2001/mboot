cp .config.k1.DDI .config
make oldconfig > /dev/null
make
make clean
make distclean
