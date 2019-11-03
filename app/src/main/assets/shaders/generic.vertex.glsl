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
    gl_Position = perspective * view * model * vec4(aPos, 1.0);
    // Stall it from being optimized away
    vColor = aColor;
    vUV = aUV;
}
