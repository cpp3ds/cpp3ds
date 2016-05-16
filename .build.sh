#!/bin/sh
set -ex

#wget https://github.com/SFML/SFML/archive/2.3.2.tar.gz
#tar xaf 2.3.2.tar.gz
#export CXXFLAGS=-I$(pwd)/SFML-2.3.2/include

mkdir build && cd build
cmake -DBUILD_EMULATOR=ON -DENABLE_OGG=ON -DBUILD_EXAMPLES=OFF ..
make -j4
cd ..
