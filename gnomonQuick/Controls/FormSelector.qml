import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

  id: _control;

  property alias model: _list_view.model;

  property string currentValue;
  property int currentIndex;

  signal toggleVisibility(int index, bool flag);
  signal deleteForm(int index, bool deleteMenu);

  implicitWidth: G.Style.smallPanelWidth;
  implicitHeight: G.Style.formSelectorHeight;
  clip: true;

  background: Rectangle {
    radius: G.Style.panelRadius;
    color: G.Style.colors.gutterColor;

  }

  layer.enabled: true
  layer.effect: OpacityMask
  {
    maskSource: Rectangle
    {
      width: _control.width
      height: _control.height
      radius: G.Style.panelRadius;
    }
  }

  ListView {
    id: _list_view;

    anchors.fill: parent;
    spacing: 2;
    clip: true;
    focus: true;

    onCountChanged: {  // tried on model changed but triggered before count was updated
        if(count>=1 && currentIndex ==-1) {
            currentIndex = 0;
            _control.currentIndex = 0;
            _control.currentValue = _list_view.itemAtIndex(0).text;
        }
    }


    delegate: G.FormSelectorDelegate {

      //make space for the scroll indicator as well as spacing
      width: parent.width;
      highlighted: _control.currentIndex == index
      text: modelData ? modelData.split(',')[0] : ""
      thumbnailIndex: modelData ? modelData.split(',')[1] : "-1"

      onClicked: {
        _control.currentIndex = index;
        _control.currentValue = modelData.split(',')[0];
      }

      onToggleVisibility: _control.toggleVisibility(index, flag);

      onDeleteForm: {

        if(index === _control.currentIndex) {
          if(_list_view.model.length) _control.currentIndex = 0;
          else _control.currentIndex = -1;
        }

        _control.deleteForm(index, _control.currentIndex == -1)
      }
    }

    ScrollIndicator.vertical: ScrollIndicator {
      id: _scroll_indicator;

      width: 8;
      visible: _list_view.contentHeight > _list_view.height;
    }

  }

}
