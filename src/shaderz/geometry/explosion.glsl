#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float expltime;

in VERTEX_CHEAT_BUFFER {
    vec2 texCoord;
    vec3 objColor;
    vec3 worldPos;
    vec3 objNormals;
} ss[];

out vec2 texCoord;
out vec3 objColor;
out vec3 worldPos;
out vec3 objNormals;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 70.0f;
    vec3 direction = normal * (magnitude * (1 - exp(-0.3f * expltime)) ); //wypłaszczona krzywa po czasie
    //vec3 direction = normal * expltime * magnitude; //alternatywa - liniowy rozpad, brak spowolnienia
    return position + vec4(direction, 0.0);
}

vec3 getPlaneNormal()
{
    vec3 a = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    return normalize(cross(a,b));
}

void passRest(int n)
{
    texCoord = ss[n].texCoord;
    objColor = ss[n].objColor;
    worldPos = ss[n].worldPos;
    objNormals = ss[n].objNormals;
}

void main() {    
    for(int i = 0; i < 3; i++)
    {
        if(expltime > 0)gl_Position = explode(gl_in[i].gl_Position, getPlaneNormal());
        else gl_Position = gl_in[i].gl_Position;
        passRest(i);
        EmitVertex();
    }
    EndPrimitive();
}