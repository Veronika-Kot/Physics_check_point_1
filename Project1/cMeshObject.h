#ifndef _cMeshObject_HG_
#define _cMeshObject_HG
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

class cMeshObject 
{

public:
	cMeshObject() {
		this->preRotation = glm::vec3(0.0f);
		this->position = glm::vec3(0.0f);
		this->postRotation = glm::vec3(0.0f);
		this->nonUniformScale = glm::vec3(1.0f);

		return;
	}
	glm::vec3 preRotation; //Happens BEFORE translation/move
	glm::vec3 position;
	glm::vec3 postRotation; //Happens AFTER translation/move
	glm::vec3 orientation;
	glm::vec3 nonUniformScale;
	std::string meshName;
	glm::vec3 objColour;
};

#endif
