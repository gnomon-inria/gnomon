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
    gnomonAbstractVisualizationCellImage
    gnomonAbstractVisualizationCellImage.h
    # gnomonVisualizationCellImageMarchingCubes
    # gnomonVisualizationCellImageMarchingCubes.h
    gnomonVisualizationCellImageVolume
    gnomonVisualizationCellImageVolume.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractVisualizationCellImage.cpp
    # gnomonVisualizationCellImageMarchingCubes.cpp
    gnomonVisualizationCellImageVolume.cpp)


######################################################################
### CMakeLists.txt ends here
