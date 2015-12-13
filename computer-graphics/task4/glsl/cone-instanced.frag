precision mediump float;

varying vec3 vcolor2;
uniform float alpha;

void main() {
    gl_FragColor = vec4(vcolor2,alpha);
}
