import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G


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
    border.width: 1
    border.color: _getButtonColor()  
    color: _getBgColor()

  }
  contentItem: Rectangle {
    id : _popup_content
        
    implicitHeight: Math.round(G.Style.smallDialogHeight / 5 * 2) 
    implicitWidth: G.Style.smallDialogWidth 
    color: _getBgColor() 

    G.Icon {
      icon: G.Icons.icons["information"]
      size: G.Style.iconMedium
      visible: true
      color: _getButtonColor()
      anchors {
      top: parent.top
      left: parent.left
      }
    }
    Text {
      id: _header
      font.bold: true
      font.pointSize: G.Style.sizes.s4
      color: G.Style.colors.gutterColor
      anchors {
        top: parent.top
        horizontalCenter: parent.horizontalCenter
      }
    }    

    Text {
      id: _message
      wrapMode: Text.Wrap
      horizontalAlignment: Text.AlignHCenter
      font.pointSize: G.Style.sizes.s3
      color: G.Style.colors.gutterColor
      anchors {
        top: _header.bottom
        horizontalCenter: parent.horizontalCenter
      }

    }
    
    Rectangle {
      anchors.margins: G.Style.smallPadding
      anchors.bottom: parent.bottom
      anchors.right: parent.right
      width: Math.round(G.Style.buttonWidth / 2)
      height: 25
      color: _getButtonColor()
      Text {
        anchors.centerIn: parent
        text: "Got it"
        color: "white"
      }
    }
  }

  MouseArea {
    anchors.fill: parent
    hoverEnabled: true
    onClicked: {
      _control.close();
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

  function _getBgColor() {
    if(_control.type === G.Style.ButtonType.Danger)
      return G.Style.colors.lightRed

    if(_control.type === G.Style.ButtonType.Warning)
      return G.Style.colors.lightOrange

    if(_control.type === G.Style.ButtonType.OK)
      return G.Style.colors.lightGreen

    if(_control.type === G.Style.ButtonType.Base)
      return G.Style.colors.lightBlue
    
    return G.Style.colors.neutralColor
  }
}
