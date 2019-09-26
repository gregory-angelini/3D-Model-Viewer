#version 330 core

#define AMOUNT_OF_POINT_LIGHTS 2

out vec4 fragColor;

in VS_OUT
{
    vec3 worldPos;  
    vec3 worldTangent;
    vec3 worldNormal;
    vec2 texCoord;
} fs_in;

 
struct Material 
{
    sampler2D diffuse[1];
    sampler2D normal[1];
    sampler2D specular[1];
    float shininess;
};

  

struct DirectionalLight 
{
    vec3 direction;

    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

 

struct PointLight
{
    vec3 position;
  
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};



uniform PointLight pointLights[AMOUNT_OF_POINT_LIGHTS];
uniform Material material;

uniform vec3 viewPos;
uniform samplerCube skybox;



vec4 CalculatePointLight(PointLight _light, vec3 _normal, vec3 _viewDir, vec3 _reflectDir);


vec3 CalculateBumpedNormal();


void main()
{
    vec4 output = vec4(0.0f);
    vec3 normal = CalculateBumpedNormal();
    vec3 viewDir = normalize(viewPos - fs_in.worldPos);
    vec3 reflectionDir = reflect(-viewDir, normal);
    output.w = texture(material.diffuse[0], fs_in.texCoord).w;

    for(int i = 0; i < AMOUNT_OF_POINT_LIGHTS; i++)
    { 
       output += CalculatePointLight(pointLights[i], normal, viewDir, reflectionDir); 
    }
        
    fragColor = output;
}



vec3 CalculateBumpedNormal()
{
    vec3 normal = normalize(fs_in.worldNormal);
    vec3 tangent = normalize(fs_in.worldTangent);

    tangent = normalize(tangent - dot(tangent, normal) * normal); 
    vec3 bitangent = cross(tangent, normal);
    mat3 TBN = mat3(tangent, bitangent, normal);

    vec3 bumpMapNormal = texture(material.normal[0], fs_in.texCoord).xyz;
    bumpMapNormal = 2.0f * bumpMapNormal - vec3(1.0f);
    vec3 newNormal = TBN * bumpMapNormal; 
    newNormal = normalize(newNormal);
    return newNormal;
}


vec4 CalculatePointLight(PointLight _light, vec3 _normal, vec3 _viewDir, vec3 _reflectionDir)
{ 
    vec3 lightDir = normalize(_light.position - fs_in.worldPos);
    vec3 halfwayDir = normalize(lightDir + _viewDir);

    float diffuseRaw = max(dot(_normal, lightDir), 0.0f);

    float specularRaw = pow(max(dot(_normal, halfwayDir), 0.0f), material.shininess);

    vec4 ambient = vec4((_light.ambient * _light.color), 1.0f) * texture(material.diffuse[0], fs_in.texCoord);
    vec4 diffuse = vec4((_light.diffuse * _light.color), 1.0f) * (diffuseRaw * texture(material.diffuse[0], fs_in.texCoord));

    vec4 specularSample = texture(material.specular[0], fs_in.texCoord);
    vec4 specular = vec4((_light.specular * _light.color), 0.0f) * (specularRaw * specularSample);


    float distance = length(_light.position - fs_in.worldPos);
    
    float attenuation = 1.0f / (_light.constant + 
                                _light.linear * distance + 
    		                _light.quadratic * (distance * distance)); 

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;

    //vec3 skyboxDiffuse = pow(texture(skybox, _reflectionDir).rgb, vec3(2.2f));
    vec4 reflectionDiffuse = vec4(texture(skybox, _reflectionDir).rgb, 0.0f) * specularSample;
    return (ambient + diffuse + specular + reflectionDiffuse);
}

