#ifndef _cMeshObject_HG_
#define _cMeshObject_HG
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class cMeshObject 
{

public:
	glm::vec3 position;

	//ignore this for now
	glm::vec3 orientation;
	glm::vec3 nonUniformScale;

};

#endif
