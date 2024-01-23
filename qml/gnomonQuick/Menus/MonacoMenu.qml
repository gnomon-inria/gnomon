import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.platform  1.0 as P

import gnomonQuick.Menus      1.0 as G
import gnomonQuick.Workspaces 1.0 as G
import gnomonQuick.Controls   1.0 as G
import gnomonQuick.Style      1.0 as G

import gnomon.Project         1.0 as GP

Item {

    id: _self;

    property var d;
    property string _current_file: "";

    ColumnLayout {
        anchors.fill: parent;
        anchors.margins: G.Style.smallPadding;

        G.ProjectBrowser {
            id: _project_browser

            Layout.fillWidth: true;
            Layout.fillHeight: true;

            rootDir: GP.ProjectManager.project.currentDir
            dataPath: GP.ProjectManager.project.dataPath

            onFileDoubleClicked: (fileUrl) => {
                let relative_path = GP.ProjectManager.project.relativePath(fileUrl)
                let file_name = relative_path.split('/').pop()

                d.code.fileName = file_name;
                d.read(relative_path, false);
            }
        }
    }
}
