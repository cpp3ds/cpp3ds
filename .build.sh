#!/bin/sh
set -ex

mkdir build && cd build
cmake -DBUILD_EMULATOR=OFF -DENABLE_OGG=ON -DBUILD_EXAMPLES=OFF ..
make -j4
cd ..
