#include "RotateGameObjectCommand.h"
#include "GameObject.h"
#include "Transform.h"

RotateGameObjectCommand::RotateGameObjectCommand(GameObject* gameobject, reactphysics3d::Quaternion new_orientation)
	: m_gameobject(gameobject), m_old_orientation(gameobject->getComponent<Transform>().getOrientation()), m_new_orientation(new_orientation)
{
	
}

void RotateGameObjectCommand::execute()
{
	m_gameobject->getComponent<Transform>().setOrientation(m_new_orientation);
}

void RotateGameObjectCommand::unexecute()
{
	m_gameobject->getComponent<Transform>().setOrientation(m_old_orientation);
}

void RotateGameObjectCommand::updateNewOrientation(reactphysics3d::Quaternion new_orientation)
{
	m_new_orientation = new_orientation;
}
