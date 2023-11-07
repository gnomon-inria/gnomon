import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Window
import QtQuick.Layouts

import Qt.labs.platform as P

import gnomon.Visualization 1.0 as GV
import gnomon.Mpl           1.0 as GV

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Rectangle {

    id: self;

    color: G.Style.colors.bgColor;

    focus: true;

    property alias view: _view;
    property alias ts_slider: _ts_slider
    property var viewLogic;
    property var visualizations;
    property int number: _view.number;

    signal droppedFromFile(string path)
    signal droppedFromManager(int index)

    signal transmit();

    GV.FigureCanvas {
        id: _view;

        dpi_ratio: Screen.devicePixelRatio

        anchors.fill: parent

        onActiveFocusChanged: {
            if (_view.activeFocus)
                window.currentView = self;
        }

        onWidthChanged: _view.setGeom(_view.width, _view.height);
        onHeightChanged: _view.setGeom(_view.width, _view.height);
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        G.Icon {
            icon: "arrow-down-drop-circle";
            size: G.Style.smallDelegateHeight;
            color: G.Style.colors.bgColor
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
        id: _screenshot_icon;

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.right: _export_icon.visible? _export_icon.left : _view.right
        anchors.rightMargin: G.Style.smallPadding

        iconName: "camera";
        size: G.Style.iconLarge;
        color: G.Style.colors.fgColor

        onClicked: {
            _screenshot_dialog.open()
        }

        P.FileDialog {
            id: _screenshot_dialog

            nameFilters: [ "Image files (*.png)" ]
            title: "Save screenshot"
            modality: Qt.WindowModal;
            fileMode: P.FileDialog.SaveFile

            onAccepted: {
                viewLogic.saveScreenshot(decodeURIComponent(_screenshot_dialog.file));
            }
        }
    }

    G.IconButton { id: _export_icon;
        iconName: viewLogic.inputView ? "arrow-down-drop-circle" : "arrow-up-drop-circle";
        enabled: !viewLogic.inputView
        visible: !viewLogic.inputView
        size: G.Style.iconLarge;
        color: viewLogic.inputView ? G.Style.colors.textColorNeutral : G.Style.colors.fgColor;
        tooltip: viewLogic.inputView? "" : "Export"

        anchors.top: _view.top
        anchors.topMargin: G.Style.smallPadding
        anchors.right: _view.right
        anchors.rightMargin: G.Style.smallPadding

        onClicked: {
            viewLogic.transmit();

            // _view.requestCapture();
            // _view.update();
        }
    }

    //layer.enabled: true
    //layer.effect: OpacityMask
    //{
    //    maskSource: Rectangle
    //    {
    //         width: self.width
    //        height: self.height
    //        radius: G.Style.panelRadius;
    //    }
    //}

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
