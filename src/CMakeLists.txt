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
add_subdirectory(gnomonPipeline)
add_subdirectory(gnomonVisualization)
add_subdirectory(gnomonWorkspace)

if(GNOMON_ENABLE_WIDGETS)
  add_subdirectory(gnomonTheme)
  add_subdirectory(gnomonWidgets)
endif(GNOMON_ENABLE_WIDGETS)

## #################################################################
## Targets
## #################################################################

export(EXPORT  ${LAYER_TARGETS}
  FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

######################################################################
### CMakeLists.txt ends here
