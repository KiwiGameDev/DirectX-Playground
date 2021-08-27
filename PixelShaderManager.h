#pragma once
#include "ResourceManager.h"

class PixelShaderManager : public ResourceManager
{
public:
	PixelShaderManager();

	PixelShaderPtr getPixelShaderFromFile(const wchar_t* file_path);

	~PixelShaderManager();

protected:
	Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};
