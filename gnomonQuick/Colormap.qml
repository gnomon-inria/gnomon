import QtQuick          2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import xQuick.Controls 1.0 as X
import xQuick.Style    1.0 as X

import gnomonQuick     1.0 as G

Control {
	id: _self

	required property var param;

	RowLayout {
		anchors.fill: parent;
		anchors.margins: 10;

		spacing: 20;

		X.LabelCaption { text: param.label }

        X.ComboBox {
            id: _cluts
            model: param.availableCluts
            currentIndex: model.indexOf(param.identifier)

            Layout.fillWidth: true;

            onCurrentValueChanged: {
				param.identifier = _cluts.currentValue
            }

            X.ToolTip {
                parent: _cluts.parent
                visible: _cluts.hovered && !_cluts.pressed
                text: param.doc
            }

            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 40

                border.width: 1
                border.color: X.Style.borderColor;
                radius: X.Style.controls.radius;

                gradient: G.ClutGradient { id: _gradient
                    cmap: param.value
                }
            }
        }
	}

	background: Rectangle{ color: X.Style.backgroundColor; radius: 4; }

    Connections {
        target: _self.param
        function onValueChanged(cmap) {
            // TODO: Find a better way to ensure gradient refresh
            _gradient.orientation = Gradient.Vertical
            _gradient.orientation = Gradient.Horizontal
        }
    }
}