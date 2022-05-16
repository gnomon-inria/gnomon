import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import xQuick.Controls      1.0 as X
import xQuick.Fonts         1.0 as X
import xQuick.Style         1.0 as X
import xQuick.Vis           1.0 as XVis

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

ItemDelegate {

  id: _control;

  implicitHeight: G.Style.formDelegateHeight;

  font: G.Style.fonts.formLabel;

  background: Rectangle {
    opacity: enabled ? 0.8 : 0.1
    color: getBgColor()
    border.width: 0.5
    border.color: getEmbossColor()
  }

  contentItem: Row {
    anchors.leftMargin: 24;
    anchors.top: _control.top;
    anchors.bottom: _control.bottom;
    spacing: 8;

    Rectangle {

      id: _thumbnail

      anchors.verticalCenter: parent.verticalCenter;

      height: 32
      width: 32
      radius: G.Style.panelRadius
      color: G.Style.colors.lightBlue
    }

    Label {

      anchors.verticalCenter: parent.verticalCenter;

      font: G.Style.fonts.formLabel;
      color: G.Style.colors.textColorBase;
      text: _control.text;
      verticalAlignment: Text.AlignVCenter;
    }

  }

  X.Icon {
    id: _visibility_icon;

    property bool checked: true;

    anchors.right: parent.right;
    anchors.verticalCenter: parent.verticalCenter;
    anchors.leftMargin: 4;
    anchors.rightMargin: 10;

    size: 24;

    icon: checked? X.Icons.icons.visibility : X.Icons.icons.visibility_off;
    //color: checked? X.Style.foregroundColor : X.Style.backgroundColor;
    color: G.Style.colors.textColorBase;

    MouseArea {
      id: _visibility_mouse_area;
      anchors.fill: parent;
      hoverEnabled: true;

      // onClicked: {
      //   _visibility_icon.checked = !_visibility_icon.checked;
      //   view.viewLogic.setFormVisible(view.viewLogic.formNames[index], _visibility_icon.checked)
      // }
    }
  }

  X.Icon {

    anchors.right: _visibility_icon.left;
    anchors.verticalCenter: parent.verticalCenter;
    anchors.margins: 4

    size: 24;

    icon: X.Icons.icons.delete;
    color: G.Style.colors.textColorBase;

    MouseArea {

      anchors.fill: parent;
      onClicked: {
        // if(index === _form_selector.currentIndex) {
        //   if(view.viewLogic.formNames.length)
        //     _form_selector.currentIndex = 0;
        //   else {
        //     _form_selector.currentIndex = -1;
        //     _internal.menu.destroy();
        //   }
        // }
        // view.viewLogic.removeForm(view.viewLogic.formNames[index]);
        // view.viewLogic.update();

      }
    }
  }

  function getEmbossColor() {
    if(down || highlighted) return G.Style.colors.embossColorBlue;
    return G.Style.colors.embossColorNeutral;
  }

  function getBgColor() {
    if(down || highlighted) return G.Style.colors.baseColor;
    if(hovered) return G.Style.colors.neutralColor;
    return G.Style.colors.fgColor;
  }

}
