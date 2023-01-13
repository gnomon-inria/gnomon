import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import Qt.labs.settings

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
        target: view? view.viewLogic : null
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
                console.log("formsChanged  no formNames ")
                _form_selector.currentIndex = -1;
                _form_selector.currentValue = "";
            }
        }
    }

    Connections {
        id: _auto_render_connect;

        target: null

        function onValueChanged() {
            if (_auto_render.checked) {
                view.viewLogic.update();
            }
        }
    }

    Label {
        id: _form_label

        anchors.top: parent.top
        anchors.left: parent.left

        text: "Forms"
        font: G.Style.fonts.header
        color: G.Style.colors.textColorBase
    }

    G.FormSelector {

        id: _form_selector

        currentValue: ""
        currentIndex: -1

        anchors.top: _form_label.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: view ? view.viewLogic.formNamesAndId : null

        onToggleVisibility: view.viewLogic.setFormVisible(view.viewLogic.formNames[index], flag)

        onDeleteForm: {
            view.viewLogic.removeForm(view.viewLogic.formNames[index]);
            view.viewLogic.update();

            if(deleteMenu & _internal.menu)
                _internal.menu.destroy()
        }

    }

    Label {
        id: _visu_label

        anchors.top: _form_selector.bottom
        anchors.left: parent.left
        anchors.topMargin: G.Style.sizes.s5
        anchors.bottomMargin: G.Style.sizes.s4

        text: "Visualization"
        font: G.Style.fonts.header
        color: G.Style.colors.textColorBase
    }

    ColumnLayout {
        anchors.top: _visu_label.bottom;
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;

        anchors.margins: 12;

        G.ComboBoxWithLabel {
            id: _visu_combobox;

            label: "Type:"
            textRole: "name"
            valueRole: "counter"
            model: view? view.viewLogic.formVisualizations(_form_selector.currentValue) : null;
            currentIndex: 0

            Layout.fillWidth: true;
            /* Layout.leftMargin: 20 */
            /* Layout.rightMargin: 20 */
            visible: view? view.viewLogic.formNames.length > 0 : false

            onCurrentValueChanged: {
                if(view) {
                    view.viewLogic.setFormVisuName(_form_selector.currentValue, model[_visu_combobox.currentIndex].key)
                    //_auto_render.checked = false
                    _params.parameters =  view.viewLogic.formVisuParameters(_form_selector.currentValue);
                    _params.updateParametersModel();
                    _control.update_menu(_visu_combobox.currentValue.key);
                }
            }
        }

        G.Parameters {
            id: _params;
        }

        Item {
            Layout.fillWidth: true
            height: G.Style.sizes.s4
        }

        Item {
            id: _menu;

            Layout.fillWidth: true;
            height: G.Style.mediumPanelHeight
            Layout.fillHeight: true;

        }

        Item {
            id: _spacer

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
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Render"
                enabled: view? view.viewLogic.formNames.length > 0 : false;

                onClicked: {
                    view.viewLogic.update();
                }

            }

            G.Button {
                id: _clear

                anchors.right: _render.left
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.margins: G.Style.smallPadding

                text: "Clear"
                flat: true
                enabled: view ? view.viewLogic.formNames.length > 0 : false;

                onClicked: {
                    view.viewLogic.clear();
                    _internal.menu.destroy();
                }

            }

            G.CheckBox {

                id: _auto_render

                anchors.left: _button_container.left
                anchors.verticalCenter: _button_container.verticalCenter
                anchors.bottomMargin: G.Style.smallPadding

                text: "Auto render"
                checked: true

                Settings {
                    property alias auto_render: _auto_render.checked
                }

                onClicked: {
                    if (_auto_render.checked) {
                        console.info('launching Render!')
                        view.viewLogic.update();
                    }
                }
            }
        }
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
