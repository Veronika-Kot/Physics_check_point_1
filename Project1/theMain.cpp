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

using namespace std;

cMeshObject objectsToDraw[100];
unsigned int numberofObjectsToDraw = 0;

glm::vec3 g_CameraEye = glm::vec3(0.0, 0.0, +10.0f);
glm::vec3 g_CameraAt = glm::vec3(0.0, 0.0, 0.0f);


cShaderManager* pTheShaderManager; 

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

	window = glfwCreateWindow(1000, 580, "Banny", NULL, NULL);
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

	//***At this point GLFM is happy and has moved to OpenGL

	//***Create a shader manager
	pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("myShader",
		vertexShader,
		fragmentShader)) {

		cout << "Compiled shaders OK" << endl;

	}
	else {
		cout << "OHH No" << endl;
		cout << pTheShaderManager->getLastError()<< endl;
	}

	//GLuint vertex_buffer, vertex_shader, fragment_shader;//, program; 

	//GLuint vertex_buffer;  
	GLuint vertex_shader;
	GLuint fragment_shader;


	GLint mvp_location, vpos_location, vcol_location;

	// NOTE: OpenGL error checks have been omitted for brevity


	//Load the  ply file...
	if (!LoadPlyFileData("bun_res3_xyz.ply"))
	{
		std::cout << "Can't load the file. Existing." << std::endl;
		return -1;
	}
	
	//if (!LoadMeshIntoGPUBuffer()) {
	if (!Load_And_Unroll_MeshIntoGPUBuffer()) {
		std::cout << "Didn't MESH load it to GPU" << std::endl;
		return -1;
	}
	else {
		std::cout << "Mesh is loaded" << std::endl;
	}

	//Copy buffer to GPU 
	//This line change the data to the way shader wants/GPU understands
	
	//copy those lines to seperate file OPenGLMesh Buffer
	//glGenBuffers(1, &vertex_buffer); //& - passing by refference, gives the address
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //gives to a new buffer a type permanetly(never change)
	//- "vertex buffer"
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 


	GLuint program = pTheShaderManager->getIDFromFriendlyName("myShader");
	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");


	//   X      Y    Z    R   G     B
	//{ -0.6f, -0.4f, 0.f, 1.f, 0.f, 0.f }, // vertex 0

	//Passing data here array above in format from vertex01.glsl
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, //"vPosition"
							3, // vec3 (xyz) 
								GL_FLOAT,
								GL_FALSE, // Don't normilize
								sizeof(float) * 6, //after you find the first value vertex, where to 
												   // find the next vertex -> 6 floats away, 6 for xyz+rgb
								(void*)0); //start at 0



	glEnableVertexAttribArray(vcol_location);



	glVertexAttribPointer(vcol_location, //cColor
								3,  //vec3 rgb
								GL_FLOAT, 
								GL_FALSE,
							sizeof(float) * 6, //changed from 5
		
	(void*)(sizeof(float) * 3)); // starts 3 floats in (changed from 2 to 3)

	//set up some bunnies
	objectsToDraw[0].position = glm::vec3( -3.0f, 0.0f, 0.0f );
	objectsToDraw[1].position = glm::vec3( +3.0f, 0.0f, 0.0f);
	objectsToDraw[2].position = glm::vec3( 0.0f, 3.0f, 0.0f);
	objectsToDraw[3].position = glm::vec3(0.0f, -3.0f, 0.0f);

	numberofObjectsToDraw = 4;

	//there is a program logic
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

		glClear(GL_COLOR_BUFFER_BIT); //clear the screen

		//Draw all the objects in the sceene

		for (unsigned int objIndex = 0;
			objIndex != numberofObjectsToDraw; 
			objIndex++) 
		{
		m = glm::mat4(1.0f); //mat4x4_identity(m); linmath - creates identity matrix
		//messing with the model matrix moves the object

		//mat4x4_rotate_Y(m, m, (float)glfwGetTime());  linmath 
		glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), //pathing the matrix
			(float)glfwGetTime(), //give the time
			glm::vec3(0.0f, 0.0, 1.0f)); //1.0f - z coordinate -- rotate around z, 
//m = m * rotateZ;

//generating the matrix which moves the object
		//glm::mat4 matMove = glm::translate(glm::mat4(1.0f),
		//	glm::vec3(-2.0f, 0.0f, 0.0f) //where we are placing the object
		//);

		glm::mat4 matMove = glm::translate(glm::mat4(1.0f),
										   objectsToDraw[objIndex].position //where we are placing the object
										   );

		m = m * matMove;

		//p = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
									//orthgrapic doesn't make things smaller when they far away
		p = glm::perspective(0.6f, 	// mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f); orthograpic linmath; ortho flat everything to 2D
			ratio,
			0.1f,
			1000.0f);

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


		//without below lines modal just filled with serface
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //draw it as a point at each vertex. Reads the data as triangles but draw them as points
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //draw a line between vertexes, GL_FILL is default

		//unsigned int numVertices = 1889;
		//glDrawArrays(GL_TRIANGLES, 0, numVertices);
		glDrawArrays(GL_TRIANGLES, 0, g_UnrolledNumberOfVertices);
		//glDrawArrays(GL_TRIANGLES, 0, 3); //Draws whatever in the buffer 3 - vertises; 
		//Can draw a point, line etc. by gining a coordinates. 

	}

		glfwSwapBuffers(window); //swap windows, so we don't see actual drawing happenin
		glfwPollEvents();
	}

	//DELETING STUFF

	delete pTheShaderManager;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
