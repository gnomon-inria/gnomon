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
## Options
## ###################################################################

set(GNOMON_STYLE CACHE STRING "Default application theme")

set_property(CACHE GNOMON_STYLE PROPERTY STRINGS "ONELIGHT" "ONEDARK")

## ###################################################################
## Setup
## ###################################################################

if(GNOMON_STYLE MATCHES "ONEDARK")
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
  set(GNOMON_STYLE_BACKGROUNDALTCOLOR "#21242b")
  set(GNOMON_STYLE_FOREGROUNDCOLOR "#ABB2BF")
  set(GNOMON_STYLE_CURSORCOLOR "#528BFF")
  set(GNOMON_STYLE_BORDERCOLOR "#32363E")
  set(GNOMON_STYLE_BORDERALTCOLOR "#1c1f24")
  set(GNOMON_STYLE_ACCENTCOLOR "#528BFF")
  set(GNOMON_STYLE_STATUSBAR "#2e333c")
endif(GNOMON_STYLE MATCHES "ONEDARK")

if(GNOMON_STYLE MATCHES "ONELIGHT")
  set(GNOMON_STYLE_BLACK "#383A42")
  set(GNOMON_STYLE_RED "#DC322F")
  set(GNOMON_STYLE_GREEN "#50A14F")
  set(GNOMON_STYLE_YELLOW "#C18401")
  set(GNOMON_STYLE_BLUE "#4078F2")
  set(GNOMON_STYLE_MAGENTA "#A626A4")
  set(GNOMON_STYLE_CYAN "#0184BC")
  set(GNOMON_STYLE_WHITE "#FAFAFA")
  set(GNOMON_STYLE_LIGHTBLACK "#A0A1A7")
  set(GNOMON_STYLE_LIGHTRED "#DC322F")
  set(GNOMON_STYLE_LIGHTGREEN "#50A14F")
  set(GNOMON_STYLE_LIGHTYELLOW "#C18401")
  set(GNOMON_STYLE_LIGHTBLUE "#4078F2")
  set(GNOMON_STYLE_LIGHTMAGENTA "#A626A4")
  set(GNOMON_STYLE_LIGHTCYAN "#0184BC")
  set(GNOMON_STYLE_LIGHTWHITE "#FAFAFA")
  set(GNOMON_STYLE_BACKGROUNDCOLOR "#FAFAFA")
  set(GNOMON_STYLE_BACKGROUNDALTCOLOR "#EFEFEF")
  set(GNOMON_STYLE_FOREGROUNDCOLOR "#383A42")
  set(GNOMON_STYLE_CURSORCOLOR "#526FFF")
  set(GNOMON_STYLE_BORDERCOLOR "#EDEDED")
  set(GNOMON_STYLE_BORDERALTCOLOR "#CCCCCC")
  set(GNOMON_STYLE_ACCENTCOLOR "#526FFF")
  set(GNOMON_STYLE_STATUSBAR "#DDDDDD")
endif(GNOMON_STYLE MATCHES "ONELIGHT")

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonStyle
  gnomonStyle.h
  gnomonStyleValues.h)

set(${PROJECT_NAME}_SOURCES
  gnomonStyle.cpp)

set(${PROJECT_NAME}_SOURCES_RCC
  gnomonStyle.qrc)

## #################################################################
## Build rules
## #################################################################

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonStyleConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/gnomonStyleConfig.h @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonStyleValues.h.in ${CMAKE_CURRENT_SOURCE_DIR}/gnomonStyleValues.h @ONLY)

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

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

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
