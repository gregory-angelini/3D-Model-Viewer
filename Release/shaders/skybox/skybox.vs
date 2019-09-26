#version 330 core

layout (location = 0) in vec3 position;

out vec3 ourTexCoord;

uniform mat4 model;
uniform mat4 projectionView;


void main()
{      
    ourTexCoord = position;// позиции вершин единичного куба, выровненного по осям, и расположенного в начале координат - соответствуют текстурным координатам кубической карты 
    gl_Position = projectionView * model * vec4(position, 1.0f);// переводим вершину в систему координат клипа [-w, w]
}