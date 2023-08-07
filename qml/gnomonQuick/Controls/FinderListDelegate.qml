import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

    id: _self;

    required property int index;
    required property string fileName;
    required property string fileUrl;
    required property int fileSize;
    required property bool fileIsDir;
    required property var fileModified;

    property var flickable: null;
    property bool isSelected: false;

    height: 24;

    background: Rectangle {
        color: _self.isSelected ? G.Style.colors.embossColor : G.Style.colors.gutterColor;
        border.width: _mouse_area.containsMouse ? 1: 0;
        border.color: G.Style.colors.embossColor;
    }

    Drag.active: _mouse_area.drag.active
    Drag.dragType: Drag.Automatic
    //Drag.hotSpot: Qt.point(_self.height/2, 0);
    Drag.mimeData: {"text/uri-list": fileUrl }

    MouseArea {

        id: _mouse_area

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: _layout;

        drag.target: _icon;
        drag.maximumX: _self.x;
        drag.maximumY: _self.y;
        hoverEnabled: true;

        onContainsMouseChanged: {
            flickable.interactive = !containsMouse;
        }

        onPressed: {
            _icon.grabToImage(function(result) {
                _self.Drag.imageSource = result.url
            }, Qt.size(_self.height,_self.height));
        }

    }

    RowLayout {
        id: _layout;

        anchors.fill: _self;


        Item {

            id: _name_and_icon

            Layout.fillWidth: true;
            Layout.fillHeight: true;
            Layout.minimumWidth: _icon.size + 60;
            Layout.preferredWidth: (_icon.size + _name.contentWidth) * 0.8;
            Layout.alignment: Qt.AlignVCenter;

            clip: true;

            G.Icon {
                id: _icon;

                anchors.verticalCenter: _name_and_icon.verticalCenter;

                icon: fileIsDir ? "folder" : "file"
                color: G.Style.colors.neutralColor;
                size: _self.height * 0.8;

                //TODO: EXPERIMENT WITH TOOLTIP POSITIONING
                ToolTip.visible: _mouse_area.containsMouse;
                ToolTip.text: fileName;
                ToolTip.delay: 500;

            }
            Label {
                id: _name;

                anchors.verticalCenter: _name_and_icon.verticalCenter;
                anchors.left: _icon.right;

                elide: Text.ElideRight;
                color: G.Style.colors.neutralColor
                text: fileName;
            }

        }

        Label {
            id: _size;

            Layout.minimumWidth: 10;
            Layout.preferredWidth: _size.contentWidth;

            clip: true;
            color: G.Style.colors.neutralColor
            text: createSizeText();

            function createSizeText() {
                if(fileSize > 10**9) return (fileSize / 10**9).toFixed(1) + " Go"
                if(fileSize > 10**6) return (fileSize / 10**6).toFixed(1) + " Mo"
                if(fileSize > 10**3) return (fileSize / 10**3).toFixed(1) + " Ko"
                return fileSize + " octets"
            }

        }

        Label {
            id: _modified;

            Layout.preferredWidth: _modified.contentWidth;

            clip: true;
            color: G.Style.colors.neutralColor
            text: createModifiedText();

            function createModifiedText() {
                const today = new Date()

                const diffYears = today.getFullYear() - fileModified.getFullYear()
                const diffMonths = today.getMonth() - fileModified.getMonth()
                const diffDays = today.getDate() - fileModified.getDate()

                if(diffYears === 0 && diffMonths === 0)
                {
                    if(diffDays === 0) return "today"
                    if(diffDays === 1) return "yesterday"
                }
                const month = fileModified.getMonth() < 10 ? "0" + fileModified.getMonth() : fileModified.getMonth()
                return fileModified.getDate() + "/" + month + "/" + fileModified.getFullYear()
            }
        }

    }

    signal clicked(var mouse)
    signal doubleClicked()

    Component.onCompleted: {
        _mouse_area.clicked.connect((mouse) => { _self.clicked(mouse); })
        _mouse_area.doubleClicked.connect(() => { _self.doubleClicked(); })
    }
}
