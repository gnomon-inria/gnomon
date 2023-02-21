project(gnomonCore VERSION ${gnomon_VERSION})

## #################################################################
## Inputs
## #################################################################

add_subdirectory(gnomonAlgorithm)
add_subdirectory(gnomonAlgorithmsLogs)
add_subdirectory(gnomonCommand)
add_subdirectory(gnomonForm)
add_subdirectory(gnomonModel)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonAbstractDataDriver.h
  gnomonCore
  gnomonCore.h
  gnomonCoreSettings
  gnomonCoreSettings.h
  gnomonCorePlugin.h
  gnomonDataDriver.h
  gnomonDataManager
  gnomonDataManager.h
  gnomonFileSystemFormReader.h
  gnomonFormVisitor
  gnomonFormVisitor.h
  gnomonLandmark.h
  gnomonMorphonetHelper.h
  gnomonPluginFactory.h
  gnomonPluginFactory.tpp
  gnomonPythonPluginLoader
  gnomonPythonPluginLoader.h
  gnomonPythonPluginParser
  gnomonPythonPluginParser.h
  gnomonTypeDef
  gnomonTypeDef.h
  gnomonTime
  gnomonTime.h
  simpleCrypt.h
  ${${PROJECT_NAME}_ALGORITHM_HEADERS}
  ${${PROJECT_NAME}_ALGORITHMSLOGS_HEADERS}
  ${${PROJECT_NAME}_COMMAND_HEADERS}
  ${${PROJECT_NAME}_FORM_HEADERS}
  ${${PROJECT_NAME}_MODEL_HEADERS})

set(${PROJECT_NAME}_SOURCES
  gnomonAbstractDataDriver.cpp
  gnomonCore.cpp
  gnomonCoreSettings.cpp
  gnomonDataDriver.cpp
  gnomonDataManager.cpp
  gnomonFileSystemFormReader.cpp
  gnomonFormVisitor.cpp
  gnomonMorphonetHelper.cpp
  gnomonPythonPluginParser.cpp
  gnomonPythonPluginLoader.cpp
  gnomonTime.cpp
  simpleCrypt.cpp
  ${${PROJECT_NAME}_ALGORITHM_SOURCES}
  ${${PROJECT_NAME}_ALGORITHMSLOGS_SOURCES}
  ${${PROJECT_NAME}_COMMAND_SOURCES}
  ${${PROJECT_NAME}_FORM_SOURCES}
  ${${PROJECT_NAME}_MODEL_SOURCES})

## #############################################################################
##
## #############################################################################

set(layer_BINARY_DIR "${PROJECT_BINARY_DIR}/..")

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Concurrent)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Network)

target_link_libraries(${PROJECT_NAME} PUBLIC  dtkCore)
target_link_libraries(${PROJECT_NAME} PUBLIC  dtkLog)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkScript)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkImagingCore)

target_link_libraries(${PROJECT_NAME} PRIVATE xtensor)

target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Python)

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
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

install( FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"  DESTINATION include/${PROJECT_NAME} )
install( FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h"  DESTINATION include/${PROJECT_NAME} )

## ###################################################################
## Install rules - files
## ###################################################################

foreach ( file ${${PROJECT_NAME}_HEADERS} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION include/${PROJECT_NAME}/${dir} )
endforeach()


## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} EXPORT ${LAYER_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
