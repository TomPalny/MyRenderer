smooth in vec3 fNormal;
smooth in vec3 fPosition;
smooth in vec3 passthroughPosition;
smooth in vec2 passthroughUV;
smooth in vec4 gouraudColor;
smooth in vec2 screenPosition;
flat in vec4 flatColor;

out vec4 fColor;


void main() 
{
	if (toonShadingStage2)
	{
		fColor = vec4(0,0,0,1);
		return;
	}
	
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
	
	if (hasTexture)
	{
		vec2 uv;
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
		fColor *= texture(myTexture, uv);
	}
} 
