smooth in vec4 fNormal;
smooth in vec4 fPosition;
smooth in vec3 passthroughPosition;
smooth in vec2 passthroughUV;
smooth in vec4 gouraudColor;
smooth in vec2 screenPosition;
flat in vec4 flatColor;

out vec4 fColor;


void applyLightingAccordingToShading()
{
	if (fillType == FILL_FLAT)
	{
		fColor = flatColor;
	}
	else if (fillType == FILL_GOURAUD)
	{
		fColor = gouraudColor;
	}
	else
	{
		calculateLighting(fPosition.xyz, fNormal.xyz, fColor);
	}
}

void getUV(out vec2 uv)
{
	if (uvType == UV_BOX)
	{
		uv.x = passthroughPosition.x;
		uv.y = 1-passthroughPosition.y;
	}
	else if (uvType == UV_SPHERE)
	{
		vec3 pos = normalize(passthroughPosition.xyz);
		float theta = acos(pos.z);
		float phi = atan(pos.y, pos.x);
		uv.x = theta / PI;
		uv.y = 1 - phi / PI;
	}
	else
	{
		uv.x = passthroughUV.x;
		uv.y = 1 - passthroughUV.y;
	}
}

void main() 
{
	if (toonShadingStage2)
	{
		fColor = vec4(0,0,0,1);
		return;
	}
	
	applyLightingAccordingToShading();
	
	if (hasTexture)
	{
		vec2 uv;
		getUV(uv);
		fColor *= texture(myTexture, uv);
	}
	else // marble
	{
		vec2 uv;
		getUV(uv);
		vec4 green = vec4(0,1,0,1);
		vec4 blue = vec4(0,0,1,1);
		vec4 color = marble(green, blue, uv.x*5);
		fColor *= color;
	}
}
