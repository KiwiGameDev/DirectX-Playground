#pragma once
#include "Screen.h"

class GameObject;

class InspectorScreen : public Screen
{
public:
	InspectorScreen(const std::string& name);

	void draw() override;

	void setSelectedGameObject(GameObject* gameobject);

private:
	const float POSITION_MIN = -10.0f;
	const float POSITION_MAX = 10.0f;
	const float ROTATION_MIN = -3.14f;
	const float ROTATION_MAX = 3.14f;
	const float SCALE_MIN = -10.0f;
	const float SCALE_MAX = 10.0f;
	
	GameObject* m_selected_gameobject = nullptr;
};
