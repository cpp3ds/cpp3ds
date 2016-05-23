#!/bin/sh
set -ex

mkdir -p cpp3ds/bin/
cp $DEVKITARM/bin/makerom cpp3ds/bin/
cp $DEVKITARM/bin/3dsxtool cpp3ds/bin/
cp $DEVKITARM/bin/bannertool cpp3ds/bin/
cp $DEVKITARM/bin/nihstro-assemble cpp3ds/bin/
cp -r $PORTLIBS/lib/ cpp3ds/
cp -r $PORTLIBS/include/ cpp3ds/
cp -r build/lib/ cpp3ds/
cp -r include/ cpp3ds/
cp -r cmake/ cpp3ds/
cp -r scripts/ cpp3ds/

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
	export RELEASE_FILENAME=cpp3ds-$TRAVIS_OS_NAME-$TRAVIS_TAG.tar.xz
	tar -cJvf $RELEASE_FILENAME cpp3ds
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
	echo "No OSX support yet"
else
	export RELEASE_FILENAME=cpp3ds-windows-$APPVEYOR_REPO_TAG_NAME.tar.xz
	7z a $RELEASE_FILENAME cpp3ds
fi
