import QtQuick
import QtQuick.Controls
import QtQuick.Window

import gnomonQuick.Style as G
import gnomonQuick.Controls as G

import gnomon.Project   as  GP

Window {
    id: splashScreen
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen

    width: window.width/2
    height: window.height/2
    x: window.x + width/2
    y: window.y + height/2

    Rectangle {
        id : _splash_image
        anchors.fill: parent
        color: G.Style.colors.bgColor;

        Text {
            id: _loading_title
            text: "Loading your session..."
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: G.Style.mediumPadding
            font: G.Style.fonts.h3
            color: G.Style.colors.neutralColor
        }

        BusyIndicator {
            id: _busy_animation;

            anchors.left: parent.left;
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: _loading_title.bottom
            anchors.margins: G.Style.smallPadding

            palette.dark: G.Style.colors.neutralColor
            width: G.Style.thumbnailLarge
            height: width / 4
            running: true
        }

        Text {
            id: _current_loading
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: _busy_animation.bottom
            anchors.margins: G.Style.largePadding
            font: G.Style.fonts.cardText
            color: G.Style.colors.neutralColor
            text : "Loading..."
        }

        Rectangle {
            id: _banner;

            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.bottom;
            anchors.margins: G.Style.mediumPadding;

            radius: G.Style.panelRadius;
            color: G.Style.colors.gutterColor;
            border.color: G.Style.colors.fgColor;
            border.width: 1;

            width: G.Style.mediumPanelWidth;
            height:  G.Style.largeLabelHeight 
            
            G.ProgressBar {
                id: _banner_progress_bar

                anchors.left: parent.left;
                anchors.right: parent.right;
                anchors.bottom: parent.bottom;
                height: G.Style.largeLabelHeight;
                value: GP.SessionManager.loadingSessionProgress*100
                visible: true
                opacity: 0.5
                to: 100.
            }
        }

        Connections {
            target: GP.SessionManager
            function onLoadProgress(step) {
                _current_loading.text = "Loading... " + step
            }
        }
    }

    Component.onCompleted: visible = true
}