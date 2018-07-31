project(gnomonCorePythonTests)

## ###################################################################
## Test rules
## ###################################################################
add_test(NAME pythonTests COMMAND ${PYTHON_EXECUTABLE} -m unittest discover -v ${CMAKE_CURRENT_SOURCE_DIR} WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )

set_tests_properties ( pythonTests PROPERTIES ENVIRONMENT "PYTHONPATH=${dtk_MODULE_PATH}/${DTK_PYTHON_INSTALL_PATH}:${dtkImaging_DIR}/${DTK_PYTHON_INSTALL_PATH}:${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")
