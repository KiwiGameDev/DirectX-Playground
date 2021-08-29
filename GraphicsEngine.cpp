#include "GraphicsEngine.h"

GraphicsEngine* Singleton<GraphicsEngine>::instance = nullptr;

GraphicsEngine::GraphicsEngine()
{

}

RenderSystem& GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

VertexShaderManager& GraphicsEngine::getVertexShaderManager()
{
	return m_vertex_shader_manager;
}

PixelShaderManager& GraphicsEngine::getPixelShaderManager()
{
	return m_pixel_shader_manager;
}

TextureManager& GraphicsEngine::getTextureManager()
{
	return m_texture_manager;
}

MeshManager& GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

GraphicsEngine::~GraphicsEngine()
{
	
}
