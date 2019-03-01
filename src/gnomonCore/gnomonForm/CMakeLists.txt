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

project(gnomonForm)


## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
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

set(${PROJECT_NAME}_SOURCES
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

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkWidgets)

target_link_libraries(${PROJECT_NAME} dtkImagingCore)

target_link_libraries(${PROJECT_NAME} gnomonCore)

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-core)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT gnomon-core
         EXPORT gnomonDepends)



######################################################################
### CMakeLists.txt ends here
