#version 450 core

layout(triangles) in;
layout(line_strip,max_vertices=4)out;

uniform mat4 projection;
uniform mat4 view;


void main()
{
    for(int i=0;i<gl_in.length();i++)
    {
        vec4 position = gl_in[i].gl_Position;

        gl_Position = projection*view*position;
        EmitVertex();
    }

    vec4 position  = gl_in[0].gl_Position;
    gl_Position = projection*view*position;
    EmitVertex();

    EndPrimitive();
}
