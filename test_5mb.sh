#!/bin/sh

gcc -o test test_copy_5mb.c
./test
cat /dev/four
