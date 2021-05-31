#version 460 core

in vec3 rayOrigin;
in vec3 rayDirection;

out vec4 color;

uniform Camera
{
    mat4 proj;
    mat4 view;
    vec4 camPos;
    vec4 forward;
} camera;

uniform sampler3D inputTexture;
uniform vec3 minB;
uniform vec3 maxB;

bool intersectCube(const in vec3 pos)
{
    return minB.x <= pos.x && maxB.x >= pos.x
            && minB.y <= pos.y && maxB.y >= pos.y
            && minB.z <= pos.z && maxB.z >= pos.z;
}

float sampleDensity(const in vec3 pos)
{
    const vec3 uv = (pos - minB) / (maxB - minB);
    return texture(inputTexture, uv).r;
}

void main()
{
    const uint MAX_MARCH_STEPS = 128;
    const vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
    const float worldStepSize = length(maxB - minB) / float(MAX_MARCH_STEPS);
    const float lightStepSize = worldStepSize * 2.0;
    const float absorption = 0.5;

    uint i = 0;
    float density = 0.0;
    float Ttotal = 1.0;

    vec3 C = vec3(0.0);
    float a = 0.0;

    while(i < MAX_MARCH_STEPS)
    {
        const vec3 samplePos = rayOrigin + rayDirection * worldStepSize * float(i);
        if(!intersectCube(samplePos))
            break;

        ++i;
        const float tDensity = sampleDensity(samplePos);

        // Compositing
        C += (1.0 - a) * vec3(1.0);
        a += (1.0 - a) * tDensity;

        if(tDensity > 0.01)
        {
            float localT = 1.0;
            for(uint j = 1; j < 6; ++j)
            {
                const vec3 lightSamplePos = samplePos + lightDirection * lightStepSize * float(j);
                if(!intersectCube(lightSamplePos))
                    break;

                localT *= (1.0 - sampleDensity(lightSamplePos) * absorption);
                if(localT < 0.05)
                    break;
            }

            density += tDensity;
            Ttotal *= clamp(localT, 0.0, 1.0) * (1.0 - tDensity * absorption);

            if(density > 0.95)
                break;
        }
    }

    color = vec4(C * clamp(Ttotal, 0.0, 1.0) * density, a);
}
