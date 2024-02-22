import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Window

import Qt.labs.platform as P
import QtCore

import xLogger  as L

import gnomon.Visualization as GV
import gnomon.Workspaces    as GW

import gnomonQuick.Controls    as G
import gnomonQuick.Workspaces  as G
import gnomonQuick.Menus       as G
import gnomonQuick.Style       as G

import gnomon.Pipeline  as GP
import gnomon.Project   as GP
import gnomon.Utils
import "." as G

G.Application {

    id: window;
    objectName: "window";

    property alias world: header.world;
    property alias workspace_list: _workspaces_model;
    property alias recent_projects: _recent_projects;
    property alias header_state: header.state
    property string opened_files: "";
    property bool drawelr_closed: false;
    property bool load_in_progress: false;

    signal getScreenshot(string id);
    signal workspaceThumbnailUpdated(int id);
    signal insideParamFigure();


    palette {
        window: G.Style.colors.bgColor
        windowText: G.Style.colors.textColorNeutral
        base: G.Style.colors.fgColor
        alternateBase: G.Style.colors.alternateFgColor
        //toolTipBase: G.Style.colors.
        //toolTipText: G.Style.colors.
        placeholderText: G.Style.colors.textColorFaded
        text: G.Style.colors.textColorBase
        button: G.Style.colors.baseColor
        buttonText: G.Style.colors.textColorBase
        brightText: G.Style.colors.textColorDarkNeutral

        disabled {

        }

        inactive {

        }
    }

// /////////////////////////////////////////////////////////////////////////////
// PoC Menu
// /////////////////////////////////////////////////////////////////////////////

    Settings {
        id: stt
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
        property alias opened_projects: window.opened_files;
        property int mode: 0
    }

    P.FileDialog {
        id: loadFileDialog
        folder: GP.ProjectManager.project == null? "" : "file://"+GP.ProjectManager.project.currentDir

        nameFilters: ["Json files (*.json)"]

        onAccepted: {
            load_session(loadFileDialog.file);
        }
    }

    P.FileDialog {
        id: saveFileDialog

        title: "save Gnomon Pipeline"

        fileMode: P.FileDialog.SaveFile
        folder: GP.ProjectManager.project == null? "" : "file://"+GP.ProjectManager.project.currentDir
        currentFile: folder + "/" + (GP.PipelineManager.pipeline.name ? GP.PipelineManager.pipeline.name : "pipeline") + ".json"

        modality: Qt.WindowModal;
        nameFilters: [ "Json files (*.json)" ]

         onAccepted: {
            GP.PipelineManager.pipeline.exportToJson(saveFileDialog.file);
        }
    }

    P.FolderDialog {
        id: folderDialog
        folder: P.StandardPaths.standardLocations(P.StandardPaths.PicturesLocation)[0]
        onAccepted : {
            GP.projectManager.newProject(folderDialog.currentFolder, "")
        }
    }

    Component {
        id: platform_menu

        P.MenuBar {
            // If you edit this menu,
            // Dont forget to edit the menu for linux in main.header.qml as well
            P.Menu {
                title: qsTr("File")

                P.MenuItem {
                    text: qsTr("Open")
                    shortcut: StandardKey.Open
                    onTriggered: {
                        loadFileDialog.folder = "file://"+GP.ProjectManager.project.currentDir
                        loadFileDialog.open();
                    }
                }
                P.MenuItem {
                    text: qsTr("Save Pipeline")
                    shortcut: StandardKey.Save
                    onTriggered: {
                        saveFileDialog.currentFile = "file://"+GP.ProjectManager.project.currentDir + "/" + (GP.PipelineManager.pipeline.name ? GP.PipelineManager.pipeline.name : "pipeline") + ".json"
                        saveFileDialog.open();
                    }
                }

                P.MenuItem {
                    text: qsTr("New Project")
                    shortcut: StandardKey.New
                    onTriggered: {
                        //TODO: come back on the behavior of new project from anywhere (should it close everything?)
                        //TODO: should use the new project dialog as well
                        folderDialog.open();
                    }
                }
                P.MenuItem {
                    text: qsTr("Save Project")
                    shortcut: StandardKey.Save
                    onTriggered: {
                        GP.ProjectManager.project.save()
                    }
                }

            }

        }
    }
    Loader {
        sourceComponent: Qt.platform.os == "osx" ? platform_menu : null;
    }

// ////////////////////////////////////////////////////////////////////////////


    Item {
        id: _internal;

        property var menu_sources: [];
    }

    ListModel {
        id: _workspaces_model;
    }

    ListModel {
        id: _recent_projects;
    }

    G.Header {

        id: header;

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;

        z: 1000;

        G.Separator {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            visible: Qt.platform.os != "osx"
            height: 1
        }
    }

    G.Dragger {
        anchors.horizontalCenter: header.horizontalCenter;
        anchors.top: header.bottom;
        anchors.topMargin: -1 * height/2;

        z: header.z+1;

        onClicked: {
            if (header.world.count > 0 && header.state == "UNANCHORED") {
                header.state = "ANCHORED"
            } else {
                header.state = "UNANCHORED"
            }
        }
    }

    G.Footer { id: footer;
        visible: true;
        start_state: stack_launcher.currentIndex == 0
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }

    G.Dragger {
        anchors.horizontalCenter: header.horizontalCenter;
        anchors.top: footer.top;
        anchors.topMargin: -1 * height/2;
        visible: false;

        z: footer.z+1;

        // onClicked: footer.state = footer.state == 'UNANCHORED' ? 'ANCHORED' : 'UNANCHORED';
    }

    FocusScope {

        anchors.fill: parent;

        focus: true;

        Keys.onPressed: (event) => {
            if (event.modifiers & Qt.ControlModifier) {
                if (event.key == Qt.Key_Right) {
                    event.accepted = true;
                    if(drawer.opened()) {
                        drawer.close()
                    } else {
                        drawer.open()
                    }
                } else if (event.key == Qt.Key_Down) {
                    event.accepted = true;
                    footer.state = footer.state == 'UNANCHORED' ? 'ANCHORED' : 'UNANCHORED';
                } else if (event.key == Qt.Key_Up) {
                    event.accepted = true;
                    header.state = header.state == 'UNANCHORED' ? 'ANCHORED' : 'UNANCHORED';
                } else if (event.key == Qt.Key_Left) {
                    event.accepted = true;
                    if(drawel.opened()) {
                        drawel.close()
                    } else {
                        drawel.open()
                    }
                // new workspace
                } else if (event.key == Qt.Key_W && event.modifiers & Qt.ShiftModifier) {
                    event.accepted = true;
                    _workspace_dialog.update_highlight();
                    _workspace_dialog.open();
                // switch workspace
                } else if (event.key == Qt.Key_W) {
                    event.accepted = true;
                    window.create_workspace_screenshot();
                    _switch_workspace_dialog.open();
                }
            }
        }

        SplitView {

            id: _split;

            anchors.fill: parent;
            anchors.topMargin: header.height;
            anchors.leftMargin: 0;
            anchors.rightMargin: 0;
            anchors.bottomMargin: footer.height;

            G.Drawel {
                id: drawel;
                SplitView.fillHeight: true;
                implicitWidth: 400;
            }

            StackLayout {
                id: stack_launcher

                currentIndex: 0

                SplitView.fillHeight: true;
                SplitView.fillWidth: true;

                G.WorkspaceLauncher { }

                StackLayout {
                    id: workspaces;
                }
            }

            G.Drawer {
                id: drawer;
                SplitView.fillHeight: true;
                implicitWidth: 400;
            }

            handle: G.Separator {

                id: _separator;

                color: G.Style.colors.embossColor;

                G.Dragger {

                    id: _dragger;

                    anchors.centerIn: parent;

                    orientation: Qt.Vertical;

                    z: 1000;

                    onClicked: {

                        var target = null;

                        for(var i = 0; i < _split.children.length; i++) {

                            if(_separator == _split.children[i]) {
                                if(i == 1)
                                    target = drawel;
                                if(i == 2)
                                    target = drawer;
                            }
                        }

                        if (target.opened())
                            target.close();
                        else
                            target.open();
                    }
                }
            }
        }
    }

    G.Settgs { id: settgs; }
    G.Journl { id: journl; }
    
    Connections {
        target: GP.SessionManager;
        function onFinished() {
            window.load_in_progress = false;
        }
        function onFailed(file) {
            _failed_pipeline_toast.file_path = file
            _failed_pipeline_toast.open()
            remove_from_history(file)
        }
    }

    G.Toast {
        id: _failed_pipeline_toast

        property string file_path: "";

        header: "Pipeline failed"
        message: "An error occured while loading the pipeline from file :\n" + file_path;

        type: G.Style.ButtonType.Warning
    }

        Timer {
            interval: 10000  //in msec
            running: true
            repeat: true
            onTriggered: {
                let system_stats = GV.World.systemStat() //memory total, used, this
                
                if( (system_stats[0] - system_stats[1]) < 1000*1000   ) // if less than 1Gb of mem left
                {
                    console.log("implement cache strategy, mem left: ", (system_stats[0] - system_stats[1]));
                    

                }
            }
    }



// /////////////////////////////////////////////////////////////////////////////
// Focused views API
// /////////////////////////////////////////////////////////////////////////////

    property Item currentView: null;

    onCurrentViewChanged: {
        drawel.update_menu();
    }

// /////////////////////////////////////////////////////////////////////////////
    // Projects/Worksapces API
// /////////////////////////////////////////////////////////////////////////////

    function setup(name, context, tags)
    {

    }

    function workspaces_container()
    {
        return workspaces;
    }

    function workspace_at(index)
    {
        return workspaces.children[index];
    }

    function current_workspace_index()
    {
        return workspaces.currentIndex;
    }

    function current_workspace()
    {
        return workspaces.children[workspaces.currentIndex];
    }

    function create_workspace_screenshot()
    {
        window.getScreenshot(workspaces.currentIndex);
    }

    function update_thumbnail(id)
    {
        window.workspaceThumbnailUpdated(id);
    }

    function history_set_last_used(_folder) {
        let folder_source = _folder.toString();
        let projectInfo = GP.ProjectManager.readProjectInfo(folder_source)

        let project_name = projectInfo.name

        if(!project_name){
            let folder_path = _folder.toString()
            project_name = folder_path.slice(folder_path.lastIndexOf("/")+1)
        }

        for(let idx = 0; idx < window.recent_projects.count; idx++) {
            let project = window.recent_projects.get(idx)
            if(project.name === project_name && project.source === folder_source) {
                window.recent_projects.move(idx, 0, 1)
                return
            }
        }
    }

    function add_to_history(_folder){
        let folder_source = _folder.toString();

        let projectInfo = GP.ProjectManager.readProjectInfo(folder_source)

        let project_name = projectInfo.name
        let project_description = projectInfo.description
        let project_last_modified = projectInfo.lastModified

        if(!project_name){
            let folder_path = _folder.toString()
            project_name = folder_path.slice(folder_path.lastIndexOf("/")+1)
        }
        const recent_projects_array = [];
        for(let idx = 0; idx < window.recent_projects.count; idx++) {
            recent_projects_array.push(window.recent_projects.get(idx))
        }
        const add_project = !recent_projects_array.some(project => (
            project.name === project_name && project.source === folder_source
        ));

        if(add_project) {
            if (window.recent_projects.count > 15) {
                window.recent_projects.remove(14)
            }
            let info = GUtils.projectInfo(folder_source)
            info.source = folder_source
            window.recent_projects.append(projectInfo)
            recent_projects_array.splice(0, 0, window.recent_projects.get(window.recent_projects.count-1))
            window.opened_files = JSON.stringify(recent_projects_array)
        } else {
            history_set_last_used(_folder)
        }
    }

    function remove_from_history(folder) {
        let folder_source = folder.toString();
        //console.log("Ref:: ", file_source)
        for(let i=0; i<window.recent_projects.count; i++) {
            //console.log(window.recent_projects.get(i).source)
            if(window.recent_projects.get(i).source === folder_source) {
                console.log("Removing from history project ", i)
                window.recent_projects.remove(i);
                break;
            }
        }
        let _projects = []
        for(let i=0; i<window.recent_projects.count; i++){
            _projects.push(window.recent_projects.get(i))
        }
        window.opened_files = JSON.stringify(_projects)
    }

    function switch_from_launcher(source: string)
    {
        if (source === undefined) {
            source = "qrc:/qt/qml/gnomonQuick/Workspaces/WorkspaceBrowsing.qml"
        }
        stack_launcher.currentIndex  = 1
        window.drawelr_closed = false
        GP.SessionManager.newWorkspace(source)
    }

// /////////////////////////////////////////////////////////////////////////////
// Project API: Workspace management
// /////////////////////////////////////////////////////////////////////////////

    function check_if_forms_in_world() {
        if(header.world) {
            let workspace = current_workspace()
            if(header.world.empty) {
                console.log("Warning, no form exported in the world !")
                return false;
            }
        }
        return true;
    }

    function add_workspace(source: string, uuid: string, fill: bool): int
    {
        if(window.current_workspace()) {
            window.current_workspace().d.saveState();
        }
        const workspace_component = Qt.createComponent(source);
        if (workspace_component.status == Component.Ready) {
            const workspace = workspace_component.createObject(workspaces, {uuid: uuid});
            if(workspace.workspace_title != "Launcher") {
                const workspace_index = workspaces.count - 1;
                footer.workspaceName = workspace.workspace_title;
                workspaces.currentIndex = workspace_index;
                const specific_menu = source.replace(".qml", "Menu.qml").replace("Workspaces", "Menus");
                _internal.menu_sources.push(specific_menu);
                drawel.update_menu()
                drawer.update_menu(specific_menu);
                if(fill) {
                    workspace.fill()
                }
                // window.create_workspace_screenshot();
                _workspaces_model.append({"title": workspace.workspace_title, "index": workspace_index});
                GP.SessionManager.setActiveWorkspace(workspace_index)
                return workspace_index;
            }
        } else {
            console.error(workspace_component.errorString());
        }
        return -1;
    }

    G.WorkspaceDialog {
        id: _workspace_dialog;
    }

    G.SwitchWorkspaceDialog {
        id: _switch_workspace_dialog;
    }

    function switch_workspace(index: int)
    {
        window.current_workspace().d.saveState();
        window.drawelr_closed = false
        stack_launcher.currentIndex  = 1
        workspaces.currentIndex = index;

        footer.workspaceName = window.current_workspace().workspace_title;

        drawel.update_menu();
        drawer.update_menu(_internal.menu_sources[index]);
        window.current_workspace().d.restoreState();
        if(window.current_workspace().viewSelected) {
            window.currentView = window.current_workspace().viewSelected
            window.currentView.forceFocus()
        }
        GP.SessionManager.setActiveWorkspace(index)
    }


// /////////////////////////////////////////////////////////////////////////////
// Load session API:
// /////////////////////////////////////////////////////////////////////////////

    function workspace_source(workspace_name) {
        let workspace_name_source
        for(let i=0; i<_workspace_dialog.available_workspaces.count; i++) {
            let temp_workspace = _workspace_dialog.available_workspaces.get(i)
            if(temp_workspace.type==workspace_name) {
                workspace_name_source = temp_workspace.source
            }
        }
        return workspace_name_source
    }

    function workspaceBrowser() : GW.WorkspaceBrowser {
        let idx_browsing = -1
        for(let i=0; i<window.workspace_list.count; i++) {
            let workspace = window.workspace_list.get(i)
            if(workspace.title == "Browsing")
                idx_browsing = i
        }
        if(idx_browsing >= 0) {
            window.switch_workspace(idx_browsing)
        } else {
            idx_browsing = GP.SessionManager.newWorkspace(workspace_source("gnomonWorkspaceBrowser"))
        }
        return window.current_workspace().d
    }

    function workspacePython(parameters: object) : GW.WorkspacePythonAlgorithm {
        GP.SessionManager.newWorkspace(workspace_source("gnomonWorkspacePythonAlgorithm"))
        let workspace = window.current_workspace()
        workspace.editor.contents = parameters["python_code"]
        workspace.d.code.text = workspace.editor.contents
        workspace.d.code.parseCode()
        workspace.updateParametersModel()
        workspace.d.editMode = false
        for(let param in parameters) {
            if (param != "python_code") {
                if ("value" in parameters[param]) {
                    workspace.d.parameters[param].value = parameters[param]["value"]
                } else if ("values" in parameters[param] && "index" in parameters[param]) {
                    workspace.d.parameters[param].values = parameters[param]["values"]
                    workspace.d.parameters[param].index = parameters[param]["index"]
                } else {
                    console.warn("function loadWorkspace TODO: implement for param : " , param, parameters[param])
                }
            }
        }
        return workspace.d

    }

    function workspaceLSystem(parameters: object) : GW.WorkspaceLSystemModel {
        GP.SessionManager.newWorkspace(workspace_source("gnomonWorkspaceLSystemModel"))
        let workspace = window.current_workspace()
        workspace.editor.contents = parameters["lsystem_code"]
        workspace.d.text = workspace.editor.contents
        workspace.d.derivationLength = parameters["derivation_length"]
        for(let param in parameters) {
            if ((param != "lsystem_code") && (param != "derivation_length")) {
                if ("value" in parameters[param]) {
                    workspace.d.parameters[param].value = parameters[param]["value"]
                } else if ("values" in parameters[param] && "index" in parameters[param]) {
                    workspace.d.parameters[param].values = parameters[param]["values"]
                    workspace.d.parameters[param].index = parameters[param]["index"]
                } else {
                    console.warn("function loadWorkspace TODO: implement for param : " , param, parameters[param])
                }
            }
        }
        return workspace.d
    }

    function workspaceMorphonet(data : objet) : GW.WorkspaceMorphonet {
        GP.SessionManager.newWorkspace(workspace_source("gnomonWorkspaceMorphonet"))
        let workspace = window.current_workspace()
        workspace.d.importDataset(data["id"], data["voxelsize"], data["start_time"], data["end_time"])
        return workspace.d
    }

    function loadWorkspace(plugin_name : string, parameters : object) : GW.AlgorithmWorkspace {
        let workspace_name = _workspace_dialog.plugin_workspace[plugin_name];
        if(typeof workspace_name === "undefined") {
            console.warn("ERROR: No plugin named ", plugin_name)
            return null;
        }
        console.log("Loading workspace ", workspace_name , " with plugin ", plugin_name)
        GP.SessionManager.newWorkspace(workspace_source(workspace_name))
        let workspace = window.current_workspace()
        workspace.d.currentIndex = workspace.d.algorithms.indexOf(plugin_name)
        for(let param in parameters) {
            if ("value" in parameters[param]) {
                workspace.d.parameters[param].value = parameters[param]["value"]
            } else if ("values" in parameters[param] && "index" in parameters[param]) {
                workspace.d.parameters[param].values = parameters[param]["values"]
                workspace.d.parameters[param].index = parameters[param]["index"]
            } else {
                console.warn("function loadWorkspace TODO: implement for param : " , param, parameters[param])
            }
        }

        return workspace.d
    }

    function load_session_from_pipeline(pipeline) {
        console.log("Loading session from", pipeline.name, "pipeline");
        window.load_in_progress = true;
        let res = GP.ProjectManager.project.loadSessionFromPipeline(pipeline);
        if(res) {
            console.log("Session Loaded ");
        } else {
            console.warn("An error occured while loading the session");
        }
        stack_launcher.currentIndex = 1;
    }

    // TODO: still necessary? (called only from the menu)
    function load_session(json_path) {
        console.log("Loading session from ", json_path);
        window.load_in_progress = true;
        let res = GP.ProjectManager.project.loadSessionFromPipelineFile(json_path);
        if(res) {
            console.log("Session Loaded ");
        } else {
            console.warn("An error occured while loading the session");
        }

        stack_launcher.currentIndex = 1;
    }

    function load_project(project_url) {
        console.log("Loading session from ", project_url);
        //window.load_in_progress = true;
        GP.ProjectManager.openProject(project_url)
        //GP.PrpjectManager.project.loadSession()

        stack_launcher.currentIndex = 1;
    }

    function open_blank_project(project_url, load_pipeline=false) {
        console.log("New session from ", project_url);
        //window.load_in_progress = true;
        GP.ProjectManager.openProject(project_url, false, load_pipeline)
        //GP.PrpjectManager.project.loadSession()

        stack_launcher.currentIndex = 1;
    }

    function closeWorkspace(index) {
        if (window.workspace_list.count == 1) {
            _switch_workspace_dialog.reject();
            reset();
            return;
        }

        _workspaces_model.remove(index)
        workspaces.children[index].destroy()

        //if we remove from index < to currentIndex, the currentIndex needs to change
        if(workspaces.currentIndex >= index) {
            switch_workspace(workspaces.currentIndex-1);
        }
    }

    function reset() {
        console.log("reset called");

        window.drawelr_closed = true
        header.state = 'UNANCHORED'
        footer.state = 'UNANCHORED'
        footer.workspaceName = ""
        stack_launcher.currentIndex = 0

        //clear workspaces
        let nb_wp = workspaces.count -1
        for(let i=nb_wp; i>=0; i--) {
            workspaces.children[i].destroy()
        }
        workspaces.currentIndex = -1
        workspaces.children = []
        _workspaces_model.clear()

        //clear world
        let nb_forms = header.world.count -1
        for(let i=nb_forms; i>=0; i--) {
            GV.World.deleteForm(header.getAndRemoveWorldId(i), true)
        }
    }

    Component.onCompleted: {
        GP.SessionManager.setWindow(window)

        G.Style.mode = stt.mode
        window.width = Math.max(window.width, G.Style.windowMinWidth)
        window.height = Math.max(window.height, G.Style.windowMinHeight)

        window.recent_projects.clear()

        if(stt.opened_projects) {
            let files = JSON.parse(stt.opened_projects)
            for(let i=0; i<files.length; i++){
                let info = GUtils.projectInfo(files[i].source)
                info.source = files[i].source
                window.recent_projects.append(info)
            }
        }
        footer.workspaceName = ""
    }
}
