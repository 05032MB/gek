#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 transform;
uniform vec3 off;
uniform float scale;

void main()
{
    gl_Position = (transform * vec4(pos * abs(scale), 1.0)) + vec4(off, 0);
}