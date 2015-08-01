#version 330 core

uniform mat4 camera;
uniform mat4 model;

in vec3 vertex;
in vec3 normal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;

void main() {
    fragmentPosition = vec3( model * vec4( vertex, 1.0 ) );
    mat3 normalMat = transpose( inverse( mat3( model ) ) );
    fragmentNormal = normalize( normalMat * normal );
    
    //apply all matrix transformations
    gl_Position = camera * model * vec4( vertex, 1.0 );
}


