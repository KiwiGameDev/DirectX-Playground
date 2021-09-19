#include "MenuScreen.h"
#include "UI.h"
#include "EditorApplication.h"
#include "ScreenNames.h"
#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "imgui.h"
#include "imfilebrowser.h"

MenuScreen::MenuScreen(const std::string& name)
	: Screen(name), saveFileBrowser(ImGuiFileBrowserFlags_EnterNewFilename)
{
	saveFileBrowser.SetTitle("Save Scene");
	saveFileBrowser.SetTypeFilters({ ".level"});
	saveFileBrowser.SetPwd("C:/dev/saves");
	saveFileBrowser.SetInputName("scene.level");
	
	openFileBrowser.SetTitle("Open Scene");
	openFileBrowser.SetTypeFilters({ ".level" });
	openFileBrowser.SetPwd("C:/dev/saves");
}

void MenuScreen::draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				openFileBrowser.Open();
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				saveFileBrowser.Open();
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

	saveFileBrowser.Display();
	openFileBrowser.Display();

	if (saveFileBrowser.HasSelected())
	{
		EditorApplication::get().saveScene(saveFileBrowser.GetSelected().string());
		saveFileBrowser.ClearSelected();
		saveFileBrowser.Close();
	}
	else if (openFileBrowser.HasSelected())
	{
		EditorApplication::get().loadScene(openFileBrowser.GetSelected().string());
		openFileBrowser.ClearSelected();
		openFileBrowser.Close();
	}
}
