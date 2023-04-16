#version 450 core

layout(location=0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;




void main(void)
{   

    FragColor = texture(ourTexture,TexCoord);

}