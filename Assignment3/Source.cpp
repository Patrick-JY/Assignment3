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





//structs
//vertex stuct
typedef struct Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
} Vertex;

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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

typedef struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


//Global Vars
const int vbo_vao_number = 7; //needed to make sure I clean up everything properly

Mesh nucleusMesh;
Mesh electronMesh[3];
Mesh orbitPathMesh[3];

GLuint g_IBO[vbo_vao_number];			// index buffer object identifier
GLuint g_VBO[vbo_vao_number];
GLuint g_VAO[vbo_vao_number];
GLuint g_shaderProgramID = 0;
GLuint g_MVP_Index = 0;
GLuint g_MV_Index = 0;
GLuint g_V_Index = 0;


GLuint g_lightPositionIndex = 0;
GLuint g_lightAmbientIndex = 0;
GLuint g_lightDiffuseIndex = 0;
GLuint g_lightSpecularIndex = 0;


glm::mat4 g_viewMatrix;
glm::mat4 g_projectionMatrix;

Light g_light;	//light properties



GLuint g_windowWidth = 800; //window dimensions
GLuint g_windowHeight = 600;




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

	//create and compile our GLSL programs from the shader files
	g_shaderProgramID = loadShaders("VS.vert", "ColorFS.frag");


	//find the location of shader vars
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint normalIndex = glGetAttribLocation(g_shaderProgramID, "aNormal");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewMatrix");
	g_V_Index = glGetUniformLocation(g_shaderProgramID, "uViewMatrix");


	g_lightPositionIndex = glGetUniformLocation(g_shaderProgramID, "uLight.position");
	g_lightAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uLight.ambient");
	g_lightDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uLight.diffuse");
	g_lightSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uLight.specular");


	

	//init model matrices
	

	//init view matrix
	g_viewMatrix = lookAt(vec3(0.0f, 0.0f, 6.0f), vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 1.0f, 0.0f));

	int width;
	int height;

	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	//initialise projection matrix
	g_projectionMatrix = perspective(radians(45.0f), aspectRatio, 0.1f, 100.0f);
	//initialise vbo and vao
	glGenBuffers(vbo_vao_number, g_VBO);
	glGenVertexArrays(vbo_vao_number, g_VAO);
	glGenBuffers(vbo_vao_number, g_IBO);

	// initialise point light properties
	g_light.position = glm::vec3(10.0f, 10.0f, 10.0f);
	g_light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	g_light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	g_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);


}


// function used to update the scene

static void update_scene() {
	
}

static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

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
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	TwEventMouseButtonGLFW(button, action);
}



//main method
int main(void)
{
	GLFWwindow* window = NULL;	// pointer to a GLFW window handle
	TwBar* TweakBar;

	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time elapsed since last update
	double frameTime = 0.0f;				// frame time
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


	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene();		// update the scene

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
