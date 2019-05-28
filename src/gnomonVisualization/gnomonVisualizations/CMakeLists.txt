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
add_subdirectory(gnomonImage)
add_subdirectory(gnomonImagesSerie)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonPointCloud)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
    gnomonAbstractMatplotlibVisualization
    gnomonAbstractMatplotlibVisualization.h
    gnomonAbstractMatplotlibVisualization_p.h
    gnomonAbstractVisualization
    gnomonAbstractVisualization.h
    gnomonAbstractVisualization_p.h
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLCOMPLEX_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLIMAGE_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_DATAFRAME_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_IMAGE_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_IMAGESSERIE_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_MESH_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_POINTCLOUD_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_TREE_HEADERS})


ADD_GNOMON_SOURCES(
    gnomonAbstractMatplotlibVisualization.cpp
    gnomonAbstractVisualization.cpp
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLCOMPLEX_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_CELLIMAGE_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_DATAFRAME_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_IMAGE_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_IMAGESSERIE_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_MESH_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_POINTCLOUD_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_TREE_SOURCES})


######################################################################
### CMakeLists.txt ends here
