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
    //required property int fileSize;
    required property bool fileIsDir;
    //required property var fileModified;

    property var flickable: null;
    property bool isSelected: false;

    height: 90;
    width: _self.height;

    background: Rectangle {
        color: _self.isSelected ? G.Style.colors.bgColor : X.Style.colors.baseColor;
        border.width: _mouse_area.containsMouse ? 1 : 0;
        border.color: G.Style.colors.embossColor;
        radius: 9;
    }

    Drag.active: _mouse_area.drag.active
    Drag.dragType: Drag.Automatic
    Drag.hotSpot: Qt.point(_self.height/2, _self.height/2);
    Drag.mimeData: {"text/uri-list": fileUrl }

    MouseArea {

        id: _mouse_area

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: _icon;

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

    G.Icon {
        id: _icon;

        anchors.centerIn: _self;

        icon: fileIsDir ? "folder" : "insert_drive_file"
        color: G.Style.colors.foregroundColor;
        size: 33;

        //TODO: EXPERIMENT WITH TOOLTIP POSITIONING
        ToolTip.visible: _mouse_area.containsMouse;
        ToolTip.text: fileName;
        ToolTip.delay: 500;

    }

    Label {
        id: _label;

        anchors.top: _icon.bottom;
        anchors.bottom: _self.bottom;
        anchors.margins: 12;
        anchors.horizontalCenter: _self.horizontalCenter

        width: _self.width - 6;


        text: fileName;
        wrapMode: Label.Wrap;
        horizontalAlignment: Text.AlignHCenter;
    }

    signal clicked(var mouse)
    signal doubleClicked()

    Component.onCompleted: {
        _mouse_area.clicked.connect((mouse) => { _self.clicked(mouse) })
        _mouse_area.doubleClicked.connect(() => { _self.doubleClicked() })
    }
}
