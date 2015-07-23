#version 330 core

uniform mat4 camera;
uniform mat4 model;

in vec3 vertex;

void main() {    
    //apply all matrix transformations
    gl_Position = camera * model * vec4( vertex, 1.0 );
}

