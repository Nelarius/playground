#version 330 core

uniform float color_r;
uniform float color_g;
uniform float color_b;

out vec4 color;

void main() {
    color = vec4( color_r, color_g, color_b, 1.0 );
}

