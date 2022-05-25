//Qt
import QtQuick
import QtQuick.Controls

//Atomic
import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {

  id: _control

  default property alias content: _panel.content
  property alias title: _panel_name.text
  property bool collapsed: true;
  property int panelHeight: _panel.panel.childrenRect.height + G.Style.smallColumnSpacing;
  property Control panel: _panel.panel;

  implicitWidth: G.Style.smallPanelWidth;
  implicitHeight: _header.height + _panel.height;
  clip: true;

  Control {
    id: _header;

    anchors.top: parent.top;
    width: parent.width;
    height: G.Style.collapsibleMinHeight;
    padding: G.Style.smallPadding;

    Label {
      id: _panel_name

      height: _header.height;
      anchors.left: _header.left;
      anchors.verticalCenter: _header.verticalCenter;

      font: G.Style.fonts.subHeader
      color: G.Style.colors.textColorBase;
    }

    G.IconButton {
      id: _collapse_button

      anchors.right: _header.right;
      anchors.verticalCenter: _header.verticalCenter;
      iconName: _control.collapsed ? G.Icons.icons["plus"] : G.Icons.icons["minus"]
      color: _control.collapsed ? G.Style.colors.textColorBase : G.Style.colors.textColorDeEmphasize
      size: G.Style.iconMedium;

      onClicked: _control.collapsed = !_control.collapsed
    }

  }
  G.Panel {
    id: _panel

    anchors.top: _header.bottom;
    implicitHeight: _control.collapsed ? 0 : _control.panelHeight;
    width: _control.width

    opacity: _control.collapsed ? 0 : 1;

    Behavior on height {
      NumberAnimation { duration: 1000 }
    }

    Behavior on opacity {
      NumberAnimation { duration: 100 }
    }

  }
}
