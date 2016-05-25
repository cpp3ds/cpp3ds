#!/bin/sh
set -ex

mkdir build && cd build
cmake -DCMAKE_AR=gcc-ar-5 -DCMAKE_RANLIB=gcc-ranlib-5 -DBUILD_EMULATOR=ON -DENABLE_OGG=ON -DBUILD_EXAMPLES=OFF ..
make -j4
cd ..
