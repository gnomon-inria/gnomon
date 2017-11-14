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

project(gnomonFonts)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonFonts
  gnomonFontAllTheIcons.h
  gnomonFontAwesome.h
  gnomonFontFileIcons.h
  gnomonFontOcticon.h
  gnomonFontSourceCodePro.h
  gnomonFontSourceSansPro.h)

set(${PROJECT_NAME}_SOURCES
  gnomonFontAllTheIcons.cpp
  gnomonFontAwesome.cpp
  gnomonFontFileIcons.cpp
  gnomonFontOcticon.cpp
  gnomonFontSourceCodePro.cpp
  gnomonFontSourceSansPro.cpp)

set(${PROJECT_NAME}_SOURCES_RCC
  gnomonFontAllTheIcons.qrc
  gnomonFontAwesome.qrc
  gnomonFontFileIcons.qrc
  gnomonFontOcticon.qrc
  gnomonFontSourceCodePro.qrc
  gnomonFontSourceSansPro.qrc)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_SOURCES_RCC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_QRC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)

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
  COMPONENT gnomon-fonts)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT gnomon-fonts
         EXPORT gnomonDepends)

######################################################################
### CMakeLists.txt ends here
