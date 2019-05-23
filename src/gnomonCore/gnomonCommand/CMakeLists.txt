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
add_subdirectory(gnomonCellImage)
add_subdirectory(gnomonDataFrame)
add_subdirectory(gnomonImagesSerie)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonPointCloud)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
  gnomonAbstractCommand
  gnomonAbstractCommand.h
  ${${PROJECT_NAME}_COMMAND_CELLCOMPLEX_HEADERS}
  ${${PROJECT_NAME}_COMMAND_CELLGRAPH_HEADERS}
  ${${PROJECT_NAME}_COMMAND_CELLIMAGE_HEADERS}
  ${${PROJECT_NAME}_COMMAND_DATAFRAME_HEADERS}
  ${${PROJECT_NAME}_COMMAND_IMAGESSERIE_HEADERS}
  ${${PROJECT_NAME}_COMMAND_MESH_HEADERS}
  ${${PROJECT_NAME}_COMMAND_POINTCLOUD_HEADERS}
  ${${PROJECT_NAME}_COMMAND_TREE_HEADERS})


ADD_GNOMON_SOURCES(
  ${${PROJECT_NAME}_COMMAND_CELLCOMPLEX_SOURCES}
  ${${PROJECT_NAME}_COMMAND_CELLGRAPH_SOURCES}
  ${${PROJECT_NAME}_COMMAND_CELLIMAGE_SOURCES}
  ${${PROJECT_NAME}_COMMAND_DATAFRAME_SOURCES}
  ${${PROJECT_NAME}_COMMAND_IMAGESSERIE_SOURCES}
  ${${PROJECT_NAME}_COMMAND_MESH_SOURCES}
  ${${PROJECT_NAME}_COMMAND_POINTCLOUD_SOURCES}
  ${${PROJECT_NAME}_COMMAND_TREE_SOURCES})


######################################################################
### CMakeLists.txt ends here
