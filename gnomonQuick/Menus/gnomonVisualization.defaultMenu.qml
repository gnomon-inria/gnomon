

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

ListView {
    id: _control;

    spacing: G.Style.smallColumnSpacing;
    clip: true;

    signal valueChanged();

    Component.onCompleted: {
        for(let i = 0; i < _control.model.count; i++)
            console.log("PARAMS: " + _control.model.get(i).group, _control.model.get(i).parameters.count)

    }

    delegate: G.ParameterGroup {
        title: group;
        model: parameters;
        width: _control.width;

        onValueChanged: {
            _control.valueChanged();
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {
        visible: _control.contentHeight > _control.height;
    }

}
