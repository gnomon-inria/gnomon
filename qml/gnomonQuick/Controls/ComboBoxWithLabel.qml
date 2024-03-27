import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

  id: _control;

  property alias model: _combobox.model;
  property alias label: _label.text;
  property alias count: _combobox.count;
  property alias currentIndex: _combobox.currentIndex;
  property alias currentValue: _combobox.currentValue;
  property alias textRole: _combobox.textRole;
  property alias valueRole: _combobox.valueRole;

  implicitHeight: G.Style.comboBoxHeight;
  implicitWidth: G.Style.smallPanelWidth;

  RowLayout {

    anchors.fill: _control;
    spacing: G.Style.largeRowSpacing

    Label {
      id: _label;

      Layout.preferredWidth: _label.contentWidth;

      font: G.Style.fonts.subHeader
      color: G.Style.colors.textColorBase;
      verticalAlignment: Text.AlignVCenter;

    }

    G.ComboBox {
      id: _combobox;

      Layout.fillWidth: true;

      onCurrentIndexChanged: _control.currentIndexChanged();
      onCurrentValueChanged: _control.currentValueChanged();
    }
  }

}
