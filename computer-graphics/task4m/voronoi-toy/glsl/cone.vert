precision mediump float;

attribute vec3 vert;
uniform vec2 position;

void main() {
    vec3 offset = vec3(position * 2.0 - 1.0, 0.0);
    gl_Position = vec4(vert + offset, 1.0);
}
