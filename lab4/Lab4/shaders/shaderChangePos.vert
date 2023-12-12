#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform vec3 position;

out vec3 colour;

void main() {
    colour = vertex_colour;
    gl_Position = vec4(vertex_position + position, 1.0);
}
