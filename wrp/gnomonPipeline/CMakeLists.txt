project(gnomonpipeline VERSION ${gnomon_VERSION})

set(LAYER_NAME gnomonPipeline)

# get include dir from layer
get_property(LAYER_INCLUDES TARGET ${LAYER_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

# add include dir from dtkScript
if(NOT DTK_SWIG_INCLUDE_PATH)
  message(FATAL_ERROR "You use a bad dtkScript version. Need DTK_SWIG_INCLUDE_PATH to continue")
endif(NOT DTK_SWIG_INCLUDE_PATH)
list(APPEND LAYER_INCLUDES ${DTK_SWIG_INCLUDE_PATH})

#message(${DTK_SWIG_INCLUDE_PATH})
# swig stuff

get_property(GNOMONCORE_INCLUDES TARGET gnomonCore PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
list(APPEND LAYER_INCLUDES ${GNOMONCORE_INCLUDES})

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i PROPERTIES INCLUDE_DIRECTORIES  "${LAYER_INCLUDES}")

set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${LAYER_NAME}PYTHON_wrap.cxx PROPERTY SKIP_AUTOMOC ON)

set(SWIG_MODULE_NAME ${PROJECT_NAME})

list(APPEND CMAKE_SWIG_FLAGS -v -O -python -c++  -threads)
#list(APPEND CMAKE_SWIG_FLAGS -v -O -python -c++  -threads -E > ~/pipeline.cpp)
swig_add_library(${PROJECT_NAME} LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR})

swig_link_libraries(${PROJECT_NAME}
  ${LAYER_NAME}
  Python3::Python
  dtkCore
  gnomonCore
  Qt6::Core
  Qt6::Gui)

target_link_libraries(${PROJECT_NAME} ${LAYER_NAME})

## #############################################################################
## Install rules
## #############################################################################

set(GNOMON_PYTHON "${CMAKE_BINARY_DIR}/python/gnomon/")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_${PROJECT_NAME}.so
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.py  ${GNOMON_PYTHON}/pipeline/__${PROJECT_NAME}.py)

install(TARGETS ${PROJECT_NAME} DESTINATION ${DTK_PYTHON_INSTALL_PATH}/)

######################################################################
### CMakeLists.txt ends here
