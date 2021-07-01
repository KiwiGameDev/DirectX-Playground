#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
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

	Matrix4x4 cube_transform(1.0f);

	Matrix4x4 world_camera(1.0f);
	world_camera *= Matrix4x4::rotationX(m_rot_x);
	world_camera *= Matrix4x4::rotationY(m_rot_y);
	Vector3 new_camera_pos = m_world_camera.getTranslation() + world_camera.getZDirection() * (m_forward * 8.0f * m_delta_time) + world_camera.getXDirection() * (m_rightward * 8.0f * m_delta_time);
	world_camera *= Matrix4x4::translation(new_camera_pos);

	m_world_camera = world_camera;
	world_camera.inverse();

	constant cc;
	cc.m_time = GetTickCount();
	cc.m_world = cube_transform;
	cc.m_view = world_camera;
	cc.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	InputSystem::get().showCursor(false);
	GraphicsEngine::get().init();

	RECT rect = getClientWindowRect();
	
	m_swap_chain = GraphicsEngine::get().getRenderSystem()->createSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Rainbow rectangle
	vertex rectangle_rainbow_vertices[] =
	{
		{ Vector3(-2.0f, -0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f) },
		{ Vector3(-2.0f,  0.5f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(-1.0f,  0.5f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f) }
	};
	UINT size_rectangle_rainbow_vertices = ARRAYSIZE(rectangle_rainbow_vertices);

	unsigned int rectangle_rainbow_indices[] =
	{
		0, 1, 2,
		3, 2, 1,
	};
	UINT size_rectangle_rainbow_indices = ARRAYSIZE(rectangle_rainbow_indices);
	m_ib_1 = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(rectangle_rainbow_indices, size_rectangle_rainbow_indices);

	// Rainbow triangle
	vertex triangle_vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3( 0.0f,  0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3( 0.5f, -0.5f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) }
	};
	UINT size_triangle_vertices = ARRAYSIZE(triangle_vertices);

	unsigned int triangle_indices[] =
	{
		0, 1, 2,
	};
	UINT size_triangle_indices = ARRAYSIZE(triangle_indices);
	m_ib_2 = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(triangle_indices, size_triangle_indices);

	// Green rectangle
	vertex rectangle_green_vertices[] =
	{
		{ Vector3(1.0f, -0.5f, 0.0f) },
		{ Vector3(1.0f,  0.5f, 0.0f) },
		{ Vector3(2.0f, -0.5f, 0.0f) },
		{ Vector3(2.0f,  0.5f, 0.0f) }
	};
	UINT size_rectangle_green_vertices = ARRAYSIZE(rectangle_green_vertices);

	unsigned int rectangle_green_indices[] =
	{
		0, 1, 2,
		3, 2, 1,
	};
	UINT size_rectangle_green_indices = ARRAYSIZE(rectangle_green_indices);
	m_ib_3 = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(rectangle_green_indices, size_rectangle_green_indices);

	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	m_vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	m_vb_1 = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(rectangle_rainbow_vertices, sizeof(vertex), size_rectangle_rainbow_vertices, shader_byte_code, size_shader_byte_code);
	m_vb_2 = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(triangle_vertices, sizeof(vertex), size_triangle_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"GreenVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	m_vs_g = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	m_vb_3 = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(rectangle_green_vertices, sizeof(vertex), size_rectangle_green_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	m_ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"GreenPixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	m_ps_g = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
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
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);
	
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb_1);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib_1);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib_1->getSizeIndices(), 0, 0);
	
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb_2);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib_2);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib_2->getSizeIndices(), 0, 0);
	
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs_g);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps_g);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb_3);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib_3);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib_3->getSizeIndices(), 0, 0);

	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = getMicrosecondsFromStart();
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000000.0f : 0.0f;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

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
