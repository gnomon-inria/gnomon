import QtQuick            2.15
import QtQuick.Controls   2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts    1.15

import xQuick.Controls    1.0 as X
import xQuick.Fonts       1.0 as X
import xQuick.Style       1.0 as X
import xQuick.Vis         1.0 as XVis

import "."                1.0 as G

Item {

    id: _self;

    // TODO: Start with a flickable

    required property Item view;

    ColumnLayout {

        anchors.fill: parent;
        anchors.margins: 12;


        ComboBox { id: _form_combobox
            model: view.viewLogic.formNames;
            visible: view.viewLogic.formNames.length > 0

            Layout.fillWidth: true;
        }

        ComboBox { id: _visu_combobox;
            model: view.viewLogic.formVisualizations(_form_combobox.currentValue);

            Layout.fillWidth: true;
            /* Layout.leftMargin: 20 */
            /* Layout.rightMargin: 20 */
            visible: view.viewLogic.formNames.length > 0

            onCurrentIndexChanged: {
                if(_visu_combobox.currentValue) {
                    view.viewLogic.setFormVisuName(_form_combobox.currentValue, model[_visu_combobox.currentIndex]);
                }
            }

            onCurrentValueChanged: {
                _visu_combobox.currentIndex = model.indexOf(view.viewLogic.formVisuName(_form_combobox.currentValue))

                _auto_render.checked = false
                _params.parameters =  view.viewLogic.formVisuParameters(_form_combobox.currentValue);
                _params.updateParametersModel();
            }
        }

        G.Parameters {
            id: _params;
        }

        ListView {
            id: _l;
            model: _params.params_model;
            spacing: 10;

            Layout.fillWidth: true;
            Layout.fillHeight: true;
            visible: view.viewLogic.formNames.length > 0
            clip: true;

            delegate: Loader {
                property var lparam: param;
                height: 70;
                width: _l.width;
                sourceComponent: component;

                Connections {
                    target: param
                    function onValueChanged() {
                        if (_auto_render.checked) {
                            console.info('launching Render!')
                            view.viewLogic.update();
                        }
                    }
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _l.contentHeight > _l.height;
            }
        }

        Item {
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            visible: view.viewLogic.formNames.length == 0
        }

        X.ButtonRaw {
            text: "Render";

            Layout.fillWidth: true;
            enabled: view.viewLogic.formNames.length > 0;
            visible: view.viewLogic.formNames.length > 0;

            onClicked: {
                view.viewLogic.update();
            }

            X.CheckBox{ id: _auto_render

                text: ""
                contentItem: { }

                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5

                height: parent.height - 10;


                MouseArea { id: _auto_render_mouse_area;
                    anchors.fill: parent;
                    hoverEnabled: true;

                    propagateComposedEvents: true

                    onClicked: mouse.accepted = false;
                    onPressed: mouse.accepted = false;
                    onReleased: mouse.accepted = false;
                    onDoubleClicked: mouse.accepted = false;
                    onPositionChanged: mouse.accepted = false;
                    onPressAndHold: mouse.accepted = false;
                }

                ToolTip.visible: _auto_render_mouse_area.containsMouse;
                ToolTip.text: "Auto-render";

                onClicked: {
                    if (_auto_render.checked) {
                        console.info('launching Render!')
                        d.run();
                    }
                }
            }
        }

        X.ButtonRaw {
            text: "Clear";

            Layout.fillWidth: true;
            enabled: view.viewLogic.formNames.length > 0;
            visible: view.viewLogic.formNames.length > 0;

            onClicked: {
                view.viewLogic.clear();
            }
        }

    }
}
