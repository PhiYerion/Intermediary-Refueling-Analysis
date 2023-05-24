#!/bin/bash
set -x

ld -L/home/user/Projects/EML2-to-Mars/src/lib -lmpfr
git submodule init
git submodule update
mkdir build
cd build
cmake .. 
cmake --build .
