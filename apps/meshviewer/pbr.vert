#version 460 core

// Already in world space
layout(location=0) in vec3 vertexPos;
layout(location=1) in vec3 vertexNormal;

out vec3 V;
out vec3 N;

uniform Camera
{
    mat4 projView;
    vec4 camPos;
} camera;

void main()
{
    V = normalize(vec3(camera.camPos) - vertexPos);
    N = vertexNormal;
    gl_Position = camera.projView * vec4(vertexPos, 1.0);
}



