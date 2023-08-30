import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Popup {
  id: _control

  x: Math.round((parent.width - contentWidth))
  y: Math.round((parent.height - contentHeight))
  clip: true

  implicitHeight: contentHeight;
  implicitWidth: contentWidth;

  margins: G.Style.smallPadding
  bottomPadding: 2

  property int _duration: 300
  property alias header: _header.text
  property alias message: _message.text
  property int type: G.Style.ButtonType.Base


  enter: Transition {
    NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: _duration}
  }

  exit: Transition {
    NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: _duration}
  }
  onOpened : {
    _timer.start()
  }

  Timer {
        id: _timer;
        interval: 5000;
        onTriggered: _control.close();
    }

  background: Rectangle {
    radius: G.Style.panelRadius

    border.width: 1
    border.color: _getButtonColor()
    color: _getBgColor()

  }
  contentItem: Rectangle {
    id : _popup_content

    implicitHeight: Math.round(G.Style.smallDialogHeight / 2)
    implicitWidth: G.Style.smallDialogWidth
    color: _getBgColor()

    G.Icon {
      id: _icon

      icon: _getIcon()
      size: G.Style.iconLarge
      visible: true
      color: _getButtonColor()
      anchors {
      top: parent.top
      left: parent.left
      }
    }

    Text {
      id: _header

      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignVCenter
      font: G.Style.fonts.header
      color: _getTextColor()

      anchors {
        verticalCenter: _icon.verticalCenter
        left: _icon.right
        margins: G.Style.smallPadding
      }
    }

    Text {
      id: _message

      wrapMode: Text.Wrap
      horizontalAlignment: Text.AlignLeft
      font: G.Style.fonts.value
      color: _getTextColor()

      anchors {
        top: _header.bottom
        left: _icon.right
        right: _button.left
        margins: G.Style.smallPadding
      }
    }

    G.Button {
      id: _button

      anchors.bottom: parent.bottom
      anchors.right: parent.right
      anchors.margins: G.Style.smallPadding

      text: "Got it"
      type: _control.type
      width: 2*G.Style.buttonWidth/3

      onClicked: {
        _control.close();
      }
    }
  }

  function _getButtonColor() {
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

  function _getTextColor() {
    if(_control.type === G.Style.ButtonType.Danger)
      return G.Style.colors.textColorDarkDanger

    if(_control.type === G.Style.ButtonType.Warning)
      return G.Style.colors.textColorDarkWarning

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.textColorDarkOk

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.textColorDarkBase

    return G.Style.colors.textColorDarkNeutral
  }


  function _getBgColor() {
    if(_control.type === G.Style.ButtonType.Danger)
      return G.Style.colors.textColorLightDanger

    if(_control.type === G.Style.ButtonType.Warning)
      return G.Style.colors.textColorLightWarning

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.textColorLightOk

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.textColorLightBase

    return G.Style.colors.textColorLightNeutral
  }

  function _getIcon() {
    if(_control.type === G.Style.ButtonType.Danger)
      return "close-circle-outline"

    if(_control.type === G.Style.ButtonType.Warning)
      return "alert-circle-outline"

    if(_control.type === G.Style.ButtonType.OK)
      return "check-circle-outline"

    if(_control.type === G.Style.ButtonType.Base)
      return "information-outline"

    return "chevron-right-circle-outline"
  }

}
