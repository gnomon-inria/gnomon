import QtQuick              2.15
import QtQuick.Controls     2.15
import QtGraphicalEffects   1.15
import QtQuick.Layouts      1.15

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import gnomonQuick.Controls 1.0 as G

Item {

    id: _self;

    // TODO: Start with a flickable

    required property Item view;

    QtObject {
        id: _internal;

        property var menu: null;
    }

    function update_menu(name) {

        if (_internal.menu) {
            _internal.menu.destroy();
            _auto_render_connect.target = null;
        }

        var source = "qrc:/qml/gnomonQuick/Menus/" + name + ".menu.qml"
        var defaultSource = "qrc:/qml/gnomonQuick/Menus/gnomonVisualization.defaultMenu.qml"

        var menu_component = Qt.createComponent(source)
        if(menu_component.status != Component.Ready) {
            const specific_error_msg = menu_component.errorString()

            console.log("Can't create visualization menu for " + name, specific_error_msg)

            menu_component = Qt.createComponent(defaultSource)

            if(menu_component.status != Component.Ready) {
                console.error("Can't create visualization menu for", name, specific_error_msg, menu_component.errorString())
                return;
            }
        }

        _internal.menu = menu_component.createObject(_menu, {
            model: _params.params_model,
            parameters: _params.parameters,
        })

        _internal.menu.anchors.fill = _menu;

        _auto_render_connect.target = _internal.menu
    }

    Connections {
        target: view.viewLogic
        function onFormVisuParametersChanged() {
            _params.parameters = view.viewLogic.formVisuParameters(_form_combobox.currentValue);
            _params.updateParametersModel();
        }
    }

    ColumnLayout {

        anchors.fill: parent;
        anchors.margins: 12;

        ComboBox {
            id: _form_combobox

            model: view.viewLogic.formNames;
            visible: view.viewLogic.formNames.length > 0

            Layout.fillWidth: true;
        }

        ComboBox {
            id: _visu_combobox;
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

                _self.update_menu(_visu_combobox.currentValue);
            }
        }

        G.Parameters {
            id: _params;
        }

        Item {
            id: _menu;

            Layout.fillWidth: true;
            Layout.fillHeight: true;


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
                contentItem: null;

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
                        view.viewLogic.update();
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

    Connections {

        id: _auto_render_connect;

        target: null

        function onValueChanged() {
            if (_auto_render.checked) {
                    console.info('launching Render!')
                    view.viewLogic.update();
            }
        }

    }
}
