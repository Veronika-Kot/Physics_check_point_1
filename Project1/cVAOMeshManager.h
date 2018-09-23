#ifndef _cVAOMeshManager_HG_
#define _cVAOMeshManager_HG_
#include <string>
#include <map>
#include "sModelDrawInfo.h"

//This class is in a charge of:
// - taking a cMesh object (loaded from file)
// - loading that mesh data into a vetex and index buffer,
//   inside a Vertex Array Object (VAO)
// - looking up the rendering (drawing) information for rendering
// - allowing the mesh onjects to be "dynamic" if needed

class cVAOMeshManager
{
public:
	cVAOMeshManager();
	~cVAOMeshManager();
	
	//NOTE: the shader program id is needed to tie 
	//the buffer to the vertex layout of the shader
	bool LoadModelIntoVAO(sModelDrawInfo &drawInfo,
						unsigned int shaderProgramID );

	//Looks up draw info by name (meshFileName)
	bool FindDrawInfoByModelName(sModelDrawInfo &drawInfo);

	void ShutDown(void);
	std::string GetLastError(bool bAndClear = true);

private:

	bool m_LoadModelFromFile(sModelDrawInfo &drawInfo);

	bool m_LoadDrawInfo_Into_VAO(sModelDrawInfo &drawInfo,
								unsigned int shaderProgramID );

	//this holds the model information
	std::map< std::string /*model name*/, sModelDrawInfo > m_mapModel_to_VAOID;

	std::string m_lastErrorString;
	void m_AppendTextToLastError(std::string text, bool addNewLineBefore = true);

};

#endif

