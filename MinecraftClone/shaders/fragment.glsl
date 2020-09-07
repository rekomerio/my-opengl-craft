#version 330 core
out vec4 FragColor;  
in vec4 vertexColor;
in vec2 TexCoord;
flat in int fUseTexture;

uniform sampler2D texture1;

void main()
{
    if (fUseTexture == 1)
        FragColor = texture(texture1, TexCoord);
    else
        FragColor = vertexColor;
}