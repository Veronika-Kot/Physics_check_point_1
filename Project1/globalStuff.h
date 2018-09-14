#ifndef _globalStuff_HG_ //Those 3 lines are header guard
#define _globalStuff_HG_
//globalStuff.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "sPlyVertex.h"
#include "sPlyTriangle.h"

bool LoadPlyFileData(std::string fileName);
bool LoadMeshIntoGPUBuffer(void);
bool Load_And_Unroll_MeshIntoGPUBuffer(void);

//1.Always put extern in front of .h variables
//2.NEVER assigning it to anyting 

//extern key means "Don't allocate it here, but it somewhere allocates, so we can avoid Linker error"
//Those are allocates in the PlyFileLoader.cpp
//extern doesn't work if assign to 0

extern sPlyVertex* g_pArrayVert;  //g for global
extern unsigned int g_numberOfVertices;

extern sPlyTriangle* g_pArrayTris; //or  NULL or nullptr
extern unsigned int g_numberOfTriangles;

extern unsigned int g_UnrolledNumberOfVertices;

#endif //last line must be empty
