#include <iostream>
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "InputSystem.h"
#include "Random.h"
#include "Time.h"
#include "UI.h"

int main()
{
	try
	{
		Time::create();
		Random::create();
		InputSystem::create();
		CameraManager::create();
		GraphicsEngine::create();
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
		try
		{
			GraphicsEngine::release();
			CameraManager::release();
			InputSystem::release();
			Random::release();
			Time::release();
		}
		catch (...)
		{
			return -1;
		}
		
		return -1;
	}

	try
	{
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
