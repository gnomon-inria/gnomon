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
  panelHeight: _parameters.childrenRect.height;

  signal valueChanged();

  // Component.onCompleted: {
  //   // for(let p in _control.parameters) {
  //   //   _parameters_model.append(_control.parameters[p]);
  //   // }

  //   console.log("BUILDING COLLAPSIBLE PANEL")
  //   //console.log(_control.parameters)
  //   console.log(_control.title)
  //   console.log("MODEL LENGTH", _control.model.count)
  //   for(let i = 0; i < _control.model.count; i++)
  //     console.log(_control.model.get(i).param.type)
  // }


  ListView {
    id: _parameters;

    spacing: G.Style.smallRowSpacing;

    delegate: Loader {

      property var lparam: param;

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
