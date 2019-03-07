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

project(gnomonVisualizationCellImageVolume)

## ###################################################################
## Build rules
## ###################################################################

add_definitions(-DQT_PLUGIN)

add_library(${PROJECT_NAME} SHARED
    gnomonVisualizationCellImageVolumePlugin.cpp
    gnomonVisualizationCellImageVolumePlugin.h)

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)

target_link_libraries(${PROJECT_NAME} gnomonCore)
target_link_libraries(${PROJECT_NAME} gnomonWidgets)
target_link_libraries(${PROJECT_NAME} gnomonVisualization)

target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkLog)


## #################################################################
## Install rules
## #################################################################

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION plugins/${DTK_CURRENT_LAYER}
  LIBRARY DESTINATION plugins/${DTK_CURRENT_LAYER}
  ARCHIVE DESTINATION plugins/${DTK_CURRENT_LAYER})

######################################################################
### CMakeLists.txt ends here
