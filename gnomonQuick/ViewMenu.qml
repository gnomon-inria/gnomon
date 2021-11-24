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
            spacing: 33;

            Layout.fillWidth: true;
            Layout.fillHeight: true;
            visible: view.viewLogic.formNames.length > 0
            //clip: true;

            delegate: Loader {
                property var lparam: param;
                height: 33;
                sourceComponent: component;
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
