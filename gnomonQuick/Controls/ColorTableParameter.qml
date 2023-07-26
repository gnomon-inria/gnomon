import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15
import Qt.labs.platform  1.0 as P

import Qt5Compat.GraphicalEffects

import gnomon.Visualization 1.0 as GV
import gnomon.Utils as G
import gnomonQuick.Controls as G
import gnomonQuick.Style as G

Control {
    id: _control

    required property var param

    implicitWidth: G.Style.smallPanelWidth
    implicitHeight: _label.implicitHeight + _colors.implicitHeight + _value.implicitHeight + 2*G.Style.smallPadding

    Label {
        id: _label

        anchors.left: parent.left
        anchors.top: parent.top

        text: _control.param ? _control.param.label.toUpperCase() : ""
        font: G.Style.fonts.label
        color: G.Style.colors.textColorBase
    }

    G.ComboBox {
        id: _colors

        anchors.top: _label.bottom
        anchors.left: _control.left
        anchors.right: _edit_appeareance.left
        anchors.rightMargin: G.Style.smallPadding

        model: _control.param ? _control.param.colorIndexCount : 0
        displayText: ""

        delegate: G.ComboBoxDelegate {
            width: _colors.width
            property int colorIndex: _control.param ? _control.param.colorIndexAt(index) : -1
            property bool isColor: _control.param ? _control.param.isColor(colorIndex) : false
            property bool isMaterial: _control.param ? _control.param.isMaterial(colorIndex) : false
            property bool isTexture: _control.param ? _control.param.isTexture(colorIndex) : false
            text: "Appearance " + colorIndex
            Image {
                id: _image
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                visible: isTexture
                cache: false
                asynchronous: true
                source: isTexture ? "file://" + param.texture(colorIndex) : ""
            }

            G.SimpleView {
                id: _d_view
                anchors.fill: parent
                visible: isMaterial
                enabled: false

                color: G.Style.colors.gutterColor

                viewLogic: _d_material_preview.view;
                export_enabled: false;


            }
            GV.MaterialPreview {
                id: _d_material_preview

                ambient: isMaterial ? param.ambient(colorIndex) : G.Style.colors.warningColor
                diffuse: isMaterial ? param.diffuse(colorIndex) : 3
                specular: isMaterial ? param.specular(colorIndex) : transparent
                emission: isMaterial ? param.emission(colorIndex) : transparent
                shininess: isMaterial ? param.shininess(colorIndex) : 0.2
                transparency: isMaterial ? param.transparency(colorIndex) : 0
            }

            Component.onCompleted: {
                G.Associator.associate(_d_view, _d_material_preview.view);
                _d_material_preview.init()
                _d_material_preview.setCamera()
            }
        }

        background: Rectangle {
            id: _color_bg

            property int _colorIndex: param.colorIndexAt(_colors.currentValue)
            property bool _isColor: param ? param.isColor(_colorIndex) : false
            property bool _isMaterial: param ? param.isMaterial(_colorIndex) : false
            property bool _isTexture: param ? param.isTexture(_colorIndex) : false

            color: _isColor ? param.color(_colorIndex) : (_isMaterial ? param.ambient(_colorIndex) : G.Style.colors.transparent)

            radius: G.Style.buttonRadius
            border.color: G.Style.colors.gutterColor
            border.width: 1

            Image {
                anchors.fill: parent
                fillMode: Image.Stretch
                visible: param && param.isTexture(param.colorIndexAt(_colors.currentValue))
                source: visible? "file://" + param.texture(param.colorIndexAt(_colors.currentValue)) : ""
            }

            G.SimpleView {
                id: _b_view
                anchors.fill: parent
                visible: _color_bg._isMaterial
                enabled: false

                color: G.Style.colors.gutterColor

                viewLogic: _b_material_preview.view;
                export_enabled: false;

            }
            GV.MaterialPreview {
                id: _b_material_preview

                ambient: _color_bg._isMaterial ? param.ambient(_color_bg._colorIndex) : G.Style.colors.warningColor
                diffuse: _color_bg._isMaterial ? param.diffuse(_color_bg._colorIndex) : 3
                specular: _color_bg._isMaterial ? param.specular(_color_bg._colorIndex) : transparent
                emission: _color_bg._isMaterial ? param.emission(_color_bg._colorIndex) : transparent
                shininess: _color_bg._isMaterial ? param.shininess(_color_bg._colorIndex) : 0.2
                transparency:_color_bg._isMaterial ? param.transparency(_color_bg._colorIndex) : 0
            }

            Component.onCompleted: {
                G.Associator.associate(_b_view, _b_material_preview.view);
                _b_material_preview.init()
                _b_material_preview.setCamera()
            }
        }
    }

    G.IconButton {
        id: _edit_appeareance

        anchors.verticalCenter: _colors.verticalCenter
        anchors.right: _add_color.left
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: "pencil"

        onClicked: {
            let i = param.colorIndexAt(_colors.currentValue)
            _edit_dialog.color_index = i

            _edit_dialog.color = param.color(i)

            _edit_dialog.texture = param.texture(i)

            _edit_dialog.ambient = param.ambient(i)
            _edit_dialog.diffuse = param.diffuse(i)
            _edit_dialog.specular = param.specular(i)
            _edit_dialog.emission = param.emission(i)
            _edit_dialog.shininess = param.shininess(i)
            _edit_dialog.transparency = param.transparency(i)

            _edit_dialog.open();
        }
    }

    G.IconButton {
        id: _add_color

        anchors.verticalCenter: _colors.verticalCenter
        anchors.right: _control.right
        anchors.rightMargin: G.Style.smallPadding
        size: G.Style.iconSmall;
        iconName: "plus"

        onClicked: {
            console.log("Add color!")
        }
    }

    Label {
        id: _value

        anchors.topMargin: G.Style.sizes.s1
        anchors.top: _colors.bottom
        anchors.left: parent.left

        property int colorIndex: _control.param ? _control.param.colorIndexAt(_colors.currentValue) : -1

        text: "Appearance " + colorIndex
        font: G.Style.fonts.value
        color: G.Style.colors.hoveredBaseColor
    }

    G.Dialog {
        id: _edit_dialog;

        property int color_index: 0

        property var color : G.Style.colors.transparent

        property var ambient : G.Style.colors.transparent
        property double diffuse : 0
        property var specular : G.Style.colors.transparent
        property var emission : G.Style.colors.transparent
        property double shininess : 0
        property double transparency : 0


        GV.MaterialPreview {
            id: material_preview

            ambient: _edit_dialog.ambient
            diffuse: _edit_dialog.diffuse
            specular: _edit_dialog.specular
            emission: _edit_dialog.emission
            shininess: _edit_dialog.shininess
            transparency: _edit_dialog.transparency
        }

        property var texture : ""

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.largeDialogWidth
        height: G.Style.largeDialogHeight

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
            anchors.margins: G.Style.mediumColumnSpacing

            currentIndex: _bar.currentIndex;

            Control {
                id: _color_panel

                //anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true

                G.SimpleView {
                    id: preview
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: G.mediumColumnSpacing
                    width: height
                    color: G.Style.colors.gutterColor

                    viewLogic: material_preview.view;
                    export_enabled: false;

                    Component.onCompleted: {
                        G.Associator.associate(preview, material_preview.view);
                        material_preview.init()
                    }
                }

                GridLayout {
                    id: grid
                    anchors.top: parent.top
                    anchors.left: preview.right
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.leftMargin: G.mediumColumnSpacing
                    columns: 2

                    Label {
                        id: _ambient_label
                        Layout.row: 0
                        Layout.column: 0
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: G.Style.mediumPadding

                        text: "Ambient"
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    Rectangle {
                        id: _ambient_color
                        Layout.row: 0
                        Layout.column: 1
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        Layout.fillWidth: true

                        color: _edit_dialog.ambient

                        radius: G.Style.buttonRadius
                        border.color: G.Style.colors.gutterColor
                        border.width: 1

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: false
                            onClicked: {
                                _ambient_dialog.color = _edit_dialog.ambient
                                _ambient_dialog.open();
                            }
                        }
                        P.ColorDialog {
                            id: _ambient_dialog
                            onAccepted: {
                                _edit_dialog.ambient = _ambient_dialog.color
                            }
                        }
                    }

                    Label {
                        id: _diffuse_label
                        Layout.row: 1
                        Layout.column: 0
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: G.Style.mediumPadding

                        text: "Diffuse"
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    G.NumericSlider {
                        id: _diffuse_slider
                        Layout.row: 1
                        Layout.column: 1
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        Layout.fillWidth: true

                        value: _edit_dialog.diffuse
                        min: 0
                        max: 4
                        decimals: 2

                        label: "Diffuse strength"
                        doc: ""
                        onValueChanged: {
                            _edit_dialog.diffuse = value
                        }

                    }
                    /*
                    Label {
                        id: _specular_label
                        Layout.row: 2
                        Layout.column: 0
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: G.Style.mediumPadding

                        text: "Specular"
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    Rectangle {
                        id: _specular_color

                        Layout.row: 2
                        Layout.column: 1
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        Layout.fillWidth: true

                        color: _edit_dialog.specular

                        radius: G.Style.buttonRadius
                        border.color: G.Style.colors.gutterColor
                        border.width: 1

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: false
                            onClicked: {
                                _specular_dialog.color = _edit_dialog.specular
                                _specular_dialog.open();
                            }
                        }
                        P.ColorDialog {
                            id: _specular_dialog
                            onAccepted: {
                                _edit_dialog.specular = _specular_dialog.color
                            }
                        }
                    }

                    Label {
                        id: _emission_label
                        Layout.row: 3
                        Layout.column: 0
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: G.Style.mediumPadding

                        text: "Emission"
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    Rectangle {
                        id: _emission_color

                        Layout.row: 3
                        Layout.column: 1
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        Layout.fillWidth: true

                        color: _edit_dialog.emission

                        radius: G.Style.buttonRadius
                        border.color: G.Style.colors.gutterColor
                        border.width: 1

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: false
                            onClicked: {
                                _emission_dialog.color = _edit_dialog.emission
                                _emission_dialog.open();
                            }
                        }
                        P.ColorDialog {
                            id: _emission_dialog
                            onAccepted: {
                                _edit_dialog.emission = _emission_dialog.color
                            }
                        }
                    }*/


                    Label {
                        id: _shininess_label
                        Layout.row: 4
                        Layout.column: 0
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: G.Style.mediumPadding

                        text: "Shininess"
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    G.NumericSlider {
                        id: _shininess_slider
                        Layout.row: 4
                        Layout.column: 1
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        Layout.fillWidth: true

                        value: _edit_dialog.shininess
                        min: 0
                        max: 1
                        decimals: 2

                        label: "Shininess"
                        doc: ""
                        onValueChanged: {
                            _edit_dialog.shininess = value
                        }

                    }

                    Label {
                        id: _transparency_label
                        Layout.row: 5
                        Layout.column: 0
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: G.Style.mediumPadding

                        text: "Transparency"
                        font: G.Style.fonts.formLabel
                        color: G.Style.colors.textColorBase
                    }

                    G.NumericSlider {
                        id: _transparency_slider
                        Layout.row: 5
                        Layout.column: 1
                        Layout.minimumHeight: G.Style.mediumButtonHeight
                        Layout.fillWidth: true

                        value: _edit_dialog.transparency
                        min: 0
                        max: 1
                        decimals: 2

                        label: "Transparency"
                        doc: ""

                        onValueChanged: {
                            _edit_dialog.transparency = value
                        }
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
                    height: G.Style.mediumButtonHeight

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: G.Style.mediumPadding

                    text: "Texture file"
                    font: G.Style.fonts.formLabel
                    color: G.Style.colors.textColorBase
                }

                Rectangle {
                    id: _texture_image

                    anchors.top: parent.top
                    anchors.left: _texture_label.right
                    anchors.right: _edit_texture.left
                    anchors.leftMargin: G.Style.mediumPadding
                    anchors.rightMargin: G.Style.mediumPadding

                    height: G.Style.mediumButtonHeight

                    color: G.Style.colors.transparent

                    radius: G.Style.buttonRadius
                    border.color: G.Style.colors.gutterColor
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: false
                        onClicked: {
                            _texture_dialog.folder = _edit_dialog.texture
                            _texture_dialog.open();
                        }
                    }

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
                    iconName: "folder-open"

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


                let v = param.value
                v[color_index] = {
                    "ambient": _edit_dialog.ambient,
                    "diffuse": _edit_dialog.diffuse,
                    "specular": _edit_dialog.specular,
                    "emission": _edit_dialog.emission,
                    "shininess": _edit_dialog.shininess,
                    "transparency": _edit_dialog.transparency,
                }

                param.value = v
            } else {
                param.setTexture(param.colorIndexAt(_colors.currentValue), _edit_dialog.texture)
            }
            let current_color_index = _colors.currentIndex
            _colors.currentIndex = current_color_index != 0 ? 0 : 1
            _colors.currentIndex = current_color_index
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
