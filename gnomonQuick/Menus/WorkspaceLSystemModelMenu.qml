import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G
import gnomonQuick.Icons      1.0 as G

Item {

    id: _menu;

    property var parameters
    property var d

    property alias model_combobox: _models;

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: G.Style.smallPadding;

        G.ComboBoxWithLabel {
            id: _models;

            label: "Model:"
            model: d ? d.models : null;

            Layout.fillWidth: true;

            onCurrentIndexChanged: {
                if (d && d.models) {
                    d.currentIndex = _models.currentIndex;
                    d.modelName = d.models[d.currentIndex];
                }
            }

            // creating an alias for signal handling
            property string modelName: d ? d.modelName : ""
            onModelNameChanged: {
                if(d.modelName != d.models[_models.currentIndex]) {
                    for(let i=0; i<model.length; i++) {
                        if(d.models[i] == d.modelName) {
                            _models.currentIndex = i
                            d.currentIndex = i
                        }
                    }
                }
            }
        }

        ListView {
            id: _control;

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            spacing: G.Style.smallColumnSpacing;
            clip: true;

            model: _menu.parameters

            signal valueChanged();

            delegate: G.ParameterGroup {
                title: _menu.getTitleString(group);
                model: parameters;
                width: _control.width;

                onValueChanged: {
                    _control.valueChanged();
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _control.contentHeight > _control.height;
            }
        }

        Item {
            id: _slider_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            Label {
                id: _label

                anchors.left: parent.left
                anchors.top: parent.top

                text: "DERIVATION LENGTH"
                font: G.Style.fonts.label
                color: G.Style.colors.textColorBase
            }

            G.Slider {
                id: _slider

                anchors.top: _label.bottom
                anchors.left:  parent.left
                anchors.right:  parent.right

                from: 0
                to: 10
                value: 1
		        stepSize: 1
		        snapMode: Slider.SnapAlways

                onValueChanged: {
                    console.log("derivation length :", _slider.value)
                    d.derivationLength = _slider.value
                }
            }

            Label {
                id: _value

                anchors.topMargin: G.Style.sizes.s1
                anchors.top: _slider.bottom
                x: 0

                text: _slider.value.toFixed(0)
                font: G.Style.fonts.value
                color: G.Style.colors.hoveredBaseColor
            }
        }

        Item {
            id: _button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {
                id: _run

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                text: "Run"
                iconName: G.Icons.icons["play"]

                onClicked: {
                    console.info('launching Run!')
                    d.run();
                }
            }

            G.Button {
                id: _step

                anchors.right: _run.left;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                text: "Step"
                empty: true
                iconName: G.Icons.icons["step-forward"]

                onClicked: {
                    console.info('launching Step.')
                    d.step();
                }
            }

            G.Button {
                id: _reset

                anchors.right: _step.left;
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                text: "Reset"
                empty: true
                iconName: G.Icons.icons["replay"]

                onClicked: {
                    console.info('launching Reset?')
                    d.reset();
                }
            }
        }
    }

    function getTitleString(group : string) : string {
        return (group.charAt(0).toUpperCase() + group.slice(1)).replace('_', ' ')
    }
}
