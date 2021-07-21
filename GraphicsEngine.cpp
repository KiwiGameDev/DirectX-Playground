#include "GraphicsEngine.h"
#include "RenderSystem.h"
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

	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	m_render_system->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader_byte_code);
	m_mesh_layout_size = size_shader_byte_code;
	m_render_system->releaseCompiledShader();
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_texture_manager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_mesh_manager;
	delete m_render_system;
	delete m_texture_manager;
}
