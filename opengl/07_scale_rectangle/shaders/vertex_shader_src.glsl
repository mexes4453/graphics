#version 410 core 

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Perspective;

smooth out vec4 theColor;
void main()
{
    vec4 newPosition = u_Perspective * u_ModelMatrix * position;
    gl_Position = newPosition;
    theColor = color;
}
