#pragma once
#include <Windows.h>

class Window
{
public:
	Window();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	bool isRunning();
	RECT getClientWindowRect();
	
	virtual ~Window();

protected:
	const float DEFAULT_WIDTH = 1280.0f;
	const float DEFAULT_HEIGHT = 720.0f;
	
	HWND m_hwnd;
	bool m_is_running;
	bool m_is_init = false;

private:
	bool broadcast();
};
