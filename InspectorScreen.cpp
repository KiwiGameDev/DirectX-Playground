#include "InspectorScreen.h"
#include "GameObject.h"
#include "imgui.h"

InspectorScreen::InspectorScreen(const std::string& name)
	: Screen(name)
{
	
}

void InspectorScreen::draw()
{
	ImGui::Begin("Inspector");

	if (m_selected_gameobject != nullptr)
	{
		Vector3 position_vec = m_selected_gameobject->getPosition();
		Vector3 rotation_vec = m_selected_gameobject->getRotation();
		Vector3 scale_vec = m_selected_gameobject->getScale();
		float position[3] = { position_vec.x, position_vec.y, position_vec.z };
		float rotation[3] = { rotation_vec.x, rotation_vec.y, rotation_vec.z };
		float scale[3] = { scale_vec.x, scale_vec.y, scale_vec.z };

		ImGui::SliderFloat3("Position", position, POSITION_MIN, POSITION_MAX);
		ImGui::SliderFloat3("Rotation", rotation, POSITION_MIN, POSITION_MAX);
		ImGui::SliderFloat3("Scale", scale, POSITION_MIN, POSITION_MAX);

		m_selected_gameobject->setPosition(position[0], position[1], position[2]);
		m_selected_gameobject->setRotation(rotation[0], rotation[1], rotation[2]);
		m_selected_gameobject->setScale(scale[0], scale[1], scale[2]);
	}
	
	ImGui::End();
}

void InspectorScreen::setSelectedGameObject(GameObject* gameobject)
{
	m_selected_gameobject = gameobject;
}
