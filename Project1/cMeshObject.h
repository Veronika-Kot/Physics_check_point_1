#ifndef _CMESHOBJECT_HG_
#define _CMESHOBJECT_HG_
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

class cMeshObject 
{
public:
	cMeshObject();

	glm::vec3 preRotation; //Happens BEFORE translation/move
	glm::vec3 position;
	glm::vec3 postRotation; //Happens AFTER translation/move
	glm::vec3 orientation;
	glm::vec3 nonUniformScale;
	std::string meshName;
	glm::vec3 objColour;
	//Things we can use to find those objects
	std::string friendlyName;
	
	unsigned int uniqueID;
	static unsigned int nextID;
	static const unsigned int START = 0;

	inline unsigned int getID(void) {
		return this->uniqueID;
	}

	bool bIsWireFrame; // = true; C++ 11
	bool bIsVisiable;
};

#endif
