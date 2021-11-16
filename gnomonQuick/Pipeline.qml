import QtQuick 2.15
import QtQuick.Shapes 1.15

import gnomon.Pipeline 1.0 as G

import gnomonQuick     1.0 as GX

// /////////////////////////////////////////////////////////////////////////////
// TODO: Use actual resolution as propertues bound to the shader
// /////////////////////////////////////////////////////////////////////////////

Rectangle {

    id: self;

    layer.enabled: true
    layer.samples: 4

    ShaderEffect {

        anchors.fill: parent;

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

// /////////////////////////////////////////////////////////////////////////////
// Nodes
// /////////////////////////////////////////////////////////////////////////////

    property var nodes: new Object();
    property var edges: [];

    Connections {
        target: G.Pipeline
        function onNodeAdded (node) {
            console.log(node.name, "(", node.algorithmClass, ")", G.Pipeline.nodeNames);
            var n = self.addNode(node);

            console.log(node.inputEdgeCount, "input edges")
            if (node.inputEdgeCount > 0) {
                for (var i=0; i<node.inputEdgeCount; i++) {
                    var edge = node.inputEdgeAt(i);
                    console.log(" --> edge", i, ":",
                                edge.source.node.name, "(", edge.source.label, ")",
                                "->",
                                edge.target.node.name, "(", edge.target.label,")")
                    var e = self.addEdge(edge);
                }
            }
        }
    }

    function addNode(node) {
        var node_component = Qt.createComponent("PipelineNode.qml");
        if (node_component.status == Component.Ready) {
            var n = node_component.createObject(self, {
                "algorithmClass": node.algorithmClass,
                "algorithmPlugin": node.algorithmPlugin,
                "inputPortsNames": node.inputPortsNames,
                "outputPortsNames": node.outputPortsNames,
                "color": node.color,
                "x": Qt.binding(function() { return self.width/2 + node.position.x }),
                "y": Qt.binding(function() { return self.height/2 + node.position.y })
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

            var e = edge_component.createObject(self, {
                //"stt": Qt.binding(function() { return src.mapToItem(self, Qt.point(src.width, src.height/2)) }),
                "stt": Qt.binding(function() { return Qt.point((src_node.x + src.parent.x + src.x + src.width),
                                                               (src_node.y + src.parent.y + src.y + src.height/2)) }),
                //"end": Qt.binding(function() { return tgt.mapToItem(self, Qt.point(0, tgt.height/2)) }),
                "end": Qt.binding(function() { return Qt.point((tgt_node.x + tgt.parent.x + tgt.x),
                                                               (tgt_node.y + tgt.parent.y + tgt.y + tgt.height/2)) }),
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
