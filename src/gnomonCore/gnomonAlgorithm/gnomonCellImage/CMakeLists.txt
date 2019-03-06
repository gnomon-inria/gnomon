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
    gnomonAbstractCellImageFilter
    gnomonAbstractCellImageFilter.h
    gnomonAbstractCellImageFromImage
    gnomonAbstractCellImageFromImage.h
    gnomonAbstractCellImageQuantification
    gnomonAbstractCellImageQuantification.h
    gnomonAbstractCellImageReader
    gnomonAbstractCellImageReader.h
    gnomonAbstractCellImageWriter
    gnomonAbstractCellImageWriter.h)

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
    gnomonAbstractCellImageFilter.cpp
    gnomonAbstractCellImageFromImage.cpp
    gnomonAbstractCellImageQuantification.cpp
    gnomonAbstractCellImageReader.cpp
    gnomonAbstractCellImageWriter.cpp)

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
