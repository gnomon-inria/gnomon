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
## swig rules
## ###################################################################

find_package(SWIG 4 REQUIRED)
include(${SWIG_USE_FILE})

mark_as_advanced(SWIG_DIR)
mark_as_advanced(SWIG_EXECUTABLE)
mark_as_advanced(SWIG_VERSION)

if(APPLE)
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -flat_namespace")
endif(APPLE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## #################################################################
## Dependencies - external
## #################################################################

add_subdirectory(gnomonCore)
add_subdirectory(gnomonWidgets)
add_subdirectory(gnomonVisualization)
add_subdirectory(gnomonWorkspace)

######################################################################
### CMakeLists.txt ends here
