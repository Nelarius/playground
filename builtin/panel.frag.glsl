#version 330

uniform sampler2D Texture;

in vec2 fragUv;
in vec4 fragColor;

out vec4 finalColor;

void main() {
   finalColor = fragColor * texture( Texture, fragUv.st );
}