#include "AppWindow.h"
#include "UI.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "EditorApplication.h"
#include "Vector3.h"
#include "VertexPositionUV.h"
#include "GameObjectManager.h"
#include "ComponentSystem.h"
#include "Transform.h"
#include "GameObjectFactory.h"
#include "Random.h"
#include "imgui.h"
#include <Windows.h>
#include <random>

#include "MeshFactory.h"

AppWindow::AppWindow()
	: m_editor_camera("EditorCamera", 1.57f, DEFAULT_WIDTH / DEFAULT_HEIGHT, 0.001f, 100.0f)
{
	m_editor_camera.getComponent<Transform>().setPosition(0.0f, 5.0f, -5.0f);
	CameraManager::get().setEditorCamera(&m_editor_camera);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get().addListener(this);

	RECT rect = getClientWindowRect();
	m_swap_chain = GraphicsEngine::get().getRenderSystem().createSwapChain(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Initialize resources
	MeshFactory::initializeAllMeshes();
	
	// Create GameObjects
	GameObject* capsule = GameObjectFactory::createCapsule("Capsule_00");
	capsule->getComponent<Transform>().setPosition(1.0f, 4.0f, 1.0f);
	GameObject* sphere = GameObjectFactory::createSphere("Sphere_00");
	sphere->getComponent<Transform>().setPosition(-2.0f, 2.0f, -2.0f);

	// Add GameObjects
	GameObjectManager::get().addGameObject(GameObjectFactory::createTeapot("Teapot_00"));
	GameObjectManager::get().addGameObject(GameObjectFactory::createStaticPhysicsPlane("StaticPhysicsPlane_00"));
	GameObjectManager::get().addGameObject(sphere);
	GameObjectManager::get().addGameObject(capsule);

	spawnCubes();

	GameObjectManager::get().saveGameObjectsStartingState();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get().update();
	
	RECT screen_rect = getClientWindowRect();
	float screen_width = (float)(screen_rect.right - screen_rect.left);
	float screen_height = (float)(screen_rect.bottom - screen_rect.top);
	GraphicsEngine::get().getRenderSystem().getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);
	GraphicsEngine::get().getRenderSystem().getImmediateDeviceContext()->setViewportSize(screen_width, screen_height);

	// Camera
	m_editor_camera.update();
	
	// Update
	if (EditorApplication::get().getState() == EditorApplication::State::Play || EditorApplication::get().getState() == EditorApplication::State::Step)
	{
		GameObjectManager::get().start();
		GameObjectManager::get().update();
		ComponentSystem::get().getPhysicsSystem().update();
	}

	if (EditorApplication::get().getState() == EditorApplication::State::Step)
	{
		EditorApplication::get().setState(EditorApplication::State::Pause);
	}

	// Draw
	GameObjectManager::get().draw();
	UI::get().draw();

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

	if (key == 'J')
	{
		spawnCubes();
	}
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

void AppWindow::spawnCubes()
{
	for (int i = 0; i < 10; i++)
	{
		GameObject* cube = GameObjectFactory::createTexturedPhysicsCube("PhysicsCube_" + std::to_string(m_cubes_count++));
		cube->getComponent<Transform>().setPosition({ Random::get().range(-1.0f, 1.0f), 10.0f, Random::get().range(-1.0f, 1.0f) });
		cube->getComponent<Transform>().setOrientationEuler({ Random::get().range(-3.0f, 3.0f), Random::get().range(-3.0f, 3.0f), Random::get().range(-3.0f, 3.0f) });
		GameObjectManager::get().addGameObject(cube);
	}
}

