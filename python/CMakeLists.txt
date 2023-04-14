project(gnomonPython VERSION ${gnomon_VERSION})

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
    set(PLUGIN_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/gnomon/utils/gnomonPlugin.py.in")
    set(PLUGIN_PY    "${CMAKE_CURRENT_BINARY_DIR}/gnomon/utils/gnomonPlugin.py")
    set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/gnomon/__init__.py")
    #set(TEMPLATES_WD "${CMAKE_CURRENT_SOURCE_DIR}/gnomon/utils/templates")
    #set(TEMPLATES_GLOB "${CMAKE_CURRENT_SOURCE_DIR}/gnomon/utils/templates/**")
    #set(TEMPLATES "${CMAKE_CURRENT_BINARY_DIR}/gnomon/utils/templates/")
    set(TEMPLATES_WD "${CMAKE_CURRENT_SOURCE_DIR}/gnomon/")
    set(TEMPLATES_GLOB "${CMAKE_CURRENT_SOURCE_DIR}/gnomon/**")
    set(TEMPLATES "${CMAKE_CURRENT_BINARY_DIR}/gnomon/")

    file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/gnomon/")

    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    #configure_file(${PLUGIN_PY_IN} ${PLUGIN_PY})
    configure_files_recur(${TEMPLATES_WD} ${TEMPLATES_GLOB} ${TEMPLATES})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml)
    #add_custom_command(
    #    OUTPUT ${OUTPUT}
    #    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml
    #        ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml
    #    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/gnomon/
    #        ${CMAKE_CURRENT_BINARY_DIR}/gnomon/
    #    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/gnomon/utils/templates.in
    #    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/gnomon/utils/gnomonPlugin.py.in
    #)

    add_custom_target(${PROJECT_NAME} ALL DEPENDS ${OUTPUT})
    add_dependencies(${PROJECT_NAME} gnomonCore)
    add_dependencies(${PROJECT_NAME} gnomoncore)
    add_dependencies(${PROJECT_NAME} gnomonPipeline)
    add_dependencies(${PROJECT_NAME} gnomonpipeline)
    add_dependencies(${PROJECT_NAME} gnomonVisualization)
    add_dependencies(${PROJECT_NAME} gnomonvisualization)

    # after the REMOVE RECURSE, copy the wrapping files
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD VERBATIM
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/wrp/gnomonCore/gnomoncore.py  ${CMAKE_CURRENT_BINARY_DIR}/gnomon/core/__gnomoncore.py
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/wrp/gnomonCore/gnomonform.py  ${CMAKE_CURRENT_BINARY_DIR}/gnomon/core/__gnomonform.py
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/wrp/gnomonPipeline/gnomonpipeline.py  ${CMAKE_CURRENT_BINARY_DIR}/gnomon/pipeline/__gnomonpipeline.py
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/wrp/gnomonVisualization/gnomonvisualization.py  ${CMAKE_CURRENT_BINARY_DIR}/gnomon/visualization/__gnomonvisualization.py)
    install(
        CODE "execute_process(COMMAND ${PYTHON} -m pip install python/)"
    )
endif()
