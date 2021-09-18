#pragma once
#include "ResourceManager.h"

class VertexShaderManager : public ResourceManager
{
public:
	VertexShaderManager();

	VertexShaderPtr getVertexShaderFromFile(const std::string& file_path);
	
	~VertexShaderManager();
	
protected:
	Resource* createResourceFromFileConcrete(const std::string& file_path) override;
};
