//DrawObject
#include "globalStuff.h"
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void DrawObject(cMeshObject* pCurrentMesh, glm::mat4x4 &matModel, GLuint program)
{

	if(!pCurrentMesh->bIsVisiable)
	{
		return;
	}

	//******Loading uniform variables
	GLint objectColor_UniLoc = glGetUniformLocation(program, "objectColor");
	GLint lightPos_UniLoc = glGetUniformLocation(program, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(program, "lightBrightness");

	//***Unoform mat MVP
	///GLint mvp_location = glGetUniformLocation(program, "MVP"); //glUniformMatrix4fv(mvp_location,...
	GLint matMoldel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	//************************************ glm transformation matrices **************************************

	matModel = glm::mat4x4(1.0f);		/// mat4x4_identity(m);

	/// Before positioning, rotating around a model's axes
	glm::mat4 preRot_X = glm::rotate(glm::mat4(1.0f),
		pCurrentMesh->preRotation.x,
		glm::vec3(1.0f, 0.0, 0.0f));
	matModel = matModel * preRot_X;

	glm::mat4 preRot_Y = glm::rotate(glm::mat4(1.0f),
		pCurrentMesh->preRotation.y,
		glm::vec3(0.0f, 1.0, 0.0f));
	matModel = matModel * preRot_Y;

	glm::mat4 preRot_Z = glm::rotate(glm::mat4(1.0f),
		pCurrentMesh->preRotation.z,
		glm::vec3(0.0f, 0.0, 1.0f));
	matModel = matModel * preRot_Z;

	//* constantly rotating around the axis
	//glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), //pathing the matrix
	//	(float)glfwGetTime(), //give the timea
	//	glm::vec3(0.0f, 0.0, 1.0f));

	//m = m * rotateZ;

	///Positioning model in the scene
	glm::mat4 matMove = glm::translate(glm::mat4(1.0f),
		pCurrentMesh->position);

	matModel = matModel * matMove;

	/// After positioning, rotating around the scene's axes
	glm::mat4 postRot_X = glm::rotate(glm::mat4(1.0f),
		pCurrentMesh->postRotation.x,
		glm::vec3(1.0f, 0.0, 0.0f));
	matModel = matModel * postRot_X;

	glm::mat4 postRot_Y = glm::rotate(glm::mat4(1.0f),
		pCurrentMesh->postRotation.y,
		glm::vec3(0.0f, 1.0, 0.0f));
	matModel = matModel * postRot_Y;

	glm::mat4 postRot_Z = glm::rotate(glm::mat4(1.0f),
		pCurrentMesh->postRotation.z,
		glm::vec3(0.0f, 0.0, 1.0f));
	matModel = matModel * postRot_Z;

	/// And now scale

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		pCurrentMesh->nonUniformScale);
	matModel = matModel * matScale;

	////**************************** End of transformation matrices *******************************

	//mvp = p * view * m;  /// mat4x4_mul(mvp, p, m); linmath multiplyin all 3 metrisis together 

	glUseProgram(program);
	/*glUniformMatrix4fv(mvp_location, ///glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp); linmath
						1,
						GL_FALSE,
						glm::value_ptr(mvp));*/

	glUniformMatrix4fv(matMoldel_location, 1, GL_FALSE, glm::value_ptr(matModel));

	///without below lines modal just filled with surface
	///glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //draw it as a point at each vertex. Reads the data as triangles but draw them as points
	///glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //draw a line between vertexes, GL_FILL is default


	glUniform3f(objectColor_UniLoc,
		pCurrentMesh->objColour.r,
		pCurrentMesh->objColour.g,
		pCurrentMesh->objColour.b);

	//***assigning unoforms
	glUniform3f(lightPos_UniLoc, g_lightPos.x,
		g_lightPos.y,
		g_lightPos.z);

	glUniform1f(lightBrightness_UniLoc, ::g_lightBrightness);


	if (pCurrentMesh->bIsWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_DEPTH); ///Enables the KEEPING of the depth information
		//glDisable(GL_DEPTH_TEST); ///When drawing check the existing depth
		glDisable(GL_CULL_FACE); ///Discared "back facing" triangles 

	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH); ///Enables the KEEPING of the depth information
		//glEnable(GL_DEPTH_TEST); ///When drawing check the existing depth
		//glEnable(GL_CULL_FACE); ///Discared "back facing" triangles 

	}

	//******************Draw mesh models in the screen

	sModelDrawInfo modelInfo;
	modelInfo.meshFileName = pCurrentMesh->meshName;

	if (g_pTheVAOMeshManager->FindDrawInfoByModelName(modelInfo))
	{
		//glDrawArrays(GL_TRIANGLES, 0, modelInfo.numberOfIndices);
		glBindVertexArray(modelInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
	else {
		std::cout << "Can't draw the mesh" << std::endl;
	}
	//**********************End of drawing{
}