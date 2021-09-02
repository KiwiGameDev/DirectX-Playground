#include "InspectorScreen.h"

#include "BoxPhysicsComponent.h"
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
		bool did_edit = false;
		Vector3 position_vec = m_selected_gameobject->getPosition();
		Vector3 rotation_vec = m_selected_gameobject->getOrientationEuler();
		Vector3 scale_vec = m_selected_gameobject->getScale();
		float position[3] = { position_vec.x, position_vec.y, position_vec.z };
		float rotation[3] = { rotation_vec.x, rotation_vec.y, rotation_vec.z };
		float scale[3] = { scale_vec.x, scale_vec.y, scale_vec.z };

		ImGui::SliderFloat3("Position", position, POSITION_MIN, POSITION_MAX);
		ImGui::SliderFloat3("Rotation", rotation, ROTATION_MIN, ROTATION_MAX);
		ImGui::SliderFloat3("Scale", scale, SCALE_MIN, SCALE_MAX);

		if (position_vec.x != position[0] || position_vec.y != position[1] || position_vec.z != position[2])
		{
			m_selected_gameobject->setPosition(position[0], position[1], position[2]);
			did_edit = true;
		}
		if (rotation_vec.x != rotation[0] || rotation_vec.y != rotation[1] || rotation_vec.z != rotation[2])
		{
			m_selected_gameobject->setOrientationEuler(rotation[0], rotation[1], rotation[2]);
			did_edit = true;
		}
		if (scale_vec.x != scale[0] || scale_vec.y != scale[1] || scale_vec.z != scale[2])
		{
			m_selected_gameobject->setScale(scale[0], scale[1], scale[2]);
			did_edit = true;
		}

		if (did_edit)
		{
			Component* component = m_selected_gameobject->getComponentByType(Component::Type::Physics);

			if (BoxPhysicsComponent* physics_component = dynamic_cast<BoxPhysicsComponent*>(component))
			{
				physics_component->setAdjusted(true);
			}
		}
	}
	
	ImGui::End();
}

void InspectorScreen::setSelectedGameObject(GameObject* gameobject)
{
	m_selected_gameobject = gameobject;
}
