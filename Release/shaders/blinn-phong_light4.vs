#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

out VS_OUT
{
    vec3 worldPos;  
    vec3 worldTangent;
    vec3 worldNormal;
    vec2 texCoord;
} vs_out;


uniform mat4 model;
uniform mat4 projectionView;
uniform mat3 normalMatrix;

void main()
{
    vs_out.texCoord = texCoord;
    vs_out.worldNormal = normalMatrix * normal;
    vs_out.worldTangent = normalMatrix * tangent;
    vs_out.worldPos = (model * vec4(position, 1.0f)).xyz;
    gl_Position = projectionView * vec4(vs_out.worldPos, 1.0f);
}