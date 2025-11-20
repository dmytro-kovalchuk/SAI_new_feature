#!/bin/bash

cd stub

./autogen.sh
./configure --prefix=/usr
make && sudo make install

cd ..

gcc -o lag_test -I ./stub/inc/ -I /usr/include/sai -L /usr/lib test_lag_api.c -lsai
./lag_test