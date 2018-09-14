//PlyFileLoader.cpp
#include <string>
#include <iostream>	  //cin cout 
#include <fstream>   //file stream
#include "globalStuff.h"

//We create 2 file sPlyVertex.h sPlyTriangle.h and include it in global stuff
//struct sPlyVertex
//{
//	//sPlyVertex() : x(0.0f), y(0.0f), z(0.0f) {}; //default constractor
//	float x;
//	float y;
//	float z;
//
//};
//struct sPlyTriangle { //"face"
//	int vertex_index_1;
//	int vertex_index_2;
//	int vertex_index_3;
//};

									//Actual varuables allocated in the memory
sPlyVertex* g_pArrayVert = nullptr; //it's where the vertices are
unsigned int g_numberOfVertices = 0;

sPlyTriangle* g_pArrayTris = nullptr; //it's where the tringles
unsigned int g_numberOfTriangles = 0;

//function will copy data from file into CPU
bool LoadPlyFileData( std::string fileName ) { 
	//***Open the file
	std::ifstream theFile(fileName.c_str() ); //converting string to C-style string
	
	if (!theFile.is_open()) { //more c or c++ ish
		std::cout << "Didn't open file" << std::endl;
		return false;
	}

	
	//***search for vertex
	std::string nextData;
	while (theFile >> nextData) {
		if (nextData == "vertex") {
			break;
		}
	}
	
	theFile >> g_numberOfVertices;
	std::cout << "Vertices: " << g_numberOfVertices << std::endl;

	//search for face
	while (theFile >> nextData) {
		if (nextData == "face") {
			break; 
		}
	}

	theFile >> g_numberOfTriangles;
	std::cout << "Triangles: " << g_numberOfTriangles << std::endl;

	//***search for end_header
	while (theFile >> nextData) {
		if (nextData == "end_header") {
			break;
		}
	}

	

	//***Create an array to store the data
	g_pArrayVert = new sPlyVertex[g_numberOfVertices]; //globally defined
	//sPlyVertex ArrayVert[100] static array in the STUCK, since there is no "new"
	//sPlyVertex* pArrayVert = new sPlyVertex[numberOfVertices]; // it's a HEAP base array,
															   //array size can be learn during run time
	
	memset(g_pArrayVert, 0, sizeof(g_pArrayVert) * g_numberOfVertices); //set the memory 
	//distanaition, value, how many bytes you want to overwrite (how many bytes * how many verts)


	//***read the vertex data into the array


	for (unsigned int index = 0; index != g_numberOfVertices; index++) {
		theFile >> g_pArrayVert[index].x >> g_pArrayVert[index].y >> g_pArrayVert[index].z;

		//theFile >> pArrayVert[index].x ; //can be seperate
		//theFile >> pArrayVert[index].y ;
		//theFile >> pArrayVert[index].z;
	}

	//***Same with triangles

	g_pArrayTris = new sPlyTriangle[g_numberOfTriangles];
	//sPlyTriangle* pArrayTris = new sPlyTriangle[numberOfTriangles]; // it's a HEAP base aray,
															   //array size can be learn during run time

	int gardbige = 0;
	for (unsigned int index = 0; index != g_numberOfTriangles; index++) {
		theFile >> gardbige;
		theFile >> g_pArrayTris[index].vertex_index_1; //can be seperate
		theFile >> g_pArrayTris[index].vertex_index_2;
		theFile >> g_pArrayTris[index].vertex_index_3;
	}

	std::cout << "Read from the file ok" << std::endl;
	return true;
}