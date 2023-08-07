import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
  id: _control

  required property var param;

  implicitHeight: _combobox.implicitHeight
  width: G.Style.smallPanelWidth

  hoverEnabled: true

  G.ToolTip {
    visible: _control.hovered && !_combobox.down
    text: param ? param.doc : ""
  }

  RowLayout {
    anchors.top: _control.top
    anchors.left: _control.left
    anchors.right: _control.right
    //anchors.fill: _control;
    spacing: G.Style.largeRowSpacing

    Label {
      id: _label;
      Layout.preferredWidth: _label.contentWidth;

      font: G.Style.fonts.subHeader
      color: G.Style.colors.textColorBase;
      verticalAlignment: Text.AlignVCenter;
      text:  _control.param ? _control.param.label.toUpperCase() : ""
    }

    G.CheckComboBox {
      id: _combobox;
      Layout.fillWidth: true;

      textRole: "name"
      model: ListModel {
        id: _internal_model;
      }

      onActivated: {
        param.value = _combobox.displayText.split(",");
      }
    }
  }

  Component.onCompleted: {
    _internal_model.clear();
    for(const v of param.list) {
      _internal_model.append({"name": v, "ischecked": param.value.includes(v)})
    }
    _combobox.displayText = param.value.toString();
    _combobox.values = param.value.toString().split(",");
  }
}
