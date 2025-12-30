#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec2 uv;

void main() {
    vec3 pos = aPos;
    pos.y += sin(pos.x * 0.1 + time) * 0.3;
    pos.y += cos(pos.z * 0.1 + time) * 0.3;

    uv = aUV + vec2(time * 0.02, time * 0.01);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
