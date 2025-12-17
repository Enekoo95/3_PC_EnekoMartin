#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform vec4 waterColor; // ej: vec4(0.0, 0.5, 1.0, 0.5)

void main() {
    FragColor = waterColor;
}
