#include <iostream>
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "Time.h"

int main()
{
	try
	{
		Time::create();
		GraphicsEngine::create();
		InputSystem::create();
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
	}
	catch(...)
	{
		return -1;
	}
}
