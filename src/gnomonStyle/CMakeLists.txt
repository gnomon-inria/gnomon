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

project(gnomonStyle)

## ###################################################################
## Setup
## ###################################################################

set(GNOMON_STYLE_BLACK "#282C34")
set(GNOMON_STYLE_RED "#E06C75")
set(GNOMON_STYLE_GREEN "#98C379")
set(GNOMON_STYLE_YELLOW "#D19A66")
set(GNOMON_STYLE_BLUE "#61AFEF")
set(GNOMON_STYLE_MAGENTA "#C678DD")
set(GNOMON_STYLE_CYAN "#56B6C2")
set(GNOMON_STYLE_WHITE "#ABB2BF")
set(GNOMON_STYLE_LIGHTBLACK "#5C6370")
set(GNOMON_STYLE_LIGHTRED "#E06C75")
set(GNOMON_STYLE_LIGHTGREEN "#98C379")
set(GNOMON_STYLE_LIGHTYELLOW "#D19A66")
set(GNOMON_STYLE_LIGHTBLUE "#61AFEF")
set(GNOMON_STYLE_LIGHTMAGENTA "#C678DD")
set(GNOMON_STYLE_LIGHTCYAN "#56B6C2")
set(GNOMON_STYLE_LIGHTWHITE "#FFFFFF")
set(GNOMON_STYLE_BACKGROUNDCOLOR "#282C34")
set(GNOMON_STYLE_BACKGROUNDALTCOLOR "##21242b")
set(GNOMON_STYLE_FOREGROUNDCOLOR "#ABB2BF")
set(GNOMON_STYLE_CURSORCOLOR "#528BFF")
set(GNOMON_STYLE_BORDERCOLOR "#32363E")
set(GNOMON_STYLE_ACCENTCOLOR "#528BFF")

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonStyle
  gnomonStyle.h)

set(${PROJECT_NAME}_SOURCES
  gnomonStyle.cpp)

set(${PROJECT_NAME}_SOURCES_RCC
  gnomonStyle.qrc)

## #################################################################
## Build rules
## #################################################################

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonStyle.qss.in ${CMAKE_CURRENT_SOURCE_DIR}/gnomonStyle.qss @ONLY)

qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_SOURCES_RCC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_QRC})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Widgets)

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(PROJECT_NAME_HEADERS
   ${PROJECT_NAME_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-style)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT gnomon-style
         EXPORT gnomonDepends)

######################################################################
### CMakeLists.txt ends here
