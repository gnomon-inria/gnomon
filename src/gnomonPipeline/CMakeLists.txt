project(gnomonPipeline VERSION ${gnomon_VERSION})

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_HEADERS
  # gnomonComposer
  # gnomonComposerNodeWorkspace
  # gnomonComposerNodeWorkspace.h
  # gnomonComposerWidget_p.h
  # gnomonComposerWidget.h
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
  # gnomonComposerNodeWorkspace.cpp
  # gnomonComposerWidget.cpp
  gnomonPipeline.cpp
  gnomonPipelineEdge.cpp
  gnomonPipelineManager.cpp
  gnomonPipelineNode.cpp
  gnomonPipelineNodeAdapter.cpp
  gnomonPipelineNodeAlgorithm.cpp
  gnomonPipelineNodeConstructor.cpp
  gnomonPipelineNodeReader.cpp
  gnomonPipelineNodeTask.cpp
  gnomonPipelineNodeWriter.cpp
  gnomonPipelinePort.cpp
  gnomonPipelineNodeMorphonet.cpp)

## set(${PROJECT_NAME}_RESOURCES gnomonPipeline.qrc)

## #################################################################
## Build rules
## #################################################################

## qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_RESOURCES})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES})
  ##${${PROJECT_NAME}_SOURCES_QRC})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
# target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Network)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Svg)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Xml)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Qml)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkCore)
# target_link_libraries(${PROJECT_NAME} PRIVATE dtkThemes)
# target_link_libraries(${PROJECT_NAME} PRIVATE dtkWidgets)
#target_link_libraries(${PROJECT_NAME} PRIVATE dtkImagingCore)

# target_link_libraries(${PROJECT_NAME} PUBLIC dtkComposer)

target_link_libraries(${PROJECT_NAME} PUBLIC gnomonCore)
# target_link_libraries(${PROJECT_NAME} PUBLIC gnomonWidgets)

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
