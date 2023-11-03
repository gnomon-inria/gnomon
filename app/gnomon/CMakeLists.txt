if(APPLE)
  set(MACOSX_BUNDLE_ICON_FILE gnomon.icns)
  set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/assets/gnomon.icns")
  set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif(APPLE)

qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  main.cpp
  MaterialPreview.hpp
  MaterialPreview.cpp
  gnomonSessionManager.h
  gnomonSessionManager.cpp
  gnomon.hpp
  gnomon.cpp
  ${app_icon_macos})

qt_add_qml_module(${PROJECT_NAME}
URI
  ${PROJECT_NAME}
VERSION
  ${PROJECT_VERSION}
QML_FILES
main.qml
main_temp.qml
  Drawel.qml
  Drawer.qml
  Footer.qml
  Header.qml
  Journl.qml
  Settgs.qml
RESOURCES
  assets/gnomon_logo.png
NO_CACHEGEN)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Qt6::Core
  Qt6::Concurrent
  Qt6::Qml
  Qt6::Quick
  Qt6::QuickControls2
  Qt6::WebEngineQuick
  Qt6::Widgets
  dtkLog
  dtkCore
  dtkImagingCore
  dtkScript
  xtensor
  xVis
  xLogger
  ${VTK_LIBRARIES}
  gnomonCore
  gnomonProject
  gnomonPipeline
  gnomonVisualization
  gnomonWorkspace
  gnomonQuickControls
  gnomonQuickMenus
  gnomonQuickMonaco
  gnomonQuickStyle
  gnomonQuickWorkspaces
  )


if(APPLE)
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework AppKit")

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.plist.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist)

  set_target_properties(${PROJECT_NAME} PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_GUI_IDENTIFIER fr.inria.${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
    MACOSX_BUNDLE_ICON_FILE "gnomon_logo.icns"
    MACOSX_BUNDLE_BUNDLE_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}
    MACOSX_BUNDLE_COPYRIGHT "inria"
    MACOSX_BUNDLE_INFO_STRING "Gnomon is a computational platform to analyse and simulate the development of life forms in 3D"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist
)


endif(APPLE)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  BUNDLE  DESTINATION bin)
