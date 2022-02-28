import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X


X.Dialog { id: self;

    property var availableReaders: ListModel{ };

    signal readerSelected(string reader)

    x: (parent.width - width)/2
    y: (parent.height - height)/2
    width: window.width * 3/4;
    height: window.height * 3/4;

    parent: Overlay.overlay

    focus: true
    modal: true
    title: "Choose a Reader"

    standardButtons: Dialog.Ok | Dialog.Cancel

    Component {  id: _delegate;
        ItemDelegate {
            width: listView.width
            text: title
            font.pointSize: 14;

            onClicked: {
                listView.currentIndex = index;
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 60
                opacity: enabled ? 0.8 : 0.3
                color: Qt.darker(X.Style.backgroundColor, 1.2)

                Rectangle {
                    width: parent.width
                    height: 1
                    color: X.Style.borderColor;
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    RowLayout {
        spacing: 6
        anchors.fill: parent;

        ListView { id: listView

            Layout.fillHeight: true
            Layout.minimumWidth: self.width/2;
            Layout.minimumHeight: self.height * 3/4;

            model: self.availableReaders

            delegate: _delegate;

            highlight: Rectangle {
                color: X.Style.foregroundColor
            }

            focus: true
        }

        ColumnLayout
        {
            spacing: 6
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            X.Label { id: titleLabel;
                      text: self.availableReaders.count > 0 ? self.availableReaders.get(listView.currentIndex).title: "";
                font {
                    weight: Font.Bold
                    pointSize: 14;
                }
                horizontalAlignment: Text.AlignRight;

                Layout.preferredHeight: 30;
                Layout.fillWidth: true
            }

            X.Label { id: descriptionLabel;
                text: self.availableReaders.count > 0 ? self.availableReaders.get(listView.currentIndex).description: "";

                Layout.preferredHeight: 30;
                Layout.fillWidth: true
            }
        }
    }

    onAccepted: {
        self.readerSelected(self.availableReaders.get(listView.currentIndex).title);
    }
}
