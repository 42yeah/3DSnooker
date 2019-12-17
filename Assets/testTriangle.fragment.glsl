#version 330 core

in vec3 normal;
in vec2 texCoord;

uniform bool texturize;
uniform sampler2D ambientTexture;
// uniform sampler2D diffuseTexture;
// uniform sampler2D specularTexture;

out vec4 color;


void main() {
    // please don't optimze me...
    vec3 bNormal = normal;
    vec2 bTexCoord = texCoord;
    if (texturize) {
//        color = vec4(bTexCoord, 0.0, 1.0);
        color = texture(ambientTexture, bTexCoord);
    } else {
        color = vec4(bNormal, 1.0);
    }
}
