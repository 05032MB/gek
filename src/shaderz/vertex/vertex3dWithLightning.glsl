#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 colors;

out vec2 texCoord;
out vec3 objColor;
out vec3 worldPos;
out vec3 objNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(pos, 1.0);
    texCoord = inTexCoord;
    objColor = colors;
    objNormals = mat3(transpose(inverse(model))) * normals; //konwersja z przestrzenii świata
    worldPos = vec3(model * vec4(pos, 1.0));
}