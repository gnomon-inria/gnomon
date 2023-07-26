import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import Qt.labs.settings

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
        function onFormVisualizationChanged() {
            let formType = _form_selector.currentValue
            if (formType) {
                let visuName = view.viewLogic.formVisuName(formType)
                let index = formVisuIndex(formType, visuName)
                _visu_combobox.currentIndex = index
            }
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
                if (_internal.menu) {
                    _internal.menu.destroy();
                }
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

    G.IconButton {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: -G.Style.smallPadding/2
        anchors.rightMargin: -G.Style.smallPadding

        size: G.Style.iconMedium;
        iconName: "cog"

        tooltip: "View Parameters"

        onClicked: {
            _view_parameters.collapsed = !_view_parameters.collapsed;
        }
    }

    G.FormSelector {

        id: _form_selector

        currentValue: ""
        currentIndex: -1

        anchors.top: _form_label.bottom
        anchors.topMargin: G.Style.smallPadding
        anchors.left: parent.left
        anchors.right: parent.right

        //model: view ? view.viewLogic.formNamesAndId : null
        view: _control.view

        onToggleVisibility: view.viewLogic.setFormVisible(view.viewLogic.formNames[index], flag)
        onCurrentValueChanged: {
            _visu_combobox.changeModel(currentValue)
        }

        onDeleteForm: function(index, deleteMenu) {
            view.viewLogic.removeForm(view.viewLogic.formNames[index]);
            //view.viewLogic.update();
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
        anchors.bottom: _button_container.top;

        anchors.margins: 12;

        G.ComboBoxWithLabel {
            id: _visu_combobox;

            label: "Type:"
            textRole: "name"
            valueRole: "counter"
            model: view? view.viewLogic.formVisualizations(_form_selector.currentValue) : null;
            currentIndex: 0

            property bool _model_changing: true;

            function changeModel(formType) {
                let previousVisuSelected = view ? view.viewLogic.lastVisuSelected(formType) : ""
                let currentVisu = view ? view.viewLogic.formVisuName(formType) : ""
                _model_changing = true
                model = view? view.viewLogic.formVisualizations(formType) : null;
                let index = formVisuIndex(formType, previousVisuSelected)
                if(index>=0 && count>=1) {
                    currentIndex = index
                } else if(count>=1 && index ==-1) {
                    currentIndex = formVisuIndex(formType, currentVisu)
                } else if(count>=1 && currentIndex ==-1) {
                    currentIndex = 0
                }
                valueChangeHandler()
                _model_changing = false;
            }


            Layout.fillWidth: true;
            /* Layout.leftMargin: 20 */
            /* Layout.rightMargin: 20 */
            visible: view? view.viewLogic.formNames.length > 0 : false

            function valueChangeHandler() {
                if(view) {
                    if (_form_selector.currentValue) {
                        let previousVisuSelected = view.viewLogic.lastVisuSelected(_form_selector.currentValue)
                        if(previousVisuSelected!=model[_visu_combobox.currentIndex].key){
                            view.viewLogic.setFormVisuName(_form_selector.currentValue, model[_visu_combobox.currentIndex].key)
                        }
                        //_auto_render.checked = false
                        _params.parameters =  view.viewLogic.formVisuParameters(_form_selector.currentValue);
                        _params.updateParametersModel();
                        _control.update_menu(_visu_combobox.currentValue.key);
                    }
                }
            }

            onCurrentValueChanged: {
                if(!_model_changing){
                    valueChangeHandler()
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
    }

    Item {
        id: _button_container

        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        height: G.Style.sizes.s8

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

    G.ViewParameters {
        id: _view_parameters

        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: -3*G.Style.smallPadding

        view: _control.view
    }

    G.Dragger {
        anchors.horizontalCenter: _view_parameters.horizontalCenter;
        anchors.bottom: _view_parameters.top;
        anchors.bottomMargin: -1 * height/2;

        z: _view_parameters.z+1
        tooltip: "View Parameters"

        onClicked: {
            _view_parameters.collapsed = !_view_parameters.collapsed
        }
    }

    QtObject {
        id: _internal;

        property var menu: null;
    }

    function formVisuIndex(formType, visuName) {
        let model = _control.view? _control.view.viewLogic.formVisualizations(formType) : null;
        let index = model? model.findIndex(v => (v.key == visuName) ) : -1
        return index;
    }

    function update_menu(name) {
        if (_internal.menu) {
            _internal.menu.destroy();
            _auto_render_connect.target = null;
        }

        var source = "qrc:/gnomonQuick/Menus/" + name + ".menu.qml"
        var defaultSource = "qrc:/gnomonQuick/Menus/gnomonVisualization.defaultMenu.qml"

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
