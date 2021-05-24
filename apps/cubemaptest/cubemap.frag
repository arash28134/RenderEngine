#version 460 core

in vec3 samplePos;

out vec4 color;

uniform samplerCube inputTexture;

void main()
{
    color = texture(inputTexture, samplePos);
}
