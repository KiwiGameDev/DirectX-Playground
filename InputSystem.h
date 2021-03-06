#pragma once
#include "Singleton.h"
#include "Point.h"
#include <unordered_set>

class IInputListener;

class InputSystem : public Singleton<InputSystem>
{
	friend class Singleton<InputSystem>;
	
public:
	void update();

	void addListener(IInputListener* input_listener);
	void removeListener(IInputListener* input_listener);

	void showCursor(bool visible);

	void setCursorPosition(const Point& pos);

	bool isKeyDown(int key);
	bool isLeftMouseButtonDown();

private:
	std::unordered_set<IInputListener*> m_listeners;

	Point m_old_mouse_pos;
	unsigned char m_keys_state[256] = { };
	unsigned char m_old_keys_state[256] = { };
	bool m_first_time = true;
};
