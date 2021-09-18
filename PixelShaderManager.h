#pragma once
#include "ResourceManager.h"

class PixelShaderManager : public ResourceManager
{
public:
	PixelShaderManager();

	PixelShaderPtr getPixelShaderFromFile(const std::string& file_path);

	~PixelShaderManager();

protected:
	Resource* createResourceFromFileConcrete(const std::string& file_path) override;
};
