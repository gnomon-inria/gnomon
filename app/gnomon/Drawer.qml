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

    function opened() { return state === "open"; }
    function open() { _self.state = "open"; }
    function close() { _self.state = "closed"; }

    Item {
        id: _content;
        anchors.fill: parent;
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    function update_menu(source)
    {
        if (menu)
            menu.destroy();

        var workspace = window.current_workspace();

        var menu_component = Qt.createComponent(source)
        if(menu_component.status == Component.Error) {
            if (menu_component.errorString().indexOf("No such file or directory") == -1) {
                console.log("Error trying to create menu from", source, " : ", menu_component.errorString())
            }
        }
        if(menu_component.status != Component.Ready) {
            var specific_error_msg = menu_component.errorString()

            menu_component = Qt.createComponent("qrc:/qt/qml/gnomonQuick/Menus/WorkspaceParameterMenu.qml")

            if(menu_component.status != Component.Ready) {
                console.error("Can't create menu for", source, specific_error_msg, menu_component.errorString());
                return;
            }
        }

        var prop;
        if(menu_component.url.toString().includes("Morphonet")) {
            prop= { mn_ds_info: workspace.mn_ds_info, d: workspace.d,}
        } else {
            prop = { parameters: workspace.parameters, d: workspace.d,}
        }

        menu = menu_component.createObject(_content, prop);
        menu.anchors.fill = _content;
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
