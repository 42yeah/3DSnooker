// === ATTRIBUTES === //
attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aUV;

// === UNIFORMS === //
uniform mat4 view, perspective, model;
uniform vec3 center;

// === VARYING === //
varying vec3 vColor;
varying vec2 vUV;


void main() {
    vec3 pos = aPos + center;
    mat4 mView = view;
    gl_Position = perspective * mView * model * vec4(pos, 1.0);
    // Stall it from being optimized away
    vColor = aColor;
    vUV = aUV;
}
