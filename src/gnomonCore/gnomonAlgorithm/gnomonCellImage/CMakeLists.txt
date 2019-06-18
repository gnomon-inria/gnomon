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
    gnomonAbstractCellImageFilter
    gnomonAbstractCellImageFilter.h
    gnomonAbstractCellImageFromImage
    gnomonAbstractCellImageFromImage.h
    gnomonAbstractCellImageQuantification
    gnomonAbstractCellImageQuantification.h
    gnomonAbstractCellImageReader
    gnomonAbstractCellImageReader.h
    gnomonAbstractCellImageWriter
    gnomonAbstractCellImageWriter.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractCellImageFilter.cpp
    gnomonAbstractCellImageFromImage.cpp
    gnomonAbstractCellImageQuantification.cpp
    gnomonAbstractCellImageReader.cpp
    gnomonAbstractCellImageWriter.cpp)


######################################################################
### CMakeLists.txt ends here
