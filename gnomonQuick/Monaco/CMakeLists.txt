qt_add_qml_module(gnomonQuickMonaco STATIC
URI
  gnomonQuick.Monaco
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickMonaco
QML_FILES
  Monaco.qml
  monaco.js
RESOURCES
  monaco.html
  dist/client/b797181c93b3755f4fa1.ttf
  dist/client/main.d.ts
  dist/client/main.d.ts.map
  dist/client/main.js
  dist/client/main.js.map
  dist/client/node_modules_monaco-editor_esm_vs_basic-languages_python_python_js.main.js
  dist/client/node_modules_monaco-editor_esm_vs_basic-languages_python_python_js.main.js.map
  dist/client/src_client_lpy_js.main.js
  dist/client/src_client_lpy_js.main.js.map
  dist/client/workers/editorWorker-es.js
  dist/client/workers/editorWorker-iife.js
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Monaco
OUTPUT_TARGETS
  gnomonQuickMonacoTargets
NO_CACHEGEN)

add_library(gnomonQuick::Monaco ALIAS gnomonQuickMonaco)

set(gnomonQuickMonacoTargets ${gnomonQuickMonacoTargets} gnomonQuickMonaco)

install(TARGETS ${gnomonQuickMonacoTargets} EXPORT gnomonQuickMonaco-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickMonaco-targets
FILE
  gnomonQuickMonacoTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickMonaco-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickMonacoTargets.cmake)
