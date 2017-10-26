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

add_subdirectory(tissueCore)
add_subdirectory(tissueFonts)
add_subdirectory(tissueVisualization)
add_subdirectory(tissueWidgets)

if(DTK_BUILD_COMPOSER)
  add_subdirectory(tissueComposer)
endif(DTK_BUILD_COMPOSER)

## #################################################################
## Targets
## #################################################################

set(TISSUE_TARGETS)

set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueCore)
set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueFonts)
set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueVisualization)
set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueWidgets)

if(DTK_BUILD_COMPOSER)
  set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueComposer)
endif(DTK_BUILD_COMPOSER)

## #################################################################
## Target export
## #################################################################

export(TARGETS ${TISSUE_TARGETS} FILE "${CMAKE_BINARY_DIR}/tissueDepends.cmake")

######################################################################
### CMakeLists.txt ends here
