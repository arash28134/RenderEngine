#version 460 core

layout(location=0) in vec3 vertexPos;

out vec3 samplePos;

uniform Camera
{
    mat4 projView;
} camera;

void main()
{
    samplePos = vec3(vertexPos.x, -vertexPos.y, vertexPos.z);
    vec4 cp = camera.projView * vec4(vertexPos, 1.0);
    gl_Position = cp.xyww; // Z == W, always last element
}
