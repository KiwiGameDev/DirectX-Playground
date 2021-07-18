#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "IndexBuffer.h"
#include "Vector2.h"
#include "Vector3.h"
#include "vertex.h"
#include "constant.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Random.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <random>


void AppWindow::update()
{
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);

	Matrix4x4 world_camera(1.0f);
	world_camera *= Matrix4x4::rotationX(m_rot_x);
	world_camera *= Matrix4x4::rotationY(m_rot_y);
	Vector3 new_camera_pos = m_world_camera.getTranslation() + world_camera.getZDirection() * (m_forward * 8.0f * m_delta_time) + world_camera.getXDirection() * (m_rightward * 8.0f * m_delta_time);
	world_camera *= Matrix4x4::translation(new_camera_pos);
	
	m_world_camera = world_camera;
	world_camera.inverse();

	cc.m_time = GetTickCount();
	
	cc.m_view = world_camera;
	cc.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.01f, 100.0f);

	m_cb->update(GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	InputSystem::get().showCursor(false);

	RECT rect = getClientWindowRect();
	m_swap_chain = GraphicsEngine::get().getRenderSystem()->createSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Constant buffer
	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	// Terrain
	terrain = new Terrain(128u, 128u, 128u, 128u);
	// Camera
	m_world_camera *= Matrix4x4::translation({ 5.0f, 10.0f, -5.0f });
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();

	update();

	terrain->update();
	
	RECT rect = getClientWindowRect();
	DeviceContextPtr deviceContext = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();
	deviceContext->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	deviceContext->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);

	terrain->draw(m_cb, cc);
	
	m_swap_chain->present(false);

	m_old_delta = m_new_delta;
	m_new_delta = getMicrosecondsFromStart();
	m_delta_time = m_old_delta ? (m_new_delta - m_old_delta) / 1000000.0f : 0.0f;
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
	else if (key == 'I')
	{
		terrain->bias *= 1.1f;
	}
	else if (key == 'K')
	{
		terrain->bias *= 0.9f;
	}
	else if (key == 'L')
	{
		terrain->octaves++;
		if (terrain->octaves > 8)
			terrain->octaves = 8;
	}
	else if (key == 'J')
	{
		terrain->octaves--;
		if (terrain->octaves < 1)
			terrain->octaves = 1;
	}
	else if (key == 'N')
	{
		terrain->seed = time(0);
		terrain->GeneratePerlinNoiseSeed();
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	RECT screen_rect = getClientWindowRect();
	float screen_width_half = (screen_rect.right - screen_rect.left) / 2.0f;
	float screen_height_half = (screen_rect.bottom - screen_rect.top + 1) / 2.0f;
	
	m_rot_x += (mouse_pos.y - screen_height_half) * m_delta_time * 4.0f;
	m_rot_y += (mouse_pos.x - screen_width_half) * m_delta_time * 4.0f;

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
