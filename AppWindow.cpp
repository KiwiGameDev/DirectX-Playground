#include "AppWindow.h"
#include "UI.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "ConstantBufferData.h"
#include "Vector3.h"
#include "VertexPositionColor.h"
#include "VertexPositionUV.h"
#include "GameObjectManager.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "PixelShaderManager.h"
#include "TextureManager.h"
#include "VertexShaderManager.h"
#include "Mathf.h"
#include "imgui.h"
#include <Windows.h>
#include <iostream>
#include <random>

AppWindow::AppWindow()
	: m_editor_camera(1.57f, DEFAULT_WIDTH / DEFAULT_HEIGHT, 0.001f, 100.0f)
{
	CameraManager::get().setEditorCamera(&m_editor_camera);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	RECT rect = getClientWindowRect();
	GraphicsEngine& graphics_engine = GraphicsEngine::get();
	RenderSystem* render_system = graphics_engine.getRenderSystem();

	InputSystem::get().addListener(this);

	m_swap_chain = render_system->createSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Cube
	MeshPtr cube_mesh = graphics_engine.getMeshManager()->getMeshFromFile(L"Assets/Meshes/cube.obj");
	TexturePtr bricks = graphics_engine.getTextureManager()->getTextureFromFile(L"Assets/Textures/brick.png");

	// Quad
	VertexPositionColor quad_vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3(-0.5f,  0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( 0.5f,  0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( 0.5f, -0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f) }
	};
	UINT size_quad_vertices = ARRAYSIZE(quad_vertices);
	
	unsigned int quad_indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	UINT size_quad_indices = ARRAYSIZE(quad_indices);
	
	IndexBufferPtr quad_ib = render_system->createIndexBuffer(quad_indices, size_quad_indices);

	// Meshes
	MeshPtr teapot_mesh = graphics_engine.getMeshManager()->getMeshFromFile(L"Assets/Meshes/teapot.obj");
	MeshPtr armadillo_mesh = graphics_engine.getMeshManager()->getMeshFromFile(L"Assets/Meshes/armadillo.obj");
	MeshPtr bunny_mesh = graphics_engine.getMeshManager()->getMeshFromFile(L"Assets/Meshes/bunny.obj");

	// Shaders
	VertexShaderPtr vs = graphics_engine.getVertexShaderManager()->getVertexShaderFromFile(L"VertexShader.hlsl");
	VertexShaderPtr colored_vs = graphics_engine.getVertexShaderManager()->getVertexShaderFromFile(L"ColoredVertexShader.hlsl");
	VertexShaderPtr textured_vs = graphics_engine.getVertexShaderManager()->getVertexShaderFromFile(L"TexturedVertexShader.hlsl");
	PixelShaderPtr ps = graphics_engine.getPixelShaderManager()->getPixelShaderFromFile(L"PixelShader.hlsl");
	PixelShaderPtr colored_ps = graphics_engine.getPixelShaderManager()->getPixelShaderFromFile(L"ColoredPixelShader.hlsl");
	PixelShaderPtr textured_ps = graphics_engine.getPixelShaderManager()->getPixelShaderFromFile(L"TexturedPixelShader.hlsl");

	// Vertex buffers
	VertexBufferPtr quad_vb = render_system->createVertexBuffer(quad_vertices, sizeof(VertexPositionColor), size_quad_vertices, colored_vs, VertexFormat::POSITION_COLOR);

	// Create constant buffer
	ConstantBufferData cbd;
	cbd.m_time = 0;
	m_cb = render_system->createConstantBuffer(&cbd, sizeof(ConstantBufferData));

	// Create cube
	Cube* cube = new Cube("Cube_00", cube_mesh->getVertexBuffer(), cube_mesh->getIndexBuffer(), m_cb, textured_vs, textured_ps);
	cube->setPosition(Vector3(0.0f, -1.0f, 1.0f));
	cube->setTexture(bricks);
	GameObjectManager::get().addGameObject(cube);

	// Create plane
	GameObject* plane = new GameObject("Plane", quad_vb, quad_ib, m_cb, colored_vs, colored_ps);
	plane->setScale(Vector3(8.0f, 8.0f, 1.0f));
	plane->setRotation(Vector3(90.0f * Mathf::deg2rad, 0.0f, 0.0f));
	plane->setPosition(Vector3(0.0f, -1.0f, 1.0f));
	GameObjectManager::get().addGameObject(plane);

	// Create teapot
	GameObject* teapot_go = new GameObject("teapot", teapot_mesh->getVertexBuffer(), teapot_mesh->getIndexBuffer(), m_cb, textured_vs, textured_ps);
	teapot_go->setPosition(Vector3(0.0f, 1.0f, 1.0f));
	teapot_go->setTexture(bricks);
	GameObjectManager::get().addGameObject(teapot_go);

	// Create armadillo
	GameObject* armadillo_go = new GameObject("armadillo", armadillo_mesh->getVertexBuffer(), armadillo_mesh->getIndexBuffer(), m_cb, vs, ps);
	armadillo_go->setPosition(Vector3(2.0f, 1.0f, 1.0f));
	GameObjectManager::get().addGameObject(armadillo_go);

	// Create bunny
	GameObject* bunny_go = new GameObject("bunny", bunny_mesh->getVertexBuffer(), bunny_mesh->getIndexBuffer(), m_cb, vs, ps);
	bunny_go->setPosition(Vector3(-2.0f, 1.0f, 1.0f));
	GameObjectManager::get().addGameObject(bunny_go);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();
	
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setViewportSize(screen_width, screen_height);
	
	// Camera
	m_editor_camera.update();

	// GameObjects
	GameObjectManager::get().update();
	GameObjectManager::get().draw();

	UI::get().draw();

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	Window::onFocus();

	InputSystem::get().addListener(this);
}

void AppWindow::onKillFocus()
{
	Window::onKillFocus();

	InputSystem::get().removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	m_editor_camera.onKeyDown(key);
}

void AppWindow::onKeyUp(int key)
{
	m_editor_camera.onKeyUp(key);
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (m_is_mouse_locked)
	{
		m_editor_camera.onMouseMove(Vector2(mouse_pos.x - m_mouse_locked_pos.x, mouse_pos.y - m_mouse_locked_pos.y));

		InputSystem::get().setCursorPosition(m_mouse_locked_pos);
	}
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_is_mouse_locked = true;
	m_mouse_locked_pos = mouse_pos;
	InputSystem::get().showCursor(false);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_is_mouse_locked = false;
	InputSystem::get().showCursor(true);
}
