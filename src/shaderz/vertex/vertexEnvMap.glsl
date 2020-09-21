#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoord = inTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
}