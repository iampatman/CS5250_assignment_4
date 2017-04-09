#!/bin/bash


sudo rmnod mydriver.ko
make
sudo insmod mydriver.ko
