#!/bin/sh
set -ex

export CC=gcc-4.9
export CXX=g++-4.9
export DEVKITPRO=${HOME}/devkitPro
export DEVKITARM=${DEVKITPRO}/devkitARM
export PATH=${DEVKITARM}/bin/:${PATH}
export CTRULIB=${DEVKITPRO}/libctru
export GL3DS=${HOME}/gl3ds
export PORTLIBS=${DEVKITPRO}/portlibs/armv6k

# Install devkitARM
if [ "$APPVEYOR" = "True" ]; then
	appveyor DownloadFile http://sourceforge.net/projects/devkitpro/files/devkitARM/devkitARM_r44/devkitARM_r44-win32.exe
	7z x devkitARM_r44-win32.exe -o$DEVKITPRO
else
	wget http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitARMupdate.pl
	perl devkitARMupdate.pl
fi

# Download portlibs used by cpp3ds
wget https://github.com/cpp3ds/3ds_portlibs/releases/download/r1/portlibs-3ds-r1.tar.xz
tar -xaf portlibs-3ds-r1.tar.xz
ln -s $(pwd)/portlibs $DEVKITPRO/portlibs

# Download latest 3ds-tools
wget https://github.com/cpp3ds/3ds-tools/releases/download/r3/3ds-tools-linux-r3.tar.gz
tar -xaf 3ds-tools-linux-r3.tar.gz
cp 3ds-tools/* $DEVKITARM/bin

# Get latest ctrulib
git clone https://github.com/Cruel/ctrulib.git
cd ctrulib/libctru && git checkout great-refactor && make -j4 install && cd -

# Build gl3ds
git clone https://github.com/cpp3ds/gl3ds.git ${GL3DS}
cd ${GL3DS} && make -j4 && cd -
