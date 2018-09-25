//DrawObject
#include "globalStuff.h"
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

void DrawElement( glm::vec3 *theVertices, glm::mat4x4 &matModel, GLuint program)
{
	sVert_xyz_rgb vertices[3];
	for (unsigned int index = 0; index != 3; index++)
	{
		vertices[index].x = theVertices[index].x;
		vertices[index].y = theVertices[index].y;
		vertices[index].z = theVertices[index].z;

		vertices[index].r = 1.0f;
		vertices[index].g = 0.0f;
		vertices[index].b = 0.0f;
	}

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer); //& - passing by refference, gives the address
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //gives to a new buffer a type permanetly(never change)
	//- "vertex buffer"
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint  vpos_location, vcol_location;
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");


	GLint matMoldel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");

	GLint objectColor_UniLoc = glGetUniformLocation(program, "objectColor");
	GLint lightPos_UniLoc = glGetUniformLocation(program, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(program, "lightBrightness");


	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, //"vPosition"
		3, // vec3 (xyz) 
		GL_FLOAT,
		GL_FALSE, // Don't normilize
		sizeof(float) * 6, //after you find the first value vertex, where to 
						   // find the next vertex -> 6 floats away, 6 for xyz+rgb
		(void*)0); //start with 0 byte

	glEnableVertexAttribArray(vcol_location);

	glVertexAttribPointer(vcol_location, //cColor
		3,  //vec3 rgb
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 6, //changed from 5
		(void*)(sizeof(float) * 3)); //changed from 2 to 3


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(program);
	glUniformMatrix4fv(matMoldel_location, 1, GL_FALSE, glm::value_ptr(matModel));


	glUniform3f(objectColor_UniLoc,
		1.0f,
		0.0f,
		0.0f);

	//***assigning unoforms
	glUniform3f(lightPos_UniLoc, g_lightPos.x,
		g_lightPos.y,
		g_lightPos.z);

	glUniform1f(lightBrightness_UniLoc, ::g_lightBrightness);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}