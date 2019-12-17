#!/bin/bash

mkdir -p build-binutils
cd build-binutils && ln -s ../scripts/build-binutils.sh .
cd ..

mkdir -p build-gcc
cd build-gcc && ln -s  ../scripts/build-gcc.sh .
cd ..

mkdir -p build-gdb
cd build-gdb && ln -s  ../scripts/build-gdb.sh .
cd ..
