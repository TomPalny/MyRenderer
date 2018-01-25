layout(triangles) in;
layout(line_strip, max_vertices=6) out;

in vec3 passthroughPosition[];
in vec3 passthroughNormal[];

uniform mat4 modelView;
uniform mat4 modelViewProjection;
uniform mat4 projection;

void emit_line(in vec4 x, in vec4 y)
{
	gl_Position = x;
	EmitVertex();
	gl_Position = y;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	vec4 point1 = modelViewProjection * vec4(passthroughPosition[0], 1);
	vec4 point2 = modelViewProjection * vec4(passthroughPosition[1], 1);
	vec4 point3 = modelViewProjection * vec4(passthroughPosition[2], 1);
	emit_line(point1, point2);
	emit_line(point2, point3);
	emit_line(point3, point1);
}