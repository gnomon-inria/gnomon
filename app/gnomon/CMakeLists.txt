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

project(gnomon)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonDropSite.h
  gnomonFinder.h
  gnomonGridLayout.h
  gnomonImageManager.h
  gnomonMainWindow.h
  gnomonOverlayPane.h
  gnomonOverlayPaneItem.h
  gnomonToolBar.h
  gnomonViewVolumic.h
  gnomonWorkspace.h
  gnomonWorkspaceBrowser.h
  gnomonWorkspaceFusion.h
  gnomonWorkspaceSegmentation.h
  gnomonWorkspacePreprocess.h
  gnomonWorkspaceRegistration.h)

set(${PROJECT_NAME}_SOURCES
  gnomonDropSite.cpp
  gnomonFinder.cpp
  gnomonGridLayout.cpp
  gnomonImageManager.cpp
  gnomonMainWindow.cpp
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
  gnomon.qrc)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME}_backend SHARED
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_RCC})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  main.cpp)

qt5_use_modules(${PROJECT_NAME}_backend Core)
qt5_use_modules(${PROJECT_NAME}_backend Widgets)
qt5_use_modules(${PROJECT_NAME}_backend OpenGL)

target_link_libraries(${PROJECT_NAME}_backend dtkCore)
target_link_libraries(${PROJECT_NAME}_backend dtkScript)
target_link_libraries(${PROJECT_NAME}_backend dtkWidgets)
target_link_libraries(${PROJECT_NAME}_backend dtkComposer)

target_link_libraries(${PROJECT_NAME}_backend dtkImagingCore)

target_link_libraries(${PROJECT_NAME}_backend gnomonCore)
target_link_libraries(${PROJECT_NAME}_backend gnomonStyle)
target_link_libraries(${PROJECT_NAME}_backend gnomonComposer)
target_link_libraries(${PROJECT_NAME}_backend gnomonFonts)
target_link_libraries(${PROJECT_NAME}_backend gnomonWidgets)

target_link_libraries(${PROJECT_NAME}_backend ${VTK_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_backend)

## #################################################################
## Bundle setup
## #################################################################

if(APPLE)
  set(${PROJECT_NAME}_RESOURCE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/Resources)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${${PROJECT_NAME}_RESOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.icns ${${PROJECT_NAME}_RESOURCE_DIR})
endif(APPLE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.plist.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist)

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist)
endif(APPLE)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}_backend
  EXPORT gnomonDepends
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)

######################################################################
### CMakeLists.txt ends here
