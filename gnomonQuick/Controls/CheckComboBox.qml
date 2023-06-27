import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

ComboBox {

  id: _control;

  property color textColor: G.Style.colors.textColorBase;
  property var values: []
  implicitWidth: G.Style.smallPanelWidth;
  implicitHeight: G.Style.comboBoxHeight;
  padding: 8;

  background: G.Panel {
    //anchors.fill: parent;
  }

  delegate: G.CheckComboBoxDelegate {
    width: _control.width;
    text: model[textRole] ? model[textRole] : modelData[textRole] ? modelData[textRole] : modelData
    highlighted: _control.highlightedIndex === index
    checked: ischecked

    onToggled: {
      if(checked) {
        if(!values.includes(text)) {
          values.push(text);
        }
        ischecked = false
      } else {
        let pos = values.indexOf(text);
        if( pos != -1) {
            values.splice(pos, 1);
        }
        ischecked = true
      }
      _control.displayText = values.toString()
    }
  }

  indicator: G.Icon {

    id: _indicator;

    anchors.right: parent.right;
    anchors.verticalCenter: parent.verticalCenter;
    anchors.rightMargin: 8;

    size: G.Style.iconSmall;
    icon: G.Icons.icons["apple-keyboard-control"]

    rotation: _control.down ? 0 : 180

    Behavior on rotation {
      NumberAnimation { duration: 200 }
    }
  }

  contentItem: Label {

    anchors.verticalCenter: parent.verticalCenter;
    anchors.left: parent.left;
    anchors.leftMargin: 8;
    width: _control.implicitWidth - _indicator.width;

    font: G.Style.fonts.formLabel;
    color: _control.textColor;
    text: _control.displayText;
    verticalAlignment: Text.AlignVCenter;
    elide: Text.ElideRight

  }

  //TODO: MAKE A BETTER LOOKING POPUP
  popup: Popup {
    y: _control.height - 1
    width: _control.width
    implicitHeight: contentItem.implicitHeight
    padding: 1

    contentItem: ListView {

      clip: true
      spacing: 2;
      implicitHeight: contentHeight
      model: _control.popup.visible ? _control.delegateModel : null
      currentIndex: _control.highlightedIndex

      ScrollIndicator.vertical: ScrollIndicator { }
    }

    background: Rectangle {
      color: G.Style.colors.gutterColor;
      radius: G.Style.panelRadius;
    }
  }

}
