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
            _params.parameters = view.viewLogic.formVisuParameters(_form_selector.currentValue);
            _params.updateParametersModel();
        }
        function onFormsChanged() {
            console.log("FORMS CHANGED", view.viewLogic.formNames)
            _form_selector.currentIndex = 0;
            _form_selector.currentValue = view.viewLogic.formNames[_form_selector.currentIndex];
        }
    }

    ListView {
        id: _form_selector;

        property string currentValue: "";

        height: parent.height / 3;

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;

        model: view.viewLogic.formNames;

        clip: true;
        focus: true;
        currentIndex: -1;


        delegate: ItemDelegate {
            width: _form_selector.width
            height: 42;
            highlighted: _form_selector.currentIndex == index

            text: modelData;
            font.pointSize: 14;

            onClicked: {
                _form_selector.currentIndex = index;
                _form_selector.currentValue = view.viewLogic.formNames[_form_selector.currentIndex];
            }

            Rectangle {
                radius: 4;
                anchors.fill: parent

                color: "transparent"
                z: Infinity

                border.width: 2;
                border.color: X.Style.accentColor;

                visible: _form_selector.currentIndex == index
            }

            CheckBox {
                id: _checkbox;

                checked: true;
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter;

                text: ""

                onClicked: {
                    console.log("PRESSED CHECKBOX", index, checkState === Qt.Checked)
                    view.viewLogic.setFormVisible(view.viewLogic.formNames[index], checkState === Qt.Checked)
                    view.viewLogic.update()
                }
            }

            X.Icon {

                anchors.right: _checkbox.left;
                anchors.verticalCenter: parent.verticalCenter;

                size: 33;

                icon: X.Icons.icons.delete;

                MouseArea {

                    anchors.fill: parent;
                    onClicked: {
                        console.log("REMOVE FORM", index)
                        if(index === _form_selector.currentIndex) {
                            if(view.viewLogic.formNames.length)
                                _form_selector.currentIndex = 0;
                            else
                                _form_selector.currentIndex = -1;
                        }
                        view.viewLogic.removeForm(view.viewLogic.formNames[index]);
                        view.viewLogic.update();
                    }
                }

            }

            background: Rectangle {
                opacity: enabled ? 0.8 : 0.1
                color: (down || highlighted || hovered) ? Qt.lighter(X.Style.backgroundColor, 1.2) : Qt.darker(X.Style.backgroundColor, 1.2)

                Rectangle {
                    width: parent.width
                    height: 1
                    color: X.Style.borderColor;
                    anchors.bottom: parent.bottom
                }
            }

        }

        ScrollIndicator.vertical: ScrollIndicator { visible: _form_selector.contentHeight > _form_selector.height; }

        Component.onCompleted: {
            console.log(view.viewLogic.formNames)
        }
    }

    X.Separator {
        anchors.top: _form_selector.bottom;
        anchors.right: parent.right;
        anchors.left: parent.left;
    }

    ColumnLayout {

        anchors.top: _form_selector.bottom;
        anchors.right: parent.right;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;

        anchors.margins: 12;

        ComboBox {
            id: _visu_combobox;
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

                _auto_render.checked = false
                _params.parameters =  view.viewLogic.formVisuParameters(_form_selector.currentValue);
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
