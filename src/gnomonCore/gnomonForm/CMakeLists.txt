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
add_subdirectory(gnomonIntensityImage)
add_subdirectory(gnomonLString)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

set(HEADERS
  gnomonAbstractDynamicForm
  gnomonAbstractDynamicForm.h
  gnomonAbstractForm
  gnomonAbstractForm.h
  gnomonDiscreteDynamicForm
  gnomonDiscreteDynamicForm.h
  gnomonSphereForm
  gnomonSphereForm.h
  gnomonWallForm
  gnomonWallForm.h
  ${${PROJECT_NAME}_FORM_CELLCOMPLEX_HEADERS}
  ${${PROJECT_NAME}_FORM_CELLGRAPH_HEADERS}
  ${${PROJECT_NAME}_FORM_CELLIMAGE_HEADERS}
  ${${PROJECT_NAME}_FORM_DATAFRAME_HEADERS}
  ${${PROJECT_NAME}_FORM_IMAGESSERIE_HEADERS}
  ${${PROJECT_NAME}_FORM_INTENSITYIMAGE_HEADERS}
  ${${PROJECT_NAME}_FORM_LSTRING_HEADERS}
  ${${PROJECT_NAME}_FORM_MESH_HEADERS}
  ${${PROJECT_NAME}_FORM_TREE_HEADERS})


get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${HEADERS})
    list(APPEND FULL_HEADERS ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_HEADERS ${FULL_HEADERS} PARENT_SCOPE)

set(SOURCES
  gnomonAbstractForm.cpp
  gnomonDiscreteDynamicForm.cpp
  gnomonSphereForm.cpp
  gnomonWallForm.cpp
  ${${PROJECT_NAME}_FORM_CELLCOMPLEX_SOURCES}
  ${${PROJECT_NAME}_FORM_CELLGRAPH_SOURCES}
  ${${PROJECT_NAME}_FORM_CELLIMAGE_SOURCES}
  ${${PROJECT_NAME}_FORM_DATAFRAME_SOURCES}
  ${${PROJECT_NAME}_FORM_IMAGESSERIE_SOURCES}
  ${${PROJECT_NAME}_FORM_INTENSITYIMAGE_SOURCES}
  ${${PROJECT_NAME}_FORM_LSTRING_SOURCES}
  ${${PROJECT_NAME}_FORM_MESH_SOURCES}
  ${${PROJECT_NAME}_FORM_TREE_SOURCES})

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${SOURCES})
    list(APPEND FULL_SOURCES ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_SOURCES ${FULL_SOURCES} PARENT_SCOPE)

######################################################################
### CMakeLists.txt ends here
