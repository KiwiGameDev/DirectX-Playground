#pragma once
#include "ICommand.h"
#include <reactphysics3d/mathematics/Quaternion.h>

class GameObject;

class RotateGameObjectCommand : public ICommand
{
public:
	RotateGameObjectCommand(GameObject* gameobject, reactphysics3d::Quaternion new_orientation);

	void execute() override;
	void unexecute() override;

	void updateNewOrientation(reactphysics3d::Quaternion new_orientation);

private:
	GameObject* m_gameobject;
	reactphysics3d::Quaternion m_old_orientation;
	reactphysics3d::Quaternion m_new_orientation;
};