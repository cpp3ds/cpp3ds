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

# Build makerom (commit 4ef27aca30a)
git clone https://github.com/profi200/Project_CTR.git
cd Project_CTR/makerom && git reset --hard 4ef27aca30a && make -j4 && cp makerom $DEVKITARM/bin && cd -

# Build latest 3dsxtool
git clone https://github.com/devkitPro/3dstools.git
cd 3dstools && ./autogen.sh && ./configure && make -j4 && cp 3dsxtool $DEVKITARM/bin && cd -

# Build latest bannertool
git clone https://github.com/Steveice10/bannertool.git
cd bannertool && cmake . && make -j4 && cp bannertool $DEVKITARM/bin && cd -

# Build nihstro
git clone https://github.com/neobrain/nihstro.git
cd nihstro && cmake . && make -j4 && cp nihstro-assemble $DEVKITARM/bin && cd -

# Get latest ctrulib
git clone https://github.com/smealum/ctrulib.git
cd ctrulib/libctru && make -j4 install && cd -

# Build gl3ds
git clone https://github.com/cpp3ds/gl3ds.git ${GL3DS}
cd ${GL3DS} && make -j4 && cd -
