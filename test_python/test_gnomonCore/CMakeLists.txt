project(gnomonCorePythonTests)

## ###################################################################
## Test rules
## ###################################################################
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  #ugly fix to avoid error with macOS 10.9 . Qt is compiled with MacOS 10.10 which has "LSCopyDefaultApplicationURLForURL(_:_:_:)"
  add_test(NAME pythonTests COMMAND ${PYTHON_EXECUTABLE} -m unittest discover -v ${CMAKE_CURRENT_SOURCE_DIR} WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )

  set_tests_properties ( pythonTests PROPERTIES ENVIRONMENT "PYTHONPATH=${dtk_MODULE_PATH}/${DTK_PYTHON_INSTALL_PATH}:${dtkImaging_DIR}/${DTK_PYTHON_INSTALL_PATH}:${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")
endif()
