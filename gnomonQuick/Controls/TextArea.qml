import QtQuick              2.15
import QtQuick.Controls     2.15
import QtQuick.Layouts      1.15

import Qt5Compat.GraphicalEffects

import gnomonQuick.Controls as G
import gnomonQuick.Style as G

TextArea {
    id: pipeline_name
    text: GP.PipelineManager.pipeline.name
    font: G.Style.fonts.value
    color: G.Style.colors.textColorBase
    placeholderTextColor: G.Style.colors.textColorDeEmphasize
    wrapMode: TextInput.WrapAnywhere
    selectByMouse: true
    //textMargin: G.Style.smallPadding
    clip: true

    cursorDelegate: Rectangle
    {
        color: G.Style.colors.baseColor;
        width: 1.5
        visible: parent.activeFocus && !parent.readOnly && parent.selectionStart === parent.selectionEnd
    }

    background: Rectangle {
        anchors.fill: parent
        color: G.Style.colors.gutterColor
        radius: G.Style.panelRadius
    }
}
