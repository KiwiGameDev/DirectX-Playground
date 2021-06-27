#pragma once
#include <Windows.h>

class Window
{
public:
	bool init();

	bool broadcast();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	void setHWND(HWND hwnd);

	bool isRunning();
	RECT getClientWindowRect();

	bool release();
	virtual ~Window() = default;

protected:
	HWND m_hwnd;
	bool m_is_running;
};
