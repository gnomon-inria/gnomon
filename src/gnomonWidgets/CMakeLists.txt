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
  gnomonFormManagerData.h
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
  gnomonInterpreterJupyter
  gnomonInterpreterJupyter.h
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
  gnomonViewFormPool.h
  gnomonViewMatplotlib
  gnomonViewMatplotlib.h
  gnomonViewVolumic
  gnomonViewVolumic.h
  gnomonViewVolumicOverlay.h
  gnomonViewVolumicPool.h
  gnomonVisualizationCellComplex.h
  gnomonVisualizationCellComplex
  gnomonVisualizationCellImage.h
  gnomonVisualizationCellImage
  gnomonVisualizationCellImageMarchingCubes.h
  gnomonVisualizationCellImageMarchingCubes
  gnomonVisualizationImagesSerie.h
  gnomonVisualizationImagesSerie
  gnomonVisualizationImagesSerieChannelBlending.h
  gnomonVisualizationImagesSerieChannelBlending
  gnomonVisualizationMesh.h
  gnomonVisualizationMesh
  gnomonWidgets
  gnomonWidgetsParameter
  gnomonWidgetsParameter.h
  gnomonWorkspace
  gnomonWorkspace.h
  gnomonWorkspaceBrowser
  gnomonWorkspaceBrowser.h
  gnomonWorkspaceCellComplexFromCellImage
  gnomonWorkspaceCellComplexFromCellImage.h
  gnomonWorkspaceCellImageFilter
  gnomonWorkspaceCellImageFilter.h
  gnomonWorkspaceFusion
  gnomonWorkspaceFusion.h
  gnomonWorkspaceLSystemSimulator
  gnomonWorkspaceLSystemSimulator.h
  gnomonWorkspaceMeshFromImage
  gnomonWorkspaceMeshFromImage.h
  gnomonWorkspacePreprocess
  gnomonWorkspacePreprocess.h
  gnomonWorkspacePythonSimulator
  gnomonWorkspacePythonSimulator.h
  gnomonWorkspaceRegistration
  gnomonWorkspaceRegistration.h
  gnomonWorkspaceSimulation
  gnomonWorkspaceSimulation.h
  gnomonWorkspaceSegmentation
  gnomonWorkspaceSegmentation.h
  gnomonWorkspaceTreeAnalysis
  gnomonWorkspaceTreeAnalysis.h
  gnomonWorkspaceTemplate_p.h
  gnomonWorkspaceTemplate_p.tpp)

set(${PROJECT_NAME}_SOURCES
  gnomonAbstractVisualization.cpp
  gnomonCodeEditor.cpp
  gnomonDropSite.cpp
  gnomonFinder.cpp
  gnomonFormManager.cpp
  gnomonFormManagerData.cpp
  gnomonFormManagerFocus.cpp
  gnomonFormManagerItem.cpp
  gnomonGridLayout.cpp
  gnomonImageManager.cpp
  gnomonImageManagerData.cpp
  gnomonImageManagerItem.cpp
  gnomonImageManagerFocus.cpp
  gnomonInterpreterJupyter.cpp
  gnomonItemButton.cpp
  gnomonLandmarkActor.cpp
  gnomonOverlayPane.cpp
  gnomonOverlayPaneItem.cpp
  gnomonToolBar.cpp
  gnomonViewForm.cpp
  gnomonViewFormPool.cpp
  gnomonViewMatplotlib.cpp
  gnomonViewVolumic.cpp
  gnomonViewVolumicOverlay.cpp
  gnomonViewVolumicPool.cpp
  gnomonVisualizationCellComplex.cpp
  gnomonVisualizationCellImage.cpp
  gnomonVisualizationCellImageMarchingCubes.cpp
  gnomonVisualizationImagesSerie.cpp
  gnomonVisualizationImagesSerieChannelBlending.cpp
  gnomonVisualizationMesh.cpp
  gnomonWidgetsParameter.cpp
  gnomonWorkspace.cpp
  gnomonWorkspaceBrowser.cpp
  gnomonWorkspaceCellComplexFromCellImage.cpp
  gnomonWorkspaceCellImageFilter.cpp
  gnomonWorkspaceCellImageQuantification.cpp
  gnomonWorkspaceFusion.cpp
  gnomonWorkspaceLSystemSimulator.cpp
  gnomonWorkspaceMeshFromImage.cpp
  gnomonWorkspacePreprocess.cpp
  gnomonWorkspacePythonSimulator.cpp
  gnomonWorkspaceRegistration.cpp
  gnomonWorkspaceSegmentation.cpp
  gnomonWorkspaceSimulation.cpp
  gnomonWorkspaceTreeAnalysis.cpp)

set(${PROJECT_NAME}_SOURCES_QRC
  gnomonWidgets.qrc)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON)
    if(DTK_PYTHON_WRAPPER MATCHES "SIP")

      set(${PROJECT_NAME}_SIP_WRAPPERS
        sip${PROJECT_NAME}gnomonInterpreterJupyter.cpp
        sip${PROJECT_NAME}gnomonViewMatplotlib.cpp)

      dtk_wrap_sip(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonWidgets ${CMAKE_SOURCE_DIR}/sip/gnomonWidgets/gnomonWidgetsmod.sip "${${PROJECT_NAME}_SIP_WRAPPERS}" "${${PROJECT_NAME}_WRAP_DEPENDS}")

      set_property(SOURCE sipgnomonWidgetscmodule.cpp                           PROPERTY SKIP_AUTOMOC ON)
      set_property(SOURCE sipgnomonWidgetsgnomonInterpreterJupyter.cpp          PROPERTY SKIP_AUTOMOC ON)
      set_property(SOURCE sipgnomonWidgetsgnomonViewMatplotlib.cpp              PROPERTY SKIP_AUTOMOC ON)
    endif()

    if (DTK_PYTHON_WRAPPER MATCHES "SWIG")
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonWidgets.i.in ${CMAKE_BINARY_DIR}/gnomonWidgets.i @ONLY)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonwidgets python ${CMAKE_BINARY_DIR}/gnomonWidgets.i "" "${dtk_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}/src/;${dtkImaging_INCLUDE_DIRS};${dtk_WRAP_PREFIX}/dtkBase;${dtk_WRAP_PREFIX}/dtkCore;${dtk_WRAP_PREFIX}/QtCore;${dtk_WRAP_PREFIX}/QtWidgets;${dtk_WRAP_PREFIX}/dtkWidgets;${dtkImaging_WRAP_PREFIX}/dtkImagingCore/;${CMAKE_SOURCE_DIR}/src")

      set_property(SOURCE gnomonWidgets_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
    endif()
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

if(DTK_BUILD_WRAPPERS)
  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
  endif()
endif()

## ###################################################################
## Target properties
## ###################################################################

if(NOT SIP_FOUND)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
  set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${gnomon_VERSION}
                                                 SOVERSION ${gnomon_VERSION_MAJOR})

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

    if(DTK_PYTHON_WRAPPER MATCHES "SIP")
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

      if(APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
          COMMAND ln -sf ../../lib${PROJECT_NAME}.dylib ${PROJECT_NAME}.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
      endif(APPLE)

      if(UNIX AND NOT APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
          COMMAND ln -fs ../../lib${PROJECT_NAME}.so ${PROJECT_NAME}.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
      endif(UNIX AND NOT APPLE)
    endif()

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
