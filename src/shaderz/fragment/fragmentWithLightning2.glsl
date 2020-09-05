#version 330 core

in vec2 texCoord;
in vec3 objColor;
in vec3 objNormals;
in vec3 worldPos;

out vec4 fragColor;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform bool hasSpecularTex;
uniform bool usesFlashlight;

uniform vec3 lightColor;
uniform float ambientLight;
uniform vec3 staticLightPos;
uniform vec3 actCameraPos;
uniform float specularLight;
uniform float shininess;

float EPS = 1e-10;

struct SpotLight
{
    vec3 pos, dir;

    float innerCutOff, outerCutOff;

    float linear, quadratic, constant;

    //kolory składowych oświetlenia
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

uniform SpotLight flashlight;

vec3 CalcFlashlight(SpotLight light, vec3 objNormal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos - worldPos);
    
    //diffuse
    float _diffuse = max(dot(objNormal, lightDir), 0);
    
    //specular
    vec3 reflectDir = reflect(-lightDir, objNormal);
    float _specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess > 0 ? shininess : 32);
    
    //attenuation
    float distance = length(light.pos - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    //"soft lightning edge"
    //spotlight intensity (I = (\theta - \outerCutoff) / \eps )
    float theta = dot(lightDir, normalize(-light.dir)); 
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    //it's all coming together
    vec3 ambient = light.ambient * texture(diffuseTex, texCoord).rgb;
    vec3 diffuse = light.diffuse * _diffuse * texture(diffuseTex, texCoord).rgb;
    vec3 specular = light.specular * _specular;

    if(hasSpecularTex)
    {
        specular *= texture(specularTex, texCoord).rgb;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

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

    fragColor = vec4(objColor * (ambientL + diffuse + specular + 
                                    (usesFlashlight ? CalcFlashlight(flashlight, normobjnorm, worldPos, viewDir) : vec3(0) ) ), 1.0);
}