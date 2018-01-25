smooth in vec4 fNormal;
smooth in vec4 fPosition;
smooth in vec3 fTangent;
smooth in vec3 fBitangent;
smooth in vec3 passthroughPosition;
smooth in vec2 passthroughUV;
smooth in vec3 passthroughNormal;
smooth in vec4 gouraudColor;
smooth in vec2 screenPosition;
flat in vec4 flatColor;

out vec4 fColor;

uniform sampler2D myTexture;
uniform sampler2D turbulenceTexture;
uniform mat4 modelView;
uniform mat4 view;

#define TEXTURE_NONE 0 
#define TEXTURE_MARBLE 1
#define TEXTURE_FILE 2
#define TEXTURE_CUBEMAP 3
#define TEXTURE_ENVIRONMENT 4

uniform int textureType;

vec4 marble(vec4 color1, vec4 color2, float x)
{
	float frac = (sin(x) + 1) / 2.0;
	return frac * color1 + (1-frac) * color2;
}

vec2 getUV()
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
	return uv;
}

void doShading()
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
		vec2 uv = getUV();
		if (uvType == UV_BOX)
		{
			calculateLighting(fPosition.xyz, fNormal.xyz, vec3(1,0,0), vec3(0,-1,0), uv, fColor);
		}
		
		else
		{
			calculateLighting(fPosition.xyz, normalize(fNormal.xyz), normalize(fTangent), normalize(fBitangent), uv, fColor);
		}
		// TODO: spherical
	}
}

void doTexture()
{	
	if (textureType == TEXTURE_NONE)
	{
		return;
	}
	
	vec2 uv = getUV();
	
	if (textureType == TEXTURE_FILE)
	{
		fColor *= texture(myTexture, uv);
	}
	else if (textureType == TEXTURE_CUBEMAP)
	{
		fColor = textureCube(cubemapTexture, passthroughPosition.xyz);
	}
	else if (textureType == TEXTURE_ENVIRONMENT)
	{
		vec3 coords = -reflect(-fPosition.xyz, fNormal.xyz);
		fColor *= textureCube(cubemapTexture, (inverse(view) * vec4(coords,0)).xyz);
	}
	else // marble
	{
		vec2 uv = getUV();
		vec4 color1 = vec4(0.52,0.37,0.26,1);
		vec4 color2 = vec4(0,0,0,1);
		float x = uv.x*uv.x +uv.y*uv.y + texture(turbulenceTexture, uv).r;
		vec4 color = marble(color1, color2, sin(x) * 20);
		fColor *= color;
	}
}

void main() 
{
	if (toonShadingStage2)
	{
		fColor = vec4(0,0,0,1);
		return;
	}
	
	doShading();
	doTexture();
}
