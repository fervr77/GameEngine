#version 450 core

layout(location=0) in vec3 aPos;


uniform mat4 transform;



void main(void)
{
    gl_Position = vec4(aPos,1.0f);

}