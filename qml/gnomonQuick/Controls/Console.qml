import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

// registered in x_logger_initialise
import xLogger          1.0 as L

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control
{
    id: _logger;

    signal itemsChanged();

    ColumnLayout {

        anchors.fill: parent;

        spacing: 0;

        RowLayout {

            Layout.fillWidth: true;
            Layout.minimumHeight: 32;
            Layout.maximumHeight: 32;

            Label {
                text: "Type"
                color: G.Style.colors.textColorBase

                Layout.leftMargin: 10;
                Layout.minimumWidth: 64;
                Layout.maximumWidth: 64;
            }

            G.Separator { Layout.fillHeight: true; }

            Label {
                text: "Description";
                color: G.Style.colors.textColorBase
                Layout.fillWidth: true;
            }

            G.Separator { Layout.fillHeight: true; }

            Label {
                text: "Time"
                color: G.Style.colors.textColorBase
                Layout.minimumWidth: 64;
                Layout.maximumWidth: 64;
            }
        }

        G.Separator {
            Layout.fillWidth: true;
        }

        ListView {

            id: _console_list;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            clip: true;

            model: L.Controller.items;

            delegate: G.ConsoleDelegate {

            }

            ScrollBar.vertical: ScrollBar { visible: _console_list.contentHeight > _console_list.height; }
        }

        G.Separator {
            Layout.fillWidth: true;
        }

        RowLayout {
            Layout.fillWidth: true;
            Layout.leftMargin: 8;
            Layout.rightMargin: 8;
            Layout.minimumHeight: 32;
            Layout.maximumHeight: 32;

            spacing: 10;

            G.Button {
              text: L.Controller.infoCount;
              down: true
              type: G.Style.ButtonType.OK
              //foregroundColor: G.Style.colors.infoColor;
              iconName: "information";

              Layout.minimumWidth: 46;
              Layout.maximumWidth: 46;
          }

            G.Button {
              text: L.Controller.debugCount;
              down: true
              type: G.Style.ButtonType.Base
              //foregroundColor: G.Style.colors.debugColor;
              iconName: "information";

              Layout.minimumWidth: 46;
              Layout.maximumWidth: 46;
            }

            G.Button {
              text: L.Controller.warningCount;
              down: true
              type: G.Style.ButtonType.Warning
              //foregroundColor: G.Style.colors.warningColor;
              iconName: "dots-triangle";

              Layout.minimumWidth: 46;
              Layout.maximumWidth: 46;
          }

            G.Button {
              text: L.Controller.errorCount;
              down: true
              type: G.Style.ButtonType.Danger
              //foregroundColor: G.Style.colors.errorColor;
              iconName: "cross";

              Layout.minimumWidth: 46;
              Layout.maximumWidth: 46;
          }

            G.Separator { Layout.fillHeight: true; }

            Item {
                Layout.fillWidth: true;
            }

            G.Separator { Layout.fillHeight: true; }

            G.Button {
              text: 'Clear';
              type: G.Style.ButtonType.OK
              implicitHeight: 24;

              onClicked: L.Controller.clear();
            }
        }
    }

    Connections {
        target: L.Controller;

        function onItemsChanged() {
            _logger.itemsChanged();
        }
    }

    background: Rectangle {
        color: G.Style.colors.gutterColor;
    }
}
