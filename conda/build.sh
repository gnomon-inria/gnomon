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

make -j${CPU_COUNT-2}
make install