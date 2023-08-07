set_source_files_properties(Style.qml PROPERTIES
  QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(gnomonQuickStyle STATIC
URI
  gnomonQuick.Style
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickStyle
QML_FILES
  Style.qml
  style.qml.js
RESOURCES
  Poppins-Black.ttf
  Poppins-BlackItalic.ttf
  Poppins-Bold.ttf
  Poppins-BoldItalic.ttf
  Poppins-ExtraBold.ttf
  Poppins-ExtraBoldItalic.ttf
  Poppins-ExtraLight.ttf
  Poppins-ExtraLightItalic.ttf
  Poppins-Italic.ttf
  Poppins-Light.ttf
  Poppins-LightItalic.ttf
  Poppins-Medium.ttf
  Poppins-MediumItalic.ttf
  Poppins-Regular.ttf
  Poppins-SemiBold.ttf
  Poppins-SemiBoldItalic.ttf
  Poppins-Thin.ttf
  Poppins-ThinItalic.ttf
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Style
OUTPUT_TARGETS
  gnomonQuickStyleTargets
NO_CACHEGEN)

add_library(gnomonQuick::Style ALIAS gnomonQuickStyle)

set(gnomonQuickStyleTargets ${gnomonQuickStyleTargets} gnomonQuickStyle)

install(TARGETS ${gnomonQuickStyleTargets} EXPORT gnomonQuickStyle-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickStyle-targets
FILE
  gnomonQuickStyleTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickStyle-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickStyleTargets.cmake)
