#version 330 core

in float lifetime;
out vec4 fragColor;

void main()
{
   fragColor = vec4( 0.281f + lifetime/3.5f, 0.238f, 0.543f, 0.5f );
}