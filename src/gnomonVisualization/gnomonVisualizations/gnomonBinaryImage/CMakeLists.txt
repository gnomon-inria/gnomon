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
        gnomonAbstractVisualizationBinaryImage.h
        gnomonAbstractVisualizationBinaryImage
        gnomonVisualizationBinaryImage
        gnomonVisualizationBinaryImage.h
        gnomonVisualizationBinaryImageChannelBlending
        gnomonVisualizationBinaryImageChannelBlending.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
        gnomonAbstractVisualizationBinaryImage.cpp
        gnomonVisualizationBinaryImage.cpp
        gnomonVisualizationBinaryImageChannelBlending.cpp)

######################################################################
### CMakeLists.txt ends here
