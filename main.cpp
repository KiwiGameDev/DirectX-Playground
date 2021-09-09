#define NOMINMAX
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ComponentSystem.h"
#include "EditorApplication.h"
#include "GameObjectManager.h"
#include "InputSystem.h"
#include "Random.h"
#include "Time.h"
#include "UI.h"
#include <iostream>

int main()
{
	try
	{
		Time::create();
		Random::create();
		InputSystem::create();
		CameraManager::create();
		GraphicsEngine::create();
		EditorApplication::create();
		GameObjectManager::create();
		ComponentSystem::create();
	}
	catch(...)
	{
		return -1;
	}

	try
	{
		AppWindow app;
		UI::create(app);

		std::cout << "Starting window...\n";

		while (app.isRunning())
			;
	}
	catch(...)
	{

	}

	try
	{
		ComponentSystem::release();
		GameObjectManager::release();
		EditorApplication::release();
		GraphicsEngine::release();
		CameraManager::release();
		InputSystem::release();
		Random::release();
		Time::release();
	}
	catch(...)
	{
		return -1;
	}
}
