#version 100
precision mediump float;

attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

varying vec3 normal;
varying vec2 texCoord;


void main() {
    gl_Position = perspective * view * model * vec4(aPos, 1.0);
    normal = aNormal;
    texCoord = aTexCoord;
}
