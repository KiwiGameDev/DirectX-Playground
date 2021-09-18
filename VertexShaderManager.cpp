#include "VertexShaderManager.h"
#include "GraphicsEngine.h"
#include "VertexShader.h"

VertexShaderManager::VertexShaderManager()
{
	
}

VertexShaderPtr VertexShaderManager::getVertexShaderFromFile(const std::string& file_path)
{
	return std::static_pointer_cast<VertexShader>(getResourceFromFile(file_path));
}

Resource* VertexShaderManager::createResourceFromFileConcrete(const std::string& file_path)
{
	VertexShader* vs = nullptr;

	try
	{
		vs = new VertexShader(file_path, GraphicsEngine::get().getRenderSystem());
	}
	catch (...)
	{

	}

	return vs;
}

VertexShaderManager::~VertexShaderManager()
{
	
}