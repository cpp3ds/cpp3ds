#!/bin/sh
set -ex

mkdir -p $CPP3DS/bin/
cp $DEVKITARM/bin/makerom $CPP3DS/bin/
cp $DEVKITARM/bin/3dsxtool $CPP3DS/bin/
cp $DEVKITARM/bin/bannertool $CPP3DS/bin/
cp $DEVKITARM/bin/nihstro-assemble $CPP3DS/bin/
cp -r $PORTLIBS/lib/ $CPP3DS
cp -r $PORTLIBS/include/ $CPP3DS
tar -cJvf $1 cpp3ds
