// === ATTRIBUTES === //
attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aUV;

// === UNIFORMS === //
uniform mat4 view, perspective, model;

// === VARYING === //
varying vec3 vColor;
varying vec2 vUV;


void main() {
    mat4 mView = view;
    mView[0][0] = 1.0;
    mView[0][1] = 0.0;
    mView[0][2] = 0.0;
    mView[1][0] = 0.0;
    mView[1][1] = 1.0;
    mView[2][2] = 0.0;
    mView[2][0] = 0.0;
    mView[2][1] = 0.0;
    mView[2][2] = 1.0;
    gl_Position = perspective * mView * model * vec4(aPos, 1.0);
    // Stall it from being optimized away
    vColor = aColor;
    vUV = aUV;
}
