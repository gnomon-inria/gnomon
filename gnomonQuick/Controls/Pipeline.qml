import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import gnomonQuick.Controls 1.0 as G
import gnomonQuick.Style    1.0 as G
import gnomonQuick.Graph    1.0 as G
import gnomonQuick.Icons    1.0 as G

import gnomon.Pipeline 1.0 as GP
import gnomon.Visualization 1.0 as GV

Control {

    id: _self;

    // these are used in case the window height or width
    // are variable (as is the case for a collapsible menu)
    property int windowHeight: _self.height
    property int windowWidth: _self.width

    clip: true;

    // /////////////////////////////////////////////////////////////////////////////
    // PIPELINE CONTROLS (SAVE AND EDIT)
    // /////////////////////////////////////////////////////////////////////////////

    G.IconButton {

        id: _save_pipeline;

        z: 1
        anchors.top: _self.top;
        anchors.topMargin: G.Style.smallPadding
        anchors.right: _self.right;
        anchors.rightMargin: G.Style.smallPadding

        size: G.Style.iconMedium;

        iconName: G.Icons.icons["content-save"];
        tooltip: "Save the pipeline"

        onClicked: {
            console.log("saving pipeline");
            //This is actually defined in main.qml
            //at some point we need some serious cleanup
            saveFileDialog.open()
        }
    }

    Label {

        z: 1
        anchors.top: _self.top;
        anchors.topMargin: G.Style.smallPadding
        anchors.horizontalCenter: _self.horizontalCenter

        height: G.Style.smallLabelHeight

        text: GP.PipelineManager.pipeline.name != "" ? GP.PipelineManager.pipeline.name : "Enter pipeline name..."
        color: GP.PipelineManager.pipeline.name != "" ? G.Style.colors.textColorNeutral : G.Style.colors.textColorDeEmphasize
        font: G.Style.fonts.value

        G.Icon {
            anchors.top: parent.top
            anchors.left: parent.right
            anchors.margins: G.Style.smallPadding;
            visible: _edit_area.containsMouse

            size: G.Style.iconSmall;
            color: GP.PipelineManager.pipeline.name != "" ? G.Style.colors.textColorNeutral : G.Style.colors.textColorDeEmphasize

            icon: G.Icons.icons["pencil"]
        }

        MouseArea {
            id: _edit_area
            anchors.fill: parent
            drag.target: parent
            hoverEnabled: true

            onClicked: {
                _pipeline_info_dialog.open()
            }
        }
    }

    G.PipelineDialog {
        id: _pipeline_info_dialog
    }

    // /////////////////////////////////////////////////////////////////////////////
    // ZOOM AND PAN
    // /////////////////////////////////////////////////////////////////////////////


    MouseArea {

        id: _zoom_and_pan_area

        anchors.fill: _self;
        propagateComposedEvents: true

        onWheel: {
            //We use only significant mouse wheel events to avoid sensitivity issues
            if(wheel.angleDelta.y < 30 && wheel.angleDelta.y > -30) return
            //We only enable 5 zoom levels by default
            if((_internal.zoomLevel === 0 && wheel.angleDelta.y > 0) || _internal.zoomLevel === -5 && wheel.angleDelta.y < 0) return
            //update zoom level
            _internal.zoomLevel = wheel.angleDelta.y > 0 ? Math.min(_internal.zoomLevel + 1, 0) : Math.max(_internal.zoomLevel - 1, -5);

            //This is to compute the pan
            let scaleChange = Math.pow(_internal.factor, _internal.zoomLevel) / _transform.scale
            let dx = (1 - scaleChange) * (wheel.x - _canvas.x);
            let dy = (1 - scaleChange) * (wheel.y - _canvas.y);

            //pan lower bounds
            let lx = _self.windowWidth - _canvas.width * Math.pow(_internal.factor, _internal.zoomLevel);
            let ly = _self.windowHeight - _canvas.height * Math.pow(_internal.factor, _internal.zoomLevel);

            // update scale (zoom factor powered to the current zoom level)
            _transform.scale = Math.pow(_internal.factor, _internal.zoomLevel)

            //update pan
            _canvas.x = Math.max(Math.min(0, _canvas.x + dx), lx);
            _canvas.y = Math.max(Math.min(0, _canvas.y + dy), ly);
        }
    }

    Rectangle {

        id: _canvas;

        x: -_canvas.width / 2 + _self.windowWidth / 2;
        y: -_canvas.height / 2+ _self.windowHeight / 2;
        width: _self.windowWidth * Math.pow(_internal.factor, 5);
        height: _self.windowHeight * Math.pow(_internal.factor, 5) + _self.windowHeight;

        color: G.Style.colors.gutterColor;

        Drag.active: _drag_area.drag.active

        MouseArea {
            id: _drag_area;
            anchors.fill: _canvas;
            drag.target: _canvas;
            drag.maximumX: 0;
            drag.minimumX: _self.windowWidth - _canvas.width * _transform.xScale;
            drag.maximumY: 0;
            drag.minimumY: _self.windowHeight - _canvas.height * _transform.yScale;
        }


        // /////////////////////////////////////////////////////////////////////////////
        // Properties for zoom and pan
        // /////////////////////////////////////////////////////////////////////////////

        transform: Scale {
            id: _transform

            property double scale: 1.0;

            xScale: _transform.scale;
            yScale: _transform.scale;

            Behavior on xScale { PropertyAnimation { duration: _internal.transitionDuration;  easing.type: Easing.InOutCubic } }
            Behavior on yScale { PropertyAnimation { duration: _internal.transitionDuration;  easing.type: Easing.InOutCubic } }

        }

        Behavior on x { PropertyAnimation { duration: _internal.transitionDuration;  easing.type: Easing.InOutCubic } }
        Behavior on y { PropertyAnimation { duration: _internal.transitionDuration;  easing.type: Easing.InOutCubic } }

        Connections {
            target: GP.PipelineManager.pipeline
            function onNodeAdded (node) {

                let n = new G.PipelineLayout.Node(node.name, node)
                _self.addNode(n);

                if (node.inputEdgeCount > 0) {
                    for (var i=0; i<node.inputEdgeCount; i++) {
                        var edge = node.inputEdgeAt(i);
                        var e = _self.addEdge(edge);
                    }
                }

                _self.computeLayout(n.id)
            }
        }

        Connections {
            target: GP.PipelineManager.pipeline
            function onNodeRemoved (node) {
                _self.removeNode(node);
            }
        }

    }

    // /////////////////////////////////////////////////////////////////////////////
    // FUNCTIONS
    // /////////////////////////////////////////////////////////////////////////////

    function addNode(nodeData) {

        const node_component = Qt.createComponent("PipelineNode.qml");

        if (node_component.status == Component.Ready) {

            const node = _internal.layout.graph.addNode(nodeData)


            const n = node_component.createObject(_canvas, {
                node: node,
                color: _getNodeColor(node.data),
                x: Qt.binding(
                    function() {
                        return _internal.originX
                    }
                ),
                y: Qt.binding(
                    function() {
                        return _internal.originY
                    }
                ),
                workspaceIndex: window.current_workspace_index(),
            });

            _internal.nodeComponents[node.id] = n;

            return node;

        } else {
            console.error(node_component.errorString());
        }
    }

    function addEdge(edge) {
        var edge_component = Qt.createComponent("PipelineEdge.qml");

        if (edge_component.status == Component.Ready) {

            //update the status of nodes that now have outgoing edges just in case
            _internal.layout.graph.nodeSet[edge.source.node.name].isSink = false;
            if(_internal.layout.graph.nodeSet[edge.source.node.name].data.inputEdgeCount === 0) {
                _internal.layout.graph.nodeSet[edge.source.node.name].isSource = true;
            }


            const src_node = _internal.layout.graph.nodeSet[edge.source.node.name];
            const src_component = _internal.nodeComponents[src_node.id];
            const src = src_component.outputPorts[edge.source];

            var tgt_node = _internal.layout.graph.nodeSet[edge.target.node.name];
            var tgt_component = _internal.nodeComponents[tgt_node.id];
            var tgt = tgt_component.inputPorts[edge.target];

            var e = edge_component.createObject(_canvas, {
                "edge" : edge,
                "stt": Qt.binding(
                    function() {
                        return Qt.point((src_component.x + src.parent.x + src.x + src.width),
                                        (src_component.y + src.parent.y + src.y + src.height / 2))
                    }
                ),
                "end": Qt.binding(
                    function() {
                        return Qt.point((tgt_component.x + tgt.parent.x + tgt.x),
                                        (tgt_component.y + tgt.parent.y + tgt.y + tgt.height/2))
                    }
                ),
                "inputWorkspaceIndex": src_component.workspaceIndex,
                "outputWorkspaceIndex": tgt_component.workspaceIndex,
            });

            _internal.edgeComponents.push(e);
            return e;
        } else {
            console.error(edge_component.errorString());
        }
    }

    function removeNode(node) {

        let edges_to_remove = []

        for(let edge_index in _internal.edgeComponents) {
            if(node == _internal.edgeComponents[edge_index].edge.target.node) {
                edges_to_remove.push(edge_index)
            }
        }

        for(let i_e in edges_to_remove) {
            let edge_index = edges_to_remove[i_e] - i_e
            _internal.edgeComponents[edge_index].destroy()
            _internal.edgeComponents.splice(edge_index, 1)
        }

        _internal.nodeComponents[node].destroy()
        delete _internal.nodeComponents[node]
    }

    function computeLayout(newNodeId) {

        const currentBoundingBox = _internal.layout.getBoundingBox();

        const offsetX = _internal.originX// - _self.windowWidth / 2
        const offsetY = _internal.originY// - _self.windowHeight / 2

        //in case of drag actions by the user
        _self.initializePositions(newNodeId)

        // use the javascript backend to compute a layout
        _internal.layout.run()

        //upadte the components
        for(let nodeId in _internal.layout.nodePoints) {

            const p = _internal.layout.nodePoints[nodeId].p

            _internal.nodeComponents[nodeId].x = Qt.binding(
                function() {

                    return p.x + offsetX - _internal.nodeComponents[nodeId].width / 2
                }
            )

            _internal.nodeComponents[nodeId].y = Qt.binding(
                function() {
                    return p.y + offsetY - _internal.nodeComponents[nodeId].height / 2
                }
            )
        }

    }

    //Dynamic object creation in qml does not let us use onXChanged and onYChanged properly
    //so I have to do this to check that the current internal positions are correct (in
    //case the user has done some drags)
    function initializePositions(newNodeId) {
        for(let nodeId in _internal.nodeComponents) {
            if(nodeId !== newNodeId) {
                _internal.layout.nodePoints[nodeId].p.x = _internal.nodeComponents[nodeId].x -_internal.originX + _internal.nodeComponents[nodeId].width / 2
                _internal.layout.nodePoints[nodeId].p.y = _internal.nodeComponents[nodeId].y -_internal.originY + _internal.nodeComponents[nodeId].height / 2
            }
        }
    }

    function _getNodeColor(node) {
        if (node.type == GP.PipelineNode.NODE_READER)
            return G.Style.colors.lightGreen
        if (node.type == GP.PipelineNode.NODE_WRITER)
            return G.Style.colors.lightOrange
        if (node.type == GP.PipelineNode.NODE_FILTER)
            return G.Style.colors.warningColor
        if (node.type == GP.PipelineNode.NODE_CONVERTER)
            return G.Style.colors.dangerColor
        if (node.type == GP.PipelineNode.NODE_ADAPTER)
            return G.Style.colors.lightRed
        if (node.type == GP.PipelineNode.NODE_CONSTRUCTOR)
            return G.Style.colors.okColor
        if (node.type == GP.PipelineNode.NODE_TASK)
            return G.Style.colors.lightBlue
        if (node.type == GP.PipelineNode.NODE_MORPHONET)
            return G.Style.colors.baseColor
        return G.Style.colors.neutralColor
    }

    // /////////////////////////////////////////////////////////////////////////////
    // INTERNAL
    // /////////////////////////////////////////////////////////////////////////////

    QtObject {
        id: _internal;

        // /////////////////////////////////////////////////////////////////////////////
        // Internal variables kept for zooming and panning
        // /////////////////////////////////////////////////////////////////////////////
        readonly property real transitionDuration: 0;
        property double factor: 1.25
        property int zoomLevel: 0;
        property double originX: (_self.windowWidth / 2 - _canvas.x) * Math.pow(_internal.factor, - _internal.zoomLevel);
        property double originY: (_self.windowHeight / 2 - _canvas.y) * Math.pow(_internal.factor, - _internal.zoomLevel);

        // /////////////////////////////////////////////////////////////////////////////
        // Internal variables kept for the graph layout calculation
        // /////////////////////////////////////////////////////////////////////////////
        property var layout: new G.PipelineLayout.Layout.ForceDirected(new G.PipelineLayout.Graph(), 10, 6, 0.2)
        property var nodeComponents: new Object()
        property var edgeComponents: []

    }

}
