import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15
import QtQuick.Window

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
    property bool _is_curve : true

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
        implicitHeight: _edit_appeareance_curve.visible ? 200 : parent.height

        onWidthChanged : {
            if (_view.visible) {
                _view.setGeom(_view.width, _view.height);
            }
        }
        onHeightChanged : {
           if (_view.visible) {
               _view.setGeom(_view.width, _view.height);
           }
       }

        backgroundColor: G.Style.figureColors.bgColor
        lineColor: G.Style.figureColors.lineColor
        parameterEditor: true

        onHoverChanged : {
            window.insideParamFigure()
        }

        onMouseReleased : {
            param.updateControlPointsFromPython()
        }

        G.IconButton {
            id: _edit_appeareance_curve

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: G.Style.smallPadding
            size: G.Style.iconSmall;
            iconName: "pencil"
            visible: !_nurbs_config_dialog.visible

            onClicked: {
                _control._is_curve = true
                _nurbs_config_dialog.visible = true
                _nurbs_view_in_dialog.visible = false
                _view.parent = _curve_container_in_dialog
                _curve_container_in_dialog.visible = true
            }
        }
    }

    G.ViewNurbs {
        id: _nurbs_view;

        anchors.top: _label.bottom

        width: parent.width
        implicitHeight: 200

        onHoveredChanged: {
            window.insideParamFigure()
        }

        G.IconButton {

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: G.Style.smallPadding
            size: G.Style.iconSmall;
            iconName: "pencil"
            visible: !_nurbs_config_dialog.visible

            onClicked: {
                _control._is_curve = false
                _nurbs_config_dialog.visible = true
                // _nurbs_view.parent = _nurbs_config_dialog.contentItem
            }
        }

        Component.onCompleted: {
            param.nurbsViewWidget.backgroundColor = Qt.binding(function() {return G.Style.figureColors.bgColor})
        }
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

    Window {
        id: _nurbs_config_dialog
        flags: Qt.Dialog

        width: window.width / 2
        height: window.height
        minimumWidth : width
        maximumWidth : width
        minimumHeight : height
        maximumHeight : height

        x: window.x + width/2
        y: window.y + height/2
        Rectangle {
            id: _curve_container_in_dialog
            visible: false
            width: parent.width
            implicitHeight: parent.height
        }

        G.ViewNurbs {
            id: _nurbs_view_in_dialog;

            width: parent.width
            implicitHeight: parent.height

            onHoveredChanged: {
                window.insideParamFigure()
            }

            
            MouseArea {
                id: _mouse_area
                anchors.fill: parent
                propagateComposedEvents: true

                onClicked: (mouse)=> {
                    if(param.nurbsType == GV.NurbsParameter.SURFACE) {
                        param.updateRenderWindow()
                    }
                    _mouse_area.enabled = false
                    mouse.accepted = false
                }
            }

            Component.onCompleted: {
                param.nurbsView.backgroundColor = Qt.binding(function() {return G.Style.figureColors.bgColor})
            }
        }

        onClosing : (close) => {
            if(_control._is_curve)
                _view.parent = _control
        }

        Rectangle {
            implicitHeight : _buttons.implicitHeight
            implicitWidth : parent.width
            anchors.bottom: parent.bottom
            anchors.topMargin: G.Style.mediumPadding

            DialogButtonBox {
                id: _buttons
                anchors.fill: parent
                visible: true
                alignment: Qt.AlignRight
                spacing: G.Style.smallPadding

                background: Rectangle {
                    anchors.fill: parent
                    color: G.Style.colors.bgColor
                }

                G.Button {
                    text: 'OK';
                    type: G.Style.ButtonType.Base
                    width: G.Style.buttonWidth

                    onClicked: {
                        _nurbs_config_dialog.close()
                        if(_control._is_curve)
                            _view.parent = _control
                        param.updateControlPointsFromPython()
                    }
                }

                G.Button {
                    text: 'Cancel';
                    flat: true
                    type: G.Style.ButtonType.Neutral
                    width: G.Style.buttonWidth

                    onClicked: {
                        _nurbs_config_dialog.close()
                        if(_control._is_curve)
                            _view.parent = _control
                        param.updateControlPointsInPython()
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        //d.onParametersChanged();
        if(param.nurbsType == GV.NurbsParameter.SURFACE) {
            G.Associator.associateNurbs(_nurbs_view_in_dialog, param.nurbsView)
            G.Associator.associateNurbs(_nurbs_view, param.nurbsViewWidget)
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
