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

project(tissueVisualization)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  tissueActor
  tissueActor.h
  tissueActorImage
  tissueActorImage.h
  tissueActorMesh
  tissueActorMesh.h
  tissueActorVolume
  tissueActorVolume.h
  tissueClutEditor
  tissueClutEditor.h
  tissueView
  tissueView.h
  tissueViewManager
  tissueViewManager.h
  tissueVisualization)

set(${PROJECT_NAME}_SOURCES
  tissueActor.cpp
  tissueActorImage.cpp
  tissueActorMesh.cpp
  tissueActorVolume.cpp
  tissueClutEditor.cpp
  tissueView.cpp
  tissueViewManager.cpp)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  include(dtkWrap)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON AND DTK_PYTHON_WRAPPER MATCHES "SWIG")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tissueVisualization.i.in ${CMAKE_CURRENT_SOURCE_DIR}/tissueVisualization.i @ONLY)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP tissuevisualization python ${CMAKE_CURRENT_SOURCE_DIR}/tissueVisualization.i ${${PROJECT_NAME}_WRAP_DEPENDS})
  endif()
endif()

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME} dtkWidgets)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${tissue_VERSION} SOVERSION ${tissue_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## #################################################################
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)

    if(DTK_PYTHON_WRAPPER MATCHES "SWIG")
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/tissueVisualization.i
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/modules
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/tissuecore.py ${CMAKE_BINARY_DIR}/modules
        COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/modules")

      set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/modules/tissuevizualization.py)

      if(APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/tissueVisualization.i
          COMMAND ln -sf ../lib/libtissueVisualization.dylib _tissuevisualization.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/modules/_tissuevisualization.so)
      endif(APPLE)

      if(UNIX AND NOT APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/tissueVisualization.i
          COMMAND ln -fs ../${CMAKE_INSTALL_LIBDIR}/libtissueVisualization.so _tissuevisualization.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/modules/_tissuevisualization.so)
      endif(UNIX AND NOT APPLE)

    endif()

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/tissue
  COMPONENT tissue-visualization)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT tissue-visualization
         EXPORT tissueDepends)

######################################################################
### CMakeLists.txt ends here
