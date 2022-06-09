import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import QtQml.Models      2.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Menus as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
    id: _menu;

    property var parameters
    property var d

    property alias algo_combobox: _algos;

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: G.Style.smallPadding;

        G.ComboBoxWithLabel {
            id: _algos;

            label: "Algorithm:"
            model: d ? d.algorithms : null;

            Layout.fillWidth: true;
            /* Layout.leftMargin: 20 */
            /* Layout.rightMargin: 20 */

            onCurrentIndexChanged: {
                _auto_apply.checked = false

                if (d && d.algorithms) {
                    d.currentIndex = _algos.currentIndex;
                    d.algoName = d.algorithms[d.currentIndex];
                }
            }

            onCurrentValueChanged: {
                _params.parameters =  d.parameters
                _params.updateParametersModel();
            }
        }

        G.Parameters {
            id: _params;
        }

        ListView {
            id: _control;

            Layout.fillWidth: true;
            Layout.fillHeight: true;


            spacing: G.Style.smallColumnSpacing;
            clip: true;

            model: _params.params_model

            signal valueChanged();

            delegate: G.ParameterGroup {
                title: group;
                model: parameters;
                width: _control.width;

                onValueChanged: {
                    _control.valueChanged();
                }
            }

            onValueChanged: {
                if (_auto_apply.checked) {
                    console.info('launching Run!')
                    d.run();
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {
                visible: _control.contentHeight > _control.height;
            }
        }

        Item {
            height: G.Style.sizes.s8
            Layout.fillWidth: true;

            CheckBox{ id: _auto_apply

                text: "Apply automatically"

                onClicked: {
                    if (_auto_apply.checked) {
                        console.info('launching Run!')
                        d.run();
                    }
                }
            }
        }

        Item {
            id: _button_container

            height: G.Style.largeButtonHeight
            Layout.fillWidth: true;

            G.Button {

                id: _apply

                anchors.right: _button_container.right;
                anchors.verticalCenter: _button_container
                anchors.margins: G.Style.smallPadding

                text: "Apply"

                onClicked: {
                    console.info('launching Run!')
                    d.run();
                }
            }
        }
    }
}
