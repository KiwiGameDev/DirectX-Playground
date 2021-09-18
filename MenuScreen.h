#pragma once
#include "Screen.h"
#include "imgui.h"
#include "imfilebrowser.h"

class MenuScreen : public Screen
{
public:
	MenuScreen(const std::string& name);

	void draw() override;

private:
	ImGui::FileBrowser saveFileBrowser;
	ImGui::FileBrowser openFileBrowser;
};
