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

project(gnomonWorkspace)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
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
  gnomonWorkspacePointCloudFromImage
  gnomonWorkspacePointCloudFromImage.h
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
  gnomonWorkspace.cpp
  gnomonWorkspaceBrowser.cpp
  gnomonWorkspaceCellComplexFromCellImage.cpp
  gnomonWorkspaceCellImageFilter.cpp
  gnomonWorkspaceCellImageQuantification.cpp
  gnomonWorkspaceFusion.cpp
  gnomonWorkspaceLSystemSimulator.cpp
  gnomonWorkspaceMeshFromImage.cpp
  gnomonWorkspacePointCloudFromImage.cpp
  gnomonWorkspacePreprocess.cpp
  gnomonWorkspacePythonSimulator.cpp
  gnomonWorkspaceRegistration.cpp
  gnomonWorkspaceSegmentation.cpp
  gnomonWorkspaceSimulation.cpp
  gnomonWorkspaceTreeAnalysis.cpp)


## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_RCC})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME} gnomonCore)
target_link_libraries(${PROJECT_NAME} gnomonWidgets)
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

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-workspace)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT gnomon-workspace
  EXPORT gnomonDepends)


######################################################################
### CMakeLists.txt ends here
