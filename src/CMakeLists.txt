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

set(layer_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

## #################################################################
## Input
## #################################################################

add_subdirectory(gnomon)
add_subdirectory(gnomonCore)
add_subdirectory(gnomonVisualization)
add_subdirectory(gnomonWidgets)
add_subdirectory(gnomonWorkspace)
add_subdirectory(gnomonComposer)

## #################################################################
## Targets
## #################################################################

export(EXPORT layer-targets FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

######################################################################
### CMakeLists.txt ends here
