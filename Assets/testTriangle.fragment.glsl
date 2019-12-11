#version 330 core

in vec3 normal;
in vec2 texCoord;

out vec4 color;


void main() {
    // please don't optimze me...
    vec3 bNormal = normal;
    vec2 bTexCoord = texCoord;
    color = vec4(bNormal, 1.0);
}
