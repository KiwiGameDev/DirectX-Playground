#pragma once
#include "Singleton.h"
#include "RenderSystem.h"
#include "PixelShaderManager.h"
#include "VertexShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"

class DeviceContext;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
	friend class Singleton<GraphicsEngine>;

public:
	RenderSystem& getRenderSystem();
	VertexShaderManager& getVertexShaderManager();
	PixelShaderManager& getPixelShaderManager();
	MeshManager& getMeshManager();
	TextureManager& getTextureManager();
	
protected:
	GraphicsEngine();
	~GraphicsEngine();
	
	RenderSystem m_render_system;
	VertexShaderManager m_vertex_shader_manager;
	PixelShaderManager m_pixel_shader_manager;
	MeshManager m_mesh_manager;
	TextureManager m_texture_manager;
};
