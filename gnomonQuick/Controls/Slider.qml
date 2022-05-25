import QtQuick
import QtQuick.Controls

import xQuick as X

import gnomonQuick.Style as G
import gnomonQuick.Controls as G
import gnomonQuick.Icons as G

X.ScientificSpinBoxReal {

    id: _self

    readonly property alias gaugeWidth: _gauge.width

    implicitHeight: G.Style.sizes.s5
    implicitWidth: G.Style.smallPanelWidth

    padding: 6

    rightPadding: padding + (_self.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))

  contentItem: Rectangle {
    id: _gauge

    height: parent.height
    radius: G.Style.panelRadius
    width: _content.width * _self.scaleValue();

    color: G.Style.colors.textColorBase;
    Component.onCompleted: {
      console.log("SCALE VALUE: " + _self.scaleValue())
    }

  }

    up.indicator: G.IconButton {
        //flat: true
        autoRepeat: true
        x: _self.mirrored ? 0 : parent.width - width
        y: 0;
        size: parent.height / 2

        iconName: G.Icons.icons["chevron-up"]
      onClicked: {
        console.log("INCREASE")
          _self.increase();
      }
    }


    down.indicator: G.IconButton {
        //flat: true
        autoRepeat: true
        x: _self.mirrored ? 0 : parent.width - width
        y: height;
        size: parent.height / 2
        iconName: G.Icons.icons["chevron-down"]
      onClicked: {
        console.log("DECREASE")
        _self.decrease();
      }
    }

    background: Rectangle {
        implicitWidth: G.Style.smallPanelWidth
        color: G.Style.colors.gutterColor;
    }


  MouseArea {
    id: _slider

    anchors.fill: _gauge

    onPressed: {
      console.log("PRESSING SLIDER")
      console.log("VALUE: " + _self.scaleValueFromRatio(_slider.mouseX / _gauge.width))
    }

    onClicked: {
      console.log("GAUGE CLICKED")
    }

  }


    Component.onCompleted: {
        if (_self.locale.name !== Qt.locale("C").name) {
            console.warn("Locale cannot be changed. It is always 'C'. Change is ignored.");
            _self.locale = Qt.locale("C");
        }
        _self.localeChanged.connect((locale) => {
            if (locale.name !== _self.locale.name && locale.name !== Qt.locale("C").name) {
                console.warn("Locale must be 'C'. 'C' locale is enforced.");
                _self.locale = Qt.locale("C");
            }
        });
    }

    Binding on value {
        when: _slider.pressed
        value: _self.scaleValueFromRatio(_slider.mouseX / _gauge.width)
        restoreMode: Binding.RestoreNone
    }

    onValueChanged: { _gauge.width = _content.width * _self.scaleValue(); }
}
