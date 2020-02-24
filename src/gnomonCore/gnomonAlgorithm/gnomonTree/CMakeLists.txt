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
    gnomonAbstractTreeConstructor.cpp
    gnomonAbstractTreeFromLString.cpp
    gnomonAbstractTreeReader.cpp
    gnomonAbstractTreeTransform.cpp
    gnomonAbstractTreeWriter.cpp)


######################################################################
### CMakeLists.txt ends here
