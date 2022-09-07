import QtQuick           2.15
import QtQuick.Controls  2.15
import QtQuick.Layouts   1.15

import Qt5Compat.GraphicalEffects

import Qt.labs.platform  1.0 as P

import xQuick.Controls   1.0 as X
import xQuick.Fonts      1.0 as X
import xQuick.Style      1.0 as X

import gnomonQuick.Controls as G
import gnomonQuick.Style as G


G.Dialog { id: self;

    property var availableReaders: ListModel{ };

    signal readerSelected(string reader)

    x: (parent.width - width)/2
    y: (parent.height - height)/2
    width: G.Style.mediumDialogWidth;
    height: G.Style.mediumDialogHeight;

    padding: 0;

    parent: Overlay.overlay

    focus: true
    modal: true
    title: "Choose a Reader"

    standardButtons: Dialog.Ok | Dialog.Cancel

    Component {  id: _delegate;
        G.ListItemDelegate {
            width: listView.width
            text: title
            font: G.Style.fonts.cardLabel

            highlighted: listView.currentIndex == index

            onClicked: {
                listView.currentIndex = index;
            }

            onDoubleClicked: {
                listView.currentIndex = index;
                self.accept();
            }
        }
    }

    RowLayout {
        spacing: G.Style.mediumRowSpacing
        anchors.fill: parent;
        anchors.margins: G.Style.smallPadding

        G.Gutter {
            id: _control
            Layout.fillHeight: true
            width: G.Style.smallPanelWidth
            ListView { id: listView

                anchors.fill: parent
                //Layout.minimumWidth: self.width/2;
                //Layout.minimumHeight: self.height * 3/4;

                model: self.availableReaders

                delegate: _delegate;

                Keys.onPressed: (event) => {
                    //console.log("........", event.key)
                    if (event.key == Qt.Key_Return) {
                        //console.log("OK......")
                        event.accepted = true;
                        self.accept();
                    } else if (event.key == Qt.Key_Down) {
                        event.accepted = true;
                        listView.incrementCurrentIndex();
                    } else if (event.key == Qt.Key_Up) {
                        event.accepted = true;
                        listView.decrementCurrentIndex();
                    }
                }

                focus: true
            }
        }


        ColumnLayout
        {
            spacing: G.Style.mediumColumnSpacing
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            //width: self.width/2

            Label { id: titleLabel;
                text: (self.availableReaders.count > 0 &&  self.availableReaders.get(listView.currentIndex)) ? self.availableReaders.get(listView.currentIndex).title: "";
                font: G.Style.fonts.header
                horizontalAlignment: Text.AlignLeft;

                Layout.preferredHeight: G.Style.mediumLabelHeight;
                Layout.fillWidth: true
            }
            ColumnLayout {
                spacing: G.Style.mediumColumnSpacing
                Layout.fillWidth: true

                Label { id: descriptionLabel;
                    text: (self.availableReaders.count > 0 && self.availableReaders.get(listView.currentIndex)) ? self.availableReaders.get(listView.currentIndex).description: "";
                    wrapMode: Text.Wrap
                    font: G.Style.fonts.label
                    Layout.preferredHeight: G.Style.mediumLabelHeight;
                    Layout.fillWidth: true
                }
                Image {
                    Layout.preferredWidth: G.Style.sizes.s13;
                    Layout.preferredHeight: G.Style.sizes.s11;
                    fillMode: Image.PreserveAspectFit
                    source: (self.availableReaders.count > 0 && self.availableReaders.get(listView.currentIndex)) ? "file:/"+self.availableReaders.get(listView.currentIndex).preview: "";
                }

            }
        }
    }

    onOpened: {
        self.enabled = true;
    }

    onAccepted: {
        self.enabled = false;
        self.readerSelected(self.availableReaders.get(listView.currentIndex).title);
        self.close();
    }
}
