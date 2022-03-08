import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebChannel
import QtWebEngine

Control {

    id: self;

    QtObject {
        id: bridge;

        property var contents: '';
        property var language: '';
        property var theme: '';

        signal sendDataChanged(string name, string value);

        function send(name, value) {
            // console.log("QML: sending", name, value);

            sendDataChanged(name, JSON.stringify(value));
        }

        function receive(name, value) {
            // console.log("QML: receiving", name, value);

            switch(name) {
            case "theme":
                theme = value;
                break;
            case "language":
                language = value;
                break;
            case "value":
                contents = value;
                break;
            default:
                break;
            }
        }

        function init() {
            send("language", "python");
            send("theme", "vs-dark");
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
            console.log(message, lineNumber, sourceID);
        }
    }
}
