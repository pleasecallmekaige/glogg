#!/bin/bash
export BOOSTDIR=/d/Lib/boost_1_75_0
export QT_BIN_PATH=/e/software/normal/Qt5/5.12.9/mingw73_32/bin
export QT_MINGW32_BIN_PATH=/E/software/normal/Qt5/Tools/mingw730_32/bin
export PATH=$QT_MINGW32_BIN_PATH:$QT_BIN_PATH:$PATH
export CROSS_COMPILE=i686-w64-mingw32-
export QMAKE=$QT_BIN_PATH/qmake
export RELEASE_PATH=./release

BOOSTDIR=/d/Lib/boost_1_75_0

if [ -z "$VERSION" ]; then
    echo "Building default version"
    qmake glogg.pro -spec win32-g++ -r CONFIG+=release BOOST_PATH=$BOOSTDIR
else
    echo "Building version $VERSION"
    qmake glogg.pro -spec win32-g++ -r CONFIG+=release BOOST_PATH=$BOOSTDIR VERSION="$VERSION"
fi
mingw32-make

cp $QT_MINGW32_BIN_PATH/{Qt5Core,Qt5Gui,Qt5Widgets,Qt5Network,libgcc_s_dw2-1,libwinpthread-1,libstdc++-6}.dll $RELEASE_PATH/

if [ -z "$VERSION" ]; then
    VERSION=`git describe`;
fi
# echo Generating installer for glogg-$VERSION
# /cygdrive/c/Program\ Files/NSIS/makensis -DVERSION=$VERSION glogg.nsi
