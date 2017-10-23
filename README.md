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
