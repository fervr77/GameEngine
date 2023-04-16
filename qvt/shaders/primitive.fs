#version 450 core

layout(location=0) out vec4 fragColor;
layout(location=1) out int color2;

uniform vec3 color;
uniform int entityID;

void main(void)
{
    fragColor = vec4(color,1.0);
    color2 = entityID;  
}