#!/bin/sh

gcc -o test test_llseek.c
./test
cat /dev/four
