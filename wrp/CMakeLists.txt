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

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(dtkCorePython REQUIRED)
find_package(dtkImagingPython REQUIRED)

## ###################################################################
## Python
## ###################################################################

find_package(PythonInterp "3")
find_package(PythonLibs 3 QUIET)

get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} PATH)

include_directories(${PYTHON_INCLUDE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(DTK_PYTHON_INSTALL_PATH "${CMAKE_INSTALL_LIBDIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages")


## ###################################################################
## swig rules
## ###################################################################

find_package(SWIG 4 REQUIRED)
include(${SWIG_USE_FILE})

#set(CMAKE_SWIG_FLAGS "-c++;-threads;-python")

mark_as_advanced(SWIG_DIR)
mark_as_advanced(SWIG_EXECUTABLE)
mark_as_advanced(SWIG_VERSION)

if(APPLE)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -flat_namespace")
endif(APPLE)

## #################################################################
## Dependencies - external
## #################################################################

set(CMAKE_MODULES_INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${DTK_PYTHON_INSTALL_PATH})

add_subdirectory(gnomonCore)
add_subdirectory(gnomonWidgets)
add_subdirectory(gnomonVisualization)



## TODO

## ###################################################################
## Export configuration
## ###################################################################

#set(${PROJECT_NAME}_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE
#  STRING "install path for ${PROJECT_NAME}Config.cmake")

#set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)
#set(GNOMON_CORE_SWIG_PYTHON_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/wrp)
