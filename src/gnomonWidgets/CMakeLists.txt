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

project(gnomonWidgets)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonCodeEditor
  gnomonCodeEditor.h
  gnomonWidgets)

set(${PROJECT_NAME}_SOURCES
  gnomonCodeEditor.cpp)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME} gnomonCore)

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-widgets)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT gnomon-widgets
  EXPORT gnomonDepends)

######################################################################
### CMakeLists.txt ends here
