#version 150

uniform mat4 modelView;

in vec4 vPosition;
in vec4 vNormal;

// out vec4 gl_Position
out vec4 fNormal;

void main()
{
	gl_Position = modelView * vPosition;
	fNormal = modelView * vNormal;
}