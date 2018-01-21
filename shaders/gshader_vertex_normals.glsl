layout(triangles) in;
layout(line_strip, max_vertices=6) out;

in vec3 passthroughPosition[];
in vec3 passthroughNormal[];

uniform mat4 modelView;
uniform mat4 modelViewProjection;
uniform mat4 projection;

void main()
{
	for(int i=0; i<gl_in.length(); i++)
	{
		vec4 point = vec4(passthroughPosition[i], 1);
		vec4 normal = vec4(passthroughNormal[i], 0) * 0.3;
		mat4 transform = transpose(inverse(modelView));
		
		gl_Position = modelViewProjection * point;
		EmitVertex();
		
		gl_Position = projection * (modelView * point + transform * normal);
		EmitVertex();
		
		EndPrimitive();
	}
}