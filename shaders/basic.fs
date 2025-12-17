#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D ourTexture;     // grass
uniform sampler2D secondTexture;  // rock
uniform vec3 lightDir;

void main()
{
    vec3 N = normalize(Normal);

    // pendiente exagerada
    float slope = 1.0 - N.y;
    slope = pow(slope, 2.2);
    slope = smoothstep(0.2, 0.6, slope);

    vec4 grass = texture(ourTexture,  UV * 0.05);
    vec4 rock  = texture(secondTexture, UV * 0.05);

    vec4 color = mix(grass, rock, slope);

    float diff = max(dot(N, normalize(-lightDir)), 0.0);
    color.rgb *= diff + 0.25;

    FragColor = color;
}
