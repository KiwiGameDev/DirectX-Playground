#include "InspectorScreen.h"

#include <iostream>

#include "GameObjectManager.h"
#include "BoxPhysicsComponent.h"
#include "GameObject.h"
#include "MoveGameObjectCommand.h"
#include "RotateGameObjectCommand.h"
#include "imgui.h"
#include "InputSystem.h"

InspectorScreen::InspectorScreen(const std::string& name)
	: Screen(name)
{
	
}

void InspectorScreen::draw()
{
	ImGui::Begin("Inspector");

	if (m_selected_gameobject != nullptr)
	{
		bool is_edit_this_frame = false;
		Vector3 position_vec = m_selected_gameobject->getPosition();
		Vector3 rotation_vec = m_selected_gameobject->getOrientationEuler();
		Vector3 scale_vec = m_selected_gameobject->getScale();
		float position[3] = { position_vec.x, position_vec.y, position_vec.z };
		float rotation[3] = { rotation_vec.x, rotation_vec.y, rotation_vec.z };
		float scale[3] = { scale_vec.x, scale_vec.y, scale_vec.z };
		
		if (ImGui::SliderFloat3("Position", position, POSITION_MIN, POSITION_MAX))
		{
			if (move_command == nullptr)
			{
				move_command = new MoveGameObjectCommand(m_selected_gameobject, Vector3(position[0], position[1], position[2]));
			}
			else
			{
				move_command->updateNewPosition(Vector3(position[0], position[1], position[2]));
			}
			
			m_selected_gameobject->setPosition(position[0], position[1], position[2]);
			is_edit_this_frame = true;
		}
		if (ImGui::SliderFloat3("Rotation", rotation, ROTATION_MIN, ROTATION_MAX))
		{
			if (rotate_command == nullptr)
			{
				rotate_command = new RotateGameObjectCommand(m_selected_gameobject, reactphysics3d::Quaternion::fromEulerAngles(rotation[0], rotation[1], rotation[2]));
			}
			else
			{
				rotate_command->updateNewOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotation[0], rotation[1], rotation[2]));
			}
			
			m_selected_gameobject->setOrientationEuler(rotation[0], rotation[1], rotation[2]);
			is_edit_this_frame = true;
		}
		if (ImGui::SliderFloat3("Scale", scale, SCALE_MIN, SCALE_MAX))
		{
			m_selected_gameobject->setScale(scale[0], scale[1], scale[2]);
			is_edit_this_frame = true;
		}

		if (is_edit_this_frame)
		{
			m_is_editing = true;
			
			Component* component = m_selected_gameobject->getComponentByType(Component::Type::Physics);

			if (BoxPhysicsComponent* physics_component = dynamic_cast<BoxPhysicsComponent*>(component))
			{
				physics_component->setAdjusted(true);
			}
		}
		
		// No longer editing
		if (m_is_editing && !is_edit_this_frame && !InputSystem::get().isLeftMouseButtonDown())
		{
			if (move_command != nullptr)
			{
				GameObjectManager::get().addAndExecuteCommand(move_command);
				move_command = nullptr;
			}
			if (rotate_command != nullptr)
			{
				GameObjectManager::get().addAndExecuteCommand(rotate_command);
				rotate_command = nullptr;
			}
			
			// TODO: Scale command

			m_is_editing = false;
		}
	}
	
	ImGui::End();
}

void InspectorScreen::setSelectedGameObject(GameObject* gameobject)
{
	m_selected_gameobject = gameobject;
}
