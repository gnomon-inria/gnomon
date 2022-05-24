

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
