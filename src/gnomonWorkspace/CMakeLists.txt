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

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Gui)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkCore)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkFonts)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkThemes)

target_link_libraries(${PROJECT_NAME} PRIVATE gnomonCore)
target_link_libraries(${PROJECT_NAME} PRIVATE gnomonWidgets)
# target_link_libraries(${PROJECT_NAME} PRIVATE gnomonVisualization)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

## #############################################################################
##
## #############################################################################

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${layer_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

## ###################################################################
## Target properties
## ###################################################################

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

install(TARGETS ${PROJECT_NAME} EXPORT layer-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
