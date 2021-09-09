#include "Event.h"

void Event::addListener(IEventCallback* callback)
{
	auto itr = std::find(callbacks.begin(), callbacks.end(), callback);

	if (itr != callbacks.end())
		return;
	
	callbacks.push_back(callback);
}

void Event::removeListener(IEventCallback* callback)
{
	auto itr = std::find(callbacks.begin(), callbacks.end(), callback);

	delete callback;
	
	if (itr == callbacks.end())
		return;
	
	delete *itr;
	callbacks.erase(itr);
}

void Event::fire()
{
	for (IEventCallback* callback : callbacks)
	{
		(*callback)();
	}
}

Event::~Event()
{
	for (IEventCallback* callback : callbacks)
	{
		delete callback;
	}
}
