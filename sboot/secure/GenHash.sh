#!/bin/sh
#############################################################################################
### $1 target file name
### $2 output has file name
###
#############################################################################################
./cryptest.exe m $1|grep 'SHA-256:'|sed 's/SHA-256: //'> hashStr
unix2dos hashStr
./str2hex.exe ./hashStr ./temphash
./Cute.exe ./temphash 0 0x1F
cat ./_temp.bin > $2
rm _temp.bin
rm hashStr
rm ./temphash