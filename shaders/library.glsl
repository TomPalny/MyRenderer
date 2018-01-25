#version 150
#define MAX_LIGHTS 5

#define PI 3.1415926535897932384626433832795

#define FILL_FLAT 0
#define FILL_GOURAUD 1
#define FILL_PHONG 2

#define UV_PREDEFINED 0
#define UV_BOX 1
#define UV_SPHERE 2

uniform int numLights;
uniform int fillType;
uniform int uvType;
uniform float animationParam;
uniform int positionAnimationType;
uniform int colorAnimationType;
uniform bool toonShading;
uniform bool toonShadingStage2;
uniform bool hasBumpTexture;
uniform sampler2D bumpTexture;
uniform samplerCube cubemapTexture;

uniform struct {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	bool directional;
} lights[MAX_LIGHTS];

uniform struct {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
} material;

vec4 applyColorAnimation(vec4 color)
{
	if (colorAnimationType == 0)
	{
		return color;
	}
	else if (colorAnimationType == 1)
	{
		float param = animationParam / 2.0f + 0.5;
		return color * abs(sin(param * 4 * PI / 2));
	}
	else
	{
		vec4 result = color;
		float param = animationParam / 2.0f + 0.5;
		result.g *= abs(sin(param * 4 * PI / 2));
		return result;
	}
}

vec4 applyToonShading(vec4 intensities, vec4 color)
{
	if (!toonShading)
	{
		// clamping probably unnecessary
		return clamp(intensities * color, 0.0, 1.0);
	}
	return round(intensities * 3) / 3 * color;
}

void calculateLighting(in vec3 position, in vec3 normal, in vec3 tangent, in vec3 bitangent, in vec2 uv, out vec4 color)
{
	color = vec4(0,0,0,0);
	for (int i=0; i<numLights; i++)
	{
		vec3 N = normalize(normal);
		vec3 L = normalize(lights[i].position.xyz - position);
		if (lights[i].directional)
		{
			L = normalize(lights[i].position.xyz);
		}
		vec3 V = normalize(-position);
		vec3 test;
		if (hasBumpTexture)
		{
			// TODO: deal with directional light
			// TODO: for UV_PREDEFINED these should be normalized!
			// for UV_BOX these shouldn't be normalized
			// I've dealt with that outside of this function...
			//mat3 TBN = transpose(mat3(normalize(tangent), normalize(bitangent), normalize(normal)));
			mat3 TBN = transpose(mat3(tangent, bitangent, normal));
			L = normalize(TBN * lights[i].position.xyz - position);
			V = TBN * normalize(-position);
			N = normalize(texture(bumpTexture, uv).rgb * 2.0 - 1.0);
			test = TBN * -position;
		}
		
		vec3 R = normalize(2 * dot(L, N) * N - L);
		float alpha = 5;
		float diffuse_modifier = max(dot(L, N), 0.0);
		
		float specular_modifier=0;
		if (dot(R,V) > 0 && diffuse_modifier > 0)
		{
			specular_modifier = clamp(pow(dot(R,V), alpha), 0.0, 1.0);
		}
		else
		{
			// no specular when the light is on the wrong side of the face
			specular_modifier = 0;
		}
		
		color += applyToonShading(lights[i].ambient, applyColorAnimation(material.ambient));
		color += applyToonShading(lights[i].diffuse * diffuse_modifier, applyColorAnimation(material.diffuse));
		color += applyToonShading(lights[i].specular * specular_modifier, applyColorAnimation(material.specular));
		color += applyColorAnimation(material.emissive); // no toon shading for this...
		//color = vec4(N, 1);
		//color = vec4(tangent, 1) * 5;
		//color = vec4(test, 1);
	}
	
	color = clamp(color, vec4(0,0,0,0), vec4(1,1,1,1));
	color = vec4(color.xyz, 1);
}
