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

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS)

set(${PROJECT_NAME}_SOURCES)

set(${PROJECT_NAME}_SOURCES_RCC
  gnomonStyle.qrc)

## #################################################################
## Build rules
## #################################################################

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
