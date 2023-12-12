#version 410 core

in vec3 colour;
in vec2 passTexture;

out vec4 fragmentColour;

//unitate de textura
uniform sampler2D diffuseTexture;

void main() {
    fragmentColour = texture(diffuseTexture, passTexture);
}