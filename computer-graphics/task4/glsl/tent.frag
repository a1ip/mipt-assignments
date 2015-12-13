precision mediump float;

varying vec3 vcolor1;

uniform float alpha;

void main() {
    gl_FragColor = vec4(vcolor1, alpha);
}
