#version 330 core

in vec2 uv;
in vec3 vertColor;

out vec4 FragColor;

uniform bool useTexture;
uniform bool useSecondTexture;   // NUEVO: controla si se mezcla la segunda textura
uniform bool useUniformColor;

uniform vec3 uniformColor;
uniform sampler2D ourTexture;
uniform sampler2D secondTexture; // NUEVO: segunda textura

void main()
{
    if(useTexture)
    {
        if(useSecondTexture)
        {
            FragColor = texture(ourTexture, uv) * 0.5 + texture(secondTexture, uv) * 0.5;
        }
        else
        {
            FragColor = texture(ourTexture, uv);
        }
    }
    else if(useUniformColor)
    {
        FragColor = vec4(uniformColor, 1.0);
    }
    else
    {
        FragColor = vec4(vertColor, 1.0);
    }
}
