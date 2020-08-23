#version 330 core

in vec2 texCoord;
in vec3 objColor;
in vec3 objNormals;
in vec3 worldPos;

out vec4 fragColor;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform bool hasSpecularTex;

uniform vec3 lightColor;
uniform float ambientLight;
uniform vec3 staticLightPos;
uniform vec3 actCameraPos;
uniform float specularLight;
uniform float shininess;

float EPS = 1e-10;

void main()
{
    ///ambient
    float ambientLightStrength = ambientLight > 0 + EPS ? ambientLight : 0.1;
    vec3 ambientL = ambientLightStrength * lightColor * texture(diffuseTex, texCoord).rgb;
    //////////
   
    //znormalizowane normale i kierunek padania światła
    vec3 normobjnorm = normalize(objNormals);
    vec3 lightDir = normalize(staticLightPos - worldPos);
    //diffuse
    float _diffuse = max(dot(normobjnorm, lightDir), 0);
    vec3 diffuse = _diffuse * lightColor * texture(diffuseTex, texCoord).rgb;
    //////////

    //specular
    float specularLightStrength = specularLight > 0 + EPS ? specularLight : 0.7;

    vec3 viewDir = normalize(actCameraPos - worldPos); //skąd patrzę
    vec3 reflectDir = reflect(-lightDir, normobjnorm);  //odbicie światła

    float _specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess > 0 ? shininess : 32);
    vec3 specular = specularLightStrength * _specular * lightColor;
    if(hasSpecularTex)
    {
        specular *= texture(specularTex, texCoord).rgb;
    }
    //////////

    fragColor = vec4(objColor * (ambientL + diffuse + specular), 1.0);
}