#include <iostream>
#include "AppWindow.h"
#include "CameraManager.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "Time.h"
#include "Random.h"

int main()
{
	try
	{
		Random::create();
		Time::create();
		GraphicsEngine::create();
		InputSystem::create();
		CameraManager::create();
	}
	catch(...)
	{
		return -1;
	}

	try
	{
		AppWindow app;

		std::cout << "Starting window...\n";

		while (app.isRunning())
			;
	}
	catch(...)
	{
		try
		{
			InputSystem::release();
			GraphicsEngine::release();
			Time::release();
			Random::release();
			CameraManager::release();
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
		InputSystem::release();
		Time::release();
		Random::release();
		CameraManager::release();
	}
	catch(...)
	{
		return -1;
	}
}
