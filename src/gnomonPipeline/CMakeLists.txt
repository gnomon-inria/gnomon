project(gnomonPipeline VERSION ${gnomon_VERSION})

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonPipeline
  gnomonPipeline.h
  gnomonPipelineEdge.h
  gnomonPipelineManager.h
  gnomonPipelineNode.h
  gnomonPipelineNodeAdapter
  gnomonPipelineNodeAdapter.h
  gnomonPipelineNodeAlgorithm
  gnomonPipelineNodeAlgorithm.h
  gnomonPipelineNodeConstructor
  gnomonPipelineNodeConstructor.h
  gnomonPipelineNodeEvolutionModel
  gnomonPipelineNodeEvolutionModel.h
  gnomonPipelineNodeReader
  gnomonPipelineNodeReader.h
  gnomonPipelineNodeTask
  gnomonPipelineNodeTask.h
  gnomonPipelineNodeWriter
  gnomonPipelineNodeWriter.h
  gnomonPipelinePort.h
  gnomonPipelineNodeMorphonet
  gnomonPipelineNodeMorphonet.h)

set(${PROJECT_NAME}_SOURCES
  gnomonPipeline.cpp
  gnomonPipelineEdge.cpp
  gnomonPipelineManager.cpp
  gnomonPipelineNode.cpp
  gnomonPipelineNodeAdapter.cpp
  gnomonPipelineNodeAlgorithm.cpp
  gnomonPipelineNodeConstructor.cpp
  gnomonPipelineNodeEvolutionModel.cpp
  gnomonPipelineNodeReader.cpp
  gnomonPipelineNodeTask.cpp
  gnomonPipelineNodeWriter.cpp
  gnomonPipelinePort.cpp
  gnomonPipelineNodeMorphonet.cpp)


## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Network)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Xml)
if(GNOMON_ENABLE_VISU)
  target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Qml)
endif(GNOMON_ENABLE_VISU)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkCore)
target_link_libraries(${PROJECT_NAME} PUBLIC gnomonCore)

## #############################################################################
##
## #############################################################################

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${layer_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${gnomon_VERSION}
                                                 SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME})

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} EXPORT ${LAYER_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
