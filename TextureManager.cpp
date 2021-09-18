#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
	
}

TexturePtr TextureManager::getTextureFromFile(const std::string& file_path)
{
	return std::static_pointer_cast<Texture>(getResourceFromFile(file_path));
}

Resource* TextureManager::createResourceFromFileConcrete(const std::string& file_path)
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
