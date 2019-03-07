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

add_subdirectory(gnomonCellComplex)
add_subdirectory(gnomonCellGraph)
add_subdirectory(gnomonCellImage)
add_subdirectory(gnomonImagesSerie)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
  gnomonAbstractAlgorithm
  gnomonAbstractAlgorithm.h
  ${${PROJECT_NAME}_ALGORITHM_CELLCOMPLEX_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_CELLGRAPH_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_CELLIMAGE_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_IMAGESSERIE_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_MESH_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_TREE_HEADERS})


ADD_GNOMON_SOURCES(
  gnomonAbstractAlgorithm.cpp
  ${${PROJECT_NAME}_ALGORITHM_CELLCOMPLEX_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_CELLGRAPH_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_CELLIMAGE_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_IMAGESSERIE_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_MESH_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_TREE_SOURCES})


######################################################################
### CMakeLists.txt ends here
