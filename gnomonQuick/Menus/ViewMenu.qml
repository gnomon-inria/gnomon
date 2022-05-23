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

    // TODO: Start with a flickable

    required property Item view;

    Connections {
        target: view.viewLogic
        function onFormVisuParametersChanged() {
            _params.parameters = view.viewLogic.formVisuParameters(_form_selector.currentValue);
            _params.updateParametersModel();
        }
        function onFormsChanged() {
            if(view.viewLogic.formNames.length) {
                _form_selector.currentIndex = 0;
                _form_selector.currentValue = view.viewLogic.formNames[_form_selector.currentIndex];
            }
            else {
                _form_selector.currentIndex = -1;
                _form_selector.currentValue = "";
            }
        }
    }

    Connections {
        id: _auto_render_connect;

        target: null

        function onValueChanged() {
            // if (_auto_render.checked) {
            //     view.viewLogic.update();
            // }
        }
    }

    X.Dialog {
        //dialog deactivated for now. maybe to delete
        id: _form_delete_modal;

        property string formName: "";
        property int formIndex: -1;

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: Math.round(window.width / 3 * 2)
        height: Math.round(window.height / 3)

        title: "Confirm Action"

        X.Label {

            font {
                pointSize: 14;
                weight: Font.Bold;
            }
            //font.weight: bold;
            //color: "red"
            text: "Are you sure you wish to delete " + _form_delete_modal.formName + " ?";
        }

        parent: Overlay.overlay

        focus: true;
        modal: true;

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if(formIndex === _form_selector.currentIndex) {
                if(view.viewLogic.formNames.length)
                    _form_selector.currentIndex = 0;
                else {
                    _form_selector.currentIndex = -1;
                    _internal.menu.destroy();
                }

            }
            view.viewLogic.removeForm(formName);
            view.viewLogic.update();
            _form_delete_modal.close();
        }
    }

    G.FormSelector {

        id: _form_selector;

        currentValue: "";
        currentIndex: -1;

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        model: view.viewLogic.formNames

        onToggleVisibility: view.viewLogic.setFormVisible(view.viewLogic.formNames[index], flag)

        onDeleteForm: {
            if(deleteMenu) _internal.menu.destroy()

            view.viewLogic.removeForm(view.viewLogic.formNames[index]);
            view.viewLogic.update();
        }

    }

    ColumnLayout {
        anchors.top: _form_selector.bottom;
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;

        anchors.margins: 12;

        G.ComboBoxWithLabel {
            id: _visu_combobox;

            label: "Type:"
            model: view.viewLogic.formVisualizations(_form_selector.currentValue);

            Layout.fillWidth: true;
            /* Layout.leftMargin: 20 */
            /* Layout.rightMargin: 20 */
            visible: view.viewLogic.formNames.length > 0

            onCurrentIndexChanged: {
                if(_visu_combobox.currentValue) {
                    view.viewLogic.setFormVisuName(_form_selector.currentValue, model[_visu_combobox.currentIndex]);
                }
            }

            onCurrentValueChanged: {
                _visu_combobox.currentIndex = model.indexOf(view.viewLogic.formVisuName(_form_selector.currentValue))

                //_auto_render.checked = false
                _params.parameters =  view.viewLogic.formVisuParameters(_form_selector.currentValue);
                _params.updateParametersModel();

                _control.update_menu(_visu_combobox.currentValue);
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

        Item {
            id: _button_container

            height: G.Style.sizes.s8
            Layout.fillWidth: true;

            G.Button {

                id: _render

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container
                anchors.margins: G.Style.smallPadding

                text: "Render"
                enabled: view.viewLogic.formNames.length > 0;

                onClicked: {
                    view.viewLogic.update();
                }

            }

            G.Button {

                anchors.right: _render.left
                anchors.verticalCenter: _button_container
                anchors.margins: G.Style.smallPadding

                text: "Clear"
                flat: true
                enabled: view.viewLogic.formNames.length > 0;

                onClicked: {
                    view.viewLogic.clear();
                    _internal.menu.destroy();
                }

            }

        }

        // X.ButtonRaw {
        //     text: "Render";

        //     Layout.fillWidth: true;
        //     enabled: view.viewLogic.formNames.length > 0;
        //     visible: view.viewLogic.formNames.length > 0;

        //     onClicked: {
        //         view.viewLogic.update();
        //     }

        //     X.CheckBox{ id: _auto_render

        //         text: ""
        //         contentItem: null;

        //         anchors.top: parent.top
        //         anchors.topMargin: 5
        //         anchors.right: parent.right
        //         anchors.rightMargin: 5

        //         height: parent.height - 10;


        //         MouseArea {
        //             id: _auto_render_mouse_area;
        //             anchors.fill: parent;
        //             hoverEnabled: true;

        //             propagateComposedEvents: true

        //             onClicked: mouse.accepted = false;
        //             onPressed: mouse.accepted = false;
        //             onReleased: mouse.accepted = false;
        //             onDoubleClicked: mouse.accepted = false;
        //             onPositionChanged: mouse.accepted = false;
        //             onPressAndHold: mouse.accepted = false;
        //         }

        //         ToolTip.visible: _auto_render_mouse_area.containsMouse;
        //         ToolTip.text: "Auto-render";

        //         onClicked: {
        //             if (_auto_render.checked) {
        //                 console.info('launching Render!')
        //                 view.viewLogic.update();
        //             }
        //         }
        //     }
        // }

        // X.ButtonRaw {
        //     text: "Clear";

        //     Layout.fillWidth: true;
        //     enabled: view.viewLogic.formNames.length > 0;
        //     visible: view.viewLogic.formNames.length > 0;

        //     onClicked: {
        //         view.viewLogic.clear();
        //         _internal.menu.destroy();
        //     }
        // }
    }

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
            menu_component = Qt.createComponent(defaultSource)

            if(menu_component.status != Component.Ready) {
                console.error("Can't create visualization menu for", name, specific_error_msg, menu_component.errorString())
                return;
            }
        }

        _internal.menu = menu_component.createObject(_menu, {
            model: _params.params_model,
            //parameters: _params.parameters,
        })

        _internal.menu.anchors.fill = _menu;

        _auto_render_connect.target = _internal.menu
    }

}
