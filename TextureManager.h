#pragma once
#include "ResourceManager.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();

	TexturePtr getTextureFromFile(const std::string& file_path);
	
	~TextureManager();

protected:
	Resource* createResourceFromFileConcrete(const std::string& file_path) override;
};

