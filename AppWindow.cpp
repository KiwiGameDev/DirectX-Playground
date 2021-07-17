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
#include "Random.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <random>

struct vertex
{
	Vector3 position;
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

	Matrix4x4 terrain_transform(1.0f);
	terrain_transform *= Matrix4x4::scale({ 16.0f, 1.0f, 16.0f });

	Matrix4x4 world_camera(1.0f);
	world_camera *= Matrix4x4::rotationX(m_rot_x);
	world_camera *= Matrix4x4::rotationY(m_rot_y);
	Vector3 new_camera_pos = m_world_camera.getTranslation() + world_camera.getZDirection() * (m_forward * 8.0f * m_delta_time) + world_camera.getXDirection() * (m_rightward * 8.0f * m_delta_time);
	world_camera *= Matrix4x4::translation(new_camera_pos);
	
	m_world_camera = world_camera;
	world_camera.inverse();

	cc.m_time = GetTickCount();
	cc.m_world = terrain_transform;
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

	// Terrain
	const float TERRAIN_SIZE_WIDTH = 16.0f;
	const float TERRAIN_SIZE_DEPTH = 16.0f;
	const int TERRAIN_SUBDIVISIONS_WIDTH = 32;
	const int TERRAIN_SUBDIVISIONS_DEPTH = 32;
	const int TERRAIN_VERTICES_WIDTH = TERRAIN_SUBDIVISIONS_WIDTH + 1;
	const int TERRAIN_VERTICES_DEPTH = TERRAIN_SUBDIVISIONS_DEPTH + 1;
	std::vector<vertex> terrainVertices;
	std::vector<unsigned int> terrainIndices;
	for (int i = 0; i <= TERRAIN_SUBDIVISIONS_DEPTH; i++)
	{
		for (int j = 0; j <= TERRAIN_SUBDIVISIONS_WIDTH; j++)
		{
			terrainVertices.push_back({{(float)j / TERRAIN_SUBDIVISIONS_WIDTH, 0.0f, (float)i / TERRAIN_SUBDIVISIONS_DEPTH }});
		}
	}
	for (int i = 0; i < TERRAIN_SUBDIVISIONS_DEPTH; i++)
	{
		for (int j = 0; j < TERRAIN_SUBDIVISIONS_WIDTH; j++)
		{
			unsigned int botLeft = j + i * TERRAIN_VERTICES_WIDTH;
			unsigned int topLeft = j + (i + 1) * TERRAIN_VERTICES_WIDTH;
			unsigned int topRight = (j + 1) + (i + 1) * TERRAIN_VERTICES_WIDTH;
			unsigned int botRight = (j + 1) + i * TERRAIN_VERTICES_WIDTH;
			terrainIndices.push_back(botLeft);
			terrainIndices.push_back(topLeft);
			terrainIndices.push_back(topRight);
			terrainIndices.push_back(botLeft);
			terrainIndices.push_back(topRight);
			terrainIndices.push_back(botRight);
		}
	}

	// Perlin Noise
	int HEIGHTMAP_SIZE = 128;
	perlin_noise_seed = (float*)malloc(HEIGHTMAP_SIZE * HEIGHTMAP_SIZE * sizeof(float));
	perlin_noise = (float*)malloc(HEIGHTMAP_SIZE * HEIGHTMAP_SIZE * sizeof(float));
	std::default_random_engine randomEngine;
	std::uniform_real_distribution distribution(0.0f, 1.0f);

	// Create seed
	for (int i = 0; i < HEIGHTMAP_SIZE * HEIGHTMAP_SIZE; i++)
	{
		perlin_noise_seed[i] = distribution(randomEngine);
	}

	// Create heightmap buffer
	Random::get().perlinNoise2D(HEIGHTMAP_SIZE, HEIGHTMAP_SIZE, perlin_noise_seed, 8, 2.0f, perlin_noise);

	heightmap = GraphicsEngine::get().getRenderSystem()->createHeightmapTexture(HEIGHTMAP_SIZE, HEIGHTMAP_SIZE, perlin_noise);

	// Create terrain index buffer
	m_ib = GraphicsEngine::get().getRenderSystem()->createIndexBuffer(terrainIndices.data(), terrainIndices.size());

	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().getRenderSystem()->compileVertexShader(L"TerrainVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	m_vs = GraphicsEngine::get().getRenderSystem()->createVertexShader(shader_byte_code, size_shader_byte_code);
	m_vb = GraphicsEngine::get().getRenderSystem()->createVertexBuffer(terrainVertices.data(), sizeof(vertex), terrainVertices.size(), shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();
	
	GraphicsEngine::get().getRenderSystem()->compilePixelShader(L"TerrainPixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	m_ps = GraphicsEngine::get().getRenderSystem()->createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	// Camera
	m_world_camera *= Matrix4x4::translation({ 5.0f, 10.0f, -5.0f });
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();

	update();

	RECT rect = getClientWindowRect();
	DeviceContextPtr deviceContext = GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext();
	deviceContext->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	deviceContext->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);
	deviceContext->setConstantBuffer(m_vs, m_cb);
	deviceContext->setConstantBuffer(m_ps, m_cb);
	deviceContext->setVertexShader(m_vs);
	deviceContext->setPixelShader(m_ps);
	deviceContext->setHeightmapVertexShader(heightmap);
	deviceContext->setVertexBuffer(m_vb);
	deviceContext->setIndexBuffer(m_ib);
	deviceContext->drawIndexedTriangleList(m_ib->getSizeIndices(), 0, 0);

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
