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
    gnomonAbstractImageVtkVisualization.h
    gnomonAbstractImageVtkVisualization
    gnomonImageVtkVisualization
    gnomonImageVtkVisualization.h
    gnomonImageVtkVisualizationChannelBlending
    gnomonImageVtkVisualizationChannelBlending.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractImageVtkVisualization.cpp
    gnomonImageVtkVisualization.cpp
    gnomonImageVtkVisualizationChannelBlending.cpp)

######################################################################
### CMakeLists.txt ends here
