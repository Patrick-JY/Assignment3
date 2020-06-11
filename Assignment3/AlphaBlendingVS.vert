#version 330 core

// input data (different for all executions of this shader)
in vec3 aPosition;
in vec3 aColour;

// uniform input data
uniform mat4 uModelViewProjectionMatrix;

// output data (will be interpolated for each fragment)
out vec3 vPosition;
out vec3 vColour;

void main()
{
	// set vertex position
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);

	// set colour
	vColour = aColour;
}

