#version 410 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColour;

//the model transformation matrix
uniform mat4 model;
//the view transformation matrix
uniform mat4 view;
//the projection matrix
uniform mat4 projection;

out vec3 color;

void main() {
    color = vertexColour;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}

