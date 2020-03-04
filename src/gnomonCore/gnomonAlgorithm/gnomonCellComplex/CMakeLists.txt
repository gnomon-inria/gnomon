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
    gnomonAbstractCellComplexConstructor
    gnomonAbstractCellComplexConstructor.h
    gnomonAbstractCellComplexFromCellImage
    gnomonAbstractCellComplexFromCellImage.h
    gnomonAbstractCellComplexReader
    gnomonAbstractCellComplexReader.h
    gnomonAbstractCellComplexWriter
    gnomonAbstractCellComplexWriter.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractCellComplexConstructor.cpp
    gnomonAbstractCellComplexFromCellImage.cpp
    gnomonAbstractCellComplexReader.cpp
    gnomonAbstractCellComplexWriter.cpp)


######################################################################
### CMakeLists.txt ends here
