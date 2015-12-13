precision mediump float;

attribute vec3 vert;
attribute vec2 wq;
attribute vec3 color;
attribute vec2 position;

uniform float conerad;

varying vec3 vcolor2;

void main() {
	vec3 defcol=vec3(0.0,0.0,0.0);
    vcolor2 =  mix(color,defcol, vert.z*vert.z);
    vec3 scaled_vert=vec3(vert.x*conerad,vert.y*conerad,vert.z*wq.x-wq.y); 
    vec3 offset = vec3(position * 2.0 - 1.0, 0.0);
    gl_Position = vec4(scaled_vert + offset, 1.0);
}
