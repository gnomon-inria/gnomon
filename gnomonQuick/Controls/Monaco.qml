import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebChannel
import QtWebEngine

Control {

    id: self;

    property var connected: false;
    property var theme;
    property var language;
    property var contents;

    signal modified(var content);

       onThemeChanged: if(self.connected) bridge.send('theme',    self.theme);
    onLanguageChanged: if(self.connected) bridge.send('language', self.language);
    onContentsChanged: if(self.connected) bridge.send('value',    self.contents);

    QtObject {
        id: bridge;

        signal sendDataChanged(string name, string value);

        function send(name, value) {
            // console.log("QML: sending", name, value);

            sendDataChanged(name, JSON.stringify(value));
        }

        function receive(name, value) {
            // console.log("QML: receiving", name, value);

            switch(name) {
            case "theme":
                console.log("Theme changed:", value);
                break;
            case "language":
                console.log("Language changed:", value);
                break;
            case "value":
                //  console.log("Contents changed", value);
                self.modified(value);
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
        }
    }

    WebEngineView {

        id: view;

        anchors.fill: parent;

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

        url: 'qrc:/js/monaco/monaco.html';

        onJavaScriptConsoleMessage: (level, message, lineNumber, sourceID) => {
            // console.log(message, lineNumber, sourceID);
        }
    }
}
