#!/bin/sh
set -e
set -x

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get -qq update
sudo apt-get -qq install libgtest-dev g++-4.9 libsfml-dev qt5
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 90

# Build and install gtest
cd /usr/src/gtest && sudo cmake . && sudo cmake --build . && sudo mv libg* /usr/local/lib/ ; cd -

# Build and install devkitARM
wget http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitARMupdate.pl
sudo perl devkitARMupdate.pl

# Build gl3ds
git clone https://github.com/cpp3ds/gl3ds.git ${GL3DS}
cd ${GL3DS} && make && cd -
