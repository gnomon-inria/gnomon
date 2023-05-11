
add_subdirectory(gnomonBinaryImage)
add_subdirectory(gnomonCellComplex)
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
    gnomonAbstractVtkVisualization
    gnomonAbstractVtkVisualization.h
    gnomonAbstractVtkVisualization_p.h
    gnomonAbstractMplVisualization
    gnomonAbstractMplVisualization.h
    gnomonAbstractQmlVisualization
    gnomonAbstractQmlVisualization.h
    gnomonAbstractVisualization
    gnomonAbstractVisualization.h
    gnomonAbstractVisualization_p.h
    ${${PROJECT_NAME}_VISUALIZATIONS_BINARYIMAGE_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLCOMPLEX_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLIMAGE_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_DATADICT_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_DATAFRAME_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_IMAGE_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_LSTRING_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_MESH_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_POINTCLOUD_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_TREE_HEADERS})

ADD_GNOMON_SOURCES(
    gnomonAbstractVtkVisualization.cpp
    gnomonAbstractMplVisualization.cpp
    gnomonAbstractQmlVisualization.cpp
    gnomonAbstractVisualization.cpp
    ${${PROJECT_NAME}_VISUALIZATIONS_BINARYIMAGE_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLCOMPLEX_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLIMAGE_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_DATADICT_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_DATAFRAME_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_IMAGE_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_LSTRING_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_MESH_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_POINTCLOUD_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_TREE_SOURCES})

######################################################################
### CMakeLists.txt ends here
