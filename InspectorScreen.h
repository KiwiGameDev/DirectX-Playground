#pragma once
#include "Screen.h"

class RotateGameObjectCommand;
class MoveGameObjectCommand;
class GameObject;

class InspectorScreen : public Screen
{
public:
	InspectorScreen(const std::string& name);

	void draw() override;

	void setSelectedGameObject(GameObject* gameobject);

private:
	GameObject* m_selected_gameobject = nullptr;

	bool m_is_editing = false;
	MoveGameObjectCommand* move_command = nullptr;
	RotateGameObjectCommand* rotate_command = nullptr;
};
