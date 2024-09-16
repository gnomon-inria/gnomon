import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

ItemDelegate {
    id: _control;
    required property int index;
    required property string modelData;

    property string formType;
    property string thumbnailIndex;
    property alias formVisible : _visibility_icon.checked

    signal toggleVisibility(bool flag);
    signal deleteForm();

    implicitHeight: G.Style.formDelegateHeight;

    text: formType
    font: G.Style.fonts.formLabel;

    background: Rectangle {
        opacity: enabled ? 0.8 : 0.1
        color: getBgColor()
        border.width: 0.5
        border.color: getEmbossColor()
    }

    contentItem: G.FormIdentifier {
        formId: _control.thumbnailIndex
        formName: _control.text
    }

    G.IconButton {
        id: _visibility_icon;

        anchors.right: parent.right;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.leftMargin: 4;
        anchors.rightMargin: 10;
        size: 24;
        checked: true;
        iconName: checked ? "eye-outline" : "eye-off-outline";

        onClicked: {
            _visibility_icon.checked = !_visibility_icon.checked;
            _control.toggleVisibility(_visibility_icon.checked)

        }
    }

    G.IconButton {

        anchors.right: _visibility_icon.left;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.margins: 4
        size: 24;
        iconName: "delete";

        onClicked: _control.deleteForm()
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
