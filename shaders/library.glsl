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

uniform sampler2D myTexture;
uniform bool hasTexture;

void calculateLighting(in vec3 position, in vec3 normal, out vec4 color)
{
	color = vec4(0,0,0,0);
	for (int i=0; i<numLights; i++)
	{
		vec3 N = normal;
		vec3 L = normalize(lights[i].position.xyz - position);
		if (lights[i].directional)
		{
			L = normalize(lights[i].position.xyz);
		}
		vec3 V = normalize(-position);
		vec3 R = normalize(2 * dot(L, N) * N - L);
		float alpha = 5;
		float diffuse_modifier = clamp(dot(L, N), 0.0, 1.0);
		
		float specular_modifier;
		if (dot(R,V) > 0)
		{
			specular_modifier = clamp(pow(dot(R,V), alpha), 0.0, 1.0);
		}
		else
		{
			// no specular when the light is on the wrong side of the face
			specular_modifier = 0;
		}
			
		// TODO: clamp each one individually
		// TODO: take into account the material
		color += lights[i].ambient * material.ambient;
		color += lights[i].diffuse * material.diffuse * diffuse_modifier;
		color += lights[i].specular * material.specular * specular_modifier;
		color += material.emissive;
	}
	
	color = clamp(color, vec4(0,0,0,0), vec4(1,1,1,1));
	color = vec4(color.xyz, 1);
}
