#version 400

//primeste variabila de culoare de la vertex shader
//in vec3 colour;

//primeste culoarea in mod dinamic, de la programul principal
uniform vec3 uniformColour;

out vec4 frag_color;

void main() {
      frag_color = vec4(uniformColour, 1.0);
}
