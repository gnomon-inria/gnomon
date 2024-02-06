qt_add_qml_module(gnomonQuickControls STATIC
URI
  gnomonQuick.Controls
VERSION
  1.0
PLUGIN_TARGET
  gnomonQuickControls
QML_FILES
  Application.qml
  BoolParameter.qml
  Button.qml
  Card.qml
  CheckBox.qml
  CheckComboBox.qml
  CheckComboBoxDelegate.qml
  ClutGradient.qml
  CollapsiblePanel.qml
  ColorMapSelector.qml
  ColorTableParameter.qml
  Colormap.qml
  ComboBox.qml
  ComboBoxDelegate.qml
  ComboBoxWithLabel.qml
  Console.qml
  ConsoleDelegate.qml
  TextView.qml
  Dialog.qml
  Dragger.qml
  Figure.qml
  FilterProxyModel.qml
  FormIdentifier.qml
  FormSelector.qml
  FormSelectorDelegate.qml
  GraphicalParameter.qml
  SessionSettings.qml
  Gutter.qml
  Icon.qml
  Icons.js
  IconButton.qml
  InList.qml
  InListStringList.qml
  ListItemDelegate.qml
  Logs.qml
  LookupTable.qml
  Menu.qml
  MenuItemDelegate.qml
  MetadataDialog.qml
  MetadataExplorer.qml
  NumericParameter.qml
  NumericSlider.qml
  NumericSpinBox.qml
  NurbsParameter.qml
  Page.qml
  Panel.qml
  ParameterGroup.qml
  Parameters.qml
  PathParameter.qml
  Pipeline.qml
  pipeline.js
  PipelineDialog.qml
  PipelineEdge.qml
  PipelineNode.qml
  PipelineNodeDialog.qml
  PipelinePort.qml
  PipelinePortEdit.qml
  Project.qml
  ProjectBrowser.qml
  ProjectImportDialog.qml
  ProgressBar.qml
  RangeParameter.qml
  RangeSlider.qml
  ScrollIndicator.qml
  Separator.qml
  SimpleView.qml
  Slider.qml
  SquareButton.qml
  Stack.qml
  StringParameter.qml
  Switch.qml
  TabButton.qml
  TextArea.qml
  TextField.qml
  Thumbnail.qml
  TimeSeriesSlider.qml
  Toast.qml
  ToolBar.qml
  ToolTip.qml
  TreeView.qml
  utils.js
  View.qml
  ViewParameters.qml
  World.qml
  WorldDelegate.qml
RESOURCES
  View-XY.png
  View-XY.pxm
  View-XY-off.png
  View-XY-off.pxm
  View-XZ.png
  View-XZ.pxm
  View-XZ-off.png
  View-XZ-off.pxm
  View-YZ.png
  View-YZ.pxm
  View-YZ-off.png
  View-YZ-off.pxm
  materialdesignicons-webfont.ttf
OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/qml/gnomonQuick/Controls
OUTPUT_TARGETS
  gnomonQuickControlsTargets
NO_CACHEGEN)

add_library(gnomonQuick::Controls ALIAS gnomonQuickControls)

set(gnomonQuickControlsTargets ${gnomonQuickControlsTargets} gnomonQuickControls)

install(TARGETS ${gnomonQuickControlsTargets} EXPORT gnomonQuickControls-targets
  OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT gnomonQuickControls-targets
FILE
  gnomonQuickControlsTargets.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/gnomonQuick)

export(EXPORT gnomonQuickControls-targets
FILE
  ${CMAKE_BINARY_DIR}/gnomonQuickControlsTargets.cmake)
