//Qt
import QtQuick
import QtQuick.Controls

//Gnomon
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Button {

  id: _control

// /////////////////////////////////////////////////////////////////////////////
// Basic properties
// /////////////////////////////////////////////////////////////////////////////


  property int type: G.Style.ButtonType.Base
  property bool empty: false
  property string iconName: undefined

  flat: false

  leftPadding: G.Style.buttonPadding
  rightPadding: G.Style.buttonPadding
  implicitWidth: Math.max(G.Style.buttonWidth,
    implicitContentWidth + leftPadding + rightPadding)
  implicitHeight: 32

// /////////////////////////////////////////////////////////////////////////////
// Content Item
// /////////////////////////////////////////////////////////////////////////////

  contentItem: Item {

    G.Icon {
        id: _icon

        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter

        size: (2/3) * _control.height

        visible: _control.iconName
        icon: _control.iconName
        color: _control.flat || _control.empty ? _getBgColor() : _getTextColor()

        MouseArea {
            id: _icon_area;

            anchors.fill: parent;
            hoverEnabled: true;
        }
    }

    Label {
        id: _text

        anchors.right: parent.right;
        anchors.left: _control.iconName? _icon.right : parent.left;
        anchors.verticalCenter: parent.verticalCenter

        text: _control.text.toUpperCase()

        font: G.Style.fonts.button

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: _control.flat || _control.empty ? _getBgColor() : _getTextColor()
    }
  }

// /////////////////////////////////////////////////////////////////////////////
// Background
// /////////////////////////////////////////////////////////////////////////////

  background: Rectangle {

    id: _background

    radius: G.Style.buttonRadius

    color: _control.empty || _control.flat ? "transparent" : _getBgColor()
    border.color: _getBgColor()
    border.width: _control.empty ? 2 : 0

  }

// /////////////////////////////////////////////////////////////////////////////
// Hover
// /////////////////////////////////////////////////////////////////////////////

  Binding on state {
    when: _control.hovered || _icon_area.containsMouse
    value: "hovered"
  }

  HoverHandler {
    // hovered: _control.hovered
    cursorShape: Qt.PointingHandCursor
  }

// /////////////////////////////////////////////////////////////////////////////
// States
// /////////////////////////////////////////////////////////////////////////////

  states: [

    State {
      name: "hovered"
      PropertyChanges {
        target: _text;
//        font: G.Style.fonts.buttonHovered
        color: _getTextColor()
      }
      PropertyChanges {
        target: _icon;
        color: _getTextColor()
      }
      PropertyChanges {
        target: _background;
        color: _getHoveredBgColor()
      }
    }

  ]


// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////


  function _getBgColor() {

    if(_control.type === G.Style.ButtonType.Danger)
      return G.Style.colors.dangerColor

    if(_control.type === G.Style.ButtonType.Warning)
      return G.Style.colors.warningColor

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.okColor

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.baseColor

    return G.Style.colors.neutralColor
  }

  function _getHoveredBgColor() {

    if(_control.type === G.Style.ButtonType.Danger)
      return G.Style.colors.hoveredDangerColor

    if(_control.type === G.Style.ButtonType.Warning)
      return G.Style.colors.hoveredWarningColor

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.hoveredOkColor

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.hoveredBaseColor

    return G.Style.colors.hoveredNeutralColor
  }

  function _getTextColor() {

    if(_control.type === G.Style.ButtonType.Danger)
      return G.Style.colors.textColorDanger

    if(_control.type === G.Style.ButtonType.Warning)
      return G.Style.colors.bgColor

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.textColorOk

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.textColorBase

    return G.Style.colors.textColorNeutral
  }

}

//
// Button.qml ends here
