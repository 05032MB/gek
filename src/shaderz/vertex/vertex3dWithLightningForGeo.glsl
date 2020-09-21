#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 colors;

out VERTEX_CHEAT_BUFFER {
    vec2 texCoord;
    vec3 objColor;
    vec3 worldPos;
    vec3 objNormals;
} ss;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = vec4(pos, 1.0);
    ss.texCoord = inTexCoord;
    ss.objColor = colors;
    ss.objNormals = mat3(transpose(inverse(model))) * normals; //konwersja z przestrzenii świata
    ss.worldPos = vec3(model * vec4(pos, 1.0));
}