import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G
import gnomonQuick.Icons    1.0 as G


Item {
    id: _self

    property bool show_description: true
    property var title: ""
    property var description: ""
    property var preview: ""
    property var plugins: []
    signal openWithAlgo(string algo_name)

    ColumnLayout {
        spacing: 6
        anchors.fill: _self;

        RowLayout {
            Layout.fillWidth: true

            Label {
                id: _title;
                Layout.preferredHeight: _title.contentHeight * 1.2;
                Layout.minimumHeight: _title.contentHeight;
                horizontalAlignment: Text.AlignLeft;

                text: _self.title;

                font: G.Style.fonts.header;
            }

            G.Icon {
                icon: G.Icons.icons["help-circle"];
                size: 18;
                color: hovered ? G.Style.colors.hoveredBaseColor : G.Style.colors.neutralColor;

                MouseArea {
                    anchors.fill: parent;

                    onClicked: {
                        // use _self.title  to choseen workspace page, once it's available
                        let formatted_title = _self.title.toLowerCase().replace(/\s+/g,'_')  + ".html";
                        Qt.openUrlExternally("https://gnomon.gitlabpages.inria.fr/gnomon/workspace/" + formatted_title);
                    }
                }
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
                visible: _desc_label.contentHeight + _desc_image.sourceSize.height > _description_view.height;
                anchors.right: _description_view.right;
            }
            ColumnLayout {
                spacing: G.Style.smallPadding
                anchors.fill: parent

                Image {
                    id: _desc_image
                    visible: _self.show_description
                    source: "qrc:/qml/gnomonQuick/assets/" + _self.preview
                    Layout.preferredWidth: _description_view.width;
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: G.Style.sizes.s11 - G.Style.smallPadding;
                }

                Label {
                    id: _desc_label
                    Layout.preferredWidth: _description_view.width;

                    visible: _self.show_description
                    text: _self.description
                    wrapMode: Text.Wrap

                    font: G.Style.fonts.value;
                }

            }
        }

        Label {
            id: _plugins_section_title;

            Layout.fillWidth: true;
            Layout.preferredHeight: _plugins_section_title.contentHeight * 1.2;
            Layout.minimumHeight: _plugins_section_title.contentHeight;

            visible: _self.show_description

            text: "Algorithms"
            font: G.Style.fonts.subHeader;
        }

        G.Gutter {
            id: _plugins_section;

            Layout.fillWidth: true;
            Layout.preferredHeight: (_self.height) * 0.3;
            Layout.minimumHeight: (_self.height) * 0.2;

            visible: _self.show_description

            ListView {
                id: _plugins;

                anchors.fill: _plugins_section;
                clip: true;
                focus: true
                model: _self.plugins

                delegate: G.ListItemDelegate {
                    width: parent.width
                    height: G.Style.smallLabelHeight
                    text: modelData
                    font: G.Style.fonts.value
                    hoverEnabled: true

                    onDoubleClicked: {
                        _plugins.currentIndex = index
                        openWithAlgo(modelData)
                    }
                }

                ScrollIndicator.vertical: ScrollIndicator { visible: _plugins.contentHeight > _plugins_section.height; }
            }
        }
    }
}
