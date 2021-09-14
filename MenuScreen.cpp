#include "MenuScreen.h"
#include "UI.h"
#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "EditorApplication.h"
#include "imgui.h"
#include "ScreenNames.h"

MenuScreen::MenuScreen(const std::string& name)
	: Screen(name)
{
	
}

void MenuScreen::draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				EditorApplication::get().saveScene();
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Game Object"))
		{
			if (ImGui::MenuItem("Create 10 Physics Cubes"))
			{
				for (int i = 0; i < 10; i++)
				{
					
				}
			}
			
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Credits"))
			{
				if (CreditsScreen* credits = dynamic_cast<CreditsScreen*>(UI::get().getScreen(ScreenNames::Credits)))
				{
					credits->show();
				}
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Color Picker"))
			{
				if (ColorPickerScreen* color_picker = dynamic_cast<ColorPickerScreen*>(UI::get().getScreen(ScreenNames::ColorPicker)))
				{
					color_picker->show();
				}
			}

			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}
}
