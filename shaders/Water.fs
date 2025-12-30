#version 330 core
out vec4 FragColor;

in vec2 uv;
uniform sampler2D waterTexture;

void main() {
    vec4 color = texture(waterTexture, uv);
    color.rgb = mix(color.rgb, vec3(0.0, 0.4, 0.6), 0.4);
    color.a = 0.6;
    FragColor = color;
}
