#pragma once
#include "Singleton.h"
#include "Screen.h"
#include "Window.h"
#include <vector>
#include <unordered_map>

class UI : public Singleton<UI>
{
	friend class Singleton<UI>;

public:
	static void create(const Window& window);
	static void create() = delete;

	void draw();

	Screen* getScreen(const std::string& name) const;
	
private:
	UI(HWND hwnd);
	~UI();

	std::vector<Screen*> screenList;
	std::unordered_map<std::string, Screen*> nameToScreenMap;
};
