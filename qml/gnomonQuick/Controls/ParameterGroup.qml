import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style 1.0 as G

G.CollapsiblePanel {

  id: _control

  property alias model: _parameters.model
  property alias collapsed: _control.collapsed

  clip: true
  collapsed: false
  panelHeight: _parameters.childrenRect.height + 2 * G.Style.smallColumnSpacing

  signal valueChanged()

  ListView {
    id: _parameters;

    //anchors.fill: _control.panel

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.topMargin: G.Style.smallColumnSpacing

    width: _control.width - G.Style.sizes.s4

    spacing: G.Style.mediumColumnSpacing

    interactive: false

    delegate: Loader {
        required property var param;
        required property var component;

      property var lparam: param

      height: _control.collapsed ? 0 :
      implicitHeight ? implicitHeight : G.Style.controlHeight
      width: _parameters.width

      opacity: _control.collapsed ? 0 : 1
      clip: true
      z: 1

      sourceComponent: component

      Connections {
        target: param
        ignoreUnknownSignals: true
        function onValueChanged() {
          _control.valueChanged()
        }
        function onControlPointsChanged() {
          _control.valueChanged()
        }
      }

      Behavior on height {
        NumberAnimation { duration: 200 }
      }

      Behavior on opacity {
        NumberAnimation { duration: 200 }
      }

    }
  }

}
