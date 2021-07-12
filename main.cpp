#include <iostream>
#include "AppWindow.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"

int main()
{
	try
	{
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
			GraphicsEngine::release();
			InputSystem::release();
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
	}
	catch(...)
	{
		return -1;
	}
}
