import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import gnomon.Visualization 1.0 as G

Rectangle {

    id: _control;

    color: Qt.darker(X.Style.alternateBaseColor);

    property alias view: _view;

    signal dropped(string path)
    signal transmit();

    XVis.Viewer {

        id: _view;

        anchors.fill: parent;

        mouseEnabled: true;
        /* XVis.Actor { */
        /*     XVis.PolyDataMapper { */
        /*         XVis.SphereSource { */
        /*             id: source; */
        /*         } */
        /*     } */
        /* } */
    }

    DropArea {

        id: _drop;

        anchors.fill: parent;

        X.Icon {
            icon: X.Icons.icons.arrow_circle_down;
            size: 56;
            color: "#44999999";
            visible: _drop.containsDrag;
            anchors.centerIn: parent;
        }

        onDropped: {
            if (drop.hasUrls)
                drop.urls.forEach(function (item, index) {
                    _control.dropped(item);
                });
            else
                console.log("Got", drag.source.ref)

            drop.accept();
        }
    }

    X.ButtonRound {
        iconSource: X.Icons.icons.arrow_circle_up;

        text: "Export"

        anchors.top: _view.top
        anchors.topMargin: 10
        anchors.right: _view.right
        anchors.rightMargin: 10

        onClicked: {
            _control.transmit();
        }
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
            width: _control.width
            height: _control.height
            radius: 4;
        }
    }
}
