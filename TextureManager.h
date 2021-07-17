#pragma once
#include "ResourceManager.h"



class TextureManager : public ResourceManager
{
public:
	TextureManager();

	TexturePtr createTextureFromFile(const wchar_t* file_path);
	HeightmapPtr createHeightmapTexture(unsigned int width, unsigned int height, float* data);
	
	~TextureManager();

protected:
	Resource* createResourceFromFileConcrete(const wchar_t* file_path) override;
};

