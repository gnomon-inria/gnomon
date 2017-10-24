# Tissue

## omero layer

We need to install omero C++ and zeroc-ice

Omero depens on ICE (https://zeroc.com/products/ice)

For some linux flavors, need to recompile from source https://github.com/zeroc-ice/ice

'''
$ git clone -b 3.7 https://github.com/zeroc-ice/ice.git
$ make
$ make install
'''
which will install the libs in /opt/Ice-3.7.0

For macOSX:

'''
$ brew install zeroc-ice/tap/ice
'''

Omero will be installed from the sources: https://docs.openmicroscopy.org/omero/5.1.4/developers/installation.html
and https://docs.openmicroscopy.org/omero/5.1.4/developers/Cpp.html#building-with-cmake-directly

'''
$ git clone https://github.com/openmicroscopy/openmicroscopy.git
$ cd openmicroscopy
$ git submodule update --init
$ mkdir build
$ cmake ..
'''

Sources can also be downloaded from:
https://codeload.github.com/openmicroscopy/openmicroscopy/zip/develop/openmicroscopy-develop.zip
in this case, compilation with cmake does not work.
Prérequisite:
- ice (cf before)
- jre (http://download.oracle.com/otn-pub/java/jdk/9.0.1+11/jre-9.0.1_osx-x64_bin.dmg)
- JDK (http://download.oracle.com/otn-pub/java/jdk/9.0.1+11/jdk-9.0.1_osx-x64_bin.dmg)


Test:
http://downloads.openmicroscopy.org/omero/5.4.0/artifacts/OMERO.insight-5.4.0-ice36-b74-mac.zip
cmake -DCMAKE_INCLUDE_PATH:PATH=/usr/local/opt/ice/include -DCMAKE_LIBRARY_PATH:PATH=/usr/local/opt/ice/lib -DICE_HOME:PATH=/usr/local/opt/ice -Dice-SLICE_DIR:PATH=/usr/local/opt/ice ..
brew install zeroc-ice/tap/ice@3.6
