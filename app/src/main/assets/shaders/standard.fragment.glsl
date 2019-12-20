#version 100
precision mediump float;

varying vec3 normal;
varying vec2 texCoord;

uniform bool texturize;
uniform sampler2D ambientTexture;
// uniform sampler2D diffuseTexture;
// uniform sampler2D specularTexture;


void main() {
    // please don't optimze me...
    vec3 bNormal = normal;
    vec2 bTexCoord = texCoord;
    if (texturize) {
//        color = vec4(bTexCoord, 0.0, 1.0);
        gl_FragColor = texture2D(ambientTexture, bTexCoord);
    } else {
        gl_FragColor = vec4(bNormal, 1.0);
    }
}
