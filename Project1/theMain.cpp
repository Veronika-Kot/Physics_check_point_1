#include "globalStuff.h"
//#include "linmath.h" //*** replace it with glm

//include glm to draw multiple objects
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <vector>
#include <stdlib.h> //<> are system include
#include <stdio.h>
#include "cShaderManager.h" 
#include <iostream>
#include "cVAOMeshManager.h"
#include "cMeshObject.h"
#include "sMOdelDrawInfo.h"

std::vector<cMeshObject* > g_vecObjectsToDraw;
///can access vecObjectsToDraw[1] too
cMeshObject* pRogerRabbit = NULL;
glm::vec3 g_CameraEye = glm::vec3(0.0, 0.0, -20.0f);
glm::vec3 g_CameraAt = glm::vec3(0.0, 0.0, 0.0f);
glm::vec3 g_lightPos = glm::vec3(4.0f, 4.0f, 0.0f);

float g_lightBrightness = 400000.0f;

cShaderManager* pTheShaderManager; 
cVAOMeshManager* g_pTheVAOMeshManager;



void LoadModelsIntoScene(void);

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
	GLFWwindow* window; ///creates window of app and free console

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1300, 880, "Banny", NULL, NULL);
	///(W, H, title, ?, ?)

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	//********************************At this point GLFM is happy and has moved to OpenGL***********************************

	//***Create a shader manager
	pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("myShader", vertexShader, fragmentShader)) 
	{
		std::cout << "Compiled shaders OK" << std::endl;
	}
	else {
		std::cout << "OHH No" << std::endl;
		std::cout << pTheShaderManager->getLastError()<< std::endl;
	}
    //***End 

	//***creating a chader program
	GLuint program = pTheShaderManager->getIDFromFriendlyName("myShader"); //glUseProgram(program);...


	//*******Loading Mesh Models
	g_pTheVAOMeshManager = new cVAOMeshManager();

	sModelDrawInfo bunnyInfo;
	bunnyInfo.meshFileName = "bun_res3_xyz.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(bunnyInfo, program)) {
		std::cout << "Didn't load the bunny" << std::endl;
		std::cout << pTheShaderManager ->getLastError() << std::endl;
	}
	else {
		std::cout << "Bunny is Loaded" << std::endl;
	}

	sModelDrawInfo airpalneInfo;
	airpalneInfo.meshFileName = "mig29xyz.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(airpalneInfo, program)) {
		std::cout << "Didn't load the airplane 1" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo fishInfo;
	fishInfo.meshFileName = "PacificCod0.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(fishInfo, program)) {
		std::cout << "Didn't load the fish"<< std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo airpalneInfo2;
	airpalneInfo2.meshFileName = "ssj100xyz.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(airpalneInfo2, program)) {
		std::cout << "Didn't load the airplane 2" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo Utah;
	Utah.meshFileName = "Utah_Teapot.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(Utah, program)) {
		std::cout << "Didn't load the Utah Teapot" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo terrainInfo;
	terrainInfo.meshFileName = "MeshLab_Fractal_Terrain_xyz.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(terrainInfo, program)) {
		std::cout << "Didn't load the terrain" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_320_faces_xyz.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(sphereInfo, program)) {
		std::cout << "Didn't load the sphere" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo triangleInfo;
	triangleInfo.meshFileName = "SingleTriangle_xyz.ply";
	if (!g_pTheVAOMeshManager->LoadModelIntoVAO(triangleInfo, program)) {
		std::cout << "Didn't load the triangle" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	//****End of loading mesh models

	LoadModelsIntoScene();
	
	//******Loading uniform variables
	GLint objectColor_UniLoc = glGetUniformLocation(program, "objectColor");
	GLint lightPos_UniLoc = glGetUniformLocation(program, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(program, "lightBrightness");

	//***Unoform mat MVP
	///GLint mvp_location = glGetUniformLocation(program, "MVP"); //glUniformMatrix4fv(mvp_location,...
	GLint matMoldel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");


	//End of uniforms

	//there is a program logic - constantly refreshing the frame
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		//glm::mat4x4 m;
		///glm::mat4x4 view = glm::mat4(1.0f);
		///glm::mat4x4 p;
		///glm::mat4x4 mvp;
		///mat4x4 m, p, mvp; linmath model projection model-view-projection

		/*glm::mat4x4 matModel = glm::mat4(1.0f);*/			// mat4x4 m, p, mvp;
		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH); ///Enables the KEEPING of the depth information
		glEnable(GL_DEPTH_TEST); ///When drawing check the existing depth
		glEnable(GL_CULL_FACE); ///Discared "back facing" triangles 

		///Color and depth buffers are two different things
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ///clear the screen = refresh the frame, clear previous frame


		matProjection = glm::perspective(0.6f, ratio, 0.1f, 1000.0f);

		///position 3D camera
		matView = glm::lookAt(g_CameraEye, //EyE --- place camera in the world
			g_CameraAt, ///At --- Look at origin
			glm::vec3(0.0f, 1.0f, 0.0f));
		//mvp = p * view * m;

		//glUniformMatrix4fv(matMoldel_location, 1, GL_FALSE, glm::value_ptr(m));
		glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));
		//glm::mat4x4 matModel = glm::mat4(1.0f);
		//DrawElement(matModel, program);

		///Draw all the objects in the sceene
		for (unsigned int objIndex = 0;
			objIndex != (unsigned int)g_vecObjectsToDraw.size();
			objIndex++) 
		{
			glm::mat4x4 matModel = glm::mat4(1.0f);
			cMeshObject* pCurrentMesh = g_vecObjectsToDraw[objIndex];
			DrawObject(pCurrentMesh, matModel, program);

			
		} // for (unsigned int objIndex = 0; 


		//At this point the scene is drawn

		/*glfwSwapBuffers(window); ///swap windows, so we don't see actual drawing happenin
		glfwPollEvents();*/


		processKeys(window);
		
		//********************************* PHYSICS CHECK POINT **********************************

		//***get  VAO
		sModelDrawInfo theTerrainMesh;
		theTerrainMesh.meshFileName = "MeshLab_Fractal_Terrain_xyz.ply";
		//***theTerrainMesh.meshFileName = "SingleTriangle_xyz.ply";
		g_pTheVAOMeshManager->FindDrawInfoByModelName(theTerrainMesh);

		//****Get it the tracking object.
		cMeshObject* pTheBunny = findObjectByFriendlyName("mig");

		//std::vector<glm::vec3> vecClosestPoints;
		
		//***Closest point in the mesh
		glm::vec3 closestPoint;
		
		//***Vertices of triangle which containes the closest point
		glm::vec3 theVertices [3];

		//***Get the closest point and the vertices of the closest triangle 
		CalculateClosestPointOnMesh(theTerrainMesh, pTheBunny->position, closestPoint, theVertices);//vecClosestPoints);
		
		//***Place a green sphere at the closest point
		cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
		pDebugSphere->position = closestPoint;
		glm::mat4x4 matModel = glm::mat4(1.0f);
		DrawObject(pDebugSphere, matModel, program);

		//*** Place a new red triangle on the top of the closest triangle 
		glm::mat4x4 matModel1 = glm::mat4(1.0f);
		DrawElement(theVertices, matModel1, program);
		
		//********************************* THE END OF PHYSICS CHECK POINT **************************************

		glfwSwapBuffers(window); ///swap windows, so we don't see actual drawing happenin
		glfwPollEvents();


		///for (unsigned int index = 0; index != vecClosestPoints.size(); index++)
		///{
		///	//cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
		///	//pDebugSphere->position = vecClosestPoints[index];
		///	//glm::mat4x4 matModel = glm::mat4(1.0f);
		///	//DrawObject(pDebugSphere, matModel, program);

		///	glm::vec3 closestDot = glm::vec3(vecClosestPoints[index].x, vecClosestPoints[index].y, vecClosestPoints[index].z);
		///	glm::vec3 segment = pTheBunny->position - closestDot;

		///	float length = glm::length(segment);

		///	std::cout << index + 1 << " tr:"
		///		<< " x= " << vecClosestPoints[index].x
		///		<< " y= " << vecClosestPoints[index].y
		///		<< " z= " << vecClosestPoints[index].z 
		///		<< " length= "<< length << std::endl;
		///}

	} //while (!glfwWindowShouldClose(window))

	//****DELETING STUFF

		delete pTheShaderManager;
		delete g_pTheVAOMeshManager;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


void LoadModelsIntoScene(void) {

	//std::vector<cMeshObject* > vecObjectsToDraw;
	//**** set up some models to draw
	{
		cMeshObject* pMig = new cMeshObject();
		pMig->position = glm::vec3(-7.0f, 0.0f, 0.0f);
		pMig->objColour = glm::vec3(0.0f, 0.05f, 1.0f);
		//pMig->nonUniformScale = glm::vec3(0.7f, 0.7f, 0.7f);
		pMig->meshName = "mig29xyz.ply";
		pMig->friendlyName = "mig";
		g_vecObjectsToDraw.push_back(pMig);
	}
	{
		cMeshObject* pFish = new cMeshObject();
		pFish->position = glm::vec3(+7.0f, 0.0f, 0.0f);
		pFish->objColour = glm::vec3(0.0f, 1.1f, 0.0f);
		pFish->nonUniformScale = glm::vec3(5.0, 5.0, 5.0);
		pFish->bIsVisiable = false;
		pFish->meshName = "PacificCod0.ply";
		g_vecObjectsToDraw.push_back(pFish);
	}
	{
		cMeshObject* pPlain = new cMeshObject();
		pPlain->position = glm::vec3(0.0f, +7.0f, 0.0f);
		pPlain->objColour = glm::vec3(0.0f, 0.0f, 1.1f);
		pPlain->nonUniformScale = glm::vec3(3.0, 3.0, 3.0);
		pPlain->bIsVisiable = false;
		pPlain->meshName = "ssj100xyz.ply";
		g_vecObjectsToDraw.push_back(pPlain);
		pPlain->friendlyName = "Air plane";
	}
	{
		cMeshObject* pTeaPot = new cMeshObject();
		pTeaPot->position = glm::vec3(0.0f, -7.0f, 0.0f);
		pTeaPot->objColour = glm::vec3(0.19f, 0.6f, 0.3f);
		pTeaPot->nonUniformScale = glm::vec3(0.05f, 0.05f, 0.05f);
		pTeaPot->bIsVisiable = false;
		// pTeaPot-> bIsWireFrame = true;
		pTeaPot->meshName = "Utah_Teapot.ply";
		pTeaPot->friendlyName = "Tea pot";

		g_vecObjectsToDraw.push_back(pTeaPot);
	}

	{
		pRogerRabbit = new cMeshObject();
		pRogerRabbit->position = glm::vec3(0.0f, 0.0f, 0.0f);
		pRogerRabbit->objColour = glm::vec3(1.0f, 1.0f, 0.0f);
		pRogerRabbit->nonUniformScale = glm::vec3(8.0f, 8.0f, 8.0f);
		pRogerRabbit->bIsVisiable = false;
		pRogerRabbit->friendlyName = "Roger";
		pRogerRabbit->meshName = "bun_res3_xyz.ply";
		g_vecObjectsToDraw.push_back(pRogerRabbit);
	}

	{
		cMeshObject* pBunny = new cMeshObject();
		pBunny->position = glm::vec3(0.0f, 0.0f, 0.0f);
		pBunny->objColour = glm::vec3(1.0f, 1.0f, 0.0f);
		pBunny->nonUniformScale = glm::vec3(8.0f, 8.0f, 8.0f);
		pBunny->bIsVisiable = false;
		pBunny->friendlyName = "Bugs";
		pBunny->meshName = "bun_res3_xyz.ply";
		g_vecObjectsToDraw.push_back(pBunny);
	}

	{
		cMeshObject* pTerrain = new cMeshObject();
		pTerrain->position = glm::vec3(0.0f, -0.0f, 0.0f);
		pTerrain->objColour = glm::vec3(1.0f, 1.0f, 1.0f);
		pTerrain->meshName = "MeshLab_Fractal_Terrain_xyz.ply";
		pTerrain->bIsWireFrame = true;
		//pTerrain->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
		g_vecObjectsToDraw.push_back(pTerrain);
	}

	{
		cMeshObject* pDebugSphere = new cMeshObject();
		pDebugSphere->position = glm::vec3(0.0f, -30.0f, 0.0f);
		pDebugSphere->objColour = glm::vec3(0.0f, 1.0f, 0.0f);
		pDebugSphere->meshName = "Sphere_320_faces_xyz.ply";
		pDebugSphere->bIsWireFrame = true;
		pDebugSphere->bIsVisiable = true;
		pDebugSphere->nonUniformScale = glm::vec3(0.07f, 0.07f, 0.07f);
		//pTerrain->nonUniformScale = glm::vec3(0.1f,0.1f,0.1f);
		pDebugSphere->friendlyName = "DebugSphere";
		g_vecObjectsToDraw.push_back(pDebugSphere);
	}
	{
		cMeshObject* triangle = new cMeshObject();
		triangle->position = glm::vec3(0.0f, -0.0f, 0.0f);
		triangle->objColour = glm::vec3(1.0f, 1.0f, 0.0f);
		triangle->meshName = "SingleTriangle_xyz.ply";
		triangle->bIsWireFrame = true;
		triangle->bIsVisiable = false;
		triangle->friendlyName = "triangle";
		g_vecObjectsToDraw.push_back(triangle);
	}

	return;
}

cMeshObject* findObjectByFriendlyName(std::string theName) {

	cMeshObject* pTheObjectWeFound = NULL;

	for (unsigned int index = 0; index != g_vecObjectsToDraw.size(); index++)
	{
		if (g_vecObjectsToDraw[index]->friendlyName == theName) {
			pTheObjectWeFound = g_vecObjectsToDraw[index];
		}
	}

	return pTheObjectWeFound;
}
cMeshObject* findObjectByUniqueID(unsigned int ID) {
	for (unsigned int index = 0; index != g_vecObjectsToDraw.size(); index++)
	{
		if (g_vecObjectsToDraw[index]->getID() == ID) {
			return g_vecObjectsToDraw[index];
		}
	}

	return NULL;
}