#version 330 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;

out vec4 color;

void main() {
    vec3 lightPosition = vec3( 10.0, 0.0, 10.0 );
    vec3 lightDirection = normalize( vec3( 1.0, 1.0, 0.0 ) );
    vec3 lightIntensity = vec3( 1.0, 1.0, 1.0 );
    
    //ambient
    vec3 ambient = vec3( 0.7, 0.4, 0.4 );

    float diffuseCoefficient = max( 0.0, dot( fragmentNormal, lightDirection ) );
    vec3 diffuse = diffuseCoefficient * ambient * lightIntensity; // instead of ambient, there should be surface color

    // calculate the final pixel color, based on 
    // brightness,
    // the color/intensity of the light
    // the ambient color
    color = vec4( diffuse, 1.0 );
    //color = vec4( brightness * lightIntensity * ambientColor.rgb, ambientColor.a );
}


