## #################################################################
## Inputs
## #################################################################

add_subdirectory(gnomonBinaryImage)
add_subdirectory(gnomonCellComplex)
add_subdirectory(gnomonCellGraph)
add_subdirectory(gnomonCellImage)
add_subdirectory(gnomonDataDict)
add_subdirectory(gnomonDataFrame)
add_subdirectory(gnomonImage)
add_subdirectory(gnomonLString)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonPointCloud)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
  gnomonAbstractAlgorithm
  gnomonAbstractAlgorithm.h
  gnomonAbstractFormAdapter
  gnomonAbstractFormAdapter.h
  gnomonAbstractFormAlgorithm
  gnomonAbstractFormAlgorithm.h
  gnomonAbstractFormWriter
  gnomonAbstractFormWriter.h
  ${${PROJECT_NAME}_ALGORITHM_BINARYIMAGE_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_CELLCOMPLEX_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_CELLGRAPH_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_CELLIMAGE_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_DATADICT_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_DATAFRAME_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_IMAGE_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_LSTRING_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_MESH_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_POINTCLOUD_HEADERS}
  ${${PROJECT_NAME}_ALGORITHM_TREE_HEADERS}
)

ADD_GNOMON_SOURCES(
  gnomonAbstractAlgorithm.cpp
  gnomonAbstractFormAlgorithm.cpp
  gnomonAbstractFormWriter.cpp
  ${${PROJECT_NAME}_ALGORITHM_BINARYIMAGE_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_CELLCOMPLEX_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_CELLGRAPH_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_CELLIMAGE_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_DATADICT_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_DATAFRAME_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_IMAGE_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_LSTRING_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_MESH_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_POINTCLOUD_SOURCES}
  ${${PROJECT_NAME}_ALGORITHM_TREE_SOURCES}
)

######################################################################
### CMakeLists.txt ends here
