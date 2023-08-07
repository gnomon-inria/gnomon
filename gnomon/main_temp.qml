import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import gnomonQuick.Style    as G
import gnomonQuick.Controls as G
import "." as G

//window containing the application
ApplicationWindow {
    id: window
    visible: true

    //title of the application
    title: qsTr("Gnomon")
    width: 640
    height: 480

    Text {
        text: "Miaou Port"
        color: G.Style.colors.baseColor
    }

    SplitView {

        id: _split;

        anchors.fill: parent;
        anchors.leftMargin: 0;
        anchors.rightMargin: 0;
        Drawel {
            id: drawel;
            SplitView.fillHeight: true;
            implicitWidth: 400;
        }

        StackLayout {
            id: stack_launcher

            currentIndex: 0

            SplitView.fillHeight: true;
            SplitView.fillWidth: true;

            StackLayout {
                id: workspaces;
            }
        }

        G.Drawer {
            id: drawer;
            SplitView.fillHeight: true;
            implicitWidth: 400;
        }
    }

    Component.onCompleted: {
        console.log("Style" , G.Style);
        console.log("Icons" , G.Icons);

        for (var p in G.Icons)
        {
            //if( typeof G.Style[p] != "function" )
            console.log(p + ":" + G.Icons[p]);
        }
        console.log(JSON.stringify(G.Icons))
        console.log(G.Style.mode);
    }
}
