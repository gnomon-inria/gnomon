qt_add_qml_module(gnomonQuickIcons STATIC
URI
  gnomonQuick.Icons
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickIcons
QML_FILES
  Icons.js
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Icons
OUTPUT_TARGETS
  gnomonQuickIconsTargets
NO_CACHEGEN)

add_library(gnomonQuick::Icons ALIAS gnomonQuickIcons)

set(gnomonQuickIconsTargets ${gnomonQuickIconsTargets} gnomonQuickIcons)

install(TARGETS ${gnomonQuickIconsTargets} EXPORT gnomonQuickIcons-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickIcons-targets
FILE
  gnomonQuickIconsTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickIcons-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickIconsTargets.cmake)
