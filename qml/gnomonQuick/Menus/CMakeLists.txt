qt_add_qml_module(gnomonQuickMenus STATIC
URI
  gnomonQuick.Menus
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickMenus
QML_FILES
PythonPluginMenu.qml
  ViewMenu.qml
  WorkspaceBrowsingMenu.qml
  WorkspaceLSystemModelMenu.qml
  WorkspaceMorphonetMenu.qml
  WorkspaceParameterMenu.qml
  WorkspacePythonAlgorithmMenu.qml
  gnomonVisualization.defaultMenu.qml
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Menus
OUTPUT_TARGETS
  gnomonQuickMenusTargets
NO_CACHEGEN)

add_library(gnomonQuick::Menus ALIAS gnomonQuickMenus)

set(gnomonQuickMenusTargets ${gnomonQuickMenusTargets} gnomonQuickMenus)

install(TARGETS ${gnomonQuickMenusTargets} EXPORT gnomonQuickMenus-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickMenus-targets
FILE
  gnomonQuickMenusTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickMenus-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickMenusTargets.cmake)
