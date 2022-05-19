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

  implicitWidth: G.Style.smallPanelWidth;
  implicitHeight: _header.height + _panel.contentHeight;
  clip: true;

  Control {
    id: _header;

    anchors.top: parent.top;
    width: parent.width;
    height: G.Style.collapsibleMinHeight;
    padding: G.Style.smallPadding;

    Label {
      id: _panel_name

      height: parent.height;
      anchors.left: parent.left;
      anchors.verticalCenter: parent.verticalCenter;

      font: G.Style.fonts.subHeader
      color: G.Style.colors.textColorBase;
    }

    G.IconButton {
      id: _collapse_button

      anchors.right: parent.right;
      anchors.verticalCenter: parent.verticalCenter;
      iconName: _control.collapsed ? G.Icons.icons["plus"] : G.Icons.icons["minus"]
      color: _control.collapsed ? G.Style.colors.textColorDeEmphasize : G.Style.colors.textColorBase
      size: G.Style.iconMedium;

      onClicked: _control.collapsed = !_control.collapsed
    }

  }
  G.Panel {
    id: _panel

    anchors.top: _header.bottom;
    height: _control.collapsed ? 0 : _panel.implicitContentHeight;
    width: _control.width

    opacity: _control.collapsed ? 0 : 1;


    Behavior on height {
      NumberAnimation { duration: 200 }
    }

    Behavior on opacity {
      NumberAnimation { duration: 200 }
    }

  }
}
