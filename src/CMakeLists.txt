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
## Input
## #################################################################

add_subdirectory(gnomon)
add_subdirectory(gnomonCore)
# add_subdirectory(gnomonStyle)
# add_subdirectory(gnomonFonts)
# add_subdirectory(gnomonVisualization)
# add_subdirectory(gnomonWidgets)
# add_subdirectory(gnomonWorkspace)

## #################################################################
## Targets
## #################################################################

export(EXPORT layer-targets FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

######################################################################
### CMakeLists.txt ends here
