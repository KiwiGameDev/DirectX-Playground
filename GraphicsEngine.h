#pragma once
#include "Singleton.h"

class RenderSystem;
class VertexShaderManager;
class PixelShaderManager;
class TextureManager;
class MeshManager;
class DeviceContext;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
	friend class Singleton<GraphicsEngine>;

public:
	RenderSystem* getRenderSystem() const;
	VertexShaderManager* getVertexShaderManager() const;
	PixelShaderManager* getPixelShaderManager() const;
	TextureManager* getTextureManager() const;
	MeshManager* getMeshManager() const;
	
protected:
	GraphicsEngine();
	~GraphicsEngine();
	
	RenderSystem* m_render_system = nullptr;
	VertexShaderManager* m_vertex_shader_manager = nullptr;
	PixelShaderManager* m_pixel_shader_manager = nullptr;
	TextureManager* m_texture_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;
};
