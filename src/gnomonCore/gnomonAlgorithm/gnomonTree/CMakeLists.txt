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
    gnomonAbstractTreeAdapter
    gnomonAbstractTreeAdapter.h
    gnomonAbstractTreeConstructor
    gnomonAbstractTreeConstructor.h
    gnomonAbstractTreeFromLString
    gnomonAbstractTreeFromLString.h
    gnomonAbstractTreeReader
    gnomonAbstractTreeReader.h
    gnomonAbstractTreeTransform
    gnomonAbstractTreeTransform.h
    gnomonAbstractTreeWriter
    gnomonAbstractTreeWriter.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractTreeAdapter.cpp
    gnomonAbstractTreeConstructor.cpp
    gnomonAbstractTreeFromLString.cpp
    gnomonAbstractTreeReader.cpp
    gnomonAbstractTreeTransform.cpp
    gnomonAbstractTreeWriter.cpp)


######################################################################
### CMakeLists.txt ends here
