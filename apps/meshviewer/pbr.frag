#version 460 core

layout(location = 0) in vec3 V;
layout(location = 1) in vec3 N;

out vec4 color;

#define PI 3.1415926535

uniform Material
{
    vec4 albedoRoughness;
    vec4 fresnelMetallic;
} material;


// UE4 PBR based on https://learnopengl.com/PBR/Theory
// NORMAL DISTRIBUTION FUNCTION
float NDFTrowbridgeReitz(vec3 normal, vec3 halfVector, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(normal, halfVector), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;

    return nom / denom;
}

// GEOMETRY FUNCTIONS
float SubGFSchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GFSmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = SubGFSchlickGGX(NdotV, k);
    float ggx2 = SubGFSchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

// FRESNEL EQUATION
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    const vec3 L = normalize(vec3(1.0, 1.0, 1.0));
    vec3 H = normalize(V + L);

    const vec3 radiance = vec3(5.0);

    // Compute Geometry and Normal distribution terms
    float NDF = NDFTrowbridgeReitz(N, H, material.albedoRoughness.a);
    float G   = GFSmith(N, V, L, material.albedoRoughness.a);
    vec3 F0 = mix(material.fresnelMetallic.rgb, material.albedoRoughness.rgb, material.fresnelMetallic.a);
    vec3 F = FresnelSchlick(max(0.0, dot(H, V)), material.fresnelMetallic.rgb);

    // Condutors do not scatter aborved light back, hence the * (1.0 - pbrMat.fresnel0Metalness.a)
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.fresnelMetallic.a;

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    float NdotL = max(0.0, dot(N, L));
    vec3 Lo = (kD * material.albedoRoughness.rgb / PI + specular) * radiance * NdotL;

    vec3 ambient = material.albedoRoughness.rgb * vec3(0.03);
    vec3 HDRColor = ambient + Lo;

    // HDR Tone mapping
    HDRColor = HDRColor / (HDRColor + vec3(1.0));
    HDRColor = pow(HDRColor, vec3(1.0 / 2.2));

    color = vec4(HDRColor, 1.0);
}
