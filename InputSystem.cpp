#include "InputSystem.h"
#include "IInputListener.h"
#include <Windows.h>

void InputSystem::update()
{
	POINT current_mouse_pos_temp;
	GetCursorPos(&current_mouse_pos_temp);
	Point current_mouse_pos(current_mouse_pos_temp.x, current_mouse_pos_temp.y);

	if (m_first_time)
	{
		m_old_mouse_pos = current_mouse_pos;
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.x || current_mouse_pos.y != m_old_mouse_pos.y)
	{
		for (auto input_listener : m_listeners)
		{
			Point point(current_mouse_pos.x - m_old_mouse_pos.x, current_mouse_pos.y - m_old_mouse_pos.y);
			input_listener->onMouseMove(point);
		}
	}

	m_old_mouse_pos = current_mouse_pos;
	
	if (!GetKeyboardState(m_keys_state))
		return;

	for (int i = 0; i < 256; i++)
	{
		if (m_keys_state[i] & 0x80)
		{
			for (auto input_listener : m_listeners)
			{
				// Mouse
				if (i == VK_LBUTTON)
				{
					if (m_keys_state[i] != m_old_keys_state[i])
					{
						input_listener->onLeftMouseDown(current_mouse_pos);
					}
				}
				else if (i == VK_RBUTTON)
				{
					if (m_keys_state[i] != m_old_keys_state[i])
					{
						input_listener->onRightMouseDown(current_mouse_pos);
					}
				}
				else // Keyboard
				{
					input_listener->onKeyDown(i);
				}
			}
		}
		else
		{
			if (m_keys_state[i] == m_old_keys_state[i])
				continue;

			for (auto input_listener : m_listeners)
			{
				if (i == VK_LBUTTON)
				{
					input_listener->onLeftMouseUp(current_mouse_pos);
				}
				else if (i == VK_RBUTTON)
				{
					input_listener->onRightMouseUp(current_mouse_pos);
				}
				else
				{
					input_listener->onKeyUp(i);
				}
			}
		}
	}

	memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
}

void InputSystem::addListener(IInputListener* input_listener)
{
	m_listeners.insert(input_listener);
}

void InputSystem::removeListener(IInputListener* input_listener)
{
	m_listeners.erase(input_listener);
}
