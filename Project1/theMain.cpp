#include "globalStuff.h"
//#include "linmath.h" //*** replace it with glm

//include glm to draw multiple objects
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h> //<> are system include
#include <stdio.h>
#include "cShaderManager.h" 
#include <iostream>
#include "cMeshObject.h"
#include "cVAOMeshManager.h"

cMeshObject objectsToDraw[100];
unsigned int numberofObjectsToDraw = 0;

glm::vec3 g_CameraEye = glm::vec3(0.0, 0.0, -10.0f);
glm::vec3 g_CameraAt = glm::vec3(0.0, 0.0, 0.0f);


cShaderManager* pTheShaderManager; 
cVAOMeshManager* pTheVAOMeshManager;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, 
						int key, 
						int scancode, 
						int action, 
						int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	const float CAMERA_SPEED = 0.1f;
	//WASD + q = "up", e = "down"
	//y axis = up and down
	//x axis = left and dowm
	//z axis = forward and backward

	// ****  Z  ****
	if (key == GLFW_KEY_W && action == GLFW_PRESS) //Forward
	{
		//checked every frame
		g_CameraEye.z += CAMERA_SPEED; //Clear the screen - Draw the bunny - check for key = over and over again
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) //Bakward
	{
		//checked every frame
		g_CameraEye.z -= CAMERA_SPEED; //Clear the screen - Draw the bunny - check for key = over and over again
	}

	// ****  X  ****
	if (key == GLFW_KEY_A && action == GLFW_PRESS) //Right
	{
		//checked every frame
		g_CameraEye.x += CAMERA_SPEED; //Clear the screen - Draw the bunny - check for key = over and over again
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) //Left
	{
		//checked every frame
		g_CameraEye.x -= CAMERA_SPEED; //Clear the screen - Draw the bunny - check for key = over and over again
	}

	// ****  Y  ****
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) //Up
	{
		//checked every frame
		g_CameraEye.y += CAMERA_SPEED; //Clear the screen - Draw the bunny - check for key = over and over again
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) //Down
	{
		//checked every frame
		g_CameraEye.y -= CAMERA_SPEED; //Clear the screen - Draw the bunny - check for key = over and over again
	}

	return;
}

int main(void)
{
	GLFWwindow* window; //creates window of app and free console

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1300, 880, "Banny", NULL, NULL);
	//(W, H, title, ?, ?)

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

	//passing just created shader to OpenGL functions
	GLuint program = pTheShaderManager->getIDFromFriendlyName("myShader"); //glUseProgram(program);...
	GLint mvp_location = glGetUniformLocation(program, "MVP"); //glUniformMatrix4fv(mvp_location,...

	

	//*******Loading Mesh Models
	cVAOMeshManager* pTheVAOMeshManager = new cVAOMeshManager();

	sModelDrawInfo bunnyInfo;
	bunnyInfo.meshFileName = "bun_res3_xyz.ply";
	if (!pTheVAOMeshManager->LoadModelIntoVAO(bunnyInfo, program)) {
		std::cout << "Didn't load the bunny" << std::endl;
		std::cout << pTheShaderManager ->getLastError() << std::endl;
	}
	else {
		std::cout << "Bunny is Loaded" << std::endl;
	}

	sModelDrawInfo airpalneInfo;
	airpalneInfo.meshFileName = "mig29xyz.ply";
	if (!pTheVAOMeshManager->LoadModelIntoVAO(airpalneInfo, program)) {
		std::cout << "Didn't load the airplane 1" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo fishInfo;
	fishInfo.meshFileName = "PacificCod0.ply";
	if (!pTheVAOMeshManager->LoadModelIntoVAO(fishInfo, program)) {
		std::cout << "Didn't load the fish"<< std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo airpalneInfo2;
	airpalneInfo2.meshFileName = "ssj100xyz.ply";
	if (!pTheVAOMeshManager->LoadModelIntoVAO(airpalneInfo2, program)) {
		std::cout << "Didn't load the airplane 2" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}

	sModelDrawInfo Utah;
	Utah.meshFileName = "Utah_Teapot.ply";
	if (!pTheVAOMeshManager->LoadModelIntoVAO(Utah, program)) {
		std::cout << "Didn't load the Utah Teapot" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}
	//****End of loading mesh models

	//**** set up some models to draw
	objectsToDraw[0].position = glm::vec3( -3.0f, 0.0f, 0.0f );
	objectsToDraw[0].objColour = glm::vec3( 1.0f, 0.0f, 0.0f );
	objectsToDraw[0].nonUniformScale = glm::vec3(0.5f, 0.5f, 0.5f);
	objectsToDraw[0].meshName = "mig29xyz.ply";

	objectsToDraw[1].position = glm::vec3( +3.0f, 0.0f, 0.0f);
	objectsToDraw[1].objColour = glm::vec3(0.0f, 1.1f, 0.0f);
	objectsToDraw[1].meshName = "PacificCod0.ply";

	objectsToDraw[2].position = glm::vec3( 0.0f, +3.0f, 0.0f);
	objectsToDraw[2].objColour = glm::vec3(0.0f, 0.0f, 1.1f);
	objectsToDraw[2].meshName = "ssj100xyz.ply";

	objectsToDraw[3].position = glm::vec3(0.0f, -3.0f, 0.0f);
	objectsToDraw[3].objColour = glm::vec3(0.19f, 0.6f, 0.3f);
	objectsToDraw[3].nonUniformScale = glm::vec3(0.005f, 0.005f, 0.005f);
	objectsToDraw[3].meshName = "Utah_Teapot.ply";

	objectsToDraw[4].position = glm::vec3(0.0f, -1.0f, 0.0f);
	objectsToDraw[4].objColour = glm::vec3(1.0f, 1.0f, 1.0f);
	objectsToDraw[4].nonUniformScale = glm::vec3(3.0f, 3.0f, 3.0f);
	objectsToDraw[4].meshName = "bun_res3_xyz.ply";

	numberofObjectsToDraw = 5;

	//*****End of setting up models to draw

	//there is a program logic - constantly refreshing the frame
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glm::mat4x4 m;
		glm::mat4x4 view = glm::mat4(1.0f);
		glm::mat4x4 p;
		glm::mat4x4 mvp;
		//mat4x4 m, p, mvp; linmath model projection model-view-projection

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT); //clear the screen = refresh the frame, clear previous frame

		//Draw all the objects in the sceene
		for (unsigned int objIndex = 0;
			objIndex != numberofObjectsToDraw; 
			objIndex++) 
		{
			//************************************ glm transformation matrices **************************************

			m = glm::mat4x4(1.0f);		// mat4x4_identity(m);

			// Before positioning, rotating around a model's axes
			glm::mat4 preRot_X = glm::rotate(glm::mat4(1.0f),
				objectsToDraw[objIndex].preRotation.x,
				glm::vec3(1.0f, 0.0, 0.0f));
			m = m * preRot_X;

			glm::mat4 preRot_Y = glm::rotate(glm::mat4(1.0f),
				objectsToDraw[objIndex].preRotation.y,
				glm::vec3(0.0f, 1.0, 0.0f));
			m = m * preRot_Y;

			glm::mat4 preRot_Z = glm::rotate(glm::mat4(1.0f),
				objectsToDraw[objIndex].preRotation.z,
				glm::vec3(0.0f, 0.0, 1.0f));
			m = m * preRot_Z;

					//* constantly rotating around the axis
					//glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), //pathing the matrix
					//	(float)glfwGetTime(), //give the timea
					//	glm::vec3(0.0f, 0.0, 1.0f));

					//m = m * rotateZ;

			//Positioning model in the scene
			glm::mat4 matMove = glm::translate(glm::mat4(1.0f),
				objectsToDraw[objIndex].position);

			m = m * matMove;

			// After positioning, rotating around the scene's axes
			glm::mat4 postRot_X = glm::rotate(glm::mat4(1.0f),
				objectsToDraw[objIndex].postRotation.x,
				glm::vec3(1.0f, 0.0, 0.0f));
			m = m * postRot_X;

			glm::mat4 postRot_Y = glm::rotate(glm::mat4(1.0f),
				objectsToDraw[objIndex].postRotation.y,
				glm::vec3(0.0f, 1.0, 0.0f));
			m = m * postRot_Y;

			glm::mat4 postRot_Z = glm::rotate(glm::mat4(1.0f),
				objectsToDraw[objIndex].postRotation.z,
				glm::vec3(0.0f, 0.0, 1.0f));
			m = m * postRot_Z;

			// And now scale

			glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
				objectsToDraw[objIndex].nonUniformScale);
			m = m * matScale;

			////**************************** End of transformation matrices *******************************

		
			p = glm::perspective(0.6f, ratio, 0.1f, 1000.0f);

			//position 3D camera
			view = glm::lookAt(g_CameraEye, //EyE --- place camera in the world
			g_CameraAt, //At --- Look at origin
			glm::vec3(0.0f, 1.0f, 0.0f));//UP --- Y axis to be up


			mvp = p * view * m;  // mat4x4_mul(mvp, p, m); linmath multiplyin all 3 metrisis together 

			glUseProgram(program);
			glUniformMatrix4fv(mvp_location, //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp); linmath
								1,
								GL_FALSE,
								glm::value_ptr(mvp));


			//without below lines modal just filled with surface
			//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //draw it as a point at each vertex. Reads the data as triangles but draw them as points
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //draw a line between vertexes, GL_FILL is default

			GLint objectColor_UniLoc = glGetUniformLocation(program, "objectColor");
			glUniform3f(objectColor_UniLoc,
								objectsToDraw[objIndex].objColour.r,
								objectsToDraw[objIndex].objColour.g,
								objectsToDraw[objIndex].objColour.b);


			//******************Draw mesh models in the screen

			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = objectsToDraw[objIndex].meshName;

			if (pTheVAOMeshManager->FindDrawInfoByModelName(modelInfo)) 
			{
				//glDrawArrays(GL_TRIANGLES, 0, modelInfo.numberOfIndices);
				glBindVertexArray(modelInfo.VAO_ID);
				glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, 0);
				glBindVertexArray( 0 );
			}
			else {
				std::cout << "Can't draw the mesh" << std::endl;
			}
			//**********************End of drawing

		} // for (unsigned int objIndex = 0; 

		glfwSwapBuffers(window); //swap windows, so we don't see actual drawing happenin
		glfwPollEvents();

	} //while (!glfwWindowShouldClose(window))

	//DELETING STUFF

		delete pTheShaderManager;
		delete pTheVAOMeshManager;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
