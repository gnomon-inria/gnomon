import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {

  id: _control;

  required property var view;
  property alias model: _list_view.model;
  model: view ? view.viewLogic.formNamesAndId : null

  property string currentValue: ""
  property alias currentIndex: _list_view.currentIndex;

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
        let index = -1
        if(view) {
            let previousFormSelected = view.viewLogic.lastFromTypeSelected()
            index = view.viewLogic.formNames.indexOf(previousFormSelected)
        }
        if(index>=0 && count>=1) {
            currentIndex = index
        } else if(count>=1 && currentIndex ==-1) {
            currentIndex = 0
        } else {
            currentIndex = -1
        }
    }

    onCurrentIndexChanged: {
        if(currentIndex>=0 && currentIndex<_list_view.count) {
            _control.currentValue = _list_view.itemAtIndex(currentIndex).text
        } else {
            _control.currentValue = ""
        }
        if(view && currentIndex>=0 && currentIndex<_list_view.count) {
            //console.log("formSelected", currentIndex, view.viewLogic.formNames[currentIndex])
            view.viewLogic.notifyFormSelected(currentIndex, view.viewLogic.formNames[currentIndex])
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
        //_control.currentValue = modelData.split(',')[0];
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
