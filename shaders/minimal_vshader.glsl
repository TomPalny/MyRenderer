#version 150

uniform mat4 modelView;
uniform mat4 projection;

in vec4 vPosition;
in vec4 vNormal;

out vec3 fNormal;
out vec3 fPosition; // unlike gl_Position this is in camera space!

void main()
{
	// TODO: this matrix (after multiplication) should be uniform
	gl_Position = projection * modelView * vPosition;
	fPosition = (modelView * vPosition).xyz;
	//fNormal = normalize((vNormal * transpose(inverse(modelView))).xyz);
	fNormal = normalize((modelView * vNormal).xyz);
}