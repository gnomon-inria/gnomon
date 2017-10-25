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

project(tissueOmero)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  tissueOmero
  tissueOmero.h
)

set(${PROJECT_NAME}_SOURCES
  tissueOmero.cpp
)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  include(dtkWrap)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tissueOmero.i.in ${CMAKE_CURRENT_SOURCE_DIR}/tissueOmero.i @ONLY)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP tissuecore python ${CMAKE_CURRENT_SOURCE_DIR}/tissueOmero.i ${${PROJECT_NAME}_WRAP_DEPENDS})
  endif()
endif()

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
#target_link_libraries(${PROJECT_NAME} Qt5::Gui)
#target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
#target_link_libraries(${PROJECT_NAME} dtkCore)
#target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

target_link_libraries(${PROJECT_NAME} tissueCore)
target_link_libraries(${PROJECT_NAME} ${OMERO_CLIENT_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${OMERO_ICE_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${Ice_LIBRARIES})

if(DTK_BUILD_WRAPPERS)

  if(DTK_WRAPPING_PYTHON)
      target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
  endif()

endif(DTK_BUILD_WRAPPERS)

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(PROJECT_NAME_HEADERS
   ${PROJECT_NAME_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## #################################################################
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${PROJECT_SOURCE_DIR}/tissueOmero.i
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/modules
      COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/tissuecore.py ${CMAKE_BINARY_DIR}/modules
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/modules")

    set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/modules/tissuecore.py)

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/tissueOmero.i
        COMMAND ln -sf ../lib/libtissueOmero.dylib _tissuecore.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/modules/_tissuecore.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/tissueOmero.i
        COMMAND ln -fs ../${CMAKE_INSTALL_LIBDIR}/libtissueOmero.so _tissuecore.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/modules/_tissuecore.so)
    endif(UNIX AND NOT APPLE)

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/tissue
  COMPONENT tissue-core)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT tissue-core
         EXPORT tissueDepends)

######################################################################
### CMakeLists.txt ends here
