#version 450 core

layout(location=0) in vec3 aPos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;



void main(void)
{
    gl_Position = projection*view*transform*vec4(aPos,1);
}