qt_add_qml_module(gnomonQuickGraph STATIC
URI
  gnomonQuick.Graph
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickGraph
QML_FILES
  pipeline.js
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Graph
OUTPUT_TARGETS
  gnomonQuickGraphTargets
NO_CACHEGEN)

add_library(gnomonQuick::Graph ALIAS gnomonQuickGraph)

set(gnomonQuickGraphTargets ${gnomonQuickGraphTargets} gnomonQuickGraph)

install(TARGETS ${gnomonQuickGraphTargets} EXPORT gnomonQuickGraph-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickGraph-targets
FILE
  gnomonQuickGraphTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickGraph-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickGraphTargets.cmake)
