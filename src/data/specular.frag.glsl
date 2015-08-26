#version 330 core

in vec3 fragPos;
in vec3 fragNorm;

uniform vec3 cameraPosition;

uniform float shininess;
uniform float ambientColor_r;
uniform float ambientColor_g;
uniform float ambientColor_b;
uniform float specColor_r;
uniform float specColor_g;
uniform float specColor_b;

uniform struct DirectionalLight {
    vec3 direction;
    vec3 intensities;
    float ambientCoefficient;
} light;

// there needs to be an array of these lights, as well as a light count.

out vec4 color;

void main() {
    vec3 lightPosition = vec3( 10.0, 0.0, 10.0 );
    vec3 lightDirection = normalize( vec3( 1.0, 1.0, 0.0 ) );
    vec3 lightIntensity = vec3( 1.0, 1.0, 1.0 );
    
    vec3 specularColor = vec3( specColor_r, specColor_g, specColor_b );
    
    vec3 surfToCamera = normalize( cameraPosition - fragPos );
    
    //ambient
    vec3 ambient = vec3( ambientColor_r, ambientColor_g, ambientColor_b );

    float diffuseCoefficient = max( 0.0, dot( fragNorm, lightDirection ) );
    vec3 diffuse = diffuseCoefficient * ambient * lightIntensity;
    
    float specularCoefficient = 0.0;
    if ( diffuseCoefficient > 0.0 ) {
        specularCoefficient = pow( max( 0.0, dot( surfToCamera, reflect( -lightDirection, fragNorm ) ) ), shininess );
    }
    vec3 specular = specularCoefficient * specularColor;
    
    color = vec4( ambient + diffuse + specular, 0.0 );
}
