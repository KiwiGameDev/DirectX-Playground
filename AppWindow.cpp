#include "AppWindow.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "PixelShader.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include <Windows.h>
#include <iostream>

struct vertex
{
	Vector3 position;
	Vector3 color;
	Vector3 color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

void AppWindow::update()
{
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	constant cc;

	Matrix4x4 cube_transform(1.0f);
	cube_transform *= Matrix4x4::translation(Vector3(cos(m_new_delta / 1000000.0f), sin(m_new_delta / 1000000.0f), 2));

	Matrix4x4 world_camera(1.0f);
	world_camera *= Matrix4x4::rotationX(m_rot_x);
	world_camera *= Matrix4x4::rotationY(m_rot_y);
	Vector3 new_camera_pos = m_world_camera.getTranslation() + world_camera.getZDirection() * (m_forward * 8.0f * m_delta_time) + world_camera.getXDirection() * (m_rightward * 8.0f * m_delta_time);
	world_camera *= Matrix4x4::translation(new_camera_pos);

	std::cout << "Rotation: " << m_rot_x << "     " << m_rot_y << '\n';
	
	m_world_camera = world_camera;
	world_camera.inverse();

	cc.m_time = GetTickCount();
	cc.m_world = cube_transform;
	cc.m_view = world_camera;
	cc.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get().getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	InputSystem::get().showCursor(false);
	GraphicsEngine::get().init();

	RECT rect = getClientWindowRect();
	m_swap_chain = GraphicsEngine::get().createSwapChain();
	m_swap_chain->init(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Temporary create triangle
	vertex cube_vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(-0.5f,  0.5f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3( 0.5f,  0.5f, -0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3( 0.5f, -0.5f, -0.5f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3( 0.5f, -0.5f,  0.5f), Vector3(1.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 1.0f) },
		{ Vector3( 0.5f,  0.5f,  0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f) },
		{ Vector3(-0.5f,  0.5f,  0.5f), Vector3(1.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(-0.5f, -0.5f,  0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) }
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

	m_vb = GraphicsEngine::get().createVertexBuffer();
	m_ib = GraphicsEngine::get().createIndexBuffer();
	m_ib->load(cube_indices, size_cube_indices);
	
	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	m_vs = GraphicsEngine::get().createVertexShader(shader_byte_code, size_shader_byte_code);
	m_vb->load(cube_vertices, sizeof(vertex), size_cube_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().releaseCompiledShader();
	
	GraphicsEngine::get().compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	m_ps = GraphicsEngine::get().createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().releaseCompiledShader();

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get().createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();

	update();

	RECT rect = getClientWindowRect();
	GraphicsEngine::get().getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	GraphicsEngine::get().getImmediateDeviceContext()->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);
	GraphicsEngine::get().getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get().getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);
	GraphicsEngine::get().getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get().getImmediateDeviceContext()->setPixelShader(m_ps);
	GraphicsEngine::get().getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get().getImmediateDeviceContext()->setIndexBuffer(m_ib);
	GraphicsEngine::get().getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndices(), 0, 0);

	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = getMicrosecondsFromStart();
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000000.0f : 0.0f;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get().release();
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
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
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
	float screen_height_half = (screen_rect.bottom - screen_rect.top) / 2.0f;
	
	m_rot_x += (mouse_pos.y - screen_height_half) * m_delta_time * 1.0f;
	m_rot_y += (mouse_pos.x - screen_width_half) * m_delta_time * 1.0f;

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

long long AppWindow::getMicrosecondsFromStart()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);

	if (s_use_qpc)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000000LL * now.QuadPart) / s_frequency.QuadPart;
	}

	return GetTickCount();
}
