#version 150

in vec4 fNormal;
out vec4 fColor;

void main() 
{ 
   fColor = vec4(fNormal.xyz, 1);
} 
