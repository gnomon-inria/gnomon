import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform as P
import QtCore

import gnomon.Utils as G
import gnomonQuick.Workspaces as G
import gnomonQuick.Controls   as G
import gnomonQuick.Style      as G

import gnomon.Workspaces as GW
import gnomon.Project    as GP

G.Workspace {
    id: _self;

    workspace_title: "Data Browsing";

    property string current_file: "";

    Settings {
        id: _cache;
        category: "ReaderDialogCache";
    }

    G.SessionSettings {
        category: uuid
        property alias state: d.state
    }

    function requestOpenFiles(urls) {
        let paths = [];
        let default_plugin = ""
        for(let i_n in urls) {
            let path = decodeURIComponent(urls[i_n])
            if (default_plugin == "") {
                default_plugin = _cache.value(path, "")
            }
            console.log(GP.ProjectManager.project.findFile(path))
            paths.push(path)
        }
        d.readerPath = paths.join(",");
        d.requestReaders(default_plugin);
    }

    function checkFileAccessibility(urls) {
        let external_paths = []
        let relative_paths = []
        for(let i_n in urls) {
            let path = decodeURIComponent(urls[i_n])
            if (!GP.ProjectManager.project.isAccessible(path)) {
                if(path.startsWith("file://")) {
                    path = path.slice(7)
                }
                path =  path.split("/").slice(0, -1).join("/")
                external_paths.push(path)
            } else {
                relative_paths.push(GP.ProjectManager.project.relativePath(path));
            }
        }
        if (external_paths.length > 0) {
            _external_data_dialog.urls = urls
            _external_data_dialog.paths = external_paths
            _external_data_dialog.open()
        } else {
            requestOpenFiles(relative_paths)
        }
    }

    fill: () => {}

    focus: true;

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d: GW.WorkspaceBrowser {
        id: d;

        onStarted: idleStart();
        onFinished: idleStop();

        onAvailable: (readers) => {
            if(Object.keys(readers).length == 1) {
                d.readWith(Object.keys(readers)[0])
            } else {
                _reader_dialog.availableReaders.clear();
                for (var r in readers) {
                    _reader_dialog.availableReaders.append({
                              "title": readers[r].metadata.name.toString(),
                              "key": r.toString(),
                            "description": readers[r].description.toString(),
                            "preview": readers[r].preview.toString()
                    });
                }
                _reader_dialog.open();
            }
        }

        onNoReaderAvailable: (ext) => {
            console.log("No reader available: extension not recognized.")
            extension_error.message = "No reader available: extension not recognized."
            extension_error.open()
        }
    }

    G.Toast {
        id: extension_error
        header: "Reader error"
        message: ""
        type: G.Style.ButtonType.Warning
    }

// /////////////////////////////////////////////////////////////////////////////

    G.View {
        id: _view;

        anchors.fill: parent;
        anchors.margins: G.Style.smallPadding;

        onDroppedFromFile: (path) => {
            let urls = path.split(',')
            checkFileAccessibility(urls)

        }
        viewLogic: d.view;

        Component.onCompleted: G.Associator.associate(_view, d.view);
    }

    G.ReaderDialog {
        id: _reader_dialog;

        onReaderSelected: (reader) => {
            idleStart();
            let paths = d.readerPath.split(",")
            for(let i in paths) {
                console.debug("Saving", reader, "as default reader for", paths[i])
                _cache.setValue(paths[i], reader)
            }
            _reader_toast.reader_name = reader;
            _reader_toast.open();
            d.readWith(reader);
        }
    }

     G.Dialog {
        id: _external_data_dialog;

        property var urls: []
        property var paths: []

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: G.Style.smallDialogWidth;
        height: G.Style.smallDialogHeight;

        padding: G.Style.smallPadding;

        parent: Overlay.overlay
        modal: true
        title: "External data"
        standardButtons:  Dialog.Cancel | Dialog.Ok

        Label {
            id: _external_label
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.right: parent.right;
            text: "Your current folder is not part of the project data directories.\n Please specify the parent directory to add to the project"
            wrapMode: Text.WordWrap

            font: G.Style.fonts.cardLabel
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            topPadding: G.Style.smallPadding;
            bottomPadding: G.Style.smallPadding;
            leftPadding: G.Style.mediumPadding
            rightPadding: G.Style.mediumPadding
        }

        ListView {
            id: _external_list_view

            anchors.top: _external_label.bottom;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            anchors.left: parent.left;
            anchors.margins: G.Style.smallPadding;

            clip: true;
            spacing: G.Style.smallPadding;

            model: _external_data_dialog.paths

            delegate: G.TextField {
                id: _delegate

                width: _external_list_view.width
                implicitHeight: G.Style.sizes.s6

                text: modelData

                selectByMouse: true
                mouseSelectionMode: TextInput.SelectCharacters
                readOnly: false

                font: G.Style.fonts.value
                color: _delegate.activeFocus? G.Style.colors.hoveredBaseColor : G.Style.colors.textColorBase
                selectionColor: G.Style.colors.fgColor
                selectedTextColor: G.Style.colors.hoveredBaseColor

                background: Rectangle {
                    color: G.Style.colors.gutterColor
                }

                Rectangle {
                    anchors.left: _delegate.left
                    anchors.right: _delegate.right
                    anchors.top: _delegate.bottom

                    height: 2

                    color: _delegate.acceptableInput? G.Style.colors.okColor : G.Style.colors.dangerColor
                }

                validator: RegularExpressionValidator {
                    id: _validator
                }

                onAcceptableInputChanged: {
                    let valid = true;
                    for (let i_f in _external_data_dialog.paths) {
                        valid = valid & _external_list_view.itemAtIndex(i_f).acceptableInput
                    }
                    _external_data_dialog.footer.standardButton(Dialog.Ok).type =  valid ? G.Style.ButtonType.Base : G.Style.ButtonType.Neutral
                    _external_data_dialog.footer.standardButton(Dialog.Ok).flat = !valid
                    _external_data_dialog.footer.standardButton(Dialog.Ok).enabled = valid
                }

                Component.onCompleted: {
                    let reg_exp  = "("
                    let paths = modelData.split("/")
                    for (var i=0; i<paths.length; i++) {
                        if (i>0) {
                            reg_exp += "|"
                        }
                        reg_exp += paths.slice(0, i+1).join("\/")
                    }
                    reg_exp  += ")\/?"
                    _validator.regularExpression = new RegExp(reg_exp)
                }
            }
        }

        onAccepted: {
            for (let i_f in _external_data_dialog.paths) {
                let data_path = _external_list_view.itemAtIndex(i_f).text
                GP.ProjectManager.project.addDirectoryToDataPath(data_path)
            }
            let relative_paths = [];
            for (let i_f in _external_data_dialog.urls) {
                let url = _external_data_dialog.urls[i_f];
                relative_paths.push(GP.ProjectManager.project.relativePath(url));
            }
            requestOpenFiles(relative_paths)
        }

        onRejected: {
            _external_data_dialog.close();
        }
    }

    G.Toast {
        id: _reader_toast

        property string reader_name: "";

        parent: Overlay.overlay
        header: "Reader updated"
        message: "Your choice has been saved! Next time you will open this file, it will be loaded using " + _reader_toast.reader_name + ". You can use \"LOAD AS\" to choose a different Reader.";

        type: G.Style.ButtonType.Base
    }

    Component.onCompleted :  window.drawelr_closed = false;
}
