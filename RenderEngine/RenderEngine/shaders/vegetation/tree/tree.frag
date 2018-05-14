#version 430 core

#ifndef SHADOW_MAP
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos;

layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inNormal;
layout (location=3) in vec3 inEmission;
layout (location=4) in vec3 inShadowMapPos;

uniform sampler2D depthTexture;

uniform vec3 lightDir;

uniform vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

#else
layout (location=0) out vec4 lightdepth;
#endif

void main()
{
#ifndef SHADOW_MAP
	vec3 rawNormal = normalize(inNormal);
#ifdef WIRE_MODE
	outColor = vec4(0,0,0,1);
	outNormal = vec4(rawNormal, 0);
	outSpecular = vec4(0,0,0,0);
	outEmissive = vec4(0,0,0,0);
	outPos = vec4(inPos, 1);
#else
	// APPLY SHADOW MAP
	// ------------------------------------------------------------------------------
	float bias = clamp(0.005 * tan(acos(dot(rawNormal, lightDir))), 0.0, 0.01);
	float curDepth = inShadowMapPos.z - bias;
	float visibility = texture(depthTexture, inShadowMapPos.xy).x < curDepth? 0.0 : 1.0;
	/*
	if(inShadowMapPos.x >= 0 && inShadowMapPos.x <= 1 && inShadowMapPos.y >= 0 && inShadowMapPos.y <= 1)
	{
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * (( texture( depthTexture, inShadowMapPos.xy + poissonDisk[i] / 700.0 ).x  <  curDepth )? 1.0 : 0.0);
		}
	}
	*/
	outColor = vec4(inColor, visibility);
	outNormal = vec4(rawNormal, 0);
	outSpecular = vec4(0,0,0,0);
	outEmissive = vec4(inEmission,0);
	outPos = vec4(inPos, 1);
#endif
#else
	lightdepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 0);
#endif
}