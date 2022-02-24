import QtQuick              2.15
import QtQuick.Controls     2.15
import QtGraphicalEffects   1.15
import QtQuick.Layouts      1.15

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import crossQuick        1.0 as C
import crossParameters   1.0 as C

import gnomonQuick.Controls 1.0 as G

ListView {
    id: _self;

    signal valueChanged();

    /* model: _params.params_model; */
    spacing: 10;

    /* Layout.fillWidth: true; */
    /* Layout.fillHeight: true; */
    /* visible: view.viewLogic.formNames.length > 0 */
    clip: true;

    delegate: Loader {
        property var lparam: param;
        height: 70;
        width: _self.width;
        sourceComponent: component;

        Connections {
            target: param
            function onValueChanged() {
                _self.valueChanged();
                /* if (_auto_render.checked) { */
                /*     console.info('launching Render!') */
                /*     view.viewLogic.update(); */
                /* } */
            }
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {
        visible: _self.contentHeight > _self.height;
    }
}
