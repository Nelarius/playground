#version 330

uniform mat4 MOrtho;

in vec2 position;
in vec2 uv;
in vec4 color;

out vec2 fragUv;
out vec4 fragColor;

void main() {
    fragUv = uv;
    fragColor = color;
    gl_Position = MOrtho * vec4( position.xy, 0.0, 1.0 );
}