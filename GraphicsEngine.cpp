#include "GraphicsEngine.h"
#include "RenderSystem.h"

GraphicsEngine* GraphicsEngine::instance = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("Render System failed to initialize!");
	}
	
	try
	{
		m_texture_manager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("Texture Manager failed to initialize!");
	}
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_texture_manager;
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_render_system;
	delete m_texture_manager;
}
