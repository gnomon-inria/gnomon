import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
  id: _control

  required property var param;
    //for crossParameters
    readonly property var paramType: _control.param ? _control.param.type : undefined

    implicitHeight: _slider.implicitHeight
    width: G.Style.smallPanelWidth
    background: Rectangle{
        color: "transparent"
    }

    G.NumericSlider {
        id: _slider

        anchors.fill: parent

        value: _control.param ? _control.param.value : 0
        min: _control.param ? _control.param.min : 0
        max: _control.param ? _control.param.max : 0
        decimals: _control.param ? (_control.param.decimals ? _control.param.decimals : 0) : 0

        label: _control.param ? _control.param.label : ""
        doc: _control.param ? _control.param.doc : ""

        onValueChanged: {
      if(_control.param)
              _control.param.value = _slider.value
        }
    }
}
