#pragma once
#include "ICommand.h"
#include "Vector3.h"

class GameObject;

class MoveGameObjectCommand : public ICommand
{
public:
	MoveGameObjectCommand(GameObject* gameobject, Vector3 new_position);

	void execute() override;
	void unexecute() override;

	void updateNewPosition(Vector3 new_position);
	
private:
	GameObject* m_gameobject;
	Vector3 m_old_position;
	Vector3 m_new_position;
};
