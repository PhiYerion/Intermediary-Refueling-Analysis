#!/bin/bash
set -x

git submodule init
git submodule update
mkdir build
cd build
cmake .. 
cmake --build .
