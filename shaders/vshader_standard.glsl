uniform mat4 modelView;
uniform mat4 modelViewProjection;
uniform mat4 projection;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;

out vec4 fNormal;
out vec4 fPosition; // unlike gl_Position this is in camera space!
out vec3 fTangent;
out vec3 fBitangent;
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
	
	fPosition = modelView * position;
	fNormal = transpose(inverse(modelView)) * vNormal;
	fTangent = (modelView * vec4(vTangent,0)).xyz;
	fBitangent = (modelView * vec4(vBitangent,0)).xyz;
	
	passthroughPosition = vPosition.xyz;
	passthroughNormal = vNormal.xyz;
	passthroughUV = vUV;
	
	calculateLighting(fPosition.xyz, fNormal.xyz, fTangent.xyz, fBitangent.xyz, vec2(0,0), flatColor);
	calculateLighting(fPosition.xyz, fNormal.xyz, fTangent.xyz, fBitangent.xyz, vec2(0,0), gouraudColor);
	
	if (toonShadingStage2)
	{
		gl_Position = projection * (fPosition + fNormal * 0.02);
	}

}