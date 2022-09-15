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
  property int size: G.Style.ButtonSize.Medium
  property bool empty: false
  property string iconName: ""
  property bool isHovered: (_control.hovered || _content.hovered || _icon_area.containsMouse) && _control.hoverEnabled && _control.enabled

  flat: false

  //leftPadding: G.Style.buttonPadding
  //rightPadding: _control.iconName ? 0 : G.Style.buttonPadding
  implicitWidth: Math.max(G.Style.buttonWidth, implicitContentWidth + leftPadding + rightPadding)
  implicitHeight: _getButtonHeight()
  hoverEnabled: enabled

// /////////////////////////////////////////////////////////////////////////////
// Content Item
// /////////////////////////////////////////////////////////////////////////////

  contentItem: Control {

    id: _content

    G.Icon {
        id: _icon

        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom
        width: height

        size: (2/3) * _control.height

        visible: _control.iconName
        icon: _control.iconName
        color: _control.flat || _control.empty ? _getBgColor() : _getTextColor()

        MouseArea {
            id: _icon_area;

            anchors.fill: parent;
            hoverEnabled: true;
            cursorShape: Qt.PointingHandCursor

            onClicked: {
              _control.clicked()
            }
        }

    }

    Rectangle {
        id: _separator

        anchors.left: _icon.right
        anchors.verticalCenter: parent.verticalCenter
        //anchors.margins: G.Style.buttonPadding

        height: parent.height - 4
        width: 2

        visible: _control.iconName && !_control.flat

        color: _control.empty ? _getBgColor() : _getTextColor()

    }

    Label {
        id: _text

        anchors.right: parent.right;
        anchors.left: _control.iconName? _separator.right : parent.left;
        anchors.verticalCenter: parent.verticalCenter
        leftPadding: G.Style.buttonPadding
        rightPadding: G.Style.buttonPadding

        text: _control.text.toUpperCase()

        font: _control.size === G.Style.ButtonSize.Small? G.Style.fonts.buttonSmall : G.Style.fonts.button

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

  HoverHandler {
    cursorShape: Qt.PointingHandCursor
  }

// /////////////////////////////////////////////////////////////////////////////
// States
// /////////////////////////////////////////////////////////////////////////////

  states: [

    State {
      name: "hovered"
      when: (_control.hovered || _content.hovered || _icon_area.containsMouse) && _control.hoverEnabled && _control.enabled
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
        target: _separator;
        color: _getTextColor()
      }
      PropertyChanges {
        target: _background;
        color: _getHoveredBgColor()
        border.width: 0
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
      return G.Style.colors.textColorWarning

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.textColorOk

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.textColorBase

    return G.Style.colors.textColorNeutral
  }

  function _getButtonHeight() {

      if(_control.size === G.Style.ButtonSize.Small)
        return G.Style.smallButtonHeight

      if(_control.size === G.Style.ButtonSize.Large)
        return G.Style.largeButtonHeight

        return G.Style.mediumButtonHeight
    }

}

//
// Button.qml ends here
