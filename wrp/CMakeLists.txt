cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

find_package(dtkCorePython REQUIRED)

## ###################################################################
## swig rules
## ###################################################################

find_package(SWIG 4 REQUIRED)
include(${SWIG_USE_FILE})
include_directories(${Python3_INCLUDE_DIRS})

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
add_subdirectory(gnomonPipeline)

if(GNOMON_ENABLE_VISU)
  add_subdirectory(gnomonVisualization)
endif()

######################################################################
### CMakeLists.txt ends here
