import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15
import Qt.labs.platform  1.0 as P

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G
import gnomonQuick.Icons as G

Control {
    id: _control

    required property var param

    implicitWidth: G.Style.smallPanelWidth
    implicitHeight: _label.implicitHeight + _colors.implicitHeight + _value.implicitHeight + 2*G.Style.smallPadding

    Label {
        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: param.label.toUpperCase()
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.ComboBox {
        id: _colors

        anchors.top: _label.bottom
        anchors.left: _control.left
        anchors.right: _edit_appeareance.left
        anchors.rightMargin: G.Style.smallPadding

        model: param.colorIndexCount
        displayText: ""

        delegate: G.ComboBoxDelegate {
            width: _colors.width
            text: "Appearance " + param.colorIndexAt(index)

            background: Rectangle {
               color: param.isColor(param.colorIndexAt(index))? param.color(param.colorIndexAt(index)) : G.Style.colors.transparent
            }

            Image {
                anchors.fill: parent
                fillMode: Image.Stretch
                visible: param.isTexture(param.colorIndexAt(index))
                source: param.isTexture(param.colorIndexAt(index))? "file://" + param.texture(param.colorIndexAt(index)) : ""
            }
        }

        background: Rectangle {
            id: _color_bg

            color: (param.value && param.isColor(param.colorIndexAt(_colors.currentValue)))? param.color(param.colorIndexAt(_colors.currentValue)) : G.Style.colors.transparent

            radius: G.Style.buttonRadius
            border.color: G.Style.colors.gutterColor
            border.width: 1

            Image {
                anchors.fill: parent
                fillMode: Image.Stretch
                visible: param.value && param.isTexture(param.colorIndexAt(_colors.currentValue))
                source: (param.value && param.isTexture(param.colorIndexAt(_colors.currentValue)))? "file://" + param.texture(param.colorIndexAt(_colors.currentValue)) : ""
            }
        }
    }

    G.IconButton {
        id: _edit_appeareance

        anchors.verticalCenter: _colors.verticalCenter
        anchors.right: _add_color.left
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: G.Icons.icons["pencil"]

        onClicked: {
            _edit_dialog.color = param.color(param.colorIndexAt(_colors.currentValue))
            _edit_dialog.texture = param.texture(param.colorIndexAt(_colors.currentValue))
            _edit_dialog.open();
        }
    }

    G.IconButton {
        id: _add_color

        anchors.verticalCenter: _colors.verticalCenter
        anchors.right: _control.right
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: G.Icons.icons["plus"]

        onClicked: {
            console.log("Add color!")
        }
    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _colors.bottom
        anchors.left: parent.left

        text: "Appearance " + param.colorIndexAt(_colors.currentValue)
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

    G.Dialog {
        id: _edit_dialog;

        property var color
        property var texture

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth
        height: G.Style.smallDialogHeight

        padding: 0;

        parent: Overlay.overlay

        focus: true
        modal: true
        title: "Edit Appeareance"

        TabBar {
            id: _bar;

            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;

            G.TabButton {
                text: "Color"
            }
            G.TabButton {
                text: "Texture"
            }
        }

        StackLayout {
            id: _container;

            anchors.top: _bar.bottom;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.left: parent.left;

            currentIndex: _bar.currentIndex;

            Control {
                id: _color_panel

                //anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true

                Label {
                    id: _ambient_label

                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: G.Style.smallPadding

                    text: "AMBIENT COLOR"
                    font: G.Style.fonts.label
                    color: G.Style.colors.textColorBase
                }

                Rectangle {
                    id: _ambient_color

                    anchors.top: _ambient_label.bottom
                    anchors.left: _color_panel.left
                    anchors.right: _edit_ambient.left
                    anchors.leftMargin: G.Style.smallPadding
                    anchors.rightMargin: G.Style.smallPadding

                    height: G.Style.mediumButtonHeight

                    color: _edit_dialog.color

                    radius: G.Style.buttonRadius
                    border.color: G.Style.colors.gutterColor
                    border.width: 1
                }

                G.IconButton {
                    id: _edit_ambient

                    anchors.verticalCenter: _ambient_color.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: G.Style.smallPadding
                    size: G.Style.iconSmall;
                    iconName: G.Icons.icons["pencil"]

                    onClicked: {
                        _color_dialog.color = _edit_dialog.color
                        _color_dialog.open();
                    }
                }
            }

            Control {
                id: _texture_panel

                //anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true

                Label {
                    id: _texture_label

                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: G.Style.smallPadding

                    text: "TEXTURE FILE"
                    font: G.Style.fonts.label
                    color: G.Style.colors.textColorBase
                }

                Rectangle {
                    id: _texture_image

                    anchors.top: _texture_label.bottom
                    anchors.left: _texture_panel.left
                    anchors.right: _edit_texture.left
                    anchors.leftMargin: G.Style.smallPadding
                    anchors.rightMargin: G.Style.smallPadding

                    height: G.Style.mediumButtonHeight

                    color: G.Style.colors.transparent

                    radius: G.Style.buttonRadius
                    border.color: G.Style.colors.gutterColor
                    border.width: 1

                    Image {
                        anchors.fill: parent
                        fillMode: Image.Stretch
                        source: "file://" + _edit_dialog.texture
                    }
                }

                G.IconButton {
                    id: _edit_texture

                    anchors.verticalCenter: _texture_image.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: G.Style.smallPadding
                    size: G.Style.iconSmall;
                    iconName: G.Icons.icons["folder-open"]

                    onClicked: {
                        _texture_dialog.folder = _edit_dialog.texture
                        _texture_dialog.open();
                    }
                }
            }
        }

        standardButtons:  Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if (_container.currentIndex == 0) {
                param.setColor(param.colorIndexAt(_colors.currentValue), _edit_dialog.color)
            } else {
                param.setTexture(param.colorIndexAt(_colors.currentValue), _edit_dialog.texture)
            }
        }

    }

    P.FileDialog {
        id: _texture_dialog

        nameFilters: [ "Image files (*.jpg)" ]
        title: "Open texrture file"
        modality: Qt.WindowModal;
        fileMode: P.FileDialog.OpenFile

        onAccepted: {
            _edit_dialog.texture = urlToPath(_texture_dialog.file.toString())
        }
    }

    P.ColorDialog {
        id: _color_dialog
        onAccepted: {
            _edit_dialog.color =_color_dialog.color
        }
    }

    function urlToPath(urlString) {
        var s
        if (urlString.startsWith("file:///")) {
            var k = urlString.charAt(9) === ':' ? 8 : 7
            s = urlString.substring(k)
        } else {
            s = urlString
        }
        return decodeURIComponent(s);
    }

    Component.onCompleted: {
    }
}