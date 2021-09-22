#include "InspectorScreen.h"
#include "GameObjectManager.h"
#include "InputSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "MoveGameObjectCommand.h"
#include "RotateGameObjectCommand.h"
#include "imgui.h"
#include "Rigidbody.h"

InspectorScreen::InspectorScreen(const std::string& name)
	: Screen(name)
{
	
}

void InspectorScreen::draw()
{
	ImGui::Begin("Inspector");

	if (m_selected_gameobject != nullptr && m_selected_gameobject->hasComponent<Transform>())
	{
		bool is_edit_this_frame = false;
		Transform& transform = m_selected_gameobject->getComponent<Transform>();
		Vector3 position_vec = transform.getPosition();
		Vector3 rotation_vec = transform.getOrientationEuler();
		Vector3 scale_vec = transform.getScale();
		float position[3] = { position_vec.x, position_vec.y, position_vec.z };
		float rotation[3] = { rotation_vec.x, rotation_vec.y, rotation_vec.z };
		float scale[3] = { scale_vec.x, scale_vec.y, scale_vec.z };
		
		if (ImGui::DragFloat3("Position", position))
		{
			if (move_command == nullptr)
			{
				move_command = new MoveGameObjectCommand(m_selected_gameobject, Vector3(position[0], position[1], position[2]));
			}
			else
			{
				move_command->updateNewPosition(Vector3(position[0], position[1], position[2]));
			}
			
			transform.setPosition(position[0], position[1], position[2]);
			is_edit_this_frame = true;
		}
		if (ImGui::DragFloat3("Rotation", rotation))
		{
			if (rotate_command == nullptr)
			{
				rotate_command = new RotateGameObjectCommand(m_selected_gameobject, reactphysics3d::Quaternion::fromEulerAngles(rotation[0], rotation[1], rotation[2]));
			}
			else
			{
				rotate_command->updateNewOrientation(reactphysics3d::Quaternion::fromEulerAngles(rotation[0], rotation[1], rotation[2]));
			}
			
			transform.setOrientationEuler(rotation[0], rotation[1], rotation[2]);
			is_edit_this_frame = true;
		}
		if (ImGui::DragFloat3("Scale", scale))
		{
			transform.setScale(scale[0], scale[1], scale[2]);
			is_edit_this_frame = true;
		}

		if (is_edit_this_frame)
		{
			m_is_editing = true;
			
			if (m_selected_gameobject->hasComponent<Rigidbody>())
			{
				m_selected_gameobject->getComponent<Rigidbody>().setAdjusted(true);
			}
		}
		
		// No longer editing
		if (m_is_editing && !is_edit_this_frame && !InputSystem::get().isLeftMouseButtonDown())
		{
			m_is_editing = false;
			
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
		}
	}
	
	ImGui::End();
}

void InspectorScreen::setSelectedGameObject(GameObject* gameobject)
{
	m_selected_gameobject = gameobject;
}
