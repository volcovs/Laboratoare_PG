#version 400

layout(location = 0) in vec3 vertex_position;
//out vec3 colour;

void main() {
    //colour = vec3(0.74, 0.16, 0.0);
    gl_Position = vec4(vertex_position, 1.0);
}
