import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style 1.0 as G

G.CollapsiblePanel {

  id: _control;

  property alias model: _parameters.model;

  clip: true;
  collapsed: false;

  signal valueChanged();

  Component.onCompleted: {
    // for(let p in _control.parameters) {
    //   _parameters_model.append(_control.parameters[p]);
    // }

    console.log("BUILDING COLLAPSIBLE PANEL")
    //console.log(_control.parameters)
    console.log(_control.title)
    console.log("MODEL LENGTH", _control.model.count)
  }


  ListView {
    id: _parameters;

    spacing: G.Style.smallRowSpacing;
    // model: ListModel {
    //   id: _parameters_model;
    //   dynamicRoles: true;
    // }


    delegate: Loader {

      property var lparam: param;

      anchors.topMargin: 10;

      height: _control.collapsed ? 0 : G.Style.controlHeight;
      width: _control.width;

      opacity: _control.collapsed ? 0 : 1;
      clip: true;
      z: 1;

      sourceComponent: component;

      Connections {
        target: param
        function onValueChanged() {
          _control.valueChanged();
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
