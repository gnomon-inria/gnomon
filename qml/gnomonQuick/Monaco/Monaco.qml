import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebChannel
import QtWebEngine

import gnomonQuick.Style    as G

Control {
    id: self;

    property var connected: false;
    property var theme;
    property var language;
    property var contents;
    property bool _suppressOnContentsChanged: false;
    property var markers
    property var fileName;
    property var tabName;
    property var readOnly;

    QtObject {
        id: _internal;

        property var tab_filenames: []
    }

    signal modified(var content);
    signal fileSwitched(var name);
    signal fileClosed(var name);
    signal ideIsReady();
    signal makeFileEditable();

    onThemeChanged: if(self.connected) bridge.send('theme',    self.theme);
    onLanguageChanged: if(self.connected) bridge.send('language', self.language);
    onContentsChanged: if(self.connected && !self._suppressOnContentsChanged) bridge.send('value',    self.contents);
    onMarkersChanged: if(self.connected) bridge.send('markers',  self.markers);

    onFileNameChanged: {
        if (!_internal.tab_filenames.includes(self.fileName)) {
            if (self.connected) {
                bridge.send('fileName', self.fileName);
            }
            _internal.tab_filenames.push(self.fileName)
            self.tabName = self.fileName
        }
    }

    onTabNameChanged: {
        if (self.tabName != self.fileName) {
            if (self.connected) {
                bridge.send('rename', self.tabName);
            }
            let i = _internal.tab_filenames.indexOf(self.fileName)
            _internal.tab_filenames.splice(i, 1)
            _internal.tab_filenames.push(self.tabName)
            self.fileName = self.tabName
        }
    }

    onReadOnlyChanged : {
        bridge.send('readonly', self.readOnly);
        bridge.send('externalFile', self.readOnly);
    }

    //to create a new tab with a name:
    //if(self.connected) bridge.send('newTab', "{\"name\": \"toto\"}");

    QtObject {
        id: bridge;

        signal sendDataChanged(string name, string value);

        function send(name, value) {
            sendDataChanged(name, JSON.stringify(value));
        }

        function receive(name, value) {
            switch(name) {
            case "theme":
                break;
            case "language":
                break;
            case "value":
                self._suppressOnContentsChanged = true;
                self.contents = eval(value)
                self._suppressOnContentsChanged = false;
                self.modified(value);
                break;
            case "filename":
                self.fileSwitched(value);
                break;
            case "new":
                let new_file = eval(value)
                if (!_internal.tab_filenames.includes(new_file)) {
                    _internal.tab_filenames.push(new_file)
                }
                break;
            case "close":
                let closed_file = eval(value)
                if (_internal.tab_filenames.includes(closed_file)) {
                    let i = _internal.tab_filenames.indexOf(closed_file)
                    _internal.tab_filenames.splice(i, 1)
                }
                self.fileClosed(closed_file)
                break;
            case "attemptReadOnly":
                self.makeFileEditable();
                break;
            default:
                break;
            }
        }

        function init() {
            self.connected = true;

            bridge.send('theme',    self.theme);
            bridge.send('language', self.language);
            bridge.send('value',    self.contents);
            bridge.send('rename',    self.tabName);

            self.ideIsReady();
        }
    }

    WebEngineView {

        id: view;

        anchors.fill: parent;

        onActiveFocusChanged: {
            if (view.activeFocus)
                window.currentView = self;
        }

        settings.javascriptEnabled: true
        settings.pluginsEnabled: true
        settings.autoLoadImages: true
        settings.showScrollBars: false;

        webChannel: WebChannel {
            id: channel;

            Component.onCompleted: {
                channel.registerObjects({
                    "bridge": bridge,
                });
            }
        }

        url: 'qrc:/qt/qml/gnomonQuick/Monaco/monaco.html';

        onJavaScriptConsoleMessage: (level, message, lineNumber, sourceID) => {
            console.log(message, lineNumber, sourceID);
        }

        onScrollPositionChanged: {
            // prevent view from scrolling (and hiding tab bar)
            view.runJavaScript("document.body.scrollIntoView({ behavior: 'smooth' });");
        }
    }

    Component.onCompleted: {
        _internal.tab_filenames.push("Tab 0")
    }

    Rectangle {
        id: _focus_indicator;

        width: self.width - 1
        height: self.height - 1
        radius: G.Style.panelRadius;

        color: G.Style.colors.transparent;

        border.width: G.Style.borderWidth;
        border.color: G.Style.colors.baseColor;

        visible: window.currentView == self;
    }

    function forceFocus() {
        view.forceActiveFocus()
    }
}
