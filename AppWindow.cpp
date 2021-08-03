#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "ConstantBufferData.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Time.h"
#include "Mathf.h"
#include <Windows.h>
#include <iostream>
#include <random>

#include "Vertex.h"

void AppWindow::update()
{
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	ConstantBufferData cbd;

	m_timer += Time::get().deltaTime();

	m_camera.update();
	Matrix4x4 view = m_camera.getInverseTransform();
	
	cbd.m_time = m_timer;
	cbd.m_view = view;
	cbd.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.01f, 100.0f);

	// Cubes
	for (Cube& cube : cubes)
	{
		cube.update();
		cube.draw(m_cb, cbd);
	}

	// Other GameObjects
	for (GameObject& gameobject : gameobjects)
	{
		gameobject.draw(m_cb, cbd);
	}
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
	UINT size_quad_vertices = ARRAYSIZE(cube_vertices);

	unsigned int quad_indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	UINT size_quad_indices = ARRAYSIZE(cube_indices);
	
	IndexBufferPtr cube_ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(cube_indices, size_cube_indices);
	IndexBufferPtr quad_ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(quad_indices, size_quad_indices);
	
	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	VertexShaderPtr vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	VertexBufferPtr cube_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(cube_vertices, sizeof(Vertex), size_cube_vertices, shader_byte_code, size_shader_byte_code);
	VertexBufferPtr quad_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(quad_vertices, sizeof(Vertex), size_quad_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	PixelShaderPtr ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	// Create constant buffer
	ConstantBufferData cb;
	cb.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cb, sizeof(ConstantBufferData));

	// Create cubes
	for (int i = 0; i < 1; i++)
	{
		Cube cube = Cube("Cube_" + i, cube_vb, cube_ib, vs, ps);
		cube.Position = Vector3(0.0f, -1.0f, 1.0f);
		cubes.push_back(cube);
	}

	// Create plane
	GameObject plane("Plane", quad_vb, quad_ib, vs, ps);
	plane.Scale = Vector3(8.0f, 8.0f, 1.0f);
	plane.Rotation = Vector3(90.0f * Mathf::deg2rad, 0.0f, 0.0f);
	plane.Position = Vector3(0.0f, -1.0f, 1.0f);
	gameobjects.push_back(plane);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();

	RECT rect = getClientWindowRect();
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	update();
	
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
	m_camera.onKeyDown(key);
}

void AppWindow::onKeyUp(int key)
{
	m_camera.onKeyUp(key);
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	RECT window_size = getClientWindowRect();
	float window_width = window_size.right - window_size.left;
	float window_width_half = window_width / 2.0f;
	float window_height = window_size.bottom - window_size.top + 1L;
	float window_height_half = window_height / 2.0f;
	
	m_camera.onMouseMove(Vector2(window_width, window_height), mouse_pos);

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
