import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import Qt.labs.platform 1.1
import Qt.labs.folderlistmodel 2.15
import Qt.labs.settings 1.0

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control
{
    id: _self;

    property alias folder: _folder_model.folder;
    property alias extensionFilters: _filter_combobox.model;
    property string selectedFolder: "";
    property var selectedFiles: [];
    property bool shiftPressed: false;

    signal folderClicked(string folderUrl)
    signal fileClicked(string fileUrl)
    signal folderDoubleClicked(string folderUrl)
    signal fileDoubleClicked(string fileUrl)
    signal fileRightClicked(string fileUrl)

    hoverEnabled: true;

    background: Rectangle {
        color: G.Style.colors.baseColor;
    }

    Settings {

        id: _settings;
        property string last_path;
        property string mode: "grid";

    }

    QtObject {
        id: _internal;

      property var history: [_self.folder];
      property int history_index: 0;

      property var icons: [
        {
          name: "left",
          icon: "keyboard_arrow_left",
          isActive: _internal.history_index > 0,
        },
        {
          name: "right",
          icon: "keyboard_arrow_right",
          isActive: _internal.history_index < _internal.history.length - 1,
        },
        {
          name: "up",
          icon: "keyboard_arrow_up",
          isActive: true,
        },
        {
          name: "home",
          icon: "home",
          isActive: true,
        },
      ]

      property var modes: [
        {
          name: "grid",
          icon: "apps",
        },
        {
          name: "list",
          icon: "list",
        }
      ]

      property var extensions: [];

    }

    FolderListModel {
        id: _folder_model
        nameFilters: _internal.extensions;
    }


    Control {
        id: _tool_bar;

        anchors.right: _self.right;
        anchors.left: _self.left;

        height: 33;

        hoverEnabled: true;

        background: Rectangle {
            color: G.Style.colors.baseColor;
        }

      Rectangle {
        anchors.bottom: _tool_bar.bottom;
        anchors.left: _tool_bar.left;
        anchors.right: _tool_bar.right;

        color: G.Style.colors.bgColor;

        implicitWidth: 1;
        implicitHeight: 1;
      }

        RowLayout {

          anchors.fill: _tool_bar;
          anchors.centerIn: _tool_bar;

          Row {
            id: _tool_bar_icons;
            Layout.fillHeight: true;
            padding: 3;

            Repeater {
              model: _internal.icons;
              Control {
                id: _icon_rectangle;

                height: 24;
                width: 24;

                background: Rectangle {
                  color: _icon_rectangle.hovered && modelData.isActive ? G.Style.colors.bgColor : G.Style.colors.baseColor;
                  radius: 9;
                }

                hoverEnabled: true;


                MouseArea {
                  id: _icon_mouse_area

                  anchors.fill: _icon_rectangle;

                  onClicked: {
                    if(modelData.isActive) _self.toolBarButtonHandler(modelData.name)
                  }

                }

                G.Icon {
                  id: _icon;

                  anchors.centerIn: _icon_rectangle;

                  icon: modelData.icon;
                  color: G.Style.colors.fgColor;
                  opacity: modelData.isActive ? 1 : 0.4;
                  size: _icon_rectangle.width * 0.8;

                }

              }
            }

          }


          Rectangle {
            Layout.fillHeight: true;

            color: G.Style.colors.bgColor;

            implicitWidth: 1;
            implicitHeight: 1;
          }


          TextField {
            id: _text_field;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            font.pointSize: 10;

            text: _folder_model.folder;

          }

          Rectangle {
            Layout.fillHeight: true;

            color: G.Style.colors.bgColor;

            implicitWidth: 1;
            implicitHeight: 1;
          }

          Row {
            id: _tool_bar_icons_r;
            Layout.fillHeight: true;
            padding: 3;

            Repeater {
              model: _internal.modes;
              Control {
                id: _mode_rectangle;

                height: 24;
                width: 24;

                background: Rectangle {
                  color: _settings.mode === modelData.name ? G.Style.colors.hoveredBaseColor : G.Style.colors.baseColor;
                  radius: 9;
                }

                hoverEnabled: true;


                MouseArea {
                  id: _mode_mouse_area

                  anchors.fill: _mode_rectangle;

                  onClicked: {
                    if(modelData.name !== _internal.mode) _self.toolBarButtonHandler(modelData.name)
                  }

                }

                G.Icon {
                  id: _icon;

                  anchors.centerIn: _mode_rectangle;

                  icon: modelData.icon;
                  color: G.Style.colors.fgColor;
                  size: _mode_rectangle.width * 0.6;

                }

              }
            }

          }

        }
    }


  Loader {
    id: _loader

    anchors.top: _tool_bar.bottom;
    anchors.bottom: _filter_combobox.top;
    anchors.right: _self.right;
    anchors.left: _self.left;
  }

  ComboBox {
    id: _filter_combobox;

    textRole: "text";
    valueRole: "regexp";

    height: 24;
    //anchors.top: _loader.bottom;
    anchors.bottom: _self.bottom;
    anchors.right: _self.right;
    anchors.left: _self.left;

    visible: _self.extensionFilters ? true : false;

    onActivated: {
      _internal.extensions = _filter_combobox.currentValue.split(",")
    }
  }

  Connections {
    target: _loader.item

    function onFileClicked(fileUrl) {
      _self.selectedFolder = ""
      if( _self.shiftPressed) {
        console.log("shift pressed ..... ")
        _self.selectedFiles.append(fileUrl)
      } else {
        _self.selectedFiles = [fileUrl]
      }
      _self.fileClicked(_self.selectedFiles)
    }

    function onFileDoubleClicked(fileUrl) {
      if( _self.shiftPressed) {
        console.log("shift pressed ..... ")
        _self.selectedFiles.append(fileUrl)
      } else {
        _self.selectedFiles = [fileUrl]
      }

      //TODO check duplicates!!!
      _self.fileDoubleClicked(_self.selectedFiles)
    }

    function onFileRightClicked(fileUrl) {
      _self.selectedFolder = ""
      _self.selectedFiles = [fileUrl]
      _self.fileRightClicked(fileUrl)
    }

    function onFolderClicked(folderUrl) {
      _self.selectedFolder = folderUrl
      _self.selectedFiles = []
      _self.folderClicked(folderUrl)
    }

    function onFolderDoubleClicked(folderUrl) {
      _self.openFolder(folderUrl)
    }

  }

  function openFolder(folderUrl) {
      _self.folderDoubleClicked(folderUrl)
      _self.pushHistory(folderUrl)
      _self.folder = folderUrl
  }

  function pushHistory(folderName) {
    _internal.history_index = _internal.history_index + 1
    _internal.history  = _internal.history.slice(0, _internal.history_index).concat([folderName])
  }

  function next() {
    _internal.history_index = _internal.history_index + 1
    _self.folder = _internal.history[_internal.history_index]
  }

  function previous() {
    _internal.history_index = _internal.history_index - 1
    _self.folder = _internal.history[_internal.history_index]
  }

  function toolBarButtonHandler(buttonName) {
    if(buttonName === "up") {
      _self.folder=_folder_model.parentFolder
      _self.pushHistory(_self.folder)
    }

    if(buttonName === "home") {
      _self.folder=StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];
      _self.pushHistory(_self.folder)
    }

    if(buttonName === "left") {
      _self.previous()
    }


    if(buttonName === "right") {
      _self.next()
    }

    if(buttonName === "grid") {
      _settings.mode = buttonName
      _loader.setSource("FinderGridView.qml", {"model": _folder_model})
    }

    if(buttonName === "list") {
      _settings.mode = buttonName
      _loader.setSource("FinderListView.qml", {"model": _folder_model})
    }

  }

  Component.onCompleted: {
    if(_settings.last_path) _self.folder = _settings.last_path;
    else _self.folder = StandardPaths.standardLocations(StandardPaths.HomeLocation)[0];

    if(_settings.mode === "grid") _loader.setSource("FinderGridView.qml", {"model": _folder_model})
    if(_settings.mode === "list") _loader.setSource("FinderListView.qml", {"model": _folder_model})

    // for(let i = 0; i < _self.extensionFilters.count; i++) {

    //   console.log("MODEL", extensionFilters.get(i).regexp.split())
    // }
  }


  Component.onDestruction: {
    _settings.last_path = _self.folder;
  }

}
