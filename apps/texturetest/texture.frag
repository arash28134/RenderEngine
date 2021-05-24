#version 460 core

in vec2 UV;

out vec4 color;

uniform sampler2D inputTexture;

void main()
{
   color = texture(inputTexture, UV);
}
