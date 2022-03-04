import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick           1.0 as X
import xQuick.Controls  1.0 as X
import xQuick.Fonts     1.0 as X
import xQuick.Style     1.0 as X


Item {
    id: _self

    property bool show_description: true
    property var title: ""
    property var description: ""
    property var plugins: []

    ColumnLayout {
        spacing: 6
        anchors.fill: _self;

        X.Label {
            id: _title;

            Layout.fillWidth: true
            Layout.preferredHeight: _title.contentHeight * 1.2;
            Layout.minimumHeight: _title.contentHeight;
            horizontalAlignment: Text.AlignLeft;

            text: _self.title;

            font {
                weight: Font.Bold
                pointSize: 16;
            }
        }

        ScrollView {
            id: _description_view;

            Layout.fillWidth: true;
            Layout.fillHeight: true;
            Layout.preferredHeight: _self.height * 0.5;
            Layout.minimumHeight: _self.height * 0.3;

            clip: true;
            contentWidth: availableWidth

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical: ScrollBar {
                visible: _description.contentHeight > _description_view.height;
                anchors.right: _description_view.right;
            }

            X.Label {
                id: _description;

                width: _description_view.width;

                visible: _self.show_description
                text: _self.description
                wrapMode: Text.Wrap

                font {
                    pointSize: 12;
                }
            }
        }

        X.Label {
            id: _plugins_section_title;

            Layout.fillWidth: true;
            Layout.preferredHeight: _plugins_section_title.contentHeight * 1.2;
            Layout.minimumHeight: _plugins_section_title.contentHeight;

            visible: _self.show_description

            text: "Algorithms"
            font {
                weight: Font.Bold
                pointSize: 14;
            }
        }

        Rectangle {
            id: _plugins_section;

            Layout.fillWidth: true;
            Layout.preferredHeight: (_self.height) * 0.3;
            Layout.minimumHeight: (_self.height) * 0.2;

            visible: _self.show_description
            color: Qt.darker(X.Style.backgroundColor);

            ListView {
                id: _plugins;

                anchors.fill: _plugins_section;
                clip: true;
                focus: true
                model: _self.plugins

                delegate: ItemDelegate {
                    width: parent.width
                    height: 24
                    text: modelData

                    background: Rectangle {
                        opacity: enabled ? 0.8 : 0.3
                        color: Qt.lighter(X.Style.backgroundColor, 1.2)

                        Rectangle {
                            width: parent.width
                            height: 1
                            color: X.Style.borderColor;
                            anchors.bottom: parent.bottom
                        }
                    }
                }

                ScrollIndicator.vertical: ScrollIndicator { visible: _plugins.contentHeight > _plugins_section.height; }
            }
        }
    }
}
