import QtQuick
import QtCore

import gnomon.Project  as GP

Settings {
    id: _self
    location : GP.ProjectManager.project ? GP.ProjectManager.project.absolutePath(".gnomon/session/session.ini", true, true) : ""
}