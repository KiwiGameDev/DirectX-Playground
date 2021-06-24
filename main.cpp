#include <iostream>
#include "AppWindow.h"

int main()
{
	AppWindow app;

	if(!app.init())
	{
		std::cerr << "Failed to initialize window!\n";
		return -1;
	}

	std::cout << "Starting window...\n";

	while (app.isRunning())
	{
		app.broadcast();
	}
}
