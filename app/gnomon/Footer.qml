import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls    as G
import gnomonQuick.Style       as G
import gnomonQuick.Workspaces  as G

G.ToolBar {

    id: _control;

    property string workspaceName: '';
    // property alias d: _internal;

    property int    baseSize:  G.Style.smallDelegateHeight;
    property int minimumSize:  G.Style.smallDelegateHeight;
    property int maximumSize: G.Style.smallPanelHeight;
    property bool start_state: true;

    location: G.Style.ToolBarLocation.Bottom
    state: 'UNANCHORED';

    Control {
        id: _anchor;

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;

        height: G.Style.controlHeight;

        G.Button {
            id: _current_workspace
            anchors.centerIn: parent

            implicitWidth: G.Style.extraLongButtonWidth
            text: _current_workspace.isHovered ? "Switch Workspace" : _control.workspaceName;
            type: G.Style.ButtonType.Base
            empty: true
            iconName: _current_workspace.isHovered ? "home-switch" : "";
            visible: !start_state;

            onClicked: {
                window.create_workspace_screenshot();
                _switch_workspace_dialog.open();
            }
        }

        G.Button {
            id: _workspace_add;

            anchors.right: parent.right;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.rightMargin: G.Style.mediumPadding;

            implicitWidth: G.Style.longButtonWidth

            type: G.Style.ButtonType.Neutral
            text: "New Workspace"
            iconName: "tab-plus";

            visible: !start_state;

            onClicked: {
                window.create_workspace_screenshot();
                _workspace_dialog.update_highlight();
                _workspace_dialog.open();
            }
        }
    }

    G.SwitchWorkspaceDialog {
        id: _switch_workspace_dialog;
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    states: [
        State {
            name: 'UNANCHORED'
            PropertyChanges {
                target: _control;
                height: _control.baseSize;
            }
        },
        State {
            name: 'ANCHORED'
            PropertyChanges {
                target: _control;
                height: _control.maximumSize;
            }
        }
    ]

    transitions: [
        Transition {
            to: "UNANCHORED"
            NumberAnimation{
                duration: 200
                properties: "height"
                easing.type: Easing.InCubic
            }
        },
        Transition {
            to: "ANCHORED"
            NumberAnimation{
                duration: 200
                properties: "height"
                easing.type: Easing.OutCubic
            }
        }
    ]
}
