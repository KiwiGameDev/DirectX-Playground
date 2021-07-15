#pragma once
#include "Singleton.h"
#include "TextureManager.h"

class RenderSystem;
class DeviceContext;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
	friend class Singleton<GraphicsEngine>;

public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();

protected:
	GraphicsEngine();
	~GraphicsEngine();
	
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_texture_manager = nullptr;
};
