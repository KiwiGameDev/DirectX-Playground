#include "MenuScreen.h"
#include "UI.h"
#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
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
			if (ImGui::MenuItem("Open..", "Ctrl+O"))
			{
				
			}
			
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				
			}
			
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Game Object"))
		{
			if (ImGui::MenuItem("Create Cube"))
			{
				
			}
			
			if (ImGui::MenuItem("Create Sphere"))
			{
				
			}
			
			if (ImGui::MenuItem("Create Plane"))
			{
				
			}
			
			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Point Light"))
				{
					
				}
				
				ImGui::EndMenu();
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
