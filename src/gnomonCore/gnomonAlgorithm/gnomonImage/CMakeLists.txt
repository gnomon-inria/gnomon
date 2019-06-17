## Version: $Id$
##
######################################################################
##
### Commentary:
##
######################################################################
##
### Change Log:
##
######################################################################
##
### Code:


## #################################################################
## Sources
## #################################################################


ADD_GNOMON_SUBDIRECTORY_HEADERS(
    gnomonAbstractImageFilter
    gnomonAbstractImageFilter.h
    gnomonAbstractImageFusion
    gnomonAbstractImageFusion.h
    gnomonAbstractImageReader
    gnomonAbstractImageReader.h
    gnomonAbstractImageRegistration
    gnomonAbstractImageRegistration.h
    gnomonAbstractImageWriter
    gnomonAbstractImageWriter.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractImageFilter.cpp
    gnomonAbstractImageFusion.cpp
    gnomonAbstractImageReader.cpp
    gnomonAbstractImageRegistration.cpp
    gnomonAbstractImageWriter.cpp)


######################################################################
### CMakeLists.txt ends here
