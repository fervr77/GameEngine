#version 450 core

layout(location=0) out vec4 fragColor;


uniform vec3 color;


void main(void)
{
    fragColor = vec4(color,1);

}