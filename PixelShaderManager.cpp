#include "PixelShaderManager.h"
#include "GraphicsEngine.h"
#include "PixelShader.h"

PixelShaderManager::PixelShaderManager()
{
	
}

PixelShaderPtr PixelShaderManager::getPixelShaderFromFile(const std::string& file_path)
{
	return std::static_pointer_cast<PixelShader>(getResourceFromFile(file_path));
}

Resource* PixelShaderManager::createResourceFromFileConcrete(const std::string& file_path)
{
	PixelShader* ps = nullptr;

	try
	{
		ps = new PixelShader(file_path, GraphicsEngine::get().getRenderSystem());
	}
	catch (...)
	{

	}

	return ps;
}

PixelShaderManager::~PixelShaderManager()
{
	
}
