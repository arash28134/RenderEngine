#version 460 core

out vec2 UV;

void main()
{
    UV = vec2(float(gl_VertexID / 2), float(gl_VertexID % 2));
    gl_Position = vec4((UV - 0.5) * 2.0, 0.0, 1.0);
}
