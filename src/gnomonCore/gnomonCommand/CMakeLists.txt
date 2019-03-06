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
add_subdirectory(gnomonImagesSerie)
add_subdirectory(gnomonMesh)
add_subdirectory(gnomonTree)

## #################################################################
## Sources
## #################################################################

set(HEADERS
  gnomonAbstractCommand
  gnomonAbstractCommand.h
  ${${PROJECT_NAME}_COMMAND_CELLCOMPLEX_HEADERS}
  ${${PROJECT_NAME}_COMMAND_CELLGRAPH_HEADERS}
  ${${PROJECT_NAME}_COMMAND_CELLIMAGE_HEADERS}
  ${${PROJECT_NAME}_COMMAND_IMAGESSERIE_HEADERS}
  ${${PROJECT_NAME}_COMMAND_MESH_HEADERS}
  ${${PROJECT_NAME}_COMMAND_TREE_HEADERS})

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${HEADERS})
    list(APPEND FULL_HEADERS ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_HEADERS ${FULL_HEADERS} PARENT_SCOPE)

set(SOURCES
  ${${PROJECT_NAME}_COMMAND_CELLCOMPLEX_SOURCES}
  ${${PROJECT_NAME}_COMMAND_CELLGRAPH_SOURCES}
  ${${PROJECT_NAME}_COMMAND_CELLIMAGE_SOURCES}
  ${${PROJECT_NAME}_COMMAND_IMAGESSERIE_SOURCES}
  ${${PROJECT_NAME}_COMMAND_MESH_SOURCES}
  ${${PROJECT_NAME}_COMMAND_TREE_SOURCES})

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${SOURCES})
    list(APPEND FULL_SOURCES ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_SOURCES ${FULL_SOURCES} PARENT_SCOPE)

######################################################################
### CMakeLists.txt ends here
