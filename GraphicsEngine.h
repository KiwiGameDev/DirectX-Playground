#pragma once
#include "Singleton.h"
#include "TextureManager.h"

class RenderSystem;
class MeshManager;
class DeviceContext;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
	friend class Singleton<GraphicsEngine>;

public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();
	void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);

protected:
	GraphicsEngine();
	~GraphicsEngine();
	
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_texture_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};
