#version 330 core

layout ( location = 4 ) in vec3 vertex_position;
layout ( location = 5 ) in vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float particleSize;

out float lifetime;

void main()
{
   vec4 position_viewspace = view * model * vec4( position.xyz , 1 );
   position_viewspace.xy += particleSize * (vertex_position.xy - vec2(0.5));
   gl_Position = projection * position_viewspace;
   lifetime = position.w;
}