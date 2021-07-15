#pragma once
#include "ResourceManager.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();

	TexturePtr createTextureFromFile(const wchar_t* file_path);
	
	~TextureManager();

protected:
	Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};

