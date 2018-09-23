//globalStuff.h
#ifndef _globalStuff_HG_
#define _globalStuff_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "sPlyVertex.h"
#include "sPlyTriangle.h"

//extern key means "Don't allocate it here, bc it is(will be) allocated somewhere else,
//It helps to avoid Linker error. "extern" doesn't work if assign to 0
//1.Always put extern in front of .h variables
//2.NEVER assigning it to anyting 


//OLD GLOBAL STUFF
//extern sPlyVertex* g_pArrayVert;  //g for global
//extern unsigned int g_numberOfVertices;
//extern sPlyTriangle* g_pArrayTris; //or  NULL or nullptr
//extern unsigned int g_numberOfTriangles;
//extern unsigned int g_UnrolledNumberOfVertices;
//bool LoadPlyFileData(std::string fileName);
//bool LoadMeshIntoGPUBuffer(void);
//bool Load_And_Unroll_MeshIntoGPUBuffer(void);
//END OF THE OLD GLOBAL STUFF

#endif
