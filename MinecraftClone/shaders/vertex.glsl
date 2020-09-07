#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform int useTexture;
uniform vec4 color;

out vec4 vertexColor;
out vec2 TexCoord;
flat out int fUseTexture;

void main()
{
    fUseTexture = useTexture;
    vertexColor = color;
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}