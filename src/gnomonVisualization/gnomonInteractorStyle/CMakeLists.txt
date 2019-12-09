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
## Inputs
## #################################################################

add_subdirectory(gnomonCellImage)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
    gnomonInteractorStyle
    gnomonInteractorStyle.h
    gnomonInteractorStyleXYZ
    gnomonInteractorStyleXYZ.h
    ${${PROJECT_NAME}_INTERACTORSTYLE_CELLIMAGE_HEADERS})


ADD_GNOMON_SOURCES(
    gnomonInteractorStyle.cpp
    gnomonInteractorStyleXYZ.cpp
    ${${PROJECT_NAME}_INTERACTORSTYLE_CELLIMAGE_SOURCES})


######################################################################
### CMakeLists.txt ends here
