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

#add_subdirectory(gnomonActorLegacy)
add_subdirectory(gnomonCellComplex)
add_subdirectory(gnomonCellImage)
add_subdirectory(gnomonImageData)
add_subdirectory(gnomonImage)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonPolyData)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
#   gnomonActor
    gnomonActor.h
    gnomonLandmarkActor.h
    #${${PROJECT_NAME}_ACTOR_ACTORLEGACY_HEADERS}
    ${${PROJECT_NAME}_ACTOR_CELLCOMPLEX_HEADERS}
    ${${PROJECT_NAME}_ACTOR_CELLIMAGE_HEADERS}
    ${${PROJECT_NAME}_ACTOR_IMAGEDATA_HEADERS}
    ${${PROJECT_NAME}_ACTOR_IMAGE_HEADERS}
    ${${PROJECT_NAME}_ACTOR_MESH_HEADERS}
    ${${PROJECT_NAME}_ACTOR_POLYDATA_HEADERS})


ADD_GNOMON_SOURCES(
    gnomonActor.cpp
    gnomonLandmarkActor.cpp
    #${${PROJECT_NAME}_ACTOR_ACTORLEGACY_SOURCES}
    ${${PROJECT_NAME}_ACTOR_CELLCOMPLEX_SOURCES}
    ${${PROJECT_NAME}_ACTOR_CELLIMAGE_SOURCES}
    ${${PROJECT_NAME}_ACTOR_IMAGEDATA_SOURCES}
    ${${PROJECT_NAME}_ACTOR_IMAGE_SOURCES}
    ${${PROJECT_NAME}_ACTOR_MESH_SOURCES}
    ${${PROJECT_NAME}_ACTOR_POLYDATA_SOURCES})


######################################################################
### CMakeLists.txt ends here
