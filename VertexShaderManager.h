#pragma once
#include "ResourceManager.h"

class VertexShaderManager : public ResourceManager
{
public:
	VertexShaderManager();

	VertexShaderPtr getVertexShaderFromFile(const wchar_t* file_path);
	
	~VertexShaderManager();
	
protected:
	Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};
