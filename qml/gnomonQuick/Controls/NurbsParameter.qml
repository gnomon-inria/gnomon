import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomon.Utils as G

import gnomon.Visualization 1.0 as GV
import gnomon.Mpl           1.0 as GV

Control {
    id: _control

    required property var param
    property alias view: _view;
    property int number: _view.number;

    implicitHeight: _label.implicitHeight + G.Style.smallPanelHeight
    width: G.Style.largePanelWidth

    G.ToolTip {
        visible: _control.hovered && !_view.pressed
        text: param ? param.doc : ""
    }

    Label {
        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    GV.FigureCanvas {
        id: _view;

        dpi_ratio: Screen.devicePixelRatio

        anchors.top: _label.bottom
        //anchors.bottom: parent.bottom

        width: parent.width
        implicitHeight: 200
        onWidthChanged: _view.setGeom(_view.width, _view.height);
        onHeightChanged: _view.setGeom(_view.width, _view.height);

        backgroundColor: G.Style.figureColors.bgColor
        lineColor: G.Style.figureColors.lineColor
        parameterEditor: true

        onHoverChanged : {
            window.insideParamFigure()
        }

        onMouseReleased : {
            param.updateControlPointsFromPython()
        }
    }

    G.ViewNurbs {
        id: _nurbs_view;

        // Component.onCompleted: G.Associator.associate_nurbs(_nurbs_view, param.nurbsView)
    }

    RowLayout {
        anchors.right: _control.right
        anchors.top: _view.bottom
        anchors.left: parent.left

        Label {
            id: _degreeLabel
            x: 0 //_slider.gaugeWidth
            text: "degree:"
            font: G.Style.fonts.value
            color: G.Style.colors.hoveredBaseColor
        }
/*
        G.TextField {
            id: _value

            anchors.margins: G.Style.smallPadding

            text: _control.param ? _control.param.degree : ""
            color: G.Style.colors.hoveredBaseColor
            font: G.Style.fonts.value

            onTextChanged: {
                if(_control.param) {
                    _control.param.degree = parseInt(text)
                }
            }
        }
        */
    }


    Component.onCompleted: {
        //d.onParametersChanged();
        let nurbs_surface = 2
        if(param.nurbsType == nurbs_surface) {
            G.Associator.associateNurbs(_nurbs_view, param.nurbsView)
            _view.visible = false
            _nurbs_view.visible = true
            param.buildNurbsPatch()
        } else {
            _view.visible = true
            _nurbs_view.visible = false
            param.figureNumber = _view.number
        }
    }
}
