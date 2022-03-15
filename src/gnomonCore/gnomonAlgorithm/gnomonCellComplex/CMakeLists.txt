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
    gnomonAbstractCellComplexAdapter
    gnomonAbstractCellComplexAdapter.h
    gnomonAbstractCellComplexConstructor
    gnomonAbstractCellComplexConstructor.h
    gnomonAbstractCellComplexFromCellImage
    gnomonAbstractCellComplexFromCellImage.h
    gnomonAbstractCellComplexReader
    gnomonAbstractCellComplexReader.h
    gnomonAbstractCellComplexWriter
    gnomonAbstractCellComplexWriter.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractCellComplexAdapter.cpp
    gnomonAbstractCellComplexConstructor.cpp
    gnomonAbstractCellComplexFromCellImage.cpp
    gnomonAbstractCellComplexReader.cpp
    gnomonAbstractCellComplexWriter.cpp)


######################################################################
### CMakeLists.txt ends here
