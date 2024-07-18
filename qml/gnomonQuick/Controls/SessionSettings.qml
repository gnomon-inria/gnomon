import QtQuick
import QtCore

import gnomon.Project  as GP

Settings {
    id: _self
    location : GP.ProjectManager.project.absolutePath(".gnomon/session/session.ini", true, true)
    onLocationChanged: {
        console.log(location)
    }
    Component.onCompleted: {
        console.log(location)
    }
}