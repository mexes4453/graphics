#version 410 core 

smooth in vec4 theColor;

out vec4 color;

void main()
{
    color = theColor;
}