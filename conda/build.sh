#!/bin/bash

if [[ "$c_compiler" == "gcc" ]]; then
  export PATH="${PATH}:${BUILD_PREFIX}/${HOST}/sysroot/usr/lib:${BUILD_PREFIX}/${HOST}/sysroot/usr/include"
fi

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="${PREFIX}" -DCMAKE_PREFIX_PATH="${PREFIX}" -DCMAKE_INSTALL_LIBDIR=lib

make -j${CPU_COUNT}
make install
if [ `uname` == Darwin ]
then
    cp $RECIPE_DIR/gnomon $PREFIX/bin
fi
