import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

    id: _control;

    property alias model: _list_view.model;
    property string currentValue: "";
    property int currentIndex: -1;

    implicitWidth: parent.width;
    implicitHeight: G.Style.formSelectorHeight;

    clip: true;

    background: Rectangle {
        radius: G.Style.panelRadius;
        color: G.Style.colors.gutterColor;
    }

    layer.enabled: true
    layer.effect: OpacityMask
    {
        maskSource: Rectangle
        {
             width: _control.width
            height: _control.height
            radius: G.Style.panelRadius;
        }
    }

    ListView {
        id: _list_view;

        anchors.fill: parent;

        clip: true;
        focus: true;

        delegate: G.FormSelectorDelegate {

          width: parent.width;
            highlighted: _control.currentIndex == index

            text: modelData ? modelData.replace('gnomon', '') :""

            onClicked: {
                _control.currentIndex = index;
                _control.currentValue = modelData;
            }

        }

        ScrollIndicator.vertical: ScrollIndicator { visible: _form_selector.contentHeight > _form_selector.height; }

    }

}
