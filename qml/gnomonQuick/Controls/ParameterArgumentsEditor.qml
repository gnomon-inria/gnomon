import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G


G.Gutter {
    id: _self

    required property var param;

    ListView {
        id: _argument_list

        anchors.fill: parent
        anchors.margins: G.Style.smallPadding

        spacing: G.Style.smallPadding
        orientation: ListView.Horizontal

        model: getArguments(param)

        delegate: Rectangle {
            height: _argument_list.height
            width: G.Style.buttonWidth

            Label {
                id: _argument_label

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: G.Style.smallPadding

                height: G.Style.mediumButtonHeight

                text: label.toUpperCase()
                font: G.Style.fonts.label
                color: G.Style.colors.textColorBase

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            G.TextField {
                anchors.top: _argument_label.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: G.Style.smallPadding

                text: _self.param ? (_self.param[name]? _self.param[name] : "") : ""
                placeholderText: qsTr("Enter value")

                validator: getValidator(type)

                onEditingFinished: {
                    _self.param[name] = text
                }
            }

            color: G.Style.colors.bgColor;
            radius: G.Style.panelRadius
        }
    }

    ListModel {
        id: _int_arguments

        ListElement {
            name: "min"
            label: "Min Value"
            type: "int"
        }

        ListElement {
            name: "max"
            label: "Max Value"
            type: "int"
        }
    }

    ListModel {
        id: _double_arguments

        ListElement {
            name: "min"
            label: "Min Value"
            type: "double"
        }

        ListElement {
            name: "max"
            label: "Max Value"
            type: "double"
        }

        ListElement {
            name: "decimals"
            label: "Decimals"
            type: "int"
        }
    }

    ListModel {
        id: _nurbs_curve_arguments

        ListElement {
            name: "degree"
            label: "Polynome Degree"
            type: "int"
        }

        ListElement {
            name: "controlPointSizeU"
            label: "Control Points Size"
            type: "int"
        }

        ListElement {
            name: "delta"
            label: "Delta"
            type: "double"
        }
    }

    ListModel {
        id: _nurbs_surface_arguments

        ListElement {
            name: "degree"
            label: "Polynome Degree"
            type: "int"
        }

        ListElement {
            name: "controlPointSizeU"
            label: "Control Points Size U"
            type: "int"
        }

        ListElement {
            name: "controlPointSizeV"
            label: "Control Points Size V"
            type: "int"
        }

        ListElement {
            name: "delta"
            label: "Delta"
            type: "double"
        }
    }

    IntValidator {
        id: _int_validator
    }

    DoubleValidator {
        id: _double_validator
        notation: DoubleValidator.StandardNotation
    }

    function getArguments(param) {
        if (param) {
            let type = param.type
            type = type.replace(",void", "")
            console.log(param.label, type)
            if (type === "dtkCoreParameterNumeric<qlonglong>") {
                return _int_arguments
            } else if (type === "dtkCoreParameterNumeric<double>") {
                return _double_arguments
            } else if  (type === "gnomonCoreParameterNurbs") {
                if(param.nurbsType === 0) {
                    return _nurbs_curve_arguments
                } else if(param.nurbsType === 1) {
                    return _nurbs_curve_arguments
                } else if(param.nurbsType === 2) {
                    return _nurbs_surface_arguments
                }
            } else {
                return undefined
            }
        } else {
            return undefined
        }
    }

    function getValidator(type) {
        if (type === "int") {
            return _int_validator
        } else if (type === "double") {
            return _double_validator
        } else {
            return undefined
        }
    }
}
