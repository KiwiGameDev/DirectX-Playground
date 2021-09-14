#include "GraphicsEngine.h"
#include "ConstantBufferData.h"

GraphicsEngine* Singleton<GraphicsEngine>::instance = nullptr;

GraphicsEngine::GraphicsEngine()
{
	ConstantBufferData cbd;
	m_cb = m_render_system.createConstantBuffer(&cbd, sizeof(ConstantBufferData));
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

ConstantBufferPtr GraphicsEngine::getConstantBuffer()
{
	return m_cb;
}

MeshManager& GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

GraphicsEngine::~GraphicsEngine()
{
	
}
