import QtQuick            2.15
import QtQuick.Controls   2.15
import QtQuick.Window     2.15
import QtQuick.Layouts    1.15

import gnomonQuick.Controls  1.0 as G
import gnomonQuick.Style     1.0 as G

import gnomon.Visualization 1.0 as GV

Rectangle {

    id: self;

    color: G.Style.colors.gutterColor;

    focus: true;

    property alias view: _view;
    property alias ts_slider: _ts_slider
    property var viewLogic;
    property bool export_enabled: !viewLogic.inputView

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    function forceFocus() {
        _text.forceActiveFocus()
    }

    ScrollView {
        id: _view;
        anchors.fill: parent
        anchors.margins: G.Style.smallPadding

        clip: true
        contentWidth: availableWidth

        TextEdit {
            id: _text

            height: Math.max(self.height - 2*G.Style.smallPadding, _text.implicitHeight)
            width: self.width - _scrollbar.width - 2*G.Style.smallPadding

            text: viewLogic? viewLogic.displayText : ""
            readOnly: true
            wrapMode: Text.WrapAnywhere
            selectByMouse: true

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Poppins"
            font.weight: Font.Medium
            font.pointSize: viewLogic.fontSize
            color: G.Style.colors.textColorNeutral

            onActiveFocusChanged: {
                if (_text.activeFocus)
                    window.currentView = self;
            }
        }

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical: ScrollBar {
            id: _scrollbar

            policy: ScrollBar.AsNeeded
            anchors.right: _view.right;
            anchors.top: _view.top;
            anchors.bottom: _view.bottom;
        }

    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        G.Icon {
            icon: "arrow-down-drop-circle";
            size: G.Style.smallDelegateHeight;
            color: G.Style.colors.fgColor
            visible: _drop.containsDrag;
            anchors.centerIn: parent;
        }

        onDropped: {
            if (drop.hasUrls) {
                console.log("DROP", drop.mimeData)
                drop.urls.forEach(function (item, index) {
                    console.log("DROP FILE", item)
                    self.droppedFromFile(item);
                });
            } else {
                self.droppedFromManager(drag.source.ref);
            }

            drop.accept();
        }
    }

    G.TimeSeriesSlider {
        id: _ts_slider;
        times: viewLogic.times
        visible: viewLogic.times.length > 1
        enabled: visible

        onValueChanged: {
            viewLogic.currentTime = times[value]
        }
    }

    G.IconButton {
        id: _export_icon;

        anchors.top: self.top
        anchors.topMargin: G.Style.smallPadding
        anchors.right: self.right
        anchors.rightMargin: G.Style.smallPadding + _scrollbar.width

        iconName: viewLogic.inputView ? "arrow-down-drop-circle" : "arrow-up-drop-circle";
        enabled: self.export_enabled
        size: G.Style.iconLarge;
        color: self.export_enabled ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        hoverColor : self.export_enabled ? G.Style.colors.hoveredBaseColor : G.Style.colors.fgColor;
        tooltip: self.export_enabled? "Export" : ""


        onClicked: {
            viewLogic.transmit()
        }
    }

    Rectangle {

        id: _focus_indicator;

         width: self.width - 1
        height: self.height - 1
        radius: G.Style.panelRadius;

        color: G.Style.colors.transparent;

        border.width: G.Style.borderWidth;
        border.color: G.Style.colors.baseColor;

        visible: window.currentView == self;
    }

    Component.onCompleted: window.currentView = self;
}
