## Version: $Id$
##
######################################################################
##
### Commentary:
##
######################################################################
##
### Author: Nicolas Niclausse / Come Le Breton
##
######################################################################
##
### Change Log:
##
######################################################################
##
### Code:

## ###################################################################
## Build tree setup
## ###################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

## ###################################################################
## Build setup
## ###################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## ###################################################################
## Inputs
## ###################################################################

set(${PROJECT_NAME}_EXTENSION_SOURCES
  gnomonComposerExtension.cpp
  gnomonComposerExtensionPlugin.cpp)

set(${PROJECT_NAME}_EXTENSION_HEADERS
  gnomonComposerExtensionPlugin.h
  gnomonComposerExtension.h)

## ###################################################################
## Build rules
## ###################################################################

add_definitions(-DQT_PLUGIN)

add_library(${PROJECT_NAME}Extension SHARED
  ${${PROJECT_NAME}_EXTENSION_HEADERS}
  ${${PROJECT_NAME}_EXTENSION_SOURCES})

## #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME}Extension PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME}Extension PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME}Extension PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME}Extension PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME}Extension PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME}Extension EXPORT_FILE_NAME "${PROJECT_NAME}ExtensionExport.h")

add_custom_command(TARGET ${PROJECT_NAME}Extension PRE_BUILD
   COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ExtensionExport.h" "${CMAKE_BINARY_DIR}"
   COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ExtensionExport.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ExtensionExport")

set(${PROJECT_NAME}_EXTENSION_HEADERS
  ${${PROJECT_NAME}_EXTENSION_HEADERS}
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ExtensionExport"
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ExtensionExport.h")

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME}Extension dtkCore)
target_link_libraries(${PROJECT_NAME}Extension dtkComposer)
target_link_libraries(${PROJECT_NAME}Extension gnomonCore)
target_link_libraries(${PROJECT_NAME}Extension gnomonComposer)

## #################################################################
## Install rules
## #################################################################

install(TARGETS ${PROJECT_NAME}Extension
  RUNTIME DESTINATION plugins/dtkComposer
  LIBRARY DESTINATION plugins/dtkComposer
  ARCHIVE DESTINATION plugins/dtkComposer)

######################################################################
### CMakeLists.txt ends here
