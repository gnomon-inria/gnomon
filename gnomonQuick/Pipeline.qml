import QtQuick 2.15
import QtQuick.Shapes 1.15

// /////////////////////////////////////////////////////////////////////////////
// TODO: Use actual resolution as propertues bound to the shader
// /////////////////////////////////////////////////////////////////////////////

Rectangle {

    id: root;

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
// Nodes                                           - TODO: Turn into a component
// /////////////////////////////////////////////////////////////////////////////

    Rectangle {

        id: _source;

        width: 100;
        height: 48;
        radius: 6;
        color: "#E4A065";

        Drag.active: dragArea_1.drag.active

        MouseArea {
            id: dragArea_1
            anchors.fill: parent

            drag.target: parent
        }
    }

    Rectangle {

        id: _destination;

        width: 100;
        height: 48;
        radius: 6;
        color: "#E49034";

        Drag.active: dragArea_2.drag.active

        MouseArea {
            id: dragArea_2
            anchors.fill: parent

            drag.target: parent
        }
    }

    Shape {

        id: _e;

        property Item src: _source;
        property Item dst: _destination;

        property point stt: Qt.point((src.x + src.width), (src.y + src.height/2));
        property point end: Qt.point( dst.x,              (dst.y + dst.height/2));
        property point mid: Qt.point((stt.x + end.x)/2,   (stt.y + end.y)/2);

        property real delt: (mid.x - stt.x)/2;

         width: end.x - stt.x;
        height: end.y - stt.y;

        ShapePath {
            fillColor: "transparent";

            strokeWidth: 3
            strokeColor: "red"

            startX: _e.stt.x;
            startY: _e.stt.y;

            PathCubic {
                control1X: _e.end.x-_e.delt; control1Y: _e.stt.y;
                control2X: _e.stt.x+_e.delt; control2Y: _e.end.y;
                        x: _e.end.x;                 y: _e.end.y
            }
        }
    }
}
