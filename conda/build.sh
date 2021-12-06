#!/bin/bash

if [[ -d build ]]; then
    rm -rf build
fi
mkdir build
cd build

if [[ "$c_compiler" == "gcc" ]]; then
  export PATH="${PATH}:${BUILD_PREFIX}/${HOST}/sysroot/usr/lib:${BUILD_PREFIX}/${HOST}/sysroot/usr/include"
fi

cmake .. \
      -DCMAKE_INSTALL_PREFIX="${PREFIX}" \
      -DCMAKE_PREFIX_PATH="${PREFIX}" \
      -DCMAKE_INSTALL_LIBDIR=lib
      -DGNOMON_ENABLE_WIDGETS=OFF

make -j${CPU_COUNT}
make install

if [ `uname` == Darwin ]
then
    cp $RECIPE_DIR/gnomon $PREFIX/bin
fi
