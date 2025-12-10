#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D ourTexture; // texturas
uniform sampler2D secondTexture;
uniform vec3 lightDir;

void main()
{
    vec3 N = normalize(Normal);
    float slope = 1.0 - N.y; // pendiente
    vec4 grass = texture(ourTexture, UV);
    vec4 rock  = texture(secondTexture, UV);

    vec4 color = mix(grass, rock, slope);

    float diff = max(dot(N, normalize(lightDir)),0.0);
    color.rgb *= diff + 0.3;
    FragColor = grass;

}
