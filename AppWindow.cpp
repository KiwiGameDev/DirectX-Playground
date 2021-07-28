#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Time.h"
#include "Random.h"
#include <Windows.h>
#include <iostream>
#include <random>

struct vertex
{
	Vector3 position;
	Vector3 color;
};

void AppWindow::update()
{
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	ConstantBufferData cc;

	m_timer += Time::get().deltaTime();
	
	Matrix4x4 world_camera(1.0f);

	m_world_camera = world_camera;
	world_camera.inverse();

	cc.m_time = m_timer;
	cc.m_view = world_camera;
	cc.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.01f, 100.0f);

	for (Cube& cube : cubes)
	{
		cube.update(Time::get().deltaTime());
		cube.draw(m_cb, cc);
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
	vertex cube_vertices[] =
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
	
	IndexBufferPtr ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(cube_indices, size_cube_indices);

	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	VertexShaderPtr vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	VertexBufferPtr vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(cube_vertices, sizeof(vertex), size_cube_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	PixelShaderPtr ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	// Create constant buffer
	ConstantBufferData cb;
	cb.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cb, sizeof(ConstantBufferData));

	// Create cubes
	std::default_random_engine engine;
	std::uniform_real_distribution distribution(-4.0f, 4.0f);
	for (int i = 0; i < 100; i++)
	{
		Cube cube = Cube("Cube_" + i, vb, ib, vs, ps);
		cube.setPosition(Vector3(distribution(engine), distribution(engine), 5.0f));
		cubes.push_back(cube);
	}
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
	
	InputSystem::get().removeListener(this);
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
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
		m_delta_time_multiplier -= Time::get().deltaTime();
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
		m_delta_time_multiplier += Time::get().deltaTime();
	}
}

void AppWindow::onKeyUp(int key)
{
	if (key == 'W' || key == 'S')
	{
		m_forward = 0.0f;
	}
	else if (key == 'A' || key == 'D')
	{
		m_rightward = 0.0f;
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	RECT screen_rect = getClientWindowRect();
	float screen_width_half = (screen_rect.right - screen_rect.left) / 2.0f;
	float screen_height_half = (screen_rect.bottom - screen_rect.top + 1) / 2.0f;
	
	m_rot_x += (mouse_pos.y - screen_height_half) * Time::get().deltaTime() * 4.0f;
	m_rot_y += (mouse_pos.x - screen_width_half) * Time::get().deltaTime() * 4.0f;

	InputSystem::get().setCursorPosition(Point((int)screen_width_half, (int)screen_height_half));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
