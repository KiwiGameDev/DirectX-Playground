#include "EditorApplication.h"

EditorApplication* Singleton<EditorApplication>::instance = nullptr;

void EditorApplication::addStateChangedEventListener(IEventCallback* callback)
{
	m_state_changed.addListener(callback);
}

void EditorApplication::removeStateChangedEventListener(IEventCallback* callback)
{
	m_state_changed.removeListener(callback);
}

void EditorApplication::setState(State state)
{
	if (m_state == state)
		return;
	
	m_state = state;
	m_state_changed.fire();
}

EditorApplication::State EditorApplication::getState() const
{
	return m_state;
}
