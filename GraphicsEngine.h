#pragma once
#include "Singleton.h"

class RenderSystem;
class DeviceContext;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
	friend class Singleton<GraphicsEngine>;

public:
	RenderSystem* getRenderSystem();

protected:
	GraphicsEngine();
	~GraphicsEngine();
	
	RenderSystem* m_render_system;
};
