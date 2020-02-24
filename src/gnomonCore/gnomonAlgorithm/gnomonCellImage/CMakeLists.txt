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
    gnomonAbstractCellImageConstructor
    gnomonAbstractCellImageConstructor.h
    gnomonAbstractCellImageFilter
    gnomonAbstractCellImageFilter.h
    gnomonAbstractCellImageFromImage
    gnomonAbstractCellImageFromImage.h
    gnomonAbstractCellImageQuantification
    gnomonAbstractCellImageQuantification.h
    gnomonAbstractCellImageReader
    gnomonAbstractCellImageReader.h
    gnomonAbstractCellImageTracking
    gnomonAbstractCellImageTracking.h
    gnomonAbstractCellImageWriter
    gnomonAbstractCellImageWriter.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractCellImageConstructor.cpp
    gnomonAbstractCellImageFilter.cpp
    gnomonAbstractCellImageFromImage.cpp
    gnomonAbstractCellImageQuantification.cpp
    gnomonAbstractCellImageReader.cpp
    gnomonAbstractCellImageTracking.cpp
    gnomonAbstractCellImageWriter.cpp)


######################################################################
### CMakeLists.txt ends here
