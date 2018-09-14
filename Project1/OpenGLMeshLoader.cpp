//OpenGLMeshLoader.cpp
//***Take a model file array and upload it to GPU
#include "sPlyVertex.h"
#include "sPlyTriangle.h"
#include "globalStuff.h"



static const struct sVert_xyz_rgb //This is based on what the shader is expecting 
{
	float x, y, z; //location
	float r, g, b; //color
};


GLuint vertex_buffer;

bool LoadMeshIntoGPUBuffer(void) { //Takes an array of vertices and indices and copies the data to GPU
	
	//***Alocate another array temp for the vertex buffer

	sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_numberOfVertices];
	
	//***Copy everything from PLY format to the vertex buffer format
	for (unsigned int index = 0; index != g_numberOfVertices; index++) {
		
		//copy data to shader
		pVertices[index].x = g_pArrayVert[index].x;
		pVertices[index].y = g_pArrayVert[index].y;
		pVertices[index].z = g_pArrayVert[index].z;

		pVertices[index].r = 1.0f;
		pVertices[index].g = 1.0f;
		pVertices[index].b = 1.0f;
	}

	//***Alocate buffer
		
	glGenBuffers(1, &vertex_buffer); //1 - is how many buffers do you want & - passing by refference, gives the address
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //gives to a new buffer a type permanetly(never change)- "vertex buffer"
	
	unsigned int bufferSizeInBytes = sizeof(sVert_xyz_rgb) * g_numberOfVertices; //we can do it bc it's local = in the stuck, so we can use pointer

	//It takes array of pVertices and copy it to GPU
	glBufferData(GL_ARRAY_BUFFER, //***MAKE ME A BUFFER
				bufferSizeInBytes, //sizeof(vertices), //ALLOCATE THAT MANY BYTES
				pVertices, //vertices, //FROM COPY DATA THIS ARRAY
				GL_STATIC_DRAW); //DRAW

	return true;
}


unsigned int g_UnrolledNumberOfVertices = 0;

bool Load_And_Unroll_MeshIntoGPUBuffer(void) { //Takes an array of triangles and indexes and put in 
	
	//each triangle has 3 vertices
	g_UnrolledNumberOfVertices = g_numberOfTriangles * 3;

	sVert_xyz_rgb* pVertices = new sVert_xyz_rgb[g_UnrolledNumberOfVertices];

	//Figure out how many vertices there would be if the
	//vertex buffer was based on the triangles ("face") list
	//extern sPlyVertex* g_pArrayVert;  //Vertices are here in .h file
	//extern sPlyTriangle* g_pArrayTris; //Triangles are here


	unsigned int vertIndex = 0; //Index into the VERTEX array

	for (unsigned int triIndex = 0; 
		triIndex != g_numberOfTriangles; 
		triIndex++, vertIndex += 3) {

		//**********************Triangle******************************
		unsigned vert1Index = g_pArrayTris[triIndex].vertex_index_1;
		sPlyVertex curVert1 = g_pArrayVert[vert1Index];

		unsigned vert2Index = g_pArrayTris[triIndex].vertex_index_2;
		sPlyVertex curVert2 = g_pArrayVert[vert2Index];

		unsigned vert3Index = g_pArrayTris[triIndex].vertex_index_3;
		sPlyVertex curVert3 = g_pArrayVert[vert3Index];
		//*************************************************************

		//Vertex 1
		pVertices[vertIndex + 0].x = curVert1.x;
		pVertices[vertIndex + 0].y = curVert1.y;
		pVertices[vertIndex + 0].z = curVert1.z;
		pVertices[vertIndex + 0].r = 1.0f;
		pVertices[vertIndex + 0].g = 1.0f;
		pVertices[vertIndex + 0].b = 1.0f;

		//Vertex 2
		pVertices[vertIndex + 1].x = curVert2.x;
		pVertices[vertIndex + 1].y = curVert2.y;
		pVertices[vertIndex + 1].z = curVert2.z;
		pVertices[vertIndex + 1].r = 1.0f;
		pVertices[vertIndex + 1].g = 1.0f;
		pVertices[vertIndex + 1].b = 1.0f;

		//Vertex 3
		pVertices[vertIndex + 2].x = curVert3.x;
		pVertices[vertIndex + 2].y = curVert3.y;
		pVertices[vertIndex + 2].z = curVert3.z;
		pVertices[vertIndex + 2].r = 1.0f;
		pVertices[vertIndex + 2].g = 1.0f;
		pVertices[vertIndex + 2].b = 1.0f;

	}
	//Alocate buffer

	//This lines change the data to the way shader wants/GPU understands
	unsigned int bufferSizeInBytes = sizeof(sVert_xyz_rgb) * g_UnrolledNumberOfVertices; //we can do it bc it's local = in the stuck 
	glGenBuffers(1, &vertex_buffer); //& - passing by refference, gives the address
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //gives to a new buffer a type permanetly(never change)
	//- "vertex buffer"
	glBufferData(GL_ARRAY_BUFFER, bufferSizeInBytes, pVertices, GL_STATIC_DRAW);


	return true;
}

