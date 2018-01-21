layout(triangles) in;
layout(line_strip, max_vertices=2) out;

in vec3 passthroughPosition[];
in vec3 passthroughNormal[];

uniform mat4 modelView;
uniform mat4 modelViewProjection;
uniform mat4 projection;

void main()
{
	vec3 p1 = passthroughPosition[0];
	vec3 p2 = passthroughPosition[1];
	vec3 p3 = passthroughPosition[2];
	
	vec3 v1 = p2 - p1;
	vec3 v2 = p3 - p2;
	
	vec4 point = vec4((p1 + p2 + p3) / 3.0, 1);
	vec4 normal = vec4(normalize(cross(v1, v2)), 0) * 0.3;
	mat4 transform = transpose(inverse(modelView));
		
	gl_Position = modelViewProjection * point;
	EmitVertex();
		
	gl_Position = projection * (modelView * point + transform * normal);
	EmitVertex();
		
	EndPrimitive();
}