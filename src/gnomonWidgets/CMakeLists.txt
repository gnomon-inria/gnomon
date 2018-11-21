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
  gnomonWorkspaceSimulation.h
  gnomonWorkspacePreprocess.h
  gnomonWorkspaceRegistration.h
  )

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

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME} gnomonCore)

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
