#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform vec4 color;
uniform sampler2D texSampler;
uniform bool usesTexture;

void main()
{
    vec4 color2 = color;
    if(usesTexture)
    {
       color2 = color2 * texture(texSampler, texCoord);
    }
    fragColor = color2;
}