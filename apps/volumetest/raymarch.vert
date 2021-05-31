#version 460 core

layout(location=0) in vec3 vertexPos;

out vec3 rayOrigin;
out vec3 rayDirection;

uniform Camera
{
    mat4 proj;
    mat4 view;
    vec4 camPos;
    vec4 forward;
} camera;

void main()
{
    rayOrigin = vertexPos;
    rayDirection = normalize(vertexPos - camera.camPos.xyz);
    gl_Position = camera.proj * camera.view * vec4(vertexPos, 1.0);
}
