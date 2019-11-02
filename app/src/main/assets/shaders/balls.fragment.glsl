precision mediump float;

// === VARYING === //
varying vec3 vColor;
varying vec2 vUV;


void main() {
    float dist = distance(vUV, vec2(0.5, 0.5));
    if (dist >= 0.5) {
        discard;
    }
    gl_FragColor = vec4(vColor, 1.0);
}
