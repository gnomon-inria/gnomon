//Qt
import QtQuick
import QtQuick.Controls

//Gnomon
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

    id: _control

    default property alias content: _panel.children

    //this comes in handy to size a panel wrt to the
    //its children, or any other thing that requires
    //to access the actual panel
    property Control panel: _panel;

    //padding: 2
    implicitWidth: G.Style.smallPanelWidth;
    implicitHeight: G.Style.smallPanelHeight;
    clip: true;

    background: Rectangle {
        radius: G.Style.panelRadius;
        color: G.Style.colors.gutterColor;
    }

    Control {

        id: _panel

        anchors.fill: parent;
        clip: true;

        leftInset: 2
        rightInset: 2
        topInset: 2
        bottomInset: 2

        leftPadding: 2
        rightPadding: 2
        topPadding: 2
        bottomPadding: 2

        background: Rectangle {
            color: G.Style.colors.bgColor;
            border.width: 0.5;
            border.color: G.Style.colors.embossColor;
            radius: G.Style.panelRadius;
        }

    }

}
