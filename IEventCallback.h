#pragma once

class IEventCallback
{
public:
	virtual void operator()() = 0;
	virtual bool operator==(IEventCallback* other) = 0;

	virtual ~IEventCallback() { }
};
