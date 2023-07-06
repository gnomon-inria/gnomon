project(gnomonCorePythonTests)

## ###################################################################
## Test rules
## ###################################################################

file(GLOB PYTHON_TESTS CONFIGURE_DEPENDS test_*.py)

foreach(PY_TEST_PATH ${PYTHON_TESTS})
cmake_path(GET PY_TEST_PATH FILENAME PY_TEST)
add_test(NAME ${PY_TEST} COMMAND ${Python3_EXECUTABLE} -m unittest ${PY_TEST} -v  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_tests_properties(${PY_TEST} PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}:${CMAKE_BINARY_DIR}/python/:${CMAKE_INSTALL_PREFIX}/${DTK_PYTHON_INSTALL_PATH}")
endforeach()

######################################################################
### CMakeLists.txt ends here
