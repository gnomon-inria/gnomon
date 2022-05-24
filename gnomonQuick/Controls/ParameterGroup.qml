import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style 1.0 as G

G.CollapsiblePanel {

  id: _control

  property alias model: _parameters.model

  clip: true
  collapsed: false
  panelHeight: _parameters.childrenRect.height + G.Style.smallColumnSpacing

  signal valueChanged()

  ListView {
    id: _parameters;

    //anchors.fill: _control.panel

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter

    width: _control.width - G.Style.sizes.s3


    spacing: G.Style.smallColumnSpacing

    delegate: Loader {

      property var lparam: param

      height: _control.collapsed ? 0 : G.Style.controlHeight
      width: _parameters.width

      opacity: _control.collapsed ? 0 : 1
      clip: true
      z: 1

      sourceComponent: component

      Connections {
        target: param
        function onValueChanged() {
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
