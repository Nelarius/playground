#version 330 core

in vec3 fragPos;
in vec3 fragNorm;

uniform vec3 cameraPosition;

uniform struct PointLight {
    vec3 position;
    vec3 intensity;
    float attenuation;
    float ambientCoefficient;
} pointLight;

uniform float shininess;
uniform vec3 base;
uniform vec3 ambient;
uniform vec3 specularColor;

// there needs to be an array of these lights, as well as a light count.

out vec4 color;

void main() {
    vec3 eye = normalize( cameraPosition - fragPos );
    vec3 lightDir = pointLight.position - fragPos;
    float lightDist = length(lightDir);
    // direction should point to the light
    // it should be normalized
    // now the dot product gives positive cosines
    lightDir = lightDir / lightDist;

    float attenuation = 1.0 / (
        1.0 +
        pointLight.attenuation * lightDist * lightDist
    );

    float diffuse = max( 0.0, dot(lightDir, fragNorm) );
    float specular = max( 0.0, dot(reflect(lightDir, fragNorm), eye) );

    if ( diffuse == 0.0 ) {
        specular = 0.0;
    } else {
        specular = pow(specular, shininess);
    }

    vec3 scatteredLight = ambient * pointLight.ambientCoefficient + diffuse * pointLight.intensity * attenuation;
    vec3 reflectedLight = specular*pointLight.intensity*specularColor*attenuation;
    vec3 rgb = min( base + scatteredLight + reflectedLight, vec3(1.0) );

    color = vec4( rgb, 1.0 );
}
