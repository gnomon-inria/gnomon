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

macro (ADD_GNOMON_HEADERS)
    set(HEADERS ${ARGN})
    get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    foreach (FILE ${HEADERS})
        list(APPEND FULL_HEADERS ${DIRECTORY}/${FILE})
    endforeach()
    string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
    string(TOUPPER ${DIRECTORY} DIRECTORY)
    set(${PROJECT_NAME}_${DIRECTORY}_HEADERS ${FULL_HEADERS} PARENT_SCOPE)
endmacro()

macro (ADD_GNOMON_SOURCES)
    set(SOURCES ${ARGN})
    get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    foreach (FILE ${SOURCES})
        list(APPEND FULL_SOURCES ${DIRECTORY}/${FILE})
    endforeach()
    string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
    string(TOUPPER ${DIRECTORY} DIRECTORY)
    set(${PROJECT_NAME}_${DIRECTORY}_SOURCES ${FULL_SOURCES} PARENT_SCOPE)
endmacro()

macro (ADD_GNOMON_SUBDIRECTORY_HEADERS)
    set(HEADERS ${ARGN})
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
endmacro()

macro (ADD_GNOMON_SUBDIRECTORY_SOURCES)
    set(SOURCES ${ARGN})
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
endmacro()


######################################################################
### gnomonConfigVersion.cmake.in ends here