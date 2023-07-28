import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

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

            // signal valueChanged();

            delegate: G.ParameterGroup {
                required property var parameters;
                required property string group;

                title: _menu.getTitleString(group);
                model: parameters;
                width: _control.width;

                onValueChanged: {
                    if (_auto_apply.checked) {
                        d.reset()
                        d.run();
                    }
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _control.contentHeight > _control.height;
            }

            Connections {
                target: window
                function onInsideParamFigure() {
                    _control.interactive = !_control.interactive;
                }
            }
        }


        RowLayout {
            Layout.fillWidth: true;

            G.CheckBox{
                id: _auto_apply
                text: "Auto apply"
                checked: false

                onClicked: {
                    if (_auto_apply.checked) {
                        d.reset();
                        d.run();
                    }
                }
            }

            Label {
                id: _anim_time_label
                text: "Animation Time(s)"
                font: G.Style.fonts.label
                color: G.Style.colors.textColorBase
            }

            TextField {
                id: _anim_time
                text: "10"
                color: G.Style.colors.hoveredBaseColor
                font: G.Style.fonts.value

                onTextChanged: {
                    d.setAnimationTime(text)
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true;

            G.NumericSpinBox {
                id: _animation_spinbox

                Layout.fillWidth: true;

                value: 1
                min: 1
                max: 1000
                decimals: 0

                label: "Animation step"
                doc: "Number of steps between two displayed times for Animate"

                onValueChanged: {
                    d.animationStep = _animation_spinbox.value
                }
            }

            G.NumericSpinBox {
                id: _derivation_spinbox

                Layout.fillWidth: true;
                spacing: G.Style.smallPadding

                value: d.derivationLength
                min: 0
                max: 10000
                decimals: 0

                label: "Derivation length"
                doc: "Total number of derivation steps to compute for Animate"

                onValueChanged: {
                    if(d.derivationLength != value) {
                        d.derivationLength = value
                    }
                }

                Connections {
                    target: d
                    function onDerivationLengthChanged(length) {
                        if(length != _derivation_spinbox.value) {
                            _derivation_spinbox.value = length
                        }
                    }
                }

            }
        }

        Item {
            id: _button_container

            height: 2*G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {
                id: _stop

                anchors.right: _animate.left;
                anchors.verticalCenter: _animate.verticalCenter
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                enabled: d.running
                text: "Stop"
                empty: true
                type: enabled? G.Style.ButtonType.Danger : G.Style.ButtonType.Neutral
                iconName: "stop"

                onClicked: {
                    console.info('Stopping the simulation.')
                    d.stop();
                }
            }

            G.Button {
                id: _run

                anchors.right: _button_container.right;
                anchors.top: _button_container.top
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                enabled: !d.running & d.fileName.endsWith(".lpy")
                text: "Run"
                type: enabled? G.Style.ButtonType.Base : G.Style.ButtonType.Neutral
                tooltip: "run the simulation from the beginning"
                iconName: "play"

                onClicked: {
                    console.info('launching Run!')
                    console.log(d.fileName)
                    d.run();
                }
            }

            G.Button {
                id: _animate

                anchors.right: _button_container.right;
                anchors.top: _run.bottom
                anchors.margins: G.Style.smallPadding

                implicitWidth: 2*G.Style.shortButtonWidth + G.Style.smallPadding
                enabled: !d.running & d.fileName.endsWith(".lpy")
                text: "Animate"
                type: enabled? G.Style.ButtonType.Base : G.Style.ButtonType.Neutral
                tooltip: "Animate: do " + _derivation_spinbox.value + " steps and display them"
                iconName: "animation-play"

                onClicked: {
                    console.info('launching animation...')
                    d.animate();
                }
            }

            G.Button {
                id: _step

                anchors.right: _run.left;
                anchors.verticalCenter: _run.verticalCenter
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                enabled: !d.running & d.fileName.endsWith(".lpy")
                text: "Step"
                tooltip: "Step: do one more step and visualize it"
                empty: true
                type: enabled? G.Style.ButtonType.Base : G.Style.ButtonType.Neutral
                iconName: "step-forward"

                onClicked: {
                    console.info('launching Step.')
                    d.step();
                }
            }

            G.Button {
                id: _reset

                anchors.right: _step.left;
                anchors.verticalCenter: _run.verticalCenter
                anchors.margins: G.Style.smallPadding

                implicitWidth: G.Style.shortButtonWidth
                enabled: !d.running  & d.fileName.endsWith(".lpy")
                text: "Reset"
                empty: true
                type: enabled? G.Style.ButtonType.Base : G.Style.ButtonType.Neutral
                iconName: "replay"

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
