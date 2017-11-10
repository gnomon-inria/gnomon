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

## #################################################################
## Input
## #################################################################

add_subdirectory(gnomon)
add_subdirectory(gnomonCore)
add_subdirectory(gnomonStyle)
add_subdirectory(gnomonFonts)
add_subdirectory(gnomonVisualization)
add_subdirectory(gnomonWidgets)

if(OMERO_ICE_FOUND AND OMERO_CLIENT_FOUND)
  add_subdirectory(gnomonOmero)
endif()

if(DTK_BUILD_COMPOSER)
  add_subdirectory(gnomonComposer)
endif(DTK_BUILD_COMPOSER)

## #################################################################
## Targets
## #################################################################

set(GNOMON_TARGETS)

set(GNOMON_TARGETS ${GNOMON_TARGETS} gnomonCore)
set(GNOMON_TARGETS ${GNOMON_TARGETS} gnomonStyle)
set(GNOMON_TARGETS ${GNOMON_TARGETS} gnomonFonts)
set(GNOMON_TARGETS ${GNOMON_TARGETS} gnomonVisualization)
set(GNOMON_TARGETS ${GNOMON_TARGETS} gnomonWidgets)

if(DTK_BUILD_COMPOSER)
  set(GNOMON_TARGETS ${GNOMON_TARGETS} gnomonComposer)
endif(DTK_BUILD_COMPOSER)

## #################################################################
## Target export
## #################################################################

export(TARGETS ${GNOMON_TARGETS} FILE "${CMAKE_BINARY_DIR}/gnomonDepends.cmake")

######################################################################
### CMakeLists.txt ends here
