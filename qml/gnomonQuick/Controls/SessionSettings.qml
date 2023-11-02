import QtQuick              
import Qt.labs.settings

import gnomon.Project  as GP

Settings {
    id: _self
    fileName : ".gnomon/session/session.ini"

    Component.onCompleted : {
        GP.SessionManager.sync()
    }
}