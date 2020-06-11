//was having some trouble with includes (string was apparently being defined twice) 
//so this is taken from lab 5
#define _USE_MATH_DEFINES
#include <cstdio>		// for C++ i/o
#include <iostream>
#include <string>
#include <cstddef>
#include <math.h>

using namespace std;	// to avoid having to use std::

#include <GLEW/glew.h>	// include GLEW
#include <GLFW/glfw3.h>	// include GLFW (which includes the OpenGL header)
#include <glm/glm.hpp>	// include GLM (ideally should only use the GLM headers that are actually used)
#include <glm/gtx/transform.hpp>
using namespace glm;	// to avoid having to use glm::

#include <AntTweakBar.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "Camera.h"
#include "bmpfuncs.h"

#define MOVEMENT_SENSITIVITY 3.0f		// camera movement sensitivity
#define ROTATION_SENSITIVITY 0.3f		// camera rotation sensitivity



//structs
//vertex stuct
typedef struct Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat tangent[3];
	GLfloat texCoord[2];
} Vertex;

typedef struct simpleVertex
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texCoord[2];
}simpleVertex;

struct simpleLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//mesh properties
typedef struct Mesh
{
	Vertex* pMeshVertices; //points to mesh vertices
	GLint numberOfVertices; //number of vertices in the mesh
	GLint* pMeshIndices;
	GLint numberOfFaces;
} Mesh;

// light and material structs
typedef struct Light {
	vec3 position;
	glm::vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int type;
};

typedef struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


Vertex wall_vertices[] = {
	// Front: triangle 1
	// vertex 1
	-1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 1.0f,			// texture coordinate
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	4.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	4.0f, 1.0f,			// texture coordinate

	// triangle 2
	// vertex 1
	4.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	4.0f, 1.0f,			// texture coordinate
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	4.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	4.0f, 0.0f,			// texture coordinate
};

Vertex floor_vertices[] = {
	// Front: triangle 1
	// vertex 1
	-2.5f, 2.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 2.5f,			// texture coordinate
	// vertex 2
	-2.5f, -2.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	2.5f, 2.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	2.5f, 2.5f,			// texture coordinate

	// triangle 2
	// vertex 1
	2.5f, 2.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	2.5f, 2.5f,			// texture coordinate
	// vertex 2
	-2.5f, -2.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	2.5f, -2.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	2.5f, 0.0f,			// texture coordinate
};

Vertex painting_vertices[] = {
	// Front: triangle 1
	// vertex 1
	-1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 1.0f,			// texture coordinate
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	1.0f, 1.0f,			// texture coordinate

	// triangle 2
	// vertex 1
	1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	1.0f, 1.0f,			// texture coordinate
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	//1.0f, 0.0f, 0.0f,	// tangent
	1.0f, 0.0f,			// texture coordinate
};


Vertex partition_vertices[] = {
	// Front: triangle 1
	// vertex 1
	-1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	
	// vertex 3
	1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	

	// triangle 2
	// vertex 1
	1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	
	// vertex 3
	1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	
};

Vertex cube_vertices[] = {
	// Front: triangle 1
	// vertex 1
	-0.5f, 0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, -0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	

	// triangle 2
	// vertex 1
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, -0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	0.5f, -0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	

	// Right: triangle 3
	// vertex 1
	0.5f, 0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	0.5f, -0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	0.5f, 0.5f, -0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	

	// triangle 4
	// vertex 1
	0.5f, 0.5f, -0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	0.5f, -0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	0.5f, -0.5f, -0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	

	// Back: triangle 5
	// vertex 1
	0.5f, -0.5f, -0.5f,	// position
	0.0f, 0.0f, -1.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, -0.5f, -0.5f,// position
	0.0f, 0.0f, -1.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 0.0f, -1.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	

	// triangle 6
	// vertex 1
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 0.0f, -1.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, -0.5f, -0.5f,// position
	0.0f, 0.0f, -1.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	-0.5f, 0.5f, -0.5f,	// position
	0.0f, 0.0f, -1.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	

	// Left: triangle 7
	// vertex 1
	-0.5f, -0.5f, 0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, 0.5f, 0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	
	// vertex 3
	-0.5f, -0.5f, -0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	

	// triangle 8
	// vertex 1
	-0.5f, -0.5f, -0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, 0.5f, 0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	
	// vertex 3
	-0.5f, 0.5f, -0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	

	// Top: triangle 9
	// vertex 1
	-0.5f, 0.5f, 0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	
	// vertex 2
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	-0.5f, 0.5f, -0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	

	// triangle 10
	// vertex 1
	-0.5f, 0.5f, -0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	
	// vertex 3
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	

	// Bottom: triangle 11
	// vertex 1
	0.5f, -0.5f, 0.5f,	// position
	0.0f, -1.0f, 0.0f,	// normal
	1.0f, 1.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, -0.5f, 0.5f,	// position
	0.0f, -1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	
	// vertex 3
	0.5f, -0.5f, -0.5f,	// position
	0.0f, -1.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	

	// triangle 12
	// vertex 1
	0.5f, -0.5f, -0.5f,	// position
	0.0f, -1.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	
	// vertex 2
	-0.5f, -0.5f, 0.5f,	// position
	0.0f, -1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	
	// vertex 3
	-0.5f, -0.5f, -0.5f,// position
	0.0f, -1.0f, 0.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	

};


//Global Vars
const int vbo_vao_number = 8; //needed to make sure I clean up everything properly



GLuint g_IBO[vbo_vao_number];			// index buffer object identifier
GLuint g_VBO[vbo_vao_number];
GLuint g_VAO[vbo_vao_number];
GLuint g_shaderProgramID = 0;
GLuint textureLight_shaderProgramID = 0;
const int shaderNumber = 2;
GLuint g_MVP_Index[shaderNumber];
GLuint g_MV_Index[shaderNumber];
GLuint g_V_Index[shaderNumber];
GLuint g_texSamplerIndex[shaderNumber];
GLuint g_normalSamplerIndex[shaderNumber];

GLuint g_lightPositionIndex[shaderNumber];
GLuint g_lightAmbientIndex[shaderNumber];
GLuint g_lightDiffuseIndex[shaderNumber];
GLuint g_lightSpecularIndex[shaderNumber];
GLuint g_lightDirectionIndex[shaderNumber];
GLuint g_lightTypeIndex[shaderNumber];
GLuint g_materialAmbientIndex[shaderNumber];
GLuint g_materialDiffuseIndex[shaderNumber];
GLuint g_materialSpecularIndex[shaderNumber];
GLuint g_materialShininessIndex[shaderNumber];

glm::mat4 floorMatrix; //floors matrix
glm::mat4 wall_modelMatrix[4]; //wall matrix
glm::mat4 painting_modelMatrix[2]; //painting matrix
glm::mat4 cube_modelMatrix[1];

Light g_lightPoint;				// light properties
Light g_lightDirectional;		// light properties
Material wall_material;			// wall material properties

glm::mat4 g_viewMatrix;
glm::mat4 g_projectionMatrix;

static int screenshotNum = 0;

double frameTime = 0.0f;				// frame time

Camera g_camera;
bool g_moveCamera = false;
bool g_centreCursor = false;

GLuint g_windowWidth = 800; //window dimensions
GLuint g_windowHeight = 600;

unsigned char* wall_texImage[2];	//image data
GLuint wall_textureID[2];			//texture id

//floor texture
unsigned char* floor_texImage[1]; //image data
GLuint floor_textureID[1];

//painting texture
unsigned char* painting_texImage[2];
GLuint painting_textureID[2];

//pedestal texture
unsigned char* pedestal_texImage;
GLuint pedestal_textureID[1];


//tw vars
float lightx;
float lighty;
float lightz;
bool wireFrame = false;




//mesh load function as seen in tuts
bool load_mesh(const char* fileName, Mesh* mesh) {
	// load file with assimp
	const aiScene* pScene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals
		| aiProcess_JoinIdenticalVertices);


	//checks whether the scene was loaded
	if (!pScene) {
		cout << "Could not load mesh." << endl;
		return false;
	}

	//get pointer to mesh 0
	const aiMesh* pMesh = pScene->mMeshes[0];

	//store number of mesh vertices
	mesh->numberOfVertices = pMesh->mNumVertices;

	// if mesh contains vertex coordinates

	if (pMesh->HasPositions()) {
		mesh->pMeshVertices = new Vertex[pMesh->mNumVertices];

		for (int i = 0; i < pMesh->mNumVertices; i++) {
			const aiVector3D* pVertexPos = &(pMesh->mVertices[i]);

			mesh->pMeshVertices[i].position[0] = (GLfloat)pVertexPos->x;
			mesh->pMeshVertices[i].position[1] = (GLfloat)pVertexPos->y;
			mesh->pMeshVertices[i].position[2] = (GLfloat)pVertexPos->z;

		}
	}

	// if mesh contains normals
	if (pMesh->HasNormals())
	{
		// read normals and store in the array
		for (int i = 0; i < pMesh->mNumVertices; i++)
		{
			const aiVector3D* pVertexNormal = &(pMesh->mNormals[i]);

			mesh->pMeshVertices[i].normal[0] = (GLfloat)pVertexNormal->x;
			mesh->pMeshVertices[i].normal[1] = (GLfloat)pVertexNormal->y;
			mesh->pMeshVertices[i].normal[2] = (GLfloat)pVertexNormal->z;
		}
	}

	// if mesh contains faces
	if (pMesh->HasFaces())
	{
		// store number of mesh faces
		mesh->numberOfFaces = pMesh->mNumFaces;

		// allocate memory for vertices
		mesh->pMeshIndices = new GLint[pMesh->mNumFaces * 3];

		// read normals and store in the array
		for (int i = 0; i < pMesh->mNumFaces; i++)
		{
			const aiFace* pFace = &(pMesh->mFaces[i]);

			mesh->pMeshIndices[i * 3] = (GLint)pFace->mIndices[0];
			mesh->pMeshIndices[i * 3 + 1] = (GLint)pFace->mIndices[1];
			mesh->pMeshIndices[i * 3 + 2] = (GLint)pFace->mIndices[2];
		}
	}

	//release the scene
	aiReleaseImport(pScene);

	return true;


}


static void init(GLFWwindow* window) {
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	lightx = 1.0f;
	lighty = 1.0f;
	lightz = 1.0f;
	//create and compile our GLSL programs from the shader files
	g_shaderProgramID = loadShaders("NormalMapVS.vert", "NormalMapFS.frag");
	textureLight_shaderProgramID = loadShaders("LightAndTextureVS.vert","LightAndTextureFS.frag" );
	GLuint positionIndex[shaderNumber];
	GLuint normalIndex[shaderNumber];
	GLuint texCoordIndex[shaderNumber];

	//find the location of shader vars for first program
	positionIndex[0] = glGetAttribLocation(g_shaderProgramID, "aPosition");
	normalIndex[0] = glGetAttribLocation(g_shaderProgramID, "aNormal");
	GLuint tangentIndex = glGetAttribLocation(g_shaderProgramID, "aTangent");
	texCoordIndex[0] = glGetAttribLocation(g_shaderProgramID, "aTexCoord");

	

	g_MVP_Index[0] = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index[0] = glGetUniformLocation(g_shaderProgramID, "uModelViewMatrix");
	g_V_Index[0] = glGetUniformLocation(g_shaderProgramID, "uViewMatrix");

	g_texSamplerIndex[0] = glGetUniformLocation(g_shaderProgramID, "uTextureSampler");
	g_normalSamplerIndex[0] = glGetUniformLocation(g_shaderProgramID, "uNormalSampler");


	g_lightPositionIndex[0] = glGetUniformLocation(g_shaderProgramID, "uLight.position");
	g_lightDirectionIndex[0] = glGetUniformLocation(g_shaderProgramID, "uLight.direction");
	g_lightAmbientIndex[0] = glGetUniformLocation(g_shaderProgramID, "uLight.ambient");
	g_lightDiffuseIndex[0] = glGetUniformLocation(g_shaderProgramID, "uLight.diffuse");
	g_lightSpecularIndex[0] = glGetUniformLocation(g_shaderProgramID, "uLight.specular");
	g_lightTypeIndex[0] = glGetUniformLocation(g_shaderProgramID, "uLight.type");

	g_materialAmbientIndex[0] = glGetUniformLocation(g_shaderProgramID, "uMaterial.ambient");
	g_materialDiffuseIndex[0] = glGetUniformLocation(g_shaderProgramID, "uMaterial.diffuse");
	g_materialSpecularIndex[0] = glGetUniformLocation(g_shaderProgramID, "uMaterial.specular");
	g_materialShininessIndex[0] = glGetUniformLocation(g_shaderProgramID, "uMaterial.shininess");


	//for texture light shader

	positionIndex[1] = glGetAttribLocation(textureLight_shaderProgramID, "aPosition");
	normalIndex[1] = glGetAttribLocation(textureLight_shaderProgramID, "aNormal");
	texCoordIndex[1] = glGetAttribLocation(textureLight_shaderProgramID, "aTexCoord");
	g_MVP_Index[1] = glGetUniformLocation(textureLight_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index[1] = glGetUniformLocation(textureLight_shaderProgramID, "uModelViewMatrix");
	g_V_Index[1] = glGetUniformLocation(textureLight_shaderProgramID, "uViewMatrix");

	g_texSamplerIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uTextureSampler");

	g_lightPositionIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uLight.position");
	g_lightAmbientIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uLight.ambient");
	g_lightDiffuseIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uLight.diffuse");
	g_lightSpecularIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uLight.specular");

	g_materialAmbientIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uMaterial.ambient");
	g_materialDiffuseIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uMaterial.diffuse");
	g_materialSpecularIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uMaterial.specular");
	g_materialShininessIndex[1] = glGetUniformLocation(textureLight_shaderProgramID, "uMaterial.shininess");



	//init model matrices
	floorMatrix = glm::mat4(1.0f);
	wall_modelMatrix[0] = glm::mat4(1.0f);
	wall_modelMatrix[1] = glm::mat4(1.0f);
	wall_modelMatrix[2] = glm::mat4(1.0f);
	wall_modelMatrix[3] = glm::mat4(1.0f);
	painting_modelMatrix[0] = glm::mat4(1.0f);
	painting_modelMatrix[1] = glm::mat4(1.0f);
	cube_modelMatrix[0] = glm::mat4(1.0f);
	//init view matrix
	

	int width;
	int height;

	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;
	g_camera.setViewMatrix(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	g_camera.setProjection(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// initialise point light properties

	g_lightPoint.position = glm::vec3(lightx, lighty, lightz);
	g_lightPoint.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.type = 0;

	// initialise material properties
	wall_material.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	wall_material.diffuse = glm::vec3(0.2f, 0.7f, 1.0f);
	wall_material.specular = glm::vec3(0.2f, 0.7f, 1.0f);
	wall_material.shininess = 40.0f;

	// read the image data
	GLint imageWidth[6];			//image width info
	GLint imageHeight[6];			//image height info
	wall_texImage[0] = readBitmapRGBImage("images/Fieldstone.bmp", &imageWidth[0], &imageHeight[0]);
	wall_texImage[1] = readBitmapRGBImage("images/FieldstoneBumpDOT3.bmp", &imageWidth[1], &imageHeight[1]);
	floor_texImage[0] = readBitmapRGBImage("images/check.bmp", &imageWidth[2], &imageHeight[2]);
	painting_texImage[0] = readBitmapRGBImage("images/painting1.bmp", &imageWidth[3], &imageHeight[3]);
	painting_texImage[1] = readBitmapRGBImage("images/tile4.bmp", &imageWidth[4], &imageHeight[4]);
	pedestal_texImage = readBitmapRGBImage("images/smile.bmp", &imageWidth[5], &imageHeight[5]);
	// generate identifier for wall texture object and set wall texture properties
	glGenTextures(2, wall_textureID);
	glBindTexture(GL_TEXTURE_2D, wall_textureID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[0], imageHeight[0], 0, GL_BGR, GL_UNSIGNED_BYTE, wall_texImage[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, wall_textureID[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[1], imageHeight[1], 0, GL_BGR, GL_UNSIGNED_BYTE, wall_texImage[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//do the same for the floor texture
	glGenTextures(1, floor_textureID);
	glBindTexture(GL_TEXTURE_2D, floor_textureID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[2], imageHeight[2], 0, GL_BGR, GL_UNSIGNED_BYTE, floor_texImage[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(2, painting_textureID);
	glBindTexture(GL_TEXTURE_2D, painting_textureID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[3], imageHeight[3], 0, GL_BGR, GL_UNSIGNED_BYTE, painting_texImage[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, painting_textureID[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[4], imageHeight[4], 0, GL_BGR, GL_UNSIGNED_BYTE, painting_texImage[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, pedestal_textureID);
	glBindTexture(GL_TEXTURE_2D, pedestal_textureID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[5], imageHeight[5], 0, GL_BGR, GL_UNSIGNED_BYTE, pedestal_texImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glGenBuffers(vbo_vao_number, g_VBO);
	glGenVertexArrays(vbo_vao_number, g_VAO);
	
	//generate walls
	for (int i = 0; i < 4; i++) {
		
		glBindBuffer(GL_ARRAY_BUFFER, g_VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(wall_vertices), wall_vertices, GL_STATIC_DRAW);
		

		glBindVertexArray(g_VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_VBO[i]);
		glVertexAttribPointer(positionIndex[0], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		glVertexAttribPointer(normalIndex[0], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glVertexAttribPointer(tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
		glVertexAttribPointer(texCoordIndex[0], 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

		glEnableVertexAttribArray(positionIndex[0]);	// enable vertex attributes
		glEnableVertexAttribArray(normalIndex[0]);
		glEnableVertexAttribArray(tangentIndex);
		glEnableVertexAttribArray(texCoordIndex[0]);
	}
	
	//setting up floor
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);
	glBindVertexArray(g_VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[4]);
	glVertexAttribPointer(positionIndex[1], 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, position)));
	glVertexAttribPointer(normalIndex[1], 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, normal)));
	//glVertexAttribPointer(tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	glVertexAttribPointer(texCoordIndex[1], 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, texCoord)));

	glEnableVertexAttribArray(positionIndex[1]);	// enable vertex attributes
	glEnableVertexAttribArray(normalIndex[1]);
	//glEnableVertexAttribArray(tangentIndex);
	glEnableVertexAttribArray(texCoordIndex[1]);
	//making paintings
	for (int i = 5; i < 7; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, g_VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(painting_vertices), painting_vertices, GL_STATIC_DRAW);
		glBindVertexArray(g_VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_VBO[i]);
		glVertexAttribPointer(positionIndex[1], 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, position)));
		glVertexAttribPointer(normalIndex[1], 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, normal)));
		//glVertexAttribPointer(tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
		glVertexAttribPointer(texCoordIndex[1], 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, texCoord)));

		glEnableVertexAttribArray(positionIndex[1]);	// enable vertex attributes
		glEnableVertexAttribArray(normalIndex[1]);
		//glEnableVertexAttribArray(tangentIndex);
		glEnableVertexAttribArray(texCoordIndex[1]);

	}

	//make pedestal
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glBindVertexArray(g_VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[7]);
	glVertexAttribPointer(positionIndex[1], 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, position)));
	glVertexAttribPointer(normalIndex[1], 3, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, normal)));
	//glVertexAttribPointer(tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	glVertexAttribPointer(texCoordIndex[1], 2, GL_FLOAT, GL_FALSE, sizeof(simpleVertex), reinterpret_cast<void*>(offsetof(simpleVertex, texCoord)));

	glEnableVertexAttribArray(positionIndex[1]);	// enable vertex attributes
	glEnableVertexAttribArray(normalIndex[1]);
	//glEnableVertexAttribArray(tangentIndex);
	glEnableVertexAttribArray(texCoordIndex[1]);

	


	//Constructing House

	//put walls in position
	//the side walls
	wall_modelMatrix[1] = glm::rotate(wall_modelMatrix[1],radians(90.0f),glm::vec3(0.0f, -1.0f, 0.0f));
	wall_modelMatrix[1] = glm::translate(wall_modelMatrix[1], glm::vec3(1.0f, 0.0f, 1.0f));
	wall_modelMatrix[2] = glm::rotate(wall_modelMatrix[2], radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	wall_modelMatrix[2] = glm::translate(wall_modelMatrix[2], glm::vec3(1.0f, 0.0f, -4.0f));

	//the end wall
	wall_modelMatrix[3] = glm::translate(wall_modelMatrix[3], glm::vec3(0.0f, 0.0f, 5.0f));

	//putting the floor in position
	floorMatrix = glm::rotate(floorMatrix, radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	floorMatrix = glm::translate(floorMatrix, vec3(1.5f, 2.5f, 1.0f));

	//paintings
	painting_modelMatrix[0] *= glm::scale(vec3(0.50f, 0.50f, 0.50f));
	painting_modelMatrix[1] *= glm::scale(vec3(0.50f, 0.50f, 0.50f));
	painting_modelMatrix[1] = glm::translate(painting_modelMatrix[1], vec3(6.0f, 0.0f, 0.05f));
	painting_modelMatrix[0] *= glm::rotate(painting_modelMatrix[0], radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	painting_modelMatrix[0] = glm::translate(painting_modelMatrix[0], vec3(-5.0f, 0.0f, -3.9f));
	
	//pedestal
	cube_modelMatrix[0] *= glm::scale(vec3(0.75f, 0.75f, 0.75f));
	cube_modelMatrix[0] = glm::translate(cube_modelMatrix[0], vec3(2.0f, -0.8f, 3.0f));


}


// function used to update the scene

static void update_scene(GLFWwindow* window) {
	

	g_lightPoint.position = glm::vec3(lightx, lighty, lightz);
	// variables to store forward/back and strafe movement
	float moveForward = 0;
	float strafeRight = 0;

	// update movement variables based on keyboard input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveForward += 1 * MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveForward -= 1 * MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		strafeRight -= 1 * MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		strafeRight += 1 * MOVEMENT_SENSITIVITY * frameTime;

	g_camera.update(moveForward, strafeRight);	// update camera
}


void draw_cube() {
	glUseProgram(textureLight_shaderProgramID);
	glm::mat4 MVP;
	glm::mat4 MV;
	glm::mat4 V;

	//render cube
	glBindVertexArray(g_VAO[7]);		// make VAO active

		// set uniform shader variables
	MVP = g_camera.getProjectionMatrix() * g_camera.getViewMatrix() * cube_modelMatrix[0] ;
	MV = g_camera.getViewMatrix() * cube_modelMatrix[0];
	V = g_camera.getViewMatrix();
	glUniformMatrix4fv(g_MVP_Index[1], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index[1], 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(g_V_Index[1], 1, GL_FALSE, &V[0][0]);

	glUniform3fv(g_lightPositionIndex[1], 1, &g_lightPoint.position[0]);
	glUniform3fv(g_lightAmbientIndex[1], 1, &g_lightPoint.ambient[0]);
	glUniform3fv(g_lightDiffuseIndex[1], 1, &g_lightPoint.diffuse[0]);
	glUniform3fv(g_lightSpecularIndex[1], 1, &g_lightPoint.specular[0]);
	//glUniform1i(g_lightTypeIndex[0], g_lightPoint.type);

	glUniform3fv(g_materialAmbientIndex[1], 1, &wall_material.ambient[0]);
	glUniform3fv(g_materialDiffuseIndex[1], 1, &wall_material.diffuse[0]);
	glUniform3fv(g_materialSpecularIndex[1], 1, &wall_material.specular[0]);
	glUniform1fv(g_materialShininessIndex[1], 1, &wall_material.shininess);

	glUniform1i(g_texSamplerIndex[1], 0);
	glUniform1i(g_normalSamplerIndex[1], 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,pedestal_textureID[0]);



	glDrawArrays(GL_TRIANGLES, 0, 36);

	//end draw floor


}


void draw_painting() {
	glUseProgram(textureLight_shaderProgramID);
	glm::mat4 MVP;
	glm::mat4 MV;
	glm::mat4 V;
	//render paintings

	for (int i = 0; i < 2; i++) {
		glBindVertexArray(g_VAO[i+5]);		// make VAO active

		// set uniform shader variables
		MVP = g_camera.getProjectionMatrix() * g_camera.getViewMatrix() * painting_modelMatrix[i];
		MV = g_camera.getViewMatrix() * painting_modelMatrix[i];
		V = g_camera.getViewMatrix();
		glUniformMatrix4fv(g_MVP_Index[1], 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(g_MV_Index[1], 1, GL_FALSE, &MV[0][0]);
		glUniformMatrix4fv(g_V_Index[1], 1, GL_FALSE, &V[0][0]);

		glUniform3fv(g_lightPositionIndex[1], 1, &g_lightPoint.position[0]);
		glUniform3fv(g_lightAmbientIndex[1], 1, &g_lightPoint.ambient[0]);
		glUniform3fv(g_lightDiffuseIndex[1], 1, &g_lightPoint.diffuse[0]);
		glUniform3fv(g_lightSpecularIndex[1], 1, &g_lightPoint.specular[0]);
		//glUniform1i(g_lightTypeIndex[1], g_lightPoint.type);

		glUniform3fv(g_materialAmbientIndex[1], 1, &wall_material.ambient[0]);
		glUniform3fv(g_materialDiffuseIndex[1], 1, &wall_material.diffuse[0]);
		glUniform3fv(g_materialSpecularIndex[1], 1, &wall_material.specular[0]);
		glUniform1fv(g_materialShininessIndex[1], 1, &wall_material.shininess);

		glUniform1i(g_texSamplerIndex[1], 0);
		glUniform1i(g_normalSamplerIndex[1], 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, painting_textureID[i]);

		

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}


void draw_walls() {
	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glm::mat4 MVP;
	glm::mat4 MV;
	glm::mat4 V;
	//render walls

	for (int i = 0; i < 4; i++) {
		glBindVertexArray(g_VAO[i]);		// make VAO active

		// set uniform shader variables
		MVP = g_camera.getProjectionMatrix() * g_camera.getViewMatrix() * wall_modelMatrix[i];
		MV = g_camera.getViewMatrix() * wall_modelMatrix[i];
		V = g_camera.getViewMatrix();
		glUniformMatrix4fv(g_MVP_Index[0], 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(g_MV_Index[0], 1, GL_FALSE, &MV[0][0]);
		glUniformMatrix4fv(g_V_Index[0], 1, GL_FALSE, &V[0][0]);

		glUniform3fv(g_lightPositionIndex[0], 1, &g_lightPoint.position[0]);
		glUniform3fv(g_lightAmbientIndex[0], 1, &g_lightPoint.ambient[0]);
		glUniform3fv(g_lightDiffuseIndex[0], 1, &g_lightPoint.diffuse[0]);
		glUniform3fv(g_lightSpecularIndex[0], 1, &g_lightPoint.specular[0]);
		glUniform1i(g_lightTypeIndex[0], g_lightPoint.type);

		glUniform3fv(g_materialAmbientIndex[0], 1, &wall_material.ambient[0]);
		glUniform3fv(g_materialDiffuseIndex[0], 1, &wall_material.diffuse[0]);
		glUniform3fv(g_materialSpecularIndex[0], 1, &wall_material.specular[0]);
		glUniform1fv(g_materialShininessIndex[0], 1, &wall_material.shininess);

		glUniform1i(g_texSamplerIndex[0], 0);
		glUniform1i(g_normalSamplerIndex[0], 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wall_textureID[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, wall_textureID[1]);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}

void draw_floor() {
	glUseProgram(textureLight_shaderProgramID);
	
	glm::mat4 MVP;
	glm::mat4 MV;
	glm::mat4 V;

	// draw floor
	glBindVertexArray(g_VAO[4]);		// make VAO active

		// set uniform shader variables
	MVP = g_camera.getProjectionMatrix() * g_camera.getViewMatrix() * floorMatrix;
	MV = g_camera.getViewMatrix() * floorMatrix;
	V = g_camera.getViewMatrix();
	glUniformMatrix4fv(g_MVP_Index[1], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index[1], 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(g_V_Index[1], 1, GL_FALSE, &V[0][0]);

	glUniform3fv(g_lightPositionIndex[1], 1, &g_lightPoint.position[0]);
	glUniform3fv(g_lightAmbientIndex[1], 1, &g_lightPoint.ambient[0]);
	glUniform3fv(g_lightDiffuseIndex[1], 1, &g_lightPoint.diffuse[0]);
	glUniform3fv(g_lightSpecularIndex[1], 1, &g_lightPoint.specular[0]);
	//glUniform1i(g_lightTypeIndex[0], g_lightPoint.type);

	glUniform3fv(g_materialAmbientIndex[1], 1, &wall_material.ambient[0]);
	glUniform3fv(g_materialDiffuseIndex[1], 1, &wall_material.diffuse[0]);
	glUniform3fv(g_materialSpecularIndex[1], 1, &wall_material.specular[0]);
	glUniform1fv(g_materialShininessIndex[1], 1, &wall_material.shininess);

	glUniform1i(g_texSamplerIndex[1], 0);
	glUniform1i(g_normalSamplerIndex[1], 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floor_textureID[0]);

	

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//end draw floor



}

static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer


	draw_floor();

	draw_walls();
	
	draw_painting();
	
	draw_cube();

	glFlush();
}


//various callback funcs

//key callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// quit if the ESCAPE key was press
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	// if the 0 key is pressed that a screen capture
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		// compute size of the image
		int size = g_windowWidth * g_windowHeight * 3;

		// allocate memory to store the image
		unsigned char* outBuffer = new unsigned char[size];

		// read pixels from the colour buffer
		glReadPixels(0, 0, g_windowWidth, g_windowHeight, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)outBuffer);

		// write the image to a file
		string intStr = to_string(screenshotNum);
		string screenshotname = "images/screenshot" + intStr + ".bmp";
		const char* cstr = screenshotname.c_str();
		screenshotNum++;
		writeBitmapRGBImage(cstr, (char*)outBuffer, g_windowWidth, g_windowHeight);

		// deallocate memory
		delete[] outBuffer;
	}
}

// error callback function
static void error_callback(int error, const char* description)
{
	cerr << description << endl;	// output error description
}






static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass mouse data to tweak bar
	TwEventMousePosGLFW(xpos, ypos);
	// variables to store mouse cursor coordinates
	static double previous_xpos = xpos;
	static double previous_ypos = ypos;
	double delta_x = previous_xpos - xpos;
	double delta_y = previous_ypos - ypos;

	if (g_moveCamera)
	{
		if (!g_centreCursor) // ignore camera update the first time mouse cursor is centred
		{
			// pass mouse movement to camera class to update its yaw and pitch
			g_camera.updateRotation(delta_x * ROTATION_SENSITIVITY * frameTime, delta_y * ROTATION_SENSITIVITY * frameTime);
		}
		else
		{
			g_centreCursor = false;
		}
	}

	// update previous mouse coordinates
	previous_xpos = xpos;
	previous_ypos = ypos;

	// pass mouse data to tweak bar
	TwEventMousePosGLFW(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	TwEventMouseButtonGLFW(button, action);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		// use mouse to move camera, hence use disable cursor mode
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		g_centreCursor = true;	// mouse cursor position is centred
		g_moveCamera = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		// use mouse to move camera, hence use disable cursor mode
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		g_moveCamera = false;
	}
}



//main method
int main(void)
{
	GLFWwindow* window = NULL;	// pointer to a GLFW window handle
	TwBar* TweakBar;

	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time elapsed since last update
	
	int frameCount = 0;						// number of frames since last update
	int FPS = 0;							// frames per second

	glfwSetErrorCallback(error_callback);	// set error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Assignment 3", NULL, NULL);

	// if failed to create window
	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		cerr << "GLEW initialisation failed" << endl;
		exit(EXIT_FAILURE);
	}

	// set key callback function
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	//initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	//give tweak bar the size of graphics window
	TwWindowSize(g_windowWidth, g_windowHeight);
	TwDefine("TW_HELP visible=false ");
	TwDefine("GLOBAL fontsize = 3");

	//create a tweak bar
	TweakBar = TwNewBar("Main");
	TwDefine("Main label = 'Assignment 2 GUI' refresh = 0.02 text = light size = '220 500'");

	// create display entries
	//create Display
	TwAddVarRW(TweakBar, "Wireframe", TW_TYPE_BOOLCPP, &wireFrame, " group='Display' ");

	//create Frame Statistics
	TwAddVarRW(TweakBar, "FPS", TW_TYPE_INT32, &FPS, "group = Frame");
	TwAddVarRW(TweakBar, "Frame Time", TW_TYPE_DOUBLE, &frameTime, "group = Frame precision = 4");

	//Create Light Frame
	TwAddVarRW(TweakBar, "Position x", TW_TYPE_FLOAT, &lightx, "group = Light");
	TwAddVarRW(TweakBar, "Position y", TW_TYPE_FLOAT, &lighty, "group = Light");
	TwAddVarRW(TweakBar, "Position z", TW_TYPE_FLOAT, &lightz, "group = Light");

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window);		// update the scene

		if (wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene();		// render the scene

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		TwDraw();
		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
		
		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// current time - last update time

		if (elapsedTime >= 1.0f)	// if time since last update >= to 1 second
		{
			frameTime = 1.0f / frameCount;	// calculate frame time

			string str = "FPS = " + to_string(frameCount) + "; FT = " + to_string(frameTime);

			glfwSetWindowTitle(window, str.c_str());	// update window title

			FPS = frameCount;
			frameCount = 0;					// reset frame count
			lastUpdateTime += elapsedTime;	// update last update time
		}
	}

	// clean up
	

	


	glDeleteProgram(g_shaderProgramID);
	glDeleteProgram(textureLight_shaderProgramID);
	glDeleteBuffers(vbo_vao_number, g_IBO);
	glDeleteBuffers(vbo_vao_number, g_VBO);
	glDeleteVertexArrays(vbo_vao_number, g_VAO);

	// uninitialise tweak bar
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
