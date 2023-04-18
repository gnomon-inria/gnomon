project(gnomoncore VERSION ${gnomon_VERSION})

## #############################################################################
## Input
## #############################################################################

set(LAYER_NAME gnomonCore)

get_property(LAYER_INCLUDES TARGET ${LAYER_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

if(NOT DTK_SWIG_INCLUDE_PATH)
  message(FATAL_ERROR "You use a bad dtkScript version. Need DTK_SWIG_INCLUDE_PATH to continue")
endif(NOT DTK_SWIG_INCLUDE_PATH)

list(APPEND LAYER_INCLUDES ${DTK_SWIG_INCLUDE_PATH})

## #############################################################################
## Build setup
## #############################################################################

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i PROPERTIES INCLUDE_DIRECTORIES  "${LAYER_INCLUDES}")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/gnomonForm.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/gnomonForm.i PROPERTIES INCLUDE_DIRECTORIES  "${LAYER_INCLUDES}")

set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${LAYER_NAME}PYTHON_wrap.cxx PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/gnomonFormPYTHON_wrap.cxx PROPERTY SKIP_AUTOMOC ON)

set(SWIG_MODULE_NAME ${PROJECT_NAME})

list(APPEND CMAKE_SWIG_FLAGS -v -O -python -c++ -threads)
#list(APPEND CMAKE_SWIG_FLAGS -v -O -python -c++ -threads -E > ~/truc.cpp)

## #############################################################################
## Build rules
## #############################################################################

swig_add_library(gnomonform LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/gnomonForm.i OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR})
swig_link_libraries(gnomonform
  ${LAYER_NAME}
  Python3::Python
  dtkCore
  Qt6::Core)

swig_add_library(${PROJECT_NAME} LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR})
swig_link_libraries(${PROJECT_NAME}
  ${LAYER_NAME}
  Python3::Python
  Python3::NumPy
  ${VTK_LIBRARIES}
  dtkCore
  dtkImagingCore
  Qt6::Core)

#target_link_libraries(${PROJECT_NAME} ${LAYER_NAME})

## #############################################################################
## Install rules
## #############################################################################

set(GNOMON_PYTHON "${CMAKE_BINARY_DIR}/python/gnomon/")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_${PROJECT_NAME}.so)

add_custom_command(
    TARGET gnomonform POST_BUILD VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:gnomonform>" ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonform.so)

install(TARGETS ${PROJECT_NAME} DESTINATION ${DTK_PYTHON_INSTALL_PATH}/)
install(TARGETS gnomonform DESTINATION ${DTK_PYTHON_INSTALL_PATH}/)

######################################################################
### CMakeLists.txt ends here
