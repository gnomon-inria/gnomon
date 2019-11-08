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
    gnomonAbstractTreeFromLString
    gnomonAbstractTreeFromLString.h
    gnomonAbstractTreeReader
    gnomonAbstractTreeReader.h
    gnomonAbstractTreeTransform
    gnomonAbstractTreeTransform.h
    gnomonAbstractTreeWriter
    gnomonAbstractTreeWriter.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractTreeFromLString.cpp
    gnomonAbstractTreeReader.cpp
    gnomonAbstractTreeTransform.cpp
    gnomonAbstractTreeWriter.cpp)


######################################################################
### CMakeLists.txt ends here
