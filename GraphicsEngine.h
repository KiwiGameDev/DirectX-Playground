#pragma once
#include "Singleton.h"

class RenderSystem;
class DeviceContext;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
public:
	GraphicsEngine();
	bool init();

	RenderSystem* getRenderSystem();

	bool release();
	~GraphicsEngine();

private:
	RenderSystem* m_render_system;
};
