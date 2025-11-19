#!/bin/bash

gcc -o final_test -I ./stub/inc/ -I /usr/include/sai -L /usr/lib test_final.c -lsai
./final_test