#pragma once
#include <string>

#include "Singleton.h"
#include "Event.h"

class EditorApplication : public Singleton<EditorApplication>
{
	friend Singleton<EditorApplication>;

public:
	enum class State
	{
		Play,
		Pause,
		Step,
		Stop
	};

	void saveScene();
	void loadScene();

	void addStateChangedEventListener(IEventCallback* callback);
	void removeStateChangedEventListener(IEventCallback* callback);
	
	void setState(State state);

	State getState() const;

private:
	Event m_state_changed;
	
	State m_state = State::Stop;
};
