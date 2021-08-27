#pragma once
#include "ResourceManager.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();

	TexturePtr getTextureFromFile(const wchar_t* file_path);
	
	~TextureManager();

protected:
	Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};

