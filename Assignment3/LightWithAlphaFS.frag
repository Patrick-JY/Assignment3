#version 330 core

// interpolated values from the vertex shaders
in vec3 vNormal;
in vec3 vPosition;

// uniform input data
struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Light uLight;
uniform Material uMaterial;
uniform mat4 uViewMatrix;

// output data
out vec4 fColor;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize((uViewMatrix * uLight.position).xyz - vPosition);
    vec3 E = normalize(-vPosition);
    vec3 H = normalize(L + E);

	// calculate the ambient, diffuse and specular components
	vec4 ambient = uLight.ambient * uMaterial.ambient;
    vec4 diffuse = uLight.diffuse * uMaterial.diffuse * max(dot(L, N), 0.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	if(dot(L, N) > 0.0f)
	    specular = uLight.specular * uMaterial.specular * pow(max(dot(N, H), 0.0), uMaterial.shininess);

	// set output color
	fColor = diffuse + specular + ambient;	
}