#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "ConstantBufferData.h"
#include "Vector3.h"
#include "Vertex.h"
#include "Mathf.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#include <iostream>
#include <random>

AppWindow::AppWindow()
	: m_editor_camera(1.57f, DEFAULT_WIDTH / DEFAULT_HEIGHT, 0.01f, 1000.0f)
{
	CameraManager::get().setEditorCamera(&m_editor_camera);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);
	
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
	ConstantBufferData cbd;
	cbd.m_time = 0;
	m_cb = GraphicsEngine::get().getRenderSystem()->createConstantBuffer(&cbd, sizeof(ConstantBufferData));

	// Create cubes
	for (int i = 0; i < 1; i++)
	{
		Cube* cube = new Cube("Cube_" + i, cube_vb, cube_ib, m_cb, vs, ps);
		cube->setPosition(Vector3(0.0f, -1.0f, 1.0f));
		gameobjects.push_back(cube);
	}

	// Create plane
	GameObject* plane = new GameObject("Plane", quad_vb, quad_ib, m_cb, vs, ps);
	plane->setScale(Vector3(8.0f, 8.0f, 1.0f));
	plane->setRotation(Vector3(90.0f * Mathf::deg2rad, 0.0f, 0.0f));
	plane->setPosition(Vector3(0.0f, -1.0f, 1.0f));
	gameobjects.push_back(plane);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();
	
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	GraphicsEngine::get().getRenderSystem()->getImmediateDeviceContext()->setViewportSize(screen_width, screen_height);

	// ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);
	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	ImGui::Begin("Credits");
	ImGui::Text("About");
	ImGui::Text("Scene Editor v0.0.1");
	ImGui::Text("By Bruce Anakin C. Miranda");
	ImGui::Text("Acknowledgements:");
	ImGui::Text("PardCode");
	ImGui::Text("Sir Neil");
	ImGui::End();
	
	// Camera
	m_editor_camera.update();

	// Other GameObjects
	for (GameObject* gameobject : gameobjects)
	{
		gameobject->update();
		gameobject->draw();
	}

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	
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
	m_editor_camera.onKeyDown(key);
}

void AppWindow::onKeyUp(int key)
{
	m_editor_camera.onKeyUp(key);
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (m_is_mouse_locked)
	{
		m_editor_camera.onMouseMove(Vector2(mouse_pos.x - m_mouse_locked_pos.x, mouse_pos.y - m_mouse_locked_pos.y));

		InputSystem::get().setCursorPosition(m_mouse_locked_pos);
	}
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_is_mouse_locked = true;
	m_mouse_locked_pos = mouse_pos;
	InputSystem::get().showCursor(false);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_is_mouse_locked = false;
	InputSystem::get().showCursor(true);
}
