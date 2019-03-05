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
  gnomonCellComplex
  gnomonCellComplex.h)


get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
get_filename_component(PARENT_PATH ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_DIRECTORY ${PARENT_PATH} NAME)
foreach (FILE ${HEADERS})
    list(APPEND FULL_HEADERS ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(REPLACE "gnomon" "" PARENT_DIRECTORY ${PARENT_DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
string(TOUPPER ${PARENT_DIRECTORY} PARENT_DIRECTORY)
set(${PROJECT_NAME}_${PARENT_DIRECTORY}_${DIRECTORY}_HEADERS ${FULL_HEADERS} PARENT_SCOPE)

set(SOURCES
  gnomonAbstractCellComplexData.cpp)

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
get_filename_component(PARENT_PATH ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_DIRECTORY ${PARENT_PATH} NAME)
foreach (FILE ${SOURCES})
    list(APPEND FULL_SOURCES ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(REPLACE "gnomon" "" PARENT_DIRECTORY ${PARENT_DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
string(TOUPPER ${PARENT_DIRECTORY} PARENT_DIRECTORY)
set(${PROJECT_NAME}_${PARENT_DIRECTORY}_${DIRECTORY}_SOURCES ${FULL_SOURCES} PARENT_SCOPE)

######################################################################
### CMakeLists.txt ends here
