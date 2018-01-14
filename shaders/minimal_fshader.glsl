#version 150

uniform struct {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} light;

uniform struct {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
} material;

smooth in vec3 fNormal;
smooth in vec3 fPosition;
out vec4 fColor;

void main() 
{ 
	vec3 N = fNormal;
	vec3 L = normalize(light.position.xyz - fPosition.xyz);
	vec3 V = normalize(-fPosition.xyz); // TODO: was originally negative
	vec3 R = normalize(L - 2 * dot(L, N) * N);
	float alpha = 5;
	float diffuse_modifier = clamp(dot(L, N), 0.0, 1.0);
	float specular_modifier = clamp(pow(dot(R,V), alpha), 0.0, 1.0);

	// TODO: clamp each one individually
	// TODO: take into account the material
	fColor = vec4(0,0,0,0);
	fColor += light.ambient * material.ambient;
	fColor += light.diffuse * material.diffuse * diffuse_modifier;
	fColor += light.specular * material.specular * specular_modifier;
	fColor += material.emissive;
	fColor = vec4(fColor.xyz, 1);
} 
