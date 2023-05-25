#!/bin/bash
set -x
pwd
ls

git submodule init
git submodule update
mkdir build
cd build
cmake .. 
cmake --build .
