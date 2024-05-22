qt_add_qml_module(gnomonQuickWorkspaces STATIC
URI
  gnomonQuick.Workspaces
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickWorkspaces
QML_FILES
  PythonPluginDialog.qml
  ReaderDialog.qml
  SwitchWorkspaceDialog.qml
  Workspace.qml
  WorkspaceBinarization.qml
  WorkspaceBrowsing.qml
  WorkspaceCellImageQuantification.qml
  WorkspaceCellImageTracking.qml
  WorkspaceDescriptionDelegate.qml
  WorkspaceDialog.qml
  WorkspaceImageMeshing.qml
  WorkspaceLSystemModel.qml
  WorkspaceLauncher.qml
  WorkspaceMeshFilter.qml
  WorkspaceMorphonet.qml
  WorkspacePointCloudQuantification.qml
  WorkspacePointDetection.qml
  WorkspacePreprocess.qml
  WorkspacePythonAlgorithm.qml
  WorkspaceRegistration.qml
  WorkspaceSegmentation.qml
  WorkspaceSimulation.qml
  WorkspaceCellImageFilter.qml
RESOURCES
  gnomonWorkspaceBinarization.png
  gnomonWorkspaceBrowser.png
  gnomonWorkspaceCellImageFilter.png
  gnomonWorkspaceCellImageQuantification.png
  gnomonWorkspaceCellImageTracking.png
  gnomonWorkspaceImageMeshing.png
  gnomonWorkspaceLSystemModel.png
  gnomonWorkspaceMeshFilter.png
  gnomonWorkspacePointCloudQuantification.png
  gnomonWorkspacePointDetection.png
  gnomonWorkspacePreprocess.png
  gnomonWorkspacePythonAlgorithm.png
  gnomonWorkspaceRegistration.png
  gnomonWorkspaceSegmentation.png
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Workspaces
OUTPUT_TARGETS
  gnomonQuickWorkspacesTargets
NO_CACHEGEN)

add_library(gnomonQuick::Workspaces ALIAS gnomonQuickWorkspaces)

set(gnomonQuickWorkspacesTargets ${gnomonQuickWorkspacesTargets} gnomonQuickWorkspaces)

install(TARGETS ${gnomonQuickWorkspacesTargets} EXPORT gnomonQuickWorkspaces-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickWorkspaces-targets
FILE
  gnomonQuickWorkspacesTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickWorkspaces-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickWorkspacesTargets.cmake)
