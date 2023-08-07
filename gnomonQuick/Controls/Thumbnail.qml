import QtQuick 2.15
import QtQuick.Controls 2.15

import gnomonQuick.Style  1.0 as G

Control {

  id: _self

  property color bgColor: G.Style.colors.neutralColor
  required property string url
  signal clicked()
  signal doubleClicked()

  height: G.Style.thumbnailLarge + _self.padding
  width: _self.height

  padding: G.Style.xSmallPadding

  background: Rectangle {
    color: _self.bgColor
    radius: G.Style.borderWidth;
  }

  contentItem: Image {
    fillMode: Image.PreserveAspectFit;
    source: _self.url
  }

  MouseArea {
    id: _form_selector_area

    anchors.fill: parent

    onClicked: _self.clicked()
    onDoubleClicked: _self.doubleClicked()
  }
}
