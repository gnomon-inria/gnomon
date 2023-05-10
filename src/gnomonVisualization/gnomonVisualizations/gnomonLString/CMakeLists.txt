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
    gnomonAbstractLStringMplVisualization
    gnomonAbstractLStringMplVisualization.h
    gnomonAbstractLStringQmlVisualization
    gnomonAbstractLStringQmlVisualization.h
    gnomonAbstractLStringVtkVisualization
    gnomonAbstractLStringVtkVisualization.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractLStringMplVisualization.cpp
    gnomonAbstractLStringQmlVisualization.cpp
    gnomonAbstractLStringVtkVisualization.cpp)


######################################################################
### CMakeLists.txt ends here
