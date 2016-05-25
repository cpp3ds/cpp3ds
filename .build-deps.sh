#!/bin/sh
set -ex

export CC=gcc-6
export CXX=g++-6
export DEVKITPRO=${HOME}/devkitPro
export DEVKITARM=${DEVKITPRO}/devkitARM
export PATH=${DEVKITARM}/bin/:${PATH}
export CTRULIB=${DEVKITPRO}/libctru
export PORTLIBS=${DEVKITPRO}/portlibs/armv6k
g++-6 -v
# Install devkitARM
if [ "$APPVEYOR" = "True" ]; then
	appveyor DownloadFile http://sourceforge.net/projects/devkitpro/files/devkitARM/devkitARM_r44/devkitARM_r44-win32.exe
	7z x devkitARM_r44-win32.exe -o$DEVKITPRO
else
	wget https://sourceforge.net/projects/devkitpro/files/devkitARM/devkitARM_r45/devkitARM_r45-x86_64-linux.tar.bz2
	mkdir $DEVKITPRO
	tar -xaf devkitARM_r45-x86_64-linux.tar.bz2 -C $DEVKITPRO
fi

# Download portlibs used by cpp3ds
wget https://github.com/cpp3ds/3ds_portlibs/releases/download/r1/portlibs-3ds-r1.tar.xz
tar -xaf portlibs-3ds-r1.tar.xz
ln -s $(pwd)/portlibs $DEVKITPRO/portlibs

# Download latest 3ds-tools
wget https://github.com/cpp3ds/3ds-tools/releases/download/r4/3ds-tools-linux-r4.tar.gz
tar -xaf 3ds-tools-linux-r4.tar.gz
cp 3ds-tools/* $DEVKITARM/bin

# Get latest ctrulib
git clone https://github.com/smealum/ctrulib.git
cd ctrulib/libctru && make -j4 install && cd -

# Build citro3d
git clone https://github.com/fincs/citro3d.git
cd citro3d && make -j4 install && cd -
