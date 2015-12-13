precision mediump float;

attribute vec3 vert;
uniform vec3 m_color;
uniform vec2 position;
uniform vec2 delta_x;
uniform vec2 delta_y;
uniform float conerad;
uniform vec2 wq;

varying vec3 vcolor1;

void main() {
    vec3 defcol = vec3(0.0, 0.0, 0.0);
    vcolor1 = mix(m_color, defcol, vert.z);
    
    vec3 offset = vec3(position * 2.0 - 1.0, 0.0);
    
    vec2 x_vec = delta_x * vert.x;
    vec2 y_vec = delta_y * vert.y;
    vec3 newvert = vec3(x_vec + y_vec, vert.z * wq.x - wq.y);  

    gl_Position = vec4(newvert + offset, 1.0);
}
