#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "ConstantBufferData.h"
#include "Vector3.h"
#include "Vertex.h"
#include "Mathf.h"
#include <Windows.h>
#include <iostream>
#include <random>

AppWindow::AppWindow()
{
	m_game_camera = new Camera(1.57f, DEFAULT_WIDTH / DEFAULT_HEIGHT, 1.0f, 10.0f);
	m_editor_camera = new Camera(1.57f, DEFAULT_WIDTH / DEFAULT_HEIGHT, 1.0f, 100.0f);
	CameraManager::get().setGameCamera(m_game_camera);
	CameraManager::get().setEditorCamera(m_editor_camera);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	InputSystem::get().showCursor(false);
	
	RECT rect = getClientWindowRect();
	
	m_swap_chain = GraphicsEngine::get().getRenderSystem()->createSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Cube
	Vertex cube_vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(-0.5f,  0.5f, -0.5f), Vector3(1.0f, 1.0f, 0.0f) },
		{ Vector3( 0.5f,  0.5f, -0.5f), Vector3(1.0f, 1.0f, 0.0f) },
		{ Vector3( 0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3( 0.5f, -0.5f,  0.5f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3( 0.5f,  0.5f,  0.5f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f,  0.5f,  0.5f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f, -0.5f,  0.5f), Vector3(0.0f, 1.0f, 0.0f) }
	};
	UINT size_cube_vertices = ARRAYSIZE(cube_vertices);

	unsigned int cube_indices[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 6, 5,
		5, 2, 1,
		7, 0, 3,
		3, 4, 7,
		3, 2, 5,
		5, 4, 3,
		7, 6, 1,
		1, 0, 7
	};
	UINT size_cube_indices = ARRAYSIZE(cube_indices);

	// Quad
	Vertex quad_vertices[] =
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

	// Frustum
	float ar = m_game_camera->getAspectRatio();
	float fov = m_game_camera->getFOV();
	float z_near = m_game_camera->getNearZ();
	float z_far = m_game_camera->getFarZ();
	float halfHeight = tanf(fov / 2.f);
	float halfWidth = halfHeight * ar;

	float xn = halfWidth * z_near;
	float yn = halfHeight * z_near;
	float xf = halfWidth * z_far;
	float yf = halfHeight * z_far;
	
	Vertex frustum_vertices[] =
	{
		{ Vector3(-xn, -yn, z_near), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3(-xn,  yn, z_near), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( xn,  yn, z_near), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( xn, -yn, z_near), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( xf, -yf, z_far),  Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( xf,  yf, z_far),  Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3(-xf,  yf, z_far),  Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3(-xf, -yf, z_far),  Vector3(0.9f, 0.9f, 0.9f) }
	};
	UINT size_frustum_vertices = ARRAYSIZE(frustum_vertices);

	IndexBufferPtr cube_ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(cube_indices, size_cube_indices);
	IndexBufferPtr quad_ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(quad_indices, size_quad_indices);
	
	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	VertexShaderPtr vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	VertexBufferPtr cube_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(cube_vertices, sizeof(Vertex), size_cube_vertices, shader_byte_code, size_shader_byte_code);
	VertexBufferPtr quad_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(quad_vertices, sizeof(Vertex), size_quad_vertices, shader_byte_code, size_shader_byte_code);
	VertexBufferPtr frustum_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(frustum_vertices, sizeof(Vertex), size_frustum_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	PixelShaderPtr ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	// Create constant buffer
	ConstantBufferData cbd;
	cbd.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cbd, sizeof(ConstantBufferData));

	// Cubes
	GameObject* cube1 = new GameObject("Cube_01", cube_vb, cube_ib, m_cb, vs, ps);
	cube1->setScale(Vector3(2.0f, 2.0f, 2.0f));
	cube1->setPosition(Vector3(4.0f, 0.0f, 8.0f));
	GameObject* cube2 = new GameObject("Cube_02", cube_vb, cube_ib, m_cb, vs, ps);
	cube2->setScale(Vector3(2.0f, 2.0f, 2.0f));
	cube2->setPosition(Vector3(0.0f, 0.0f, 2.0f));

	// NDC Cube
	GameObject* cube_ndc = new GameObject("Cube_03", cube_vb, cube_ib, m_cb, vs, ps);
	cube_ndc->setScale(Vector3(2.0f, 2.0f, 1.0f));
	cube_ndc->setPosition(Vector3(0.0f, 0.0f, 0.5f));
	
	// Create plane
	GameObject* plane = new GameObject("Plane", quad_vb, quad_ib, m_cb, vs, ps);
	plane->setScale(Vector3(8.0f, 8.0f, 1.0f));
	plane->setRotation(Vector3(90.0f * Mathf::deg2rad, 0.0f, 0.0f));
	plane->setPosition(Vector3(0.0f, -3.0f, 0.0f));
	
	gameobjects.push_back(cube1);
	gameobjects.push_back(cube2);
	gameobjects.push_back(plane);
	gameobjects.push_back(cube_ndc);

	// Frustum Visualization
	m_frustum = new GameObject("Frustum", frustum_vb, cube_ib, m_cb, vs, ps);
	m_frustum->setPosition(0.0f, 0.0f, 0.0f);
	m_game_camera->setPosition(0.0, 0.0f, 0.0f);
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
	m_editor_camera->update();
	m_game_camera->update();

	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setSolidRasterizerState();
	gameobjects[0]->draw(true); // Cube 1
	gameobjects[1]->draw(true); // Cube 2
	gameobjects[2]->draw(false); // Plane
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setWireframeRasterizerState();
	gameobjects[3]->draw(false); // NDC Cube
	m_frustum->draw(false); // Frustum Visualization
	
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
	m_editor_camera->onKeyDown(key);
}

void AppWindow::onKeyUp(int key)
{
	m_editor_camera->onKeyUp(key);
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	RECT window_size = getClientWindowRect();
	float window_width = window_size.right - window_size.left;
	float window_width_half = window_width / 2.0f;
	float window_height = window_size.bottom - window_size.top + 1L;
	float window_height_half = window_height / 2.0f;
	
	m_editor_camera->onMouseMove(Vector2(window_width, window_height), mouse_pos);

	InputSystem::get().setCursorPosition(Point((int)window_width_half, (int)window_height_half));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	
}
