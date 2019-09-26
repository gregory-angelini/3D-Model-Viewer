#version 330 core

out vec4 fragColor;// �������� ���� ��������� (�������� ������ ������������ �������)

in vec3 ourTexCoord;

uniform samplerCube diffuse;


void main()
{
    fragColor = texture(diffuse, ourTexCoord);
}

