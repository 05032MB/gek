#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(pos, 1.0);
    texCoord = inTexCoord;
}