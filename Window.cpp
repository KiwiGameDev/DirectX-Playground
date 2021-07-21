#include "Window.h"
#include <string>
#include <exception>

#include "Time.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		
		break;
	}
	case WM_SETFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
			window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);	
	}

	return NULL;
}

Window::Window()
{
	WNDCLASSEX wc = {};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc))
		throw std::exception("Window failed to initialize successfully");

	m_hwnd = ::CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		L"MyWindowClass",
		L"DirectX Application",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024,
		768,
		NULL,
		NULL,
		NULL,
		NULL);

	if (!m_hwnd)
		throw std::exception("Window failed to initialize successfully");

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_is_running = true;
}

bool Window::broadcast()
{
	Time::get().logStart();
	
	if (!m_is_init)
	{
		m_is_init = true;
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		onCreate();
	}
	
	MSG msg;
	
	onUpdate();

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(0);

	Time::get().logEnd();
	
	return true;
}

void Window::onCreate()
{
	
}

void Window::onUpdate()
{
	
}

void Window::onDestroy()
{
	m_is_running = false;
}

void Window::onFocus()
{
	
}

void Window::onKillFocus()
{
	
}

bool Window::isRunning()
{
	if (m_is_running)
		broadcast();
	
	return m_is_running;
}

RECT Window::getClientWindowRect()
{
	RECT rect;
	::GetClientRect(m_hwnd, &rect);
	return rect;
}

Window::~Window()
{
	if (!::DestroyWindow(m_hwnd))
		throw std::exception("Window failed to delete successfully");
}
