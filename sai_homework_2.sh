#!/bin/bash

cd stub

./autogen.sh
./configure --prefix=/usr
make && sudo make install

cd ..

gcc -o final_test -I ./stub/inc/ -I /usr/include/sai -L /usr/lib test_final.c -lsai
./final_test