#version 330 core

// interpolated values from the vertex shaders
in vec3 vPosition;
in vec3 vColour;

// uniform input data
uniform float uAlpha;

// output data
out vec4 fColor;

void main()
{
	// set output color
	fColor = vec4(vColour, uAlpha);
}