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
    gnomonAbstractVisualizationImage.h
    gnomonAbstractVisualizationImage
    gnomonVisualizationImage
    gnomonVisualizationImage.h
    gnomonVisualizationImageChannelBlending
    gnomonVisualizationImageChannelBlending.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractVisualizationImage.cpp
    gnomonVisualizationImage.cpp
    gnomonVisualizationImageChannelBlending.cpp)

######################################################################
### CMakeLists.txt ends here
