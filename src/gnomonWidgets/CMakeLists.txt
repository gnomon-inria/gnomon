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
  gnomonCodeEditor
  gnomonCodeEditor.h
  gnomonDropSite.h
  gnomonFinder.h
  gnomonGridLayout.h
  gnomonImageManager.h
  gnomonOverlayPane.h
  gnomonOverlayPaneItem.h
  gnomonToolBar.h
  gnomonViewVolumic.h
  gnomonWidgets
  gnomonWorkspace.h
  gnomonWorkspaceBrowser.h
  gnomonWorkspaceFusion.h
  gnomonWorkspaceSegmentation.h
  gnomonWorkspacePreprocess.h
  gnomonWorkspaceRegistration.h)

set(${PROJECT_NAME}_SOURCES
  gnomonCodeEditor.cpp
  gnomonDropSite.cpp
  gnomonFinder.cpp
  gnomonGridLayout.cpp
  gnomonImageManager.cpp
  gnomonOverlayPane.cpp
  gnomonOverlayPaneItem.cpp
  gnomonToolBar.cpp
  gnomonViewVolumic.cpp
  gnomonWorkspace.cpp
  gnomonWorkspaceBrowser.cpp
  gnomonWorkspaceFusion.cpp
  gnomonWorkspaceSegmentation.cpp
  gnomonWorkspacePreprocess.cpp
  gnomonWorkspaceRegistration.cpp)

set(${PROJECT_NAME}_SOURCES_QRC
  gnomonWidgets.qrc)

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

######################################################################
### CMakeLists.txt ends here
