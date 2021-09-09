#include "PlayMenuScreen.h"
#include "EditorApplication.h"
#include "imgui.h"

PlayMenuScreen::PlayMenuScreen(const std::string& name)
	: Screen(name)
{
	
}

void PlayMenuScreen::draw()
{
	ImGui::Begin("Play Menu");
	if (EditorApplication::get().getState() == EditorApplication::State::Stop)
	{
		if (ImGui::Button("Play"))
		{
			EditorApplication::get().setState(EditorApplication::State::Play);
		}
	}
	else if (EditorApplication::get().getState() == EditorApplication::State::Play)
	{
		if (ImGui::Button("Pause"))
		{
			EditorApplication::get().setState(EditorApplication::State::Pause);
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Stop"))
		{
			EditorApplication::get().setState(EditorApplication::State::Stop);
		}
	}
	else if (EditorApplication::get().getState() == EditorApplication::State::Pause)
	{
		if (ImGui::Button("Resume"))
		{
			EditorApplication::get().setState(EditorApplication::State::Play);
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Step"))
		{
			EditorApplication::get().setState(EditorApplication::State::Step);
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Stop"))
		{
			EditorApplication::get().setState(EditorApplication::State::Stop);
		}
	}
	ImGui::End();
}
