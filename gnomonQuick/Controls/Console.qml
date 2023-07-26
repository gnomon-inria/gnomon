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

                Layout.leftMargin: 10;
                Layout.minimumWidth: 64;
                Layout.maximumWidth: 64;
            }

            G.Separator { Layout.fillHeight: true; }

            Label {
                text: "Description";

                Layout.fillWidth: true;
            }

            G.Separator { Layout.fillHeight: true; }

            Label {
                text: "Time"

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
                backgroundColor: G.Style.colors.bgColor;
                foregroundColor: G.Style.colors.infoColor;
                iconName: G.Icons.icons["info"];

                Layout.minimumWidth: 42;
                Layout.maximumWidth: 42;
            }

            G.Button {
                text: L.Controller.debugCount;
                backgroundColor: G.Style.colors.bgColor;
                foregroundColor: G.Style.colors.debugColor;
                iconName: G.Icons.icons["info"];

                Layout.minimumWidth: 42;
                Layout.maximumWidth: 42;
            }

            G.Button {
                text: L.Controller.warningCount;
                backgroundColor: G.Style.colors.bgColor;
                foregroundColor: G.Style.colors.warningColor;
                iconName: G.Icons.icons["warning"];

                Layout.minimumWidth: 42;
                Layout.maximumWidth: 42;
            }

            G.Button {
                text: L.Controller.errorCount;
                backgroundColor: G.Style.colors.bgColor;
                foregroundColor: G.Style.colors.errorColor;
                iconName: G.Icons.icons["error"];

                Layout.minimumWidth: 42;
                Layout.maximumWidth: 42;
            }

            G.Separator { Layout.fillHeight: true; }

            Item {
                Layout.fillWidth: true;
            }

            G.Separator { Layout.fillHeight: true; }

            G.Button {

                text: 'Clear';
                implicitHeight: 24;
                outlined: true;

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
        color: G.Style.colors.baseColor;
    }
}
