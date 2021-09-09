#pragma once
#include "IEventCallback.h"

template<typename T>
class EventCallback : public IEventCallback
{
public:
	EventCallback(T* instance, void(T::*function)())
		: instance(instance), function(function) { }

	void operator()() override
	{
		(instance->*function)();
	}
	
	bool operator==(IEventCallback* other) override
	{
		EventCallback* otherEventCallback = dynamic_cast<EventCallback*>(other);

		if (otherEventCallback == nullptr)
			return false;

		return function == otherEventCallback->function && instance == otherEventCallback->instance;
	}

private:
	T* instance;
	void(T::* function)();
};
