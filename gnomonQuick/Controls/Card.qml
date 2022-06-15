//Qt
import QtQuick
import QtQuick.Controls

//Gnomon
import gnomonQuick.Controls as G
import gnomonQuick.Style as G


Control {
    id: _control


// /////////////////////////////////////////////////////////////////////////////
// PROPERTY AND SIGNAL DECLARATIONS
// /////////////////////////////////////////////////////////////////////////////

    property int type: G.Style.CardType.Base
    property bool outline: false
    property bool rounded: true
    property string tooltip: undefined
    property string thumbnail: undefined

    property alias title: _title.text
    property alias body: _body.text

    signal clicked

    implicitHeight: _title.implicitHeight + _body.implicitHeight
    implicitWidth: _control.thumbnail ? _thumbnail.width + _title.implicitWidth + _body.implicitWidth : _title.implicitWidth + _body.implicitWidth

    padding: G.Style.smallPadding

    background: Rectangle {
        color: _mouse_area.containsMouse? _getHoveredBgColor() : _getBgColor()
        radius: _control.rounded? G.Style.cardRadius : 0

        border.width: _control.outline? 2: 0
        border.color: _getBorderColor()
    }

    ToolTip.visible: _control.tooltip && _mouse_area.containsMouse
    ToolTip.text: _control.tooltip

// /////////////////////////////////////////////////////////////////////////////
// CHILD OBJECTS
// /////////////////////////////////////////////////////////////////////////////


    Rectangle {
        id: _thumbnail;

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: G.Style.smallPadding

        height: parent.height - 2*G.Style.smallPadding
        width: parent.height - 2*G.Style.smallPadding
        radius: _control.rounded? G.Style.panelRadius : 0
        color: G.Style.colors.lightBlue

        visible: _control.thumbnail

        Image {
            anchors.fill: _thumbnail
            fillMode: Image.PreserveAspectFit
            source: _control.thumbnail
        }
    }

    Label {
        id: _title

        anchors.left: _control.thumbnail? _thumbnail.right : _control.left
        anchors.right: _control.right
        anchors.top: _control.thumbnail? _thumbnail.top : _control.top

        anchors.margins: _control.thumbnail? G.Style.smallPadding : G.Style.mediumPadding

        text: _control.title
        font: G.Style.fonts.cardTitle

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop

        wrapMode: Text.Wrap
        color: G.Style.colors.textColorBase
    }

    Label {
        id: _body

        anchors.left: _control.thumbnail? _thumbnail.right : _control.left
        anchors.right: _control.right
        anchors.bottom: _control.thumbnail? _thumbnail.bottom : _control.bottom

        anchors.topMargin: G.Style.smallPadding
        anchors.bottomMargin: _control.thumbnail? G.Style.smallPadding : G.Style.mediumPadding
        anchors.leftMargin: _control.thumbnail? G.Style.smallPadding : G.Style.mediumPadding
        anchors.rightMargin: _control.thumbnail? G.Style.smallPadding : G.Style.mediumPadding

        text: _control.body
        font: G.Style.fonts.cardLabel

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop

        wrapMode: Text.Wrap
        color: G.Style.colors.textColorBase
    }

    MouseArea {
        id: _mouse_area
        anchors.fill: _control
        hoverEnabled: true
        onClicked: {
            _control.clicked()
        }
    }

// /////////////////////////////////////////////////////////////////////////////
// JAVASCRIPT FUNCTIONS
// /////////////////////////////////////////////////////////////////////////////


    function _getHoveredBgColor() {
        if(_control.type === G.Style.CardType.Background)
            return G.Style.colors.fgColor

        if(_control.type === G.Style.CardType.Foreground)
            return G.Style.colors.neutralColor

        return G.Style.colors.hoveredNeutralColor
    }

    function _getBgColor() {
        if(_control.type === G.Style.CardType.Background)
            return G.Style.colors.bgColor

        if(_control.type === G.Style.CardType.Foreground)
            return G.Style.colors.fgColor

        return G.Style.colors.neutralColor
    }

    function _getBorderColor() {
        if(_control.type === G.Style.CardType.Background)
            return G.Style.colors.baseColor

        if(_control.type === G.Style.CardType.Foreground)
            return G.Style.colors.baseColor

        return G.Style.colors.baseColor
    }
}
