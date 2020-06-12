#version 330 core

// interpolated values from the vertex shaders
in vec3 vPosition;
in vec3 vColour;

struct ColorMaterial {
	float r;
	float g;
	float b;
	
};

// uniform input data
uniform float uAlpha;
uniform ColorMaterial uMaterial;

vec3 colour;
// output data
out vec4 fColor;

void main()
{
	colour = vec3(uMaterial.r,uMaterial.g,uMaterial.b);
	
	// set output color
	fColor = vec4(colour, uAlpha);
}