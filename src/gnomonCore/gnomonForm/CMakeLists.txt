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
add_subdirectory(gnomonDataFrame)
add_subdirectory(gnomonImagesSerie)
add_subdirectory(gnomonImage)
add_subdirectory(gnomonLString)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonPointCloud)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
  gnomonAbstractDynamicForm
  gnomonAbstractDynamicForm.h
  gnomonAbstractForm
  gnomonAbstractForm.h
  gnomonSphereForm
  gnomonSphereForm.h
  gnomonTimeSeries
  gnomonTimeSeries.h
  gnomonTimeSeries.tpp
  gnomonWallForm
  gnomonWallForm.h
  ${${PROJECT_NAME}_FORM_CELLCOMPLEX_HEADERS}
  ${${PROJECT_NAME}_FORM_CELLGRAPH_HEADERS}
  ${${PROJECT_NAME}_FORM_CELLIMAGE_HEADERS}
  ${${PROJECT_NAME}_FORM_DATAFRAME_HEADERS}
  ${${PROJECT_NAME}_FORM_IMAGE_HEADERS}
  ${${PROJECT_NAME}_FORM_IMAGESSERIE_HEADERS}
  ${${PROJECT_NAME}_FORM_LSTRING_HEADERS}
  ${${PROJECT_NAME}_FORM_MESH_HEADERS}
  ${${PROJECT_NAME}_FORM_POINTCLOUD_HEADERS}
  ${${PROJECT_NAME}_FORM_TREE_HEADERS})


ADD_GNOMON_SOURCES(
  gnomonAbstractForm.cpp
  gnomonSphereForm.cpp
  gnomonWallForm.cpp
  ${${PROJECT_NAME}_FORM_CELLCOMPLEX_SOURCES}
  ${${PROJECT_NAME}_FORM_CELLGRAPH_SOURCES}
  ${${PROJECT_NAME}_FORM_CELLIMAGE_SOURCES}
  ${${PROJECT_NAME}_FORM_DATAFRAME_SOURCES}
  ${${PROJECT_NAME}_FORM_IMAGE_SOURCES}
  ${${PROJECT_NAME}_FORM_IMAGESSERIE_SOURCES}
  ${${PROJECT_NAME}_FORM_LSTRING_SOURCES}
  ${${PROJECT_NAME}_FORM_MESH_SOURCES}
  ${${PROJECT_NAME}_FORM_POINTCLOUD_SOURCES}
  ${${PROJECT_NAME}_FORM_TREE_SOURCES})


######################################################################
### CMakeLists.txt ends here
