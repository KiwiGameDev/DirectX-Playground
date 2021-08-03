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

#include "Random.h"

struct vertex
{
	Vector3 position;
	Vector3 color1;
	Vector3 color2;
};

void AppWindow::update()
{
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	ConstantBufferData cbd;

	m_timer += Time::get().deltaTime() * 2.0f;

	Matrix4x4 camera_transform = m_camera.getTransform();
	Vector3 new_camera_pos = m_camera.getLocalPosition() + camera_transform.getZDirection() * m_forward * 4.0f * Time::get().deltaTime() + camera_transform.getXDirection() * m_rightward * 4.0f * Time::get().deltaTime();
	m_camera.setPosition(new_camera_pos);

	Matrix4x4 view = m_camera.getTransform();
	view.inverse();
	
	cbd.m_time = m_timer;
	cbd.m_view = view;
	cbd.m_proj = Matrix4x4::perspectiveFovLH(1.57f, screen_width / screen_height, 0.0001f, 100.0f);

	// Cubes
	for (Cube& cube : cubes)
	{
		//cube.update(Time::get().deltaTime());
		//cube.setScale(Vector3::Lerp(Vector3(1.0f, 1.0f, 1.0f), Vector3(6.0f, 6.0f, 0.0f), (sin(m_timer) + 1.0f) / 2.0f));
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
	vertex cube_vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.9f, 0.2f, 0.3f), Vector3(0.2f, 0.1f, 0.9f) },
		{ Vector3(-0.5f,  0.5f, -0.5f), Vector3(0.1f, 0.9f, 0.3f), Vector3(0.5f, 0.9f, 0.2f) },
		{ Vector3( 0.5f,  0.5f, -0.5f), Vector3(0.2f, 0.6f, 0.9f), Vector3(0.9f, 0.1f, 0.1f) },
		{ Vector3( 0.5f, -0.5f, -0.5f), Vector3(0.1f, 0.9f, 0.2f), Vector3(0.1f, 0.9f, 0.2f) },
		{ Vector3( 0.5f, -0.5f,  0.5f), Vector3(0.9f, 0.1f, 0.4f), Vector3(0.9f, 0.2f, 0.9f) },
		{ Vector3( 0.5f,  0.5f,  0.5f), Vector3(0.9f, 0.9f, 0.9f), Vector3(0.2f, 0.2f, 0.2f) },
		{ Vector3(-0.5f,  0.5f,  0.5f), Vector3(0.8f, 0.8f, 0.2f), Vector3(0.8f, 0.1f, 0.9f) },
		{ Vector3(-0.5f, -0.5f,  0.5f), Vector3(0.8f, 0.1f, 0.9f), Vector3(0.8f, 0.9f, 0.2f) }
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
	vertex quad_vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3(-0.5f,  0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( 0.5f,  0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f), Vector3(0.9f, 0.9f, 0.9f) },
		{ Vector3( 0.5f, -0.5f, 0.0f), Vector3(0.9f, 0.9f, 0.9f), Vector3(0.9f, 0.9f, 0.9f) }
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
	VertexBufferPtr cube_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(cube_vertices, sizeof(vertex), size_cube_vertices, shader_byte_code, size_shader_byte_code);
	VertexBufferPtr quad_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(quad_vertices, sizeof(vertex), size_quad_vertices, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	PixelShaderPtr ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	// Create constant buffer
	ConstantBufferData cb;
	cb.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cb, sizeof(ConstantBufferData));

	// Create cubes
	/*for (int i = 0; i < 50; i++)
	{
		Cube cube = Cube("Cube_" + i, cube_vb, cube_ib, vs, ps);
		cube.setPosition(0, 0, 0);
		cubes.push_back(cube);
	}*/

	Cube cube1 = Cube("Cube_01", cube_vb, cube_ib, vs, ps);
	cube1.setPosition(0, 0.9, 0);
	cubes.push_back(cube1);
	
	Cube cube2 = Cube("Cube_02", cube_vb, cube_ib, vs, ps);
	cube2.setPosition(-1.5, 2.0, 0);
	cubes.push_back(cube2);
	
	Cube cube3 = Cube("Cube_03", cube_vb, cube_ib, vs, ps);
	cube3.setPosition(-1.5, 3.0, -2.0);
	cubes.push_back(cube3);

	// Create plane
	GameObject plane("Plane", quad_vb, quad_ib, vs, ps);
	plane.setRotation(90.0f * Mathf::deg2rad, 0.0f, 0.0f);
	plane.setPosition(0.0f, 0.0f, 0.0f);
	plane.setScale(12.0f, 12.0f, 1.0f);
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
	
	m_camera.setRotationX(m_camera.getLocalRotationX() + (mouse_pos.y - screen_height_half) * Time::get().deltaTime() * 4.0f);
	m_camera.setRotationY(m_camera.getLocalRotationY() + (mouse_pos.x - screen_width_half) * Time::get().deltaTime() * 4.0f);

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
