#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "IndexBuffer.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <Windows.h>
#include <iostream>

#include "Mesh.h"
#include "MeshManager.h"
#include "Time.h"

struct vertex
{
	Vector3 position_start;
	Vector3 position_end;
	Vector3 color_start;
	Vector3 color_end;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_time;
};

void AppWindow::update()
{
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	constant cc;

	//m_timer += Time::get().deltaTime() * sin(Time::get().timeSinceApplicationStart() * 0.33f) * 3.0f;
	m_timer += Time::get().deltaTime();

	//Matrix4x4 cube_transform(1.0f);

	//Matrix4x4 world_camera(1.0f);
	//world_camera *= Matrix4x4::translation({ 0.0f, 0.0f, 5.0f });
	
	//m_world_camera = world_camera;
	//world_camera.inverse();

	cc.m_time = m_timer;
	//cc.m_world = cube_transform;
	//cc.m_view = world_camera;
	//cc.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.01f, 100.0f);

	m_cb->update(GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	InputSystem::get().showCursor(false);
	
	m_wood_tex = GraphicsEngine::get().getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");

	RECT rect = getClientWindowRect();
	
	m_swap_chain = GraphicsEngine::get().getRenderSystem()->createSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Cube
	Vector3 position_start[] =
	{
		Vector3(-0.8f, -0.9f, 0.0f),
		Vector3(-0.9f,  0.1f, 0.0f),
		Vector3(-0.5f, -0.5f, 0.0f),
		Vector3(-0.1f, -0.6f, 0.0f)
	};
	
	Vector3 position_end[] =
	{
		Vector3(0.4f, 0.3f, 0.0f),
		Vector3(0.6f, 0.9f, 0.0f),
		Vector3(0.9f, 0.9f, 0.0f),
		Vector3(0.8f, 0.5f, 0.0f)
	};
	
	Vector3 color_start[] =
	{
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(0.0f, 0.0f, 1.0f)
	};
	
	Vector3 color_end[] =
	{
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(1.0f, 0.0f, 0.0f)
	};

	vertex vertex_list[] =
	{
		{ position_start[0], position_end[0], color_start[0], color_end[0] },
		{ position_start[1], position_end[1], color_start[1], color_end[1] },
		{ position_start[2], position_end[2], color_start[2], color_end[2] },
		{ position_start[3], position_end[3], color_start[3], color_end[3] }
	};
	
	UINT size_vertices = ARRAYSIZE(vertex_list);

	unsigned int indices_list[] =
	{
		3,  0,  1,
		1,  2,  3
	};
	UINT size_indices = ARRAYSIZE(indices_list);

	m_ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(indices_list, size_indices);

	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	m_vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	m_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	m_ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();

	update();

	RECT rect = getClientWindowRect();
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndices(), 0, 0);

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
