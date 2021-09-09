#pragma once
#include "EventCallback.h"
#include <vector>

class Event
{
public:
	void addListener(IEventCallback* callback);
	void removeListener(IEventCallback* callback);
	void fire();

	~Event();

private:
	std::vector<IEventCallback*> callbacks;
};
