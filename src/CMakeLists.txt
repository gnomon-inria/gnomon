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

if(OMERO_ICE_FOUND AND OMERO_CLIENT_FOUND)
  add_subdirectory(tissueOmero)
endif()

if(DTK_BUILD_COMPOSER)
  add_subdirectory(tissueComposer)
endif(DTK_BUILD_COMPOSER)

## #################################################################
## Targets
## #################################################################

set(TISSUE_TARGETS)

set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueCore)

if(DTK_BUILD_COMPOSER)
  set(TISSUE_TARGETS ${TISSUE_TARGETS} tissueComposer)
endif(DTK_BUILD_COMPOSER)

## #################################################################
## Target export
## #################################################################

export(TARGETS ${TISSUE_TARGETS} FILE "${CMAKE_BINARY_DIR}/tissueDepends.cmake")

######################################################################
### CMakeLists.txt ends here
