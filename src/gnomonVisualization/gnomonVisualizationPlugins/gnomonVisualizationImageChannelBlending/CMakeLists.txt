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

project(gnomonVisualizationImageChannelBlending)

## ###################################################################
## Build rules
## ###################################################################

add_definitions(-DQT_PLUGIN)

add_library(${PROJECT_NAME} SHARED
    gnomonVisualizationImageChannelBlendingPlugin.cpp
    gnomonVisualizationImageChannelBlendingPlugin.h)

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)

target_link_libraries(${PROJECT_NAME} gnomonCore)
target_link_libraries(${PROJECT_NAME} gnomonWidgets)
target_link_libraries(${PROJECT_NAME} gnomonVisualization)

target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkLog)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

## #################################################################
## Install rules
## #################################################################

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION plugins/${DTK_CURRENT_LAYER}
  LIBRARY DESTINATION plugins/${DTK_CURRENT_LAYER}
  ARCHIVE DESTINATION plugins/${DTK_CURRENT_LAYER})

######################################################################
### CMakeLists.txt ends here
