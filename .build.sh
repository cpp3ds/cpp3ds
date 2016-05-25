#!/bin/sh
set -ex

mkdir build && cd build
cmake -DCMAKE_RANLIB=/usr/bin/gcc-ranlib-6 -DCMAKE_AR=/usr/bin/gcc-ar-6 -DBUILD_EMULATOR=ON -DENABLE_OGG=ON -DBUILD_EXAMPLES=OFF ..
make -j4 cpp3ds-emu VERBOSE=1
nm src/emu3ds/CMakeFiles/cpp3ds-emu.dir/Graphics/Texture.cpp.o | c++filt | grep basic_string
nm lib/libcpp3ds-emu.a | c++filt | grep saveToFile
cd ..
