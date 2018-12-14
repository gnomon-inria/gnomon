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

project(gnomonWidgets)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonAbstractVisualization.h
  gnomonAbstractVisualization_p.h
  gnomonAbstractVisualization
  gnomonCodeEditor
  gnomonCodeEditor.h
  gnomonDropSite
  gnomonDropSite.h
  gnomonFinder
  gnomonFinder.h
  gnomonFormManager
  gnomonFormManager.h
  gnomonFormManager_p.h
  gnomonFormManagerFocus.h
  gnomonFormManagerItem.h
  gnomonGridLayout
  gnomonGridLayout.h
  gnomonImageManager
  gnomonImageManager.h
  gnomonImageManager_p.h
  gnomonImageManagerData.h
  gnomonImageManagerItem.h
  gnomonImageManagerFocus.h
  gnomonItemButton.h
  gnomonLandmarkActor.h
  gnomonOverlayPane
  gnomonOverlayPane.h
  gnomonOverlayPaneItem
  gnomonOverlayPaneItem.h
  gnomonToolBar
  gnomonToolBar.h
  gnomonViewForm
  gnomonViewForm.h
  gnomonViewVolumic
  gnomonViewVolumic.h
  gnomonViewVolumicOverlay.h
  gnomonViewVolumicPool.h
  gnomonVisualizationImagesSerie.h
  gnomonVisualizationImagesSerie
  gnomonVisualizationMesh.h
  gnomonVisualizationMesh
  gnomonWidgets
  gnomonWidgetsParameter
  gnomonWidgetsParameter.h
  gnomonWorkspace
  gnomonWorkspace.h
  gnomonWorkspaceBrowser
  gnomonWorkspaceBrowser.h
  gnomonWorkspaceFusion
  gnomonWorkspaceFusion.h
  gnomonWorkspacePreprocess
  gnomonWorkspacePreprocess.h
  gnomonWorkspaceRegistration
  gnomonWorkspaceRegistration.h
  gnomonWorkspaceSimulation
  gnomonWorkspaceSimulation.h
  gnomonWorkspaceSegmentation
  gnomonWorkspaceSegmentation.h
  gnomonWorkspaceTemplate_p.h
  gnomonWorkspaceTemplate_p.tpp)

set(${PROJECT_NAME}_SOURCES
  gnomonAbstractVisualization.cpp
  gnomonCodeEditor.cpp
  gnomonDropSite.cpp
  gnomonFinder.cpp
  gnomonFormManager.cpp
  gnomonFormManagerFocus.cpp
  gnomonFormManagerItem.cpp
  gnomonGridLayout.cpp
  gnomonImageManager.cpp
  gnomonImageManagerData.cpp
  gnomonImageManagerItem.cpp
  gnomonImageManagerFocus.cpp
  gnomonItemButton.cpp
  gnomonLandmarkActor.cpp
  gnomonOverlayPane.cpp
  gnomonOverlayPaneItem.cpp
  gnomonToolBar.cpp
  gnomonViewForm.cpp
  gnomonViewVolumic.cpp
  gnomonViewVolumicOverlay.cpp
  gnomonViewVolumicPool.cpp
  gnomonVisualizationMesh.cpp
  gnomonVisualizationImagesSerie.cpp
  gnomonWidgetsParameter.cpp
  gnomonWorkspace.cpp
  gnomonWorkspaceBrowser.cpp
  gnomonWorkspaceFusion.cpp
  gnomonWorkspaceSegmentation.cpp
  gnomonWorkspaceSimulation.cpp
  gnomonWorkspacePreprocess.cpp
  gnomonWorkspaceRegistration.cpp)

set(${PROJECT_NAME}_SOURCES_QRC
  gnomonWidgets.qrc)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON AND DTK_PYTHON_WRAPPER MATCHES "SWIG")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonWidgets.i.in ${CMAKE_BINARY_DIR}/gnomonWidgets.i @ONLY)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonwidgets python ${CMAKE_BINARY_DIR}/gnomonWidgets.i "" "${dtk_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}/src/;${dtkImaging_INCLUDE_DIRS};${dtk_WRAP_PREFIX}/dtkBase;${dtk_WRAP_PREFIX}/dtkCore;${dtk_WRAP_PREFIX}/QtCore;${dtk_WRAP_PREFIX}/QtWidgets;${dtk_WRAP_PREFIX}/dtkWidgets;${dtkImaging_WRAP_PREFIX}/dtkImagingCore/;${CMAKE_SOURCE_DIR}/src")

    set_property(SOURCE gnomonWidgets_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
  endif()
endif()

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_RCC})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME} gnomonCore)
target_link_libraries(${PROJECT_NAME} gnomonVisualization)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

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
        DEPENDS ${PROJECT_SOURCE_DIR}/gnomonWidgets.i
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/gnomonwidgets.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

      set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomonwidgets.py)

      if(APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/gnomonWidgets.i
          COMMAND ln -sf ../../libgnomonWidgets.dylib _gnomonwidgets.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonwidgets.so)
      endif(APPLE)

      if(UNIX AND NOT APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/gnomonWidgets.i
          COMMAND ln -fs ../../libgnomonWidgets.so _gnomonwidgets.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonwidgets.so)
      endif(UNIX AND NOT APPLE)

    endif()

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-widgets)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT gnomon-widgets
  EXPORT gnomonDepends)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
    install(FILES ${CMAKE_BINARY_DIR}/gnomonWidgets.i DESTINATION wrp/gnomonWidgets/)
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
