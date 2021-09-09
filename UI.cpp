#include "UI.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "ScreenNames.h"
#include "MenuScreen.h"
#include "ViewportScreen.h"
#include "CreditsScreen.h"
#include "ColorPickerScreen.h"
#include "InspectorScreen.h"
#include "SceneHierarchyScreen.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "PlayMenuScreen.h"

UI* Singleton<UI>::instance = nullptr;

UI::UI(HWND hwnd)
{
	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	RenderSystem& render_system = GraphicsEngine::get().getRenderSystem();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(render_system.m_d3d_device, render_system.m_imm_device_context->m_device_context);

	ImGui::StyleColorsDark();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ViewportScreen* viewport = new ViewportScreen(ScreenNames::Viewport);
	MenuScreen* menu = new MenuScreen(ScreenNames::Menu);
	CreditsScreen* credits = new CreditsScreen(ScreenNames::Credits);
	ColorPickerScreen* color_picker = new ColorPickerScreen(ScreenNames::ColorPicker);
	InspectorScreen* inspector = new InspectorScreen(ScreenNames::Inspector);
	SceneHierarchyScreen* scene_hierarchy = new SceneHierarchyScreen(inspector, ScreenNames::SceneHierarchy);
	PlayMenuScreen* play_menu = new PlayMenuScreen(ScreenNames::PlayMenu);
	
	screenList.push_back(viewport);
	screenList.push_back(menu);
	screenList.push_back(credits);
	screenList.push_back(color_picker);
	screenList.push_back(inspector);
	screenList.push_back(scene_hierarchy);
	screenList.push_back(play_menu);

	nameToScreenMap.insert({ viewport->getName(), viewport });
	nameToScreenMap.insert({ menu->getName(), menu });
	nameToScreenMap.insert({ credits->getName(), credits });
	nameToScreenMap.insert({ color_picker->getName(), color_picker });
	nameToScreenMap.insert({ inspector->getName(), inspector });
	nameToScreenMap.insert({ scene_hierarchy->getName(), scene_hierarchy });
	nameToScreenMap.insert({ play_menu->getName(), play_menu });
}

void UI::create(const Window& window)
{
	if (instance)
		throw std::exception("Singleton has already been created!");

	instance = new UI(window.m_hwnd);
}

void UI::draw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (Screen* const screen : screenList)
	{
		screen->draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

Screen* UI::getScreen(const std::string& name) const
{
	return nameToScreenMap.at(name);
}

UI::~UI()
{
	
}
