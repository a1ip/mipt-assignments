precision mediump float;
attribute vec2 position;

void main() {
    gl_Position = vec4(position * 2.0 - 1.0, -0.97, 1.0);
    gl_PointSize = 20.0;
}
