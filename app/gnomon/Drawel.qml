import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G

G.Page {

    id: _self;

    state: window.drawelr_closed ? "closed": "open";

    property int minimumSize:  10;
    property int maximumSize: 400;
    property var menu: null;

    default property alias _contentChildren: _content.data

    padding: 10;

    function opened() { return state === "open"; }
    function open() { _self.state = "open"; }
    function close() { _self.state = "closed"; }

    Item {
        id: _content;

        anchors.fill: parent;
    }

    // background: Rectangle {
    //     color: Qt.platform.os === "osx" ? "#00000000" : X.Style.baseColor;
    // }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    function update_menu()
    {
        if (menu)
            menu.destroy();

        var workspace = window.current_workspace();
        var source;
        var prop;

        if(workspace.workspace_title === "Python Algorithm" & workspace.d.editMode)
        {
            source = "qrc:/qt/qml/gnomonQuick/Menus/MonacoMenu.qml"
            prop = { d: workspace.d }
        } else {
            source = "qrc:/qt/qml/gnomonQuick/Menus/ViewMenu.qml";
            var view = window.currentView;
            prop = { view: view }
        }

        var menu_component = Qt.createComponent(source)

        if (menu_component.status == Component.Ready) {
            menu = menu_component.createObject(_content, prop);
            menu.anchors.fill = _content;
        } else {
            console.error(menu_component.errorString());
        }
    }

    states: [
        State{
            name: "closed"
            PropertyChanges {
                target: _self;
                SplitView.preferredWidth: _self.minimumSize;
            }
            PropertyChanges {
                target: _content;
                opacity: 0;
            }
        },
        State{
            name: "open"
            PropertyChanges {
                target: _self;
                SplitView.preferredWidth: _self.maximumSize;
            }
            PropertyChanges {
                target: _content;
                opacity: 1;
            }
        }
    ]

    transitions: [
        Transition {
            to: "closed"
            NumberAnimation{
                duration: 200
                properties: "SplitView.preferredWidth, opacity"
                easing.type: Easing.InCubic
            }
        },
        Transition {
            to: "open"
            NumberAnimation{
                duration: 200
                properties: "SplitView.preferredWidth, opacity"
                easing.type: Easing.OutCubic
            }
        }
    ]

}
