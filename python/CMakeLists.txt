find_program(PYTHON "python")
set(PYTHON_VERSION "${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}")

macro(configure_files_recur WORKDIR GLOBPAT DESTINATION)
    file(GLOB_RECURSE COPY_FILES
            RELATIVE ${WORKDIR}
            ${GLOBPAT})
    # add_custom_target(copy ALL COMMENT "Copying files: ${GLOBPAT}")

    foreach(FILENAME ${COPY_FILES})
        set(SRC "${WORKDIR}/${FILENAME}")
        set(DST "${DESTINATION}/${FILENAME}")
        # message(${FILENAME} " -> " ${DST})
        configure_file(${SRC} ${DST})
    endforeach(FILENAME)
endmacro(configure_files_recur)

if (PYTHON)
    set(PYTHON_VERSION "${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}")
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
    set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(PLUGIN_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/gnomonPlugin.py.in")
    set(PLUGIN_PY    "${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/gnomonPlugin.py")
    set(OUTPUT      "${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/__init__.py")
    set(TEMPLATES_WD "${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/templates.in")
    set(TEMPLATES_GLOB "${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/templates.in/**")
    set(TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/templates/")

    file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils.egg-info")

    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    configure_file(${PLUGIN_PY_IN} ${PLUGIN_PY})
    configure_files_recur(${TEMPLATES_WD} ${TEMPLATES_GLOB} ${TEMPLATES})
    add_custom_command(
        OUTPUT ${OUTPUT}
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/*.py
                                                   ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/gnomonDecorator
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/gnomonDecorator/*.py
                                                   ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/gnomonDecorator/
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/gnomonMpl
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/gnomon_utils/gnomonMpl/*.py
                                                   ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomon_utils/gnomonMpl/)

    add_custom_target(target ALL DEPENDS ${OUTPUT})

    install(
        CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} clean)"
        CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)"
    )
endif()
