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
## Sources
## #################################################################



set(HEADERS
  gnomonAbstractCellComplexData
  gnomonAbstractCellComplexData.h
  gnomonAbstractCellGraphData
  gnomonAbstractCellGraphData.h
  gnomonAbstractCellImageData
  gnomonAbstractCellImageData.h
  gnomonAbstractDataFrameData
  gnomonAbstractDataFrameData.h
  gnomonAbstractDynamicForm
  gnomonAbstractDynamicForm.h
  gnomonAbstractForm
  gnomonAbstractForm.h
  gnomonAbstractIntensityImageData
  gnomonAbstractIntensityImageData.h
  gnomonAbstractLStringData
  gnomonAbstractLStringData.h
  gnomonAbstractMeshData
  gnomonAbstractMeshData.h
  gnomonAbstractTreeData
  gnomonAbstractTreeData.h
  gnomonCellComplex
  gnomonCellComplex.h
  gnomonCellGraph
  gnomonCellGraph.h
  gnomonCellImage
  gnomonCellImage.h
  gnomonDataFrame
  gnomonDataFrame.h
  gnomonDiscreteDynamicForm
  gnomonDiscreteDynamicForm.h
  gnomonImagesSerie
  gnomonImagesSerie.h
  gnomonIntensityImage
  gnomonIntensityImage.h
  gnomonLString
  gnomonLString.h
  gnomonMesh
  gnomonMesh.h
  gnomonSphereForm
  gnomonSphereForm.h
  gnomonTree
  gnomonTree.h
  gnomonWallForm
  gnomonWallForm.h)


get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${HEADERS})
    list(APPEND FULL_HEADERS ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_HEADERS ${FULL_HEADERS} PARENT_SCOPE)

set(SOURCES
  gnomonAbstractCellComplexData.cpp
  gnomonAbstractCellGraphData.cpp
  gnomonAbstractCellImageData.cpp
  gnomonAbstractDataFrameData.cpp
  gnomonAbstractForm.cpp
  gnomonAbstractIntensityImageData.cpp
  gnomonAbstractLStringData.cpp
  gnomonAbstractMeshData.cpp
  gnomonAbstractTreeData.cpp
  gnomonDiscreteDynamicForm.cpp
  gnomonImagesSerie.cpp
  gnomonSphereForm.cpp
  gnomonWallForm.cpp)

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${SOURCES})
    list(APPEND FULL_SOURCES ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_SOURCES ${FULL_SOURCES} PARENT_SCOPE)

######################################################################
### CMakeLists.txt ends here
