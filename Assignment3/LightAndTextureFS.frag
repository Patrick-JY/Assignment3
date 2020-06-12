#version 330 core

// interpolated values from the vertex shaders
in vec3 vPos;
in vec3 vN;
in vec2 vTexCoord;

// light and material structs
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

// uniform input data
uniform mat4 uViewMatrix;
uniform Light uLight;
uniform Material uMaterial;
uniform sampler2D uTextureSampler;
uniform float uAlpha = 1.0f;

vec3 fColor;

// output data, My hack to allow for minimal shader changes
out vec4 alphafColor;

void main()
{
    vec3 N = normalize(vN);
    vec3 L = normalize((uViewMatrix * vec4(uLight.position, 1.0f)).xyz - vPos);
    vec3 E = normalize(-vPos);
    vec3 H = normalize(L + E);

	// calculate the ambient, diffuse and specular components
	vec3 ambient  = uLight.ambient * uMaterial.ambient;
    vec3 diffuse  = uLight.diffuse * uMaterial.diffuse * max(dot(L, N), 0.0);
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	if(dot(L, N) > 0.0f)
	    specular = uLight.specular * uMaterial.specular * pow(max(dot(N, H), 0.0), uMaterial.shininess);

	// set output color
	fColor = ambient + diffuse + specular;
	fColor *= texture(uTextureSampler, vTexCoord).rgb;

	alphafColor = vec4(fColor,uAlpha);
	

	//fColor = texture(uTextureSampler, vTexCoord).rgb;
}