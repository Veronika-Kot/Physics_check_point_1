//UserIO.cpp
#include "globalStuff.h"
#include <iostream>

unsigned int activeId = 0;
cMeshObject* activeModel = NULL;//findObjectByUniqueID(activeId);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	activeModel = findObjectByUniqueID(activeId);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		if (activeId == 0) {
			activeId = (unsigned int)g_vecObjectsToDraw.size();
			activeId -= 1; 
		}
		else {
			activeId -= 1;
		}

		activeModel = findObjectByUniqueID(activeId);
		std::cout << activeId << " " << activeModel->friendlyName <<std::endl;
		/*glfwSetWindowTitle(window, "Hi");*/
		
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		unsigned int num = (unsigned int)g_vecObjectsToDraw.size();
		if (activeId >= num - 1) {
			activeId = 0;
		}
		else {
			activeId += 1;
		}

		activeModel = findObjectByUniqueID(activeId);
		std::cout << activeId << " " << activeModel->friendlyName << std::endl;
		//std::string fname = activeModel->friendlyName;
		/*std::string fname = "HERE";
		glfwSetWindowTitle(window, fname.c_str);*/
	}


	return;
}

bool IsShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool IsCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool IsAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool AreAllModifiersUp(GLFWwindow* window)
{
	if (IsShiftDown(window)) { return false; }
	if (IsCtrlDown(window)) { return false; }
	if (IsAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void processKeys(GLFWwindow* window)
{
	const float CAMERA_SPEED_SLOW = 0.1f;
	const float CAMERA_SPEED_FAST = 1.0f;
	float cameraSpeed = CAMERA_SPEED_SLOW;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		cameraSpeed = CAMERA_SPEED_FAST;
	}
	
	if (!IsAltDown(window) && !IsAltDown(window)) {
		// ****  Z  ****
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //Forward
		{
			//checked every frame
			g_CameraEye.z += cameraSpeed; //Clear the screen - Draw the bunny - check for key = over and over again
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //Bakward
		{
			g_CameraEye.z -= cameraSpeed;
		}

		// ****  X  ****
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //Right
		{
			g_CameraEye.x += cameraSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //Left
		{
			g_CameraEye.x -= cameraSpeed;
		}

		// ****  Y  ****
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)//Up
		{
			g_CameraEye.y += cameraSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //Down
		{
			g_CameraEye.y -= cameraSpeed;
		}

	}
	const float MIN_LIGHT_BRIGHTNESS = 0.001f;

	// Control (ctrl) key down? Move light
	if (IsCtrlDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W)) { ::g_lightPos.z += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_S)) { ::g_lightPos.z -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_A)) { ::g_lightPos.x -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_D)) { ::g_lightPos.x += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_Q)) { ::g_lightPos.y += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_E)) { ::g_lightPos.y -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_PERIOD))
		{	// Increase the "brightness"
			::g_lightBrightness *= 1.01f;		// Increase by 1 percent
		}
		if (glfwGetKey(window, GLFW_KEY_COMMA))
		{	// Decrease the "brightness"
			::g_lightBrightness *= 0.99f;		// Decrease by 1 percent
			if (::g_lightBrightness < MIN_LIGHT_BRIGHTNESS)
			{
				::g_lightBrightness = MIN_LIGHT_BRIGHTNESS;
			}
		}
	}//if(!IsShiftDown(window) )

	//std::cout << ::g_lightPos.x << ", " << ::g_lightPos.y << ", " << ::g_lightPos.z << std::endl;


	if (IsAltDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { activeModel->position.z += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_S)) { activeModel->position.z -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_A)) { activeModel->position.x -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_D)) { activeModel->position.x += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_Q)) { activeModel->position.y += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_E)) { activeModel->position.y -= cameraSpeed; }
	}//if(!IsAltDown(window) )

	return;

}
