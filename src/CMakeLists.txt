set(layer_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

## #################################################################
## Input
## #################################################################

add_subdirectory(gnomon)
add_subdirectory(gnomonCore)
add_subdirectory(gnomonPipeline)

if(GNOMON_ENABLE_VISU)
  add_subdirectory(gnomonVisualization)
  add_subdirectory(gnomonWorkspace)
endif(GNOMON_ENABLE_VISU)

add_subdirectory(logger)

## #################################################################
## Targets
## #################################################################

export(EXPORT  ${LAYER_TARGETS}
  FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

######################################################################
### CMakeLists.txt ends here
