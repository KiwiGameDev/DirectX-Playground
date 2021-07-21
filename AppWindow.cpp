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
	Vector3 position;
	Vector2 texcoord;
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
	cube_transform *= Matrix4x4::translation(Vector3(cos(Time::get().timeSinceApplicationStart()), sin(Time::get().timeSinceApplicationStart()), 2));

	Matrix4x4 world_camera(1.0f);
	world_camera *= Matrix4x4::rotationX(m_rot_x);
	world_camera *= Matrix4x4::rotationY(m_rot_y);
	Vector3 new_camera_pos = m_world_camera.getTranslation() + world_camera.getZDirection() * (m_forward * 8.0f * Time::get().deltaTime()) + world_camera.getXDirection() * (m_rightward * 8.0f * Time::get().deltaTime());
	world_camera *= Matrix4x4::translation(new_camera_pos);
	
	m_world_camera = world_camera;
	world_camera.inverse();

	cc.m_time = Time::get().timeSinceApplicationStart();
	cc.m_world = cube_transform;
	cc.m_view = world_camera;
	cc.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.01f, 100.0f);

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
	Vector3 position_list[] =
	{
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(-0.5f,  0.5f, -0.5f),
		Vector3( 0.5f,  0.5f, -0.5f),
		Vector3( 0.5f, -0.5f, -0.5f),
		Vector3( 0.5f, -0.5f,  0.5f),
		Vector3( 0.5f,  0.5f,  0.5f),
		Vector3(-0.5f,  0.5f,  0.5f),
		Vector3(-0.5f, -0.5f,  0.5f)
	};
	
	Vector2 texcoord_list[] =
	{
		Vector2(0.0f, 0.0f), 
		Vector2(0.0f, 1.0f), 
		Vector2(1.0f, 0.0f), 
		Vector2(1.0f, 1.0f), 
	};

	vertex vertex_list[] =
	{
		{ position_list[0], texcoord_list[1] },
		{ position_list[1], texcoord_list[0] },
		{ position_list[2], texcoord_list[2] },
		{ position_list[3], texcoord_list[3] },
		
		{ position_list[4], texcoord_list[1] },
		{ position_list[5], texcoord_list[0] },
		{ position_list[6], texcoord_list[2] },
		{ position_list[7], texcoord_list[3] },
		
		{ position_list[1], texcoord_list[1] },
		{ position_list[6], texcoord_list[0] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[2], texcoord_list[3] },
		
		{ position_list[7], texcoord_list[1] },
		{ position_list[0], texcoord_list[0] },
		{ position_list[3], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },
		
		{ position_list[3], texcoord_list[1] },
		{ position_list[2], texcoord_list[0] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },
		
		{ position_list[7], texcoord_list[1] },
		{ position_list[6], texcoord_list[0] },
		{ position_list[1], texcoord_list[2] },
		{ position_list[0], texcoord_list[3] }
	};
	
	UINT size_vertices = ARRAYSIZE(vertex_list);

	unsigned int indices_list[] =
	{
		0,  1,  2,
		2,  3,  0,
		4,  5,  6,
		6,  7,  4,
		8,  9,  10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
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
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setTexturePixelShader(m_wood_tex);
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
