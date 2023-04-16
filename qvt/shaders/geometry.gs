#version 450 core
layout(points) in;
layout(triangle_strip,max_vertices = 5) out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;


void main(void)
{
    gl_Position = projection*view*transform*(gl_in[0].gl_Position + vec4(-10,10,0,0));
    EmitVertex();

    gl_Position = projection*view*transform*(gl_in[0].gl_Position + vec4(10,-10,0,0));
    EmitVertex();

    gl_Position = projection*view*transform*(gl_in[0].gl_Position + vec4(0,0,10,0));
    EmitVertex();

    gl_Position = projection*view*transform*(gl_in[0].gl_Position + vec4(0,0,-10,0));
    EmitVertex();

    gl_Position = projection*view*transform*(gl_in[0].gl_Position + vec4(-10,10,0,0));
    EmitVertex();



    EndPrimitive();
}