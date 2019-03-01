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

project(gnomonVisualization)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonActor
  gnomonActor.h
  gnomonActor2DCellImage
  gnomonActor2DCellImage.h
  gnomonActor2DImageWidget
  gnomonActor2DImageWidget.h
  gnomonActor2DImageRGBAWidget
  gnomonActor2DImageRGBAWidget.h
  gnomonActor2DPolyData.h
  gnomonActor2DPolyData
  gnomonActor2DPolyData.h
  gnomonActorImage
  gnomonActorImage.h
  gnomonActorImageVolume
  gnomonActorImageVolume.h
  gnomonActorImageRGBAVolume
  gnomonActorImageRGBAVolume.h
  gnomonActorMesh
  gnomonActorMesh.h
  gnomonActorMesh_p.h
  gnomonActorMeshCellComplex
  gnomonActorMeshCellComplex.h
  gnomonActorMeshCellGraph
  gnomonActorMeshCellGraph.h
  gnomonActorMeshCellImage
  gnomonActorMeshCellImage.h
  gnomonActorPolyData
  gnomonActorPolyData.h
  gnomonActorVolume
  gnomonActorVolume.h
  gnomonActorScalarBar
  gnomonActorScalarBar.h
  gnomonClutEditor
  gnomonClutEditor.h
  gnomonColorMapEditor.h
  gnomonCoreParameterColor
  gnomonCoreParameterColor.h
  gnomonDoubleEditor.h
  gnomonStringEditor.h
  gnomonDoubleRangeEditor.h
  gnomonImageDataChannelBlending
  gnomonImageDataChannelBlending.h
  gnomonInspector
  gnomonInspector.h
  gnomonInspectorCellGraph
  gnomonInspectorCellGraph.h
  gnomonInspectorSlicePlanes
  gnomonInspectorSlicePlanes.h
  gnomonInspectorImage
  gnomonInspectorImage.h
  gnomonInspectorImageProperties
  gnomonInspectorImageProperties.h
  gnomonInspectorMesh
  gnomonInspectorMesh.h
  gnomonInspectorViewTree
  gnomonInspectorViewTree.h
  gnomonInspectorViewWidget
  gnomonInspectorViewWidget.h
  gnomonInspectorVolume
  gnomonInspectorVolume.h
  gnomonLookupTable.h
  gnomonLookupTableEditor.h
  gnomonPolyData
  gnomonPolyData.h
  gnomonPolyDataCellComplex
  gnomonPolyDataCellComplex.h
  gnomonPolyDataCellImage
  gnomonPolyDataCellImage.h
  gnomonPolyDataMesh
  gnomonPolyDataMesh.h
  gnomonStringListEditor.h
  gnomonView
  gnomonView.h
  gnomonViewManager
  gnomonViewManager.h
  gnomonVisualization)

set(${PROJECT_NAME}_SOURCES
  gnomonActor.cpp
  gnomonActor2DCellImage.cpp
  gnomonActor2DImageWidget.cpp
  gnomonActor2DImageRGBAWidget.cpp
  gnomonActor2DPolyData.cpp
  gnomonActorImage.cpp
  gnomonActorImageVolume.cpp
  gnomonActorImageRGBAVolume.cpp
  gnomonActorMesh.cpp
  gnomonActorMeshCellComplex.cpp
  gnomonActorMeshCellGraph.cpp
  gnomonActorMeshCellImage.cpp
  gnomonActorPolyData.cpp
  gnomonActorVolume.cpp
  gnomonActorScalarBar.cpp
  gnomonClutEditor.cpp
  gnomonColorMapEditor.cpp
  gnomonCoreParameterColor.cpp
  gnomonStringEditor.cpp
  gnomonDoubleEditor.cpp
  gnomonDoubleRangeEditor.cpp
  gnomonImageDataChannelBlending.cpp
  gnomonInspector.cpp
  gnomonInspectorCellGraph.cpp
  gnomonInspectorSlicePlanes.cpp
  gnomonInspectorImage.cpp
  gnomonInspectorImageProperties.cpp
  gnomonInspectorMesh.cpp
  gnomonInspectorViewTree.cpp
  gnomonInspectorViewWidget.cpp
  gnomonInspectorVolume.cpp
  gnomonLookupTable.cpp
  gnomonLookupTableEditor.cpp
  gnomonPolyData.cpp
  gnomonPolyDataCellComplex.cpp
  gnomonPolyDataCellImage.cpp
  gnomonPolyDataMesh.cpp
  gnomonStringListEditor.cpp
  gnomonView.cpp
  gnomonViewManager.cpp)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON AND DTK_PYTHON_WRAPPER MATCHES "SWIG")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonVisualization.i.in ${CMAKE_BINARY_DIR}/gnomonVisualization.i @ONLY)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonvisualization python ${CMAKE_BINARY_DIR}/gnomonVisualization.i "" "${dtk_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}/src/;${dtkImaging_INCLUDE_DIRS};${dtk_WRAP_PREFIX}/dtkBase;${dtk_WRAP_PREFIX}/dtkCore;${dtk_WRAP_PREFIX}/QtCore;${dtk_WRAP_PREFIX}/QtWidgets;${dtk_WRAP_PREFIX}/dtkWidgets;${dtkImaging_WRAP_PREFIX}/dtkImagingCore/;${CMAKE_SOURCE_DIR}/src")

    set_property(SOURCE gnomonVisualization_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
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

target_link_libraries(${PROJECT_NAME} dtkImagingCore)
target_link_libraries(${PROJECT_NAME} dtkImagingWidgets)

target_link_libraries(${PROJECT_NAME} gnomonCore)
target_link_libraries(${PROJECT_NAME} gnomonForm)
target_link_libraries(${PROJECT_NAME} gnomonFonts)
target_link_libraries(${PROJECT_NAME} gnomonStyle)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

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
        DEPENDS ${PROJECT_SOURCE_DIR}/gnomonVisualization.i
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/gnomonvisualization.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

      set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomonvisualization.py)

      if(APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/gnomonVisualization.i
          COMMAND ln -sf ../../libgnomonVisualization.dylib _gnomonvisualization.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonvisualization.so)
      endif(APPLE)

      if(UNIX AND NOT APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/gnomonVisualization.i
          COMMAND ln -fs ../../libgnomonVisualization.so _gnomonvisualization.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonvisualization.so)
      endif(UNIX AND NOT APPLE)

    endif()

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-visualization)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT gnomon-visualization
         EXPORT gnomonDepends)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
    install(FILES ${CMAKE_BINARY_DIR}/gnomonVisualization.i DESTINATION wrp/gnomonVisualization/)
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
