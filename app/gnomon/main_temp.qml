import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import gnomonQuick.Style    as G
//import gnomonQuick.Controls as G
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

    SwitchDelegate {
        width: parent.width;
        text: G.Style.mode == G.Style.Mode.Dark ? "Dark" : "Light";
        palette.text: G.Style.colors.textColorBase
        onToggled: {
            G.Style.mode = checked ? G.Style.Mode.Dark :  G.Style.Mode.Light;
            console.log("new mode", G.Style.mode)
        }
        checked: G.Style.mode = G.Style.Mode.Dark;
    }

    Component.onCompleted: {
        console.log("Style" , G.Style);

        //for (var p in G.Icons)
        //{
            //if( typeof G.Style[p] != "function" )
        //    console.log(p + ":" + G.Icons[p]);
        //}
        //console.log(JSON.stringify(G.Icons))
        console.log(G.Style.mode);
        console.log(G.Style.colors);
    }
}
