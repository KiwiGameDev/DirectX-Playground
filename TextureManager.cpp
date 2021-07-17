#include "TextureManager.h"
#include "Texture.h"
#include "Heightmap.h"

TextureManager::TextureManager()
{
	
}

TexturePtr TextureManager::createTextureFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}

HeightmapPtr TextureManager::createHeightmapTexture(unsigned width, unsigned height, float* data)
{
	return std::make_shared<Heightmap>(width, height, data);
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	Texture* tex = nullptr;

	try
	{
		tex = new Texture(file_path);
	}
	catch(...)
	{
		
	}

	return tex;
}

TextureManager::~TextureManager()
{
	
}
