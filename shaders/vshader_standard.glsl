uniform mat4 modelView;
uniform mat4 modelViewProjection;
uniform mat4 projection;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vUV;

out vec3 fNormal;
out vec3 fPosition; // unlike gl_Position this is in camera space!
out vec3 passthroughPosition; // this is in unmodified object space 
out vec3 passthroughNormal;   // this is in unmodified object space 
out vec2 passthroughUV; // TODO: do we need to transform this? e.g. how should stretching impact this?
out vec2 screenPosition;
out vec4 gouraudColor;
flat out vec4 flatColor;

void main()
{
	vec4 position = vPosition;
	if (positionAnimationType != 0)
	{
		float param = animationParam / 2.0f + 0.5;
		position.y = clamp(abs(sin(param * 4 * PI / 2)), 0.30, 1) * position.y;
	}
	gl_Position = modelViewProjection * position;
	screenPosition = gl_Position.xy / gl_Position.w;
	
	fPosition = (modelView * position).xyz;
	fNormal = normalize((transpose(inverse(modelView)) * vNormal).xyz);
	passthroughPosition = vPosition.xyz;
	passthroughNormal = vNormal.xyz;
	passthroughUV = vUV;
	
	calculateLighting(fPosition, fNormal, flatColor);
	calculateLighting(fPosition, fNormal, gouraudColor);
	
	if (toonShadingStage2)
	{
		gl_Position = projection * (vec4(fPosition, 1) + vec4(fNormal, 0) * 0.02);
	}
}