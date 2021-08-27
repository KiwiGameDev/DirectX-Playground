#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "VertexShaderManager.h"
#include "PixelShaderManager.h"
#include "TextureManager.h"
#include "MeshManager.h"

GraphicsEngine* Singleton<GraphicsEngine>::instance = nullptr;

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
		m_vertex_shader_manager = new VertexShaderManager();
	}
	catch (...)
	{
		throw std::exception("Vertex Shader Manager failed to initialize!");
	}
	
	try
	{
		m_pixel_shader_manager = new PixelShaderManager();
	}
	catch (...)
	{
		throw std::exception("Pixel Shader Manager failed to initialize!");
	}
	
	try
	{
		m_texture_manager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("Texture Manager failed to initialize!");
	}
	
	try
	{
		m_mesh_manager = new MeshManager();
	}
	catch (...)
	{
		throw std::exception("Mesh Manager failed to initialize!");
	}
}

RenderSystem* GraphicsEngine::getRenderSystem() const
{
	return m_render_system;
}

VertexShaderManager* GraphicsEngine::getVertexShaderManager() const
{
	return m_vertex_shader_manager;
}

PixelShaderManager* GraphicsEngine::getPixelShaderManager() const
{
	return m_pixel_shader_manager;
}

TextureManager* GraphicsEngine::getTextureManager() const
{
	return m_texture_manager;
}

MeshManager* GraphicsEngine::getMeshManager() const
{
	return m_mesh_manager;
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_mesh_manager;
	delete m_render_system;
	delete m_texture_manager;
}
