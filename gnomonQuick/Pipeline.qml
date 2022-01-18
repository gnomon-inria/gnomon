import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import gnomon.Pipeline 1.0 as G

import gnomonQuick     1.0 as GX

// /////////////////////////////////////////////////////////////////////////////
// TODO: Use actual resolution as propertues bound to the shader
// /////////////////////////////////////////////////////////////////////////////

Control {
    id: _self;
    clip: true;

    Item {
        id: _internal;

        property double factor: 1.25
        property int zoomLevel: 0;
        property double originX: _canvas.width / 2;

    }

    MouseArea {
        id: _mouse_area
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
            /* let dx = (1 - scaleChange) * wheel.x * _transform.scale; */
            /* let dy = (1 - scaleChange) * wheel.y * _transform.scale; */
            //pan lower bounds
            let lx = _self.width - _canvas.width * Math.pow(_internal.factor, _internal.zoomLevel);
            let ly = _self.height - _canvas.height * Math.pow(_internal.factor, _internal.zoomLevel);

            console.log("SCALE CHANGE, dx, dy", scaleChange, dx, dy)
            console.log("X, X + dx, lx", _canvas.x, _canvas.x + dx, lx)

            // update scale (zoom factor powered to the current zoom level)
            _transform.scale = Math.pow(_internal.factor, _internal.zoomLevel)

            //update pan
            _canvas.x = dx < 0 ? Math.max(_canvas.x + dx, lx) : Math.min(0, _canvas.x + dx);
            _canvas.y = dy < 0 ? Math.max(_canvas.y + dy, ly) : Math.min(0, _canvas.y + dy);

        }
    }

    Rectangle {

        id: _canvas;
        width: _self.width * Math.pow(_internal.factor, 5);
        height: _self.height * Math.pow(_internal.factor, 5);
        x: -_canvas.width / 2 + _self.width / 2;
        y: -_canvas.height / 2+ _self.height / 2;

        layer.enabled: true
        layer.samples: 4

        readonly property real transitionDuration: 200;


        ShaderEffect {

            anchors.fill: _canvas;

            property real u_time;

            fragmentShader: "#version 330
uniform lowp float qt_Opacity;
uniform float u_time;

in highp vec2 qt_TexCoord0;
in highp vec4 gl_FragCoord;

out highp vec4 fragColor;

vec2 u_resolution = vec2(3840/5, 2160/5);

float random(in float x){ return fract(sin(x)*43758.5453); }
float random(in vec2 st){ return fract(sin(dot(st.xy ,vec2(12.9898,78.233))) * 43758.5453); }

float grid(vec2 st, float res){
    vec2 grid = fract(st*res);
    return 1.0-(step(res,grid.x) * step(res,grid.y));
}

float box(in vec2 st, in vec2 size){
    size = vec2(0.5) - size*0.5;
    vec2 uv = smoothstep(size,
                        size+vec2(0.001),
                        st);
    uv *= smoothstep(size,
                    size+vec2(0.001),
                    vec2(1.0)-st);
    return uv.x*uv.y;
}

float cross(in vec2 st, vec2 size){
    return  clamp(box(st, vec2(size.x*0.5,size.y*0.125)) +
            box(st, vec2(size.y*0.125,size.x*0.5)),0.,1.);
}

void main() {


    vec2 st = gl_FragCoord.st/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.0);

    // Grid
    vec2 grid_st = st*300.;
    color += vec3(0.5,0.,0.)*grid(grid_st,0.01);
    color += vec3(0.2,0.,0.)*grid(grid_st,0.02);
    color += vec3(0.2)*grid(grid_st,0.1);

    // Crosses
    vec2 crosses_st = st + .5;
    crosses_st *= 3.;
    vec2 crosses_st_f = fract(crosses_st);
    color *= 1.-cross(crosses_st_f,vec2(.3,.3));
    color += vec3(.9)*cross(crosses_st_f,vec2(.2,.2));

    // Digits
    vec2 blocks_st = floor(st*6.);
    float t = u_time*.8+random(blocks_st);
    float time_i = floor(t);
    float time_f = fract(t);
    color.rgb += step(0.9,random(blocks_st+time_i))*(1.0-time_f) * 0.2;

    // Darken
    color *= 0.2;

    fragColor = vec4( color , 1.0);
}
"
            NumberAnimation on u_time { running: true; from: 0; to: 1000; loops: Animation.Infinite; duration: 1000000; }
        }

        /* ***************************************************************************
           ;;
           ;; ****************************************************************************/

        Drag.active: _drag_area.drag.active

        MouseArea {
            id: _drag_area;
            anchors.fill: _canvas;
            drag.target: _canvas;
            drag.maximumX: 0;
            drag.minimumX: _self.width - _canvas.width * _transform.xScale;
            drag.maximumY: 0;
            drag.minimumY: _self.height - _canvas.height * _transform.yScale;
        }


/* ***************************************************************************
;; Properties for zoom and pan
;; ****************************************************************************/

        transform: Scale {
            id: _transform

            property double scale: 1.0;

            xScale: _transform.scale;
            yScale: _transform.scale;
            Behavior on xScale { PropertyAnimation { duration: _canvas.transitionDuration;  easing.type: Easing.InOutCubic } }
            Behavior on yScale { PropertyAnimation { duration: _canvas.transitionDuration;  easing.type: Easing.InOutCubic } }
        }

        Behavior on x { PropertyAnimation { duration: _canvas.transitionDuration;  easing.type: Easing.InOutCubic } }
        Behavior on y { PropertyAnimation { duration: _canvas.transitionDuration;  easing.type: Easing.InOutCubic } }

        // /////////////////////////////////////////////////////////////////////////////
        // Nodes
        // /////////////////////////////////////////////////////////////////////////////

        property var nodes: new Object();
        property var edges: [];

        Connections {
            target: G.Pipeline
            function onNodeAdded (node) {
                console.log(node.name, "(", node.algorithmClass, ")", G.Pipeline.nodeNames);
                var n = _canvas.addNode(node);

                console.log(node.inputEdgeCount, "input edges")
                if (node.inputEdgeCount > 0) {
                    for (var i=0; i<node.inputEdgeCount; i++) {
                        var edge = node.inputEdgeAt(i);
                        console.log(" --> edge", i, ":",
                                    edge.source.node.name, "(", edge.source.label, ")",
                                    "->",
                                    edge.target.node.name, "(", edge.target.label,")")
                        var e = _canvas.addEdge(edge);
                    }
                }
            }
        }

        function addNode(node) {
            var node_component = Qt.createComponent("PipelineNode.qml");
            if (node_component.status == Component.Ready) {
                var n = node_component.createObject(_canvas, {
                    "algorithmClass": node.algorithmClass,
                    "algorithmPlugin": node.algorithmPlugin,
                    "inputPortsNames": node.inputPortsNames,
                    "outputPortsNames": node.outputPortsNames,
                    "color": node.color,
                    "x": Qt.binding(function() { return _canvas.width/2 + node.position.x }),
                    "y": Qt.binding(function() { return _canvas.height/2 + node.position.y }),
                    "workspaceIndex": window.current_workspace_index()
                });
                nodes[node.name] = n;
                console.log("Adding node...", n)
                return n;
            } else {
                console.error(node_component.errorString());
            }
        }

        function addEdge(edge) {
            var edge_component = Qt.createComponent("PipelineEdge.qml");
            if (edge_component.status == Component.Ready) {
                var src_node = nodes[edge.source.node.name];
                var src = src_node.outputPorts[edge.source.label];
                var tgt_node = nodes[edge.target.node.name];
                var tgt = tgt_node.inputPorts[edge.target.label]

                var e = edge_component.createObject(_canvas, {
                    //"stt": Qt.binding(function() { return src.mapToItem(_canvas, Qt.point(src.width, src.height/2)) }),
                    "stt": Qt.binding(function() { return Qt.point((src_node.x + src.parent.x + src.x + src.width),
                                                                   (src_node.y + src.parent.y + src.y + src.height/2)) }),
                    //"end": Qt.binding(function() { return tgt.mapToItem(_canvas, Qt.point(0, tgt.height/2)) }),
                    "end": Qt.binding(function() { return Qt.point((tgt_node.x + tgt.parent.x + tgt.x),
                                                                   (tgt_node.y + tgt.parent.y + tgt.y + tgt.height/2)) }),
                    "inputWorkspaceIndex": src_node.workspaceIndex,
                    "outputWorkspaceIndex": tgt_node.workspaceIndex,
                });

                edges.push(e);
                console.log("Adding edge...", e)
                return e;
            } else {
                console.error(edge_component.errorString());
            }
        }

        /*GX.PipelineNode { id: _source;
          algorithmClass: "source";
          algorithmPlugin: "dummySource";
          outputPortsNames: ["output1", "output2"];

          x:300
          y:100

          Component.onCompleted: {
          console.log(_source.outputPorts);
          }
          }

          GX.PipelineNode { id: _target;
          algorithmClass: "target";
          algorithmPlugin: "dummyTarget";
          inputPortsNames: ["input1", "input2"];

          x:600
          y:100

          Component.onCompleted: {
          console.log(_target.inputPorts);
          }
          }

          GX.PipelineEdge { id: _edge;
          src: _source.outputPorts["output2"];
          tgt: _target.inputPorts["input1"];
          }*/
    }

}
