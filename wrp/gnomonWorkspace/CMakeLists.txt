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

project(gnomonworkspace VERSION ${gnomon_VERSION})

set(LAYER_NAME gnomonWorkspace)

get_property(LAYER_INCLUDES TARGET ${LAYER_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

if(NOT DTK_SWIG_INCLUDE_PATH)
  message(FATAL_ERROR "You use a bad dtkScript version. Need DTK_SWIG_INCLUDE_PATH to continue")
endif(NOT DTK_SWIG_INCLUDE_PATH)

list(APPEND LAYER_INCLUDES ${DTK_SWIG_INCLUDE_PATH})

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i PROPERTIES INCLUDE_DIRECTORIES  "${LAYER_INCLUDES}")

set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${LAYER_NAME}PYTHON_wrap.cxx PROPERTY SKIP_AUTOMOC ON)

set(SWIG_MODULE_NAME ${PROJECT_NAME})

list(APPEND CMAKE_SWIG_FLAGS -v -O -python -c++  -threads)

swig_add_library(${PROJECT_NAME} LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR})

swig_link_libraries(${PROJECT_NAME}
  ${LAYER_NAME}
  Python3::Python
  dtkCore
  dtkWidgets
  gnomonCore
  gnomonWidgets
  gnomonWorkspace)

target_link_libraries(${PROJECT_NAME} ${LAYER_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD VERBATIM
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_${PROJECT_NAME}.so
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.py  ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.py)

install(TARGETS ${PROJECT_NAME} DESTINATION ${DTK_PYTHON_INSTALL_PATH}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.py DESTINATION ${DTK_PYTHON_INSTALL_PATH}/)
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${LAYER_NAME}.i  DESTINATION ${CMAKE_INSTALL_PREFIX}/wrp/${LAYER_NAME}/)

######################################################################
### CMakeLists.txt ends here
