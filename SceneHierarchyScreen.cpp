#include "SceneHierarchyScreen.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "InspectorScreen.h"
#include "imgui.h"

SceneHierarchyScreen::SceneHierarchyScreen(InspectorScreen* inspector_screen, const std::string& name)
	: Screen(name)
{
	m_inspector_screen = inspector_screen;
}

void SceneHierarchyScreen::draw()
{
	ImGui::Begin("Hierarchy");

	for (auto name_gameobject_pair : GameObjectManager::get().getGameObjectMap())
	{
		if (ImGui::Button(name_gameobject_pair.second->getName().c_str()))
		{
			m_inspector_screen->setSelectedGameObject(name_gameobject_pair.second);
		}
	}
	
	ImGui::End();
}
