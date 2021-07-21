find_program(PYTHON "python")

if (PYTHON)
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
    set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(OUTPUT      "${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/__init__.py")

    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    add_custom_command(
        OUTPUT ${OUTPUT}
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils
                                                             ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils)

    add_custom_target(target ALL DEPENDS ${OUTPUT})

    install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)")
endif()
