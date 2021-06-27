#include "AppWindow.h"

#include <iostream>

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

void AppWindow::updateQuadPosition()
{
	RECT rect = getClientWindowRect();
	constant cc;

	m_delta_pos += m_delta_time * 0.1f;
	if (m_delta_pos > 1.0f)
		m_delta_pos -= 1.0f;
	
	m_delta_scale += m_delta_time * 0.5f;

	Matrix4x4 transform(1.0f);
	//temp *= Matrix4x4::scale(Vector3::Lerp(Vector3(-1.0f, -1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), m_delta_scale));
	//temp *= Matrix4x4::translation(Vector3::Lerp(Vector3(-2.0f, -2.0f, 0.0f), Vector3(2.0f, 2.0f, 0.0f), m_delta_pos));
	transform *= Matrix4x4::scale(Vector3(m_scale_cube, m_scale_cube, m_scale_cube));
	transform *= Matrix4x4::rotationZ(0.0f);
	transform *= Matrix4x4::rotationY(m_rot_y);
	transform *= Matrix4x4::rotationX(m_rot_x);

	cc.m_time = GetTickCount();
	cc.m_world = transform;
	cc.m_view = Matrix4x4::identity();
	cc.m_proj = Matrix4x4::orthoLH((rect.right - rect.left) / 300.0f, (rect.bottom - rect.top) / 300.0f, -4.0f, 4.0f);

	m_cb->update(GraphicsEngine::get().getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	GraphicsEngine::get().init();

	m_swap_chain = GraphicsEngine::get().createSwapChain();
	RECT rect = getClientWindowRect();
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

	std::cout << "Delta Time: " << m_delta_time << '\n';
	
	InputSystem::get().update();

	RECT rect = getClientWindowRect();

	updateQuadPosition();

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
	m_delta_time = (m_new_delta - m_old_delta) / 1000000.0f;
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
		m_rot_x += 3.14f * m_delta_time;
	}
	else if (key == 'S')
	{
		m_rot_x -= 3.14f * m_delta_time;
	}
	else if (key == 'A')
	{
		m_rot_y += 3.14f * m_delta_time;
	}
	else if (key == 'D')
	{
		m_rot_y -= 3.14f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{

}

void AppWindow::onMouseMove(const Point& delta_mouse_pos)
{
	m_rot_x -= delta_mouse_pos.y * m_delta_time;
	m_rot_y -= delta_mouse_pos.x * m_delta_time;
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
